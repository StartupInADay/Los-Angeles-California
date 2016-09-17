<?php
/**
 * @file
 * Returns the HTML for a timeline fieldable panels pane.
 *
 * Complete documentation for this file is available online.
 * @see https://drupal.org/node/1728164
 */
?>
<?php

// Pane style.
if (!empty($content['field_pane_style']) and !empty($content['field_pane_style'][0])) {
  $pane_style = $content['field_pane_style']['#items'][0]['taxonomy_term'];
  $pane_style = $pane_style->field_style_class[LANGUAGE_NONE][0]['value'];
} else {
  $pane_style = '';
}

// Timeline field collection list.
if (!empty($content['field_timeline_items']) and !empty($content['field_timeline_items'][0])) {
  $items = $content['field_timeline_items']['#items'];
} else {
  $items = array();
}

?>
<div class="panel__timeline style_variant <?php print $pane_style; ?> <?php print $classes; ?>" <?php print $attributes; ?>>

    <?php

    /*
    <div class="timeline-arrow_left" id="timeline-tmpl_arrow_left"></div>
    <div class="timeline-arrow_right" id="timeline-tmpl_arrow_right"></div>
    */
    ?>

    <?php

    /*
    <div id="timeline_content" >
      <div id="timeline-content_area"  class="timeline-content_area">
      </div>
    </div>
    */
    ?>
    <div class="timeline__container">

      <div class="timeline-illustration">
        <div class="timeline-large_cloud"></div>
        <div class="timeline-bldgs_move1"></div>
        <div class="timeline-bldgs_move2"></div>
        <div class="timeline-bldgs_move3"></div>

        <div class="timeline-small_cloud"></div>
        <div class="timeline-blimp"></div>
        <div class="timeline-truck"></div>
        <div class="timeline-food_truck"></div>
      </div>
      <div class="timeline-items slick-carousel">

    <?php
      foreach ($items as $item) {
        $item = field_collection_item_load($item['value']);
        $title = $item->field_timeline_title[LANGUAGE_NONE][0]['safe_value'];
        if (empty($item->field_timeline_body)) {
          $body = '';
        }
        else {
          $body = $item->field_timeline_body[LANGUAGE_NONE][0]['value'];
        }
        $cta = $item->field_timeline_cta[LANGUAGE_NONE][0];
        // Banner image.
        if (!empty($item->field_banner_image) and !empty($item->field_banner_image[LANGUAGE_NONE][0])) {
          $banner_image = file_create_url($item->field_banner_image[LANGUAGE_NONE][0]['uri']);
          $banner_image_alt = $item->field_banner_image[LANGUAGE_NONE][0]['alt'];
        } else {
          $banner_image = '';
        }
      ?>
        <div class="timeline__item">
          <div class = "timeline__item--inner">
            <div class="timeline__item__title"><?php print $title; ?></div>
            <hr class="timeline__item__hr"/>
            <div class="timeline__item__body"><?php print $body; ?></div>
            <div class="timeline__item__button"><a href="<?php print $cta['url']; ?>"><?php print $cta['title']; ?></a></div>
          </div>
        </div>
      <?php
      }
    ?>
    </div>

  </div>
</div>
