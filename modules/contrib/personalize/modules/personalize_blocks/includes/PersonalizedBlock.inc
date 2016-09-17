<?php

/**
 * Class for rendering a personalized block.
 */
class PersonalizedBlock {

  public function __construct($blocks, $executor = 'show') {
    $this->blocks = $blocks;
    $this->executor = $executor;
  }

  public function build() {
    switch ($this->executor) {
      // Callback executor determines the option and then displays after
      // an Ajax callback.
      case 'callback':
        $default_block = array_shift($this->blocks);
        return array(
          'default_block' => $this->buildBlock($default_block['bid']),
        );
        break;

      // Show executor renders all blocks and then determines which one to show.
      default:
        $render_array = array(
          'sub_blocks' => array(
          )
        );
        $renderable_blocks = array();
        foreach ($this->blocks as $block) {
          $block_ra = $this->buildBlock($block['bid']);
          if (!empty($block_ra)) {
            $renderable_blocks[] = $block_ra;
          }
        }
        if (empty($renderable_blocks)) {
          throw new Exception('No available blocks to render');
        }
        $render_array['sub_blocks'] += $renderable_blocks;
        return $render_array;
    }
  }

  public function buildDefault() {
    $default_block = array_shift($this->blocks);
    return $this->buildBlock($default_block['bid']);
  }

  /**
   * Extracts the module and delta for a block from the personalized block id.
   *
   * @param $bid
   *   The personalized block id stored with option sets.
   * @return array
   *   An array with two keys:
   *   - the module name
   *   - the block delta
   */
  public static function extractBlockId($bid) {
    $pos = strpos($bid, '_delta_');
    $module = drupal_substr($bid, 0, $pos);
    $delta = drupal_substr($bid, $pos + 7);
    return array($module, $delta);
  }

  /**
   * Builds specific block data and optionally generates renderable
   * output for personalization region.
   *
   * @param $bid
   *   The id of the block to build.
   * @param bool $render
   *   True builds renderable array, False returns data only; defaults to true.
   */
  public function buildBlock($bid, $render = TRUE) {
    $output = array();
    list($module, $delta) = self::extractBlockId($bid);
    // Make sure the user can access the block.
    if ($this->accessBlock($module, $delta)) {
      $block = block_load($module, $delta);
      $block->region = 'personalized_blocks';
      $block->personalized = TRUE;
      if (!$render) {
        return $block;
      }
      if ($block_arr = _block_render_blocks(array($block))) {
        // Add the hide_title option to make this configurable?
        //$block_arr["{$block->module}_{$block->delta}"]->subject = FALSE;
        if (!empty($block_arr["{$block->module}_{$block->delta}"]->content)) {
          $build = _block_get_renderable_array($block_arr);
          $output = $build;
        }
      }
    }
    return $output;
  }

  /**
   * Checks if the current user can access a block.
   */
  private function accessBlock($module, $delta) {
    // Get current user's rids.
    global $user;
    $rids = array_keys($user->roles);

    // Get authorized rids.
    $authorized_rids = db_select('block_role', 'br')
      ->fields('br', array('rid'))
      ->condition('module', $module, '=')
      ->condition('delta',  $delta,  '=')
      ->execute()
      ->fetchCol('rid');

    // Return whether the user can access the block:
    // - Either all roles have access - no record in {block_role}
    // - Or only specific roles have access - in which case rids should match.
    return (count($authorized_rids) == 0)
      || (count(array_intersect($authorized_rids, $rids)) != 0);
  }
}

