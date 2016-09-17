<?php

namespace Drupal\sps\Exception;

class SPSException extends \Exception {
  /**
  * Recursive method to fine the original exception
  *
  * @param $ex
  *  Null to start with this exception or and Exception to start with
  * @return \Exception
  *   The Original Exception
  */
  public function getOriginal($ex = NULL) {
    $ex = $ex ?: $this;
    if(($previous = $this->getPrevious()) &&
       ($previous !== $ex)) {
      return $this->getOriginal($previous);
    }
    else {
      return $ex;
    }
  }

  /**
  * Check to see if the original exception was of a particular class
  *
  * @param $class
  *   name of a class
  *
  * @return bool
  */
  public function originalIs($class) {
    return is_a($this->getOriginal(), $class);
  }
}
