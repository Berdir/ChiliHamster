<?php

require_once 'hamster.inc';
require_once 'chili.inc';

$from = isset($argv[1]) ? new DateTime($argv[1]) : new DateTime();
$until = isset($argv[2]) ? new DateTime($argv[2]) : $from;

$activities = get_chili_activities($from, $until);

foreach ($activities as $activity) {
  $project_id = get_project_id($activity->category);
  print "Project ID: " . $project_id . "\n";

  if (!($project_id > 0)) {
    print "ERROR: Failed to load project id, ignoring activity";
    continue;
  }

  $start_time = new DateTime($activity->start_time);
  $end_time = new DateTime($activity->end_time);
  print "Time: " . $start_time->format('d.m.Y - H:i') . ' - ' . $end_time->format('d.m.Y - H:i') . "\n";

  $seconds = (float)$end_time->getTimestamp() - $start_time->getTimestamp();
  $hours = round($seconds / 60 / 60, 2);
  print "Durance: " .  $hours . "h\n";

  $issue_id = 0;
  if (preg_match('/#([0-9]+)/', $activity->activity, $matches)) {
    $issue_id = $matches[1];
    print "Issue: #" . $issue_id . "\n";

    $activity->activity = trim(preg_replace('/#([0-9]+)/', '', $activity->activity));
  }

  print "Description: " . $activity->activity . "\n";

  $return = readline('Save this activity as a time entry? (y/n)');
  if ($return == 'y') {
    $data = array(
      'project_id' => $project_id,
      'hours' => $hours,
      'comments' => $activity->activity,
      'spent_on' => $start_time->format('Y-m-d'),
    );
    if (!empty($issue_id)) {
      $data['issue_id'] = $issue_id;
    }
    save_time_entry($data);
  }
  print "\n";
}
