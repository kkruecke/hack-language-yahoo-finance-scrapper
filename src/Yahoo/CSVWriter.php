<?php
namespace Yahoo;

class CSVWriter {

  private $fh;

  public function __construct($start_date, $number_of_days)
  {
   /* j --> day without leading zeroes
    * m --> month with leading zeroes
    * T --> four digit year
    */
    
    $csv_file_name = $start_date->format('j') . $start_date->format('m') . $start_date->format('Y') . "plus-$number_of_days";
    
    $csv_file_name .= '.csv';
    
    $this->fh = fopen($csv_file_name, "w");
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
  }

}
?>
