<?php
/**
 * @file
 * Returns the HTML for a four_column_cta fieldable panels pane.
 *
 * Complete documentation for this file is available online.
 * @see https://drupal.org/node/1728164
 */
?>
<?php

$pane_title = $content['title']['#value'];
$pane_style = $field_pane_style[0]['taxonomy_term'];
$pane_style = $pane_style->field_style_class[LANGUAGE_NONE][0]['value'];

?>
<div class="panel__icons style_variant <?php print $pane_style; ?> <?php print $classes; ?>" <?php print $attributes; ?>>
  <?php if (!empty($pane_title)) {
  ?>
    <hr class="panel__header-line_thin"/>
  <?php
  } ?>
  <?php if (!empty($content['field_component_body'])) { ?>
  <div class="three-col-body"><?php print $content['field_component_body'][0]['#markup']; ?></div>
  <?php } ?>
  <div class="three-col-wrapper">
      <?php
      foreach ($field_three_ctas as $cta_key => $cta_value) {
        if (!empty($cta_value)) {
          $cta_id = $cta_value['value'];
          $cta_field_collection = entity_load('field_collection_item', array($cta_id));
          $cta_field_collection = $cta_field_collection[$cta_id];

          $cta_title = $cta_field_collection->field_title[LANGUAGE_NONE][0]['value'];
          $cta_description = $cta_field_collection->field_description[LANGUAGE_NONE][0]['value'];
          if (!empty($cta_field_collection->field_icon)) {
            $cta_icon = file_create_url($cta_field_collection->field_icon[LANGUAGE_NONE][0]['uri']);
            $cta_icon_alt = $cta_field_collection->field_icon[LANGUAGE_NONE][0]['alt'];
          }
          if (!empty($cta_field_collection->field_link)) {
            $cta_link = $cta_field_collection->field_link[LANGUAGE_NONE];
          }
      ?>
          <div class="panel__section">
          <?php if (!empty($cta_icon) || !empty($cta_icon_alt)): ?>
            <div class="panel__icon"><img
                alt="<?php print $cta_icon_alt; ?>"
                src="<?php print $cta_icon; ?>"/></div>
          <?php endif; ?>
            <h3 class="panel__icon_header"><?php if (!empty($cta_title)): print $cta_title; endif; ?></h3>
            <hr class="panel__header-line_thin"/>
            <p class="panel__paragraph">
              <?php
              if (!empty($cta_description)) {
                print $cta_description;
              }
              ?>
            </p>
            <?php if (!empty($cta_link)) { ?>
            <div class="panel__buttons">
            <?php
            foreach ($cta_link as $link) {
              ?>
              <a class="panel__button" href="<?php print $link['url']; ?>"><?php print $link['title']; ?></a>
              <?php
            }
            ?>
            </div>
            <?php } ?>

          </div>
      <?php
        }
      }

      ?>

  </div>
</div>