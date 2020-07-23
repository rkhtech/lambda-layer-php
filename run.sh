#!/bin/bash



version=$(cat Dockerfile | grep '^ENV PHP_VERSION' | awk '{print $3}')
docker run --rm -it rkhtech/lambda-layer-php:latest bash

