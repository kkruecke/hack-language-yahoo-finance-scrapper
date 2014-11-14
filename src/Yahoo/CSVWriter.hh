<?hh
namespace Yahoo;

class CSVWriter {

  private \SplFileObject $splfile;
  private string         $file_name;
  private int            $line_count;
  
  public function __construct($start_date, int $number_of_days)
  {
   /* j --> day without leading zeroes
    * m --> month with leading zeroes
    * T --> four digit year
    */
    
    $this->file_name = $start_date->format('jmY') . "-plus-$number_of_days";
    
    $this->file_name .= '.csv';
                 
    $this->splfile = new \SplFileObject($this->file_name, "w");

    $this->splfile->setFlags(\SplFileObject::READ_AHEAD | \SplFileObject::SKIP_EMPTY);
       
    $this->line_count = 0;
  }

  public function getFileName()
  {
     return $this->file_name;
  }

  public function __destruct()
  {
     unset($this->splfile);
  }

  public function writeLine(array $row_data) : void
  {
      $csv_str = implode(',', $row_data);
      
      $csv_str .= "\n"; // replace terminating ',' with newline.
                   
      $this->splfile->fputs($csv_str);
      
      $this->line_count++;
  }
  
  public function getLineCount() : int
  {
      return $this->line_count;
  }

}
