<?php
　$Arr = array ('A' => 'apple', 'B' => 'banana');
　print_r($Arr);　// 第一次印出陣列
　print_r($Arr, true);　// 不會印出陣列
　$result=print_r($Arr, true);
　echo $result;　// 第二次印出陣列
?>