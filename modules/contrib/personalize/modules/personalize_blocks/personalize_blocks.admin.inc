<?php
/**
 * @file
 * Provides the personalize_blocks administrative interface.
 */

/**
 * Page callback to list personalize_blocks instances in the system.
 */
function personalize_blocks_list() {
  $header = array(
    array('data' => t('Block')),
    array('data' => t('Operations'), 'colspan' => 3),
  );
  $rows = array();
  foreach (personalize_option_set_load_by_type('block') as $personalized_block) {
    $tablerow = array(
      array('data' => check_plain($personalized_block->data['block_title'])),
      array('data' => l(t('Edit'), 'admin/structure/personalize/variations/personalize-blocks/manage/'. $personalized_block->osid .'/edit')),
      array('data' => l(t('Clone'), 'admin/structure/personalize/variations/personalize-blocks/manage/'. $personalized_block->osid .'/clone')),
      array('data' => l(t('Delete'), 'admin/structure/personalize/variations/personalize-blocks/manage/'. $personalized_block->osid .'/delete')),
    );
    $rows[] = $tablerow;
  }
  if (empty($rows)) {
    $rows[] = array(array('data' => t('No personalize_blocks instances available.'), 'colspan' => 4));
  }
  $build = array(
    '#theme' => 'table',
    '#header' => $header,
    '#rows' => $rows,
    '#attributes' => array('id' => 'personalize_blocks'),
  );
  return $build;
}

/**
 * Build the personalize_blocks creation and edit form.
 */
function personalize_blocks_form($form, &$form_state, $formtype, $pblock = NULL) {
  if (!isset($pblock)) {
    if (!empty($form_state['pblock'])) {
      $pblock = $form_state['pblock'];
    }
    else {
      $pblock = new stdClass;
    }
  }
  // The contents of $pblock will either come from the db or from $form_state.
  if (isset($form_state['values']['title'])) {
    $pblock = _personalize_blocks_convert_form_to_personalized_block($form_state);
  }

  if (!empty($pblock->osid)) {
    $form['osid'] = array(
      '#type' => 'value',
      '#value' => $pblock->osid,
    );
    $form['agent_select'] = array(
      '#type' => 'value',
      '#value' => $pblock->agent,
    );
  }
  else {
    $current_agent = isset($pblock->agent) ? $pblock->agent : '';
    $form += personalize_get_agent_selection_form($current_agent, TRUE, TRUE);
  }

  $path = drupal_get_path('module', 'personalize_blocks');
  $form['#attached']['js'][] = $path . '/js/personalize_blocks.admin.js';
  $form['#attached']['css'][] = $path . '/css/personalize_blocks.admin.css';

  $form['title'] = array(
    '#title' => t('Title'),
    '#description' => t('This will be the administrative title of the block.'),
    '#type' => 'textfield',
    '#default_value' => isset($pblock->data['block_title']) ? $pblock->data['block_title'] : '',
    '#weight' => -100,
    '#required' => TRUE,
  );

  $form['options'] = array(
    '#tree' => TRUE,
    '#theme_wrappers' => array('container'),
    '#attributes' => array('id' => 'options-replace'),
  );

  // Add a wrapper for the blocks and Add Another Tab button.
  $form['pblock_wrapper'] = array(
    '#tree' => FALSE,
    '#theme_wrappers' => array('fieldset'),
    '#title' => t('Variations'),
    '#attributes' => array('id' => 'personalize-blocks-wrapper'),
    '#weight' => -3,
  );

  $form['pblock_wrapper']['blocks'] = array(
    '#tree' => TRUE,
    '#theme_wrappers' => array('container'),
    '#attributes' => array('id' => 'personalized-block-variations'),
    '#theme' => empty($pblock->osid) ? 'personalize_blocks_admin_form_draggable_blocks' : 'personalize_blocks_admin_form_blocks',
  );

  $form['pblock_wrapper']['blocks_more'] = array(
    '#type' => 'submit',
    '#value' => t('Add another'),
    '#attributes' => array('class' => array('personalize-blocks-add-block'), 'title' => t('Click here to add more blocks.')),
    '#weight' => 1,
    '#submit' => array('personalize_blocks_more_blocks_submit'),
    '#ajax' => array(
      'callback' => 'personalize_blocks_ajax_callback',
      'wrapper' => 'personalized-block-variations',
      'effect' => 'fade',
    ),
    '#limit_validation_errors' => array(),
  );

  // If creating a new personalize_blocks instance, start off with 2 empty blocks.
  if (empty($pblock->options)) {
    $pblock->options = array(
      0 => array('option_label' => personalize_generate_option_label(0)),
      1 => array('option_label' => personalize_generate_option_label(1)),
    );
    if (isset($_GET['original_block'])) {
      // Use the original block as the first option in our new
      // option set.
      $query = db_select('block', 'b')
        ->fields('b', array('module', 'delta'))
        ->condition('bid', $_GET['original_block']);
      $result = $query->execute()->fetchAssoc();
      $pblock->options[0]['bid'] = $result['module'] . '_delta_' . $result['delta'];
    }
  }

  // If the "Add another" button was clicked, we need to increment the number of
  // blocks by one.
  $num_blocks = count($pblock->options);
  $num_blocks_requested = isset($form_state['num_blocks']) ? $form_state['num_blocks'] : $num_blocks;
  while ($num_blocks < $num_blocks_requested) {
    $pblock->options[] = array('option_label' => personalize_generate_option_label($num_blocks));
    $num_blocks++;
  }
  $form_state['num_blocks'] = $num_blocks;
  // Keep track of block options throughout Ajax submits.
  $form_state['pblock'] =  $pblock;

  // If the "Remove" button was clicked for a block, we need to remove that block
  // from the form.
  if (isset($form_state['to_remove'])) {
    unset($pblock->options[$form_state['to_remove']]);
    unset($form_state['to_remove']);
    $form_state['num_blocks']--;
  }

  // Add current tabs to the form.
  foreach ($pblock->options as $delta => $block) {
    $block['delta'] = $delta;
    $form['pblock_wrapper']['blocks'][$delta] = _personalize_blocks_form($block, $pblock);
  }
  // If there's only one block, it shouldn't be removeable.
  if (count($pblock->options) == 1) {
    $form['pblock_wrapper']['blocks'][$delta]['remove']['#access'] = FALSE;
  }

  $form['original_block'] = array(
    '#type' => 'value',
    '#value' => isset($_GET['original_block']) ? $_GET['original_block'] : NULL,
  );

  $form['actions'] = array('#type' => 'actions');
  $form['actions']['submit_form'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
  );
  return $form;
}

