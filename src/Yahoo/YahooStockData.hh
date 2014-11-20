<?hh
namespace Yahoo;

class YahooStockData extends \ArrayObject implements CSVFormatter {

    private $csvformatter;

    public function __construct(CSVFormatter $f)
    {
	    $this->csvformatter = $f;
    } 

    public function format() : string
    {
	$str = "";
	return $str;
    }
}
