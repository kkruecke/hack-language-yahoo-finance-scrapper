<?hh
namespace Yahoo;

class Registry {

  static private  \ArrayObject $arrayObject; 

  public function __construct()
  {
     /* TODO: Read ini file */
     if (!isset(self::$arrayObject)) {	  
	  
        self::$arrayObject = new \ArrayObject(); 
     }
  }

  public static function set($property, $value) : void
  {
      self::$arrayObject[$property] = $value;
  }

  public static function get($key) : mixed
  {
       if (self::$arrayObject->offSetExists($key)) {

	  return self::$arrayObject[$key];

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
