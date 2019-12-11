#!/usr/bin/env bash

#######################  UPDATE OS
yum update -y && \
yum install -y autoconf bison gcc gcc-c++ glibc-devel.i686 glibc-devel libcurl-devel libxml2-devel gzip tar make git \
php-devel zip unzip libjpeg libjpeg-turbo-devel libpng libpng-devel jq wget mlocate vim gmp gmp-devel python36 \
texi2html texinfo help2man re2c diffutils openssl-devel

# Setup directory structure (similar to how it will be deployed in a lambda layer
mkdir -p /opt/{bin,src,ini,ssl,lib}/
mkdir -p /opt/ssl/certs


# Install AWS CLI
curl -O https://bootstrap.pypa.io/get-pip.py && \
    python3 get-pip.py && \
    pip3 install awscli

