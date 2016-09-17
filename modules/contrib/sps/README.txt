## Running Tests:

Run the following command from Drupal root replacing http://lsd.dev:9999/ with your URL

php scripts/run-tests.sh --color --verbose --url http://lsd.dev:9999/ <group>

Test Groups
* SPS - Unit Tests
* SPSIntegration - Integration Unit Tests
* SPSInteractive - Web Tests
* SPSInteractiveIntegration - Integration Web Tests
* SPS_FPP - Integration with Fieldable Panel Panes

## SPS (Site Preview System)

Primary Namespace is Drupal\\sps
Files are autoloaded using the xautoload module.

## Required Patches to Core

http://drupal.org/project/1728568: http://drupal.org/files/1728568_add_alias_to_node_query.patch
http://drupal.org/project/1730874: http://drupal.org/files/1730874_0.patch

