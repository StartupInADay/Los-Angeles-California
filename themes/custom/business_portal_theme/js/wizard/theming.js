(function($) {
  $( document ).ready(function(){
    $('body').addClass('wizard-page');
    var hostnames = ["labusinessportal.dev2-cityofla.acsitefactory.com", "business.lacity.org", "labp.cityofla.acsitefactory.com"];
    var paths = ["/sites/g/files/wph5571dev2/themes/site/", "/sites/g/files/wph521/themes/site/", "/sites/g/files/wph521/themes/site/"];
    var hostedpath = '';
    var logofilename = '';
    for (var ind = 0; ind < hostnames.length; ind++) {
      if (hostnames[ind] == window.location.hostname) {
        hostedpath = paths[ind];
      }
    }
    if (hostedpath != '') {
      logofilename = hostedpath + "logo-dark.png";
    } else {
      logofilename = "/profiles/labp/themes/la/lasbptheme/logo-dark.png";
    }
  $('.header__logo img').attr("src",logofilename);
  });
})(jQuery);
