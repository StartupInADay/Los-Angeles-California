<?php

/**
 * @file
 * Theme and processing functions for the Personalize module.
 */

/**
 * Themes a button that allows the text to be set independent of the value.
 *
 * @ingroup themeable
 */
function theme_personalize_html_tag($variables) {
  $element = $variables['element'];
  $attributes = isset($element['#attributes']) ? drupal_attributes($element['#attributes']) : '';
  $output = '<' . $element['#tag'] . $attributes . '>';
  $output .= $element['#text'];
  $output .= '</' . $element['#tag'] . ">\n";
  return $output;
}

/**
 * Theme wrapper for option sets.
 *
 * Wraps an option set in a script tag so that options are not parsed
 * by the browser on page load. Surrounds this with a div containing
 * all the information needed by our js.
 *
 * @ingroup themeable
 */
function theme_personalize_options_wrapper($variables) {
  if (!isset($variables['element']['#personalize_option_set'])) {
    return $variables['element']['#children'];
  }

  $option_set = $variables['element']['#personalize_option_set'];
  if (personalize_agent_get_status($option_set->agent) === PERSONALIZE_STATUS_COMPLETED) {
    if (!empty($option_set->winner)) {
      return drupal_render($variables['element']['#personalize_options'][$option_set->winner]);
    }
    elseif (isset($variables['element']['#first_option'])) {
      return drupal_render($variables['element']['#first_option']);
    }
    else {
      $first_option = reset($variables['element']['#personalize_options']);
      return drupal_render($first_option);
    }
  }
  $js_id = personalize_stringify_osid($option_set->osid);
  $prefix = '<div class="' . PERSONALIZE_OPTION_SET_CLASS . '" data-personalize="' . $js_id . '">';
  if (isset($variables['element']['#first_option'])) {
    $no_script_content = drupal_render($variables['element']['#first_option']);
    // Strip any noscript tags from the content so we don't end up with nested
    // noscript tags.
    $pattern = '/\<noscript\>(.*)\<\/noscript\>/';
    $no_script_content = preg_replace($pattern, '', $no_script_content);
    $prefix .= '<noscript>' . $no_script_content . '</noscript>';
  }

  $prefix .= '<script type="text/template" data-personalize-script="' . $js_id . '">';
  $suffix = '</script></div>';

  // Build array of rendered options for JSON encoding.
  $rendered = array();
  $i = 0;
  foreach($variables['element']['#personalize_options'] as $option_name => $option) {
    $rendered[$option_name] = array(
      'html' => drupal_render($option),
      'index' => $i,
    );
    $i++;
  }
  return $prefix . json_encode($rendered) . $suffix;
}

/**
 * Theme wrapper for option sets that are populated via callback.
 *
 * Provides a div to use with Ajax callback population.
 *
 * @ingroup themeable
 */
function theme_personalize_options_callback_wrapper($variables) {
  $option_set = $variables['element']['#personalize_option_set'];
  $js_id = personalize_stringify_osid($option_set->osid);
  $rendered = '<div class="' . PERSONALIZE_OPTION_SET_CLASS . '" data-personalize="' . $js_id . '"></div>';
  $rendered .= '<noscript>' . drupal_render($variables['element']['#first_option']) . '</noscript>';
  return $rendered;
}

/**
 * Theme wrapper for campaign status alert update message.
 *
 * @param $variables
 *   Associative array of information to display about the status alert.
 *   - agent_name: The machine name of the campaign that is being updated.
 *   - alert_message: An overall alert message to display.
 *   - option_message: A message to show to introduce the winning options.
 *   - option_sets: An associative array of options sets within this campaign.
 *     - label: The label of the option set
 *     - counter: A counter for the current option set
 *     - winner_label: The label of the winning variation for the option set.
 *     - winner_type: The type of winning variation (e.g., winner, control)
 *     - alert: Additional alert to show the end user regarding change.
 *
 * @ingroup themeable
 */
