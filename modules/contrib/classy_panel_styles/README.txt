CONTENTS OF THIS FILE
---------------------

 * About Classy Panel Styles
 * Configuration
 * Known Issues
 * Acknowledgements

ABOUT CLASSY PANEL STYLES
-------------------------

- CPS allows themers and designers to create a simplified pool of ready-made
  styles for site builders to choose from when laying out their pages. These
  translate directly to template and class changes in the DOM in a predictable
  way.
- Bridges the gap between styles applied to the site regions and styles applied
  to the content itself... where the "theme" ends and the WYSIWYG editor begins.
- Builds on the drag-and-drop content layout freedom that Panels provides.
- Allows editors to apply ready-made styles a themer has whipped up for them —
  without having to remember class names!

CONFIGURATION
-------------

- Visit admin/config/content/classy_panel_styles.
- Clone the styles provided by the "cps_example" sub-module, or create your own.
- Create the CSS that contains the classes you specify.
- Specify the CSS file at admin/config/content/classy_panel_styles/settings.
- If desired, featurize your CPS styles and settings.
- Configure your Panels panes and regions to use the Classy Panel Styles style.

KNOWN ISSUES
------------

- When saving a panel page after modifying region style settings, you may see
  PHP notices. This is a known Panels bug.
  @see https://drupal.org/node/2098515

ACKNOWLEDGEMENTS
----------------

- The maintainers: Kendall Totten, Derek DeRaps, and Matt Davis.
- Jason Smith, for the initial concept and development work.
- Jeff Diecks, for encouraging us to make this module to happen.
- Other Mediacurrent developers who contributed to this module in its early
  stages, including but not limited to:
  - Damien McKenna,
  - Jay Callicot,
  - Alex McCabe,
  - and more.
- Mediacurrent, for funding many of the development hours.
- Various clients of Mediacurrent, including The Weather Channel, out of whose
  website development this module grew.
