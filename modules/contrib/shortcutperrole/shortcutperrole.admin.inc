<?php

/**
 * @file
 * Admin page callbacks for the Shortcut per Role module.
 */

/**
 * Process callback for the admin page
 *
 * @return form
 */
function shortcutperrole_admin_form() {
  $form['#tree'] = TRUE;
  $roles = user_roles();
  $sets = shortcut_sets();
  $options = array();
  foreach ($sets as $name => $set) {
    $options[$name] = check_plain($set->title);
  }
  $ss_per_role = variable_get('shortcutperrole', array());
  $form['title'] = array(
    '#markup' => '<h3 class="title">' . t('Assign Shortcuts for  Role:') .
    '</h3>',
  );
  foreach ($roles as $rid => $role) {
    $default_value_ss= array_key_exists($rid, $ss_per_role) ?
    $ss_per_role[$rid] : 'shortcut_set_1' ;
    $form['shortcutperrole'] [$rid ] = array(
      '#type' => 'select',
      '#default_value' => $default_value_ss,
      '#options' =>  $options,
      '#prefix' => '<b>' . $role . '</b>' ,
    );
  }
  $form['create_new_ss'] = array(
    '#markup' => '<h3 class="title">' . l(t('Add a shortcut set'),
    'admin/config/user-interface/shortcut/add-set') . '</h3>',
  );
  $form['#submit'] = array('shortcutperrole_admin_form_submit');
   return system_settings_form($form);
}

function shortcutperrole_admin_form_submit($form, &$form_state) {
  variable_set('shortcutperrole', $form_state['values']['shortcutperrole']);
}