function theme_personalize_campaign_status_update($variables) {
  $option_message = $variables['option_message'];
  $option_sets = $variables['option_sets'];

  $update_message = empty($variables['alert_message']) ? '' : $variables['alert_message'];
  if (!empty($option_sets)) {
    if (!empty($option_message)) {
      $update_message .= ' ' . $option_message;
    }
    $winner_defaults = array(
      'label' => 'Content variation',
      'counter' => '',
      'winner_label' => t('Variation'),
      'winner_type' => t('Control'),
      'alert' => '',
    );
    $winners = array();
    foreach($option_sets as $option) {
      $option += $winner_defaults;
      $winner_message = "<strong>{$option['label']}:</strong> {$option['counter']} {$option['winner_label']} ({$option['winner_type']})";
      if (!empty($option['alert'])) {
        $winner_message .= ' <span class="personalize-alert">' . $option['alert'] . '</span>';
      }
      $winners[] = $winner_message;
    }
    $update_message .= theme('item_list', array('items' => $winners));
  }
  return $update_message;
}

/**
 * Theme function to display the title of an enumerated list.
 *
 * @param $variables
 *   Associative array of enumerated list item details
 *   - enum: The enumeration text; e.g., Set 1, Goal 2, etc.
 *   - title: The title of the list item.
 *   - title_prefix: An optional prefix for the title.
 *   - title_suffix: An optional suffix for the title.
 *
 * @ingroup themeable
 */
function theme_personalize_admin_enumerated_item($variables) {
  $variables['attributes']['class'][] = 'personalize-admin-enumerated-item';
  $text = '<div' . drupal_attributes($variables['attributes']) . '><span class="personalize-admin-enumerated-counter">' . $variables['enum'] . '</span> ';
  if (!empty($variables['title_prefix'])) {
    $text .= $variables['title_prefix'];
  }
  $text .= $variables['title'];
  if (!empty($variables['title_suffix'])) {
    $text .= $variables['title_suffix'];
  }
  $text .= '</div>';
  return $text;
}

/**
 * Theme wrapper for an administrative option that displays optional
 * details beyond the base description.
 *
 * @param $variables
 *   Associative array of variables for the theme.
 *   - details: The optional details that should only be shown when requested.
 *
 * @ingroup themeable
 */
function theme_personalize_admin_info_details($variables) {
  if (!empty($variables['details'])) {
    return '<div class="personalize-admin-optional">' . $variables['details'] . '</div>';
  }
  return '';
}

/********************************************************
 *
 * C A M P A I G N  W I Z A R D
 *
 *******************************************************/

/**
 * Theme function to output the navigation for the personalize campaign wizard.
 *
 * @param array $variables
 *   - element:  The element to theme.  This element should have:
 *     - #navigation: An array of subform navigation options with keyed by the
 *       section name with the label to display as the value.
 *     - #navigation_url: The base path for navigating to this wizard.  The
 *       navigation section names will be appended to generate a url.
 *     - #current: Indicates the current step
 *     - actions: Renderable array for the action buttons
 *     - message: Renderable array for the shorter instructional message.
 *     - help: Renderable array of section help details.
 *
 * @ingroup themeable
 */