/*
 * Build one row (one block) on the personalized_blocks admin form.
 *
 * @param array $block
 *  An array containing the details of this particular block.
 *
 * @param object $pblock
 *  An object representing the personalize_blocks instance that the blocks are
 *  being built for.
 */
function _personalize_blocks_form(array $block, $pblock) {
  $form['#tree'] = TRUE;
  $delta = $block['delta'];

  $form['option_label'] = array(
    '#type' => 'textfield',
    '#size' => '10',
    '#default_value' => isset($block['option_label']) ? $block['option_label'] : '',
  );
  $form['weight'] = array(
    '#type' => 'weight',
    '#default_value' => isset($block['weight']) ? $block['weight'] : $delta-100,
    '#delta' => 100,
  );
  $default = '';
  if (isset($block['bid'])) {
    $default = $block['bid'];
  }
  elseif (isset($_GET['personalize_blocks_new_block'])) {
    $default = 'block_delta_' . $_GET['personalize_blocks_new_block'];
    unset($_GET['personalize_blocks_new_block']);
  }

  $form['block'] = array(
    '#type' => 'container',
  );

  $select_class = 'personalize-blocks-select-' . $delta;
  $form['block']['block_type'] = array(
    '#type' => 'radios',
    '#options' => array(
      'select' => t('Select existing block'),
      'add' => t('Create new block'),
    ),
    '#attributes' => array(
      'class' => array('personalize-blocks-inline', $select_class),
    ),
    '#default_value' => 'select',
  );
  $form['block']['bid'] = array(
    '#type' => 'select',
    '#options' => personalize_blocks_get_blocks(),
    '#empty_option' => t('Select a block...'),
    '#default_value' => $default,
    '#title' => t('Select a block'),
    '#title_display' => 'invisible',
    '#states' => array(
      'visible' => array(
        ':input.' . $select_class => array('value' => 'select'),
      ),
    ),
  );

  $form['block']['add'] = array(
    '#type' => 'fieldset',
    '#title' => t('Add a new block'),
    '#states' => array(
      'visible' => array(
        ':input.' . $select_class => array('value' => 'add'),
      ),
    ),
  );
  $form['block']['add'] += _personalize_blocks_add_block_form($form_state);

  $form['remove'] = array(
    '#type' => 'submit',
    '#tag' => 'button',
    '#text' => t('Remove'),
    '#value' => 'remove_' . $delta,
    '#theme_wrappers' => array('personalize_html_tag'),
    '#attributes' => array(
      // The ID is necessary for the AJAX replace to function correctly. It
      // is fragile to declare it like this, essentially hard-coding the
      // #parents, but I know of no other way to do this programmatically.
      'id' => 'edit-blocks-' . $delta . '-remove',
      'class' => array('personalize-blocks-delete-block', 'form-submit',),
      'title' => t('Remove the block.'),
    ),

    '#submit' => array('personalize_blocks_remove_block_submit'),
    '#ajax' => array(
      'callback' => 'personalize_blocks_ajax_callback',
      'wrapper' => 'personalized-block-variations',
      'method' => 'replace',
      'effect' => 'fade',
    ),
    '#limit_validation_errors' => array(),
  );
  if (isset($block['option_id'])) {
    $form['option_id'] = array(
      '#type' => 'value',
      '#value' => $block['option_id'],
    );
  }
  else {
    $form['option_label']['#attributes']['class'][] = 'personalize-blocks-add-option-label';
    $form['block']['bid']['#attributes']['class'][] = 'personalize-blocks-add-block-select';
  }
  return $form;
}

