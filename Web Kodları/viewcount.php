<?php 


header ('Content-Type: application/json');
$servername = "localhost";
$username = "cllkocco_celo";
$password = "504504";
$dbname = "cllkocco_kontrol";

$conn = new mysqli($servername, $username, $password, $dbname);
// Check connection
if ($conn->connect_error) {
    die("Connection failed: " . $conn->connect_error);
} 

$sql = "SELECT x,y,p,angle,volt,f FROM location WHERE id=1";
$result = $conn->query($sql);

if ($result->num_rows > 0) {
    while($row = $result->fetch_assoc()) {
        $keywords[0] = $row["x"];$keywords[1] = $row["y"];$keywords[2] = $row["p"];$keywords[3] = $row["angle"]; $keywords[4] = $row["volt"]; $keywords[5] = $row["f"];
}}


$data = [
    'viewX' => (int) $keywords[0],
    'viewY'=> (int)$keywords[1],
    'Ping'=> (int)$keywords[2],
    'Angle'=>(int)$keywords[3],
    'volt' => (int)$keywords[4],
    'f' => (int)$keywords[5]
    ];
$conn->close();
echo json_encode($data);











?>