<?hh
/*
 * Input: $argc, $argv, reference to $error_msg string to return
 * Returns: boolean: true if input good, false otherwise.
 */

function validate_user_input(int $arg_number, array $params, string &$error_msg) : bool
{

   if ( isset($arg_number) && $arg_number != 3 ) {
      
     $error_msg = "Two input paramters are required\n";
     return false;
   }
  
   // validate the date
   $mm_dd_yy_regex = "@^(\d{1,2})/(\d{1,2})/(\d{4})$@";
      
   $matches = array();
      
   $count = preg_match($mm_dd_yy_regex, $params[1], $matches);
          
   if ($count === FALSE || $count != 1) {
          
       $error_msg =  "The date " . $params[1] . " is not in a valid format.\n" ;
       return false;
   }
          
   $bRc = checkdate ($matches[1], $matches[2], $matches[3]);
      
   if ($bRc === FALSE) {
          
       $error_msg = $params[1] . " is not a valid date\n";
       return false;
   }
      
   // validate that second parameter is between 1 and 40 
   if ( (preg_match("/^[0-9][0-9]?$/", $params[2]) == 0) || ( ((int) $params[2]) > 40) ) {
        
        $error_msg = $params[2] . " is not a number between 0 and 40\n";
        return false;
    } 
    
    return true;
}
    
function url_exists($url) : bool
{
    $file_headers = get_headers($url);
    return ($file_headers[0] == 'HTTP/1.1 404 Not Found') ? false : true;
}

function  build_date_period(string $startDate, int $number_of_days) : \DatePeriod
{    
  // Add additional dates initaldate and then append to $requested_dates[]
  $start_date = \DateTime::createFromFormat('m/d/Y', $startDate); 
  
  $one_day_interval = new \DateInterval('P1D');
  
  // Determine the end date
  $end_date = \DateTime::createFromFormat('m/d/Y', $startDate); 
  
  $end_date->add(new \DateInterval("P" . ($number_of_days + 1 ) ."D")); 
  
  $date_period = new \DatePeriod($start_date, $one_day_interval, $end_date);
 
  return $date_period;
}

/*
 * TODO: Decide what this should do
 * Maybe we need a build_urls_array/Vector()
 */ 
function  validate_url_existence(\DateTime $date_time, string &$error_mg) : bool
{
   $bRc = true;	

    // 1. create htlm file name.
    $url = make_url($date_time);

    // 2. validate its existence.
   if (!url_exists($url)) {

        $date =  \DateTime::createFromFormat('m/d/Y', $date_time);

	$error_msg = "For date of $date, there is no corresponding .html named $url\n";

	$bRc = false;
    }     

   return $bRc;
}
// Prospective callback
function make_url(\DateTime $date_time)
{
    // Build yyyymmdd.html name
   return  Registry::registry('url-path')  . $date_time->format('Ymd') . ".html";
}

// TODO: See old php code for building the full .html path
function build_url_vector(string $url_path, \DatePeriod $date_period ) : Vector<Pair<\DateTime, string> >
{
  $v = Vector {};

  foreach($date_period as $date) {

     // Build yyyymmdd.html name
     $url = $url_path . $date->format('Ymd') . ".html";
     $v[] =  Pair ( $date, $url );

  }	  

  return $v;
}
