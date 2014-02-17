<?php
namespace Yahoo;

class CSVWriter {

  private $fh;

  public function __construct($date)
  {
   /* j --> day without leading zeroes
    * m --> month with leading zeroes
    * T --> four digit year
    */
    
    $csv_file_name = $date->format('j') . $date->format('m') . $date->format('Y');
    
    $csv_file_name .= '.csv';
    
    $this->fh = fopen($csv_file_name, "w");
  }

  public function __destruct()
  {
     fclose($this->fh);
  }

  public function write_csv_file($row_data, $date)
  {
      $csv_str = implode(',', $row_data);
      
      $csv_str .= "\n"; // replace terminating ',' with newline.
                   
      fputs($this->fh, $csv_str);
  }

}
?>
