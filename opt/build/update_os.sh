#!/usr/bin/env bash

#######################  UPDATE OS
yum update -y && \
yum install autoconf bison gcc gcc-c++ glibc-devel.x86_64 libcurl-devel libxml2-devel gzip tar make git \
php-devel zip unzip libjpeg libjpeg-turbo-devel libpng libpng-devel jq wget mlocate vim gmp gmp-devel -y

# Setup directory structure (similar to how it will be deployed in a lambda layer
mkdir -p /opt/{bin,src,ini,ssl,lib}/
mkdir -p /opt/ssl/certs

