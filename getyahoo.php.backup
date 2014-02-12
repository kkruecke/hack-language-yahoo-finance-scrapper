<?php
/*
 The CSV file needs six columns 

Col 1 - Company 
Col 2 - Symbol 
Col 3 - Date -- the date entered as the argument, in the form xx-yyy, where xx is the day as a digit, and yyy is the 3-letter abbrev. of the month.
Col 4 - Time -- as a special one letter code, defined below:

	if ( Col 4 = "After Market Close")
		 Col4 = A
	else if ( Col 4 = "Before Market Open")
		 Col4 = B
	else if ( Col 4 = "Time Not Supplied")
		 Col4 = U
	else if ( col 4 has a time supplied in form of ????)
		 Col4 = D
	else
		 Col4 = U

In order to get the entire text of the column without html, I will need to use DOMDocument methods.
 
Col 5 - Eps 
Col 6 - PrevYr  -- a column with the string "Add"

Sample:      
Company:Acorn International Inc 
Symbol: ATV
Date:19-Mar
Time:B
Eps: N/A
PrevYr:Add

------ Logic to get one letter code for Time ---
Col 4 -
If ( Col 4 = After Market Close)
 Col4 = A
If ( Col 4 = Before Market Open)
 Col4 = B
If ( Col 4 = Time Not Supplied)
 Col4 = U
If ( col 4 is supplied)
 Col4 = D
else
 Col4 = U
 
 

 */

define('YAHOO_BIZ_URL', "http://biz.yahoo.com/research/earncal/");

define('HELP', "Enter a date or several dates in month/day/year format, for example: 5/31/2013");


if ( isset($argc) && $argc == 1 ) {
    
  echo HELP . "\n"; 
  exit;
}

// An array of associative arrays with keys of 'year', 'month' and 'day'.
$requested_dates = array();

$mm_dd_yy_regex = "@^(\d{1,2})/(\d{1,2})/(\d{4})$@";

$matches = array();

for($i = 1; $i < $argc; $i++) {
    
    $count = preg_match($mm_dd_yy_regex, $argv[$i], $matches);
        
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

// main loop
foreach ($requested_dates as $date)  {

    $row_data = array();
    
    try {
        
        get_data($date, $row_data);
        
    } catch(Exception $e) {
        
        echo $e->getMessage() . "\n";
    }

    write_csv_file($row_data, $date);
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
       
       $csv_str[strlen($csv_str) - 1] = "\n"; // replace terminating ',' with newline.
                    
       fputs($fp, $csv_str);
    }
}

function get_data($date, &$row_data)
{
 // Build yyyymmdd.html name
 $html_file_name = sprintf("%d%02d%02d.html", $date['year'], $date['month'], $date['day']);
 
 $url = YAHOO_BIZ_URL . $html_file_name;
        
 $page = file_get_contents($url);

 // Replace newlines with ' '.
 $page_nonewlines = str_replace("\n", ' ', $page, $count);
   
 $date_string = $date['month'] . '/' . $date['day'] . '/' . $date['year'];

 // get timestamp from date string.
 $time = strtotime($date_string);

/*
 * Date formats strings are at: http://www.php.net/manual/en/datetime.formats.date.php
*/    

// Build regex string to search table header.
$day_month_digit = date('l', $time) . ',\s+' . date('F', $time) . '\s+' . date('j', $time);

$regex = "/Earnings\s+Announcements\s+for\s+" . $day_month_digit . "/"; 

$matches = array();

// Capture the offset of the "Earnings Announcement for DAY-OF-WEEK, MONTH DAY-AS-DIGIT"
$results = preg_match($regex, $page_nonewlines, $matches, PREG_OFFSET_CAPTURE);

if ($results === FALSE) {
    
    throw new Exception("Earnings Announcements data not found at $url. Termintating\n");
}

// Get substring from  "Earnings Announcement for DAY-OF-WEEK, MONTH DAY-AS-DIGIT" to first </table> tag.
$offset_start = $matches[0][1];

$offset_end = strpos($page_nonewlines, "</table", $offset_start);

$substring =  substr($page_nonewlines, $offset_start, $offset_end - $offset_start);

// Get array of rows.
$rows = preg_split("/<\/tr>/U", $substring);

/*
 * Extract the cell data from the rows. The first row has the columns.
 * Skip last two rows. They are not data. The last row is a colspan=4 row. The last is the empty </tbody>
 */
 $column_headers = $cell_data  = get_cells_from_row($rows[1]); 

$total_rows = count($rows);
$last_index = $total_rows - 2; 

for ($i = 2; $i < $last_index; $i++) {
        
   $cell_data  = get_cells_from_row($rows[$i]); 
   
   // Test if the cell data is for a US stock
    $stock_length = strlen($cell_data[1]);
    
    if (($stock_length > 1 && $stock_length < 5) && ( strpos($cell_data[1], '.') === FALSE)) {
      
         // Change html entities back into ASCII (or Unicode) characters.             
         array_walk($cell_data, function(&$item, $key) { $item = html_entity_decode($item); });
              
       
    } else {
         // skip it if it is not a US Stock
         continue;
    }
    
    // insert date, in form DD-MON, as array[2], with no leading zeroes, 'j' means no leading zeroes
    $date_column = date('j-M', $time);

    array_splice($cell_data, 1, 0, $date_column);   

    $row_data[] = $cell_data;
 } // end for each $row

 return;
} // end get_data

function get_cells_from_row($row)
{
  /* A regex for preg_match, like below, is not needed, as we will use preg_match_all
   * 
   * <td[^>]*>                      -- matches <td ...> opening cell tag
   * (?:<\w+[^>]*>)*                -- matches but does not capture any non-td tags following <td> -- assumes row html is compliant           
   * ([^<>]*)                       -- captures inner text
   * (?:</\w+\s*>)*                 -- matches (but does not capture) and non </td> tags before the </td> tag, between zero and many times
   * </td\s*>                         -- matches clsoing </td> tag
    
  $row_regex = '@(?:<td[^>]*>(?:<\w+[^>]*>)*([^<>]*)(?:</\w+\s*>)*</td\s*>){4}@i';
  $bRc = preg_match($row_regex, $row, $matches);
  */
  $row_regex = '/<td[^>]*>(?:<\w+[^>]*>)*([^<]*)/i'; // This works well: maybe add PREG_MATCH_OFFSET, then search to 
    
  $matches = array();
  
  $bRc = preg_match_all($row_regex, $row, $matches);
  
  if ($bRc === FALSE) { // correct?
      
      throw new Exception("Fatal Error. preg_match_all() failed on table.\n");
  }
    
  return $matches[1];
  /* Alternate code using a regular expression
   * 
  $stock_regex = '/^(?:[a-zA-Z0-9]{1,4})$/i';
  $stock_match = array();

  $bRc = preg_match($stock_regex, $matches[1][1], $stock_match); // Stock Symbol is 2nd element
   
  // Skip if it is a non-US stock
  if ($bRc == 0) {
      
      continue;
  }
  array_walk($matches[1], function(&$item, $key) { $item = html_entity_decode($item); });
  $row_results = $matches[1];
  */
}
?>
