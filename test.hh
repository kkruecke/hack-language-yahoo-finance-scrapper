<?hh
$a = array("a", 'b', 'c', 'd', 'e', 'f');

$insert = 'x';

echo "\nPrinting \$a\n";

echo "\n---------------\n";

print_r($a);

echo "\n---------------\n";

$result = array_splice($a, 2, 0, $insert);

echo "\n" . 'Printing return value of: array_splice($a, 2, 0, $insert)' . "\n";

if (empty($result)) {

   echo "\$result is empty and is:\n\n"; 

} else {

   echo "\n" . "\$result is not empty and is\n\n"; 
}

print_r($result);

echo "\n\n-----------------------\n\nPrinting \$a:\n";

print_r($a);


echo "\n";
