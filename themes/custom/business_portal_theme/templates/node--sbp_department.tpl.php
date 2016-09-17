<?php
/**
 * @file
 * Returns the HTML for a sbp_department node.
 *
 * Complete documentation for this file is available online.
 * @see https://drupal.org/node/1728164
 */
?>
<article class="node-<?php print $node->nid; ?> <?php print $classes; ?> clearfix"<?php print $attributes; ?>>

  <?php if ($title_prefix || $title_suffix || $display_submitted || $unpublished || !$page && $title): ?>
    <header>
      <?php if ($display_submitted): ?>
        <p class="submitted">
          <?php print $user_picture; ?>
          <?php print $submitted; ?>
        </p>
      <?php endif; ?>

      <?php if ($unpublished): ?>
        <mark class="unpublished"><?php print t('Unpublished'); ?></mark>
      <?php endif; ?>
    </header>
  <?php endif; ?>

  <?php
  // We hide the comments and links now so that we can render them later.
  hide($content['comments']);
  hide($content['links']);
  hide($content['field_addresses']);
  hide($content['field_link']);
  hide($content['field_phone']);
  print render($content);
  ?>

  <h3>Contact information:</h3>

  <?php print render($content['field_addresses']); ?>

  <?php print render($content['field_link']); ?>

  <?php print render($content['field_phone']); ?>

  <?php print render($content['links']); ?>

  <?php print render($content['comments']); ?>

</article>
