-- SUMMARY --

Picture element

Hide image
----------

If you use the '- empty image -' option, you have to add the following
to your theme css to completely hide the image, otherwise it will
still take some space.

img[width="1"][height="1"] {
  display: none;
}
