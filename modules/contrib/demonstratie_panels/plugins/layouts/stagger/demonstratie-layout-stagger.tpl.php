<?php
/**
 * @file
 * Markup for theme_demonstratie_layout_stagger().
 */
?>

<div class="<?php print $classes; ?>"<?php print $attributes; ?>>
  <div<?php print $content_attributes; ?>>
    <?php if (!empty($content['header'])): ?>
      <div<?php print $header_attributes; ?>>
        <?php print $content['header']; ?>
      </div>
    <?php endif; ?>

    <?php if ($content['primary_features'] || $content['secondary_features']): ?>
      <div class="content-container clearfix">
        <?php if (!empty($content['primary_features'])): ?>
          <div<?php print $primary_features_attributes; ?>>
            <?php print $content['primary_features']; ?>
          </div>
        <?php endif; ?>

        <?php if (!empty($content['secondary_features'])): ?>
          <div<?php print $secondary_features_attributes; ?>>
            <?php print $content['secondary_features']; ?>
          </div>
        <?php endif; ?>
      </div>
    <?php endif; ?>

    <?php if ($content['tertiary_features'] || $content['quaternary_features']): ?>
      <div class="content-container clearfix">
        <?php if (!empty($content['tertiary_features'])): ?>
          <div<?php print $tertiary_features_attributes; ?>>
            <?php print $content['tertiary_features']; ?>
          </div>
        <?php endif; ?>

        <?php if (!empty($content['quaternary_features'])): ?>
          <div<?php print $quaternary_features_attributes; ?>>
            <?php print $content['quaternary_features']; ?>
          </div>
        <?php endif; ?>
      </div>
    <?php endif; ?>

    <?php if ($content['primary_supplements'] || $content['secondary_supplements'] || $content['tertiary_supplements']): ?>
      <div class="content-container clearfix">
        <?php if (!empty($content['primary_supplements'])): ?>
          <div<?php print $primary_supplements_attributes; ?>>
            <?php print $content['primary_supplements']; ?>
          </div>
        <?php endif; ?>

        <?php if (!empty($content['secondary_supplements'])): ?>
          <div<?php print $secondary_supplements_attributes; ?>>
            <?php print $content['secondary_supplements']; ?>
          </div>
        <?php endif; ?>

        <?php if (!empty($content['tertiary_supplements'])): ?>
          <div<?php print $tertiary_supplements_attributes; ?>>
            <?php print $content['tertiary_supplements']; ?>
          </div>
        <?php endif; ?>
      </div>
    <?php endif; ?>

    <?php if (!empty($content['footer'])): ?>
      <div<?php print $footer_attributes; ?>>
        <?php print $content['footer']; ?>
      </div>
    <?php endif; ?>
  </div>
</div>
