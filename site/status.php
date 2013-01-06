<?php

$file = fopen("light_status","r");
$light = fgets($file);
fclose($file);

$file = fopen("fan_status","r");
$fan = fgets($file);
fclose($file);

$data = array("light" => $light, "fan" => $fan);
echo json_encode($data);


?>
