<?hh
use Yahoo\CSVWriter,
    Yahoo\CSVYahooFormatter,
    Yahoo\YahooTable, 
    Yahoo\Registry;

require_once("utility.hh");
/*
date_default_timezone_set("America/Chicago"); // Workaround for PHP5 

$spl_loader = new SplClassLoader('Yahoo', 'src');

$spl_loader->setFileExtension('.hh');
$spl_loader->register();
 */
boot_strap();

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

  $str =  Registry::registry('xpath-query');

  $debug = 10;

  echo $str;

