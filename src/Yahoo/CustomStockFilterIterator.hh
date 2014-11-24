<?hh
class CustomStockFilterIterator extends \FilterIterator {

   public function accept() 
   {
        // Only accept strings with a length of 10 and greater
        $row = parent::current();

	/* TODO
	 * Implement Criteria: 
	 *  All columns non-empty
	 *  US Stocks only
	 */  

    }
}
