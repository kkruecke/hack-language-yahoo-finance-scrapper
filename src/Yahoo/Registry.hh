<?hh
namespace Yahoo;

class Registry {

  private  static \ArrayObject $arrayObject;

  public function __construct()
  {
     /* TODO: Read ini file */
     if (self::$arrayObject == null) {	  
	  
        self::$arrayObject = new \ArrayObject(); // TODO: What should I pass ctor?
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
