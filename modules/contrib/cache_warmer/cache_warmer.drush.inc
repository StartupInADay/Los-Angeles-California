<?php
/**
 * @file   cache_warmer.drush.inc
 * @author António P. P. Almeida <appa@perusio.net>
 * @date   Tue Jan 10 21:44:07 2012
 *
 * @brief Implements a cache warmer issuing HTTP requests to a site following
 *        certain criteria.
 *
 */

// __DIR__ was introduced in PHP 5.3.0.
if (!defined('__DIR__')) {
  define('__DIR__', dirname(__FILE__));
}

// The default timeout in seconds for each request.
define('CACHE_WARMER_DEFAULT_TIMEOUT', 20);

// The link to the PHP documentation on the cURL extension.
define('CACHE_WARMER_CURL_PHP_LINK', 'http://php.net/manual/en/book.curl.php');

/**
 * Implements hook_drush_help().
 */
function cache_warmer_drush_help($section) {
  switch ($section) {
    case 'drush:cache-warmer':
      return dt('Keeps a cache warm by issuing HTTP requests to a site following certain criteria');
  }
} // cache_warmer_drush_help

/**
 * Implements hook_drush_command().
 */
function cache_warmer_drush_command() {
  $items = array();
  // The command options.
  $options = array(
    'latest-n' => 'Hit the URIs latest <n> content items: keep the cache primed with them',
    'updated-last' => 'Hit the URIS for the content items updated in the last <s> seconds (accepts strotime strings)',
    'hub-pages-file' => 'Path to a file containing a bunch of hub pages for a site',
    'timeout' =>
    sprintf("The timeout in seconds for each URI hit (default: %ds)", DRUSH_CACHE_WARMER_DEFAULT_TIMEOUT),
    'parallel' => 'The number of requests to issue in parallel (requires Nginx with Lua)',
    'crawler-service-uri' => 'The URI of the crawler web service',
    'no-aliases' => 'Do not use aliases when hitting the URIs',
    'username' => 'The username with which to log into Drupal, if running in serial mode. Anonymous access is the default. If set in parallel mode, this option will be ignored.',
    'password' => 'The password associated with the specified username. See "--username".',
    'trace' => 'Output debugging information.',
  );
  // The provided commands.
  $items['cache-warmer'] = array(
    'callback' => 'cache_warmer_execute',
    'description' => 'Keeps an external cache primed by issuing HTTP requests according to several criteria.',
    'bootstrap' => DRUSH_BOOTSTRAP_DRUPAL_DATABASE,
    'arguments' => array('site' => 'Site base URI.'),
    'examples' => array(
      'drush cache-warmer --latest-n=300 --hub-pages-file=hub_pages.txt --timeout=5 http://example.com'
      => 'Hit the URIs for the 300 content items that where updated last, crawl the hub pages listed in hub_pages.txt for the example.com site with a timeout of 5 seconds for each request.',
      'drush cache-warmer --update-last="-4 hours" --timeout= --hub-pages-file=hpages.txt --timeout=10 http://foobar.com.'
      => 'Hit the URIs for the content items updated in the last 4 hours, crawl the hub pages listed in hub_pages.txt for the foobar.com site with a timeout of 10 seconds.',
      'drush cache-warmer --hub-pages-file=hub_pages.txt --username=foo --password=bar http://example.com'
      => 'Crawl only the hub pages listed in hub_pages.txt, with no timeout, logging as as user "foo" with password "bar" for the example.com site.',
    ),
    'options' => $options,
    'aliases' => array('cw'),
  );
  return $items;
} // cache_warmer_drush_command

/**
 * Validates the given base URI of the site to be crawled.
 *
 * @param $url string
 *   The base URI of the site to be crawled.
 * @return string or boolean
 *   The base URI or FALSE if there's an error.
 *
 */
