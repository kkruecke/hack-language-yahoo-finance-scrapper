<?hh
 
// Change html entities back into ASCII (or Unicode) characters.             
 $v = Vector { "&amp;", "&larr;" };

 $v = $v->map( $t ==> { return html_entity_decode($t); } ); // function(&$item) { $item = html_entity_decode($item); } );

echo "\nprinting v \n";
 var_dump($v);
 echo "\n";
return;

 $a = array( "&amp;", "&larr;" );
 array_walk($a,
           function(&$item) { $item = html_entity_decode($item); }
             );

 print_r($a);
 echo "\n";
