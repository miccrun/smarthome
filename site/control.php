<?php

$cmd = $_GET['cmd'];

switch($cmd)
{
case "101":
    system("sudo /home/pi/smarthome/rf24bb 101 >> /home/pi/control_log");
    $file = fopen("light_status","w");
    fwrite($file,"1");
    fclose($file);
    break;
case "102":
    system("sudo /home/pi/smarthome/rf24bb 102 >> /home/pi/control_log");
    $file = fopen("light_status","w");
    fwrite($file,"0");
    fclose($file);
    break;
case "103":
    system("sudo /home/pi/smarthome/rf24bb 103 >> /home/pi/control_log");
    $file = fopen("fan_status","w");
    fwrite($file,"1");
    fclose($file);
    break;
case "104":
    system("sudo /home/pi/smarthome/rf24bb 104 >> /home/pi/control_log");
    $file = fopen("fan_status","w");
    fwrite($file,"0");
    fclose($file);
    break;
}

echo "OK";
