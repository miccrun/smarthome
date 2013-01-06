<?php

$db = mysql_connect("127.0.0.1","user","passwd`");
if (!$db)
{
    die('Could not connect: ' . mysql_error());
}
mysql_select_db("smarthome", $db);

$temperature = intval($_GET["tmp"]);
$humidity = intval($_GET["hum"]);
$valid = intval($_GET["valid"]);
$heat = floatval($_GET["heat"]);
$time = time();

if ($valid == 1 && ($temperature < 0 || $temperature > 50 || $humidity < 20 || $humidity > 90 || $heat < 0 || $heat > 50))
    echo "ERR";
else
{
    mysql_query("INSERT monitor SET temperature=$temperature,humidity=$humidity,valid=$valid,update_time=$time,heat_index=$heat");
    echo "OK";
}


?>
