<?php

/**
 * Plugins are described by creating a $plugin array which will be used
 * by the system that includes this file.
 */

$plugin = array(
  'title' => t('Create User'),
  'description' => t('Allows the creation of a user.  This circumvents the typical drupal access controls on user creation.'),
  'content form' => 'context_admin_user_create_menu_content_form',
  'content form submit' => 'context_admin_user_create_menu_content_form_submit',
  'render' => 'context_admin_user_create_menu_render_page',
  'form alter' => 'context_admin_user_create_menu_form_alter',
);


function context_admin_user_create_menu_content_form($form, $form_state) {
  $conf = $form_state['conf'];
  $default = isset($conf['context_admin_user_roles']) ? $conf['context_admin_user_roles'] : NULL;
  $default_alter = isset($conf['context_admin_user_roles_alter']) ? $conf['context_admin_user_roles_alter'] : NULL;
  $default_limited = isset($conf['context_admin_user_roles_limited']) ? $conf['context_admin_user_roles_limited'] : NULL;
  $default_status = isset($conf['context_admin_user_status']) ? $conf['context_admin_user_status'] : NULL;
  $default_status_alter = isset($conf['context_admin_user_status_alter']) ? $conf['context_admin_user_status_alter'] : NULL;
  $default_help = isset($conf['context_admin_user_registration_help']) ? $conf['context_admin_user_registration_help'] : NULL;
  $default_notify = isset($conf['context_admin_user_notify']) ? $conf['context_admin_user_notify'] : NULL;

  $roles = user_roles(TRUE);
  $options = array();
  foreach ($roles as $key => $role) {
    $options[$key] = $role;
  }
  $form['context_admin_user_roles'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Default Roles'),
    '#description' => t('Select the default role(s) for users created from this administration, if necessary.'),
    '#options' => $options,
    '#default_value' => is_array($default) ? $default : array($default),
  );
  $form['context_admin_user_roles_alter'] = array(
    '#type' => 'radios',
    '#title' => t('Alter Roles'),
    '#options' => array(
      'disallow' => t('Disallow'),
      'any' => t('Any'),
      'limited' => t('Limited'),
    ),
    '#default_value' => $default_alter,
  );
  $form['context_admin_user_roles_alter']['disallow']['#description'] = t('Do not allow users of this form to set roles.');
  $form['context_admin_user_roles_alter']['any']['#description'] = t('Allow users of this form to set any role. *NOTE: This is HUGELY dangerous and will override the limit roles list. Use with care.');
  $form['context_admin_user_roles_alter']['limited']['#description'] = t('Allow users of this form to set only the roles that have been selected.');
  $form['context_admin_user_roles_limited'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Limited Roles'),
    '#description' => t('Select the roles to show when using this form.'),
    '#options' => $options,
    '#default_value' => is_array($default_limited) ? $default_limited : array($default_limited),
    '#dependency' => array('radio:context_admin_user_roles_alter' => array('limited')),
  );
  $form['context_admin_user_status'] = array(
    '#type' => 'radios',
    '#title' => 'Default Status',
    '#default_value' => 1,
    '#options' => array(
      0 => 'Blocked',
      1 => 'Active',
    ),
    '#default_value' => isset($default_status) ? $default_status : 1,
    '#description' => t('Set the default user status for users created from this form.'),
  );
  $form['context_admin_user_status_alter'] = array(
    '#type' => 'checkbox',
    '#title' => t('Alter Status'),
    '#description' => t('Allow users of this form to determine if users will be active or blocked when created.'),
    '#default_value' => $default_status_alter,
  );
  /*$form['context_admin_user_registration_help'] = array(
    '#type' => 'checkbox',
    '#title' => t('Registration Help'),
    '#description' => t('Includes the User Registration Help for this instance of user registration.'),
    '#default_value' => $default_help,
  );*/
  $form['context_admin_user_notify'] = array(
    '#type' => 'checkbox',
    '#title' => t('User Notification'),
    '#description' => t('Include the User Notification option.'),
    '#default_value' => $default_notify,
  );
  return $form;
}

