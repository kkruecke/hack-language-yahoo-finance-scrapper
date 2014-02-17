<?php
use Yahoo\CSVWriter;
use Yahoo\NandishTableRowExtractor;

include "loader/SplClassLoader.php";

$spl_loader = new SplClassLoader('Yahoo', 'src/');

$spl_loader->register();

define('YAHOO_BIZ_URL', "http://biz.yahoo.com/research/earncal/");

define('HELP', "How to use: Enter a date in mm/dd/YYYYY format follow by number between 0 and 40.\n");
/*
 * Input: $argc, $argv, reference to $error_msg string to return
 * Returns: boolean: true if input good, false otherwise.
 */
function validate_input($arg_number, $params, &$error_msg)
{
    
   if ( isset($arg_number) && $arg_number != 3 ) {
      
     $error_msg = "Two input paramters are required\n";
     return;
   }
  
   // validate the date
   $mm_dd_yy_regex = "@^(\d{1,2})/(\d{1,2})/(\d{4})$@";
      
   $matches = array();
      
   $count = preg_match($mm_dd_yy_regex, $params[1], $matches);
          
   if ($count === FALSE || $count != 1) {
          
       $error_msg =  "The date " . $params[1] . " is not in a valid format.\n" ;
       return false;
   }
          
   $bRc = checkdate ($matches[1], $matches[2], $matches[3]);
      
   if ($bRc === FALSE) {
          
       $error_msg = $params[$i] . "is not a valid date\n";
       return false;
   }
      
   // validate that second parameter is between 1 and 40 
   if ( (preg_match("/^[0-9][0-9]?$/", $params[2]) == 0) || ( ((int) $params[2]) > 40) ) {
        
        $error_msg = $params[2] . " is not a number between 0 and 40\n";
        return false;
    } 
    
    return true;
}
  
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
    
  // Start main loop
  
  // Add additional dates initaldate and then append to $requested_dates[]
  $start_date = DateTime::createFromFormat('m/d/Y', $argv[1]); 
  
  $one_day_interval = new DateInterval('P1D');
  
  // Determine the end date
  $end_date = DateTime::createFromFormat('m/d/Y', $argv[1]); 
  
  $end_date->add(new DateInterval("P{$number_of_days_plus1}D")); 
    
  $csv_writer = new CSVWriter($start_date, $argv[2]);
  
  $date_period = new DatePeriod($start_date, $one_day_interval, $end_date);
  
  foreach ($date_period as $date) {
      
      try {
  
          $extractor = new NandishTableRowExtractor(YAHOO_BIZ_URL, $date, '/html/body/table[3]/tr/td[1]/table[1]');
  
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
