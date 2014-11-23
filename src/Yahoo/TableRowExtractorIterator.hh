<?hh
namespace Yahoo;

class TableRowExtractorIterator implements \Iterator {

  protected   string $startDate_column3;
  protected   int $current_row;
  protected   Vector<mixed> $row_data;
  private     int  $end_iter;

  private    TableRowExtractor $extractor; 

  /*
   *  This is what should be passed as $xpath_query
   * '/html/body/table[3]/tr/td[1]/table[1]'
   *
   */
  public function __construct(string $base_url, \DateTime $startDate, string $xpath_table_query)
  {
      $this->extractor = new TableRowExtractor($base_url, $xpath_table_query);
     
     // Convert the date entered as the argument into the form xx-yyy, where xx is the day as a digit with no
     // leading zeroes and yyy is the 3-letter abbrev. of the month.  'j' means no leading zeroes

     $this->startDate_column3 = $startDate->format('j-M');

     $this->current_row = 2; // We skip the first two rows, the table heading and the column header, respectively

     $this->end_iter = 0;    // This is required to make HHVM happy.
     $this->row_data = Vector {};

     $temp  =  $this->extractor->getRowsNodesList()->length - 1; // We skip the last row, thus -1.

     $this->end_iter =  is_null($temp) ? 0 : $temp;
  }
  /*
   * Iterator methods
   */  
  public function rewind() : void
  {
     $this->current_row = 2;
     $this->getNextRow();
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
     $this->getNextRow();
  }
  /*
   * Sets $this->row_data
   */ 
  protected function getNextRow() : void
  {
      $row_data = $this->getRowData($this->current_row);
      
      // Change html entities back into ASCII (or Unicode) characters.             
      $row_data = $row_data->map( $x ==> { return html_entity_decode($x); } );

      $this->row_data = $row_data;
  }

  // Input be $rowNode
  protected function getRowData($row_id) : Vector<mixed>
  {
     $row_data = Vector{};

     // get DOMNode for row $row_id
     $rowNode =  $this->extractor->getRowsNodesList()->item($row_id);

     // get DOMNodeList for td cells in the row     
     $tdNodeList = $rowNode->getElementsByTagName('td');
   
     // for first four td cells... 
     for($i = 0; $i < 4; $i++) {
         
        $index = $i;

        // Get td DOMNode for td cell number $i from DOMNodeList
        $td = $tdNodeList->item($i);
   
	$cell_text = $td->nodeValue;

        $row_data[] = $cell_text; 
             
     } 
   
     return $row_data;
  }

  // get td node list for row 
  protected function getTdNodeList($row_id) : \DOMNodeList
  {
     $row_data = Vector{};

     // get DOMNode for row $row_id
     $rowNode =  $this->extractor->getRowsNodesList()->item($row_id);

     // get DOMNodeList for td cells in the row     
     $tdNodeList = $rowNode->getElementsByTagName('td');

     return $tdNodeList;
  }
 
} // end class
