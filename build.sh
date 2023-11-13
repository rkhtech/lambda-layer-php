#!/bin/bash

set -e

php_version=$(cat default-php-version)

if [[ ! -z $1 ]];then
  php_version=$1
fi


if [[ ! -d $php_version ]]; then
  echo "usage: ./build.sh phpversiondir"
  exit 1
fi

cd $php_version

cd opt/downloads
./download_packages.sh
cd ../..

docker pull amazonlinux:2

time docker build -t rkhtech/lambda-layer-php:latest .

version=$(cat Dockerfile | grep '^ENV PHP_VERSION' | awk '{print $3}')

docker tag rkhtech/lambda-layer-php:latest rkhtech/lambda-layer-php:${version}

docker run --name php-$version -d rkhtech/lambda-layer-php:latest

docker cp php-$version:/opt/php-${version}.zip php-${version}.zip
docker cp php-$version:/opt/phpinfo.txt phpinfo.txt
cp phpinfo.txt ~/git/smalf.cloud/html/nav/layers/phpinfo.txt

docker rm -f php-$version
