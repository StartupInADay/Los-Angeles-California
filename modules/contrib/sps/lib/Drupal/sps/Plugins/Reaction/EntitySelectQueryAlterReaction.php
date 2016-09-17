<?php
namespace Drupal\sps\Plugins\Reaction;

class EntitySelectQueryAlterReaction implements \Drupal\sps\Plugins\ReactionInterface {
  protected $entities = array();
  protected $alias = array();

  /**
   * The EntitySelectQueryAlterReaction can work with any number of Entities.
   * Each entity must be shown in the $config['entities'] array.  It will
   * then alter any query (it can touch) and use the override revision id
   * instead of the one in base
   *
   * @param $config
   *   has one entry entities which list info about each entity for which
   *   query should be altered
   *   entities = array(
   *     array(
   *       'base_table' =>  'node', //the base table of the entity
   *       'revision_table' => 'node_revision', //The table for the revision
   *       'revision_fields' => array('uid', 'sticky', 'promote', 'status'),
   *       // The fields that are in both base and entity that should pull
   *       // from revision if we are altering the query
   *       'base_id' => 'nid',  // the base table id
   *       'revision_id => 'vid'// the revision table's id in the base table
   *     )
   *   )
   * @param $manager \Drupal\sps\Manager
   *                 the Current Manager Object
   *
   * @return \Drupal\sps\Plugins\Reaction\EntitySelectQueryAlterReaction
   */
  public function __construct(array $config, \Drupal\sps\Manager $manager) {
    $this->entities = $config['entities'];
  }

  /**
   * Alters a fields array change the table in which fields should be pulled
   *
   * For each entity, all fields in revision_field have the table change to the
   * revision table
   *
   * For each entity the entity_id field's table is changed to the overrides table
   *
   * @param $fields
   *   a field array from a SelectQuery object
   *
   * @param Array $alias
   *   a map of the queries alias to table
   *
   * @return \Drupal\sps\Plugins\Reaction\EntitySelectQueryAlterReaction
   *  Self
   */
  protected function fieldReplace(&$fields, $alias) {
    foreach($fields as &$field) {
      foreach($this->entities as $entity) {
        if(isset($alias[$entity['base_table']])) {
          //check for revision fields in base
          if (
            isset($alias[$entity['revision_table']]) &&
            ($field['table'] == $alias[$entity['base_table']]) &&
            (in_array($field['field'], $entity['revision_fields']))
            ){
            $field['table'] = $alias[$entity['revision_table']];
          }

          //Check for vid in base
          if (
            ($field['table'] == $alias[$entity['base_table']]) &&
            ($field['field'] == $entity['revision_id'])
             && FALSE
            ){
            $field['table'] = $this->getOverrideAlias($entity);
          }
        }
      }
    }

    return $this;
  }

  /**
   * construct on override table alias
   *
   * @param $entity
   *   an array representing an alias
   *
   * @see EntitySelectQueryAlterReaction::__construct()
   *
   * @return string
   *    an alias for the overrides table
   */
  protected function getOverrideAlias($entity) {
    return $entity['base_table'] . "_overrides";
  }

  /**
   * add override tables to a query
   *
   * This is mostly a passthrough to the override_controller
   *
   * @param $query
   *   The query to alter
   * @param $override_controller
   *   The Override controller that will provide the override tables;
   *
   * @return \Drupal\sps\Plugins\Reaction\EntitySelectQueryAlterReaction
   *  Self
   */
  protected function addOverrideTable($query, $override_controller) {
    $alias = $this->extractAlias($query);
    foreach($this->entities as $entity) {
      if(isset($alias[$entity['base_table']])) {
        $base_alias = $alias[$entity['base_table']];
        $base_id = $entity['base_id'];
        $overrides_alias = $this->getOverrideAlias($entity);
        if($base_alias) {
          $type = $entity['base_table'];
          $override_controller->addOverrideJoin($query, $base_alias, $base_id, $overrides_alias, $type);
          $t = $query->getTables();
        }
      }
    }

    return $this;
  }

