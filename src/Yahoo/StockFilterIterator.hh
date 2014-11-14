<?hh
namespace Yahoo;

class StockFilterIterator extends \FilterIterator {

   public function __construct(\Iterator $iter)
   {
	parent::__construct($iter);
   }

   public function accept()
   {
      $row_data = $this->getInnerIterator()->current();

      // Test if the cell data is for a US stock
      $stock_length = strlen($row_data[1]);

      return (($stock_length > 1 && $stock_length < 5) && ( strpos($row_data[1], '.') === FALSE)) ? true : false;
   }

}
