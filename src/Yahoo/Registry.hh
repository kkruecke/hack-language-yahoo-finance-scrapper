<?hh
namespace Yahoo;

class Registry  {

  static private \ArrayObject  $registry; 

  public function __construct(array $ini_array)
  {
    //  self::init();
  }

  protected static function init()
  {	  
     if (!isset(self::$registry)) {	  
	  
        @$ini_map = parse_ini_file("yahoo.ini", true); 
        self::$registry = new Registry($ini_map); 
     }
  }

  public static function register($property, $value) : void
  {
	  self::init();
	  self::$registry->offsetSet($property, $value);
  }

  public static function registry($key) : mixed
  {
       self::init();
       // Debug
       if (isset(self::$registry)) {
	       echo 'self::$registry isset' . "\n";

	       $reflection = new ReflectionClass('MyClass');

               var_dump( $reflection->getMethods(ReflectionMethod::IS_STATIC) );
	      
       }
       if (self::$registry->offsetExists($key)) {

	  return self::$registry->offsetGet($key);

       } else {

 	  return null;	
       }
  }
}