function theme_personalize_wizard_process_bar($variables) {
  if (empty($variables['element'])) {
    return;
  }
  $element = $variables['element'];
  $navigation = empty($element['navigation']) ? array() : $element['navigation'];
  $current = empty($element['#current']) ? '' : $element['#current'];
  $actions = empty($element['actions']) ? '' : drupal_render($element['actions']);
  $message = empty($element['message']) ? '' : drupal_render($element['message']);
  $help = empty($element['help']) ? '' : drupal_render($element['help']);
  $base_path = $element['#base_url'];
  if (substr($base_path, -1) != '/') {
    $base_path .= '/';
  }

  $output = '';
  $output .= '<div id="personalize-wizard-process-bar">';
  // Subform navigation.
  $classes = empty($navigation['#attributes']['class']) ? array() : $navigation['#attributes']['class'];
  $classes[] = 'clearfix';
  $output .= '<div class="' . implode(' ', $classes) . '">';
  if (!empty($navigation)) {
    $output .= '<ul>';
    foreach ($navigation['#options'] as $key => $option) {
      $option_url = $base_path . $key;
      $class = array('personalize-wizard-navigation-item');
      $title = empty($option['description']) ? $option['label'] : $option['description'];
      $link_attributes = array(
        'data-personalize-section' => $key,
        'title' => $title,
      );
      if ($current === $key) {
        $class[] = 'personalize-wizard-navigation-current';
        $link_attributes['class'] = array('active');
      }
      else if (!empty($option['disabled'])) {
        $class[] = 'personalize-wizard-navigation-disabled';
      }
      $output .= '<li class="' . implode(' ', $class) . '">';
      // We avoid using the l() function here, because l() automatically adds an
      // "active" class to the link, which misrepresents in UI.
      $output .= '<a href="' . check_plain(url($option_url)) . '"' . drupal_attributes($link_attributes) . '>' . check_plain($option['label']) . '</a>';
      $output .= '</li>';
    }
    $output .= '</ul>';
    // Action buttons (status, etc).
    if (!empty($actions)) {
      $output .= $actions;
    }
    $output .= '</div>';
  }
  // Help
  if (!empty($message) || !empty($help)) {
    $output .= '<div class="personalize-wizard-section-help">';
    $output .= '<div class="personalize-wizard-instructions">' . $message;
    if (!empty($help)) {
      $output .= '<a href="" class="personalize-wizard-learn-more">' . t('Learn more') . '</a>';
    }
    $output .= '</div>';
    $output .= '<div class="personalize-wizard-expanded-help">' . $help . '</div>';
    $output .= '</div>';
  }
  $output .= '</div>';
  return $output;
}

/**
 * Theme function to output a personalize wizard subform.
 *
 * This theme ensures that there is at least one personalize column on the page.
 * If there are to be multiple columns, then each column must be specified using
 * the personalize-wizard-column class on each container.  If no columns are
 * specified then this theme function will add a single default column for
 * all children.
 *
 * @param array $variables
 *   - element: The element to theme.  This element should be a container.
 *
 * @ingroup themeable
 */
function theme_personalize_wizard_section($variables) {
  $output = '';
  $element = $variables['element'];
  $element['#attributes']['class'][] = 'personalize-wizard-section';
  $element['#attributes']['class'][] = 'clearfix';
  // Find out if the element has columns already defined.  If it doesn't
  // then wrap the section content in a column container.
  $has_column = $title_child = FALSE;
  $child_keys = element_children($element);
  foreach ($child_keys as $child) {
    if (!empty($element[$child]['#attributes']['class']) && in_array('personalize-wizard-column', $element[$child]['#attributes']['class'])) {
      $has_column = TRUE;
    }
    if (!empty($element[$child]['#theme']) && $element[$child]['#theme'] === 'personalize_wizard_section_title') {
      $title_child = $child;
    }
    // No reason to keep looping if all of the information has been found.
    if ($has_column && !empty($title_child)) {
      break;
    }
  }
  if ($has_column) {
    unset($element['#theme']);
    return drupal_render($element);
  }
  else {
    if (!empty($element['#attributes']['id'])) {
      unset($element['#attributes']['id']);
    }
    $output .= '<div ' . drupal_attributes($element['#attributes']) . '>';
    if (!empty($title_child)) {
      $output .= drupal_render($element[$title_child]);
      unset($element[$title_child]);
    }
    $output .= '<div class="personalize-wizard-column">';
    $output .= drupal_render_children($element);
    $output .= '</div></div>';
  }
  return $output;
}

/**
 * Theme function to output the header section for a personalize campaign
 * wizard sub-form.
 *
 * @param array $variables
 *   - element:  The element to theme.  This element should have a #title
 *     attribute and may have children to include within the header.
 *
 * @ingroup themeable
 */