/**
 * Attaches a form to add a new block.
 */
function _personalize_blocks_add_block_form(&$form_state) {
  $form = array();

  // Recreating only the relevant portions of the custom block form.
  // Make the info and body fields "look" required, but don't actually make
  // them required or all add block forms will cause validation to fail (even
  // when an existing block is selected to use).
  // @see personalize_blocks_form_validate().
  $form['title'] = array(
    '#type' => 'textfield',
    '#title' => t('Block title'),
    '#maxlength' => 64,
    '#description' => t('The title of the block as shown to the user.'),
    '#weight' => -19,
  );
  $form['info'] = array(
    '#type' => 'textfield',
    '#title' => t('Block description') . ' <span class="form-required" title="This field is required.">*</span>',
    '#maxlength' => 64,
    '#description' => t('A brief description of your block. Used on the <a href="@overview">Blocks administration page</a>.', array('@overview' => url('admin/structure/block'))),
    '#weight' => -18,
    '#attributes' => array(
      'class' => array('personalize-blocks-add-block-info'),
    ),
  );
  $form['body'] = array(
    '#type' => 'text_format',
    '#title' => t('Block body') . ' <span class="form-required" title="This field is required.">*</span>',
    '#rows' => 15,
    '#description' => t('The content of the block as shown to the user.'),
    '#weight' => -17,
  );
  return $form;
}

/**
 * Theme function for personalize_blocks edit page.
 * Theme the form elements for the blocks as table rows.
 *
 * @ingroup themeable
 */
