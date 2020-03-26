<?php

/*
 * The lambda function should have the "Handler" value set to the following syntax:  <filename>.<function>
 * Example:  (since this example file is called example-hello-world.php, and the main function is called handler,
 * your lambda function should have the "Handler" value set to:  example-hello-world.handler
 *
 * Description:  This would be this simplest version of a lambda function that would return a json object callable
 * from an AWS Application Load Balancer.
 */

function handler($event,$context) {
    echo "HELLO CLOUD-WATCH LOGS.\n";              // ECHO statements only appear in CloudWatch logs

    $applicationLoadBalancerResponse = [           // actual returnable values defined in:
        'isBase64Encoded' => true,                 // if you use the urlencode() function, set this to true, otherwise false
        'statusCode' => 200,                       // standard html codes
        'statusDescription' => "200 OK",           // description is optional
        'headers' => [                             // This is an array of standard HTTP headers
            'Content-Type' => 'application/json',  // If returning a json_encoded value, this should be set to application/json.  (can be anything you choose to return)
        ],
        'body' => urlencode(json_encode([
            'hello' => 'world'
        ])),
    ];

    return json_encode($applicationLoadBalancerResponse); // return must ALWAYS be converted into a json object.  This is what is returned by the lambda function
}