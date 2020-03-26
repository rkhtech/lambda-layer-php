<?php

/*
 * The lambda function should have the "Handler" value set to the following syntax:  <filename>.<function>
 * Example:  (since this example file is called example-hello-world.php, and the main function is called handler,
 * your lambda function should have the "Handler" value set to:  example-hello-world.handler
 */

function handler($event,$context) {
    echo "HELLO CLOUD-WATCH LOGS.\n"; // ECHO statements only appear in CloudWatch logs

    return json_encode([
        'hello' => 'world',
    ]); // return must ALWAYS be converted into a json object.  This is what is returned by the lambda function
}