function cache_warmer_validate_url($url = '') {
  // If the argument is missing we try to get from an alias.
  if (empty($url)) {
    // Try to get the site alias if given. We cannot use the '@self' alias
    // since it requires a full bootstrap.
    $alias = drush_get_context('alias');
    // If there's an alias given we try to get the base URI from the alias.
    if (array_key_exists('uri', $alias)) {
      $base_uri = $alias['uri'];
      drush_log(dt('Using base URI: @base-uri', array('@base-uri' => $base_uri)), 'warning');
    }
    else {
      return drush_set_error('CACHE_WARMER_NO_BASE_URI_FAIL',
                             dt('You must specify a base URI. Either through a site alias or explicitly'));
    }
  }
  else {
    $base_uri = $url;
  }
  // Parse the URL.
  $components = parse_url($base_uri);
  // Check if the URI is well formed.
  if ($components === FALSE) {
    return drush_set_error('CACHE_WARMER_URI_COMPONENTS_FAIL',
                           dt('Invalid base URI @uri given.', array('@uri' => $base_uri)));
  }
  // Get the URI scheme and verify it.
  $scheme = strtolower($components['scheme']);
  if ($scheme != 'http' && $scheme != 'https') {
    if (empty($scheme)) {
      return drush_set_error('CACHE_WARMER_EMPTY_SCHEME', dt('Invalid URI scheme given.'));
    }
    else {
      return drush_set_error('CACHE_WARMER_INVALID_SCHEME',
                             dt('Invalid URI scheme @scheme given.',
                                array('@scheme' => $scheme)), 'error');
    }
  }
  // Check if we have a query string.
  if (!empty($components['query'])) {
    return drush_set_error(
      'CACHE_WARMER_URL_QUERY_GIVEN',
      dt('Query string @qstring not allowed.',
         array('@qstring' => $components['query'])));
  }
  // Check if we have a fragment.
  if (!empty($components['fragment'])) {
    return drush_set_error('CACHE_WARMER_URL_FRAGMENT_GIVEN',
                           dt('Fragment @fragment not allowed.',
                              array('@fragment' => $components['fragment'])));
  }
  // Strip the trailing '/' if it exists.
  return trim($base_uri, '/');
} // cache_warmer_validate_url

/**
 * Check the arguments given to the drush command.
 *
 * @param $base_uri string
 *   The base URI.
 * @param $latest integer
 *   The number of the latest items to crawl.
 * @param $updated integer
 *   The elapsed time in seconds since the first item we want to crawl.
 * @param $hub_pages string
 *   The filename of the hub pages file.
 * @return string or boolean
 *   The base URI or FALSE if there's an error.
 */
function cache_warmer_check_arguments($base_uri = '', $latest = 0 , $updated = 0, $hub_pages) {
  // Validate the base URI given.
  $url_check = cache_warmer_validate_url($base_uri);
  if (!$url_check) {
    return $url_check;
  }

  if (!is_int($latest) || $latest < 0) {
    return drush_set_error('CACHE_WARMER_INVALID_LATEST',
                           dt('latest-n must be an integer greater than 0.'));
  }
  if (!is_int($updated ) || $updated < 0) {
    return drush_set_error(
      'CACHE_WARMER_INVALID_UPDATED',
      dt('updated-last must be the elapsed time in seconds since the first item we want to cache.')
    );
  }
  // Check if the hub pages file is readable.
  if (!empty($hub_pages) && (!is_readable($hub_pages) || filesize($hub_pages) == 0)) {
    return drush_set_error(
      'CACHE_WARMER_HUB_PAGES_FILE_FAIL',
      dt('Cannot open hub pages file @hub-file.',
         array('@hub-file' => $hub_pages)));
  }
  // Warn if both latest-n and are specified we'll user whichever returns the
  // greatest number of records.
  if ($latest > 0 && $updated > 0) {
    drush_log(
      dt('Both latest-n and updated last are specified. The one returning the most records will be used.'),
      'warning');
  }

  return $url_check;
} // cache_warmer_check_arguments

/**
 * Crawl the URIs of the site specified starting at the given base URI.
 *
 * @param $base_uri string
 *   The base URI of the site being crawled.
 *
 * @return string
 *   The request responses, status and timeouts in JSON format.
 */
