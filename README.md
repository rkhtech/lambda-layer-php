# Description
This is a pre-complied Lambda layer for PHP

## Current PHP Version: 7.3.x
build date: (7.3.12 release date: Nov 21, 2019)


## Usage and Deployment

```bash
docker run -t --rm \
    -e AWS_ACCESS_KEY_ID=YOURACCESSKEY \
    -e AWS_SECRET_ACCESS_KEY=YOURSECRETACCESSKEY \
    -e LAYER_NAME=php73 \
    rkhtech/lambda-layer-php:73 deploy
```

This will deploy the lambda layer called 'php73' to aws lambda.
If the layer already exists then it will deploy a new version of the layer with the same name.  

### Expected output:
```json
{
  "Content": {
    "CodeSize": 26559645,
    "CodeSha256": "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx",
    "Location": "https://awslambda-us-west-2-layers.s3.us-west-2.amazonaws.com/snapshots/000000000000/php73-xxxxxxxxxxxxxxxxxx"
  },
  "LayerVersionArn": "arn:aws:lambda:us-west-2:000000000000:layer:php73:15",
  "Version": 15,
  "Description": "",
  "CreatedDate": "2019-08-16T23:56:44.202+0000",
  "LayerArn": "arn:aws:lambda:us-west-2:000000000000:layer:php73"
}
```
