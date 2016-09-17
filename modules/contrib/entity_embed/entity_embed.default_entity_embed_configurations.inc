<?php

/**
 * @file
 * Default button configuration for the default entity types.
 */

/**
 * Implements hook_default_entity_embed_configuration().
 */
function entity_embed_default_entity_embed_configuration() {
  $configurations = array();

  $configuration = new stdClass();
  $configuration->disabled = FALSE; /* Edit this to true to make a default configuration disabled initially */
  $configuration->api_version = 1;
  $configuration->admin_title = 'Node';
  $configuration->name = 'node';
  $configuration->entity_type = 'node';
  $configuration->entity_type_bundles = array();
  $configuration->button_icon_fid = 0;
  $configuration->display_plugins = array();
  $configurations['node'] = $configuration;

  return $configurations;
}
