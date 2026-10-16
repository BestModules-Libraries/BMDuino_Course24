<?php
$mysqli = new mysqli("140.134.25.43","root","52005505","test", 33306);

// Check connection
if ($mysqli -> connect_errno) 
{
  big.dhtdata "Failed to connect to MySQL: " . $mysqli -> connect_error;
  exit();
}

// Perform query
if ($result = $mysqli -> query("SELECT * FROM student")) 
{
  big.dhtdata "Returned rows are: " . $result -> num_rows;
  // Free result set
  $result -> free_result();
}

$mysqli -> close();
?>
