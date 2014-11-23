<?hh
namespace Yahoo;

// TODO: 
// 1. Is <mixed> the correct type of should it be  <string>
// 2. Do we also want to implement ArrayAccess
class TableRowExtractor {

   private   \DOMDocument $dom;	
   private   \DOMXPath $xpath;	
   private   \DOMNodeList $trNodesList;

   protected $trdNodesList;
 
  /*
   *  This is what should be passed as $xpath_query
   * '/html/body/table[3]/tr/td[1]/table[1]'
   *
   */
  
  public function __construct(string $url, string $xpath_table_query)
  {

     $page = @file_get_contents($url);

    //Debug:- file_put_contents("./$html_file_name", $page); // Debug only
    
     // a new dom object
    $this->dom = new \DOMDocument();
     
    // load the html into the object
    $this->dom->strictErrorChecking = false; // default is true.

    @$this->dom->loadHTML($page);  // Turn off error reporting
     
    // discard redundant white space
    $this->dom->preserveWhiteSpace = false;
    
    $this->xpath = new \DOMXPath($this->dom);
    
    // returns \DOMNodeList. We must first get the first and only node, the table.
    // 
    $xpathNodeList = $this->xpath->query($xpath_table_query);
    
    if ($xpathNodeList->length != 1) { 
        
        throw new Exception("XPath Query\n $xpath_table_query\n   Failed. Page format has evidently changed. Cannot proceed.\n");
    } 

    // DOMNode representing the table. 
    $tableNodeElement = $xpathNodeList->item(0);
    
    /* 
     * We need to as the $tableNodeElement->length to get the number of rows. We will subtract the first two rows --
     * the "Earnings Announcement ..." and the columns headers -- and we ignore the last row.
     * Path queries for the first two rows:
     *
     * 1.  /html/body/table[3]/tr/td[1]/table[1]/tr[1] is "Earnings Announcements for Wednesday, May 15"
     * 2.  /html/body/table[3]/tr/td[1]/table[1]/tr[2] is column headers
     */
    
     if (!$tableNodeElement->hasChildNodes()) {
         
        throw new Exception("This is no table element at \n $xpath_table_query\n. Page format has evidently changed. Cannot proceed.\n");

     } 
     // DOMNodelist for rows of the table
     $this->trNodesList = $tableNodeElement->childNodes;

  } // end __construct()

   public function getRowsNodesList() : \DOMNodeList
   {
	 return $this->trNodesList;
   }


   // get td node list for row 
  public function getTdNodeList($row_id) : \DOMNodeList
  {
     // get DOMNode for row $row_id
     $rowNode =  $this->getRowsNodesList()->item($row_id);

     // get DOMNodeList for td cells in the row     
     $tdNodeList = $rowNode->getElementsByTagName('td');

     return $tdNodeList;
  }
 
} // end class