function context_admin_user_create_menu_content_form_submit($form, &$form_state) {
  $form_state['conf']['context_admin_user_roles'] = $form_state['values']['context_admin_user_roles'];
  $form_state['conf']['context_admin_user_roles_alter'] = $form_state['values']['context_admin_user_roles_alter'];
  $form_state['conf']['context_admin_user_roles_limited'] = $form_state['values']['context_admin_user_roles_limited'];
  $form_state['conf']['context_admin_user_status'] = $form_state['values']['context_admin_user_status'];
  $form_state['conf']['context_admin_user_status_alter'] = $form_state['values']['context_admin_user_status_alter'];
  //$form_state['conf']['context_admin_user_registration_help'] = $form_state['values']['context_admin_user_registration_help'];
  $form_state['conf']['context_admin_user_notify'] = $form_state['values']['context_admin_user_notify'];
}

function context_admin_user_create_menu_render_page($handler, $base_contexts, $args, $test = TRUE) {
  return drupal_get_form('context_admin_user_create_menu_render_form');
}

function context_admin_user_create_menu_render_form($form, $form_state) {
  $form['#user'] = drupal_anonymous_user();
  $form['#user_category'] = 'register';

  $form['#attached']['library'][] = array('system', 'jquery.cookie');
  $form['#attributes']['class'][] = 'user-info-from-cookie';

  // Start with the default user account fields.
  context_admin_user_edit_form($form, $form_state);

  // Attach field widgets, and hide the ones where the 'user_register_form'
  // setting is not on.
  field_attach_form('user', $form['#user'], $form, $form_state);
  foreach (field_info_instances('user', 'user') as $field_name => $instance) {
    if (empty($instance['settings']['user_register_form'])) {
      $form[$field_name]['#access'] = FALSE;
    }
  }

  $form['actions'] = array('#type' => 'actions');
  $form['actions']['submit'] = array(
    '#type' => 'submit', 
    '#value' => t('Create new account'),
  );

  $form['#validate'][] = 'user_register_validate';
  // Add the final user registration form submit handler.
  $form['#submit'][] = 'user_register_submit';

  return $form;
}

function context_admin_user_create_menu_render_form_submit($form, &$form_state) {
  global $base_url;
  $page = page_manager_get_current_page();

  $mail = $form_state['values']['mail'];
  $name = $form_state['values']['name'];
  if (!$form_state['#handler']->conf['exclude_password']) {
    $pass = $form_state['values']['pass'];
  }
  else {
    $pass = user_password();
  };
  $notify = isset($form_state['values']['notify']) ? $form_state['values']['notify'] : NULL;
  $from = variable_get('site_mail', ini_get('sendmail_from'));
  if (isset($form_state['values']['roles']) || isset($page['handler']->conf['context_admin_user_roles'])) {
    // Remove unset roles.
    if (isset($form_state['values']['roles'])) {
      $roles = array_filter($form_state['values']['roles']);
    }
    if (isset($page['handler']->conf['context_admin_user_roles'])) {
      $roles += array_filter($page['handler']->conf['context_admin_user_roles']);
    }
  }
  else {
    $roles = array();
  }
  // The unset below is needed to prevent these form values from being saved as
  // user data.
  unset($form_state['values']['form_token'], $form_state['values']['submit'], $form_state['values']['op'], $form_state['values']['notify'], $form_state['values']['form_id'], $form_state['values']['affiliates'], $form_state['values']['destination']);

  $merge_data = array('pass' => $pass, 'init' => $mail, 'roles' => $roles);
  if (!$page['handler']->conf['context_admin_user_status_alter']) {
    // Set the user's status because it was not displayed in the form.
    $merge_data['status'] = $page['handler']->conf['context_admin_user_status'] == 1;
  }
  $account = user_save('', array_merge($form_state['values'], $merge_data));
  // Terminate if an error occured during user_save().
  if (!$account) {
    drupal_set_message(t("Error saving user account."), 'error');
    $form_state['redirect'] = '';
    return;
  }
  $form_state['user'] = $account;

  watchdog('user', 'New user: %name (%email).', array('%name' => $name, '%email' => $mail), WATCHDOG_NOTICE, l(t('edit'), 'user/'. $account->uid .'/edit'));
  
  // Add plain text password into user account to generate mail tokens.
  $account->password = $pass;
  if (!$notify) {
    drupal_set_message(t('Created a new user account for <a href="@url">%name</a>. No e-mail has been sent.', array('@url' => url("user/$account->uid"), '%name' => $account->name)));
  }
  else if ($account->status || $notify) {
    // Create new user account, no administrator approval required.
    $op = $notify ? 'register_admin_created' : 'register_no_approval_required';
    _user_mail_notify($op, $account);
    if ($notify) {
      drupal_set_message(t('Password and further instructions have been e-mailed to the new user <a href="@url">%name</a>.', array('@url' => url("user/$account->uid"), '%name' => $account->name)));
    }
    else {
      drupal_set_message(t('Your password and further instructions have been sent to your e-mail address.'));
      $form_state['redirect'] = '';
      return;
    }
  }
  else {
    // Create new user account, administrator approval required.
    _user_mail_notify('register_pending_approval', $account);
    drupal_set_message(t('Thank you for applying for an account. Your account is currently pending approval by the site administrator.<br />In the meantime, a welcome message with further instructions has been sent to your e-mail address.'));
    $form_state['redirect'] = '';
    return;
  }
}

