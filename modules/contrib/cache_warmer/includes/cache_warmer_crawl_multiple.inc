<?php
/**
 * @file   cache_warmer_crawl_multiple.inc
 * @author António P. P. Almeida <appa@perusio.net>
 * @date   Sat Feb  4 18:02:29 2012
 *
 * @brief  Include file for cache_warmer providing parallel crawling using Nginx
 *         embedded Lua module.
 *
 */

/**
 * Crawls the site using the given list of URIs using parallel requests.
 *
 * @param $base_uri string
 *   The base URI of the site to be crawled.
 * @param $uris array
 *   The list of URIs to be crawled.
 * @param $timeout integer
 *   The timeout in seconds.
 * @param $parallel string
 *   The number of requests to issue simultaneously.
 * @param $crawler_uri string
 *   The URI of the web service that implements the parallel crawl.
 * @return array
 *   Array containing the responses,
 *   status codes and request times for each crawled URI.
 *
 */
function cache_warmer_crawl_multiple($base_uri = '', $uris = array(), $hub_pages = '',
                                     $timeout, $parallel, $crawler_uri) {

  // Getting the number of URIs to be processed each time.
  $hub_pages_uris = explode("\n", file_get_contents($hub_pages));
  // Remove the last element. It's a '\n'.
  $temp = array_pop($hub_pages_uris); // temp var necessary for PHP :(
  $m = count($hub_pages_uris); // number of hub pages
  $n = count($uris); // number of URIs
  $rem = ($n + $m) % $parallel;
  $steps = ($n + $m - $rem) / $parallel; // integer division
  // Getting the timeout of each step. Multiply each request timeout by the
  // number of simultaneous requests.
  $step_timeout = $timeout * $steps;

  // Create a new array with shifted elements.
  $all_uris = array();
  // First the hub pages.
  for ($i = 0; $i < $m; $i++) {
    // The front page is a special case.
    $all_uris[$i] = $hub_pages_uris[$i] != '<front>' ? $hub_pages_uris[$i] : '';
  }

  // The other URIs after.
  $uris_keys = array_keys($uris);
  for ($i = 0; $i < $n; $i++) {
    $all_uris[$i + $m] = $uris[$uris_keys[$i]];
  }

  $ch = curl_init();
  // cURL request basic options.
  curl_setopt_array($ch,
                    array(CURLOPT_POST => TRUE, // POST request.
                          CURLOPT_TIMEOUT => $step_timeout,
                          CURLOPT_RETURNTRANSFER => TRUE,
                          CURLOPT_URL => $crawler_uri,
                    ));

  // Main loop posting the requests according to the given parallel processes.
  $post_data = array();
  $requests = array();
  for ($i = 0; $i < $steps; $i++) {
    // Fill in the POST data array.
    for ($j = 0; $j < $parallel; $j++) {
      $post_data["data$j"] = $all_uris[$j + ($i * $parallel)];
    }
    // Send the base URI as a specific field.
    $post_data['base_uri'] = $base_uri;
    // Create an object to store the request result.
    $request = new stdClass();
    $request->timestamp = $_SERVER['REQUEST_TIME'];
    // Make the POST request.
    curl_setopt($ch, CURLOPT_POSTFIELDS, http_build_query($post_data, '', '&'));
    $request->reply = curl_exec($ch);
    // Get the remainder of the request information.
    $request->status = curl_getinfo($ch, CURLINFO_HTTP_CODE);
    $request->time = curl_getinfo($ch, CURLINFO_TOTAL_TIME);
    $requests[$i] = $request;
  }

  // The remainder of the URIs to be hit.
  if ($rem > 0) {
    $post_data = array();
    for ($k = 0; $k < $rem; $k++) {
      $post_data["data$k"] = $all_uris[$k + $steps * $parallel];
    }
    // Send the base URI as a specific field.
    $post_data['base_uri'] = $base_uri;
    // Create an object to store the request result.
    $request = new stdClass();
    $request->timestamp = $_SERVER['REQUEST_TIME'];
    // Make the POST request.
    curl_setopt($ch, CURLOPT_POSTFIELDS, http_build_query($post_data, '', '&'));
    $request->reply = curl_exec($ch);
    // Get the remainder of the request information.
    $request->status = curl_getinfo($ch, CURLINFO_HTTP_CODE);
    $request->time = curl_getinfo($ch, CURLINFO_TOTAL_TIME);
    $requests[$i] = $request;
  } // if
  // Release the cURL handler.
  curl_close($ch);

  return $requests;
} // cache_warmer_crawl_multiple