# Cache warmer for Drupal

## Introduction

`cache_warmer` is a [drush](http://drush.ws) command that hits a set
of URIs of a drupal site based on the **freshness** of the
content.

The main purpose is to offer a **complete** setup for running a mostly
cached drupal site without having to deal with expiration logic and
instead use
[microcaching](http://fennb.com/microcaching-speed-your-app-up-250x-with-no-n).

Dealing with the expiration logic is messy and inefficient it requires
additional work, be it from the drupal side of things, be it from the
external cache side.
 
Microcaching can be used with **any** type of site. Be it your
portfolio or personal blog or a high traffic news site. You have to
tweak the cache validity (or Time To Live: TTL) for your site traffic
profile.

Although microcaching is particularly useful for the Nginx filesystem
based cache it can be used with other caching systems like Varnish.

It can be used also for **priming** any type of external cache.

Note that as is usual with drush although its tagged `7.x` it works
with both **drupal 6** and **drupal 7**.

## How it works

The drush command is quite light, it performs a drush bootstrap up to
`DRUSH_BOOTSTRAP_DRUPAL_DATABASE`, which is equivalent to Drupal's
`BOOTSTRAP_DRUPAL_DATABASE`, and queries the database for content that
is fresh according to the given criteria.

As example consider that we want to hit the lastest 300 nodes in the
`http://example.com` site plus the URIs listed in *hub pages* file
`hub_pages.txt` site in **single threaded** mode.

    drush cache-warmer --latest-n=300 --hub-pages-file=hub_pages.txt http://example.com
    
If instead we wanted to hit the URIs for all the nodes updated in the last 2 days we do:

    drush cache-warmer --updated-last='-2 days' --hub-pages-file=hub_pages.txt http://example.com 
    
The `--updated-last` option accepts its argument as an integer
representing the **number of seconds** ago the content was updated,
e.g., `--updated-last=3600` means the content updated in the last
hour. It accepts also strings in
[`strotime`](http://php.net/manual/en/function.strtotime.php)
format. Hence the above `-2 days` (latest 48 hours).

The only command argument is the base URI of the site to be hit.

If you specify **both** a number of nodes through the `--latest-n`
option and a time range through the `--updated-last` option. The one
returning the most number of items will be used. Note that this
involves doing **sub-queries** hence is less efficient in SQL terms.

## Single threaded and Parallel operating modes

You can run `cache_warmer` in **single-threaded** (the default mode)
or **parallel**. Parallel means that the requests are made in
**parallel**, i.e., simultaneously.

The single threaded mode uses PHP
[cURL](http://php.net/manual/en/book.curl.php) extension only.

Note that in order to use 
[drupal_http_request](http://api.drupal.org/api/drupal/includes--common.inc/function/drupal_http_request/7)
requires a higher level of boostrap than `BOOTSTRAP_DRUPAL_DATABASE`
hence it would make `cache_warmer` less performant. Hence the option
for cURL which is pretty much standard everywhere when we're
considering a dependable standards observing HTTP client library.

In **single-threaded** mode the request are made **sequentially**,
i.e, the URIs are hit one after the other. Hence in each request cURL
**blocks** up until it either **times out** or it gets a response. You
can specify the timeout that cURL uses through the `--timeout`
option. To specify a timeout of 15 seconds specify
`--timeout=15`. Re-using the above example:

    drush cache-warmer --updated-last='-2 days' --timeout=15 --hub-pages-file=hub_pages.txt http://example.com 

In parallel mode the requests are made in parallel in a specified size
batch. To perform the above with **20** requests in parallel batches do:

    drush cache-warmer --updated-last='-2 days' --parallel=20 --crawler-service-uri=http://crawl.example.com/cache-warmer --timeout=15 --hub-pages-file=hub_pages.txt http://example.com 

As you can see there's a **new** option `--crawler-service-uri` that
specifies the URI if the **crawler** to be used for hitting the list
of URIs in batches of 20, i.e., making 20 parallel requests.

The parallel crawler is implemented as a web service allowing for the
crawler to be pluggable. There's a *default* crawler provided relying
on Nginx Embedded Lua module
[`ngx.location.capture_multi`](http://wiki.nginx.org/HttpLuaModule#ngx.location.capture_multi)
directive, that allows to make an arbitrary number of requests in a non-blocking
way using Nginx event driven architecture coupled with Lua speed.

The focus is on **speed** and **simplicity**. The limit of the
parallelism will be at the OS and network level and not in any way
inherent to this command. You can make **1000** requests in parallel
if you wish. The limiting factors will be the either at the network
level (the network gets saturated) or your drupal site cannot cope
with such a number of simultaneous requests.

## Requirements

### Single threaded mode

To use the single threaded mode just the
[cURL](http://php.net/manual/en/book.curl.php) PHP extension and PHP 5
are required.

### Parallel mode

This requires to either code your own parallel crawler or to have
[Nginx](http://nginx.org) with the
[Embedded Lua module](http://wiki.nginx.org/HttpLuaModule).

In Debian, for example, this module is available in the
[nginx-extras](http://packages.debian.org/sid/nginx-extras). I provide
a *bleeding edge* Nginx [debian](http://debian.perusio.net) package
that includes this module and a few less common ones while not
including mail related modules. Other option is for you to build your
own nginx package or binary.

It requires the
[Lua Socket](http://w3.impa.br/~diego/software/luasocket/home.html)
library.

## Hub pages

Hub pages are pages in your site that function as a hub for accessing
content. For example if you have a page for each taxonomy term you may
specify some taxonomy term pages in the hub pages file. Here's a
simple **hub pages** file. It's a text file with a URI relative to the
base URI of the site to be hit by the crawler on each line. The sole
exception to that rule is the front page which is denoted by `<front>`

    <front>
    foobar/term1
    foobar/term2
    featured/users

Besides the front page, `<front>`, we're also hitting the hub pages
with URIs `/foobar/term1`, `/foobar/term2` and `/featured/users`. Note
that each URI is **always** relative to the base URI of the site and
is specified **always without** leading slash.

## Aliases and Pathauto

By default the crawler assumes that you're using clean URIs with
aliases. Usually you run something like
[pathauto](http://drupal.org/project/pathauto) on your drupal site. If
your site doesn't use aliases you can disable the crawler default
behavior with the option `--no-aliases`.

## Installation and Usage: Single threaded mode 

 1. Download the command and install it in a drush aware location. The
    easiest option is to put it in your `~/.drush` directory. 
 
 2. Create your hub pages file.
 
 3. Specify the necessary options and you're done, if just testing or
    priming a cache.
 
 4. For using microcaching in a consistent manner. Create a cronjob
    with a schedule adapted to your site traffic profile and cache
    validity. Consider the above example. The cache TTL is 5 minutes
    and we want to keep the last 24 hours worth of content in cache
    and also all the hub pages. We specify a cronjob that runs every 
    8 minutes.
    
        */8 * * * * drush cache-warmer --updated-last='-24 hours' --timeout=15 --hub-pages-file=hub_pages.txt http://example.com 
    
 5. Processing the replies. The command returns a structure of the
    response status, timestamp, time spent in request for each URI
    hit in JSON encoded format. You can pipe this output to a file for
    further processing and analysis of the way the cache warmer is
    interacting with the site. With the above example.
    
        */8 * * * * drush cache-warmer --updated-last='-24 hours' --timeout=15 --hub-pages-file=hub_pages.txt http://example.com >> /path/to/cache_warmer.log
    
## Installation and Usage: Parallel mode

 1. Download the command and install it in a drush aware location. The
    easiest option is to put it in your `~/.drush` directory. 
 
 2. Create your hub pages file. 
    
 3. Create your custom parallel crawler web service or install Nginx
    and setup a host for functioning as the parallel crawler web
    service. There's a suggested config in the `config`
    subdirectory. Adapt it to your liking.
    
 4. Specify the necessary options and you're done, if just testing or
    priming a cache.
 
 5. For using microcaching in a consistent manner. Create a cronjob
    with a schedule adapted to your site traffic profile and cache
    validity. Consider the above example. The cache TTL is 5 minutes
    and we want to keep the last 24 hours worth of content in cache
    and also all the hub pages. We specify a cronjob that runs every 
    8 minutes.
    
        */8 * * * * drush cache-warmer --updated-last='-24 hours' --timeout=15 --parallel=20 --crawler-service-uri=http://crawler.example.com/cache-warmer --hub-pages-file=hub_pages.txt http://example.com
    
 6. Processing the replies. The command returns a structure of the
    response status, timestamp, time spent in request for each URI
    hit in JSON encoded format. You can pipe this output to a file for
    further processing and analysis of the way the cache warmer is
    interacting with the site. With the above example.
   
        */8 * * * * drush cache-warmer --updated-last='-24 hours' --timeout=15 --hub-pages-file=hub_pages.txt http://example.com >> /path/to/cache_warmer.log
   
## 100-continue responses

The parallel mode works by POSTing the URIs to be hit in a form. cURL
obeys the standards and sends a `Expect: 100-continue` header to the
server when the POST data has a size above 1024 bytes. The server
should reply with a `100` status code thus instructing cURL to go
ahead and send the POST data.

Therefore you could see a bunch of 100 status in the JSON array
containing the responses of the crawler. That's how it should be. If
you want to **force** cURL to POST immediately and thus avoid this
additional round trip to the server then send an **empty** `Expect`
header.

Here's a [gist](https://gist.github.com/1724301) with the option you
need to add to the command. I might consider making it an option to
the command in the future.  

## Scheduling the cache warmer with precision

[Vixie's](https://en.wikipedia.org/wiki/Vixie_cron#Modern_versions)
cron is the *default* cron on most UNIX flavours. Unfortunately not
happy with only having an abstruse syntax it's also very
imprecise. The smallest time unit is one minute and there's no
certainty that a job will be triggered precisely in the exact second
of the minute, i.e., if now the job is triggered at the 10 second mark
of the current minute, there's no garantuee that the next minute
trigger will happen also at the 10 second mark.

For greater precision and an **expressive** language for scheduling
jobs there's the Scheme (Guile) based scheduler
[mcron](http://www.gnu.org/software/mcron/), that has **second**
precision.

You can even use the abstruse Vixie's cron syntax with it if you
prefer.

## Microcaching configuration for Drupal

Configuring Nginx for microcaching is a little beyond the scope of
this documentation. I maintain a generic
[Nginx configuration](https://github.com/perusio/drupal-with-nginx)
that includes microcaching for both anonymous and authenticated users.

## TODO

 1. Add support for using Nginx embedded Lua module
    [`cosocket`](http://wiki.nginx.org/HttpLuaModule#ngx.socket.tcp)
    and thus create **non-blocking** sockets from within Nginx and
    thus avoid the Lua Socket library dependence.

 2. Integrate with [httprl](http://drupal.org/project/httprl) to provide a
    purely drupal (PHP) based parallel crawler.

 3. Add `mcron` configuration example.
 
 3. Add a script for doing graphical analysis of the crawler responses.   

 4. Benchmark this approach against more *usual* approaches to caching
    like using [Boost](http://drupal.org/project/boost) for example.
