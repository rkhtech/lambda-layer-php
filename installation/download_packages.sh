#!/bin/bash

cd /opt

curl -sL http://pecl.php.net/get/memcached-${MEMCACHED_VERSION}.tgz | tar -xz

curl -sL http://www.openssl.org/source/openssl-${OPENSSL_VERSION}.tar.gz | tar -xz

curl -sL https://github.com/php/php-src/archive/php-${PHP_VERSION}.tar.gz | tar -xz

curl -sL https://github.com/Kitware/CMake/releases/download/v${CMAKE_VERSION}/cmake-${CMAKE_VERSION}-Linux-x86_64.tar.gz | tar -xz
cd /opt/cmake-${CMAKE_VERSION}-Linux-x86_64 && cp -r * /usr
cd /opt

curl -sL https://libzip.org/download/libzip-${LIBZIP_VERSION}.tar.gz | tar -xz

curl -sL http://ftp.gnu.org/gnu/bison/bison-${BISON_VERSION}.tar.gz | tar -xz

curl -sL https://sourceforge.net/projects/freetype/files/freetype2/${FREETYPE_VERSION}/freetype-${FREETYPE_VERSION}.tar.gz | tar -xz
