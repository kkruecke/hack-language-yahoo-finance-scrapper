<?hh
use Yahoo\CSVWriter;
use Yahoo\TableRowExtractorIterator;

require_once("loader/SplClassLoader.php");
require_once("utility.hh");

$spl_loader = new SplClassLoader('Yahoo', 'src/');

$spl_loader->register();

$registry = new Registry(); // Work around to get class autoloaed.

  if ($argc == 2) {

    $argv[2] = 0; 
    $argc = 3;
  }

  $error_msg = '';

  if (validate_user_input($argc, $argv, $error_msg) == false) {

       echo $error_msg;

       echo Registry::registry('help'); 
       return;
  }

  /* TODO: 
   Decide on the data structure to use for the main loop.

  $date_period = build_date_period($argv[1], (int) $argv[2]);

  $urls = build_url_vector(Registry::registry('url-path'), $date_period);
	  
  validate_url_existence($url);
   */
  
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
