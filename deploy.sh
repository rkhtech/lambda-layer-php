#!/bin/bash

PHP_VERSION=$(cat Dockerfile | grep '^ENV PHP_VERSION' | awk '{print $3}')

echo "PHP_VERSION: $PHP_VERSION"

aws lambda publish-layer-version --layer-name PHP2 --content S3Bucket=rkh-pub,S3Key=lambda-layer-php/php-latest.zip --compatible-runtimes provided

