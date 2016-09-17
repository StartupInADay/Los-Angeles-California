<?php
/**
 * @file
 * Returns the HTML for a pull_quote fieldable panels pane.
 *
 * Complete documentation for this file is available online.
 * @see https://drupal.org/node/1728164
 */
?>
<?php

$pane_title = $content['title']['#value'];
$quote = $content['field_quote'][0]['#markup'];
$author = $content['field_author'][0]['#markup'];
$subtitle = $content['field_quote_subtitle'][0]['#markup'];
if (!empty($content['field_pane_style']) and !empty($content['field_pane_style'][0])) {
  $pane_style = $content['field_pane_style']['#items'][0]['taxonomy_term'];
  $pane_style = $pane_style->field_style_class[LANGUAGE_NONE][0]['value'];
} else {
  $pane_style = '';
}
if (!empty($field_component_body)) {
  $pane_body = $field_component_body[0]['value'];
}
if (!empty($content['field_quote_image']) and !empty($content['field_quote_image'][0])) {
  $quote_image = file_create_url($content['field_quote_image'][0]['#item']['uri']);
  $quote_image_alt = $content['field_quote_image'][0]['#item']['alt'];
} else {
  $quote_image = '';
}

?>
  <div class="panel__pull_quote style_variant <?php print $pane_style; ?> <?php print $classes; ?>" <?php print $attributes; ?> >
    <div class="panel__content_wrapper">
    <?php if (!empty($quote_image)): ?>
    <div class="panel__quote_col_1">
      <div class="panel__quote_image">
        <img src="<?php print $quote_image; ?>"
             alt="<?php print $quote_image_alt; ?>"/>
      </div>
    </div>
    <?php endif; ?>
    <div class="panel__quote_col_2">
      <div class="panel__quote_quote">
        <?php print $quote; ?>
      </div>

      <?php
      if(!empty($author) or !empty($subtitle)):
      ?>

      <hr class="panel__quote-line"/>

      <div class="panel__quote_attribution">
        <?php if (!empty($author)): ?>
        <div class="panel__quote_author">
          <?php print $author; ?>
        </div>
        <?php endif; ?>
        <?php if (!empty($subtitle)): ?>
        <div class="panel__quote_subtitle">
          <?php print $subtitle; ?>
        </div>
        <?php endif; ?>
      </div>
      <?php
      endif;
      ?>
      </div>
    </div>
  </div>

