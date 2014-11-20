<?hh
/*
 * Used be CSVWriter to return customize output
 * Should this be an Abstract class or an Interface since we don't need an implementation.
 * Interface seems the best choice.
 */ 
class FormatterStrategy {

   public function __construct() // input Stock?
   {
   }

   public function format() : string
   {
	   $str = "";
	   return $str; 

   }

}
