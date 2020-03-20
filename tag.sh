#!/bin/bash

if [ -z "$1" ]; then
	echo provide the name of the tag
	exit
fi

docker tag rkhtech/lambda-layer-php:latest rkhtech/lambda-layer-php:$1
docker login -u rkhtech -p xyzzy123
docker push rkhtech/lambda-layer-php:$1
