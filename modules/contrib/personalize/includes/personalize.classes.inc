<?php
/**
 * @file
 * Provides interfaces and base classes for Personalize plugins.
 */

/**
 * Interface that all personalize agent type plugins must implement.
 */
interface PersonalizeAgentInterface {

  /**
   * Accessor for the machine_name property.
   *
   * @return string
   */
  public function getMachineName();

  /**
   * Accessor for the title property.
   *
   * @retun string
   */
  public function getTitle();

  /**
   * Accessor for the data property.
   *
   * @return array
   */
  public function getData();

  /**
   * Accessor for the status property.
   *
   * @return int
   */
  public function getStatus();

  /**
   * Accessor for the type property.
   *
   * @return string
   */
  public function getType();

  /**
   * Access for agent start property.
   */
  public function getStartTime();

  /**
   * Returns an array of assets that can be set as the #attached property
   * of an element in a render array.
   *
   * @return array
   *   An array of assets keyed by type, e.g. 'js', 'css', 'library'.
   */
  public function getAssets();

  /**
   * Factory method for getting an instance of the agent type.
   *
   * @param $agent_data
   *   The stored information about the agent.
   * @return PersonalizeAgentInterface
   *   An object implementing PersonalizeAgentInterface.
   */
  public static function create($agent_data);

  /**
   * A method to call once an agent has been saved.
   *
   * @param $old_data
   *   An object representing the agent data before the save.
   *
   * This is useful for agent plugins that need to sync data with another
   * service.
   */
  public function postSave($old_data);

  /**
   * Converts a key/value pair to a feature string that this agent can consume.
   *
   * @param $name
   *   The key name, could be something like "gender".
   * @param $value
   *   The value, e.g. "male", "female" or "other".
   * @param $is_mutex
   *   Whether this context has mutually exclusive values.
   * @return string
   *   A string that this agent can consume for explicit targeting.
   */
  public static function convertContextToFeatureString($name, $value, $is_mutex = FALSE);

  /**
   * Checks whether the agent is ready to run.
   *
   * @return array
   *   A non-empty array indicates that the agent did not pass the check. Each
   *   element in the array should be a string providing information about why
   *   the agent failed the check.
   */
  public function errors();

  /**
   * Whether this agent should be paused now.
   *
   * An agent class can implement some logic to determine whether or not the agent
   * in question should be set to the "paused" status. For example, an agent type
   * might have some notion of having a "winner", and once it has found this winner
   * the agent should pause on that.
   *
   * @return boolean
   *   TRUE if the agent should be paused, FALSE otherwise.
   */
  public function stopNow();

  /**
   * Whether the agent supports multivariate tests.
   *
   * @return boolean
   *   TRUE if the agent supports MVTs, FALSE otherwise.
   */
  public function supportsMVTs();

  /**
   * Whether the agent supports multiple decision points.
   *
   * @return boolean
   *   TRUE if the agent supports multiple decision points, FALSE otherwise.
   */
  public function supportsMultipleDecisionPoints();

}

interface PersonalizeAgentGoalInterface {

  /**
   * Whether to use the client-side goal delivery mechanism.
   *
   * @return boolean
   */
  public function useClientSideGoalDelivery();

  /**
   * Sends a goal to an agent.
   *
   * Only for server-side goal sending.
   *
   * @param string $goal_name
   *   The name of the goal being sent.
   * @param int $value
   *   (optional) The value of the goal being sent.
   */
  public function sendGoal($goal_name, $value = NULL);
}

/**
 * Exceptions specific to personalize functionality.
 */
class PersonalizeException extends Exception {};

/**
 * An interface to implement when an agent supports explicit targeting for
 * content variations.
 */
interface PersonalizeExplicitTargetingInterface extends PersonalizeAgentInterface {

  const EXPLICIT_TARGETING_MULTIPLE_OR = 1;
  const EXPLICIT_TARGETING_MULTIPLE_AND = 2;
  const EXPLICIT_TARGETING_MULTIPLE_BOTH = 3;

  /**
   * Returns how this agent supports explicit targeting
   * with multiple features.
   *
   * @return int
   *   Returns one of the above constants.
   */
  public static function explicitTargetingSupportMultiple();

}


/**
 * Abstract base class for agents, providing default implementations of some methods.
 */
abstract class PersonalizeAgentBase implements PersonalizeAgentInterface, PersonalizeLoggerAwareInterface {

  /**
   * The machine_name of the agent.
   *
   * @var string
   */
  protected $machineName;

  /**
   * The title of the agent.
   *
   * @var string
   */
  protected $title;

  /**
   * An array of data describing the agent.
   *
   * @var array
   */
  protected $data;


  /**
   * An integer representing the status of the agent.
   *
   * @var int
   */
  protected $status;

