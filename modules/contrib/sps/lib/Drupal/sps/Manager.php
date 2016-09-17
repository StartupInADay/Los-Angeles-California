<?php

namespace Drupal\sps;

/**
 * The Manager is the heart of the SPS system, taking inputs from different
 * parts of the system and pushing them to the correct object for processing
 * it can be organized in to a few different sections
 *
 * Managing SiteState
 *
 * The Site State hold information about what should be alter for preview, mode
 * as well as the form used to select those overrides.
 *
 * @see getSiteState()
 * @see setSiteState()
 * @see clearSiteState()
 *
 * Managing Preview Form
 *
 * The Preview form is the tool used by SPS to let the User select what
 * entities show be overridden
 *
 * @see getPreviewForm()
 * @see previewFormSubmitted()
 *
 * Manging Preview Reactions
 *
 * The Reactions are the part of SPS that use override data, to change things
 * in drupal so that the user can view a preview site
 *
 * @see react()
 *
 * Accessing Plugins
 *
 * SPS plugins are the abstract tool used to extend the what, when and how of
 * overrides.
 *
 * @see getPlugin()
 * @see getPluginInfo()
 * @see getPluginByMeta()
 * @see sps.plugin.api.php
 *
 * Accessing System controllers
 *
 * Controllers are used to abstract parts of the SPS system, where there is an
 * expectation that plugins will be addative, Controllers are seen as
 * singletons, that might be replaced but there will always be only one.
 *
 * @see getDrupalController()
 * @see getPluginController()
 * @see getStateController()
 * @see getConfigController()
 */
class Manager {
  protected $state_controller;
  protected $config_controller;
  protected $drupal_controller;
  protected $root_condition;
  protected $plugin_controller;
  protected $override_controller;

  /**
   * Constructor for \Drupal\sps\Manager.
   *
   * Load the Drupal, Plugin and State controller form the configuration.
   *
   * @param \Drupal\sps\StorageControllerInterface $config_controller
   *   the control to be used when accessing config
   *
   * @return \Drupal\sps\Manager
   */
  public function __construct(StorageControllerInterface $config_controller) {

    $this->setConfigController($config_controller)
      ->setDrupalController($this->createControllerFromConfig(SPS_CONFIG_DRUPAL_CONTROLLER))
      ->setPluginController($this->createControllerFromConfig(SPS_CONFIG_PLUGIN_CONTROLLER))
      ->setStateController($this->createControllerFromConfig(SPS_CONFIG_STATE_CONTROLLER));
  }

  /**
   * Create a Controller Object based upon a configuration key.
   *
   * @param $key
   *  The key from the configuration array that contains the controller
   *  information.
   *
   * @return StateControllerInterface|PluginControllerInterface|Drupal
   */
  protected function createControllerFromConfig($key) {
    $controller_info = $this->getConfigController()->get($key);
    $controller_class = $controller_info['class'];
    $controller_settings = $controller_info['instance_settings'];
    return new $controller_class($controller_settings, $this);
  }

  /**
   * Store the state controller.
   *
   * @param \Drupal\sps\StateControllerInterface $controller
   *   The control to use when accessing State info (like site state)
   *
   * @return \Drupal\sps\Manager
   *   Self
   */
  protected function setStateController(StateControllerInterface $controller) {
    $this->state_controller = $controller;
    return $this;
  }

  /**
   * Store the config controller.
   *
   * @param \Drupal\sps\StorageControllerInterface $controller
   *   The control to be used when accessing config.
   *
   * @return \Drupal\sps\Manager
   *   Self
   */
  protected function setConfigController(StorageControllerInterface $controller) {
    $this->config_controller = $controller;
    return $this;
  }

  /**
   * Store the override controller.
   *
   * @param \Drupal\sps\StorageControllerInterface $controller
   *   the control to use when accessing overrides
   *
   * @return \Drupal\sps\Manager
   *   Self
   */
  protected function setOverrideController(StorageControllerInterface $controller) {
    $this->override_controller = $controller;
    return $this;
  }

  /**
   * Store the override controller.
   *
   * @param \Drupal\sps\PluginControllerInterface $controller
   *   The control to use when accessing plugins
   *
   * @return \Drupal\sps\Manager
   *   Self
   */
  protected function setPluginController(PluginControllerInterface $controller) {
    $this->plugin_controller = $controller;
    return $this;
  }

  /**
   * Store the hook controller.
   *
   * @param \Drupal\sps\Drupal $controller
   *   The control to use when accessing drupal invoke and alter function
   * @return \Drupal\sps\Manager
   *   Self
   */
  protected function setDrupalController(Drupal $controller) {
    $this->drupal_controller = $controller;
    return $this;
  }

