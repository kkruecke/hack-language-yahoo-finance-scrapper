<?hh
use Yahoo\CSVWriter,
    Yahoo\CSVYahooFormatter,
    Yahoo\YahooTable, 
    Yahoo\Registry;

require_once("loader/SplClassLoader.php");
require_once("utility.hh");

$spl_loader = new SplClassLoader('Yahoo', 'src');

$spl_loader->setFileExtension('.hh');
$spl_loader->register();

  if ($argc == 2) {

    $argv[2] = 0; 
    $argc = 3;
  }

  $error_msg = '';

  if (validate_user_input($argc, $argv, $error_msg) == false) {

       echo $error_msg . "\n";

       echo Registry::registry('help'); 
       return;
  }

  $date_period = build_date_period($argv[1], (int) $argv[2]);

  /*
   * CSVYahooFormatter determines the format of the output, the rows of the CSV file.
   */   
  $csv_writer = new CSVWriter(new CSVYahooFormatter(),  $start_date, $argv[2]);

  // Start main loop
  foreach ($date_period as $date_time) {
      
      $url = make_url($date_time); // Build yyyymmdd.html name

      $pretty_date = $date_time->format("m-d-Y"); // User-friendly date format
      
      if (validate_url_existence($url)) {
          
           echo 'Skipping date ' . $pretty_date . " there is no webpage $url ...\n";               
           continue;    
      }
      
      try {
          
	  $table = new YahooTable($url, Registry::registry('xpath-query'));

	  $max_rows = $table->rowCount(); // first row is 0, last is $max_rows - 1
	     
	  // We skip the first two rows, the table description and column headers, and the last row which has no financial data
	  $limitIter = new \LimitIterator($table->getIterator(), 2, $max_rows - 2); // TODO: Check whether it is "- 2" or "- 1"?

	  /*
	   * The filter iterator should include all the filters of the original code:
	   *   1. no column may be blank
	   *   2. only US Stocks are selected
	   *   3. ? any other filters
	   */   
	  $filterIter = new \CustomStockFilterIterator($limitIter);
          /*
	   * Alternately, a custom callback filter iterator could be used: 
	   *   $callbackFilterIter = new \CallbackFilterIterator($rowExtractorIter, 'isUSStock_callback');
	   */ 
     
          foreach($filterIter as $us_stock_row) {

               $csv_writer->writeLine($us_stock_data); 
	  }

	  echo "Date $pretty_date processed\n";

  
      } catch(Exception $e) {
          
          echo $e->getMessage() . "\n";
          return;
      }
  }

  $line_count = $csv_writer->getLineCount();
  
  echo  $csv_writer->getFileName() . " has been created. It contains $line_count US stocks entries.\n";
    
  return;
