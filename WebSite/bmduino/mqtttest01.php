
  <?php
  
  		function BWSwitch($mm,$cc)
		{
			$str = '{"Device":"%s",	"NO":"1","Command":"%s","Mode":"BW","Color":"FFFFFF"}' ;
			return sprintf($str,$mm,$cc) ;
			
		}
	
		
	//echo "01  <br>" ;
	require("./phpMQTT/phpMQTT.php");
	//echo "02  <br>" ;
	//echo $topicname."/03  <br>" ;
//	echo $payload."/04  <br>" ;
//	echo "02  <br>" ;
	$client_id = "tw".sprintf("%d",Math.rand(100,10000)); // make sure this is unique for connecting to sever - you could use uniqid()
	$server = "broker.emqx.io";     // change if necessary
	$port = 1883;                     // change if necessary
	$username = "";                   // set your username
	$password = "";                   // set your password
	$mqtt = new Bluerhinos\phpMQTT($server, $port, $client_id);

	$topicname = "/ncnu/controller" ;
	$payload = BWSwitch("E89F6DE869F8","ON") ;
	echo "ready";
	echo "set";
	if ($mqtt->connect(true, NULL, $username, $password)) {
		echo "connect";
		$mqtt->publish($topicname , $payload, 0);
		$mqtt->close();
		echo "SUCCESS!\n";
	} else {
		echo "Time out!\n";
	}

	
  


?>
