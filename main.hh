<?hh
use Yahoo\CSVWriter;
use Yahoo\TableRowExtractorIterator;

require_once("loader/SplClassLoader.php");
require_once("utility.hh");

$spl_loader = new SplClassLoader('Yahoo', 'src/');

$spl_loader->register();

define('YAHOO_BIZ_URL', "http://biz.yahoo.com/research/earncal/");

define('HELP', "How to use: Enter a date in mm/dd/YYYYY format follow by number between 0 and 40.\n");

  if ($argc == 2) {

    $argv[2] = 0; 
    $argc = 3;
  }

  $error_msg = '';

  if (validate_input($argc, $argv, $error_msg) == false) {

       echo $error_msg;

       echo HELP . "\n"; 
       return;
  }

  $data_period = build_date_period($argv[1], (int) $argv[2]);

  /*
   *
   $number_of_days = (int) $argv[2];

   $number_of_days_plus1 = $number_of_days + 1;

  // start date DateTime 
  $start_date = DateTime::createFromFormat('m/d/Y', $argv[1]); 
  
  $one_day_interval = new DateInterval('P1D');
  
  // format end date DateTime
  $end_date = DateTime::createFromFormat('m/d/Y', $argv[1]); 
  
  $end_date->add(new DateInterval("P" . $number_of_days_plus1 . "D")); 
  
  $date_period = new DatePeriod($start_date, $one_day_interval, $end_date);
  */  
  validate_urls_existence($date_period);
  
  $csv_writer = new CSVWriter($start_date, $argv[2]);

  // Start main loop
  foreach ($date_period as $date) {
      
      // Build yyyymmdd.html name
      $url = YAHOO_BIZ_URL . $date->format('Ymd') . ".html";
      
      if (url_exists($url) == false) {
          
           echo 'The Yahoo url for date ' . $date->format("m-d-Y") . ", $url , " . " does not exists. Skipping\n";               
           continue;    
      }
      
      try {
     
         $rowExtractorIter = new TableRowExtractorIterator($url, $date, '/html/body/table[3]/tr/td[1]/table[1]');
  
          foreach($rowExtractorIter as $stock_data) {
         
               $csv_writer->writeLine($stock_data); 
          }
  
      } catch(Exception $e) {
          
          echo $e->getMessage() . "\n";
          return;
      }
  }

  $us_stock_count = $csv_writer->getLineCount();
  
  echo "A total of " . $us_stock_count . " US stocks were extracted.\n";
  echo  $csv_writer->getFileName() . " has been created. It contains $us_stock_count US stocks entries.\n";
    
  return;
