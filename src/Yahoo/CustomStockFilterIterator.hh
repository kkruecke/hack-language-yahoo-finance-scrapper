<?hh
class CustomStockFilterIterator extends \FilterIterator {

   public function accept() 
   {
        // Only accept strings with a length of 10 and greater
        $row = parent::current();

	/* 
	 * Filter criteria: Only accept rows with all non-empty columns
	 */  
	foreach($row as $columnn_num => $column_text) {
                /*
		 * All column must be non-empty
		 */  
		if (strlen($column_text) == 0) {
			return false;
		}
	}
        /*
	 * Criteria for US Stock below
	 */ 
        $stock_length = strlen($row[1]);

        return (($stock_length > 1 && $stock_length < 5) && ( strpos($row[1], '.') === FALSE)) ? true : false;

    }
}
