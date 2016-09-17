<?php
/**
 * @file
 * Returns the HTML for a la_calendar fieldable panels pane.
 *
 * Complete documentation for this file is available online.
 * @see https://drupal.org/node/1728164
 */
?>
<?php

//dpm($content);

// Preprocessing field_unique_id.
$unique_id = $content['field_unique_id'][0]['#markup'];

// Preprocessing field_display_type.
$display_type = $content['field_display_type']['#items'][0]['value'];

$departments = '';
if (!empty($content['field_departments']) and !empty($content['field_departments']['#items'])) {
  $departments = $content['field_departments']['#items'][0]['value'];
}


if (user_is_logged_in()) {
?>

<div>LA Calendar widget is disabled for authenticated users due to an issue with content editing.</div>

<?php
} else {
?>
<div class="lacity-calendar-container">
  <script
    src="http://calendar.dev2-cityofla.acsitefactory.com/app/loader.php?name=embed-calendar"
    data-version="v1"
    data-widget-id="0001"
    id="<?php print $unique_id; ?>"
    type="text/javascript" async>
    (function () {
      function la_cal_<?php print $unique_id; ?>_async_load() {
        var s = document.createElement('script');
        s.type = 'text/javascript';
        s.async = true;
        s.src = 'http://calendar.lacity.org/app/loader.php?name=embed-calendar';
        var head = document.getElementsByTagName('head');
        (document.getElementsByTagName("head")[0] || document.documentElement).appendChild(s);
      }

      if (window.attachEvent)
        window.attachEvent('onload', la_cal_<?php print $unique_id; ?>_async_load);
      else
        window.addEventListener('load', la_cal_<?php print $unique_id; ?>_async_load, false);
    })();
  </script>
  <div class="<?php print $unique_id; ?>-container" id="fullcalendar"
       data-display="<?php print $display_type; ?>"
       data-showshare="0"
       data-showmonth="1"
    <?php
     if (!empty($departments)) {
       ?>
       data-department="<?php print $departments; ?>"
       <?php
     }
    ?>>Loading
  </div>
</div>

<?php
}
?>
