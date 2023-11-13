#!/bin/bash

php_version=$(cat default-php-version)

if [[ ! -z $1 ]];then
  php_version=$1
fi


if [ ! -d $php_version ]; then
  echo "usage: ./build.sh phpversiondir"
  exit 1
fi

cd $php_version

version=$(cat Dockerfile | grep '^ENV PHP_VERSION' | awk '{print $3}')

echo "PHP_VERSION: $version"

if [ -f "php-$version.zip" ]; then
  aws s3 cp php-$version.zip s3://rkh-pub/lambda-layers/php-${version}.zip --profile rkhtech
#  if [ "$1" == "php81" ]; then
  aws s3 cp s3://rkh-pub/lambda-layers/php-${version}.zip s3://rkh-pub/lambda-layers/php-latest.zip --profile rkhtech
  aws s3 cp phpinfo.txt s3://rkh-pub/lambda-layers/phpinfo-${version}.txt --profile rkhtech
  aws s3 cp phpinfo.txt s3://rkh-pub/lambda-layers/phpinfo-latest.txt --profile rkhtech
#  fi
else
  echo "file doesnt exist, can't publish, try building it first."
fi
