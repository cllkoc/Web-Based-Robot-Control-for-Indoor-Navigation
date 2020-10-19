<!DOCTYPE html>
<html>
<body>

<?php
$servername = "localhost";
$username = "cllkocco_celo";
$password = "504504";
$dbname = "cllkocco_kontrol";



// Create connection
$conn = new mysqli($servername, $username, $password, $dbname);
// Check connection
if ($conn->connect_error) {
    die("Connection failed: " . $conn->connect_error);
} 
if(isset($_GET["code"]))
{
    $int = (int)$_GET["code"];
    $sql = "UPDATE control SET CODE=$int WHERE id=1";
    $conn->query($sql);
}
if($int > 10)
 $int = 10;
if(isset($_POST))
{
   $sql = "DROP TABLE pozisyon";
   $conn->query($sql);
   $sql = "CREATE TABLE pozisyon(id INT(6) UNSIGNED AUTO_INCREMENT PRIMARY KEY, x INT(16),y INT(16))";
   $conn->query($sql);
   $xArray = json_decode($_POST['xArray']);
   $yArray = json_decode($_POST['yArray']);
   $sql = "INSERT INTO pozisyon (x,y) VALUES";
   $multiple;
   for($i=0; $i < count($xArray) ; $i++)
   {
        $sayiX = (int)$xArray[$i];
        $sayiY = (int)$yArray[$i];
        $multiple=$multiple."($sayiX,$sayiY)";
        if($i!= count($xArray)-1)
        $multiple=$multiple.",";
  }
  $sql = $sql.$multiple;
}

if ($conn->query($sql) === TRUE) {
    echo "Fucking data updated succesfuly";
} else {
    echo "Error creating table: " . $conn->error;
}

$conn->close();
?>




</body>
</html>