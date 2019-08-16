# Description
This is a pre-complied Lambda layer for PHP

## Current PHP Version: 7.3.x
build date: (7.3.8 release date: Aug 1, 2019)


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

```