  /**
   * Pull the site state form site state controller.
   *
   * Note the state controller is responsible for reasonable caching of the
   * site state.
   *
   * @return \Drupal\sps\SiteState | NULL
   */
  public function getSiteState() {
    if ($this->state_controller->exists()) {
      return $this->state_controller->get();
    }

    return NULL;
  }

  /**
   * Create A SiteState form an override, and store it.
   *
   *   _________________________
   *   \                        \
   *    \  Manager::setSiteState \
   *     ) called                 )
   *    /                        /
   *   /________________________/
   *                 |
   *        Condition Object
   *                 |
   *                 v
   *  ____________________________
   *  \                           \      .----------------------------.
   *   \  Get map                  \     |       Manager (this)       |
   *    ) API => OverrideController )--->|----------------------------|
   *   /                           / ^   | getOverrideControllerMap() |
   *  /___________________________/  |   '----------------------------'
   *                 |               |                  |
   *                 v               '-map api=>objects-'
   *       _________________
   *       \                \                     .-------------------.
   *        \  Get Class     \                    | Config Controller |
   *         ) for Site State )------------------>|-------------------|
   *        /                / ^                  | get()             |
   *       /________________/  |                  '-------------------'
   *                 |         |                            |
   *                 v         '-name of SiteState class----'
   * _____________________________
   * \                            \
   *  \   Use the Condition Object \
   *   \  and the Map to            \
   *    ) Construct Site State       )
   *   /  of the Class retrieve     /
   *  /   from config              /
   * /____________________________/
   *                 |
   *                 v
   *       _________________
   *       \                \                     .-------------------.
   *        \  Store         \                    | State Controller  |
   *         ) Site State     )-SiteState object->|-------------------|
   *        /                /                    | set()             |
   *       /________________/                     '-------------------'
   *
   * @param \Drupal\sps\Plugins\ConditionInterface $condition
   *
   * @return \Drupal\sps\Manager
   *   Self
   */
  public function setSiteState(\Drupal\sps\Plugins\ConditionInterface $condition) {
    $controller_map = $this->getOverrideControllerMap();
    $site_state_class = $this->getConfigController()->get(SPS_CONFIG_SITESTATE);
    $site_state = new $site_state_class($condition, $controller_map);
    $this->state_controller->set($site_state);
    return $this;
  }

  /**
   * If there is a current Site State Remove it.
   *
   * The method us mostly use when one is cancaling out of a preview state.
   *
   * @return \Drupal\sps\Manager self
   */
  public function clearSiteState() {
    $this->getStateController()->clear();
    return $this;
  }

  /**
   * Find all active Reaction Plugins and return the info from hook_sps_reaction_info
   *
   * Currently there is no restriction on what reactions are active.
   *
   * @TODO add check to configuration to see if there is a list of reactions
   * that should be active
   *
   * @return array
   *   map of reaction plugin names to thier info
   */
  protected function getActiveReactionInfo() {
    return $this->getPluginInfo('reaction');
  }

