<?php
namespace Drupal\sps\Test;

class TableOverrideStorageController extends \Drupal\sps\Plugins\OverrideController\AbstractTableOverrideStorageController {
  public $table;
  public function addOverrideJoin(\SelectQueryInterface $query, $base_alias, $base_id, $overrides_alias, $type) {
    $alias = $query->addJoin("LEFT OUTER", 'test_override', $overrides_alias, "$base_alias.$base_id = $overrides_alias.id");
    $tables =& $query->getTables();
    $new_tables = array();
    $found_base = FALSE;
    foreach($tables as $key => $table) {

      if ($table['alias'] == $base_alias) {
        $new_tables[$key] = $table;
        $new_tables[$alias] = $tables[$alias];
      }
      else if ($key == $alias) {}
      else {
        $new_tables[$key] = $table;
      }
    }
    $tables = $new_tables;
    return $alias;
  }

  public function set($table) {
    $this->table = $table;
  }
  
  

  public function __construct(array $config, \Drupal\sps\Manager $manager) {}
}