  /**
   *
   * Add Revision tables to a query if we have the base table but no revision table
   *
   * This has to be add so that if we are showing a different revsion field
   * can be pulled from the correct revision
   *
   * @param $query
   *   the query to alter
   *
   * @return \Drupal\sps\Plugins\Reaction\EntitySelectQueryAlterReaction
   *  Self
   */
  protected function addRevisionTables($query) {
    $alias = $this->extractAlias($query);
    $tables = $query->getTables();
    $aliases = array();
    foreach($this->entities as $entity) {
      if(isset($alias[$entity['base_table']]) && !isset($alias[$entity['revision_table']])) {
        $query->join($entity['revision_table'], 'override_' . $entity['revision_table'], "{$alias[$entity['base_table']]}.{$entity['revision_id']} = override_{$entity['revision_table']}.{$entity['revision_id']}");
      }
    }

    return $this;
  }


  /**
   * @param       $datum
   * @param       $alias
   * @param array $override_property_map
   *
   * @return mixed
   */
  protected function replaceDatum($datum, $alias, $override_property_map = array()) {
    foreach($this->entities as $entity) {
      if(isset($alias[$entity['base_table']])) {
        //replace revision_id
        if(isset($override_property_map['revision_id'])) {
          $datum = preg_replace("/\b(".$alias[$entity['base_table']]."\.{$entity['revision_id']})/", "COALESCE(". $this->getOverrideAlias($entity) ."." .$override_property_map['revision_id'] .", $1)", $datum);
        }


        //filter the override_property_map to only inlcude items that in revision fields
        $coalesce_revision_map = array_intersect_key($override_property_map, array_fill_keys($entity['revision_fields'], TRUE));

        //we have a revision table so lets set values to use that table
        if(isset($alias[$entity['revision_table']])) {
            $datum = $this->replaceTableSwitch($datum, $entity['revision_fields'],$alias[$entity['base_table']], $alias[$entity['revision_table']]);
          // we have propties to override lets give the override table priority
          // on those fields
          if(!empty($coalesce_revision_map)) {
            $datum = $this->replaceCoalesce($datum, $coalesce_revision_map,$alias[$entity['revision_table']],  $this->getOverrideAlias($entity));
          }
        }
        else {
          // we have propties to override lets give the override table priority
          // on those fields
          if(!empty($coalesce_revision_map)) {
            $datum = $this->replaceCoalesce($datum, $coalesce_revision_map,$alias[$entity['base_table']],  $this->getOverrideAlias($entity));
          }
        }
      }
    }
    return $datum;
  }

  /**
   * Recursivily cycle through data replacing base.vid with revision.vid
   *
   * This function finds any ref to the base table vid fields and replace it
   * with a reference to the override vid field.
   *
   * also if there is a revision table it looks for base table version of title and status
   * and changes them to the revision table
   *
   * @param $data
   *   This is data form a query
   *
   * @param $alias
   *
   * @param array $override_property_map
   *
   * @return  \Drupal\sps\Plugins\Reaction\EntitySelectQueryAlterReaction
   *  Self
   */
  protected function recursiveReplace(&$data, $alias, $override_property_map = array()) {
    $reorder = array();
    foreach($data as $key => &$datum) {

      //check to see if we need to rewrite the keys.
          //$new_key = preg_replace("/".$alias[$entity['base_table']]."\.(".implode("|", $entity['revision_fields']).")/", $alias[$entity['revision_table']] .'.$1', $key);
          $new_key = $this->replaceDatum($key, $alias, $override_property_map);
          if($new_key != $key) {
            $reorder[$key] = $new_key;
      }

      //if an array lets run the kids
      if (is_array($datum)) {
        $this->recursiveReplace($datum, $alias, $override_property_map);
      }
      //@TODO this is for exceptions basicly
      elseif (is_object($datum)) {
        if (is_a($datum, 'DatabaseCondition')) {
        //if ($this->checkInterface($datum, 'QueryConditionInterface')) {
          $this->upgradeCondition($datum);
          $sub_condition =& $datum->conditions();

          $this->recursiveReplace($sub_condition, $alias, $override_property_map);
        }
      }
      //ok we have a single datum lets work on it.
      else {
        if($datum !== NULL) {
          $datum = $this->replaceDatum($datum, $alias, $override_property_map);
        }
      }
    }
    if($reorder) {
      $data = $this->keyRename($data, $reorder);
    }

    return $this;
  }

