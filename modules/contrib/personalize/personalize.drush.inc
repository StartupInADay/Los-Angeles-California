<?php
/**
* @file personalize.drush.inc
 * Provides drush commands for Personalize module.
*/

/**
 * Implements hook_drush_command().
 */
function personalize_drush_command() {
  $items['personalize-complete-agents'] = array (
    'description' => 'Completes any personalizations that are due to be completed',
    'aliases' => array('pers-complete'),
  );
  return $items;
}

/**
 * Completes any agents due to be completed.
 *
 * This command is the same as what gets run on cron to check for agents that need
 * to be completed.
 */
function drush_personalize_complete_agents() {
  $completed = personalize_complete_agents();
  if (empty($completed)) {
    $msg = dt('No personalizations are due to be completed at this time.');
    $status = 'ok';
  }
  else {
    $msg = dt('The following personalizations have been completed: @agents.', array('@agents' => implode(', ', $completed)));
    $status = 'success';
  }
  drush_log($msg, $status);
}
