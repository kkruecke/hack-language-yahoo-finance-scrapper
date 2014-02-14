<?php
namespace Yahoo;

// Should it implement Iterator? I don't think so.
abstract class AbstractTableRowExtractor implements Iterator {

  private $dom;
  private $xpath;
  private $start_date;
  private $childNodesList;
 
  /* Iterator interface methods */
  /* Methods */
  abstract public mixed current ( void );
  abstract public scalar key ( void );
  abstract public void next ( void );
  abstract public void rewind ( void );
  abstract public boolean valid ( void );

  /*
   *  This is what should be passed as $xpath_query
   * '/html/body/table[3]/tr/td[1]/table[1]'
   *
   */
  public __construct($base_url, $start_date, $xpath_table_query)
  {
     // Build yyyymmdd.html name
     $html_file_name = sprintf("%d%02d%02d.html", $date['year'], $date['month'], $date['day']);
     
     $url = $base_url . $html_file_name;
    
     // Do I need to download the filer?        
     $page = file_get_contents($url);
     
    //Debug:- file_put_contents("./$html_file_name", $page); // Debug only
    
     // a new dom object
    $this->dom = new DOMDocument;
     
    // load the html into the object
    $this->dom->strictErrorChecking = false; // default is true.
    $this->dom->loadHTML($page);
     
    // discard redundant white space
    $this->dom->preserveWhiteSpace = false;
    
    $this->xpath = new DOMXPath($this->dom);
    
    // returns nodelist -- must first get the first and only node, the table.
    // 
    $xpathNodeList = $this->xpath->query($xpath_table_query);
    
    if ($xpathNodeList->length != 1) { // TODO: Change to throw Exception
        
        throw new Exception("XPath Query\n $xpath_table_query\n   Failed. Page format has evidently changed. Cannot proceed.\n");
    } 
 
    $tableNodeElement = $xpathNodeList->item(0);
    
    /* 
     * We need to as the $tableNodeElement->length to get the number of rows. We will subtract the first two rows --
     * the "Earnings Announcement ..." and the columns headers, and we ignore the last row.
     * Query Paths for the rows:
     * 1.  /html/body/table[3]/tr/td[1]/table[1]/tr[1] is "Earnings Announcements for Wednesday, May 15"
     * 2.  /html/body/table[3]/tr/td[1]/table[1]/tr[2] is column headers
     */
    
     if (!$tableNodeElement->hasChildNodes()) {
         
        throw new Exception("This is no table element at \n $xpath_table_query\n. Page format has evidently changed. Cannot proceed.\n");

     } else {

        // Seems ok 
        $childNodesList = $tableNodeElement->childNodes;
     }  

  } // end __construct()


  // code simply copy from getyahoo.php
  // Does it belong it here. See question below about whether getRowData() implies that this class is a table row iterator? Maybe I should make such an iterator?
  abstract function parseRow($rowNode, &$cell_data);
  // Since the method getRowData() iterators of the rows, it is a candidate for an iterator.
  //
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
    $cell_data = array();
    
    // Iterate of rows, skipping first two -- header and column header -- and last row,
    for($i = 2; $i < $childNodesList->length - 1; $i++)  { // skip last row. it is a colspan.
        
       $rowNode =  $childNodesList->item($i);

        // START prospective parseRow(), which is an abstract base class method.                                
        if (false == $this->parseRowData($rowNode, $cell_data)) {
            
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
   } end getTableData

} // end class
?>
