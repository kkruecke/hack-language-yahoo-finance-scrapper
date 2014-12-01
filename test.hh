<?hh
use Yahoo\CSVWriter,
    Yahoo\CSVYahooFormatter,
    Yahoo\YahooTable, 
    Yahoo\X;

require_once("utility.hh");

boot_strap();

  if ($argc == 2) {

    $argv[2] = 0; 
    $argc = 3;
  }

  $error_msg = '';

  if (validate_user_input($argc, $argv, $error_msg) == false) {

       echo $error_msg . "\n";

       echo X::registry('help'); 
       return;
  }

  $str =  X::registry('xpath-query');
  var_dump(X::$registry); 

  $debug = 10;

  echo $str;

