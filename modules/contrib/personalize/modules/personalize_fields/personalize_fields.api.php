<?php

/**
 * Allows alteration of the title and machine name of a campaign.
 *
 * Personalize Fields module automatically creates a campaign when a personalized
 * field is added to an entity. It generates a title and machine name for the
 * campaign based on the entity type and field name. This hook allows modules to
 * alter the generated title and machine name.
 * NOTE: It is the responsibility of the module implementing the hook to ensure that
 * if it alters the machine name, it provides a new, unique machine name. This can
 * be done with the personalize_generate_machine_name() function, passing
 * 'personalize_agent_machine_name_exists' as the second argument.
 *
 * @param array $agent_info
 *   An array with keys 'title' and 'machine_name' providing the title and machine
 *   name that have been generated for the campaign.
 * @param array $context
 *   An array with keys 'entity_type', 'entity', 'entity_id', and 'field_name'.
 */
function hook_personalize_fields_auto_agent_create_alter(&$agent_info, &$context) {

}
