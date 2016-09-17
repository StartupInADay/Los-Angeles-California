<?php
/**
 * @file
 * Markup for theme_demonstratie_layout_landing().
 */
?>

<div class="<?php print $classes; ?>"<?php print $attributes; ?>>
  <div class="medium-third-x-2 row-item-first" >
   <div class="feature-row clearfix">
    <?php if (!empty($content['primary_features'])): ?>
      <div class="region-featured-medium row-item-first">
        <?php print $content['primary_features']; ?>
      </div>
    <?php endif; ?>
        <?php if (!empty($content['secondary_features'])): ?>
          <div class="region-featured-medium row-item-last">
            <?php print $content['secondary_features']; ?>
          </div>
        <?php endif; ?>
      </div>
        <?php if (!empty($content['main_content'])): ?>
          <div>
            <?php print $content['main_content']; ?>
          </div>
        <?php endif; ?>

  </div>
    <div class="medium-third-x-1 row-item-last">
    <?php if (!empty($content['sidebar'])): ?>
         <div>
            <?php print $content['sidebar']; ?>
          </div>
    <?php endif; ?>
    </div>

</div>
