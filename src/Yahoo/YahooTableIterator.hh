<?hh
namespace Yahoo;

class YahooTableIterator implements \SeekableIterator<Vector<string>>> {

  protected   YahooTable $html_table;
  protected   int $current_row;
  protected   Vector<string> $row_data;
  private     int $end;
  private     int $start_column;
  private     int $end_column;

  /*
   * Parameters: range of columns to return from each row.
   */
  public function __construct(YahooTable $htmltable, int $start_column, int $end_column)
  {
     $this->html_table = $htmltable;
     $this->start_column = $start_column; 
     $this->end_column = $end_column;

     $this->current_row = 0; 

     $this->end = 0;    // This is required to make HHVM happy.
     $this->row_data = Vector {};

      $this->end = $this->html_table->rowCount(); 
  }

  /*
   * Iterator methods
   */  
  public function rewind() : void
  {
     $this->current_row = 0;
  }

  public function valid() : bool
  {
     return $this->current_row != $this->end;
  }

  public function current() : Vector<string>
  {
    return   $this->getRowData($this->current_row);	  
  }

  public function key()  : int
  {
     return $this->current_row;
  }

  public function next() : void
  {
     ++$this->current_row;
  }
  public function seek(int $pos) : void
  {
	if ($pos < 0) {

             $this->current_row = 0;

	} else if ($pos < $this->end) {

	     $this->current_row = $pos;

	} else {

	     $this->current_row = $this->end % $pos;
	}
  }
  /*
   * returns Vector<string> of cell text for $rowid
   */ 
  protected function getRowData(int $rowid) : Vector<string>
  {
     $row_data = Vector{};     

     for($cellid = $this->start_column; $cellid < $this->end_column; $cellid++) {

        $row_data[] = $this->html_table->getCellText($rowid, $cellid);
     }	     
      
     // Change html entities back into ASCII (or Unicode) characters.             
     $row_data = $row_data->map( $x ==> { return html_entity_decode($x); } );

     return $row_data;
  }
 
} 
