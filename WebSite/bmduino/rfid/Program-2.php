<?php
// Program-2: Handle form submission and insert data
include "iotcnn.php";

$link = Connection();

if ($_SERVER["REQUEST_METHOD"] == "POST") {
    // Get form data and sanitize
    $systime = mysqli_real_escape_string($link, $_POST['systime']);
    $IP = !empty($_POST['IP']) ? mysqli_real_escape_string($link, $_POST['IP']) : NULL;
    $rfidkey = mysqli_real_escape_string($link, $_POST['rfidkey']);
    $activate = (int)$_POST['activate'];
    $startdatetime = !empty($_POST['startdatetime']) ? mysqli_real_escape_string($link, $_POST['startdatetime']) : NULL;
    $enddatetime = !empty($_POST['enddatetime']) ? mysqli_real_escape_string($link, $_POST['enddatetime']) : NULL;

    // Prepare SQL insert statement
    $sql = "INSERT INTO rfidlist (systime, IP, rfidkey, activate, startdatetime, enddatetime) 
            VALUES ('$systime', " . ($IP ? "'$IP'" : "NULL") . ", '$rfidkey', $activate, " . 
            ($startdatetime ? "'$startdatetime'" : "NULL") . ", " . ($enddatetime ? "'$enddatetime'" : "NULL") . ")";

    // Execute query
    if (mysqli_query($link, $sql)) {
        // Close connection
        mysqli_close($link);
        // Redirect to display page
        header("Location: rfid_display.php");
        exit();
    } else {
        echo "Error: " . mysqli_error($link);
    }

    // Close connection
    mysqli_close($link);
}
?>