function theme_personalize_blocks_admin_form_blocks($variables) {
  $has_operations = FALSE;
  $blocks = $variables['blocks'];
  $draggable = isset($blocks['#draggable']) && $blocks['#draggable'];
  $rows = array();
  $header = array(
    t('Variation label'),
  );
  if ($draggable) {
    drupal_add_tabledrag('personalize-blocks-table', 'order', 'sibling', 'personalized-blocks-weight');
    $header[] = t('Weight');
  }
  $header = array_merge($header, array(
    t('Block'),
    t('Operations'),
  ));
  foreach (element_children($blocks) as $key) {
    $block = &$blocks[$key];
    if ($draggable) {
      $block['weight']['#attributes']['class'] = array('personalized-blocks-weight');
    }

    // tab settings fields
    $fields = array(
      array('data' => drupal_render($block['option_label']), 'class' => array('personalized-block-option-name')),
    );

    if ($draggable) {
      $fields[] = array('data' => drupal_render($block['weight']), 'class' => array('personalized-blocks-weight'));
    }

    if (!isset($block['remove']['#access']) || $block['remove']['#access'] === TRUE) {
      $has_operations = TRUE;
    }

    $fields = array_merge($fields, array(
      array('data' => drupal_render($block['block']), 'class' => array('personalized-block-block')),
      array('data' => drupal_render($block['remove']), 'class' => array('personalize-block-remove'))
    ));

    // Build the table row.
    $row = array(
      'data' => $fields,
      'class' => $draggable ? array('draggable') : array(),
    );

    // Add additional attributes to the row, such as a class for this row.
    if (isset($block['#attributes'])) {
      $row = array_merge($row, $block['#attributes']);
    }
    $rows[] = $row;
  }

  // If there were never any operations available then remove the column.
  if (!$has_operations) {
    array_pop($header);
    foreach ($rows as &$row) {
      array_pop($row['data']);
    }
  }

  $build['personalized_block'] = array(
    '#theme' => 'table',
    '#sticky' => FALSE,
    '#header' => $header,
    '#rows' => $rows,
    '#attributes' => array('id' => 'personalize-blocks-table'),
    '#weight' => -1,
  );

  $output = drupal_render($build);
  return $output;
}

/**
 * Theme function for personalize_blocks add page.
 * Theme the form elements for the blocks as draggable table rows.
 *
 * @ingroup themeable
 */
function theme_personalize_blocks_admin_form_draggable_blocks($variables) {
  $variables['blocks']['#draggable'] = 1;
  return theme('personalize_blocks_admin_form_blocks', $variables);
}

/**
 * Ajax callback for the add tab and remove tab buttons.
 */
function personalize_blocks_ajax_callback($form, $form_state) {
  // Clear messages since they don't need to be repeated within the AJAX
  // callback area.
  drupal_get_messages();

  return $form['pblock_wrapper']['blocks'];
}

/**
 * Submit handler for the "Add Tab" button.
 */
function personalize_blocks_more_blocks_submit($form, &$form_state) {
  // Increment the number of blocks to be rendered.
  $form_state['num_blocks']++;
  $form_state['rebuild'] = TRUE;
}

/**
 * Submit handler for the "Remove Tab" button.
 */
function personalize_blocks_remove_block_submit($form, &$form_state) {
  // Get the tab delta for the clicked button.
  $delta = $form_state['clicked_button']['#parents'][1];
  $form_state['to_remove'] = $delta;
  $form_state['rebuild'] = TRUE;
}

/**
 * Validation handler for personalize_blocks admin page.
 */
function personalize_blocks_form_validate($form, &$form_state) {
  $valid = FALSE;
  if (isset($form_state['values']['blocks'])) {
    $num_blocks = 0;
    foreach ($form_state['values']['blocks'] as $j => $block) {
      if (empty($block['option_label'])) {
        form_set_error('blocks][' . $j . '][option_label', t('A label is required for each block option.'));
      }
      if ($block['block']['block_type'] === 'add') {
        if (empty($block['block']['add']['info'])) {
          form_set_error('blocks][' . $j . '][block][add][info', t('A block description is required when creating a new block.'));
        }
        if (empty($block['block']['add']['body']['value'])) {
          form_set_error('blocks][' . $j . '][block][add][body', t('The block body is required when creating a new block.'));
        }
        // @see block_add_block_form_validate().
        $custom_block_exists = (bool) db_query_range('SELECT 1 FROM {block_custom} WHERE info = :info', 0, 1, array(':info' => $block['block']['add']['info']))->fetchField();

        if ($custom_block_exists) {
          form_set_error('blocks][' . $j . '][block][add][info', t('Ensure that each block description is unique.'));
        }
        else {
          $num_blocks++;
        }
      }
      if (!empty($block['block']['bid'])) {
        $num_blocks++;
      }
    }
    $valid = $num_blocks > 1;
  }
  if (!$valid) {
    form_set_error('', t('You must add at least 2 blocks to your personalized block'));
  }

  if ($form_state['values']['agent_select'] == '__new__') {
    $agent_title = trim($form_state['values']['agent_basic_info']['title']);

    if (empty($agent_title)) {
      form_error($form['personalize']['agent_basic_info']['title'], t('Personalization name is required'));
    }
  }
}

