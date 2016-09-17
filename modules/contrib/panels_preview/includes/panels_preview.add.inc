<?php

/**
 * Preprocess the panels_add_content_modal() function to add the preview HTML.
 */
function panels_preview_preprocess_panels_add_content_modal(&$vars) {
  // Generate the pane preview.
  $plugins = &$vars['categories'][$vars['category']]['content'];
  $i = 0;
  if (!empty($plugins)) {
    foreach ($plugins as $name => &$plugin) {
      $pane = panels_new_pane($plugin['type_name'], $plugin['subtype_name'], TRUE);
      $display = $vars['renderer']->display;
      $context = $vars['renderer']->display->context;
      $args = $vars['renderer']->display->args;
      $incoming_content = $vars['renderer']->display->incoming_content;
      $keywords = (!empty($vars['renderer']->display->keywords)) ? $vars['renderer']->display->keywords : array();
      // Workaround for Views autocomplete not rendering content.
      if (strpos($name, 'autocomplete') !== FALSE && $pane->type == 'views') {
        return;
      }
      $content = ctools_content_render($pane->type, $pane->subtype, $pane->configuration, $keywords, $args, $context, $incoming_content);
      if (isset($content) && is_object($content)) {
        $preview = theme('panels_pane', array('content' => $content, 'pane' => $pane, 'display' => $display));
        $plugin['preview'] = $preview;
        if (variable_get('panels_preview_strip_js_from_preview', 0) == TRUE && !empty($preview)) {
          $plugin['preview'] = preg_replace('#<script(.*?)>(.*?)</script>#is', '', $preview);
        }
      }
    }
  }
}

/**
 * Process the panels_add_content_modal() adjusting preview presentaton.
 */
function panels_preview_process_panels_add_content_modal(&$vars) {
  $content = !empty($vars['categories'][$vars['category']]['content']) ? $vars['categories'][$vars['category']]['content'] : array();
  // If no category is selected or the category is empty or our special empty
  // category render a 'header' that will appear instead of the columns.
  if (empty($vars['category']) || empty($content) || $vars['category'] == 'root') {
    // Nothing to do since there is no preview
  }
  else {
    $titles = array_keys($content);

    // Zero out the existing column data.
    unset($vars['columns']);
    $vars['columns'] = array();

    // Read the column data with our preview functionality.
    foreach ($titles as $title) {
      // Use title if description is empty.
      if (empty($content[$title]['description'])) {
        $content[$title]['description'] = $title;
      }
      $content[$title]['title'] = 'Add';
      $legend_title = !empty($title) ? $title : 'Untitled ' . $content[$title]['type_name'];
      // Check if there is a preview
      $classes = 'collapsed';
      if (!empty($content[$title]['preview'])) {
        $classes .= ' collapsible';
      }
      // Empty, do not allow user to expand preview and update title
      else {
        $legend_title .= ' (' . t('No Preview') . ')';
      }
      $vars['columns'][0] .= '<fieldset class="' . $classes .' widget-preview"><legend><div class="widget-preview-title">' . theme('panels_add_content_link', array('renderer' => $vars['renderer'], 'region' => $vars['region'], 'content_type' => $content[$title])) . '</div><span class="fieldset-legend">' . $legend_title . '</span></legend><div class="fieldset-wrapper">' . (!empty($content[$title]['preview']) ? $content[$title]['preview'] : t('No Preview')) . '</div></fieldset>';
    }

    // Make sure the collapse library is added
    drupal_add_library('system', 'drupal.collapse');
  }
}
