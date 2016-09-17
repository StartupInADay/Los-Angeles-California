<?php
/**
 * @file
 * sbp_users.features.user_permission.inc
 */

/**
 * Implements hook_user_default_permissions().
 */
function sbp_users_user_default_permissions() {
  $permissions = array();

  // Exported permission: 'access content'.
  $permissions['access content'] = array(
    'name' => 'access content',
    'roles' => array(
      'Editor' => 'Editor',
      'administrator' => 'administrator',
      'anonymous user' => 'anonymous user',
      'authenticated user' => 'authenticated user',
      'curator' => 'curator',
    ),
    'module' => 'node',
  );

  // Exported permission: 'access content overview'.
  $permissions['access content overview'] = array(
    'name' => 'access content overview',
    'roles' => array(
      'Editor' => 'Editor',
      'administrator' => 'administrator',
      'curator' => 'curator',
    ),
    'module' => 'node',
  );

  // Exported permission: 'access media browser'.
  $permissions['access media browser'] = array(
    'name' => 'access media browser',
    'roles' => array(
      'Editor' => 'Editor',
      'administrator' => 'administrator',
      'curator' => 'curator',
    ),
    'module' => 'media',
  );

  // Exported permission: 'access navbar'.
  $permissions['access navbar'] = array(
    'name' => 'access navbar',
    'roles' => array(
      'Editor' => 'Editor',
      'administrator' => 'administrator',
      'curator' => 'curator',
    ),
    'module' => 'navbar',
  );

  // Exported permission: 'add media from remote sources'.
  $permissions['add media from remote sources'] = array(
    'name' => 'add media from remote sources',
    'roles' => array(
      'administrator' => 'administrator',
      'curator' => 'curator',
    ),
    'module' => 'media_internet',
  );

  // Exported permission: 'add terms in business_needs'.
  $permissions['add terms in business_needs'] = array(
    'name' => 'add terms in business_needs',
    'roles' => array(
      'Editor' => 'Editor',
      'administrator' => 'administrator',
    ),
    'module' => 'taxonomy_access_fix',
  );

  // Exported permission: 'add terms in business_ownership'.
  $permissions['add terms in business_ownership'] = array(
    'name' => 'add terms in business_ownership',
    'roles' => array(
      'Editor' => 'Editor',
      'administrator' => 'administrator',
    ),
    'module' => 'taxonomy_access_fix',
  );

  // Exported permission: 'add terms in business_portal_wizard_sections'.
  $permissions['add terms in business_portal_wizard_sections'] = array(
    'name' => 'add terms in business_portal_wizard_sections',
    'roles' => array(
      'Editor' => 'Editor',
      'administrator' => 'administrator',
    ),
    'module' => 'taxonomy_access_fix',
  );

  // Exported permission: 'add terms in carousel'.
  $permissions['add terms in carousel'] = array(
    'name' => 'add terms in carousel',
    'roles' => array(
      'Editor' => 'Editor',
      'administrator' => 'administrator',
    ),
    'module' => 'taxonomy_access_fix',
  );

  // Exported permission: 'add terms in department_type'.
  $permissions['add terms in department_type'] = array(
    'name' => 'add terms in department_type',
    'roles' => array(
      'Editor' => 'Editor',
      'administrator' => 'administrator',
    ),
    'module' => 'taxonomy_access_fix',
  );

  // Exported permission: 'add terms in document_business_need'.
  $permissions['add terms in document_business_need'] = array(
    'name' => 'add terms in document_business_need',
    'roles' => array(
      'Editor' => 'Editor',
      'administrator' => 'administrator',
    ),
    'module' => 'taxonomy_access_fix',
  );

  // Exported permission: 'add terms in document_business_stage'.
  $permissions['add terms in document_business_stage'] = array(
    'name' => 'add terms in document_business_stage',
    'roles' => array(
      'Editor' => 'Editor',
      'administrator' => 'administrator',
    ),
    'module' => 'taxonomy_access_fix',
  );

  // Exported permission: 'add terms in document_type'.
  $permissions['add terms in document_type'] = array(
    'name' => 'add terms in document_type',
    'roles' => array(
      'Editor' => 'Editor',
      'administrator' => 'administrator',
    ),
    'module' => 'taxonomy_access_fix',
  );

  // Exported permission: 'add terms in geographical_area'.
  $permissions['add terms in geographical_area'] = array(
    'name' => 'add terms in geographical_area',
    'roles' => array(
      'Editor' => 'Editor',
      'administrator' => 'administrator',
    ),
    'module' => 'taxonomy_access_fix',
  );

  // Exported permission: 'add terms in industry_tags'.
  $permissions['add terms in industry_tags'] = array(
    'name' => 'add terms in industry_tags',
    'roles' => array(
      'Editor' => 'Editor',
      'administrator' => 'administrator',
    ),
    'module' => 'taxonomy_access_fix',
  );

  // Exported permission: 'add terms in permit_government_level'.
  $permissions['add terms in permit_government_level'] = array(
    'name' => 'add terms in permit_government_level',
    'roles' => array(
      'Editor' => 'Editor',
      'administrator' => 'administrator',
    ),
    'module' => 'taxonomy_access_fix',
  );

  // Exported permission: 'add terms in permit_issuing_department'.
  $permissions['add terms in permit_issuing_department'] = array(
    'name' => 'add terms in permit_issuing_department',
    'roles' => array(
      'Editor' => 'Editor',
      'administrator' => 'administrator',
    ),
    'module' => 'taxonomy_access_fix',
  );

  // Exported permission: 'add terms in permit_tags'.
  $permissions['add terms in permit_tags'] = array(
    'name' => 'add terms in permit_tags',
    'roles' => array(
      'Editor' => 'Editor',
      'administrator' => 'administrator',
    ),
    'module' => 'taxonomy_access_fix',
  );

  // Exported permission: 'add terms in question_category'.
  $permissions['add terms in question_category'] = array(
    'name' => 'add terms in question_category',
    'roles' => array(
      'Editor' => 'Editor',
      'administrator' => 'administrator',
    ),
    'module' => 'taxonomy_access_fix',
  );

  // Exported permission: 'add terms in required_featured_items'.
  $permissions['add terms in required_featured_items'] = array(
    'name' => 'add terms in required_featured_items',
    'roles' => array(
      'Editor' => 'Editor',
      'administrator' => 'administrator',
    ),
    'module' => 'taxonomy_access_fix',
  );

  // Exported permission: 'add terms in resource_type'.
  $permissions['add terms in resource_type'] = array(
    'name' => 'add terms in resource_type',
    'roles' => array(
      'Editor' => 'Editor',
      'administrator' => 'administrator',
    ),
    'module' => 'taxonomy_access_fix',
  );

  // Exported permission: 'add terms in sbp_tags'.
  $permissions['add terms in sbp_tags'] = array(
    'name' => 'add terms in sbp_tags',
    'roles' => array(
      'Editor' => 'Editor',
      'administrator' => 'administrator',
    ),
    'module' => 'taxonomy_access_fix',
  );

  // Exported permission: 'add terms in style_template'.
  $permissions['add terms in style_template'] = array(
    'name' => 'add terms in style_template',
    'roles' => array(
      'Editor' => 'Editor',
      'administrator' => 'administrator',
    ),
    'module' => 'taxonomy_access_fix',
  );

  // Exported permission: 'add terms in terms_definitions'.
  $permissions['add terms in terms_definitions'] = array(
    'name' => 'add terms in terms_definitions',
    'roles' => array(
      'Editor' => 'Editor',
      'administrator' => 'administrator',
    ),
    'module' => 'taxonomy_access_fix',
  );

  // Exported permission: 'add terms in terms_definitions_grouping'.
  $permissions['add terms in terms_definitions_grouping'] = array(
    'name' => 'add terms in terms_definitions_grouping',
    'roles' => array(
      'Editor' => 'Editor',
      'administrator' => 'administrator',
    ),
    'module' => 'taxonomy_access_fix',
  );

  // Exported permission: 'administer content revisions'.
  $permissions['administer content revisions'] = array(
    'name' => 'administer content revisions',
    'roles' => array(
      'Editor' => 'Editor',
      'administrator' => 'administrator',
      'reviewer' => 'reviewer',
    ),
    'module' => 'nra',
  );

  // Exported permission: 'administer fieldable panels panes'.
  $permissions['administer fieldable panels panes'] = array(
    'name' => 'administer fieldable panels panes',
    'roles' => array(
      'Editor' => 'Editor',
      'administrator' => 'administrator',
      'marketer' => 'marketer',
    ),
    'module' => 'fieldable_panels_panes',
  );

  // Exported permission: 'administer menu'.
  $permissions['administer menu'] = array(
    'name' => 'administer menu',
    'roles' => array(
      'Editor' => 'Editor',
      'administrator' => 'administrator',
      'curator' => 'curator',
    ),
    'module' => 'menu',
  );

  // Exported permission: 'administer nodes'.
  $permissions['administer nodes'] = array(
    'name' => 'administer nodes',
    'roles' => array(
      'Editor' => 'Editor',
      'administrator' => 'administrator',
      'reviewer' => 'reviewer',
    ),
    'module' => 'node',
  );

  // Exported permission: 'administer panelizer comment comment_node_sbp_starter_kit defaults'.
  $permissions['administer panelizer comment comment_node_sbp_starter_kit defaults'] = array(
    'name' => 'administer panelizer comment comment_node_sbp_starter_kit defaults',
    'roles' => array(),
    'module' => 'panelizer',
  );

  // Exported permission: 'administer panelizer node faq defaults'.
  $permissions['administer panelizer node faq defaults'] = array(
    'name' => 'administer panelizer node faq defaults',
    'roles' => array(
      'administrator' => 'administrator',
    ),
    'module' => 'panelizer',
  );

  // Exported permission: 'administer panelizer node incentives content'.
  $permissions['administer panelizer node incentives content'] = array(
    'name' => 'administer panelizer node incentives content',
    'roles' => array(
      'Editor' => 'Editor',
      'administrator' => 'administrator',
    ),
    'module' => 'panelizer',
  );

  // Exported permission: 'administer panelizer node incentives context'.
  $permissions['administer panelizer node incentives context'] = array(
    'name' => 'administer panelizer node incentives context',
    'roles' => array(
      'administrator' => 'administrator',
    ),
    'module' => 'panelizer',
  );

  // Exported permission: 'administer panelizer node incentives defaults'.
  $permissions['administer panelizer node incentives defaults'] = array(
    'name' => 'administer panelizer node incentives defaults',
    'roles' => array(
      'administrator' => 'administrator',
    ),
    'module' => 'panelizer',
  );

  // Exported permission: 'administer panelizer node incentives layout'.
  $permissions['administer panelizer node incentives layout'] = array(
    'name' => 'administer panelizer node incentives layout',
    'roles' => array(
      'administrator' => 'administrator',
    ),
    'module' => 'panelizer',
  );

  // Exported permission: 'administer panelizer node incentives overview'.
  $permissions['administer panelizer node incentives overview'] = array(
    'name' => 'administer panelizer node incentives overview',
    'roles' => array(
      'administrator' => 'administrator',
    ),
    'module' => 'panelizer',
  );

  // Exported permission: 'administer panelizer node incentives settings'.
  $permissions['administer panelizer node incentives settings'] = array(
    'name' => 'administer panelizer node incentives settings',
    'roles' => array(
      'administrator' => 'administrator',
    ),
    'module' => 'panelizer',
  );

  // Exported permission: 'administer panelizer node labp_service_center content'.
  $permissions['administer panelizer node labp_service_center content'] = array(
    'name' => 'administer panelizer node labp_service_center content',
    'roles' => array(
      'Editor' => 'Editor',
      'administrator' => 'administrator',
    ),
    'module' => 'panelizer',
  );

  // Exported permission: 'administer panelizer node labp_service_center context'.
  $permissions['administer panelizer node labp_service_center context'] = array(
    'name' => 'administer panelizer node labp_service_center context',
    'roles' => array(
      'administrator' => 'administrator',
    ),
    'module' => 'panelizer',
  );

  // Exported permission: 'administer panelizer node labp_service_center defaults'.
  $permissions['administer panelizer node labp_service_center defaults'] = array(
    'name' => 'administer panelizer node labp_service_center defaults',
    'roles' => array(
      'administrator' => 'administrator',
    ),
    'module' => 'panelizer',
  );

  // Exported permission: 'administer panelizer node labp_service_center layout'.
  $permissions['administer panelizer node labp_service_center layout'] = array(
    'name' => 'administer panelizer node labp_service_center layout',
    'roles' => array(
      'administrator' => 'administrator',
    ),
    'module' => 'panelizer',
  );

  // Exported permission: 'administer panelizer node labp_service_center overview'.
  $permissions['administer panelizer node labp_service_center overview'] = array(
    'name' => 'administer panelizer node labp_service_center overview',
    'roles' => array(
      'administrator' => 'administrator',
    ),
    'module' => 'panelizer',
  );

  // Exported permission: 'administer panelizer node labp_service_center settings'.
  $permissions['administer panelizer node labp_service_center settings'] = array(
    'name' => 'administer panelizer node labp_service_center settings',
    'roles' => array(
      'administrator' => 'administrator',
    ),
    'module' => 'panelizer',
  );

  // Exported permission: 'administer panelizer node landing layout'.
  $permissions['administer panelizer node landing layout'] = array(
    'name' => 'administer panelizer node landing layout',
    'roles' => array(
      'administrator' => 'administrator',
      'marketer' => 'marketer',
    ),
    'module' => 'panelizer',
  );

  // Exported permission: 'administer panelizer node online_resource content'.
  $permissions['administer panelizer node online_resource content'] = array(
    'name' => 'administer panelizer node online_resource content',
    'roles' => array(
      'Editor' => 'Editor',
      'administrator' => 'administrator',
    ),
    'module' => 'panelizer',
  );

  // Exported permission: 'administer panelizer node online_resource context'.
  $permissions['administer panelizer node online_resource context'] = array(
    'name' => 'administer panelizer node online_resource context',
    'roles' => array(
      'administrator' => 'administrator',
    ),
    'module' => 'panelizer',
  );

  // Exported permission: 'administer panelizer node online_resource defaults'.
  $permissions['administer panelizer node online_resource defaults'] = array(
    'name' => 'administer panelizer node online_resource defaults',
    'roles' => array(
      'administrator' => 'administrator',
    ),
    'module' => 'panelizer',
  );

  // Exported permission: 'administer panelizer node online_resource layout'.
  $permissions['administer panelizer node online_resource layout'] = array(
    'name' => 'administer panelizer node online_resource layout',
    'roles' => array(
      'administrator' => 'administrator',
    ),
    'module' => 'panelizer',
  );

  // Exported permission: 'administer panelizer node online_resource overview'.
  $permissions['administer panelizer node online_resource overview'] = array(
    'name' => 'administer panelizer node online_resource overview',
    'roles' => array(
      'administrator' => 'administrator',
    ),
    'module' => 'panelizer',
  );

  // Exported permission: 'administer panelizer node online_resource settings'.
  $permissions['administer panelizer node online_resource settings'] = array(
    'name' => 'administer panelizer node online_resource settings',
    'roles' => array(
      'administrator' => 'administrator',
    ),
    'module' => 'panelizer',
  );

  // Exported permission: 'administer panelizer node sbp_basic_page content'.
  $permissions['administer panelizer node sbp_basic_page content'] = array(
    'name' => 'administer panelizer node sbp_basic_page content',
    'roles' => array(
      'administrator' => 'administrator',
    ),
    'module' => 'panelizer',
  );

  // Exported permission: 'administer panelizer node sbp_basic_page context'.
  $permissions['administer panelizer node sbp_basic_page context'] = array(
    'name' => 'administer panelizer node sbp_basic_page context',
    'roles' => array(
      'administrator' => 'administrator',
    ),
    'module' => 'panelizer',
  );

  // Exported permission: 'administer panelizer node sbp_basic_page defaults'.
  $permissions['administer panelizer node sbp_basic_page defaults'] = array(
    'name' => 'administer panelizer node sbp_basic_page defaults',
    'roles' => array(
      'administrator' => 'administrator',
    ),
    'module' => 'panelizer',
  );

  // Exported permission: 'administer panelizer node sbp_basic_page layout'.
  $permissions['administer panelizer node sbp_basic_page layout'] = array(
    'name' => 'administer panelizer node sbp_basic_page layout',
    'roles' => array(
      'administrator' => 'administrator',
    ),
    'module' => 'panelizer',
  );

  // Exported permission: 'administer panelizer node sbp_basic_page overview'.
  $permissions['administer panelizer node sbp_basic_page overview'] = array(
    'name' => 'administer panelizer node sbp_basic_page overview',
    'roles' => array(
      'administrator' => 'administrator',
    ),
    'module' => 'panelizer',
  );

  // Exported permission: 'administer panelizer node sbp_basic_page settings'.
  $permissions['administer panelizer node sbp_basic_page settings'] = array(
    'name' => 'administer panelizer node sbp_basic_page settings',
    'roles' => array(
      'administrator' => 'administrator',
    ),
    'module' => 'panelizer',
  );

  // Exported permission: 'administer panelizer node sbp_department content'.
  $permissions['administer panelizer node sbp_department content'] = array(
    'name' => 'administer panelizer node sbp_department content',
    'roles' => array(
      'Editor' => 'Editor',
      'administrator' => 'administrator',
    ),
    'module' => 'panelizer',
  );

  // Exported permission: 'administer panelizer node sbp_department context'.
  $permissions['administer panelizer node sbp_department context'] = array(
    'name' => 'administer panelizer node sbp_department context',
    'roles' => array(
      'administrator' => 'administrator',
    ),
    'module' => 'panelizer',
  );

  // Exported permission: 'administer panelizer node sbp_department defaults'.
  $permissions['administer panelizer node sbp_department defaults'] = array(
    'name' => 'administer panelizer node sbp_department defaults',
    'roles' => array(
      'administrator' => 'administrator',
    ),
    'module' => 'panelizer',
  );

  // Exported permission: 'administer panelizer node sbp_department layout'.
  $permissions['administer panelizer node sbp_department layout'] = array(
    'name' => 'administer panelizer node sbp_department layout',
    'roles' => array(
      'administrator' => 'administrator',
    ),
    'module' => 'panelizer',
  );

  // Exported permission: 'administer panelizer node sbp_department overview'.
  $permissions['administer panelizer node sbp_department overview'] = array(
    'name' => 'administer panelizer node sbp_department overview',
    'roles' => array(
      'administrator' => 'administrator',
    ),
    'module' => 'panelizer',
  );

  // Exported permission: 'administer panelizer node sbp_department settings'.
  $permissions['administer panelizer node sbp_department settings'] = array(
    'name' => 'administer panelizer node sbp_department settings',
    'roles' => array(
      'administrator' => 'administrator',
    ),
    'module' => 'panelizer',
  );

  // Exported permission: 'administer panelizer node sbp_landing_page choice'.
  $permissions['administer panelizer node sbp_landing_page choice'] = array(
    'name' => 'administer panelizer node sbp_landing_page choice',
    'roles' => array(
      'administrator' => 'administrator',
    ),
    'module' => 'panelizer',
  );

  // Exported permission: 'administer panelizer node sbp_landing_page content'.
  $permissions['administer panelizer node sbp_landing_page content'] = array(
    'name' => 'administer panelizer node sbp_landing_page content',
    'roles' => array(
      'Editor' => 'Editor',
      'administrator' => 'administrator',
    ),
    'module' => 'panelizer',
  );

  // Exported permission: 'administer panelizer node sbp_landing_page context'.
  $permissions['administer panelizer node sbp_landing_page context'] = array(
    'name' => 'administer panelizer node sbp_landing_page context',
    'roles' => array(
      'administrator' => 'administrator',
    ),
    'module' => 'panelizer',
  );

  // Exported permission: 'administer panelizer node sbp_landing_page defaults'.
  $permissions['administer panelizer node sbp_landing_page defaults'] = array(
    'name' => 'administer panelizer node sbp_landing_page defaults',
    'roles' => array(
      'administrator' => 'administrator',
    ),
    'module' => 'panelizer',
  );

  // Exported permission: 'administer panelizer node sbp_landing_page layout'.
  $permissions['administer panelizer node sbp_landing_page layout'] = array(
    'name' => 'administer panelizer node sbp_landing_page layout',
    'roles' => array(
      'Editor' => 'Editor',
      'administrator' => 'administrator',
    ),
    'module' => 'panelizer',
  );

  // Exported permission: 'administer panelizer node sbp_landing_page overview'.
  $permissions['administer panelizer node sbp_landing_page overview'] = array(
    'name' => 'administer panelizer node sbp_landing_page overview',
    'roles' => array(
      'administrator' => 'administrator',
    ),
    'module' => 'panelizer',
  );

  // Exported permission: 'administer panelizer node sbp_landing_page settings'.
  $permissions['administer panelizer node sbp_landing_page settings'] = array(
    'name' => 'administer panelizer node sbp_landing_page settings',
    'roles' => array(
      'administrator' => 'administrator',
    ),
    'module' => 'panelizer',
  );

  // Exported permission: 'administer panelizer taxonomy_term resource_type defaults'.
  $permissions['administer panelizer taxonomy_term resource_type defaults'] = array(
    'name' => 'administer panelizer taxonomy_term resource_type defaults',
    'roles' => array(
      'administrator' => 'administrator',
    ),
    'module' => 'panelizer',
  );

  // Exported permission: 'administer panels layouts'.
  $permissions['administer panels layouts'] = array(
    'name' => 'administer panels layouts',
    'roles' => array(
      'Editor' => 'Editor',
      'administrator' => 'administrator',
      'marketer' => 'marketer',
    ),
    'module' => 'panels',
  );

  // Exported permission: 'bypass node access'.
  $permissions['bypass node access'] = array(
    'name' => 'bypass node access',
    'roles' => array(
      'administrator' => 'administrator',
    ),
    'module' => 'node',
  );

  // Exported permission: 'change layouts in place editing'.
  $permissions['change layouts in place editing'] = array(
    'name' => 'change layouts in place editing',
    'roles' => array(
      'Editor' => 'Editor',
      'administrator' => 'administrator',
      'marketer' => 'marketer',
    ),
    'module' => 'panels',
  );

  // Exported permission: 'change own username'.
  $permissions['change own username'] = array(
    'name' => 'change own username',
    'roles' => array(
      'administrator' => 'administrator',
    ),
    'module' => 'user',
  );

  // Exported permission: 'create business_portal_sponsors content'.
  $permissions['create business_portal_sponsors content'] = array(
    'name' => 'create business_portal_sponsors content',
    'roles' => array(
      'Editor' => 'Editor',
    ),
    'module' => 'node',
  );

  // Exported permission: 'create faq content'.
  $permissions['create faq content'] = array(
    'name' => 'create faq content',
    'roles' => array(
      'Editor' => 'Editor',
      'administrator' => 'administrator',
    ),
    'module' => 'node',
  );

  // Exported permission: 'create fieldable content'.
  $permissions['create fieldable content'] = array(
    'name' => 'create fieldable content',
    'roles' => array(
      'Editor' => 'Editor',
      'administrator' => 'administrator',
    ),
    'module' => 'fieldable_panels_panes',
  );

  // Exported permission: 'create fieldable featured_resources'.
  $permissions['create fieldable featured_resources'] = array(
    'name' => 'create fieldable featured_resources',
    'roles' => array(
      'Editor' => 'Editor',
    ),
    'module' => 'fieldable_panels_panes',
  );

  // Exported permission: 'create fieldable fieldable_panels_pane'.
  $permissions['create fieldable fieldable_panels_pane'] = array(
    'name' => 'create fieldable fieldable_panels_pane',
    'roles' => array(
      'Editor' => 'Editor',
      'administrator' => 'administrator',
      'marketer' => 'marketer',
    ),
    'module' => 'fieldable_panels_panes',
  );

  // Exported permission: 'create fieldable four_column_cta'.
  $permissions['create fieldable four_column_cta'] = array(
    'name' => 'create fieldable four_column_cta',
    'roles' => array(
      'Editor' => 'Editor',
      'administrator' => 'administrator',
    ),
    'module' => 'fieldable_panels_panes',
  );

  // Exported permission: 'create fieldable general_content'.
  $permissions['create fieldable general_content'] = array(
    'name' => 'create fieldable general_content',
    'roles' => array(
      'Editor' => 'Editor',
    ),
    'module' => 'fieldable_panels_panes',
  );

  // Exported permission: 'create fieldable map'.
  $permissions['create fieldable map'] = array(
    'name' => 'create fieldable map',
    'roles' => array(
      'Editor' => 'Editor',
      'administrator' => 'administrator',
      'marketer' => 'marketer',
    ),
    'module' => 'fieldable_panels_panes',
  );

  // Exported permission: 'create fieldable media_pane'.
  $permissions['create fieldable media_pane'] = array(
    'name' => 'create fieldable media_pane',
    'roles' => array(
      'Editor' => 'Editor',
      'administrator' => 'administrator',
      'marketer' => 'marketer',
    ),
    'module' => 'fieldable_panels_panes',
  );

  // Exported permission: 'create fieldable pull_quote'.
  $permissions['create fieldable pull_quote'] = array(
    'name' => 'create fieldable pull_quote',
    'roles' => array(
      'Editor' => 'Editor',
    ),
    'module' => 'fieldable_panels_panes',
  );

  // Exported permission: 'create fieldable quick_links'.
  $permissions['create fieldable quick_links'] = array(
    'name' => 'create fieldable quick_links',
    'roles' => array(
      'Editor' => 'Editor',
      'administrator' => 'administrator',
      'marketer' => 'marketer',
    ),
    'module' => 'fieldable_panels_panes',
  );

  // Exported permission: 'create fieldable raw_html'.
  $permissions['create fieldable raw_html'] = array(
    'name' => 'create fieldable raw_html',
    'roles' => array(
      'Editor' => 'Editor',
      'administrator' => 'administrator',
      'marketer' => 'marketer',
    ),
    'module' => 'fieldable_panels_panes',
  );

  // Exported permission: 'create fieldable section_content'.
  $permissions['create fieldable section_content'] = array(
    'name' => 'create fieldable section_content',
    'roles' => array(
      'Editor' => 'Editor',
      'administrator' => 'administrator',
    ),
    'module' => 'fieldable_panels_panes',
  );

  // Exported permission: 'create fieldable text'.
  $permissions['create fieldable text'] = array(
    'name' => 'create fieldable text',
    'roles' => array(
      'Editor' => 'Editor',
      'administrator' => 'administrator',
      'marketer' => 'marketer',
    ),
    'module' => 'fieldable_panels_panes',
  );

  // Exported permission: 'create fieldable three_column_cta'.
  $permissions['create fieldable three_column_cta'] = array(
    'name' => 'create fieldable three_column_cta',
    'roles' => array(
      'Editor' => 'Editor',
    ),
    'module' => 'fieldable_panels_panes',
  );

  // Exported permission: 'create fieldable timeline'.
  $permissions['create fieldable timeline'] = array(
    'name' => 'create fieldable timeline',
    'roles' => array(
      'Editor' => 'Editor',
    ),
    'module' => 'fieldable_panels_panes',
  );

  // Exported permission: 'create files'.
  $permissions['create files'] = array(
    'name' => 'create files',
    'roles' => array(
      'Editor' => 'Editor',
      'administrator' => 'administrator',
      'curator' => 'curator',
    ),
    'module' => 'file_entity',
  );

  // Exported permission: 'create incentives content'.
  $permissions['create incentives content'] = array(
    'name' => 'create incentives content',
    'roles' => array(
      'Editor' => 'Editor',
      'administrator' => 'administrator',
    ),
    'module' => 'node',
  );

  // Exported permission: 'create labp_service_center content'.
  $permissions['create labp_service_center content'] = array(
    'name' => 'create labp_service_center content',
    'roles' => array(
      'Editor' => 'Editor',
      'administrator' => 'administrator',
    ),
    'module' => 'node',
  );

  // Exported permission: 'create online_resource content'.
  $permissions['create online_resource content'] = array(
    'name' => 'create online_resource content',
    'roles' => array(
      'Editor' => 'Editor',
      'administrator' => 'administrator',
    ),
    'module' => 'node',
  );

  // Exported permission: 'create sbp_basic_page content'.
  $permissions['create sbp_basic_page content'] = array(
    'name' => 'create sbp_basic_page content',
    'roles' => array(
      'administrator' => 'administrator',
    ),
    'module' => 'node',
  );

  // Exported permission: 'create sbp_department content'.
  $permissions['create sbp_department content'] = array(
    'name' => 'create sbp_department content',
    'roles' => array(
      'Editor' => 'Editor',
      'administrator' => 'administrator',
    ),
    'module' => 'node',
  );

  // Exported permission: 'create sbp_landing_page content'.
  $permissions['create sbp_landing_page content'] = array(
    'name' => 'create sbp_landing_page content',
    'roles' => array(
      'Editor' => 'Editor',
      'administrator' => 'administrator',
    ),
    'module' => 'node',
  );

  // Exported permission: 'create url aliases'.
  $permissions['create url aliases'] = array(
    'name' => 'create url aliases',
    'roles' => array(
      'Editor' => 'Editor',
      'administrator' => 'administrator',
      'curator' => 'curator',
    ),
    'module' => 'path',
  );

  // Exported permission: 'delete any audio files'.
  $permissions['delete any audio files'] = array(
    'name' => 'delete any audio files',
    'roles' => array(
      'Editor' => 'Editor',
      'administrator' => 'administrator',
      'reviewer' => 'reviewer',
    ),
    'module' => 'file_entity',
  );

  // Exported permission: 'delete any business_portal_sponsors content'.
  $permissions['delete any business_portal_sponsors content'] = array(
    'name' => 'delete any business_portal_sponsors content',
    'roles' => array(
      'Editor' => 'Editor',
    ),
    'module' => 'node',
  );

  // Exported permission: 'delete any faq content'.
  $permissions['delete any faq content'] = array(
    'name' => 'delete any faq content',
    'roles' => array(
      'Editor' => 'Editor',
      'administrator' => 'administrator',
    ),
    'module' => 'node',
  );

  // Exported permission: 'delete any image files'.
  $permissions['delete any image files'] = array(
    'name' => 'delete any image files',
    'roles' => array(
      'Editor' => 'Editor',
      'administrator' => 'administrator',
      'reviewer' => 'reviewer',
    ),
    'module' => 'file_entity',
  );

  // Exported permission: 'delete any incentives content'.
  $permissions['delete any incentives content'] = array(
    'name' => 'delete any incentives content',
    'roles' => array(
      'Editor' => 'Editor',
      'administrator' => 'administrator',
    ),
    'module' => 'node',
  );

  // Exported permission: 'delete any labp_service_center content'.
  $permissions['delete any labp_service_center content'] = array(
    'name' => 'delete any labp_service_center content',
    'roles' => array(
      'Editor' => 'Editor',
      'administrator' => 'administrator',
    ),
    'module' => 'node',
  );

  // Exported permission: 'delete any online_resource content'.
  $permissions['delete any online_resource content'] = array(
    'name' => 'delete any online_resource content',
    'roles' => array(
      'Editor' => 'Editor',
      'administrator' => 'administrator',
    ),
    'module' => 'node',
  );

  // Exported permission: 'delete any sbp_basic_page content'.
  $permissions['delete any sbp_basic_page content'] = array(
    'name' => 'delete any sbp_basic_page content',
    'roles' => array(
      'administrator' => 'administrator',
    ),
    'module' => 'node',
  );

  // Exported permission: 'delete any sbp_department content'.
  $permissions['delete any sbp_department content'] = array(
    'name' => 'delete any sbp_department content',
    'roles' => array(
      'Editor' => 'Editor',
      'administrator' => 'administrator',
    ),
    'module' => 'node',
  );

  // Exported permission: 'delete any sbp_landing_page content'.
  $permissions['delete any sbp_landing_page content'] = array(
    'name' => 'delete any sbp_landing_page content',
    'roles' => array(
      'Editor' => 'Editor',
      'administrator' => 'administrator',
    ),
    'module' => 'node',
  );

  // Exported permission: 'delete any video files'.
  $permissions['delete any video files'] = array(
    'name' => 'delete any video files',
    'roles' => array(
      'Editor' => 'Editor',
      'administrator' => 'administrator',
      'reviewer' => 'reviewer',
    ),
    'module' => 'file_entity',
  );

  // Exported permission: 'delete fieldable content'.
  $permissions['delete fieldable content'] = array(
    'name' => 'delete fieldable content',
    'roles' => array(
      'Editor' => 'Editor',
      'administrator' => 'administrator',
    ),
    'module' => 'fieldable_panels_panes',
  );

  // Exported permission: 'delete fieldable featured_resources'.
  $permissions['delete fieldable featured_resources'] = array(
    'name' => 'delete fieldable featured_resources',
    'roles' => array(
      'Editor' => 'Editor',
    ),
    'module' => 'fieldable_panels_panes',
  );

  // Exported permission: 'delete fieldable fieldable_panels_pane'.
  $permissions['delete fieldable fieldable_panels_pane'] = array(
    'name' => 'delete fieldable fieldable_panels_pane',
    'roles' => array(
      'Editor' => 'Editor',
      'administrator' => 'administrator',
      'marketer' => 'marketer',
    ),
    'module' => 'fieldable_panels_panes',
  );

  // Exported permission: 'delete fieldable four_column_cta'.
  $permissions['delete fieldable four_column_cta'] = array(
    'name' => 'delete fieldable four_column_cta',
    'roles' => array(
      'Editor' => 'Editor',
      'administrator' => 'administrator',
    ),
    'module' => 'fieldable_panels_panes',
  );

  // Exported permission: 'delete fieldable general_content'.
  $permissions['delete fieldable general_content'] = array(
    'name' => 'delete fieldable general_content',
    'roles' => array(
      'Editor' => 'Editor',
    ),
    'module' => 'fieldable_panels_panes',
  );

  // Exported permission: 'delete fieldable map'.
  $permissions['delete fieldable map'] = array(
    'name' => 'delete fieldable map',
    'roles' => array(
      'Editor' => 'Editor',
      'administrator' => 'administrator',
      'marketer' => 'marketer',
    ),
    'module' => 'fieldable_panels_panes',
  );

  // Exported permission: 'delete fieldable media_pane'.
  $permissions['delete fieldable media_pane'] = array(
    'name' => 'delete fieldable media_pane',
    'roles' => array(
      'Editor' => 'Editor',
      'administrator' => 'administrator',
      'marketer' => 'marketer',
    ),
    'module' => 'fieldable_panels_panes',
  );

  // Exported permission: 'delete fieldable pull_quote'.
  $permissions['delete fieldable pull_quote'] = array(
    'name' => 'delete fieldable pull_quote',
    'roles' => array(
      'Editor' => 'Editor',
    ),
    'module' => 'fieldable_panels_panes',
  );

  // Exported permission: 'delete fieldable quick_links'.
  $permissions['delete fieldable quick_links'] = array(
    'name' => 'delete fieldable quick_links',
    'roles' => array(
      'Editor' => 'Editor',
      'administrator' => 'administrator',
      'marketer' => 'marketer',
    ),
    'module' => 'fieldable_panels_panes',
  );

  // Exported permission: 'delete fieldable raw_html'.
  $permissions['delete fieldable raw_html'] = array(
    'name' => 'delete fieldable raw_html',
    'roles' => array(
      'Editor' => 'Editor',
      'administrator' => 'administrator',
      'marketer' => 'marketer',
    ),
    'module' => 'fieldable_panels_panes',
  );

  // Exported permission: 'delete fieldable section_content'.
  $permissions['delete fieldable section_content'] = array(
    'name' => 'delete fieldable section_content',
    'roles' => array(
      'Editor' => 'Editor',
      'administrator' => 'administrator',
    ),
    'module' => 'fieldable_panels_panes',
  );

  // Exported permission: 'delete fieldable text'.
  $permissions['delete fieldable text'] = array(
    'name' => 'delete fieldable text',
    'roles' => array(
      'Editor' => 'Editor',
      'administrator' => 'administrator',
      'marketer' => 'marketer',
    ),
    'module' => 'fieldable_panels_panes',
  );

  // Exported permission: 'delete fieldable three_column_cta'.
  $permissions['delete fieldable three_column_cta'] = array(
    'name' => 'delete fieldable three_column_cta',
    'roles' => array(
      'Editor' => 'Editor',
    ),
    'module' => 'fieldable_panels_panes',
  );

  // Exported permission: 'delete fieldable timeline'.
  $permissions['delete fieldable timeline'] = array(
    'name' => 'delete fieldable timeline',
    'roles' => array(
      'Editor' => 'Editor',
    ),
    'module' => 'fieldable_panels_panes',
  );

  // Exported permission: 'delete own audio files'.
  $permissions['delete own audio files'] = array(
    'name' => 'delete own audio files',
    'roles' => array(
      'Editor' => 'Editor',
      'administrator' => 'administrator',
      'curator' => 'curator',
    ),
    'module' => 'file_entity',
  );

  // Exported permission: 'delete own business_portal_sponsors content'.
  $permissions['delete own business_portal_sponsors content'] = array(
    'name' => 'delete own business_portal_sponsors content',
    'roles' => array(
      'Editor' => 'Editor',
    ),
    'module' => 'node',
  );

  // Exported permission: 'delete own faq content'.
  $permissions['delete own faq content'] = array(
    'name' => 'delete own faq content',
    'roles' => array(
      'Editor' => 'Editor',
      'administrator' => 'administrator',
    ),
    'module' => 'node',
  );

  // Exported permission: 'delete own image files'.
  $permissions['delete own image files'] = array(
    'name' => 'delete own image files',
    'roles' => array(
      'Editor' => 'Editor',
      'administrator' => 'administrator',
      'curator' => 'curator',
    ),
    'module' => 'file_entity',
  );

  // Exported permission: 'delete own incentives content'.
  $permissions['delete own incentives content'] = array(
    'name' => 'delete own incentives content',
    'roles' => array(
      'Editor' => 'Editor',
      'administrator' => 'administrator',
    ),
    'module' => 'node',
  );

  // Exported permission: 'delete own labp_service_center content'.
  $permissions['delete own labp_service_center content'] = array(
    'name' => 'delete own labp_service_center content',
    'roles' => array(
      'Editor' => 'Editor',
      'administrator' => 'administrator',
    ),
    'module' => 'node',
  );

  // Exported permission: 'delete own online_resource content'.
  $permissions['delete own online_resource content'] = array(
    'name' => 'delete own online_resource content',
    'roles' => array(
      'Editor' => 'Editor',
      'administrator' => 'administrator',
    ),
    'module' => 'node',
  );

  // Exported permission: 'delete own sbp_basic_page content'.
  $permissions['delete own sbp_basic_page content'] = array(
    'name' => 'delete own sbp_basic_page content',
    'roles' => array(
      'administrator' => 'administrator',
    ),
    'module' => 'node',
  );

  // Exported permission: 'delete own sbp_department content'.
  $permissions['delete own sbp_department content'] = array(
    'name' => 'delete own sbp_department content',
    'roles' => array(
      'Editor' => 'Editor',
      'administrator' => 'administrator',
    ),
    'module' => 'node',
  );

  // Exported permission: 'delete own sbp_landing_page content'.
  $permissions['delete own sbp_landing_page content'] = array(
    'name' => 'delete own sbp_landing_page content',
    'roles' => array(
      'Editor' => 'Editor',
      'administrator' => 'administrator',
    ),
    'module' => 'node',
  );

  // Exported permission: 'delete own video files'.
  $permissions['delete own video files'] = array(
    'name' => 'delete own video files',
    'roles' => array(
      'Editor' => 'Editor',
      'administrator' => 'administrator',
      'curator' => 'curator',
    ),
    'module' => 'file_entity',
  );

  // Exported permission: 'delete revisions'.
  $permissions['delete revisions'] = array(
    'name' => 'delete revisions',
    'roles' => array(
      'Editor' => 'Editor',
      'administrator' => 'administrator',
      'reviewer' => 'reviewer',
    ),
    'module' => 'node',
  );

  // Exported permission: 'delete terms in business_needs'.
  $permissions['delete terms in business_needs'] = array(
    'name' => 'delete terms in business_needs',
    'roles' => array(
      'Editor' => 'Editor',
      'administrator' => 'administrator',
    ),
    'module' => 'taxonomy',
  );

  // Exported permission: 'delete terms in business_ownership'.
  $permissions['delete terms in business_ownership'] = array(
    'name' => 'delete terms in business_ownership',
    'roles' => array(
      'Editor' => 'Editor',
      'administrator' => 'administrator',
    ),
    'module' => 'taxonomy',
  );

  // Exported permission: 'delete terms in carousel'.
  $permissions['delete terms in carousel'] = array(
    'name' => 'delete terms in carousel',
    'roles' => array(
      'Editor' => 'Editor',
      'administrator' => 'administrator',
    ),
    'module' => 'taxonomy',
  );

  // Exported permission: 'delete terms in department_type'.
  $permissions['delete terms in department_type'] = array(
    'name' => 'delete terms in department_type',
    'roles' => array(
      'Editor' => 'Editor',
      'administrator' => 'administrator',
    ),
    'module' => 'taxonomy',
  );

  // Exported permission: 'delete terms in geographical_area'.
  $permissions['delete terms in geographical_area'] = array(
    'name' => 'delete terms in geographical_area',
    'roles' => array(
      'Editor' => 'Editor',
      'administrator' => 'administrator',
    ),
    'module' => 'taxonomy',
  );

  // Exported permission: 'delete terms in identifiers'.
  $permissions['delete terms in identifiers'] = array(
    'name' => 'delete terms in identifiers',
    'roles' => array(
      'administrator' => 'administrator',
    ),
    'module' => 'taxonomy',
  );

  // Exported permission: 'delete terms in industry_tags'.
  $permissions['delete terms in industry_tags'] = array(
    'name' => 'delete terms in industry_tags',
    'roles' => array(
      'Editor' => 'Editor',
      'administrator' => 'administrator',
    ),
    'module' => 'taxonomy',
  );

  // Exported permission: 'delete terms in permit_government_level'.
  $permissions['delete terms in permit_government_level'] = array(
    'name' => 'delete terms in permit_government_level',
    'roles' => array(
      'Editor' => 'Editor',
      'administrator' => 'administrator',
    ),
    'module' => 'taxonomy',
  );

  // Exported permission: 'delete terms in permit_issuing_department'.
  $permissions['delete terms in permit_issuing_department'] = array(
    'name' => 'delete terms in permit_issuing_department',
    'roles' => array(
      'Editor' => 'Editor',
      'administrator' => 'administrator',
    ),
    'module' => 'taxonomy',
  );

  // Exported permission: 'delete terms in permit_tags'.
  $permissions['delete terms in permit_tags'] = array(
    'name' => 'delete terms in permit_tags',
    'roles' => array(
      'Editor' => 'Editor',
      'administrator' => 'administrator',
    ),
    'module' => 'taxonomy',
  );

  // Exported permission: 'delete terms in question_category'.
  $permissions['delete terms in question_category'] = array(
    'name' => 'delete terms in question_category',
    'roles' => array(
      'Editor' => 'Editor',
      'administrator' => 'administrator',
    ),
    'module' => 'taxonomy',
  );

  // Exported permission: 'delete terms in required_featured_items'.
  $permissions['delete terms in required_featured_items'] = array(
    'name' => 'delete terms in required_featured_items',
    'roles' => array(
      'Editor' => 'Editor',
      'administrator' => 'administrator',
    ),
    'module' => 'taxonomy',
  );

  // Exported permission: 'delete terms in resource_type'.
  $permissions['delete terms in resource_type'] = array(
    'name' => 'delete terms in resource_type',
    'roles' => array(
      'Editor' => 'Editor',
      'administrator' => 'administrator',
    ),
    'module' => 'taxonomy',
  );

  // Exported permission: 'delete terms in sbp_tags'.
  $permissions['delete terms in sbp_tags'] = array(
    'name' => 'delete terms in sbp_tags',
    'roles' => array(
      'Editor' => 'Editor',
      'administrator' => 'administrator',
    ),
    'module' => 'taxonomy',
  );

  // Exported permission: 'delete terms in style_template'.
  $permissions['delete terms in style_template'] = array(
    'name' => 'delete terms in style_template',
    'roles' => array(
      'Editor' => 'Editor',
    ),
    'module' => 'taxonomy',
  );

  // Exported permission: 'delete terms in tags'.
  $permissions['delete terms in tags'] = array(
    'name' => 'delete terms in tags',
    'roles' => array(
      'administrator' => 'administrator',
    ),
    'module' => 'taxonomy',
  );

  // Exported permission: 'delete terms in term_alphabet'.
  $permissions['delete terms in term_alphabet'] = array(
    'name' => 'delete terms in term_alphabet',
    'roles' => array(
      'administrator' => 'administrator',
    ),
    'module' => 'taxonomy',
  );

  // Exported permission: 'delete terms in terms_definitions'.
  $permissions['delete terms in terms_definitions'] = array(
    'name' => 'delete terms in terms_definitions',
    'roles' => array(
      'Editor' => 'Editor',
      'administrator' => 'administrator',
    ),
    'module' => 'taxonomy',
  );

  // Exported permission: 'delete terms in terms_definitions_grouping'.
  $permissions['delete terms in terms_definitions_grouping'] = array(
    'name' => 'delete terms in terms_definitions_grouping',
    'roles' => array(
      'Editor' => 'Editor',
      'administrator' => 'administrator',
    ),
    'module' => 'taxonomy',
  );

  // Exported permission: 'download any audio files'.
  $permissions['download any audio files'] = array(
    'name' => 'download any audio files',
    'roles' => array(
      'Editor' => 'Editor',
      'administrator' => 'administrator',
      'curator' => 'curator',
    ),
    'module' => 'file_entity',
  );

  // Exported permission: 'download any image files'.
  $permissions['download any image files'] = array(
    'name' => 'download any image files',
    'roles' => array(
      'Editor' => 'Editor',
      'administrator' => 'administrator',
      'curator' => 'curator',
    ),
    'module' => 'file_entity',
  );

  // Exported permission: 'download any video files'.
  $permissions['download any video files'] = array(
    'name' => 'download any video files',
    'roles' => array(
      'Editor' => 'Editor',
      'administrator' => 'administrator',
      'curator' => 'curator',
    ),
    'module' => 'file_entity',
  );

  // Exported permission: 'download own audio files'.
  $permissions['download own audio files'] = array(
    'name' => 'download own audio files',
    'roles' => array(
      'Editor' => 'Editor',
      'administrator' => 'administrator',
      'curator' => 'curator',
    ),
    'module' => 'file_entity',
  );

  // Exported permission: 'download own image files'.
  $permissions['download own image files'] = array(
    'name' => 'download own image files',
    'roles' => array(
      'Editor' => 'Editor',
      'administrator' => 'administrator',
      'curator' => 'curator',
    ),
    'module' => 'file_entity',
  );

  // Exported permission: 'download own video files'.
  $permissions['download own video files'] = array(
    'name' => 'download own video files',
    'roles' => array(
      'Editor' => 'Editor',
      'administrator' => 'administrator',
      'curator' => 'curator',
    ),
    'module' => 'file_entity',
  );

  // Exported permission: 'edit any audio files'.
  $permissions['edit any audio files'] = array(
    'name' => 'edit any audio files',
    'roles' => array(
      'Editor' => 'Editor',
      'administrator' => 'administrator',
      'curator' => 'curator',
    ),
    'module' => 'file_entity',
  );

  // Exported permission: 'edit any business_portal_sponsors content'.
  $permissions['edit any business_portal_sponsors content'] = array(
    'name' => 'edit any business_portal_sponsors content',
    'roles' => array(
      'Editor' => 'Editor',
    ),
    'module' => 'node',
  );

  // Exported permission: 'edit any faq content'.
  $permissions['edit any faq content'] = array(
    'name' => 'edit any faq content',
    'roles' => array(
      'Editor' => 'Editor',
      'administrator' => 'administrator',
    ),
    'module' => 'node',
  );

  // Exported permission: 'edit any image files'.
  $permissions['edit any image files'] = array(
    'name' => 'edit any image files',
    'roles' => array(
      'Editor' => 'Editor',
      'administrator' => 'administrator',
      'curator' => 'curator',
    ),
    'module' => 'file_entity',
  );

  // Exported permission: 'edit any incentives content'.
  $permissions['edit any incentives content'] = array(
    'name' => 'edit any incentives content',
    'roles' => array(
      'Editor' => 'Editor',
      'administrator' => 'administrator',
    ),
    'module' => 'node',
  );

  // Exported permission: 'edit any labp_service_center content'.
  $permissions['edit any labp_service_center content'] = array(
    'name' => 'edit any labp_service_center content',
    'roles' => array(
      'Editor' => 'Editor',
      'administrator' => 'administrator',
    ),
    'module' => 'node',
  );

  // Exported permission: 'edit any online_resource content'.
  $permissions['edit any online_resource content'] = array(
    'name' => 'edit any online_resource content',
    'roles' => array(
      'Editor' => 'Editor',
      'administrator' => 'administrator',
    ),
    'module' => 'node',
  );

  // Exported permission: 'edit any sbp_basic_page content'.
  $permissions['edit any sbp_basic_page content'] = array(
    'name' => 'edit any sbp_basic_page content',
    'roles' => array(
      'administrator' => 'administrator',
    ),
    'module' => 'node',
  );

  // Exported permission: 'edit any sbp_department content'.
  $permissions['edit any sbp_department content'] = array(
    'name' => 'edit any sbp_department content',
    'roles' => array(
      'Editor' => 'Editor',
      'administrator' => 'administrator',
    ),
    'module' => 'node',
  );

  // Exported permission: 'edit any sbp_landing_page content'.
  $permissions['edit any sbp_landing_page content'] = array(
    'name' => 'edit any sbp_landing_page content',
    'roles' => array(
      'Editor' => 'Editor',
      'administrator' => 'administrator',
    ),
    'module' => 'node',
  );

  // Exported permission: 'edit any video files'.
  $permissions['edit any video files'] = array(
    'name' => 'edit any video files',
    'roles' => array(
      'Editor' => 'Editor',
      'administrator' => 'administrator',
      'curator' => 'curator',
    ),
    'module' => 'file_entity',
  );

  // Exported permission: 'edit fieldable content'.
  $permissions['edit fieldable content'] = array(
    'name' => 'edit fieldable content',
    'roles' => array(
      'Editor' => 'Editor',
      'administrator' => 'administrator',
    ),
    'module' => 'fieldable_panels_panes',
  );

  // Exported permission: 'edit fieldable featured_resources'.
  $permissions['edit fieldable featured_resources'] = array(
    'name' => 'edit fieldable featured_resources',
    'roles' => array(
      'Editor' => 'Editor',
    ),
    'module' => 'fieldable_panels_panes',
  );

  // Exported permission: 'edit fieldable fieldable_panels_pane'.
  $permissions['edit fieldable fieldable_panels_pane'] = array(
    'name' => 'edit fieldable fieldable_panels_pane',
    'roles' => array(
      'Editor' => 'Editor',
      'administrator' => 'administrator',
      'marketer' => 'marketer',
    ),
    'module' => 'fieldable_panels_panes',
  );

  // Exported permission: 'edit fieldable four_column_cta'.
  $permissions['edit fieldable four_column_cta'] = array(
    'name' => 'edit fieldable four_column_cta',
    'roles' => array(
      'Editor' => 'Editor',
      'administrator' => 'administrator',
    ),
    'module' => 'fieldable_panels_panes',
  );

  // Exported permission: 'edit fieldable general_content'.
  $permissions['edit fieldable general_content'] = array(
    'name' => 'edit fieldable general_content',
    'roles' => array(
      'Editor' => 'Editor',
    ),
    'module' => 'fieldable_panels_panes',
  );

  // Exported permission: 'edit fieldable map'.
  $permissions['edit fieldable map'] = array(
    'name' => 'edit fieldable map',
    'roles' => array(
      'Editor' => 'Editor',
      'administrator' => 'administrator',
      'marketer' => 'marketer',
    ),
    'module' => 'fieldable_panels_panes',
  );

  // Exported permission: 'edit fieldable media_pane'.
  $permissions['edit fieldable media_pane'] = array(
    'name' => 'edit fieldable media_pane',
    'roles' => array(
      'Editor' => 'Editor',
      'administrator' => 'administrator',
      'marketer' => 'marketer',
    ),
    'module' => 'fieldable_panels_panes',
  );

  // Exported permission: 'edit fieldable pull_quote'.
  $permissions['edit fieldable pull_quote'] = array(
    'name' => 'edit fieldable pull_quote',
    'roles' => array(
      'Editor' => 'Editor',
    ),
    'module' => 'fieldable_panels_panes',
  );

  // Exported permission: 'edit fieldable quick_links'.
  $permissions['edit fieldable quick_links'] = array(
    'name' => 'edit fieldable quick_links',
    'roles' => array(
      'Editor' => 'Editor',
      'administrator' => 'administrator',
      'marketer' => 'marketer',
    ),
    'module' => 'fieldable_panels_panes',
  );

  // Exported permission: 'edit fieldable raw_html'.
  $permissions['edit fieldable raw_html'] = array(
    'name' => 'edit fieldable raw_html',
    'roles' => array(
      'Editor' => 'Editor',
      'administrator' => 'administrator',
      'marketer' => 'marketer',
    ),
    'module' => 'fieldable_panels_panes',
  );

  // Exported permission: 'edit fieldable section_content'.
  $permissions['edit fieldable section_content'] = array(
    'name' => 'edit fieldable section_content',
    'roles' => array(
      'Editor' => 'Editor',
      'administrator' => 'administrator',
    ),
    'module' => 'fieldable_panels_panes',
  );

  // Exported permission: 'edit fieldable text'.
  $permissions['edit fieldable text'] = array(
    'name' => 'edit fieldable text',
    'roles' => array(
      'Editor' => 'Editor',
      'administrator' => 'administrator',
      'marketer' => 'marketer',
    ),
    'module' => 'fieldable_panels_panes',
  );

  // Exported permission: 'edit fieldable three_column_cta'.
  $permissions['edit fieldable three_column_cta'] = array(
    'name' => 'edit fieldable three_column_cta',
    'roles' => array(
      'Editor' => 'Editor',
    ),
    'module' => 'fieldable_panels_panes',
  );

  // Exported permission: 'edit fieldable timeline'.
  $permissions['edit fieldable timeline'] = array(
    'name' => 'edit fieldable timeline',
    'roles' => array(
      'Editor' => 'Editor',
    ),
    'module' => 'fieldable_panels_panes',
  );

  // Exported permission: 'edit own audio files'.
  $permissions['edit own audio files'] = array(
    'name' => 'edit own audio files',
    'roles' => array(
      'Editor' => 'Editor',
      'administrator' => 'administrator',
      'curator' => 'curator',
    ),
    'module' => 'file_entity',
  );

  // Exported permission: 'edit own business_portal_sponsors content'.
  $permissions['edit own business_portal_sponsors content'] = array(
    'name' => 'edit own business_portal_sponsors content',
    'roles' => array(
      'Editor' => 'Editor',
    ),
    'module' => 'node',
  );

  // Exported permission: 'edit own faq content'.
  $permissions['edit own faq content'] = array(
    'name' => 'edit own faq content',
    'roles' => array(
      'Editor' => 'Editor',
      'administrator' => 'administrator',
    ),
    'module' => 'node',
  );

  // Exported permission: 'edit own image files'.
  $permissions['edit own image files'] = array(
    'name' => 'edit own image files',
    'roles' => array(
      'Editor' => 'Editor',
      'administrator' => 'administrator',
      'curator' => 'curator',
    ),
    'module' => 'file_entity',
  );

  // Exported permission: 'edit own incentives content'.
  $permissions['edit own incentives content'] = array(
    'name' => 'edit own incentives content',
    'roles' => array(
      'Editor' => 'Editor',
      'administrator' => 'administrator',
    ),
    'module' => 'node',
  );

  // Exported permission: 'edit own labp_service_center content'.
  $permissions['edit own labp_service_center content'] = array(
    'name' => 'edit own labp_service_center content',
    'roles' => array(
      'Editor' => 'Editor',
      'administrator' => 'administrator',
    ),
    'module' => 'node',
  );

  // Exported permission: 'edit own online_resource content'.
  $permissions['edit own online_resource content'] = array(
    'name' => 'edit own online_resource content',
    'roles' => array(
      'Editor' => 'Editor',
      'administrator' => 'administrator',
    ),
    'module' => 'node',
  );

  // Exported permission: 'edit own sbp_basic_page content'.
  $permissions['edit own sbp_basic_page content'] = array(
    'name' => 'edit own sbp_basic_page content',
    'roles' => array(
      'administrator' => 'administrator',
    ),
    'module' => 'node',
  );

  // Exported permission: 'edit own sbp_department content'.
  $permissions['edit own sbp_department content'] = array(
    'name' => 'edit own sbp_department content',
    'roles' => array(
      'Editor' => 'Editor',
      'administrator' => 'administrator',
    ),
    'module' => 'node',
  );

  // Exported permission: 'edit own sbp_landing_page content'.
  $permissions['edit own sbp_landing_page content'] = array(
    'name' => 'edit own sbp_landing_page content',
    'roles' => array(
      'Editor' => 'Editor',
      'administrator' => 'administrator',
    ),
    'module' => 'node',
  );

  // Exported permission: 'edit own video files'.
  $permissions['edit own video files'] = array(
    'name' => 'edit own video files',
    'roles' => array(
      'Editor' => 'Editor',
      'administrator' => 'administrator',
      'curator' => 'curator',
    ),
    'module' => 'file_entity',
  );

  // Exported permission: 'edit terms in business_needs'.
  $permissions['edit terms in business_needs'] = array(
    'name' => 'edit terms in business_needs',
    'roles' => array(
      'Editor' => 'Editor',
      'administrator' => 'administrator',
    ),
    'module' => 'taxonomy',
  );

  // Exported permission: 'edit terms in business_ownership'.
  $permissions['edit terms in business_ownership'] = array(
    'name' => 'edit terms in business_ownership',
    'roles' => array(
      'Editor' => 'Editor',
      'administrator' => 'administrator',
    ),
    'module' => 'taxonomy',
  );

  // Exported permission: 'edit terms in carousel'.
  $permissions['edit terms in carousel'] = array(
    'name' => 'edit terms in carousel',
    'roles' => array(
      'Editor' => 'Editor',
      'administrator' => 'administrator',
    ),
    'module' => 'taxonomy',
  );

  // Exported permission: 'edit terms in department_type'.
  $permissions['edit terms in department_type'] = array(
    'name' => 'edit terms in department_type',
    'roles' => array(
      'Editor' => 'Editor',
      'administrator' => 'administrator',
    ),
    'module' => 'taxonomy',
  );

  // Exported permission: 'edit terms in geographical_area'.
  $permissions['edit terms in geographical_area'] = array(
    'name' => 'edit terms in geographical_area',
    'roles' => array(
      'Editor' => 'Editor',
      'administrator' => 'administrator',
    ),
    'module' => 'taxonomy',
  );

  // Exported permission: 'edit terms in identifiers'.
  $permissions['edit terms in identifiers'] = array(
    'name' => 'edit terms in identifiers',
    'roles' => array(
      'administrator' => 'administrator',
    ),
    'module' => 'taxonomy',
  );

  // Exported permission: 'edit terms in industry_tags'.
  $permissions['edit terms in industry_tags'] = array(
    'name' => 'edit terms in industry_tags',
    'roles' => array(
      'Editor' => 'Editor',
      'administrator' => 'administrator',
    ),
    'module' => 'taxonomy',
  );

  // Exported permission: 'edit terms in permit_government_level'.
  $permissions['edit terms in permit_government_level'] = array(
    'name' => 'edit terms in permit_government_level',
    'roles' => array(
      'Editor' => 'Editor',
      'administrator' => 'administrator',
    ),
    'module' => 'taxonomy',
  );

  // Exported permission: 'edit terms in permit_issuing_department'.
  $permissions['edit terms in permit_issuing_department'] = array(
    'name' => 'edit terms in permit_issuing_department',
    'roles' => array(
      'Editor' => 'Editor',
      'administrator' => 'administrator',
    ),
    'module' => 'taxonomy',
  );

  // Exported permission: 'edit terms in permit_tags'.
  $permissions['edit terms in permit_tags'] = array(
    'name' => 'edit terms in permit_tags',
    'roles' => array(
      'Editor' => 'Editor',
      'administrator' => 'administrator',
    ),
    'module' => 'taxonomy',
  );

  // Exported permission: 'edit terms in question_category'.
  $permissions['edit terms in question_category'] = array(
    'name' => 'edit terms in question_category',
    'roles' => array(
      'Editor' => 'Editor',
      'administrator' => 'administrator',
    ),
    'module' => 'taxonomy',
  );

  // Exported permission: 'edit terms in required_featured_items'.
  $permissions['edit terms in required_featured_items'] = array(
    'name' => 'edit terms in required_featured_items',
    'roles' => array(
      'Editor' => 'Editor',
      'administrator' => 'administrator',
    ),
    'module' => 'taxonomy',
  );

  // Exported permission: 'edit terms in resource_type'.
  $permissions['edit terms in resource_type'] = array(
    'name' => 'edit terms in resource_type',
    'roles' => array(
      'Editor' => 'Editor',
      'administrator' => 'administrator',
    ),
    'module' => 'taxonomy',
  );

  // Exported permission: 'edit terms in sbp_tags'.
  $permissions['edit terms in sbp_tags'] = array(
    'name' => 'edit terms in sbp_tags',
    'roles' => array(
      'Editor' => 'Editor',
      'administrator' => 'administrator',
    ),
    'module' => 'taxonomy',
  );

  // Exported permission: 'edit terms in style_template'.
  $permissions['edit terms in style_template'] = array(
    'name' => 'edit terms in style_template',
    'roles' => array(
      'Editor' => 'Editor',
    ),
    'module' => 'taxonomy',
  );

  // Exported permission: 'edit terms in tags'.
  $permissions['edit terms in tags'] = array(
    'name' => 'edit terms in tags',
    'roles' => array(
      'administrator' => 'administrator',
    ),
    'module' => 'taxonomy',
  );

  // Exported permission: 'edit terms in term_alphabet'.
  $permissions['edit terms in term_alphabet'] = array(
    'name' => 'edit terms in term_alphabet',
    'roles' => array(
      'administrator' => 'administrator',
    ),
    'module' => 'taxonomy',
  );

  // Exported permission: 'edit terms in terms_definitions'.
  $permissions['edit terms in terms_definitions'] = array(
    'name' => 'edit terms in terms_definitions',
    'roles' => array(
      'Editor' => 'Editor',
      'administrator' => 'administrator',
    ),
    'module' => 'taxonomy',
  );

  // Exported permission: 'edit terms in terms_definitions_grouping'.
  $permissions['edit terms in terms_definitions_grouping'] = array(
    'name' => 'edit terms in terms_definitions_grouping',
    'roles' => array(
      'Editor' => 'Editor',
      'administrator' => 'administrator',
    ),
    'module' => 'taxonomy',
  );

  // Exported permission: 'import media'.
  $permissions['import media'] = array(
    'name' => 'import media',
    'roles' => array(
      'Editor' => 'Editor',
      'administrator' => 'administrator',
    ),
    'module' => 'media_bulk_upload',
  );

  // Exported permission: 'manipulate all queues'.
  $permissions['manipulate all queues'] = array(
    'name' => 'manipulate all queues',
    'roles' => array(
      'Editor' => 'Editor',
      'administrator' => 'administrator',
    ),
    'module' => 'nodequeue',
  );

  // Exported permission: 'manipulate queues'.
  $permissions['manipulate queues'] = array(
    'name' => 'manipulate queues',
    'roles' => array(
      'Editor' => 'Editor',
      'administrator' => 'administrator',
    ),
    'module' => 'nodequeue',
  );

  // Exported permission: 'revert revisions'.
  $permissions['revert revisions'] = array(
    'name' => 'revert revisions',
    'roles' => array(
      'Editor' => 'Editor',
      'administrator' => 'administrator',
      'curator' => 'curator',
    ),
    'module' => 'node',
  );

  // Exported permission: 'use panels in place editing'.
  $permissions['use panels in place editing'] = array(
    'name' => 'use panels in place editing',
    'roles' => array(
      'Editor' => 'Editor',
      'administrator' => 'administrator',
      'marketer' => 'marketer',
    ),
    'module' => 'panels',
  );

  // Exported permission: 'use text format filtered_html'.
  $permissions['use text format filtered_html'] = array(
    'name' => 'use text format filtered_html',
    'roles' => array(
      'administrator' => 'administrator',
      'anonymous user' => 'anonymous user',
      'authenticated user' => 'authenticated user',
      'curator' => 'curator',
      'reviewer' => 'reviewer',
    ),
    'module' => 'filter',
  );

  // Exported permission: 'use text format full_html'.
  $permissions['use text format full_html'] = array(
    'name' => 'use text format full_html',
    'roles' => array(
      'administrator' => 'administrator',
      'curator' => 'curator',
      'reviewer' => 'reviewer',
    ),
    'module' => 'filter',
  );

  // Exported permission: 'use text format raw_html'.
  $permissions['use text format raw_html'] = array(
    'name' => 'use text format raw_html',
    'roles' => array(
      'Editor' => 'Editor',
      'administrator' => 'administrator',
      'marketer' => 'marketer',
    ),
    'module' => 'filter',
  );

  // Exported permission: 'use text format userbase'.
  $permissions['use text format userbase'] = array(
    'name' => 'use text format userbase',
    'roles' => array(
      'administrator' => 'administrator',
      'authenticated user' => 'authenticated user',
    ),
    'module' => 'filter',
  );

  // Exported permission: 'view ember support mods'.
  $permissions['view ember support mods'] = array(
    'name' => 'view ember support mods',
    'roles' => array(
      'administrator' => 'administrator',
      'curator' => 'curator',
      'marketer' => 'marketer',
      'reviewer' => 'reviewer',
    ),
    'module' => 'ember_support',
  );

  // Exported permission: 'view files'.
  $permissions['view files'] = array(
    'name' => 'view files',
    'roles' => array(
      'Editor' => 'Editor',
      'administrator' => 'administrator',
      'anonymous user' => 'anonymous user',
      'authenticated user' => 'authenticated user',
      'curator' => 'curator',
    ),
    'module' => 'file_entity',
  );

  // Exported permission: 'view own files'.
  $permissions['view own files'] = array(
    'name' => 'view own files',
    'roles' => array(
      'Editor' => 'Editor',
      'administrator' => 'administrator',
      'curator' => 'curator',
    ),
    'module' => 'file_entity',
  );

  // Exported permission: 'view own private files'.
  $permissions['view own private files'] = array(
    'name' => 'view own private files',
    'roles' => array(
      'Editor' => 'Editor',
      'administrator' => 'administrator',
      'curator' => 'curator',
    ),
    'module' => 'file_entity',
  );

  // Exported permission: 'view own unpublished content'.
  $permissions['view own unpublished content'] = array(
    'name' => 'view own unpublished content',
    'roles' => array(
      'Editor' => 'Editor',
      'administrator' => 'administrator',
      'curator' => 'curator',
    ),
    'module' => 'node',
  );

  // Exported permission: 'view private files'.
  $permissions['view private files'] = array(
    'name' => 'view private files',
    'roles' => array(
      'Editor' => 'Editor',
      'administrator' => 'administrator',
      'curator' => 'curator',
    ),
    'module' => 'file_entity',
  );

  // Exported permission: 'view revisions'.
  $permissions['view revisions'] = array(
    'name' => 'view revisions',
    'roles' => array(
      'Editor' => 'Editor',
      'administrator' => 'administrator',
      'curator' => 'curator',
    ),
    'module' => 'node',
  );

  // Exported permission: 'view the administration theme'.
  $permissions['view the administration theme'] = array(
    'name' => 'view the administration theme',
    'roles' => array(
      'Editor' => 'Editor',
      'administrator' => 'administrator',
      'curator' => 'curator',
    ),
    'module' => 'system',
  );

  return $permissions;
}
