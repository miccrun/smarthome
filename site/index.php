<?php
date_default_timezone_set('America/Chicago');
$result = file_get_contents("https://home.micbase.com:11443/get.php");
$data = json_decode($result,true);
$result = file_get_contents("http://localhost/status.php");
$status = json_decode($result,true);
?>
<!DOCTYPE html>
<html lang="en">
    <head>
        <title>Smart Home</title>
        <meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
        <meta name="viewport" content="width=device-width, initial-scale=1.0">
        <meta name="description" content="">
        <meta name="author" content="">
        <link href="bootstrap.min.css" rel="stylesheet">
        <script src="jquery-1.8.3.min.js"></script>
        <script src="bootstrap-button.js"></script>
    </head>

    <body>
        <div class="container-fluid">
            <div class="row-fluid">
                <div class="span12">
                    <legend>Control</legend>
                    <button id="lightBtn" type="button" class="btn btn-large btn-block <?php if ($status['light']) echo 'btn-primary'; ?>" data-toggle="button">Lights</button>
                    </br>
                    <button id="fanBtn" type="button" class="btn btn-large btn-block <?php if ($status['fan']) echo 'btn-primary'; ?>" data-toggle="button">Fan</button>
                    </br>

<script type="text/javascript">
$('#lightBtn').click(function(){
    $.getJSON('status.php', function(data) {
        if (parseInt(data.light))
            $.get('control.php?cmd=102');
        else
            $.get('control.php?cmd=101');

        $('#lightBtn').toggleClass('btn-primary');
    });
});

$('#fanBtn').click(function(){
    $.getJSON('status.php', function(data) {
        if (parseInt(data.fan))
            $.get('control.php?cmd=104');
        else
            $.get('control.php?cmd=103');

        $('#fanBtn').toggleClass('btn-primary');
    });
});
</script>

                    <legend>Monitoring</legend>
                    <table class="table">
                        <thead>
                            <tr>
                                <th>Temperature(℃)</th>
                                <th>Humidity</th>
                                <th>Heat Index(℃)</th>
                            </tr>
                        </thead>
                        <tbody>
                            <tr>
                                <td><?php echo $data['temperature']; ?></td>
                                <td><?php echo $data['humidity']; ?></td>
                                <td><?php if($data['heat_index'] != 0) echo $data['heat_index']; else echo 'N/A';?></td>
                            </tr>
                            <tr>
                            <td colspan="3">Last Updated: <?php echo date("Y-m-d H:i:s",$data['update_time']);?></td>
                            </tr>
                        </tbody>
                    </table>
                </div>
            </div>
        </div>
    </body>
</html>
