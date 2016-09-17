<?php
namespace Drupal\sps\Test;

class SelectQuery extends \SelectQuery {
  /**
   * The fields to SELECT.
   *
   * @var array
   */
  public $fields = array();

  /**
   * The expressions to SELECT as virtual fields.
   *
   * @var array
   */
  public $expressions = array();

  /**
   * The tables against which to JOIN.
   *
   * This property is a nested array. Each entry is an array representing
   * a single table against which to join. The structure of each entry is:
   *
   * array(
   *   'type' => $join_type (one of INNER, LEFT OUTER, RIGHT OUTER),
   *   'table' => $table,
   *   'alias' => $alias_of_the_table,
   *   'condition' => $condition_clause_on_which_to_join,
   *   'arguments' => $array_of_arguments_for_placeholders_in_the condition.
   *   'all_fields' => TRUE to SELECT $alias.*, FALSE or NULL otherwise.
   * )
   *
   * If $table is a string, it is taken as the name of a table. If it is
   * a SelectQuery object, it is taken as a subquery.
   *
   * @var array
   */
  public $tables = array();

  /**
   * The fields by which to order this query.
   *
   * This is an associative array. The keys are the fields to order, and the value
   * is the direction to order, either ASC or DESC.
   *
   * @var array
   */
  public $order = array();

  /**
   * The fields by which to group.
   *
   * @var array
   */
  public $group = array();

  /**
   * The conditional object for the WHERE clause.
   *
   * @var DatabaseCondition
   */
  public $where;

  /**
   * The conditional object for the HAVING clause.
   *
   * @var DatabaseCondition
   */
  public $having;

  /**
   * Whether or not this query should be DISTINCT
   *
   * @var boolean
   */
  public $distinct = FALSE;

  /**
   * The range limiters for this query.
   *
   * @var array
   */
  public $range;

  /**
   * An array whose elements specify a query to UNION, and the UNION type. The
   * 'type' key may be '', 'ALL', or 'DISTINCT' to represent a 'UNION',
   * 'UNION ALL', or 'UNION DISTINCT' statement, respectively.
   *
   * All entries in this array will be applied from front to back, with the
   * first query to union on the right of the original query, the second union
   * to the right of the first, etc.
   *
   * @var array
   */
  public $union = array();

  /**
   * Indicates if preExecute() has already been called.
   * @var boolean
   */
  public $prepared = FALSE;

  /**
   * The FOR UPDATE status
   */
  public $forUpdate = FALSE;


  public function __construct() {
  /*
    $this->where = new \DatabaseCondition('AND');
    $this->having = new \DatabaseCondition('AND');
    $having_c = & $this->having->conditions();
    $where_c = & $this->where->conditions();
    $this->forUpdate = $config['forUpdate'];
    $this->prepared = $config['prepared'];
    $this->union = $config['union'];
    $this->range = $config['range'];
    $this->distinct = $config['distinct'];
    $having_c = $config['having->conditions'];
    $this->group = $config['group'];
    $this->order = $config['order'];
    $this->tables = $config['tables'];
    $this->expressions = $config['expressions'];
    $this->fields = $config['fields'];
    $where_c = $config['where->conditions'];
    */
  }
  static function extractConfig($query) {
    $config = array( 
      'forUpdate' => FALSE,
      'prepared' => FALSE,
      'union' => array(),
      'range' => NULL,
      'distinct' => FALSE,
      'having->conditions' => $query->havingConditions(),
      'where->conditions' => $query->conditions(),
      'group' => $query->getGroupBy(),
      'order' => $query->getOrderBy(),
      'tables' => $query->getTables(),
      'expressions' => $query->getExpressions(),
      'fields' => $query->getFields(),
    );
    return $config;
  }
  static function selectQuery($query) {
    $query->__sleep();
    $config =  var_export($query, TRUE);
    $config = preg_replace("/SelectQuery/", "Drupal\sps\Test\SelectQuery", $config);
    $config = preg_replace("/DatabaseConnection_mysql::__set_state/", "Drupal\sps\Test\SelectQuery::getNull", $config);
    $config = preg_replace("/DatabaseCondition/", "Drupal\sps\Test\DatabaseCondition", $config);
    $query = eval("return " .$config .";");
    unset($query->connection);
    return $query;
  }
  static function exportSelect($query) {
    $query = SelectQuery::selectQuery($query);
    return var_export($query, TRUE);
  }
  static function getNull() {
  
  }
  
  static function __set_state($state) {
    $select = new SelectQuery(array());
    foreach($state as $key => $value) {
      $select->{$key} = $value;
    }
    return $select;
  }
}
