<!DOCTYPE html>
<html>
<head>
<title>Robot Kontrol</title>
<script src="jquery.min.js"> </script>
<link rel="stylesheet" type="text/css" href="styles.css">
</head>
<?php 
$servername = "localhost";
$username = "cllkocco_celo";
$password = "504504";
$dbname = "cllkocco_kontrol";
if(isset($_POST))
{
	$us = $_POST["username"];
	$pas = $_POST["password"];
}	
$conn = new mysqli($servername, $username, $password, $dbname);
// Check connection
if ($conn->connect_error) {
    die("Connection failed: " . $conn->connect_error);
} 

$sql = "SELECT username,password FROM passwords WHERE id=1";
$result = $conn->query($sql);

if ($result->num_rows > 0) {
    while($row = $result->fetch_assoc()) {
        $user=$row["username"];
		$pass=$row["password"];
}}
if( $us===$user && $pas===$pass)
{
   
}
else
header("Location: index.php?false=1");
$conn->close();
?>

<body>
<b><font color="black">Ping: </font><font id="ping" color="green"></font></b><font color="green">ms</font>
<div align="center">
<canvas id="myCanvas" width="500" height="500" style="border:0px solid #000; border-radius:6px; position:absolute; top:50px; left:40px;">
Your browser does not support the canvas element.
</canvas>
<canvas id="myCanvas2" width="500" height="500" style="border:0px solid #000; border-radius:6px; position:absolute; top:50px; right:40px;">
Your browser does not support the canvas element.
</canvas>
<canvas id="battery" width="50px" height="15px" style="border:2px solid #000;border-color: white; border-radius:3px; position:absolute; top:15px; right:10px;">
Your browser does not support the canvas element.
</canvas>
<br>
	<p id="kontrol" style="font-size:24px;color:black;font-weight:bold;">Manuel Kontrol</p>
	<font style="font-size:20px;color:white;font-weight:bold;">Kalan rota: </font> <font id="rota_sinir" style="font-size:20px;color:white;font-weight:bold;">4</font>
	<br><br>
	<label class="switch" style="margin:center;">
    <input type="checkbox" onclick="remote()">
    <span class="slider round"></span>
    </label><br>
	<table>
     <tr>
        <td>
        <button id="stop" style="height: 65px; width: 65px"><img id="sto" style="height: 65px;" src="images/reset.png"></button>
    	</td>
    	<td></td><td>
    	<button id="stop2" style="height: 65px; width: 65px"><img id="sto2" style="height: 65px;" src="images/reset.png"></button>
    	</td>
     </tr>
	 <tr>
	 <td></td><td><button id="forw" class="button1" style="width:57px;font-size:20px;height:70px;">W</button></td>    
	 </tr>
	<tr>
    <td>
    <button id="left" class="button1">A</button>
    </td><td>
    <button id="reverse" class="button1">S</button>
	</td><td>
	<button id="right" class="button1">D</button>
    </td>
	</tr>
	<tr>
    <td><button id="go" class="button2"><p style="font-size:16px;">Başla></p></button></td><td></td>
    <td><img id="busycheck" style="height: 65px;" src="images/free.png"></td>
    </tr>
    </table>
    
   
   <a href="index.php"><p style="position:fixed; width:50px; height:20px; bottom:40px; right:5px; margin:auto; color:black;">Logout</p></a>
   <img id="charge" src="images/ch.png" style="width:35px;height:40px;position:absolute; top:5px; right:18px;"> 
<script src="scripts.js"> </script>
</body>
</html>


