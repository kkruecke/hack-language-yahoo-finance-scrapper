<?hh
namespace Yahoo;

class TableRowExtractorIterator extends AbstractTableRowIterator { // implements \Iterator {

  protected   string $start_date_col3;
  protected   int $current_row;
  protected   $row_data = array();
  private     int  $end_iter;

  /*
   *  This is what should be passed as $xpath_query
   * '/html/body/table[3]/tr/td[1]/table[1]'
   *
   */
  public function __construct(string $base_url, \DateTime $start_date, string $xpath_table_query)
  {
     parent::__construct($base_url, $xpath_table_query);
     
     // Convert the date entered as the argument into the form xx-yyy, where xx is the day as a digit with no
     // leading zeroes and yyy is the 3-letter abbrev. of the month.  'j' means no leading zeroes

     $this->start_date_col3 = $start_date->format('j-M');

     $this->current_row = 2;
     $this->end_iter = 0;    // This is required to make HHVM happy.

     $temp  =  $this->getRowsNodesList()->length - 1; // <-- Is "- 1" correct?

     $this->end_iter =  is_null($temp) ? 0 : $temp;

  }
  /*
   * Iterator methods
   */  
  public function rewind() : void
  {
     $this->current_row = 2;
     $this->getNextUSStock();
  }

  public function valid() : bool
  {
     return $this->current_row < $this->end_iter;
  }

  public function current()
  {
    return $this->row_data;
  }

  public function key() // 
  {
     return $this->current_row;
  }

  public function next()
  {
     ++$this->current_row;
     $this->getNextUSStock();
  }

  // Returns: false if row did not have five columns
  // Input be $rowNode
  protected function getRowData($row_id)
  {
     $row_data = array();

     $rowNode =  $this->getRowsNodesList()->item($row_id);
                            
     $tdNodeList = $rowNode->getElementsByTagName('td');
   
     // TODO: Make sure this is working compared to original code. 
     for($i = 0; $i < 4; $i++) {
         
        $index = $i;
        
        $td = $tdNodeList->item($i);
   
        $cell_text = $td->nodeValue;
             
        $rc = preg_match ('/^\s*$/', $cell_text); // Handles empty cells and cells with only whitespace, like last row.
                
        if ($rc == 1) {
            
            return false;
        }
        
        if ($i == 2) {

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
        } 	
   
        $row_data[$index] = $cell_text; 
     
     }
     
     ksort($row_data);
     return $row_data;
  }

  protected function isUSStock($row_data) : bool
  {
      // Test if the cell data is for a US stock
      $stock_length = strlen($row_data[1]);

      return (($stock_length > 1 && $stock_length < 5) && ( strpos($row_data[1], '.') === FALSE)) ? true : false;
  }

  protected function addDataSuffix(&$row_data) : void 
  {  
     // date, in form DD-MON, as array[2], with no leading zeroes, 'j' means no leading zeroes
     array_splice($row_data, 2, 0, $this->start_date_col3);   

     $row_data[] = "Add"; // required hardcoded value
  } 
  /*
   * Sets $this->row_data
   */ 
  protected function getNextUSStock()
  {
     for (;$this->current_row < $this->end_iter; $this->current_row++) {

         $row_data = array();
         
         $row_data = $this->getRowData($this->current_row);
         
         // We only keep rows with all four columns of data
         if (count($row_data) != 4) {

              continue;
         }
 
         // Only want US stocks.
         if ($this->isUSStock($row_data)) {

             // This is non-web page data that is required in the output.
             $this->addDataSuffix($row_data);
 
             // Change html entities back into ASCII (or Unicode) characters.             
	     // TODO: change to Hack lambda
	     array_walk($row_data,
		     function(&$item) { $item = html_entity_decode($item); }
	               );

             $this->row_data = $row_data;
 
             return;
         }
     } 
  }

} // end class
