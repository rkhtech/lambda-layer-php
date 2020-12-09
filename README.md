# Description
This is a pre-complied Lambda layer for PHP

## Current PHP Version: 7.4.x
build date: (7.4.13 release date: Nov 26, 2020)

## Deploy directly from S3
```bash
aws lambda publish-layer-version --layer-name LAMBDA-LAYER-PHP --content S3Bucket=rkh-pub,S3Key=lambda-layer-php/php-latest.zip
```
This aws command will create a layer called 'LAMBDA-LAYER-PHP' into your AWS account.

## Build Locally

* Download all source files
```bash
git clone git@github.com:rkhtech/lambda-layer-php.git
cd lambda-layer-php
./build.sh
```
This will produce a local file called: `php-7.4.6.zip` in your local directory.  You can use this file to publish your lambda layer.

## Publish your local build
```bash
aws lambda publish-layer-version --layer-name LAMBDA-LAYER-PHP --zip-file fileb://content.zip --compatible-runtimes provided

This will deploy the lambda layer called 'php74' to aws lambda.
If the layer already exists then it will deploy a new version of the layer with the same name.

### Expected output:
```json
{
  "Content": {
    "CodeSize": 26559645,
    "CodeSha256": "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx",
    "Location": "https://awslambda-us-west-2-layers.s3.us-west-2.amazonaws.com/snapshots/000000000000/php73-xxxxxxxxxxxxxxxxxx"
  },
  "LayerVersionArn": "arn:aws:lambda:us-west-2:000000000000:layer:php74:15",
  "Version": 15,
  "Description": "",
  "CreatedDate": "2019-08-16T23:56:44.202+0000",
  "LayerArn": "arn:aws:lambda:us-west-2:000000000000:layer:php74"
}
```
