#!/bin/bash

docker pull amazonlinux:1

time docker build -t rkhtech/lambda-layer-php:latest .

docker tag rkhtech/lambda-layer-php:latest rkhtech/lambda-layer-php:73
