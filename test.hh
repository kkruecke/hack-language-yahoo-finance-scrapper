<?hh

function  make_date_period(string $startDate, int $number_of_days) : \DatePeriod
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

$date = "12/02/2014";

$period = make_date_period($date, 2);

print_r($period);

