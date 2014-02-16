<?php
namespace Yahoo;

// Should it implement Iterator? I don't think so.
class NandishTableRowExtractor extends AbstractTableRowExtractor {

  private $dom;
  private $xpath;
  private $start_date;
  private $childNodesList;

  /*
   *  This is what should be passed as $xpath_query
   * '/html/body/table[3]/tr/td[1]/table[1]'
   *
   */
  public function __construct($base_url, $start_date, $xpath_table_query)
  {
     parent::__construct($base_url, $start_date, $xpath_table_query);
  } 


  // Returns: false if row did not have five columns or did not contain a US stock. 
  // TODO: Should input be $rowNodeList?
  function parseRow(DOMNodeList $rowNode, array &$cell_data)
  {
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
        
      // Test if the cell data is for a US stock
      $stock_length = strlen($row_data[1]);

      if (($stock_length > 1 && $stock_length < 5) && ( strpos($row_data[1], '.') === FALSE)) {
          
          // Change html entities back into ASCII (or Unicode) characters.             
          array_walk($row_data, function(&$item, $key) { $item = html_entity_decode($item); });
          
          $bool_return = true; 

      } else { 
          
          $bool_return = false; 
      }

      return $bool_return;
  }

  // Does getRowData() imply that this class is a table row iterator? Maybe I should make such an iterator?
  public function getRowData($date, $xpath_query) // Should I pass the xpath, or should it be in a general Config/Registry class?
  {
    $date_string = $date['month'] . '/' . $date['day'] . '/' . $date['year'];
    
    // get timestamp from date string.
    $time = strtotime($date_string);
    
    // date, in form DD-MON, as array[2], with no leading zeroes, 'j' means no leading zeroes
    // date_column will be used at the end of the row loop
    $date_column = date('j-M', $time);
    
    /* 
     * We need to as the $tableNodeElement->length to get the number of rows. We will subtract the first two rows --
     * the "Earnings Announcement ..." and the columns headers, and we ignore the last row.
     * Query Paths for the rows:
     * 1.  /html/body/table[3]/tr/td[1]/table[1]/tr[1] is "Earnings Announcements for Wednesday, May 15"
     * 2.  /html/body/table[3]/tr/td[1]/table[1]/tr[2] is column headers
     */
    
    // Skip first row. First row is "Earnings for ...".  Second row is column headers. 
    $all_row_data = array();
    
    // Iterate of rows, skipping first two -- header and column header -- and last row,
    for($i = 2; $i < $childNodesList->length - 1; $i++)  { // skip last row. it is a colspan.
        
       $rowNode =  $childNodesList->item($i); // getRowNode

        if (false == $this->parseTableRow($rowNode, $row_data)) {
            
            continue; 
        }          
          
        array_splice($row_data, 2, 0, $date_column);  // Insert the date column 

        $row_data[] = "Add"; // required hardcode value, but not sure what it means?
       
        // END Prospective parseRow() encapsulation
        $all_row_data[] = $row_data;

     } // end for
   } end getTableData

} // end class
?>
