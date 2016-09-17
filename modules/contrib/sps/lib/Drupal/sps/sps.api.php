<?php

/**
 * Implements hook_sps_plugin_types().
 *
 * Define the plugin types.
 */
function hook_sps_plugin_types() {
  $plugins = array();
  $plugins['MyPluginType'] = array(
    'interface' => 'MyPluginType',
  );
}
