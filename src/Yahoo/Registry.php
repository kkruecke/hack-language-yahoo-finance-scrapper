<?php
namespace Yahoo;

class Registry {

	private  static  $arrayObject;
	private  static  $initialized;
  public function __construct()
  {
	  // TODO: Read ini file
	  // 
     if (self::$arrayObject !== null) {
	     self::$arrayObject = new \ArrayObject();
     }
  }

  public static function set($property, $value) 
  {
	  
	  self::$arrayObject[$property] = $value;
  }

  public static function get($key)
  {
       if (self::$arrayObject->offsetExists($key)) {

	  return self::$arrayObject[$key];

       } else {

 	  return null;	

       }
  }
}

$r = new Registry();
$r::set('ini' , "some-value");
echo $r::get('ini');
