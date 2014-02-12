<?php

/*
 * DOMDocument: http://www.codekites.com/php-tutorial-parsing-html-with-domdocument/ 
 *
 * XPath: http://stackoverflow.com/questions/12569341/php-domdocument-parse-nested-tables

Design
==================================

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

if ( isset($argc) && $argc != 3 ) {
    
  echo HELP . "\n"; 
  exit;
}

// An array of associative arrays with keys of 'year', 'month' and 'day'.
$requested_dates = array();

// validate the date
$mm_dd_yy_regex = "@^(\d{1,2})/(\d{1,2})/(\d{4})$@";

$matches = array();

$count = preg_match($mm_dd_yy_regex, $argv[1], $matches);
    
if ($count === FALSE) {
    
    echo "The date " . $argv[$i] . " is not in a valid format.\n" . HELP . "\n";
    exit;
}
    
$bRc = checkdate ($matches[1], $matches[2], $matches[3]);

if ($bRc === FALSE) {
    
    echo $argv[$i] . "is not a valid date\n";
    exit;
}

// validate that second parameter is between 1 and 40 
if ( (preg_match("/^[0-9][0-9]?$/", $argv[2]) == 0) || ( ((int) $argv[2]) > 40) ) {
  
  echo $argv[2] . " must be a number between 0 and 40\n";
  return;

} 

$number_of_days = (int) $argv[2]; 
return;

$requested_dates[] = array('month' => $matches[1], 'day' => $matches[2], 'year' => $matches[3]);

// Add to date and then append to $requested_dates[]
$prior_date = DateTime::createFromFormat('d/m/Y', $argv[1]); 

for ($i = 1; $i < $number_of_days; $i++) {

    $new_date =  $prior_date->add('P1D');

    // Add it to requested_dates[]
    //$new_date_string = $new_date->format(???);

    $requested_dates[] = array('month' => $matches[1], 'day' => $matches[2], 'year' => $matches[3]);
}
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
// main loop
foreach ($requested_dates as $date)  {

    $row_data = array();
    
    try {
        
        get_table_data($date, $row_data);
        
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
       
       $csv_str .= "\n"; // replace terminating ',' with newline.
                    
       fputs($fp, $csv_str);
    }
}

function get_table_data($date, &$row_data)
{
// Build yyyymmdd.html name
 $html_file_name = sprintf("%d%02d%02d.html", $date['year'], $date['month'], $date['day']);
 
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

// returns nodelist -- must first get the first and only node, the table.
$xpathNodeList = $xpath->query('/html/body/table[3]/tr/td[1]/table[1]');

if ($xpathNodeList->length != 1) {
    
    echo "XPath Query Failed. Page format has evidently changed. Cannot proceed\n";
    exit;
} 

$date_string = $date['month'] . '/' . $date['day'] . '/' . $date['year'];

// get timestamp from date string.
$time = strtotime($date_string);

// date, in form DD-MON, as array[2], with no leading zeroes, 'j' means no leading zeroes
// date_column will be used at the end of the row loop
$date_column = date('j-M', $time);

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

    // Skip first row. First row is "Earnings for ...".  Second row is column headers. 
    $cell_data = array();
    $row_count--; // ignore last row
    
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

  //--$cell_count = $tdNodeList->length;
      
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
