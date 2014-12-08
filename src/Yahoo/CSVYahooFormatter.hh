<?hh
namespace Yahoo;

function debug_yahoo_formatter() : void
{
	$debug = 10;
	return;
}


/*
 * Used be CSVWriter to return customize output
 * Should this be an Abstract class or an Interface since we don't need an implementation.
 * Interface seems the best choice.
 */ 
class CSVYahooFormatter implements CSVFormatter {

   private \DateTime $start_date;

   public function __construct(\DateTime $start_date) // input Stock?
   {
	   $this->start_date = $start_date;
   }

   public function format(Vector<string> $row) : string
   {
      
     if ($row->count() < 4) {

	  throw new \RangeException("Size of Vector<string> is less than four\n");
     }	   

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
      
     $row[2] = $column3_text; // TODO: is index of 2 correct? Am I overwriting existing data.
      /*
       * This is the prior php code's TableRowExtractorIterator::addDataSuffix() method, which was invoked after
       * TableRowExtractorIterator::getRowData()
       */
     array_splice($row, 2, 0, array($this->start_date->format('j-M')));

     $row[] = "Add"; // Also from addDataSuffix()

     $csv_str = implode(",", $row);

     return $csv_str;
   }

}
