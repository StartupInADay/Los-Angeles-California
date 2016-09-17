<?php
/**
 * @file
 * Markup for theme_demonstratie_layout_single().
 */
?>
<div<?php print $content_attributes; ?>>
  <?php foreach($content as $region => $region_content): ?>
    <?php $wrapper = ${$region . '_wrapper'}; ?>
    <?php if (!empty($region_content)): ?>
      <div class="<?php print $wrapper; ?>">
        <?php if ($wrapper == 'row') { print '<div class="columns large-12">'; } ?>
          <div<?php print ${$region . '_attributes'}; ?>>
            <?php print $region_content; ?>
          </div>
        <?php if ($wrapper == 'row') { print '</div>'; } ?>
      </div>
    <?php endif; ?>
  <?php endforeach; ?>
</div>