function context_admin_user_edit_form(&$form, &$form_state) {
  $account = $form['#user'];
  $register = ($form['#user']->uid > 0 ? FALSE : TRUE);

  $form['#validate'][] = 'user_account_form_validate';

  // Account information.
  $form['account'] = array(
    '#weight' => -10,
  );
  // Only show name field on registration form or user can change own username.
  $form['account']['name'] = array(
    '#type' => 'textfield', 
    '#title' => t('Username'), 
    '#maxlength' => USERNAME_MAX_LENGTH, 
    '#description' => t('Spaces are allowed; punctuation is not allowed except for periods, hyphens, apostrophes, and underscores.'), 
    '#required' => TRUE, 
    '#attributes' => array('class' => array('username')), 
    '#default_value' => (!$register ? $account->name : ''), 
    '#access' => ($register || ($user->uid == $account->uid && user_access('change own username')) || $admin), 
    '#weight' => -10,
  );

  $form['account']['mail'] = array(
    '#type' => 'textfield', 
    '#title' => t('E-mail address'), 
    '#maxlength' => EMAIL_MAX_LENGTH, 
    '#description' => t('A valid e-mail address. All e-mails from the system will be sent to this address. The e-mail address is not made public and will only be used if you wish to receive a new password or wish to receive certain news or notifications by e-mail.'), 
    '#required' => TRUE, 
    '#default_value' => (!$register ? $account->mail : ''),
  );

  // Display password field only for existing users or when user is allowed to
  // assign a password during registration.
  if (!$register) {
    $form['account']['pass'] = array(
      '#type' => 'password_confirm', 
      '#size' => 25, 
      '#description' => t('To change the current user password, enter the new password in both fields.'),
    );
    // To skip the current password field, the user must have logged in via a
    // one-time link and have the token in the URL.
    $pass_reset = isset($_SESSION['pass_reset_' . $account->uid]) && isset($_GET['pass-reset-token']) && ($_GET['pass-reset-token'] == $_SESSION['pass_reset_' . $account->uid]);
    $protected_values = array();
    $current_pass_description = '';
    // The user may only change their own password without their current
    // password if they logged in via a one-time login link.
    if (!$pass_reset) {
      $protected_values['mail'] = $form['account']['mail']['#title'];
      $protected_values['pass'] = t('Password');
      $request_new = l(t('Request new password'), 'user/password', array('attributes' => array('title' => t('Request new password via e-mail.'))));
      $current_pass_description = t('Enter your current password to change the %mail or %pass. !request_new.', array('%mail' => $protected_values['mail'], '%pass' => $protected_values['pass'], '!request_new' => $request_new));
    }
    // The user must enter their current password to change to a new one.
    if ($user->uid == $account->uid) {
      $form['account']['current_pass_required_values'] = array(
        '#type' => 'value', 
        '#value' => $protected_values,
      );
      $form['account']['current_pass'] = array(
        '#type' => 'password', 
        '#title' => t('Current password'), 
        '#size' => 25, 
        '#access' => !empty($protected_values), 
        '#description' => $current_pass_description, 
        '#weight' => -5,
      );
      $form['#validate'][] = 'user_validate_current_pass';
    }
  }
  $form['account']['pass'] = array(
    '#type' => 'password_confirm', 
    '#size' => 25, 
    '#description' => t('Provide a password for the new account in both fields.'), 
    '#required' => TRUE,
  );

  $form['account']['status'] = array(
    '#type' => 'radios', 
    '#title' => t('Status'), 
    '#options' => array(t('Blocked'), t('Active')), 
  );

  $roles = array_map('check_plain', user_roles(TRUE));
  // The disabled checkbox subelement for the 'authenticated user' role
  // must be generated separately and added to the checkboxes element,
  // because of a limitation in Form API not supporting a single disabled
  // checkbox within a set of checkboxes.
  // @todo This should be solved more elegantly. See issue #119038.
  $checkbox_authenticated = array(
    '#type' => 'checkbox', 
    '#title' => $roles[DRUPAL_AUTHENTICATED_RID], 
    '#default_value' => TRUE, 
    '#disabled' => TRUE,
  );
  unset($roles[DRUPAL_AUTHENTICATED_RID]);
  $form['account']['roles'] = array(
    '#type' => 'checkboxes', 
    '#title' => t('Roles'), 
    '#default_value' => (!$register && isset($account->roles) ? array_keys($account->roles) : array()), 
    '#options' => $roles, 
    DRUPAL_AUTHENTICATED_RID => $checkbox_authenticated,
  );

  $form['account']['notify'] = array(
    '#type' => 'checkbox', 
    '#title' => t('Notify user of new account'), 
    '#access' => $register,
  );

  // Signature.
  $form['signature_settings'] = array(
    '#type' => 'fieldset', 
    '#title' => t('Signature settings'), 
    '#weight' => 1, 
    '#access' => (!$register && variable_get('user_signatures', 0)),
  );

  $form['signature_settings']['signature'] = array(
    '#type' => 'text_format', 
    '#title' => t('Signature'), 
    '#default_value' => isset($account->signature) ? $account->signature : '', 
    '#description' => t('Your signature will be publicly displayed at the end of your comments.'), 
    '#format' => isset($account->signature_format) ? $account->signature_format : NULL,
  );

  // Picture/avatar.
  $form['picture'] = array(
    '#type' => 'fieldset', 
    '#title' => t('Picture'), 
    '#weight' => 1, 
    '#access' => (!$register && variable_get('user_pictures', 0)),
  );
  $form['picture']['picture'] = array(
    '#type' => 'value', 
    '#value' => isset($account->picture) ? $account->picture : NULL,
  );
  $form['picture']['picture_current'] = array(
    '#markup' => theme('user_picture', array('account' => $account)),
  );
  $form['picture']['picture_delete'] = array(
    '#type' => 'checkbox', 
    '#title' => t('Delete picture'), 
    '#access' => !empty($account->picture->fid), 
    '#description' => t('Check this box to delete your current picture.'),
  );
  $form['picture']['picture_upload'] = array(
    '#type' => 'file', 
    '#title' => t('Upload picture'), 
    '#size' => 48, 
    '#description' => t('Your virtual face or picture. Pictures larger than @dimensions pixels will be scaled down.', array('@dimensions' => variable_get('user_picture_dimensions', '85x85'))) . ' ' . filter_xss_admin(variable_get('user_picture_guidelines', '')),
  );
  $form['#validate'][] = 'user_validate_picture';
}