function theme_personalize_wizard_section_title($variables) {
  $output = '';
  if (empty($variables['element']) || empty($variables['element']['#title'])) {
    return $output;
  }
  $element = $variables['element'];
  $output .= '<div class="personalize-wizard-section-title"><h2>' . $element['#title'] . '</h2>';
  $output .= drupal_render_children($element);
  $output .=  '</div>';
  if (!empty($element['#instructions'])) {
    $output .= '<div class="personalize-wizard-section-instructions">' . $element['#instructions'] . '</div>';
  }
  return $output;
}

/**
 * Theme function to output header information for a variation set.
 *
 * @param $variables
 *   An associative array of variables.
 *   - variation_title: the title of the variation set.
 *   - variation_count: the number of variations within the option set.
 *   - report: an array of reporting statistics for this option set.
 *   - report_link: the link to the report for this option set.
 *   - edit_link: the link to edit the option set.
 *   - delete_link: the link to delete the option set.
 * @return string
 *   The themed output.
 *
 * @ingroup themeable
 */
function theme_personalize_wizard_variations_header($variables) {
  $output = '';
  $output .= '<h3>';
  $output .= $variables['variation_title'] . ' ' . format_plural($variables['variation_count'], '1 variation', '@count variations');
  $output .= '</h3>';
  if (!empty($variables['edit_link'])) {
    $output .= '<span class="personalize-option-header-item">' . $variables['edit_link'] . '</span>';
  }
  if (!empty($variables['delete_link'])) {
    $output .= '<span class="personalize-option-header-item">' . $variables['delete_link'] . '</span>';
  }
  $output = '<span class="personalize-option-header">' . $output . '</span>';
  return $output;
}

/**
 * Theme function to display a summary item on the campaign wizard "Review"
 * screen that indicates the count of an item.
 *
 * @param $variables
 *   Associative array of enumerated list item details
 *   - count: The main count to be displayed
 *   - details: Previously themed and cleaned output to display for each
 *     count.
 *
 * @ingroup themeable
 */
function theme_personalize_wizard_summary_count($variables) {
  $text = '';
  $text .= '<div class="personalize-wizard-summary-item clearfix">';
  $text .= '<span class="personalize-wizard-summary-count">' . $variables['count'] . '</span>';
  if (!empty($variables['details'])) {
    $text .= '<div class="personalize-wizard-summary-details">' . $variables['details'] . '</div>';
  }
  $text .= '</div>';
  return $text;
}

/**
 * Theme function to display a summary item that shows the scheduling component
 * for a particular agent.
 *
 * @param $variables
 *   Associative array of enumerated list item details
 *   - start_date: The timestamp to start the campaign or 0 if not indicated.
 *   - stop_date: The timestamp to stop the campaign or 0 if not indicated.
 *   - started_date: The timestamp that the campaign was started last or 0 if
 *     never started.
 *
 * @ingroup themeable
 */
function theme_personalize_wizard_summary_schedule($variables) {
  $text = '';
  $text .= '<div class="personalize-wizard-summary-schedule">';
  if (!empty($variables['started_date'])) {
    $text .= '<div class="personalize-wizard-summary-started"><strong>' . t('Actual start:') . ' </strong>' . format_date($variables['started_date'], 'custom', 'M. j, Y') . '</div>';
  }
  if (!empty($variables['start_date'])) {
    $text .= '<div class="personalize-wizard-summary-start"><strong>' . t('Scheduled start: ') . ' </strong>' . format_date($variables['start_date'], 'custom', 'M. j, Y') . '</div>';
  }
  if (!empty($variables['stop_date'])) {
    $text .= '<div class="personalize-wizard-summary-stop"><strong>' . t('Scheduled stop: ') . ' </strong>' . format_date($variables['stop_date'], 'custom', 'M. j, Y') . '</div>';
  }
  $text .= '</div>';
  return $text;
}
