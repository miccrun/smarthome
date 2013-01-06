<?php

$db = mysql_connect("127.0.0.1","user","passwd");
if (!$db)
{
    die('Could not connect: ' . mysql_error());
}
mysql_select_db("smarthome", $db);

$result = mysql_query("SELECT * FROM monitor WHERE valid=1 ORDER BY update_time DESC LIMIT 1;");
$data = mysql_fetch_array($result,MYSQL_ASSOC);
$json = json_encode($data);
echo $json;


?>
