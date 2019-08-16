#!/usr/bin/env bash

OPENSSL_VERSION=1.0.2

#######################  INSTALL OPENSSL  (Note current version: 1.0.2r)
# Compile OpenSSL v1.0.1 from source, as Amazon Linux uses a newer version than the Lambda Execution Environment, which
# would otherwise produce an incompatible binary.

cd /

case $OPENSSL_VERSION in
1.1.1)
    curl -sL http://www.openssl.org/source/openssl-1.1.1b.tar.gz | tar -xvz
    cd /openssl-1.1.1b && ./config && ./Configure --prefix=/opt/ssl  --openssldir=/opt/ssl linux-x86
    make && make install
;;
1.0.2)
    curl -sL http://www.openssl.org/source/openssl-1.0.2r.tar.gz | tar -xvz
    cd /openssl-1.0.2r && ./config --openssldir=/opt/ssl && make && make install
;;
1.0.1)
    curl -sL http://www.openssl.org/source/openssl-1.0.1k.tar.gz | tar -xvz
    cd /openssl-1.0.1k && ./config --openssldir=/opt/ssl && make && make install
;;
esac

mkdir -p /opt/ssl/certs
cd /opt/ssl/certs && wget http://curl.haxx.se/ca/cacert.pem
cp /opt/ssl/certs/cacert.pem /opt/ssl/cert.pem
