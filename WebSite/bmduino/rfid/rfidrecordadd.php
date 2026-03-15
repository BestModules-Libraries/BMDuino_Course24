<?php
$servername = "localhost";
$username = "big";
$password = "12345678";
$dbname = "big";

try {
    $conn = new PDO("mysql:host=$servername;dbname=$dbname", $username, $password);
    $conn->setAttribute(PDO::ATTR_ERRMODE, PDO::ERRMODE_EXCEPTION);
    
    $rfidkey = filter_input(INPUT_POST, 'rfidkey', FILTER_SANITIZE_STRING);
    $activate = filter_input(INPUT_POST, 'activate', FILTER_VALIDATE_INT);
    $startdatetime = filter_input(INPUT_POST, 'startdatetime', FILTER_SANITIZE_STRING);
    $enddatetime = filter_input(INPUT_POST, 'enddatetime', FILTER_SANITIZE_STRING);
    $systime = filter_input(INPUT_POST, 'systime', FILTER_SANITIZE_STRING);
    $IP = filter_input(INPUT_POST, 'IP', FILTER_SANITIZE_STRING);
    
    $sql = "INSERT INTO rfidlist (rfidkey, activate, startdatetime, enddatetime, systime, IP) 
            VALUES (:rfidkey, :activate, :startdatetime, :enddatetime, :systime, :IP)";
            
    $stmt = $conn->prepare($sql);
    $stmt->bindParam(':rfidkey', $rfidkey);
    $stmt->bindParam(':activate', $activate, PDO::PARAM_INT);
    $stmt->bindParam(':startdatetime', $startdatetime);
    $stmt->bindParam(':enddatetime', $enddatetime);
    $stmt->bindParam(':systime', $systime);
    $stmt->bindParam(':IP', $IP);
    
    $stmt->execute();
    
    header("Location: add_rfid_form.php");
    exit();
    
} catch(PDOException $e) {
    echo "Error: " . $e->getMessage();
}
$conn = null;
?>