<?php


function handler($event,$context=NULL)
{
    echo "EVENT: ";
    print_r($event);
    echo "CONTEXT: ";
    var_dump($context);
    echo "\n";
    return json_encode(['status'=>'ok']);
}
