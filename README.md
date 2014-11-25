This is a data scrapper for Yahoo financial data. It is configuration-driven by an .ini file that specifies the

* The base url of the Yahoo financial data
* the xpath query used to return the HTML table with the data

The date entered, passed on the command line, is used to construct the specific url path that is then appended to the base url. 

The **YahooTable** is the model the html table. Its constructor includes parameters to specify the start and end column which its external iterator **YaooTableIterator**
will return.  To limit the range of rows of the iteration, pass **YahooTableIterator** to PHP's **LimitIterator**.

	  // To skip the first two rows, the table description and column headers, as well as the last row, use a LimitIterator.
	  $limitIter = new \LimitIterator($table->getIterator(), 2, $max_rows - 1); // TODO: ...or is it "- 2"?

 To further filter the rows returned, extend **FilterIterator** and pass it either **YahooTableIterator** or as explained above **LimitIterator**.