  /**
   * An integer representing the Unix timestamp when the agent was started.
   *
   * @var int
   */
  protected $startTime;

  /**
   *
   * @var PersonalizeLoggerInterface
   */
  protected $logger;


  /**
   * Constructs and agent.
   *
   * @param string $machine_name
   *   The machine name of the agent.
   * @param string $title
   *   The title of the agent.
   * @param array $data
   *   An array of data describing the agent.
   * @param int $status
   *   The current status of the agent (running, paused, etc.).
   * @param int $startTime
   *   Unix timestamp when the agent was started.
   */
  public function __construct($machine_name, $title, $data, $status, $startTime = NULL) {
    $this->machineName = $machine_name;
    $this->title = $title;
    $this->data = $data;
    $this->status = $status;
    $this->startTime = empty($startTime) ? time() : $startTime;
  }

  /**
   * Implements PersonalizeAgentInterface::getMachineName().
   */
  public function getMachineName() {
    return $this->machineName;
  }

  /**
   * Implements PersonalizeAgentInterface::getTitle().
   */
  public function getTitle() {
    return $this->title;
  }

  /**
   * Implements PersonalizeAgentInterface::getData().
   */
  public function getData() {
    return $this->data;
  }


  /**
   * Implements PersonalizeAgentInterface::getStatus().
   */
  public function getStatus() {
    return $this->status;
  }

  /**
   * Implements PersonalizeAgentInterface:getStartTime().
   */
  public function getStartTime() {
    return $this->startTime;
  }

  /**
   * Implements PersonalizeAgentInterface::getAssets().
   */
  public function getAssets() {
    return array();
  }

  /**
   * Implements PersonalizeAgentInterface::postSave().
   */
  public function postSave($old_data) {

  }

  /**
   * Implements PersonalizeAgentInterface::convertContextToFeatureString().
   */
  public static function convertContextToFeatureString($name, $value, $is_mutex = FALSE) {
    $value = self::cleanFeatureValue($value);
    return $name . '--' . $value;
  }

  /**
   * Implements PersonalizeAgentInterface::errors().
   */
  public function errors() {
    return array();
  }

  /**
   * Implements PersonalizeAgentInterface::stopNow().
   */
  public function stopNow() {
    return FALSE;
  }

  /**
   * Implements PersonalizeAgentInterface::supportsMVTs().
   */
  public function supportsMVTs() {
    if (!empty($this->data['auto_created'])) {
      return FALSE;
    }
    return TRUE;
  }

  /**
   * Implements PersonalizeAgentInterface::supportsMultipleDecisionPoints().
   */
  public function supportsMultipleDecisionPoints() {
    if (!empty($this->data['auto_created'])) {
      return FALSE;
    }
    return TRUE;
  }

  /**
   * Returns the logger to use.
   *
   * @return PersonalizeLoggerInterface
   */
  protected function logger() {
    if ($this->logger !== NULL) {
      return $this->logger;
    }
    return new PersonalizeLogger();
  }

  /**
   * Implements PersonalizeLoggerAwareInterface::setLogger().
   */
  public function setLogger(PersonalizeLoggerInterface $logger)
  {
    $this->logger = $logger;
  }

  protected static function cleanFeatureValue($string) {
    $string = preg_replace('/[^A-Za-z0-9_-]/', '-', $string);
    // Get rid of instances of multiple '-' characters after replacement.
    return preg_replace('/\-{2,}/', '-', $string);
  }

}

interface PersonalizeContextInterface {

  /**
   * Factory method to construct a visitor context object.
   *
   * @param PersonalizeAgentInterface $agent
   *   The agent this context is for.
   * @param $selected_context
   *   The context options that have been selected for use.
   * @return PersonalizeContextInterface
   *   An object implementing PersonalizeContextInterface.
   */
  public static function create(PersonalizeAgentInterface $agent = NULL, $selected_context = array());

  /**
   * Returns the list of options that can be selected from.
   *
   * @return array
   *   An associative array keyed by option name and with an associative
   *   array as the value. The value array can have the following keys:
   *   - name A human-readable name for the option
   *   - (optional) group An optional group name that the option should be
   *     categorized under.
   *   - (optional) cache_type: indicates the type of web storage caching (if
   *     available); one of "session", "local", "none" (default).
   *   - (optional) cache_expiration:  the number of minutes that this option
   *     should remain in the cache or none to keep indefinitely.  Only valid
   *     when cache_type = "local".
   */
  public static function getOptions();

  /**
   * Indicates if the current context is valid for a particular
   * personalize agent.
   *
   * @param $agent
   *   The PersonalizeAgent plugin data.
   *
   * @return
   *   True if this context is available, false otherwise.
   */
  public static function allowedFromAgent(StdClass $agent);

