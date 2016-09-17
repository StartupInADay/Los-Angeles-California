<?php
/**
 * @file
 * Markup for theme_demonstratie_layout_three_col().
 */
?>
<div<?php print $content_attributes; ?>>
  <?php if ($content['featured']): ?>
    <?php $wrapper = $featured_wrapper; ?>
    <div class="<?php print $wrapper; ?>">
    <?php if ($wrapper == 'row') { print '<div class="columns large-12">'; } ?>
      <div<?php print $featured_attributes; ?>>
        <?php print $content['featured']; ?>
      </div>
    <?php if ($wrapper == 'row') { print '</div>'; } ?>
    </div>
  <?php endif; ?>
  <?php if ($content['first'] || $content['second'] || $content['third']): ?>
  <div class="row">
    <div class="large-4 columns">
      <div<?php print $first_attributes; ?>>
        <?php print $content['first']; ?>
      </div>
    </div>
    <div class="large-4 columns">
      <div<?php print $second_attributes; ?>>
        <?php print $content['second']; ?>
      </div>
    </div>
    <div class="large-4 columns">
      <div<?php print $third_attributes; ?>>
        <?php print $content['third']; ?>
      </div>
    </div>
  </div>
  <?php endif; ?>
  <?php if ($content['footer_area']): ?>
    <?php $wrapper = $footer_area_wrapper; ?>  
    <div class="<?php print $wrapper; ?>">
    <?php if ($wrapper == 'row') { print '<div class="large-12 columns">'; } ?>
      <div<?php print $footer_area_attributes; ?>>
        <?php print $content['footer_area']; ?>
      </div>
    <?php if ($wrapper == 'row') { print '</div>'; } ?>    
    </div>
  <?php endif; ?>
</div>
