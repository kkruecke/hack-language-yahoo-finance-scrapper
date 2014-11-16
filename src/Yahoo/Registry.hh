<?hh
namespace Yahoo;

class Registry extends \ArrayObject {

//  private  $variables;

  public function __construct()
  {
	  parent::__construct();
  }

  public function __set($property, $value) 
  {
	  
         $this->property = $vl;
  }

  public function __get($dt) 
  {
      return $this->data[$dt];
  }
}
