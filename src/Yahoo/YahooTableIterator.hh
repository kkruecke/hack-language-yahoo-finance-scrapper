<?hh
namespace Yahoo;

class YahooTableIterator implements \Iterator {

  protected   YahooTable $html_table;
  protected   int $current_row;
  protected   Vector<string> $row_data;
  private     int  $end_iter;

  public function __construct(YahooTable $htmltable)
  {
     $this->html_table = $htmltable;

     // TODO: Remove this and have the FilterIterator set this.  
     $this->current_row = 0; // We skip the first two rows, the table heading and the column header, respectively

     $this->end_iter = 0;    // This is required to make HHVM happy.
     $this->row_data = Vector {};

     $temp  =  $this->html_table->rowCount() - 1; // We skip the last row, thus - 1.

     $this->end_iter =  is_null($temp) ? 0 : $temp;
  }
  /*
   * Iterator methods
   */  
  public function rewind() : void
  {
     $this->current_row = 0;
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

  public function key()  
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
     $row_data = Vector{};     

     // For first four td cells... 
     for($cellid = 0; $cellid < 4; $cellid++) {

        $row_data[] = $this->html_table->getCellText($this->current_row, $cellid);
     }	     
      
      // Change html entities back into ASCII (or Unicode) characters.             
      $this->row_data = $row_data->map( $x ==> { return html_entity_decode($x); } );
  }
 
} // end class
