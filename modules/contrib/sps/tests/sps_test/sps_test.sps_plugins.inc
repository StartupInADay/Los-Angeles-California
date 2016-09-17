<?php

function sps_test_sps_override_plugins() {
  return array(
    'node_first_revision'=> array(
      'class' => 'Drupal\sps_test\NodeFirstRevisionOverride',
      'condition' => 'first_revision',
    ),
  );
}
function sps_test_sps_condition_plugins() {
  return array(
    'first_revision_condition'=> array(
      'instance_settings' => array(
        'name' => 'first_revision',
        'widget' => 'first_revision_widget',
      ),
    ),
  );
}
function sps_test_sps_widget_plugins() {
  return array(
    'first_revision_widget'=> array(
      'class' => '\Drupal\sps_test\SingleCheckWidget',
      'instance_settings' => array(
        'title' => 'Show First Revision'
      ),
    ),
  );
}
