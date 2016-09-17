<?php
/**
 * @file   cache_warmer_db6.inc
 * @author António P. P. Almeida <appa@perusio.net>
 * @date   Thu Jan 26 21:17:50 2012
 *
 * @brief  Functions for DB handling of the cache_warmer drush command for
 *         Drupal 6.
 *
 */

/**
 * Gets the array with the URIs of the items to be hit when both the latest n
 * and updated in the latest specified time for Drupal 6.
 *
 * @param $db_spec array
 *   The DB connection parameters.
 * @param $latest integer
 *   The number of the latest items to crawl.
 * @param $updated integer
 *   The elapsed time in seconds since the first item we want to crawl.
 * @return array
 *   The array of records that fit the criteria specified containing the URIs.
 */
function cache_warmer_get_items_drupal6_both($db_spec = NULL,
                                             $latest = 0,
                                             $updated = 0,
                                             $no_aliases = FALSE) {

  $items_latest = array();
  $items_updated = array();

  $result_latest = db_query_range('SELECT nid FROM {node} WHERE status = 1 ORDER BY changed DESC', 0, $latest);
  $num_latest = db_result(
    db_query_range('SELECT COUNT(nid) FROM {node} WHERE status = 1 ORDER BY changed DESC', 0, $latest)
  );
  $result_updated = db_query(
    "SELECT nid FROM {node} WHERE changed <= '%d' AND status = 1 ORDER BY changed DESC ", $updated
  );
  $num_updated = db_result(
    db_query("SELECT COUNT(nid) FROM {node} WHERE status = 1 AND changed >= '%d' ORDER BY changed DESC", $updated)
  );
  // Whichever is greater is used.
  $result = $num_updated > $num_latest ? $result_updated : $result_latest;
  // Get just the node URIs.
  $items_uris = array();
  while($node_record = db_fetch_object($result)) {
    $items_uris[] = 'node/' . $node_record->nid;
  }

  // If the site to be crawled has no aliases return now with the item list.
  if ($no_aliases) {
    return $items_uris;
  }
  // Get all the aliases. Return the result as a keyed array: 'src' =>
  // 'dst'.
  $result_aliases =
    db_query(
      'SELECT src, dst FROM {url_alias} a WHERE src IN ('
      .
      db_placeholders($items_uris, 'int') . ')', $items_uris);

} // cache_warmer_get_items_drupal6_both

/**
 * Gets the array with the URIs of the items to be hit by the crawler for
 * Drupal 6.
 *
 * @param $db_spec array
 *   The DB connection parameters.
 * @param $latest integer
 *   The number of the latest items to crawl.
 * @param $updated integer
 *   The elapsed time in seconds since the first item we want to crawl.
 * @return array
 *   The array of records that fit the criteria specified containing the URIs.
 */
function cache_warmer_get_items_drupal6($db_spec = NULL, $latest = 0, $updated = 0, $no_aliases = FALSE) {

  // If both the latest and last n updated are given we perform the DB lookup
  // using subqueries.
  if ($latest > 0 && $updated > 0) {
    if ($no_aliases) {
      return cache_warmer_get_items_drupal6_both($db_spec, $latest, $updated, $no_aliases);
    }
    $result_aliases = cache_warmer_get_items_drupal6_both($db_spec, $latest, $updated, $no_aliases);
  }
  elseif ($updated > 0) {
    // If no aliases are used then just get the node IDs.
    if ($no_aliases) {
      $result = db_query("SELECT nid from {node} WHERE status = 1 AND changed >= '%d' ORDER BY changed DESC", $updated);
    }
    else {
      // Use a JOIN there are aliases.
      $result_aliases = db_query(
        "SELECT src, dst FROM {url_alias} a INNER JOIN {node} n ON SUBSTR(a.src, 6) = n.nid WHERE status = 1 AND changed >= '%d' ORDER BY changed DESC", $updated
      );
    }
  }
  else {
    if ($no_aliases) {
      // If no aliases are used then just get the node IDs.
      $result = db_query_range("SELECT nid from {node} WHERE status = 1 ORDER BY changed DESC", 0, $latest);
    }
    else {
      // Use a JOIN if there are aliases.
      $result_aliases = db_query_range(
        "SELECT src, dst FROM {url_alias} a INNER JOIN {node} n ON SUBSTR(a.src, 6) = n.nid WHERE status = 1 ORDER BY changed DESC", 0, $latest
      );
    }
  }
  // If no aliases return node IDs in an array.
  if ($no_aliases) {
    $items_uris = array();
    // Fill up the array with the node IDs.
    while ($node_record = db_fetch_object($result)) {
      $items_uris[] = 'node/' . $node_record->nid;
    }
    return $items_uris;
  }
  // Create the array with the results.
  $pairs = array();
  while($alias_record = db_fetch_object($result_aliases)) {
    $pairs[$alias_record->src] = $alias_record->dst;
  }

  return $pairs;
} // cache_warmer_get_items_drupal6