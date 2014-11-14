<?hh
namespace Yahoo;

class CSVWriter {

  private $fh;
  private $file_name;
  private $line_count;
  
  public function __construct($start_date, int $number_of_days)
  {
   /* j --> day without leading zeroes
    * m --> month with leading zeroes
    * T --> four digit year
    */
    
    $this->file_name = $start_date->format('jmY') . "-plus-$number_of_days";
    
    $this->file_name .= '.csv';
       
    $this->fh = fopen($this->file_name, "w");
    
    $this->line_count = 0;
  }

  public function getFileName()
  {
     return $this->file_name;
  }

  public function __destruct()
  {
     fclose($this->fh);
  }

  public function writeLine($row_data)
  {
      $csv_str = implode(',', $row_data);
      
      $csv_str .= "\n"; // replace terminating ',' with newline.
                   
      fputs($this->fh, $csv_str);
      
      $this->line_count++;
  }
  
  public function getLineCount()
  {
      return $this->line_count;
  }

}
