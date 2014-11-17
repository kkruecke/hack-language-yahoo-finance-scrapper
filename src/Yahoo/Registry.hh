<?hh
namespace Yahoo;

class Registry {

  private  static \ArrayObject $arrayObject;

  public function __construct()
  {
     // TODO: Read ini file
     $self::arrayObject = new ArrayObject();
  }

  public static function set($property, $value) 
  {
	  
	  self::$arrayObject[$property] = $value;
  }

  public static function get($key) : mixed
  {
       if (offsetExists(self::$arrayObject)) {

	  return self::$arrayObject[$key];

       } else {

 	  return null;	

       }
  }
}
