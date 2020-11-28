<?php 
$_EVENT=json_decode('{"hel\'lo":"world"}',true);
require 'main.php';
$rv=handler($_EVENT);
echo "\n-=-=-=-=RV=-=-=-=-\n";
echo "$rv\n";
