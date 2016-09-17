<?php
namespace Drupal\sps\Test;

class DatabaseCondition extends \DatabaseCondition {

  /**
   * Array of conditions.
   *
   * @var array
   */
  public $conditions = array();

  /**
   * Array of arguments.
   *
   * @var array
   */
  public $arguments = array();

  /**
   * Whether the conditions have been changed.
   *
   * TRUE if the condition has been changed since the last compile.
   * FALSE if the condition has been compiled and not changed.
   *
   * @var bool
   */
  public $changed = TRUE;

  /**
   * The identifier of the query placeholder this condition has been compiled against.
   */
  public $queryPlaceholderIdentifier;

  public function __construct() {}
  static function __set_state($state) {
    $select = new DatabaseCondition();
    foreach($state as $key => $value) {
      $select->{$key} = $value;
    }
    return $select;
  }
}
