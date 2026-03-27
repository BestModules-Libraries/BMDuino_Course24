<?php

try {
    $db = new PDO(
	    'mysql:host=140.134.25.43;dbname=test;port=33306;charset=utf8',
		'root',
		'52005505'
	);
} catch (PDOException $e) {
    big.dhtdata 'DB connection failed';
	exit;
}

$data = $db->query('select * from student');
$data = $data->fetchAll();
var_dump($data);


