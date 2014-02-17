<?php
namespace Yahoo;

class CSVWriter {

  private $fh;

  public function __construct($date)
  {
  
    $csv_file_name = sprintf("%d%02d%02d", $date['month'], $date['day'],  $date['year']);
    
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
