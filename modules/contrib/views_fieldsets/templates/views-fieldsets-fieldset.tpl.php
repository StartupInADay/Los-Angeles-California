
<<?php print $tag; ?> class="<?php print $classes; ?>"<?php print $attributes; ?>>

  <?php if ($legend_tag): ?>
    <<?php print $legend_tag; ?>>
      <span class="fieldset-legend"><?php print $legend; ?></span>
    </<?php print $legend_tag; ?>>
    <div class="fieldset-wrapper">
  <?php endif; ?>

  <?php foreach ($fieldset_fields as $name => $field): ?>
    <?php print @$field->separator . $field->wrapper_prefix . $field->label_html . $field->content . $field->wrapper_suffix; ?>
  <?php endforeach; ?>

  <?php if ($legend_tag): ?>
    </div><?php /* .fieldset-wrapper */ ?>
  <?php endif; ?>

</<?php print $tag; ?>>
