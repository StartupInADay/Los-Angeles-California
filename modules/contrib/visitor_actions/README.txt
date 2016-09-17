*******************************************************************************

Visitor Actions

Description:
-------------------------------------------------------------------------------

This module allows client-side and server side actions to be defined and sub-
scribed to.

The actions people take on your site could be of interest to you for any number
of reasons - gamification, site analytics, personalization, etc. This module
comes with some common actions predefined (e.g. visitor registers on the site,
visitor submits any form, visitor scrolls to the bottom of the page) and is
extremely extensible, allowing custom actions to be defined via an admin UI, or
in code, or indeed using Rules module.

The sub-module Visitor Actions UI provides an extremely intuitive UX for defining
these actions right from the front-end of your site: you navigate to the page
where the desired action would take place and select the thing (link, form, the
entire page) on which the action will be performed.

Once actions have been defined, it is up to the consuming code--be it a module 
for gamification, analytics, personalization or anything else--to subscribe to 
that action.


Installation & Use:
-------------------------------------------------------------------------------

1.  Enable Visitor Actions and Visitor Actions UI modules.
2.  Go to admin/structure/visitor_actions and click on "Add action in context"
3.  You can now navigate to the page where you want to add an action.
4.  Once on the correct page, click on a link or a form, or click on the "Add
page action" button if you want to add an action on the entire page.
5.  In the form that pops up, edit the name of the action as desired, select
the type of action to create, and if desired modify options in the Advanced
Options, e.g. the list of pages this action should be tied to.
6.  Click Save.
7.  Implement visitor_actions_subscribe in your module that needs to react to
this action. See visitor_actions.api.php.


For Developers:
-------------------------------------------------------------------------------
Please refer to visitor_actions.api.php for documentation on how to extend this
module.

Current maintainers:
-------------------------------------------------------------------------------
 * Katherine Bailey (katbailey) - http://drupal.org/user/172987
 * Dave Ingram (Dave.Ingram) - http://drupal.org/user/352282
