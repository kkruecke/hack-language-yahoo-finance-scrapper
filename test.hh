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



       echo Registry::registry('help'); 
       return;