function context_admin_user_create_menu_form_alter(&$form, $form_state, $form_id, $page) {
  if ($form_id == 'context_admin_user_create_menu_render_form' && $page) {
    if (isset($page['handler']->conf['context_admin_user_roles'])) {
      $form['account']['roles']['#value'] = array_filter($page['handler']->conf['context_admin_user_roles']);
    }
    $form['account']['roles']['#value'][2] = TRUE;
    if (!isset($page['handler']->conf['context_admin_user_roles_alter']) || !$page['handler']->conf['context_admin_user_roles_alter'] || $page['handler']->conf['context_admin_user_roles_alter'] == 'disallow') {
      $form['account']['roles']['#disabled'] = TRUE;
    }
    elseif ($page['handler']->conf['context_admin_user_roles_alter'] == 'limited') {
      foreach ($page['handler']->conf['context_admin_user_roles_limited'] as $id => $status) {
        if ($status == 0) {
          unset($form['account']['roles']['#options'][$id]);
        }
      }
    }
    $form['account']['status']['#default_value'] = $page['handler']->conf['context_admin_user_status'];
    if (!isset($page['handler']->conf['context_admin_user_status_alter']) || !$page['handler']->conf['context_admin_user_status_alter']) {
      $form['account']['status']['#disabled'] = TRUE;
    }
    if (!isset($page['handler']->conf['context_admin_user_notify']) || !$page['handler']->conf['context_admin_user_notify']) {
      $form['account']['notify']['#access'] = FALSE;
    }
  }
}
