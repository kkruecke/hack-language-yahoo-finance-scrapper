<?php
namespace Yahoo;

// Should it implement Iterator? I don't think so.
class NandishTableRowExtractor extends AbstractTableRowExtractor {

  private $base_url;
  private $parser;

  public __construct($base_page, $start_date, $number_of_days)
  {
     $this->base_url = $base_page;
     $this->start_date = $start_date;
     $this->number_of_days = $number_of_days;
     $this->parser = $row_parser;
  }

  public function getRowData($date, $xpath_query) // Should I pass the xpath, or should it be in a general Config/Registry class?
  {

    //--$xpathNodeList = $xpath->query('/html/body/table[3]/tr/td[1]/table[1]');
    $xpathNodeList = $xpath->query($xpath_query);

    // Build yyyymmdd.html name
     $html_file_name = sprintf("%d%02d%02d.html", $date['year'], $date['month'], $date['day']);
     
     $url = $this->base_url . $html_file_name;
    
     // Do I need to download the filer?        
     $page = file_get_contents($url);
     
    //Debug:- file_put_contents("./$html_file_name", $page); // Debug only
    
     // a new dom object
    $dom = new DOMDocument;
     
    // load the html into the object
    $dom->strictErrorChecking = false; // default is true.
    $dom->loadHTML($page);
     
    // discard redundant white space
    $dom->preserveWhiteSpace = false;
    
    $xpath = new DOMXPath($dom);
    
    // returns nodelist -- must first get the first and only node, the table.
    $xpathNodeList = $xpath->query('/html/body/table[3]/tr/td[1]/table[1]');
    
    if ($xpathNodeList->length != 1) {
        
        echo "XPath Query Failed. Page format has evidently changed. Cannot proceed\n";
        exit;
    } 
    
    $date_string = $date['month'] . '/' . $date['day'] . '/' . $date['year'];
    
    // get timestamp from date string.
    $time = strtotime($date_string);
    
    // date, in form DD-MON, as array[2], with no leading zeroes, 'j' means no leading zeroes
    // date_column will be used at the end of the row loop
    $date_column = date('j-M', $time);
    
    $tableNodeElement = $xpathNodeList->item(0);
    
    /* 
     * We need to as the $tableNodeElement->length to get the number of rows. We will subtract the first two rows --
     * the "Earnings Announcement ..." and the columns headers, and we ignore the last row.
     * Query Paths for the rows:
     * 1.  /html/body/table[3]/tr/td[1]/table[1]/tr[1] is "Earnings Announcements for Wednesday, May 15"
     * 2.  /html/body/table[3]/tr/td[1]/table[1]/tr[2] is column headers
     */
    
     if ( $tableNodeElement->hasChildNodes())  {
    
        // loop through rows
        $childNodesList = $tableNodeElement->childNodes;
        $row_count = $childNodesList->length;
    
        // Skip first row. First row is "Earnings for ...".  Second row is column headers. 
        $cell_data = array();
        $row_count--; // ignore last row
        
        for($i = 2; $i < $row_count; $i++)  { // skip last row. it is a colspan.
            
           $rowNode =  $childNodesList->item($i);

            // START prospective parseRow(), which is an abstract base class method.                                
            if (false == get_cells_from_row($rowNode, $cell_data)) {
                
                continue; // if row did not have five columns of data
            }          
            
   	    // Test if the cell data is for a US stock
            $stock_length = strlen($cell_data[1]);
            
            if (($stock_length > 1 && $stock_length < 5) && ( strpos($cell_data[1], '.') === FALSE)) {
              
                 // Change html entities back into ASCII (or Unicode) characters.             
                 array_walk($cell_data, function(&$item, $key) { $item = html_entity_decode($item); });
               
            } else {
                 // skip the row; it if it is not a US Stock
                 continue;
            }
    
	    array_splice($cell_data, 2, 0, $date_column);   
            $cell_data[] = "Add"; // required hardcode value
           
            // END Prospective parseRow() encapsulation
	    $row_data[] = $cell_data;

         } // end for

      } // end if

   } end getTableData

  // input? 
  // output? 
   protected function ParseRowData($data)
   {
       // START prospective parseRow(), which is an abstract base class method.                                
        if (false == get_cells_from_row($rowNode, $cell_data)) {
            
            continue; // if row did not have five columns of data
        }          
        
        // Test if the cell data is for a US stock
        $stock_length = strlen($cell_data[1]);
        
        if (($stock_length > 1 && $stock_length < 5) && ( strpos($cell_data[1], '.') === FALSE)) {
          
             // Change html entities back into ASCII (or Unicode) characters.             
             array_walk($cell_data, function(&$item, $key) { $item = html_entity_decode($item); });
           
        } else {
             // skip the row; it if it is not a US Stock
             continue;
        }
    
        array_splice($cell_data, 2, 0, $date_column);   
        $cell_data[] = "Add"; // required hardcode value

   }

} // end class
?>
