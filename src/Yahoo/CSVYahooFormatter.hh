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

   public function format() : string
   {
     // for first four td cells... 
     for($i = 0; $i < 4; $i++) {
         
        $index = $i;

        // Get td DOMNode for td cell number $i from DOMNodeList
        $td = $tdNodeList->item($i);
   
        $cell_text = $td->nodeValue;
             
        $rc = preg_match ('/^\s*$/', $cell_text); // Handles empty cells and cells with only whitespace, like last row.
                
        if ($rc == 1) {
            
	    // Prior code: return false;
	    // TODO: Do I need to a check to the caller?
	    return Vector {};	 
        }
        
        if ($i == 2) { // third cell

	   $index = 3;
           
        }  else if ($i == 3) {
   
            if (is_numeric($cell_text[0])) { // a time was specified
   
                 $cell_text =  'D';
   
            } else if (FALSE !== strpos($cell_text, "After")) { // "After market close"
   
                  $cell_text =  'A';
   
            } else if (FALSE !== strpos($cell_text, "Before")) { // "Before market close"
   
                 $cell_text =  'B';
   
            } else if (FALSE !== strpos($cell_text, "Time")) { // "Time not supplied"
   
  	       $cell_text =  'U';
   
            } else { // none of above cases
   
                 $cell_text =  'U';
            }  
            
	    $index = 2;

        } // end for 	
 
	   $str = "";
	   return $str; 

   }

}