  /**
   * Returns the possible values that this context plugin could send as
   * feature strings.
   * @param $limit
   *   True if values should be limited by the selected context; false to
   *   return all possible values regardless of selected context.
   * @return array
   *   An associative array of values with feature strings as keys and
   *   human-readable names as values.
   */
  public function getPossibleValues($limit = FALSE);

  /**
   * Returns the assets required for this context object to work.
   *
   * @return array
   *   An array suitable to be added as the #attached property of a render
   *   array.
   */
  public function getAssets();
}

abstract class PersonalizeContextBase implements PersonalizeContextInterface, PersonalizeLoggerAwareInterface {

  /**
   * Holds the list of context options that have been selected for use.
   *
   * @var array
   */
  protected $selectedContext;

  /**
   *
   * @var PersonalizeLoggerInterface
   */
  protected $logger;

  /**
   * Constructs a PersonalizeContextBase object.
   */
  public function __construct(PersonalizeAgentInterface $agent = NULL, $selected_context = array()) {
    $this->agent = $agent;
    $this->selectedContext = $selected_context;
  }

  /**
   * Implements PersonalizeContextInterface::allowedFromAgent().
   */
  public static function allowedFromAgent(StdClass $agent) {
    return TRUE;
  }

  /**
   * Implements PersonalizeContextInterface::getAssets().
   */
  public function getAssets() {
    return array();
  }

  /**
   * Returns the logger to use.
   *
   * @return PersonalizeLoggerInterface
   */
  protected function logger() {
    if ($this->logger !== NULL) {
      return $this->logger;
    }
    return new PersonalizeLogger();
  }

  /**
   * Implements PersonalizeLoggerAwareInterface::setLogger().
   */
  public function setLogger(PersonalizeLoggerInterface $logger)
  {
    $this->logger = $logger;
  }

  /**
   * Implements PersonalizeContextInterface::getPossibleValues().
   */
  public function getPossibleValues($limit = FALSE) {
    return array();
  }
}

class PersonalizeInvalidContextSpecifierException extends Exception {

}

/**
 * Interface PersonalizeLoggerInterface
 *
 * Provided to make it easier to switch to PSR-3 logger. See PersonalizeLogger
 * class.
 */
interface PersonalizeLoggerInterface {
  public function log($level, $message, array $context = array());
}

/**
 * Class PersonalizeLogLevel
 *
 * Provided to make it easier to switch to PSR-3 logger. See PersonalizeLogger
 * class.
 */
class PersonalizeLogLevel
{
  const EMERGENCY = 'emergency';
  const ALERT     = 'alert';
  const CRITICAL  = 'critical';
  const ERROR     = 'error';
  const WARNING   = 'warning';
  const NOTICE    = 'notice';
  const INFO      = 'info';
  const DEBUG     = 'debug';
}

/**
 * Class PersonalizeLogger
 *
 * Provided to make it easier to switch to PSR-3 logger in D8 and also to
 * make it easier to write unit-testable classes. Tests can switch out this
 * watchdog-wrapper and provide a fake logger.
 */
class PersonalizeLogger implements PersonalizeLoggerInterface {
  static $level_map = array(
    PersonalizeLogLevel::EMERGENCY => WATCHDOG_EMERGENCY,
    PersonalizeLogLevel::ALERT => WATCHDOG_ALERT,
    PersonalizeLogLevel::CRITICAL => WATCHDOG_CRITICAL,
    PersonalizeLogLevel::ERROR => WATCHDOG_ERROR,
    PersonalizeLogLevel::WARNING => WATCHDOG_WARNING,
    PersonalizeLogLevel::NOTICE => WATCHDOG_NOTICE,
    PersonalizeLogLevel::INFO => WATCHDOG_INFO,
    PersonalizeLogLevel::DEBUG => WATCHDOG_DEBUG,
  );

  /**
   * Logs with an arbitrary level.
   *
   * @param mixed $level
   * @param string $message
   * @param array $context
   * @return null
   */
  public function log($level, $message, array $context = array()) {
    if (variable_get('personalize_watchdog_min_severity', WATCHDOG_WARNING) >= $level) {
      $variables = array();
      foreach ($context as $key => $value) {
        if (strpos($message, '{' . $key . '}') !== FALSE) {
          $variables['@' . $key] = $value;
          $message = str_replace('{' . $key . '}', '@' . $key, $message);
        }
      }

      watchdog(
        'personalize',
        $message,
        $variables + $context,
        self::$level_map[$level]
      );
    }
  }
}

/**
 * Interface PersonalizeLoggerAwareInterface
 *
 * Provided to make it easier to switch to PSR-3 logger. See PersonalizeLogger
 * class.
 */
interface PersonalizeLoggerAwareInterface
{
  /**
   * Sets a logger instance on the object
   *
   * @param LoggerInterface $logger
   * @return null
   */
  public function setLogger(PersonalizeLoggerInterface $logger);
}
