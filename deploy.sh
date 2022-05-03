#!/usr/local/bin/bash

if [ ! -d $1 ]; then
  echo "usage: ./build.sh phpversiondir"
  exit 1
fi

cd $1

PHP_VERSION=$(cat Dockerfile | grep '^ENV PHP_VERSION' | awk '{print $3}')

echo "PHP_VERSION: $PHP_VERSION"

#aws lambda publish-layer-version --layer-name $(echo $1 | tr  [:lower:] [:upper:]) --content S3Bucket=rkh-pub,S3Key=lambda-layer-php/php-latest.zip --compatible-runtimes provided.al2
aws lambda publish-layer-version --layer-name $(echo $1 | tr  [:lower:] [:upper:]) --zip-file fileb://php-${PHP_VERSION}.zip --compatible-runtimes provided.al2
