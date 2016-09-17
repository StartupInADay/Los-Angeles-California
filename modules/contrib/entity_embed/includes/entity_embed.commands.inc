<?php

/**
 * @file
 * AJAX commands.
 */

/**
 * Creates a Drupal Ajax 'insert' command that is capable of targeting elements
 * within iFrames.
 *
 * @param string $html
 *   The HTML content that will replace the matched element(s).
 *
 * @return
 *   An array suitable for use with the ajax_render() function.
 */
function entity_embed_command_insert($html) {
  return array(
    'command' => 'entityEmbedInsertEditor',
    'html' => $html,
  );
}
