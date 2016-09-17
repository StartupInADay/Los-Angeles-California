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

if (!empty($content['field_pane_style']) and !empty($content['field_pane_style'][0])) {
  $pane_style = $content['field_pane_style']['#items'][0]['taxonomy_term'];
  $pane_style = $pane_style->field_style_class[LANGUAGE_NONE][0]['value'];
} else {
  $pane_style = '';
}

$field_featured_resources = array();

if (!empty($content['field_featured_resources']) and !empty($content['field_featured_resources']['#items'])) {
  $field_featured_resources = $content['field_featured_resources']['#items'];
}


?>
  <div class="panel__featured_resources style_variant <?php print $pane_style; ?> <?php print $classes; ?>" <?php print $attributes; ?> >
    <div class = "featured-resources__container dynamic-layout--wrapper">
  <?php
  foreach ($field_featured_resources as $key => $resource) {
    $resource = $resource['entity'];
    $hero = field_collection_item_load($resource->field_hero[LANGUAGE_NONE][0]['value']);
    ?>

      <div class="featured-resources__block dynamic-layout--item">
        <div class = "featured-resources__block--inner">
          <div class="featured-resources__title"><?php print $resource->title; ?></div>
          <div class="featured-resources__description"><?php print $hero->field_description[LANGUAGE_NONE][0]['safe_value']; ?></div>
          <div class="featured-resources__button">
            <a href="<?php print drupal_get_path_alias('node/' . $resource->nid); ?>">DETAILS</a>
          </div>
        </div>
      </div>
    <?php
  }
  ?>
      </div>

  </div>

