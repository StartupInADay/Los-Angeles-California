<?php

function sbp_wizard_email_form() {
  $form['emailIntro'] = array(
    '#markup' => t('Please fill out your e-mail address to receive the checklist.'),
  );
  $form['emailResults'] = array(
    '#type' => 'textfield',
    '#title' => t('E-mail address'),
    '#required' => TRUE,
  );
  $form['emailCheck'] = array(
    '#type' => 'textfield',
    '#title' => t('Validate e-mail address'),
    //'#attributes' => array('class' => array('element-hidden')),
    '#prefix' => '<div class="element-hidden">',
    '#suffix' => '</div>',
  );
  $form['emailSubmit'] = array(
    '#type' => 'submit',
    '#value' => t('Send'),
    '#id' => 'wizard-results-submit'
  );

  return $form;
}
