#!/bin/bash

version=$(cat Dockerfile | grep '^ENV PHP_VERSION' | awk '{print $3}')

echo "PHP_VERSION: $version"

if [ -f "php-$version.zip" ]; then
  aws s3 cp php-$version.zip s3://rkh-pub/lambda-layers/php-${version}.zip --acl public-read --profile rkhtech
  aws s3 cp s3://rkh-pub/lambda-layers/php-${version}.zip s3://rkh-pub/lambda-layer-php/php-latest.zip --acl public-read --profile rkhtech
else
  echo "file doesnt exist, can't publish, try building it first."
fi
