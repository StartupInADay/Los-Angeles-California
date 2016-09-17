<?php

namespace Drupal\sps;

use Drupal\sps\Exception\InvalidPluginException;
use Drupal\sps\Exception\ClassLoadException;
use Drupal\sps\Exception\DoesNotImplementException;


/**
 * The plugin factory will load the plugins objects and info
 */
class PluginFactory implements PluginControllerInterface {
  // Array of plugin info.
  protected $plugin_info = array();
  // The info array for the plugin type info.
  protected $plugin_type_info = array();
  // control for invoking drupal hooks.
  protected $manager;

  /**
   * @param $settings array
   * @param $manager Manager
   */
  public function __construct($settings, Manager $manager) {
    $this->manager = $manager;
    $this->loadPluginTypeInfo();
  }

  /**
   * get meta info on a plugin.
   *
   * @param $plugin_type
   *   the type of plugin as defined in hook_sps_plugin_types_info
   * @param $name
   *   the name of the plugin as defined in hook_sps_PLUGIN_TYPE_plugin_info;
   *
   * @throws \Drupal\sps\Exception\InvalidPluginException
   * @return mixed
   *  array of meta data for the plugin or an array of plugin arrays
   */
  public function getPluginInfo($plugin_type, $name = NULL) {
    $this->loadPluginTypeInfo();
    if (empty($this->plugin_type_info[$plugin_type])) {
      throw new InvalidPluginException("Plugin Type $plugin_type does not exist");
    }

    $this->loadPluginInfo($plugin_type);

    if (isset($name)) {
      if (!isset($this->plugin_info[$plugin_type][$name])) {
        throw new InvalidPluginException("Plugin $name for Plugin type $plugin_type does not exist");
      }

      return $this->plugin_info[$plugin_type][$name];
    }

    return $this->plugin_info[$plugin_type];
  }


  /**
   * Factory for building a plugin object.
   *
   * @param $type
   *   the type of plugin as defined in hook_sps_plugin_types_info
   * @param $name
   *   the name of the plugin as defined in hook_sps_PLUGIN_TYPE_plugin_info;
   * @param \Drupal\sps\Manager $manager
   *
   * @param Array $settings
   *   an array that should be used for instance settings (instance settings from the plugin info are
   *   add to this array
   *
   * @return mixed|\Drupal\sps\Plugins\PluginInterface|NULL
   *   An instance of the plugin object or NULL if the plugin does not use a class
   *
   * @throws \Drupal\sps\Exception\InvalidPluginException
   * @throws \Drupal\sps\Exception\ClassLoadException
   * @throws \Drupal\sps\Exception\DoesNotImplementException
   */
  public function getPlugin($type, $name, Manager $manager, $settings = NULL) {
    $plugin_info = $this->getPluginInfo($type, $name);
    // Start with settings for our instance settings.
    $settings = $settings ?: array();
    $plugin_info['instance_settings'] = $settings + $plugin_info['instance_settings'];

    if (isset($plugin_info['class'])) {
      $plugin_type_info = $this->getPluginInfo($type);

      $class_name = isset($plugin_info['class']) ? $plugin_info['class'] : $plugin_type_info['class'];

      try {
        $plugin_obj = new $class_name($plugin_info['instance_settings'], $manager);
      }
      catch (\Exception $e) {
        throw new ClassLoadException("Plugin $name was not loaded: " . $e->getMessage(), NULL, $e);
      }

      return $plugin_obj;
    }

    return NULL;
  }

  /**
   * Load the Plugin info into the objects cache.
   *
   * @param $plugin_type
   *
   * @return PluginFactory
   *  self
   */
  protected function loadPluginInfo($plugin_type) {
    $this->loadPluginTypeInfo();
    if (empty($this->plugin_info[$plugin_type])) {
      $this->plugin_info[$plugin_type] = array();

      $hook = "sps_{$plugin_type}_plugins";
      foreach ($this->getDrupalController()->module_implements($hook) as $module) {
        $module_infos = $this->getDrupalController()->module_invoke($module, $hook);
        foreach ($module_infos as $plugin_name => $plugin_info) {
          if (!is_array($this->plugin_type_info[$plugin_type]['defaults'])) {
          }
          $plugin_info += $this->plugin_type_info[$plugin_type]['defaults'] + array(
            'plugin_type' => $plugin_type,
            'module' => $module,
            'name' => $plugin_name,
            'instance_settings' => array(),
          );

          $this->getDrupalController()->ref['drupal_alter']("sps_plugin_info_{$plugin_type}_{$plugin_info['name']}", $plugin_info);
          $this->validatePluginInfo($plugin_info);

          $this->plugin_info[$plugin_type][$plugin_name] = $plugin_info;
        }
      }
    }

    return $this;
  }

  /**
   * Validate a plugin info array.
   *
   * @param $plugin_info
   *
   * @return \Drupal\sps\PluginFactory
   *  self
   * @throws \Drupal\sps\Exception\InvalidPluginException
   */
  protected function validatePluginInfo($plugin_info) {
    $type_info = $this->getPluginTypeInfo($plugin_info['plugin_type']);
    if (isset($type_info['requires'])) {
      $this->validatePluginInfoRequirements($plugin_info, $type_info['requires']);
    }

    if (isset($plugin_info['class'])) {
      $this->validatePluginClass($plugin_info);
    }

    $this->getDrupalController()->module_invoke_all("sps_validate_plugin_info",
      $plugin_info, $plugin_info['plugin_type'],
      $this->getPluginTypeInfo($plugin_info['plugin_type']));

    return $this;
  }