function cache_warmer_execute($base_uri = '') {

  // Check if the cURL extension is available. Bail out it not.
  if (!extension_loaded('curl')) {
    return drush_set_error('CACHE_WARMER_CURL_MISSING',
                           dt('The cURL PHP extension is required. See: @curl_uri',
                              array('@curl_uri' => CACHE_WARMER_CURL_PHP_LINK)));
  }
  // The n latest updated items.
  $latest_arg = drush_get_option('latest-n');
  $latest = is_numeric($latest_arg) ? (int) $latest_arg : 0;
  // The updated last argument can be the number of seconds or a string
  // accepted by strtotime.
  $updated_arg = drush_get_option('updated-last');
  if (is_numeric($updated_arg)) {
    $updated = $_SERVER['REQUEST_TIME'] - (int) $updated_arg;
  }
  elseif (is_string($updated_arg)) {
    $updated = (int) date('U', strtotime($updated_arg));
  }
  else {
    $updated = 0;
  }
  // The hub pages file.
  $hub_pages_arg = drush_get_option('hub-pages-file');
  $hub_pages = is_string($hub_pages_arg) ? $hub_pages_arg : '';
  // The timeout.
  $timeout_arg = drush_get_option('timeout');
  $timeout = is_numeric($timeout_arg) ? (int) $timeout_arg : CACHE_WARMER_DEFAULT_TIMEOUT;
  // The number of parallel "threads" to run.
  $parallel_arg = drush_get_option('parallel');
  $parallel = is_numeric($parallel_arg) ? (int) $parallel_arg : 0;
  // The URI of the parallel crawler web service.
  if (!empty($parallel)) {
    $crawler_service_uri = drush_get_option('crawler-service-uri');
    // Validate the URI.
    cache_warmer_validate_url($crawler_service_uri);
  }
  // Whether or not the site has aliases.
  $no_aliases = drush_get_option('no-aliases') ? TRUE : FALSE;
  // Trace the execution for debugging purposes.
  $trace = drush_get_option('trace') ? TRUE : FALSE;

  // Get the credentials for logging in, if available.
  $username = drush_get_option('username');
  $password = drush_get_option('password');
  if (empty($username) && empty($password)) {
    $credentials = array();
  }
  else if (!empty($username) && !empty($password)) {
    $credentials = array('username' => $username, 'password' => $password);
  }
  else {
    return drush_set_error(
      CACHE_WARMER_MISSING_USERNAME_OR_PASSWORD_FAIL,
      dt('Either both username and password must be set, or neither.')
    );
  }

  // Check the arguments.
  $base_url = cache_warmer_check_arguments($base_uri, $latest, $updated, $hub_pages);
  if (!$base_url) {
    drush_set_error(CACHE_WARMER_NO_BASE_URI_FAIL,
                    dt('Cannot determine base URI to be crawled.'));
  }

  // Print the arguments (debug).
  if ($trace) {
    drush_print('Arguments');
    drush_print_r(array(
      'latest-n' => $latest,
      'updated-last' => $updated,
      'hub-pages-file' => $hub_pages,
      'base-uri' => $base_url,
      'timeout' => $timeout,
      'parallel' => $parallel,
      'crawler-service-uri' => $crawler_service_uri,
      'trace' => $trace,
      'username' => !empty($credentials) ? $credentials['username'] : '',
      'password' => !empty($credentials) ? $credentials['password'] : '',
    ));
  }

  // Getting the URIs to be hit. First we get the drupal major version.
  $items = array();
  $drupal_version = drush_drupal_major_version();

  switch ($drupal_version) {
    case '7':
      // Include the DB handling functions.
      if (!function_exists('cache_warmer_get_items_drupal7_both')) {
        require_once __DIR__ . '/includes/cache_warmer_db7.inc';
      }
      $items = cache_warmer_get_items_drupal7(_drush_sql_get_db_spec(), $latest, $updated, $no_aliases);
      break;
    case '6':
    case '5':
      // Include the DB handling functions.
      if (!function_exists('cache_warmer_get_items_drupal6_both')) {
        require_once __DIR__ . '/includes/cache_warmer_db6.inc';
      }
      $items = cache_warmer_get_items_drupal6(_drush_sql_get_db_spec(), $latest, $updated, $no_aliases);
      break;
    case FALSE:
      return drush_set_error('CACHE_WARMER_DRUPAL_VERSION_FAIL',
                             dt('Cannot determine the Drupal version.'));
    default:
      return drush_set_error('CACHE_WARMER_DRUPAL_VERSION_FAIL',
                             dt('Unsupported Drupal version.'));
  } // switch

  if ($trace) { // print the DB items: debug tracing
    drush_print('Items to be hit (DB)');
    drush_print_r($items);
  }

  // Crawling the given URIs.
  if ($parallel == 0) {
    // Include the functions for crawling the site.
    if (!function_exists('cache_warmer_crawl_single')) {
      require_once __DIR__ . '/includes/cache_warmer_crawl_single.inc';
    }
    // cURL invocation for single threaded mode.
    return json_encode(cache_warmer_crawl_single($base_url, $items, $hub_pages, $timeout, $credentials)) . "\n";
  }
  else {
    // Include the functions for crawling the site.
    if (!function_exists('cache_warmer_crawl_multiple')) {
      require_once __DIR__ . '/includes/cache_warmer_crawl_multiple.inc';
    }
    // cURL invocation for parallel mode. (POST to Lua location.)
    return json_encode(cache_warmer_crawl_multiple($base_url, $items, $hub_pages,
                                                   $timeout, $parallel, $crawler_service_uri)) . "\n";
  }
} // cache_warmer_execute

