<?php
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
          
   if ($count === FALSE) {
          
       $error_msg =  "The date " . $params[$i] . " is not in a valid format.\n" ;
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

  if (validate_input($argc, $argv, $error_msg) == false) {
       echo $error_msg;
       echo HELP . "\n"; 
       return;
  }
  
  //--return;

$number_of_days = (int) $argv[2]; 

/* Old code
for($i = 1; $i < $argc; $i++) {
    
    $count = preg_match($mm_dd_yy_regex, $argv[1], $matches);
        
    if ($count === FALSE) {
        
        echo "The date " . $argv[$i] . " is not in a valid format.\n" . HELP . "\n";
        exit;
    }
    
    $bRc = checkdate ( $matches[1], $matches[2], $matches[3]);
    
    if ($bRc === FALSE) {
        
        echo $argv[$i] . "is not a valid date\n";
        exit;
    }
    
    $requested_dates[] = array('month' => $matches[1], 'day' => $matches[2], 'year' => $matches[3]);
}
*/
// Start main loop

// An array of associative arrays with keys of 'year', 'month' and 'day'.
$requested_dates = array();

// Add additional dates initaldate and then append to $requested_dates[]
$start_date = DateTime::createFromFormat('m/d/Y', $argv[1]); 

$one_day_interval = new DateInterval('P1D');

$end_date = $start_date->add(new DateInterval("P{$number_of_days}D")); // Q: $number_of_days + 1 better?

for ($date = DateTime::createFromFormat('m/d/Y', $argv[1]); $date < $end_date; $date->add($one_day_interval) ) {
    
    try {

        $extractor = new NandishTableRowExtractor($base_url, $date, '/html/body/table[3]/tr/td[1]/table[1]');

        $row_data = array();

        $extractor->getRowData($row_data);

      // TODO: write to CSV files. 
        $csv_writer->write($row_data); 
        
    } catch(Exception $e) {
        
        echo $e->getMessage() . "\n";
    }

    write_csv_file($row_data, $date);

    $prior_date = $new_date;
}

return;

function write_csv_file($row_data, $date)
{
    // Open files
    
    $csv_file_name = sprintf("%d%02d%02d", $date['month'], $date['day'],  $date['year']);
    
    $csv_file_name .= '.csv';
    
    $fp = fopen($csv_file_name, "w");
      
    $count = count($row_data);
    
    for($i = 0; $i < $count; $i++) {
               
       $csv_str = implode(',', $row_data[$i]);
       
       $csv_str .= "\n"; // replace terminating ',' with newline.
                    
       fputs($fp, $csv_str);
    }
}

// Change to use DateTime
function get_table_data(DateTime $date, &$row_data)
{
// Build yyyymmdd.html name
$html_file_name = sprintf("%d%02d%02d.html", $date->format('Y'), $date->format('m'), $date->format('d') );

 $url = YAHOO_BIZ_URL . $html_file_name;
        
 $page = file_get_contents($url);
 
//Debug:- file_put_contents("./$html_file_name", $page); // Debug only

 // a new dom object
$dom = new DOMDocument;
 
// load the html into the object
$dom->strictErrorChecking = false; // default is true.

$dom->loadHTML($page);
 
// discard redundant white space
$dom->preserveWhiteSpace = false;

$xpath = new DOMXPath($dom);

// Get rows starting with third row
$query = '/html/body/table[3]/tr/td[1]/table[1]';

//--$query = '/html/body/table[3]/tr/td[1]/table[1]/tbody/tr[position()>1]';

$xpathNodeList = $xpath->query($query);

if ($xpathNodeList->length != 1) {
    
    echo "XPath Query Failed. Page format has evidently changed. Cannot proceed\n";
    exit;
} 


/* Commented out for now
$date_string = $date['month'] . '/' . $date['day'] . '/' . $date['year'];

// get timestamp from date string.
$time = strtotime($date_string);

// date, in form DD-MON, as array[2], with no leading zeroes, 'j' means no leading zeroes
// date_column will be used at the end of the row loop
$date_column = date('j-M', $time);
*/

$tableNodeElement = $xpathNodeList->item(0);

/* 
 * We need to as the $tableNodeElement->length to get the number of rows. We will subtract the first two rows --
 * the "Earnings Announcement ..." and the columns headers, and we ignore the last row.
 * Query Paths for the rows:
 * 1.  /html/body/table[3]/tr/td[1]/table[1]/tr[1] is "Earnings Announcements for Wednesday, May 15"
 * 2.  /html/body/table[3]/tr/td[1]/table[1]/tr[2] is column headers
 */

 if ( $tableNodeElement->hasChildNodes())  {

    // loop through rows
    $childNodesList = $tableNodeElement->childNodes;
    $row_count = $childNodesList->length;

    $cell_data = array();

    $row_count--; // ignore last row

    // Skip first row. First row is "Earnings for ...".  Second row is column headers. 
    for($i = 2; $i < $row_count; $i++)  { // skip last row. it is a colspan.
        
       $rowNode =  $childNodesList->item($i);
                            
        if (false == get_cells_from_row($rowNode, $cell_data)) {
            
            continue; // if row did not have five columns of data
        }          
        
   	// Test if the cell data is for a US stock
        $stock_length = strlen($cell_data[1]);
        
        if (($stock_length > 1 && $stock_length < 5) && ( strpos($cell_data[1], '.') === FALSE)) {
          
             // Change html entities back into ASCII (or Unicode) characters.             
             array_walk($cell_data, function(&$item, $key) { $item = html_entity_decode($item); });
           
        } else {
             // skip the row; it if it is not a US Stock
             continue;
        }

	array_splice($cell_data, 2, 0, $date_column);   
        $cell_data[] = "Add"; // required hardcode value
	$row_data[] = $cell_data;
     }
 }    
} // end function

function get_cells_from_row($rowNode, &$cell_data)
{
  $cell_data = array();

  $tdNodeList = $rowNode->getElementsByTagName('td');

  for($i = 0; $i < 4; $i++) {

     $td = $tdNodeList->item($i);

     $cell_text = $td->nodeValue;
          
     $rc = preg_match ('/^\s*$/', $cell_text); // Handles empty cells and cells with only whitespace, like last row.
             
     if ($rc == 1) {
         
         return false;
     }
         
     if ($i == 3) {

         if (is_numeric($cell_text[0])) { // a time was specified

              $cell_text =  'D';

         } else if (FALSE !== strpos($cell_text, "After")) { // "After market close"

               $cell_text =  'A';

         } else if (FALSE !== strpos($cell_text, "Before")) { // "Before market close"

              $cell_text =  'B';

         } else if (FALSE !== strpos($cell_text, "Time")) { // "Time not supplied"

  	    $cell_text =  'U';

         } else { // none of above cases

              $cell_text =  'U';
         }  
     }

     $cell_data[] = $cell_text; 
  
   }
   
   return true;
}
