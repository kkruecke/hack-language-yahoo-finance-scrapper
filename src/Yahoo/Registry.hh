<?hh
namespace Yahoo;

class Registry {

  static private  \ArrayObject $array_object; 

  public function __construct()
  {
     if (!isset(self::$array_object)) {	  
	  
        @$ini_map = parse_ini_file("maude.ini", true); 
        self::$array_object = new \ArrayObject($ini_map); 
     }
  }

  public static function register($property, $value) : void
  {
	  self::$array_object->offsetSet($property, $value);
  }

  public static function registry($key) : mixed
  {
       if (self::$array_object->offsetExists($key)) {

	  return self::$array_object->offsetGet($key);

       } else {

 	  return null;	

       }
  }
}
