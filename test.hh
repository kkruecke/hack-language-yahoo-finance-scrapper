<?hh
/*
 * Used be CSVWriter to return customize output
 * Should this be an Abstract class or an Interface since we don't need an implementation.
 * Interface seems the best choice.
 */ 
function format(Vector<string> &$row) : void 
{
     $column3_text = $row[3];	   

     $column3_text ="junk"; 
       
     $row[3] = $column3_text;
}

$v = Vector {"firstttt", "secondddd", "thirdddd", "fourthhhhhhhh"};

foreach($v as $value) {

	echo "$value\n";
}
echo "\n";
format($v);

echo "\n";

foreach($v as $value) {

	echo "$value\n";
}



