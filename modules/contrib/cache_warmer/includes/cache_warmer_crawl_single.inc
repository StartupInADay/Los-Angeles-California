<?php
/**
 * @file   cache_warmer_crawl_single.inc
 * @author António P. P. Almeida <appa@perusio.net>
 * @date   Sat Feb  4 18:04:29 2012
 *
 * @brief  Provides the function crawling a site in single threaded mode for cache_warmer.
 *
 *
 */

/**
 * Crawls the site using the given list of URIs using a single thread.
 *
 * @param $base_uri string
 *   The base URI of the site to be crawled.
 * @param $uris array
 *   The list of URIs to be crawled.
 * @param $hub_pages string
 *   The filename of hub pages to load.
 * @param $credentials array
 *   If not empty, the username and password with which to log in.
 * @param $timeout integer
 *   The timeout in seconds.
 *
 * @return array
 *   Array containing the status codes and request times for each crawled URI.
 *
 */
function cache_warmer_crawl_single($base_uri = '', $uris = array(), $hub_pages = '', $timeout, $credentials) {

  // Initialize cURL.
  $ch = curl_init();

  // Use any credentials we were given for logging in.
  if (!empty($credentials)) {
    $login_url = $base_uri . "/user/login";

    // Set cURL options for logging in.
    curl_setopt($ch, CURLOPT_URL, $login_url);
    curl_setopt($ch, CURLOPT_HEADER, TRUE);
    curl_setopt($ch, CURLOPT_COOKIEJAR, "/tmp/cookie.txt");
    curl_setopt($ch, CURLOPT_COOKIEFILE, "'/tmp/cookie.txt");
    curl_setopt($ch, CURLOPT_FOLLOWLOCATION, 1);
    curl_setopt($ch, CURLOPT_RETURNTRANSFER, 1);
    curl_setopt($ch, CURLOPT_POST, 1);
    curl_setopt($ch, CURLOPT_USERAGENT, 'Cache Warmer Drupal module');

    // Set the POST data.
    $postdata = array(
      "name" => $credentials['username'],
      "pass" => $credentials['password'],
      "form_id" => "user_login",
      "op" => "Log in",
    );
    curl_setopt($ch, CURLOPT_POSTFIELDS, $postdata);

    // Fire off the request and check if it worked.
    $result = curl_exec($ch);
    $headers = curl_getinfo($ch);
    if ($headers['url'] == $login_url) {
      watchdog(
        'cache_warmer',
        t('Login failed! Warming caches unauthenticated...'),
        array(),
        WATCHDOG_ERROR
      );
    }
  }

  // Initialize the list of requests.
  $requests = array();

  // cURL request basic options.
  curl_setopt_array($ch, array(
    CURLOPT_NOBODY => TRUE, // HEAD request.
    CURLOPT_TIMEOUT => $timeout,
  ));

  // Start the timer.
  $timer_start = time();

  // We first deal with the hub pages.
  if (!empty($hub_pages)) {
    $fp = fopen($hub_pages, 'r'); // get the handle
    if (!$fp) {
      drush_set_error(CACHE_WARMER_CANNOT_OPEN_HUBPAGES,
                      dt('Cannot open the hub pages file.'));
    }
    // Crawl the hub pages URIs.
    while (($line = fgets($fp)) !== FALSE) {
      $uri = trim($line); // remove white space on both ends
      // If the uri is '<front>' then it's a special case. The front page.
      $uri = $uri == '<front>' ? '' : $uri;
      // Create an object to store the request result.
      $request = new stdClass();
      $request->timestamp = $_SERVER['REQUEST_TIME'];
      curl_setopt($ch, CURLOPT_URL, $base_uri . '/' . $uri);
      curl_exec($ch);
      $request->status = curl_getinfo($ch, CURLINFO_HTTP_CODE);
      $request->time = curl_getinfo($ch, CURLINFO_TOTAL_TIME);
      $requests[$uri] = $request;
    }
    // Close the file handle.
    fclose($fp);
  }

  // Main loop. We store the total request time and status.
  foreach ($uris as $uri) {
    // Create an object to store the request result.
    $request = new stdClass();
    $request->timestamp = $_SERVER['REQUEST_TIME'];
    curl_setopt($ch, CURLOPT_URL, $base_uri . '/' . $uri);
    curl_exec($ch);
    $request->status = curl_getinfo($ch, CURLINFO_HTTP_CODE);
    $request->time = curl_getinfo($ch, CURLINFO_TOTAL_TIME);
    $requests[$uri] = $request;
  }

  // Record the total time.
  watchdog(
    'cache_warmer',
    t('Total cache-warming time was @time seconds.'),
    array('@time' => time() - $timer_start)
  );

  // Release the cURL handler.
  curl_close($ch);

  return $requests;
} // cache_warmer_crawl_single
