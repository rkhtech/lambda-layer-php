#!/bin/bash

if [ ! -d $1 ]; then
  echo "usage: ./build.sh phpversiondir"
  exit 1
fi

cd $1

version=$(cat Dockerfile | grep '^ENV PHP_VERSION' | awk '{print $3}')

echo "PHP_VERSION: $version"

if [ -f "php-$version.zip" ]; then
  aws s3 cp php-$version.zip s3://rkh-pub/lambda-layers/php-${version}.zip --profile rkhtech
  if [ "$1" == "php81" ]; then
    aws s3 cp s3://rkh-pub/lambda-layers/php-${version}.zip s3://rkh-pub/lambda-layer-php/php-latest.zip --profile rkhtech
  fi
else
  echo "file doesnt exist, can't publish, try building it first."
fi