/**
 * Submit handler for personalize_blocks admin page.
 */
function personalize_blocks_form_submit($form, &$form_state) {
  if ($form_state['clicked_button']['#id'] == 'edit-submit-form') {
    // Create any new blocks
    foreach($form_state['values']['blocks'] as $delta => &$block) {
      if ($block['block']['block_type'] === 'add') {
        $block['block']['bid'] = _personalize_blocks_add_custom_block($block['block']['add']);
      }
    }
    $pblock = _personalize_blocks_convert_form_to_personalized_block($form_state);
    // @todo This section should really be the responsibility of personalize module, figure
    //   out a way to not have to add this to every submit callback of a form that uses
    //   the embedded campaign creation form.
    if ($form_state['values']['agent_select'] == PERSONALIZE_NEW_AGENT_FORM_VALUE) {
      if ($new_agent = personalize_save_agent_from_form_values($form_state['values'])) {
        $pblock->agent = $new_agent->machine_name;
      }
      else {
        drupal_set_message('There was a problem saving the new personalization', 'error');
        return;
      }
    }
    personalize_option_set_save($pblock);
    if ($form_state['values']['original_block']) {
      personalize_blocks_replace_existing($form_state['values']['original_block'], $pblock);
    }
    else {
      $form_state['redirect'] = 'admin/structure/personalize/manage/' . $pblock->agent . '/variations';
    }
  }
}

/**
 * Deletion of personalized_block block.
 */
function personalize_blocks_block_delete($form, $form_state, $pblock) {
  $form['osid'] = array('#type' => 'hidden', '#value' => $pblock->osid);
  $form['title'] = array('#type' => 'hidden', '#value' => $pblock->data['block_title']);
  $form['agent'] = array('#type' => 'value', '#value' => $pblock->agent);
  return confirm_form($form, t('Are you sure you want to delete the personalized_block block %title?', array('%title' => $pblock->data['block_title'])), 'admin/structure/personalize/manage/' . $pblock->agent . '/variations', '', t('Delete'), t('Cancel'));
}

/**
 * Submit handler for personalized_block block deletion.
 */
function personalize_blocks_block_delete_submit($form, &$form_state) {
  personalize_option_set_delete($form_state['values']['osid']);
  drupal_set_message(t('The personalize_blocks instance %name has been removed.', array('%name' => $form_state['values']['title'])));
  cache_clear_all();
  $form_state['redirect'] = 'admin/structure/personalize/manage/' . $form_state['values']['agent'] . '/variations';
}

/**
 * Deletion of personalized_block block option from within an existing
 * personalized block.
 */
function personalize_blocks_block_option_delete($form, $form_state, $pblock, $option_id) {
  $option_label = $option_id;
  foreach($pblock->options as $option) {
    if ($option['option_id'] === $option_id) {
      $option_label = $option['option_label'];
      break;
    }
  }
  $form['osid'] = array('#type' => 'hidden', '#value' => $pblock->osid);
  $form['option_id'] = array('#type' => 'hidden', '#value' => $option_id);
  $form['block_title'] = array('#type' => 'hidden', '#value' => $pblock->data['block_title']);
  $form['option_label'] = array('#type' => 'hidden', '#value' => $option_label);
  return confirm_form($form, t('Are you sure you want to delete variation %option_label from the %title personalized block?', array('%option_label' => $option_label, '%title' => $pblock->data['block_title'])), 'admin/structure/personalize/variations/personalize-blocks/manage/' . $pblock->osid, '', t('Delete'), t('Cancel'));
}