  /**
   * product a map from controller api to override controller instances
   *
   * we start with a list of apis need by reations
   * then we add to it the first override controller we come to for apis
   * not in the config. Also if a controller implements 2 apis we do not
   * create two instances but instead point to the same one.
   *  ____________________________________
   *  \                                   \
   *   \                                   \
   *    ) Manager::getOverrideControllerMap )
   *   /                                   /
   *  /___________________________________/
   *                     |
   *                     v
   *           __________________
   *           \                 \             .-------------------.
   *            \  Extract Needed \            | Plugin Controller |
   *             ) Controller APIs )---------->|-------------------|
   *            /  from Plugins   / ^          | getPluginInfo()   |
   *           /_________________/  |          '-------------------'
   *                     |          |                    |
   *                     v          '---Reaction Info ---'
   *           __________________
   *           \                 \             .-------------------.
   *            \  Get Override   \            | Config Controller |
   *             ) Controller Map  )---------->|-------------------|
   *            /  from Config    / ^          | get()             |
   *           /_________________/  |          '-------------------'
   *                     |          |                    |
   *                     v       Override controller Map-'
   *          ____________________
   *          \                   \             .-------------------.
   *           \  for missing APIs \            | Plugin Controller |
   *            ) get Controllers   )---------->|-------------------|
   *           /  from plugins     / ^          | getPluginInfo()   |
   *          /___________________/  |          '-------------------'
   *                     |           |                    |
   *                     v           '--Override Controller Info
   *           __________________
   *           \                 \              .-------------------.
   *            \  Get Controller \             | Plugin Controller |
   *             ) Plugins         )----------->|-------------------|
   *            /                 / ^           | getPlugin()       |
   *           /_________________/  |           '-------------------'
   *                     |          |                     |
   *                     |          '--Override Controller Objects
   *                     |
   *                     |
   *                     v
   *  .------------------------------------.
   *  | Return Map of                      |
   *  | API => Override Controller Objects |
   *  '------------------------------------'
   *
   * @return array
   *   a mapping of override controller api keys to override controller objects.
   */
  protected function getOverrideControllerMap() {

    $controllers = array();
    $controllers_instances = array();
    $instances = array();

    // Find all need apis.
    foreach ($this->getActiveReactionInfo() as $info) {
      $controllers[$info['use_controller_api']] = NULL;
    }
    $config = $this->getConfigController()->exists(SPS_CONFIG_OVERRIDE_CONTROLLERS) ?
      $this->getConfigController()->get(SPS_CONFIG_OVERRIDE_CONTROLLERS) : array();
    $infos = $this->getPluginInfo('override_controller');

    // If the config has valid controllers use them.
    foreach ($config as $api => $name) {
      if (isset($infos[$name])) {
        $controllers[$api]  = $name;
      }
    }
    foreach ($controllers as $api => $name) {
      if (!$name) {
        foreach ($infos as $info_name => $info) {
          // If we have not found a controller yet lets see if this one
          // implements the api.
          if (!$name) {
            $imp = is_array($info['implements_controller_api']) ? $info['implements_controller_api'] : array($info['implements_controller_api']);
            foreach ($imp as $imp_api) {
              if ($imp_api == $api) {
                $name = $info_name;
              }
            }
          }
        }
      }
      if ($name) {
        if (!isset($instances[$name])) {
          $instances[$name] = $this->getPlugin('override_controller', $name);
        }
        $controllers_instances[$api] = $instances[$name];
      }
    }
    return $controllers_instances;
  }

  /**
   * Passthrough from Drupal form to the correct condition for building the preview form
   *
   * not that RootCondition is configurable using SPS_CONFIG_ROOT_CONDITION
   *
   * @see sps_condition_preview_form
   *
   * @return array|mixed
   *  A drupal form array created by the root condition
   */
  public function getPreviewForm() {
    $root_condition = $this->getRootCondition();
    return $this->getDrupalController()->drupal_get_form('sps_condition_preview_form', $root_condition);
  }

  /**
   * Notify the manager that the preview form submission is complete.
   *
   * This method is call buy the sps_condition_preview_form submit function
   *
   * @see sps_condition_preview_form_submit
   *
   * @param Plugins\ConditionInterface $root_condition
   *
   * @return \Drupal\sps\Manager
   *  Self
   */
  public function previewFormSubmitted(\Drupal\sps\Plugins\ConditionInterface $root_condition) {
    $this->setSiteState($root_condition);
    return $this;
  }

  /**
  * Helper method for getting and causing the root Condition
  *
  * The Root condition is the use as the basis for the constructing the preview form
  * It can be expect that it will be much more complicated then the other conditions
  *
  * This method select the condition and its config using the config controller.
  *
  * @return \Drupal\sps\Plugins\ConditionInterface
  *   the current root condition object
  */
  protected function getRootCondition() {
    if ($site_state = $this->getSiteState()) {
      return $site_state->getCondition();
    }
    $settings = $this->config_controller->get(SPS_CONFIG_ROOT_CONDITION);
    $root_condition_plugin = $settings['name'];
    return  $this->getPlugin('condition', $root_condition_plugin);
  }

