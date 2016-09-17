<?php
/**
 * @file
 * Returns the HTML for a general_content fieldable panels pane.
 *
 * Complete documentation for this file is available online.
 * @see https://drupal.org/node/1728164
 */
?>
<?php
if (!empty($content['field_intro_title']) and !empty($content['field_intro_title'][0])) {
  $pane_title = $content['field_intro_title'][0]['#markup'];
} else {
  $pane_title = '';
}
if (!empty($content['field_pane_style']) and !empty($content['field_pane_style'][0])) {
  $pane_style = $content['field_pane_style']['#items'][0]['taxonomy_term'];
  $pane_style = $pane_style->field_style_class[LANGUAGE_NONE][0]['value'];
} else {
  $pane_style = '';
}
if (!empty($content['field_description']) and !empty($content['field_description'][0])) {
  $pane_body = $content['field_description'][0]['#markup'];
}
if (!empty($content['field_call_to_action']) and !empty($content['field_call_to_action'][0])) {
  $cta = $content['field_call_to_action'][0];
}
?>
  <div class="panel__general_content style_variant <?php print $pane_style; ?> <?php print $classes; ?>" <?php print $attributes; ?> >
    <?php
    if (!empty($pane_title)) { ?>
      <div
        class="panel__general_content__title"><?php print $pane_title; ?></div>
      <?php
    }
    ?>
        <?php

        if (!empty($pane_body)) { ?>
          <div class="panel__general_content__body">
            <?php print $pane_body; ?>
          </div>
        <?php
        }

        if (!empty($cta)) { ?>
          <div class="panel__button">
            <a href="<?php print $cta['#element']['url']; ?>"><?php print $cta['#element']['title']; ?></a>
          </div>
          <?php
        }

        if (!empty($field_calls_to_action)):
        ?>
        <div class="panel__button_wrapper">
          <?php
          foreach ($field_calls_to_action as $key => $button) {
            ?>
            <div class="panel__button">
              <a href="<?php print $button['url']; ?>"><?php print $button['title']; ?></a>
            </div>
            <?php
          }
          ?>
        </div>
        <?php
        endif;
        ?>
  </div>


