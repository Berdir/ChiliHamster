<?php

$from = isset($argv[0]) ? $argv[0] : new DateTime();

$db = new PDO('sqlite:/home/berdir/.local/share/hamster-applet/hamster.db');
$result = $db->query('SELECT a.name as activity, c.name as category, end_time, start_time
  FROM activities a
  INNER JOIN categories c ON c.id = a.category_id
  INNER JOIN facts f on f.activity_id = a.id
  WHERE c.id > 3');
while ($row = $result->fetchObject()) {
  print $row->activity . ' - ' . $row->category . ' | ' . $row->start_time . ' - ' . $row->end_time . "\n";


}
