#!/bin/bash

$(cat ../../Dockerfile | grep ENV | grep -v '^#' | awk '{print "export "$2"="$3}')

if [ ! -f php-${PHP_VERSION}.tar.gz ]; then
  echo Downloading PHP_VERSION ${PHP_VERSION}
  #curl -sL https://github.com/php/php-src/archive/php-${PHP_VERSION}.tar.gz | tar -xz
  wget https://www.php.net/distributions/php-${PHP_VERSION}.tar.gz
  if [ $PHP_SHA256 == $(openssl sha256 php-${PHP_VERSION}.tar.gz | cut -d' ' -f2) ]; then
#    tar -xzf php-${PHP_VERSION}.tar.gz
    echo "php download passed sha256 signature validation"
  else
    rm php-${PHP_VERSION}.tar.gz
    echo "php download doesn't pass sha256 signature"
    exit 1
  fi
fi

if [ ! -f memcached-${MEMCACHED_VERSION}.tgz ]; then
  echo Downloading MEMCACHED_VERSION ${MEMCACHED_VERSION}
  wget http://pecl.php.net/get/memcached-${MEMCACHED_VERSION}.tgz
fi

if [ ! -f openssl-${OPENSSL_VERSION}.tar.gz ]; then
  echo Downloading OPENSSL_VERSION ${OPENSSL_VERSION}
  wget http://www.openssl.org/source/openssl-${OPENSSL_VERSION}.tar.gz
fi

if [ ! -f cmake-${CMAKE_VERSION}-Linux-x86_64.tar.gz ]; then
  echo Downloading CMAKE_VERSION ${CMAKE_VERSION}
  wget https://github.com/Kitware/CMake/releases/download/v${CMAKE_VERSION}/cmake-${CMAKE_VERSION}-Linux-x86_64.tar.gz
fi

if [ ! -f libzip-${LIBZIP_VERSION}.tar.gz ]; then
  echo Downloading LIBZIP_VERSION ${LIBZIP_VERSION}
  wget https://libzip.org/download/libzip-${LIBZIP_VERSION}.tar.gz
fi

if [ ! -f bison-${BISON_VERSION}.tar.gz ]; then
  echo Downloading BISON_VERSION ${BISON_VERSION}
  wget http://ftp.gnu.org/gnu/bison/bison-${BISON_VERSION}.tar.gz
fi

if [ ! -f freetype-${FREETYPE_VERSION}.tar.gz ]; then
  echo Downloading FREETYPE_VERSION ${FREETYPE_VERSION}
  wget https://sourceforge.net/projects/freetype/files/freetype2/${FREETYPE_VERSION}/freetype-${FREETYPE_VERSION}.tar.gz
fi

### Download PHPREDIS
if [ ! -f phpredis-${PHPREDIS_VERSION}.tar.gz ]; then
  echo Downloading PHPREDIS_VERSION ${PHPREDIS_VERSION}
  wget https://github.com/phpredis/phpredis/archive/refs/tags/${PHPREDIS_VERSION}.tar.gz
fi
#phpredis-5.3.4.tar.gz
