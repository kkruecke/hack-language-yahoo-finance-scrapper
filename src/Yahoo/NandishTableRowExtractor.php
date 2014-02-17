<?php
namespace Yahoo;

// TODO: put reuseable code in base class 
class NandishTableRowExtractor extends AbstractTableRowExtractor implements \Iterator {

  private $start_date;
  private $current_row;
  private $row_data = array();
  private $end_iter;

  /*
   *  This is what should be passed as $xpath_query
   * '/html/body/table[3]/tr/td[1]/table[1]'
   *
   */
  public function __construct($base_url, $start_date, $xpath_table_query)
  {
     //TODO: What instance variables does base class have?

     parent::__construct($base_url, $start_date, $xpath_table_query);
     $this->end_iter =  $this->trNodesList->length - 1; // <-- Is "- 1" correct?
  }

  // Returns: false if row did not have five columns or did not contain a US stock. 
  // Input be $rowNode
  protected function getRowData($row_id)
  {
     $rowNode =  $this->trNodeList->item($row_id);
                            
     $tdNodeList = $rowNode->getElementsByTagName('td');
   
     for($i = 0; $i < 4; $i++) {
   
        $td = $tdNodeList->item($i);
   
        $cell_text = $td->nodeValue;
             
        $rc = preg_match ('/^\s*$/', $cell_text); // Handles empty cells and cells with only whitespace, like last row.
                
        if ($rc == 1) {
            
            return false;
        }
            
        if ($i == 3) {
   
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
        }
   
        $cell_data[] = $cell_text; 
     
      }
        
     // TODO: This may not be needed every time, and can be moved somewhere else like ctor.
     // 
     $date_string = $date['month'] . '/' . $date['day'] . '/' . $date['year'];
    
     // get timestamp from date string.
     $time = strtotime($date_string);
    
     // date, in form DD-MON, as array[2], with no leading zeroes, 'j' means no leading zeroes
     // date_column will be used at the end of the row loop
     $date_column = date('j-M', $time);
    
     array_splice($cell_data, 2, 0, $date_column);   
     $cell_data[] = "Add"; // required hardcode value

     return $bool_return;
  }

  function isUSStock($row_data)
  {
      // Test if the cell data is for a US stock
      $stock_length = strlen($row_data[1]);

      /* 
      if (($stock_length > 1 && $stock_length < 5) && ( strpos($row_data[1], '.') === FALSE)) {
          
          $bool_return = true; 

      } else { 
          
          $bool_return = false; 
      }

      return $bool_return;
      */

      return (($stock_length > 1 && $stock_length < 5) && ( strpos($row_data[1], '.') === FALSE)) ? true : false;
  }

 
  protected function getNextUSStock()
  {
     while ($this->current_row < $this->end_iter) {

         $this->row_data = $this->getRowData($this->current_row);

         if ($this->isUSStock($this->row_data)) {

             // Change html entities back into ASCII (or Unicode) characters.             
             array_walk($row_data, function(&$item, $key) { $item = html_entity_decode($item); });
             return;
         }

         $this->current_row++;
     } 
  }
 
  public function rewind()
  {
     $this->current_row = 2;
     $this->getNextUSStock();
  }

  public function valid()
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
  }

} // end class
?>
