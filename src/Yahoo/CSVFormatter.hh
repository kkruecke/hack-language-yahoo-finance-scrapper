<?hh
namespace Yahoo;

interface CSVFormatter {
  public function format(Vector<string> $row, \DateTime $date) : string;
}
