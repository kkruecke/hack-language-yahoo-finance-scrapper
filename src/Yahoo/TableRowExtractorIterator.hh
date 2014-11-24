<?hh
namespace Yahoo;

class TableRowExtractorIterator implements \Iterator {

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
     
     $this->current_row = 2; // We skip the first two rows, the table heading and the column header, respectively

     $this->end_iter = 0;    // This is required to make HHVM happy.
     $this->row_data = Vector {};

     $temp  =  $this->extractor->rowCount() - 1; // We skip the last row, thus - 1.

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
     return $this->current_row != $this->end_iter;
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

  protected function getRowData($rowid) : Vector<string> 
  {
     $row_data = Vector{};     

     // For first four td cells... 
     for($cellid = 0; $cellid < 4; $cellid++) {

        $row_data[] = $this->extractor->getCellText($rowid, $cellid);
     }	     
 
     return $row_data;
  }
 
} // end class
