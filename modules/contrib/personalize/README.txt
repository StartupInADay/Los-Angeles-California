*******************************************************************************

Personalize

Description:
-------------------------------------------------------------------------------

This module provides a suite of extendable plug-ins and APIs for personalizing 
Drupal content. Two guiding principles of this module are that it should be just 
as easy to personalize content for anonymous users as it is for authenticated 
users, and that personalization should continue to work even when pages are fully 
cached (including in varnish and CDNs). There are five primary components to the 
module which enable extension in numerous ways:

Decision Agents
This module defines a ctools plugin for defining a type of decision agent. A
decision agent is responsible for making a decision about which piece of content
to choose, out of multiple options, for display to the site visitor. The
personalize_target sub-module provides one implementation of a Decision Agent
type, which uses fixed targeting rules to decide what to display. For example,
you could configure it to work with a Drupal user profile field you have set up
on your site and specify that users with a particular value in that field should
be shown one option while users with a different value should be shown a different
option. Other implementations of Decision Agents might connect to third-party
systems to get the decision as to which option to show a user, or they might calculate 
the information client-side based on values from a cookie or in local storage.

Option Sets
These are the sets of content variations from which a decision agent must choose
a single one to be shown. The Personalize module defines another ctools plugin
for providing different types of Option Sets. There are two implementations provided
in the sub-modules Personalize Fields and Personalize Blocks. Personalize Fields lets
you configure a multi-value field on an entity to be personalizable, such that each
value in the field is a possible option to be shown (and only one will be shown to
the site visitor, depending on the decision made by the decision agent). Personalize
Blocks lets you choose any number of blocks as options, one of which will be
displayed to the site visitor, depending on the decision made by the decision agent.

Executors
Once a decision has been made about what to show on a page, there are a variety of
things one might want to do. Personalize module ships with an Option Set that renders
all options on the page as initially hidden. The Executor used for this option set
provides a “show” operation which reveals the chosen Option. Other forms of Executors
could include an AJAX request to load content, or any other operation that can be 
performed on the page.

Goals
Certain types of decision agents need to collect data about actions the visitor takes. 
This can be for machine-learning to test the relative performance of each option and 
adapt their decision-making over time, or it can be for reporting the results of simple
A/B or Multi-variate testing. Both take place through goal-based reinforcement. In
conjunction with the Visitor Actions module, Personalize module allows you to define
actions on your site that are to be considered goals for use by decision agents in
assessing the value of each option in an option set.

Visitor Context
Visitor context is about giving the decision agent more information about the context
of the site visitor. This could be information about the page being visited, information
gleaned from the visitor's IP address, or, in the case of a logged in user, information
explicitly given to us by the user (Drupal user profile fields). The final ctools
plugin defined by this module allows modules to provide different sources of visitor
context. There is one implementation in personalize module itself, which allows you
to use Drupal user profile field values as visitor context.


Installation & Use:
-------------------------------------------------------------------------------
The following steps will help you get set up with basic personalization using
fixed targeting rules (without the need for any third-party decision-making
system.) This assumes you have one or more user profile fields set up on the site
that use the "options" field widget, so they have a predefined set of possible
values.

1.  Enable personalize, personalize_target and personalize_blocks modules.
2.  Go to admin/structure/personalize and click on "Add Personalization Campaign".
3.  Give the campaign a name, e.g. "My personalization campaign"
4.  Hit "Save campaign settings"
5.  You're now on the edit page for your new campaign. We need to configure one
more aspect of the campaign before we're ready to create content variations. Hit
the "configure" link beside the campaign.
6.  Under Visitor Contexts, you'll see all user profile fields that exist on your
site. Choose one that has a predefined set of possible values. Hit save.
7.  You should see a "Content variations" section with an edit link to the right
- click on this link, then click Add new > Personalized Block
8.  Give the block an administrative title
9.  Under "Variations", let's create 2 options. Select a block using the dropdown
in each option. You can give each option a more meaningful label than the default
Option A and Option B that are filled in for you - that way each can refer to what
you have actually chosen as the block option.
10. Hit save and you will be redirected back to the campaign edit screen where
you'll see your personalized block listed under Content Variations.
11. To edit it and add fixed targeting, click 'edit' beside Content Variations, and
then click the 'edit' link beside your personalized block. Under each option,
check the "Show to users with specific contexts" link and choose the value of
the user profile field selected above for which this option should always be
shown.
12. Click "Save content variations".
13. At the top of the campaign page, hit the start button.
14. Now all you need to do is place your personalized block somewhere on a page and
it will personalize based on the user contexts you specified.


For Developers:
-------------------------------------------------------------------------------
Please refer to personalize.api.php for documentation on how to extend this
module.

Current maintainers:
-------------------------------------------------------------------------------
 * Katherine Bailey (katbailey) - http://drupal.org/user/172987
 * Lisa Backer (eshta) - http://drupal.org/user/1951462
 * Dave Ingram (Dave.Ingram) - http://drupal.org/user/352282
 * Yuan Xie (ynx) - https://www.drupal.org/user/3161427
