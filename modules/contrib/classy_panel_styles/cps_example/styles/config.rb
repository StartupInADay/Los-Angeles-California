##
## This file is only needed for Compass/Sass integration. If you are not using
## Compass, you may safely ignore or delete this file.
##
## If you'd like to learn more about Sass and Compass, see the sass/README.txt
## file for more information.
##

# Change this to :production when ready to deploy the CSS to the live server.
## environment = :production

output_style = :expanded

# Location of the theme's resources.
css_dir = "css"
sass_dir = "sass"
javascripts_dir = "js"

# Require any additional compass plugins installed on your system.
require 'compass'
require 'toolkit'
require 'brevis'
require 'singularitygs'


# Compass will automatically add cache busters to your images based on image timestamps.
# This will keep browser caches from displaying the wrong image if you change the image but not the url.
# If you don’t want this behavior, it's easy to configure or disable:
# UNCOMMENT THE NEXT THREE LINES
asset_cache_buster do |http_path, real_path|
  nil
end


line_comments = false
color_output = false
##
## You probably don't need to edit anything below this.
##

# You can select your preferred output style here (:expanded, :nested, :compact
# or :compressed).
# output_style = (environment == :development) ? :expanded : :expanded

# To enable relative paths to assets via compass helper functions. Since Drupal
# themes can be installed in multiple locations, we don't need to worry about
# the absolute path to the theme from the server omega.
#relative_assets = true
#
## Conditionall enable line comments when in development mode.
#line_comments = (environment == :development) ? true : false
#
## Output debugging info in development mode.
#sass_options = (environment == :development) ? {:debug_info => true} : {}