  /**
   * call a reaction rect method
   *
   * The manager is use as an interface for Drupal hooks that need to have a
   * reaction react
   *
   * ___________________
   * \                  \
   *  \  Manager::react  \
   *   ) called           )----Reaction Name and Data
   *  /                  /     |
   * /__________________/      |
   *            ^              v
   *            |      ______________
   *            |      \             \
   *            |       \  Check if   \
   *       On Admin Page-) on admin    )
   *            |       /             /
   *            |      /_____________/
   *            |              |
   *            |              v
   *            |      ______________
   *            |      \             \                        .------------------.
   *            |       \  Get Site   \                       | State Controller |
   *       No SiteState--) State       )--------------------->|------------------|
   *            |       /             / ^                     | get()            |
   *            |      /_____________/  |                     '------------------'
   *            |              |        |                               |
   *            |              v        '------SiteState object---------'
   *            |    _________________
   *            |    \                \
   *            |     \  Get Reaction  \
   *      No Reaction  ) ORC api key    )---------reaction name---------.
   *            |     /  and object    / ^                              v
   *            |    /________________/  |                    .-------------------.
   *            |              |         |                    | Plugin Controller |
   *            |              |         |                    |-------------------|
   *            |              |         |                    | getPluginInfo()   |
   *            |              |         |                    | getPlugin()       |
   *            |              |         |                    '-------------------'
   *            |              v         |                              |
   *            |    _________________   '--Override Controller API Key-'
   *            |    \                \     Reaction Object
   *            |     \  Get Override  \
   *          No ORC---) Controller     )-Override Controller API Key-.
   *            |     /                / ^                            v
   *            |    /________________/  |               .-------------------------.
   *            |              |         |               |       Site State        |
   *            |              |         |               |-------------------------|
   *            |              |         |               | getOverrideController() |
   *            |              |         |               '-------------------------'
   *            |              |         |                            |
   *            |              v         '-OverrideController object -'
   *            |        _________
   *            |        \        \
   *            |         \  Call  \
   *     retrun data-------) React  )---data and OverrideController obj-.
   *                      /        /                                    v
   *                     /________/                           .------------------.
   *                           ^                              |     Reaction     |
   *                           |                              |------------------|
   *                           |                              | react()          |
   *                           |                              '------------------'
   *                           |                                        |
   *                           '--------return data---------------------'
   *
   *
   * @param String $reaction
   *   the name of a reaction plugin;
   * @param mixed $data
   *   data to be passed to the react method
   *
   * @return mixed
   *   Data used by the item calling reaction
   */
  public function react($reaction, $data) {
    $infos = $this->getActiveReactionInfo();
    if (isset($infos[$reaction]) &&
       ($site_state = $this->getSiteState()) &&
       ($controller = $site_state->getOverrideController($infos[$reaction]['use_controller_api']))
      ) {
      // Calling path_is_admin() to early can lead to cache poisoning of
      // entity_get_info() - which means a whole bunch of entity information is
      // missing. Thus check it as separated final condition.
      // @TODO Find a better way to deal with this. This is just a lucky shot
      // and not a proper solution.
      if (!sps_drupal()->path_is_admin(sps_drupal()->current_path())) {
        return $this->getPlugin("reaction", $reaction)->react($data, $controller);
      }
    }
  }

  /**
   * Passthough function to the Plugin Controller for building plugin objects
   *
   * @param String $type
   *   the type of plugin as defined in hook_sps_plugin_types_info
   * @param String $name
   *   the name of the plugin as defined in hook_sps_PLUGIN_TYPE_plugin_info;
   * @param Array $settings
   *   an array that should be used for instance settings (instance settings from the plugin info are
   *   add to this array
   * @return \Drupal\sps\Plugins\PluginInterface
   *   An instance of the requested Plugin
   */
  public function getPlugin($type, $name, $settings = NULL) {
    return $this->plugin_controller->getPlugin($type, $name, $this, $settings);
  }

  /**
   * Passthough function to the Plugin Controller for retrieving plugin info
   *
   * @param String $type
   *   the type of plugin as defined in hook_sps_plugin_types_info
   * @param String | Null $name
   *   the name of the plugin as defined in hook_sps_PLUGIN_TYPE_plugin_info;
   *
   * @return Array
   *   an array of meta data for the plugin
   */
  public function getPluginInfo($type, $name=NULL) {
    return $this->plugin_controller->getPluginInfo($type, $name);
  }

  /**
   * Passthough function to the Plugin Controller for retrieving plugin info on filtered results
   *
   * @param String $type
   *   the type of plugin as defined in hook_sps_plugin_types_info
   * @param String $property
   *   the meta property to compare to the value
   * @param mixed $value
   *   the value to compare to the meta property
   *
   * @return Array
   *   an array of meta data for the plugins
   */
  public function getPluginByMeta($type, $property, $value) {
    return $this->plugin_controller->getPluginByMeta($type, $property, $value);
  }

  /**
   * Get the hook controller
   *
   * @return
   *   Drupal object
   */
  public function getDrupalController() {
    return $this->drupal_controller;
  }

  /**
   * Get the Plugin Controller
   *
   * @return PluginControllerInterface
   */
  public function getPluginController() {
    return $this->plugin_controller;
  }

  /**
   * Get the State Controller
   *
   * @return StateControllerInterface
   */
  public function getStateController() {
    return $this->state_controller;
  }

  /**
   * Get the config Controller
   *
   * @return StorageControllerInterface
   */
  public function getConfigController() {
    return $this->config_controller;
  }

}
