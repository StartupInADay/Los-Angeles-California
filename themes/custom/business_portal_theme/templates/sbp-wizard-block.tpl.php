<?php
//dpm(get_defined_vars());
?>
<div id="wizard" class="wizard"></div>

<script type="text/template" id="start-template">
  <div class="wizard__header--start">
    <div class="wizard__header--title-start-super">Welcome to the</div>
    <div class="wizard__header--title-start">Startup Guide</div>
    <div class="wizard__header-line"></div>
  </div>
  <div class="wizard__copy--section_start">{{Description}}</div>
  <div class="wizard__copy--time-estimate">This guide is estimated to take 10 to 15 minutes.</div>
</script>

<script type="text/template" id="header-template">
  <div class="wizard__header--title-government">{{jurisdiction}}</div>
  <div class="wizard__header--title-intro">{{Name}}</div>
  <div class="wizard__header-line"></div>
</script>

<script type="text/template" id="header-for-question-template">
  <div class="wizard__header--title">{{Name}}</div>
  <div class="wizard__header-line"></div>
</script>

<script type="text/template" id="header-for-contextual-template">
  <div class="icon-info"></div>
  <div class="constrained">
    <div class="wizard__question--help">{{title}}</div>
  </div>
  <hr class="wizard__help_hr"/>
</script>


<script type="text/template" id="description-for-contextual-template">
  <div class="wizard__help_content">{{Description}}</div>
</script>

<script type="text/template" id="header-for-confirm-template">
  <div class="wizard__header--title-government">{{jurisdiction}}</div>
  <div class="wizard__header--title">{{Name}}</div>
  <div class="wizard__header-line"></div>
</script>

<script type="text/template" id="intro-template">
  <div class="wizard__copy--section_intro">{{Description}}</div>
</script>

<script type="text/template" id="intro-with-illustration-template">
  <div class="wizard__illustration"><img src="{{illustration}}"></div>
  <div class="wizard__copy--section_intro">{{Description}}</div>
</script>

<script type="text/template" id="question-template">
  <div class="wizard__question">{{title}}</div>
</script>

<script type="text/template" id="buttons-template">
  <div class="wizard__buttons"></div>
</script>

<script type="text/template" id="wizard-results-cta-template">
  <p><a class="wizard__button print">PRINT</a><a class="wizard__button email">EMAIL</a></p>
</script>

<script type="text/template" id="wizard-results-email-template">
  <div id="wizard-email" class="element-invisible">
    <?php print $email_form; ?>
  </div>
  <div id="message-response"></div>
</script>

<script type="text/template" id="wizard-nav-start-template">
  <hr class="wizard__arrow-line"/>
  <a href="#" class="wizard__arrow-down start enabled">Next</a>
  <hr class="wizard__arrow-line"/>
</script>

<script type="text/template" id="wizard-nav-section-template">
  <a href="#" class="wizard__arrow-up">Previous</a>
  <hr class="wizard__arrow-line"/>
  <a href="#" class="wizard__arrow-down enabled section">Next</a>
  <hr class="wizard__arrow-line"/>
</script>


<script type="text/template" id="wizard-nav-question-template">
  <a href="#" class="wizard__arrow-up">Previous</a>
  <hr class="wizard__arrow-line"/>
  <a href="#" class="wizard__arrow-down">Next</a>
</script>

<script type="text/template" id="wizard-nav-contextual-help-template">
  <hr class="wizard__arrow-line"/>
  <a href="#" class="wizard__arrow-back left enabled">Back to the question</a>
  <div style="clear:both;"></div>
  <hr class="wizard__arrow-line"/>
</script>

<script type="text/template" id="wizard-address-form-template">
  <?php print $address_form; ?>
  <div class="address-result"></div>
</script>

<script type="text/template" id="wizard-nav-address-template">
  <a href="#" class="wizard__tip_button wizard__address_back_button left enabled">BACK TO ANSWERS</a>
  <a href="#" class="wizard__arrow-up">Previous</a>
  <hr class="wizard__arrow-line"/>
  <a href="#" class="wizard__arrow-down">Next</a>
</script>

<script type="text/template" id="wizard-nav-start-over-template">
  <hr class="wizard__arrow-line"/>
  <a href="#" class="wizard__arrow-up enabled left start-over">Start over</a>
  <hr class="wizard__arrow-line"/>
</script>

<script type="text/template" id="tip-template">
  <div class="wizard__tip_icon"></div>
  <div class="wizard__tip_content">
    <div class="wizard__tip_copy">{{tip}}</div>
  </div>
</script>

<script type="text/template" id="results-template">
  <div class="wizard__step"> step {{index}}</div>
  <div class="wizard__result">{{result}}</div>
  <div class="wizard__result_buttons">{{buttons}}</div>
</script>


<script type="text/template" id="progress-bar-section-template">
  <div class="wizard__progress-bar-section"></div>
</script>

<script type="text/template" id="progress-bar-section-with-icon-template">
  <div class="wizard__progress-bar-section">
    <img src="{{icon}}" />
  </div>
</script>

<script type="text/template" id="progress">
  <div class="wizard__nav"></div>
</script>