  /**
   * Validate that the plugin class is valid.
   *
   * @param $plugin_info
   *
   * @throws Exception\DoesNotImplementException
   *
   * @return \Drupal\sps\PluginFactory
   *  Self
   */
  protected function validatePluginClass($plugin_info) {
    $interface = $this->getPluginTypeInfo($plugin_info['plugin_type'], 'interface');
    if (!(self::checkInterface($plugin_info['class'], $interface))) {
      throw new DoesNotImplementException("Plugin {$plugin_info['name']} is not using the correct interface $interface");
    }

    if (!(self::checkInterface($plugin_info['class'], "Drupal\\sps\\Plugins\\PluginInterface"))) {
      throw new DoesNotImplementException("Plugin {$plugin_info['name']} is not using the correct interface Drupal\\sps\\Plugins\\PluginInterface");
    }

    return $this;
  }

  /**
   * Validate that the plugin info array contains the all of the required keys.
   *
   * @param $plugin_info
   *   an array of plugin data
   * @param $requirements
   *   an array of required fields that should be check, this should
   *   match the structure of the $plugin_info with TRUE for the values
   *   so to check that there is a parent field with a child field in it one whould do
   *   array("parent"=>array("child")
   * @param $parents
   *   a string used for recusion
   * @param $name
   *   a string used for recusion
   *
   * @throws Exception\InvalidPluginException
   *
   *
   * @return \Drupal\sps\PluginFactory
   *  self
   */
  protected function validatePluginInfoRequirements($plugin_info, $requirements, $parents = '', $name = NULL) {
    $name = $name ?: $plugin_info['name'];
    foreach ($requirements as $r_key => $r_type) {
      if (!isset($plugin_info[$r_key])) {
        throw new InvalidPluginException(
          "Plugin $name does not contain required element $parents$r_key");
      }
      else {
        if (is_array($r_type)) {
          $parents .= "$r_key=>";
          $this->validatePluginInfoRequirements($plugin_info[$r_key], $r_type, $parents, $name);
        }
        else {
          // TODO add type checking based on the value of $r_type.
        }
      }
    }
    return $this;
  }

  /**
   * Get Info about a plugin type
   *
   * @param $plugin_type
   *  The name of the plugin type
   *
   * @param null|string $key
   *  The element to retrieve
   *
   * @throws \Drupal\sps\Exception\InvalidPluginException
   * @return array
   */
  public function getPluginTypeInfo($plugin_type = NULL, $key = NULL) {
    $this->loadPluginTypeInfo();
    if (isset($plugin_type)) {
      if (empty($this->plugin_type_info[$plugin_type])) {
        throw new InvalidPluginException("Plugin Type $plugin_type does not exist");
      }

      if (isset($key)) {
        return !isset($this->plugin_type_info[$plugin_type][$key]) ?: $this->plugin_type_info[$plugin_type][$key];
      }

      return $this->plugin_type_info[$plugin_type];
    }

    return $this->plugin_type_info;
  }

  /**
   * Load the plugin type info.
   *
   * @return PluginFactory
   *  self
   */
  protected function loadPluginTypeInfo() {
    if (empty($this->plugin_type_info)) {
      foreach ($this->getDrupalController()->module_implements('sps_plugin_types') as $module) {
        $module_infos = $this->getDrupalController()->module_invoke($module, 'sps_plugin_types');

        foreach ($module_infos as $plugin_type_name => $plugin_type_info) {
          $plugin_type_info += array(
            'module' => $module,
            'name' => $plugin_type_name,
            'requires' => array(),
          );

          $this->getDrupalController()->ref['drupal_alter']("sps_plugin_type_info_{$plugin_type_info['name']}", $plugin_type_info);
          $this->getDrupalController()->module_invoke_all("sps_validate_plugin_type_info", $plugin_type_info);

          $this->plugin_type_info[$plugin_type_name] = $plugin_type_info;
        }
      }
    }

    return $this;
  }

  /**
   * @static
   *
   * @param $object
   * @param $interface
   *
   * @return bool
   */
  public static function checkInterface($object, $interface) {
    $ref_class = new \ReflectionClass($object);
    if (in_array($interface, $ref_class->getInterfaceNames())) {
      return TRUE;
    }
    return FALSE;
  }

  /**
   * Get meta info on a plugin.
   *
   * @param $type
   *   the type of plugin as defined in hook_sps_plugin_types_info
   * @param $property
   *   the meta property to compare to the value
   *   this can be a string or a array of tree keys
   * @param $value
   *   the value to compare to the meta property
   *
   * @return array
   *   an array of meta data for the plugins
   */
  public function getPluginByMeta($type, $property, $value) {
    $this->loadPluginInfo($type);

    $plugin_matches = array();
    foreach ($this->plugin_info[$type] as $plugin => $info) {
      if ($this->checkPluginMeta($info, $property, $value)) {
        $plugin_matches[$plugin] = $info;
      }
    }
    return $plugin_matches;
  }

  /**
   * Recursive function to search the meta data
   *
   * @param $plugin_info
   * @param $property
   *   this can be a string or a array of tree keys
   * @param $value
   *
   * @return bool
   */
  protected function checkPluginMeta($plugin_info, $property, $value) {
    $tree = array();
    if (is_array($property)) {
      $tree = $property;
      $property = array_shift($tree);
    }
    foreach ($plugin_info as $plugin_info_key => $plugin_info_value) {
      if ($plugin_info_key == $property) {
        if($tree) {
        return $this->checkPluginMeta($plugin_info[$plugin_info_key], $tree, $value);
        }
        else if ($plugin_info_value == $value) {
          return TRUE;
        }
      }
    }
    return FALSE;
  }

  /**
   * Get the Drupal Controller for SPS
   *
   * @return Drupal
   */
  protected function getDrupalController() {
    return $this->manager->getDrupalController();
  }
}
