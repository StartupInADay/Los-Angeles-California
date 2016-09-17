<?php

/**
 * @file field.tpl.php
 * Default template implementation to display the value of a field.
 *
 * This file is not used and is here as a starting point for customization only.
 * @see theme_field()
 *
 * Available variables:
 * - $items: An array of field values. Use render() to output them.
 * - $label: The item label.
 * - $label_hidden: Whether the label display is set to 'hidden'.
 * - $classes: String of classes that can be used to style contextually through
 *   CSS. It can be manipulated through the variable $classes_array from
 *   preprocess functions. The default values can be one or more of the
 *   following:
 *   - field: The current template type, i.e., "theming hook".
 *   - field-name-[field_name]: The current field name. For example, if the
 *     field name is "field_description" it would result in
 *     "field-name-field-description".
 *   - field-type-[field_type]: The current field type. For example, if the
 *     field type is "text" it would result in "field-type-text".
 *   - field-label-[label_display]: The current label position. For example, if
 *     the label position is "above" it would result in "field-label-above".
 *
 * Other variables:
 * - $element['#object']: The entity to which the field is attached.
 * - $element['#view_mode']: View mode, e.g. 'full', 'teaser'...
 * - $element['#field_name']: The field name.
 * - $element['#field_type']: The field type.
 * - $element['#field_language']: The field language.
 * - $element['#field_translatable']: Whether the field is translatable or not.
 * - $element['#label_display']: Position of label display, inline, above, or
 *   hidden.
 * - $field_name_css: The css-compatible field name.
 * - $field_type_css: The css-compatible field type.
 * - $classes_array: Array of html class attribute values. It is flattened
 *   into a string within the variable $classes.
 *
 * @see template_preprocess_field()
 * @see theme_field()
 *
 * @ingroup themeable
 */
$heros = $items[0]['entity']['field_collection_item'];
foreach($heros as $hero){
  $width = strtolower($hero['field_width'][0]['#markup']);
  if (!empty($hero['field_background_image'])) {
    $background_image = $hero['field_background_image']['#items']['0']['uri'];
    $background_image_path = file_create_url($background_image);
  }

  $pane_style = $hero['field_pane_style']['#items'][0]['taxonomy_term'];
  $pane_style = $pane_style->field_style_class[LANGUAGE_NONE][0]['value'];

  $hero_title = $hero['field_hero_title'][0]['#markup'];
  if (!empty($hero['field_icon'])) {
    $icon = $hero['field_icon']['#items']['0']['uri'];
    $icon_alt_text = $hero['field_icon']['#items']['0']['alt'];
    $icon_path = file_create_url($icon);
  }
  if (!empty($hero['field_subtitle_long'])) {
    $hero_subtitle = $hero['field_subtitle_long']['#items'][0]['value'];
  }
  if (!empty($hero['field_subtitle_2'])) {
    $hero_subtitle_long = $hero['field_subtitle_2']['#items'][0]['value'];
  }
  if (!empty($hero['field_description'])) {
    $hero_description = $hero['field_description']['#items'][0]['value'];
  }
  $calls_to_action = array();
  if (!empty($hero['field_calls_to_action'])) {
    foreach($hero['field_calls_to_action']['#items'] as $cta) {
        $calls_to_action[]= array('url'=> $cta['url'],'title'=>$cta['title']);
    }
  }
}
  $theme_path = drupal_get_path('theme', variable_get('theme_default', NULL));
?>
<!-- hero start -->
<div class="pane__hero pane__hero_<?php print $width; ?> style_variant <?php print $pane_style; ?> <?php print $classes; ?>">
    <?php if($width == "full") { ?>
        <div class = "hero__image--bg hero__layout--<?php print $width;?>"
            <?php if(!empty($background_image_path)): ?>
                style = "background-image: url('<?php print $background_image_path;?>')"
            <?php endif; ?>>
          <div class = "hero__image--overlay">
            <div class = "hero__content-wrapper">
              <div class="hero__icon_bottom_alignment">
                <div class="hero__headline"><?php print $hero_title; ?></div>
                <hr class = "hero__line">
              </div>
              <?php if(!empty($hero_subtitle) || !empty($hero_subtitle_long)): ?>
                <div class="hero__bottom_align--bn">
                  <?php if(!empty($hero_subtitle_long)): ?>
                    <h5 class="hero__business__name"><?php print $hero_subtitle_long; ?></h5>
                  <?php endif; ?>
                  <?php if(!empty($hero_subtitle)): ?>
                    <p class="hero__business__description">
                      <?php print $hero_subtitle; ?>
                    </p>
                  <?php endif; ?>
                </div>
              <?php endif; ?>
            </div>
          </div>
        </div>

    <?php } else{ ?>
        <!-- Half width starts -->
        <div class = "hero__image--bg hero__layout--<?php print $width; ?> ">
                <div class = "hero__content-wrapper">
                    <div class = "left" >
                        <div class = "hero__background-image"
                            <?php if (!empty($background_image_path)){ ?>
                                style = "background-image: url('<?php print $background_image_path;?>')"
                             <?php } else { ?>
                                id = "hero--left-bg"
                             <?php } ?> >
                          <div class = "hero__image--overlay">
                            <div class = "hero__header--inner">
                              <?php if (!empty($icon) || !empty($icon_alt_text)): ?>
                                  <div class = "hero__icon--mobile">
                                      <img src = "<?php print $icon_path; ?>"
                                           alt = "<?php print $icon_alt_text ?>" />
                                  </div>
                              <?php endif; ?>
                              <div class="hero__headline--half--mobile"><?php print $hero_title; ?></div>
                              <?php if(!empty($hero_subtitle) || !empty($hero_subtitle_long)): ?>
                                  <div class="hero__bottom_align--bn">
                                      <?php if(!empty($hero_subtitle_long)): ?>
                                          <h5 class="hero__business__name"><?php print $hero_subtitle_long; ?></h5>
                                      <?php endif; ?>
                                      <?php if(!empty($hero_subtitle)): ?>
                                          <p class="hero__business__description">
                                              <?php print $hero_subtitle; ?>
                                          </p>
                                      <?php endif; ?>
                                  </div>
                              <?php endif; ?>
                            </div>
                          </div>
                        </div>
                    </div>

                    <div class = "right">
                      <div class = "hero__header--inner">
                        <?php if (!empty($icon) || !empty($icon_alt_text)): ?>
                            <div class = "hero__icon">
                                <img src = "<?php print $icon_path; ?>"
                                     alt = "<?php print $icon_alt_text ?>"/>
                            </div>
                        <?php endif; ?>
                        <div class="hero__headline--half"><?php print $hero_title; ?></div>
                        <hr class = "hero__line--half">
                        <?php if(!empty($hero_description)): ?>
                          <p class="hero__description">
                             <?php print $hero_description; ?>
                          </p>
                        <?php endif; ?>

                        <?php if(!empty($calls_to_action)): ?>
                            <div class="hero__call-to-action--wrapper">
                                <?php foreach($calls_to_action as $cta){ ?>
                                    <div class="hero__call-to-action--item">
                                        <a href="<?php print $cta['url']; ?>"><?php print $cta['title']; ?></a>
                                    </div>
                                <?php } ?>
                            </div>
                        <?php endif; ?>
                      </div>
                    </div>
                </div>
            </div>

    <?php } ?>
</div>

