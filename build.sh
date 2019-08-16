#!/bin/bash

docker pull amazonlinux:1

time docker build -t rkhtech/lambda-layer-php:73 .