  /**
   * This alter the $datum replacing the base_alias with the revision_alias when we have fields
   *
   * @param $datum
   *   the piece of content to alter
   * @param $fields
   *   which fields should have there table moved
   * @param $base_alias
   *   what is the table we are moving from
   * @param $revision_alias
   *   what is the table we are moving to
   *
   * @return mixed
   */
  protected function replaceTableSwitch($datum, $fields, $base_alias, $revision_alias) {
    return preg_replace("/".$base_alias."\.(".implode("|", $fields).")/", $revision_alias.'.$1', $datum);
  }


  /**
   * This returns the datum alter as a coalesce of the base field and its partner field in the
   * override tables
   *
   * @param $datum
   *   the piece of content to alter
   * @param $fields_map
   *   the mapping of field names to the same field in the override table
   * @param $base_alias
   *   the alias of the base table
   * @param $override_alias
   *   the alias of the override table
   *
   * @return mixed
   */
  protected function replaceCoalesce($datum, $fields_map, $base_alias, $override_alias) {
    return preg_replace_callback(
      "/".$base_alias."\.(".implode("|", array_keys($fields_map)).")/",
      function ($m) use ($override_alias, $base_alias, $fields_map) {
        return 'COALESCE('.$override_alias .'.'. $fields_map[$m[1]]  .', '. $base_alias.'.'.$m[1] .')';
      },
      $datum
    );
  }

  /**
   * helper function to rename keys in an assoc array but keep the order
   *
   * @param $data
   *   array
   * @param $moves
   *   array of old key => new key items
   *
   * @return array
   */
  protected function keyRename($data, $moves) {
    $new_data = array();
    foreach($data as $key => $datum) {
      if (isset($moves[$key])) {
        $new_data[$moves[$key]] = $datum;
      }
      else {
        $new_data[$key] = $datum;
      }
    }
    return $new_data;
  }

  /**
   * Check a query for tables that match our entities tables
   * and return the alais for everyone that was found
   *
   * @param $query \SelectQueryInterface
   *   a SelectQuery is expected
   *
   * @return array
   *   an array whose keys are table names and values are alias
   */
  protected function extractAlias(\SelectQueryInterface $query) {
    $tables = $query->getTables();
    $aliases = array();
    foreach($tables as $alias => $table) {
      foreach($this->entities as $entity) {
        if ($table['table'] == $entity['base_table']) {
          $aliases[$entity['base_table']] = $alias;
        }
        if ($table['table'] == $entity['revision_table']) {
          $aliases[$entity['revision_table']] = $alias;
        }
      }
    }
    return $aliases;
  }

  /**
   * Move Items in a field for a query to a expression
   *
   * As we are going to use COALECSE on fields we are transfering them form
   * fields into expressions.
   *
   * We make sure that we run our version of escapeField and escapeTable
   * to insure we are not allowing through fields and table that would have been
   * stopped if we were not transforming the field into an expression.
   *
   * It is worth noting that this is only happening on fields and tables that
   * are identified by sps and its plugins.
   *
   * @param $query
   *   the query to manipulate
   * @param $fields_to_move
   *   name of fields to move
   *
   * @param $alias
   *
   * @return \Drupal\sps\Plugins\Reaction\EntitySelectQueryAlterReaction
   *  Self
   */
  protected function fieldsToExpressions(&$query, $fields_to_move, $alias) {
    $fields = & $query->getFields();
    foreach($this->entities as $entity) {
      if(isset($alias[$entity['base_table']])) {
        $fields_to_move_entity = $fields_to_move;
        if(in_array('revision_id', $fields_to_move)) {
          $fields_to_move_entity[array_search('revision_id', $fields_to_move_entity)] = $entity['revision_id'];
        }
        foreach ($fields as $field_alias => $field) {
          if (in_array($field['field'], $fields_to_move_entity) &&
              ($field['table'] == $alias[$entity['base_table']])) {

            //We are turning this into an expression so we need to do the escape that would be
            //use if it was a field on the table and field
            $query->addExpression($field['table'] .'.'. $this->escapeField($field['field']), $field_alias);
            unset($fields[$field_alias]);
          }
        }
      }
    }
    return $this;
  }

