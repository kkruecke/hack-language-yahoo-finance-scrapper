<?hh
namespace Yahoo;

/*
 * Used be CSVWriter to return customize output
 * Should this be an Abstract class or an Interface since we don't need an implementation.
 * Interface seems the best choice.
 */ 
class CSVYahooFormatter implements CSVFormatter {

   public function format(Vector<string> $row, \DateTime $date) : string
   {
      
     if ($row->count() < 4) {

	  throw new \RangeException("Size of Vector<string> is less than four\n");
     }	   
     // Remove commas from company names

    $row[0] = str_replace(',', "", $row[0]);

     // Alter a column per specification.txt     
     $column4_text = $row[3];	   
   
     if (is_numeric($column4_text[0])) { // a time was specified
   
           $column4_text =  'D';
   
      } else if (FALSE !== strpos($column4_text, "After")) { // "After market close"
   
            $column4_text =  'A';
   
      } else if (FALSE !== strpos($column4_text, "Before")) { // "Before market close"
   
           $column4_text =  'B';
   
      } else if (FALSE !== strpos($column4_text, "Time")) { // "Time not supplied"
   
         $column4_text =  'U';
   
      } else { // none of above cases
   
           $column4_text =  'U';
      }  
  
     /*
      * This is taken from the prior php code TableRowExtractorIterator::addDataSuffix() method, which was invoked after
      * TableRowExtractorIterator::getRowData()
      */
     $date = $date->format('j-M');
    
     $array = $row->toArray();

     array_splice($array, 2, 0, $date); // Insert date after first third columns.

     $temp = $array[3];
     
     $array[3] = $column4_text;
     
     $array[4] = $temp;
     
     $array[] = "Add"; // Also taken from TableRowExtractorIterator::addDataSuffix() in prior PHP code.

     $csv_str = implode(",", $array);

     return $csv_str;
   }

}