/**
 * Submit handler for personalized_block block deletion.
 */
function personalize_blocks_block_option_delete_submit($form, &$form_state) {
  $option_set = personalize_option_set_load($form_state['values']['osid']);
  foreach($option_set->options as $i => $option) {
    if ($option['option_id'] === $form_state['values']['option_id']) {
      unset($option_set->options[$i]);
      break;
    }
  }
  personalize_option_set_save($option_set);
  drupal_set_message(t('The variation %option_label has been removed from the %block_title personalized block.', array('%option_label' => $form_state['values']['option_label'], '%block_title' => $form_state['values']['block_title'])));
  cache_clear_all();
  $form_state['redirect'] = 'admin/structure/personalize/manage/' . $option_set->agent . '/variations';
}

/**
 * Helper function to get all blocks.
 */
function personalize_blocks_get_blocks() {
  $blocksarray = &drupal_static(__FUNCTION__, array());
  if (empty($blocksarray)) {
    $blocks = _block_rehash();
    $blocksarray = array();
    foreach ($blocks as $block) {
      if ($block['module'] != 'personalize_blocks') {
        $key = $block['module'] . '_delta_' . $block['delta'];
        $blocksarray[$key] = $block['info'] . ' (' . $block['module'] . ':' . $block['delta'] . ')';
      }
    }
  }
  return $blocksarray;
}

/**
 * Helper function to convert the data on admin form into personalized_block presentation.
 *
 * @param $form_state
 *   The current form state to read data from.
 */
function _personalize_blocks_convert_form_to_personalized_block($form_state) {
  $formvalues_blocks = array();
  if (!empty($form_state['values']['blocks'])) {
    foreach ($form_state['values']['blocks'] as $i => $block) {
      $formvalues_blocks[$i] = $block;
      $weight[$i] = $block['weight'];
    }
    array_multisort($weight, SORT_ASC, $formvalues_blocks);
  }
  $options = array();
  foreach ($formvalues_blocks as $block) {
    if (empty($block['block']['bid'])) {
      continue;
    }
    $option = array(
      'option_label' => $block['option_label'],
      'bid' => $block['block']['bid'],
    );
    if (isset($block['option_id'])) {
      $option['option_id'] = $block['option_id'];
    }
    $options[] = $option;
  }
  $pblock = new stdClass();
  $pblock->plugin = 'block';
  $pblock->agent = $form_state['values']['agent_select'];
  $pblock->label = $form_state['values']['title'];
  $pblock->data = array('block_title' => $form_state['values']['title']);
  $pblock->options = $options;

  if (isset($form_state['values']['osid'])) {
    $pblock->osid = $form_state['values']['osid'];
  }

  return $pblock;
}

/**
 * Creates a new block for use within a personalized block.
 *
 * @param $block_values
 *   The values for this particular block from the form_state.
 *   These should include:
 *   - body: An array with body and format fields
 *   - info: The block description
 *   - title: The block title.
 * @return string
 *   The bid for the newly created block.
 */
function _personalize_blocks_add_custom_block($block_values) {
  // Create a new custom block.
  // @see block_add_form_submit().
  $delta = db_insert('block_custom')
    ->fields(array(
      'body' => $block_values['body']['value'],
      'info' => $block_values['info'],
      'format' => $block_values['body']['format'],
    ))
    ->execute();
  // Add the custom block for each theme.
  $query = db_insert('block')->fields(array('visibility', 'pages', 'custom', 'title', 'module', 'theme', 'status', 'weight', 'delta', 'cache'));
  foreach (list_themes() as $key => $theme) {
    if ($theme->status) {
      $query->values(array(
        'visibility' => 0,
        'pages' => '',
        'custom' => 0,
        'title' => $block_values['title'],
        'module' => 'block',
        'theme' => $theme->name,
        'status' => 0,
        'weight' => 0,
        'delta' => $delta,
        'cache' => DRUPAL_NO_CACHE,
      ));
    }
  }
  $query->execute();
  drupal_set_message(t('The block %title has been created.', array('%title' => $block_values['info'])));
  return 'block_delta_' . $delta;
}
