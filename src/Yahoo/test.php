<?php
namespace Yahoo;

class Registry {

  static private   $arrayObject;

  public function __construct()
  {
     if (self::$arrayObject == null) {	  
	  
        self::$arrayObject = new \ArrayObject(\ArrayObject::STD_PROP_LIST); // TODO: What should I pass ctor?
     }
  }

  public static function set($property, $value) 
  {
      self::$arrayObject->property = $value;
  }

  public static function get($key) 
  {
       if (self::$arrayObject->offSetExists($key)) {

	  return self::$arrayObject->key;

       } else {

 	  return null;	

       }
  }
}

$r1 = new Registry();
$r2 = new Registry();
$r1::set('key', 'some-value');
$x = $r1::get('key');

var_dump($x);
echo "\n";
var_dump($r1);
