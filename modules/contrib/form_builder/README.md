[![Build Status (7.x-1.x)](https://travis-ci.org/torotil/form_builder.svg?branch=7.x-1.x)](https://travis-ci.org/torotil/form_builder)

## Overview

This is a Drupal module that provides an interface for editing and configuring forms. It started out as a module to edit Drupal7 Form-API arrays but has been extended since then to edit [webforms](https://www.drupal.org/project/webform) and others. In theory it can manage every list of configurable items.

### Features

* Edit forms by drag&droping form elements.
* Immediately get a preview of the form.

### Installation

Just install it like [any other drupal module](https://www.drupal.org/documentation/install/modules-themes/modules-7).

### Requirements

* [Options Element](https://www.drupal.org/project/options_element)
* _PHP 5.4_ or higher.

### Integrations / Modules based on form_builder

* [webform](https://www.drupal.org/project/webform) - enable the form_builder_webform sub-module.


## Development

### Maintenance status

The 7.x-1.x branch is actively developed. The 6.x-1.x branch is only minimally maintained and will receive security fixes only.

### Contributing

Apart from contributing code there is numerous other ways to contribute:

* Triage bugs: Try to confirm reported bugs. Provide steps-by-step instructions to reproduce them preferably starting from a clean Drupal installation.
* [Review and test patches](https://www.drupal.org/patch/review): Does the patch really fix the issue? Does it have any unwanted side-effects?
* Re-roll patches against the latest dev-version if needed.
* Write tests.
* Add documentation.

If you want to help out feel free to use either the [Drupal issue queue](https://www.drupal.org/project/issues/form_builder) or pitch in on [github](https://github.com/torotil/form_builder).

### Executing the tests

To execute the tests you need [phpunit](https://phpunit.de) and [upal](https://github.com/torotil/upal).

You also need a drupal installation that has form_builder enabled.

Once everything is set up correctly you can execute the tests with a command like:

    export UPAL_ROOT=path/to/the/drupal/root
    export UPAL_DB_URL=mysql://test:@localhost/test
    export UPAL_WEB_URL=http://url.to.your.installation
    phpunit -c phpunit.xml --bootstrap path/to/upal/bootstrap.php tests
