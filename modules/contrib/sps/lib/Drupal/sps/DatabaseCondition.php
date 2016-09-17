<?php

namespace Drupal\sps;

class DatabaseCondition extends \DatabaseCondition {
  /**
   * Implements QueryConditionInterface::compile().
   */
  public function compile(\DatabaseConnection $connection, \QueryPlaceholderInterface $queryPlaceholder) {

    // Re-compile if this condition changed or if we are compiled against a
    // different query placeholder object.
    if ($this->changed || isset($this->queryPlaceholderIdentifier) && ($this->queryPlaceholderIdentifier != $queryPlaceholder->uniqueIdentifier())) {
      $this->queryPlaceholderIdentifier = $queryPlaceholder->uniqueIdentifier();

      $condition_fragments = array();
      $arguments = array();

      $conditions = $this->conditions;
      $conjunction = $conditions['#conjunction'];
      unset($conditions['#conjunction']);
      foreach ($conditions as $condition) {
        if (empty($condition['operator'])) {
          // This condition is a literal string, so let it through as is.
          $condition_fragments[] = ' (' . $condition['field'] . ') ';
          $arguments += $condition['value'];
        }
        else {
          // It's a structured condition, so parse it out accordingly. Note that
          // $condition['field'] will only be an object for a dependent
          // DatabaseCondition object, not for a dependent subquery.
          if ($condition['field'] instanceof \QueryConditionInterface) {
            // Compile the sub-condition recursively and add it to the list.
            $condition['field']->compile($connection, $queryPlaceholder);
            $condition_fragments[] = '(' . (string) $condition['field'] . ')';
            $arguments += $condition['field']->arguments();
          }
          else {
            // For simplicity,we treat all operators as the same data structure.
            // In the typical degenerate case, this won't get changed.
            $operator_defaults = array(
              'prefix' => '',
              'postfix' => '',
              'delimiter' => '',
              'operator' => $condition['operator'],
              'use_value' => TRUE,
            );
            $operator = $connection->mapConditionOperator($condition['operator']);
            if (!isset($operator)) {
              $operator = $this->mapConditionOperator($condition['operator']);
            }
            $operator += $operator_defaults;

            $placeholders = array();
            if ($condition['value'] instanceof \SelectQueryInterface) {
              $condition['value']->compile($connection, $queryPlaceholder);
              $placeholders[] = (string) $condition['value'];
              $arguments += $condition['value']->arguments();
              // Subqueries are the actual value of the operator, we don't
              // need to add another below.
              $operator['use_value'] = FALSE;
            }
            // We assume that if there is a delimiter, then the value is an
            // array. If not, it is a scalar. For simplicity, we first convert
            // up to an array so that we can build the placeholders in the same
            // way.
            elseif (!$operator['delimiter']) {
              $condition['value'] = array($condition['value']);
            }
            if ($operator['use_value']) {
              foreach ($condition['value'] as $value) {
                $placeholder = ':db_condition_placeholder_' . $queryPlaceholder->nextPlaceholder();
                $arguments[$placeholder] = $value;
                $placeholders[] = $placeholder;
              }
            }
            $condition_fragments[] = ' (' . $condition['field'] . ' ' . $operator['operator'] . ' ' . $operator['prefix'] . implode($operator['delimiter'], $placeholders) . $operator['postfix'] . ') ';
          }
        }
      }

      $this->changed = FALSE;
      $this->stringVersion = implode($conjunction, $condition_fragments);
      $this->arguments = $arguments;
    }
  }
}