  /**
  * Replace a \DatabaseCondition with a \Drupal\sps\DatabaseCondition
  *
  * The standard DatabaseCondition calls escapeField on the field element in
  * compile.  As we need to add COALESCE to the field, the '(),' chars will all
  * get striped cause silent failure.  So we are replacing the
  * DatabaseCondition with our own that does not call escapeField in compile.
  *
  * To insure that the fields that we are getting are proper before we change
  * the condition type, we are running our own version of escapeField on the
  * field property.
  *
  * @param $condition
  *   the \DatabaseCondition to replace
  *
  * @return NULL
  */
  protected function upgradeCondition(&$condition) {
    $sub_condition =& $condition->conditions();
    foreach($sub_condition as $key => $item) {
      if(!isset($item['operator']) && isset($item['field'])) {
        $item['field'] = $this->escapeField($item['field']);
      }
    }
    $replacement = new \Drupal\sps\DatabaseCondition($sub_condition['#conjunction']);
    $replacement_c = &$replacement->conditions();
    $replacement_c = $sub_condition;
    $condition = $replacement;
  }

  /**
   * This function takes an array from a DatabaseCondition::conditions() call
   * and use that info to create a new DatabaseCondition. it then replace the
   * original array with a single item that is the new DatabaseCondition.
   *
   * This is used to move conditions out of the base where and having parts of
   * a query, so that the class of the DatabaseCondition can  be changed
   *
   * @param $condition_info array
   *   an array from a DatabaseCondition::conditions()
   *
   * @return void NULL
   */
  protected function encapCondition(&$condition_info) {
    if(isset($condition_info[0])) {
      $where_info = $condition_info;
      $replacement = db_and();
      $relacement_info = &$replacement->conditions();
      $relacement_info = $where_info;
      $condition_info = array(
        '#conjunction' => 'AND',
        0 => array(
          'field' => $replacement,
          'operator' => '=',
          'value' => array(),
        )
      );
    }
  }
 /**
  * copy of DatabaseConnection::escapeField
  * @see DatabaseConnection::escapeField
  *
  */
  protected function escapeField($field) {
    return preg_replace('/[^A-Za-z0-9_.]+/', '', $field);
  }

 /**
  * copy of DatabaseConnection::escapeTable
  * @see DatabaseConnection::escapeTable
  *
  */
  public function escapeTable($table) {
    return preg_replace('/[^A-Za-z0-9_.]+/', '', $table);
  }

  /**
   * Alter a query to use the overridden revision id as well as
   * revision fields.
   *
   * @param \SelectQueryInterface $data
   *   This expect a SelectQuery Object to alter
   * @param $override_controller
   *   This is an override controller to use to find
   *   override data
   *
   * @return \Drupal\sps\Plugins\Reaction\EntitySelectQueryAlterReaction
   *  Self
   */
  public function react($data, \Drupal\sps\Plugins\OverrideControllerInterface $override_controller) {
    $query = $data->query;
      $alias = $this->extractAlias($query);
    //exit prematurly if we ha a no alter tag
    if($query->hasTag(SPS_NO_ALTER_QUERY_TAG)) {
      return;
    }
    $alias = $this->extractAlias($query);
    if($alias) {
      $this->addRevisionTables($query);
      $alias = $this->extractAlias($query);

      $property_map = $override_controller->getPropertyMap();
      $this->addOverrideTable($query, $override_controller);

      $fields =& $query->getFields();
      $this->fieldsToExpressions($query,  array_keys($property_map), $alias);
      $this->fieldReplace($fields, $alias, $property_map);

      $expressions =& $query->getExpressions();

      $this->recursiveReplace($expressions, $alias, $property_map);

      $tables =& $query->getTables();
      $this->recursiveReplace($tables, $alias, $property_map);

      $where =& $query->conditions();
      $this->encapCondition($where);

      $this->recursiveReplace($where, $alias, $property_map);

      $order =& $query->getOrderBy();
      $this->recursiveReplace($order, $alias, $property_map);

      $group =& $query->getGroupBy();
      $this->recursiveReplace($group, $alias, $property_map);

      $having =& $query->havingConditions();
      $this->recursiveReplace($having, $alias, $property_map);
      /*
      $this->recursiveReplace($fields);

      */
    }
    return $this;
  }
}
