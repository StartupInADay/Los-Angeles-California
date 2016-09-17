<?php

/**
 * @file
 * DOM processing functions.
 */

/**
 * Parses an HTML snippet and returns it as a DOM object.
 *
 * This function loads the body part of a partial (X)HTML document and returns
 * a full \DOMDocument object that represents this document.
 *
 * @param string $html
 *   The partial (X)HTML snippet to load. Invalid markup will be corrected on
 *   import.
 *
 * @return \DOMDocument
 *   A \DOMDocument that represents the loaded (X)HTML snippet.
 */
function entity_embed_dom_load_html($html) {
  $document = <<<EOD
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Strict//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head><meta http-equiv="Content-Type" content="text/html; charset=utf-8" /></head>
<body>!html</body>
</html>
EOD;
  // PHP's \DOMDocument serialization adds straw whitespace in case the markup
  // of the wrapping document contains newlines, so ensure to remove all
  // newlines before injecting the actual HTML body to process.
  $document = strtr($document, array("\n" => '', '!html' => $html));

  $dom = new \DOMDocument();
  // Ignore warnings during HTML soup loading.
  @$dom->loadHTML($document);

  return $dom;
}

/**
 * Adds comments around a <!CDATA section in a \DOMNode.
 *
 * \DOMDocument::loadHTML() in \Drupal\Component\Utility\Html::load() makes
 * CDATA sections from the contents of inline script and style tags. This can
 * cause HTML4 browsers to throw exceptions.
 *
 * This function attempts to solve the problem by creating a
 * \DOMDocumentFragment to comment the CDATA tag.
 *
 * @param \DOMNode $node
 *   The element potentially containing a CDATA node.
 * @param string $comment_start
 *   (optional) A string to use as a comment start marker to escape the CDATA
 *   declaration. Defaults to '//'.
 * @param string $comment_end
 *   (optional) A string to use as a comment end marker to escape the CDATA
 *   declaration. Defaults to an empty string.
 */
function entity_embed_escape_cdata_element(\DOMNode $node, $comment_start = '//', $comment_end = '') {
  foreach ($node->childNodes as $child_node) {
    if ($child_node instanceof \DOMCdataSection) {
      $embed_prefix = "\n<!--{$comment_start}--><![CDATA[{$comment_start} ><!--{$comment_end}\n";
      $embed_suffix = "\n{$comment_start}--><!]]>{$comment_end}\n";

      // Prevent invalid cdata escaping as this would throw a DOM error.
      // This is the same behavior as found in libxml2.
      // Related W3C standard: http://www.w3.org/TR/REC-xml/#dt-cdsection
      // Fix explanation: http://en.wikipedia.org/wiki/CDATA#Nesting
      $data = str_replace(']]>', ']]]]><![CDATA[>', $child_node->data);

      $fragment = $node->ownerDocument->createDocumentFragment();
      $fragment->appendXML($embed_prefix . $data . $embed_suffix);
      $node->appendChild($fragment);
      $node->removeChild($child_node);
    }
  }
}

/**
 * Converts the body of a \DOMDocument back to an HTML snippet.
 *
 * The function serializes the body part of a \DOMDocument back to an (X)HTML
 * snippet. The resulting (X)HTML snippet will be properly formatted to be
 * compatible with HTML user agents.
 *
 * @param \DOMDocument $document
 *   A \DOMDocument object to serialize, only the tags below the first <body>
 *   node will be converted.
 *
 * @return string
 *   A valid (X)HTML snippet, as a string.
 */
function entity_embed_serialize(\DOMDocument $document) {
  $body_node = $document->getElementsByTagName('body')->item(0);
  $html = '';

  foreach ($body_node->getElementsByTagName('script') as $node) {
    entity_embed_escape_cdata_element($node);
  }
  foreach ($body_node->getElementsByTagName('style') as $node) {
    entity_embed_escape_cdata_element($node, '/*', '*/');
  }
  foreach ($body_node->childNodes as $node) {
    $html .= $document->saveXML($node);
  }
  return $html;
}

/**
 * Rename a DOMNode tag.
 *
 * @param \DOMNode $node
 *   A DOMElement object.
 * @param string $name
 *   The new tag name.
 */
function entity_embed_change_dom_node_name(\DOMNode &$node, $name = 'div') {
  if ($node->nodeName != $name) {
    /** @var \DOMElement $replacement_node */
    $replacement_node = $node->ownerDocument->createElement($name);

    // Copy all children of the original node to the new node.
    if ($node->childNodes->length) {
      foreach ($node->childNodes as $child) {
        $child = $replacement_node->ownerDocument->importNode($child, TRUE);
        $replacement_node->appendChild($child);
      }
    }

    // Copy all attributes of the original node to the new node.
    if ($node->attributes->length) {
      foreach ($node->attributes as $attribute) {
        $replacement_node->setAttribute($attribute->nodeName, $attribute->nodeValue);
      }
    }

    $node->parentNode->replaceChild($replacement_node, $node);
    $node = $replacement_node;
  }
}

/**
 * Set the contents of a DOMNode.
 *
 * @param \DOMNode $node
 *   A DOMNode or DOMElement object.
 * @param string $content
 *   The text or HTML that will replace the contents of $node.
 */
function entity_embed_set_dom_node_content(\DOMNode $node, $content) {
  // Remove all children of the DOMNode.
  while ($node->hasChildNodes()) {
    $node->removeChild($node->firstChild);
  }

  if (strlen($content)) {
    // Load the contents into a new DOMDocument and retrieve the elements.
    $replacement_nodes = entity_embed_dom_load_html($content)->getElementsByTagName('body')->item(0);

    // Finally, import and append the contents to the original node.
    foreach ($replacement_nodes->childNodes as $replacement_node) {
      $replacement_node = $node->ownerDocument->importNode($replacement_node, TRUE);
      $node->appendchild($replacement_node);
    }
  }
}

/**
 * Replace the contents of a DOMNode.
 *
 * @param \DOMElement $node
 *   A DOMElement object.
 * @param string $content
 *   The text or HTML that will replace the contents of $node.
 */
function entity_embed_replace_dom_node_content(\DOMElement $node, $content) {
  if (strlen($content)) {
    // Load the contents into a new DOMDocument and retrieve the element.
    $replacement_nodes = entity_embed_dom_load_html($content)->getElementsByTagName('body')
      ->item(0)
      ->childNodes;
  }
  else {
    $replacement_nodes = array($node->ownerDocument->createTextNode(''));
  }

  foreach ($replacement_nodes as $replacement_node) {
    // Import the replacement node from the new DOMDocument into the original
    // one, importing also the child nodes of the replacement node.
    $replacement_node = $node->ownerDocument->importNode($replacement_node, TRUE);
    $node->parentNode->insertBefore($replacement_node, $node);
  }
  $node->parentNode->removeChild($node);
}

/**
 * Convert the attributes on a DOMNode object to an array.
 *
 * This will also un-serialize any attribute values stored as JSON.
 *
 * @param \DOMNode $node
 *   A DOMNode object.
 *
 * @return array
 *   The attributes as an associative array, keyed by the attribute names.
 */
function entity_embed_get_dom_node_attributes_as_array(\DOMNode $node) {
  $return = array();

  // Convert the data attributes to the context array.
  foreach ($node->attributes as $attribute) {
    $key = $attribute->nodeName;
    $return[$key] = $attribute->nodeValue;

    // Check for JSON-encoded attributes.
    $data = json_decode($return[$key], TRUE, 10);
    if ($data !== NULL && json_last_error() === JSON_ERROR_NONE) {
      $return[$key] = $data;
    }
  }

  return $return;
}
