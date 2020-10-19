
<?php

$servername = "localhost";
$username = "cllkocco_celo";
$password = "504504";
$dbname = "cllkocco_kontrol";

$conn = new mysqli($servername, $username, $password, $dbname);
// Check connection
if ($conn->connect_error) {
    die("Connection failed: " . $conn->connect_error);
} 

$sql = "SELECT CODE FROM control WHERE id=1";
$result = $conn->query($sql);
    if ($result->num_rows > 0) {
    while($row = $result->fetch_assoc()) {
          $code = $row['CODE'];
}}
if($code==7)
{
    $i=0;
    $sql2 = "SELECT x,y FROM pozisyon";
    $result = $conn->query($sql2);
    if ($result->num_rows > 0) {
    while($row = $result->fetch_assoc()) {
        $arrayX[$i] = (int)$row["x"] ; $arrayY[$i]=(int)$row["y"]; $i++;
}}
echo '{"x":';
echo json_encode($arrayX);
echo ',"y":';
echo json_encode($arrayY);  
echo "}";    
    
}else
{
    echo $code;
}
 
    


if(isset($_GET['x']) && isset($_GET['y']) && isset($_GET['p']) && isset($_GET['a']) && isset($_GET['v'])&& isset($_GET['f']))
{
    $X =(int) $_GET['x'];
    $Y =(int) $_GET['y'];
    $P =(int) $_GET['p'];
    $A = (int) $_GET['a']; 
    $V = (int) $_GET['v'];
    $F = (int) $_GET['f']; 
    $sql = "UPDATE location SET x=$X,y=$Y,p=$P,angle=$A,volt=$V,f=$F WHERE id=1";
    $conn->query($sql);
    
}
$conn->close();
?>

