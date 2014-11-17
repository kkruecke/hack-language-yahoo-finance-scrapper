<?hh
namespace Yahoo;

class Registry {

  static private  \ArrayObject $array_object; 

  public function __construct()
  {
     if (!isset(self::$array_object)) {	  
	  
        self::$array_object = new \ArrayObject(); 

        /* TODO: Read ini file */
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

try {
  $r1 = new Registry();
  $r2 = new Registry();
  $r1::set('key', 'some-value');
  $x = $r1::get('key');
  
  var_dump($x);
  echo "\n";
  print_r($r1);
} catch (\Exception $e) {

	echo $e->getMessage(). "\n============\n\n";
	echo $e->getTraceAsString() . "\n";
}
