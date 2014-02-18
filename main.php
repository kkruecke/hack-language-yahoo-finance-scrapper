<?php
use Yahoo\CSVWriter;
use Yahoo\NandishTableRowExtractor;

include "utility.php";
include "loader/SplClassLoader.php";


$spl_loader = new SplClassLoader('Yahoo', 'src/');

$spl_loader->register();

define('YAHOO_BIZ_URL', "http://biz.yahoo.com/research/earncal/");

define('HELP', "How to use: Enter a date in mm/dd/YYYYY format follow by number between 0 and 40.\n");

  $error_msg = "";
  
  if ($argc == 2) {
    $argv[2] = 0; 
    $argc = 3;
  }

  if (validate_input($argc, $argv, $error_msg) == false) {
       echo $error_msg;
       echo HELP . "\n"; 
       return;
  }
  
  $number_of_days = (int) $argv[2];
  $number_of_days_plus1 = $number_of_days + 1;
    
  // Add additional dates initaldate and then append to $requested_dates[]
  $start_date = DateTime::createFromFormat('m/d/Y', $argv[1]); 
  
  $one_day_interval = new DateInterval('P1D');
  
  // Determine the end date
  $end_date = DateTime::createFromFormat('m/d/Y', $argv[1]); 
  
  $end_date->add(new DateInterval("P{$number_of_days_plus1}D")); 
    
  $csv_writer = new CSVWriter($start_date, $argv[2]);
  
  $date_period = new DatePeriod($start_date, $one_day_interval, $end_date);
  
  // Start main loop
  foreach ($date_period as $date) {
      
      // Build yyyymmdd.html name
      $url = YAHOO_BIZ_URL . $date->format('Ymd') . ".html";
      
      if (url_exists($url) == false) {
          
           echo 'The url for ' . $date->format("m-d-Y") . ", $url , " . " does not exists\n";               
           continue;    
      }
      
      try {
     
         $extractor = new NandishTableRowExtractor($url, $date, '/html/body/table[3]/tr/td[1]/table[1]');
  
          foreach($extractor as $stock_data) {
  
               $csv_writer->writeLine($stock_data); 
          }
  
      } catch(Exception $e) {
          
          echo $e->getMessage() . "\n";
          return;
      }
  }
  
  echo  $csv_writer->getFileName() . " has been created\n";
  
  return;
