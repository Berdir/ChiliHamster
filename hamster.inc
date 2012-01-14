<?php

const HAMSTER_DB = 'sqlite:/home/berdir/.local/share/hamster-applet/hamster.db';

function get_chili_activities($from, $until) {
  $db = new PDO(HAMSTER_DB);
  $result = $db->query("SELECT a.name as activity, c.name as category, end_time, start_time
  FROM activities a
  INNER JOIN categories c ON c.id = a.category_id
  INNER JOIN facts f on f.activity_id = a.id
  WHERE c.id > 3 AND start_time > '" . $from->format('Y-m-d'). " 00:00:00'
  AND end_time < '" . $until->format('Y-m-d'). " 23:59:59'
  ORDER BY end_time ASC");

  if (!$result) {
    print $db->errorCode( ). ' - ' . print_r($db->errorInfo()) . "\n";
    exit(1);
  }
  return $result->fetchAll(PDO::FETCH_OBJ);
}