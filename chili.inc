<?php

require_once 'phpactiveresource/ActiveResource.php';

function config_get($key, $default = NULL) {
  static $properties = NULL;
  if (is_null($properties)) {
    // if configuration file (config.ini.php) exists use it (overwrite class properties/attribute values).
    $config_file_path = dirname(__FILE__) . '/' . 'config.ini';
    if (file_exists($config_file_path)) {
      $properties = parse_ini_file($config_file_path);
    }
  }
  if (isset($properties[$key])) {
    return $properties[$key];
  }

  return $default;
}

class Chili extends ActiveResource {

  var $site;
  var $user;
  var $password;
  var $request_format = 'xml';

  public function __construct($data = array()) {
    // Fetch configuration values.
    $this->site = config_get('site');
    $this->user = config_get('user');
    $this->password = config_get('password', 'ignored');
    parent::__construct($data);
  }

}

class Project extends Chili {

}

class TimeEntry extends Chili {

  public $element_name = 'time_entry';

}

function get_project_id($identifier) {
  static $projects = NULL;

  static $project_ids = array(
  'web-tnt-ch' => 75,
  'web-unicef-ch' => 49,
  );

  if (isset($project_ids[$identifier])) {
    return $project_ids[$identifier];
  }

  if (empty($projects)) {
    $project = new Project();
    $projects = $project->find('all', array('limit' => 100));
  }

  foreach ($projects as $project) {
    if ($project->identifier == $identifier) {
      $project_ids[$identifier] = $project->id;
    }
  }

  if (empty($project_ids[$identifier])) {
    $project_ids[$identifier] = FALSE;
  }

  return $project_ids[$identifier];
}

function save_time_entry($data) {
  $data['activity_id'] = config_get('activity_id');
  $data['user_id'] = config_get('user_id');

  // Everything is expected to be a string.
  foreach ($data as &$value) {
    $value = (string) $value;
  }

  $time_entry = new TimeEntry($data);
  if ($time_entry->save()) {
    print "OK\n";
  }
  else {
    print "ERROR\n";
    var_dump($time_entry->request_body);

    var_dump($time_entry->errno);
    var_dump($time_entry->error);
  }
}
