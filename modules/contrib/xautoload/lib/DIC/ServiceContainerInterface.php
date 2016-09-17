<?php
namespace Drupal\xautoload\DIC;

use Drupal\xautoload\Adapter\ClassFinderAdapter;
use Drupal\xautoload\Adapter\DrupalExtensionAdapter;
use Drupal\xautoload\ClassFinder\ExtendedClassFinderInterface;
use Drupal\xautoload\CacheManager\CacheManager;
use Drupal\xautoload\ClassFinder\ProxyClassFinder;
use Drupal\xautoload\Discovery\ClassMapGenerator;
use Drupal\xautoload\Discovery\ClassMapGeneratorInterface;
use Drupal\xautoload\DrupalSystem\DrupalSystemInterface;
use Drupal\xautoload\Main;

/**
 * @property Main $main
 * @property ClassFinderAdapter $adapter
 * @property ClassMapGeneratorInterface $classMapGenerator
 * @property ClassMapGenerator $classMapGeneratorRaw
 * @property CacheManager $cacheManager
 * @property ProxyClassFinder $proxyFinder
 * @property ExtendedClassFinderInterface $classFinder
 * @property ExtendedClassFinderInterface $finder
 *   Alias for ->classFinder
 * @property DrupalSystemInterface $system
 * @property DrupalExtensionAdapter $extensionRegistrationService
 */
interface ServiceContainerInterface {

  /**
   * Retrieves a lazy-instantiated service.
   *
   * @param string $key
   *   A key to specify a service.
   * @return mixed
   *   The service for the given key. Usually an object.
   */
  function __get($key);
}
