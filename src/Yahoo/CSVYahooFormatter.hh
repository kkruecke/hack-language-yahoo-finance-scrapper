<?hh
/*
 * Used be CSVWriter to return customize output
 * Should this be an Abstract class or an Interface since we don't need an implementation.
 * Interface seems the best choice.
 */ 
class CSVYahooFormatter implements CSVFormatter {

   public function __construct() // input Stock?
   {
   }

   public function format(&Vector<string> $row) : string
   {
     $column3_text = $row[3];	   
       
   
      if (is_numeric($column3_text[0])) { // a time was specified
   
           $column3_text =  'D';
   
      } else if (FALSE !== strpos($column3_text, "After")) { // "After market close"
   
            $column3_text =  'A';
   
      } else if (FALSE !== strpos($column3_text, "Before")) { // "Before market close"
   
           $column3_text =  'B';
   
      } else if (FALSE !== strpos($column3_text, "Time")) { // "Time not supplied"
   
         $column3_text =  'U';
   
      } else { // none of above cases
   
           $column3_text =  'U';
      }  
            
      $row[3] = $column3_text;
   }

}
