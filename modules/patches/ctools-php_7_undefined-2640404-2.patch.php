diff --git a/page_manager/page_manager.module b/page_manager/page_manager.module
index f3cb743..a498f7f 100644
--- a/page_manager/page_manager.module
+++ b/page_manager/page_manager.module
@@ -1202,7 +1202,7 @@ function page_manager_page_manager_pages_to_hook_code($names = array(), $name =
     foreach ($objects as $object) {
       // Have to implement our own because this export func sig requires it
       $code .= $export['export callback']($object, TRUE, '  ');
       -      $code .= "  \${$export['identifier']}s['" . check_plain($object->$export['key']) . "'] = \${$export['identifier']};\n\n";
       +      $code .= "  \${$export['identifier']}s['" . check_plain($object->{$export['key']}) . "'] = \${$export['identifier']};\n\n";
     }
     $code .= "  return \${$export['identifier']}s;\n";
     $code .= "}\n";