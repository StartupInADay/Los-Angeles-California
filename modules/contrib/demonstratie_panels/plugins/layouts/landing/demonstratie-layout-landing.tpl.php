<?php
/**
 * @file
 * Markup for theme_demonstratie_layout_landing().
 */
?>

<div class="<?php print $classes; ?>"<?php print $attributes; ?>>
  <div<?php print $content_attributes; ?>>
    <?php if (!empty($content['primary_features'])): ?>
      <div<?php print $primary_features_attributes; ?>>
        <?php print $content['primary_features']; ?>
      </div>
    <?php endif; ?>
    <?php if (!empty($content['secondary_features']) || !empty($content['tertiary_features'])): ?>
      <div class="row">
        <div class="large-12 columns">
          <div class="feature-row alternate-scheme clearfix">
            <?php if (!empty($content['secondary_features'])): ?>
              <div<?php print $secondary_features_attributes; ?>>
                <?php print $content['secondary_features']; ?>
              </div>
            <?php endif; ?>

            <?php if (!empty($content['tertiary_features'])): ?>
              <div<?php print $tertiary_features_attributes; ?>>
                <?php print $content['tertiary_features']; ?>
              </div>
            <?php endif; ?>
          </div>
        </div>
      </div>
    <?php endif; ?>
     <?php foreach($content as $region => $region_content): ?>
      <?php if ($region != 'primary_features' && $region != 'secondary_features' && $region != 'tertiary_features' && !empty($region_content)): ?>
        <div class="row">
          <div class="large-12 columns">
            <div<?php print ${$region . '_attributes'}; ?>>
            <?php print $region_content; ?>
            </div>
          </div>
        </div>
      <?php endif; ?>
    <?php endforeach; ?>
  </div>
</div>