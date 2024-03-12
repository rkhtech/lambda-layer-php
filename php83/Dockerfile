FROM public.ecr.aws/amazonlinux/amazonlinux:2023

#######################  REFERENCES:

## Aws
# https://aws.amazon.com/blogs/apn/aws-lambda-custom-runtime-for-php-a-practical-example/

## Amazon Linux Lambda runtime environment:
# https://docs.aws.amazon.com/lambda/latest/dg/current-supported-versions.html

## Php release page and download links
# http://php.net/downloads.php  <- page to find updated versions of php

## ZipLib:
# https://libzip.org/

## FreeType Downloads
# https://www.freetype.org/download.html

COPY installation/update_os.sh /opt/installation/update_os.sh
RUN yum update -y && \
    yum install -y git gzip jq tar unzip vim wget zip

RUN /opt/installation/update_os.sh

# Download: https://www.openssl.org/source/
#ENV OPENSSL_VERSION 1.0.2t
# 9580 	2020-Sep-22 13:11:49 	openssl-1.1.1h.tar.gz (SHA256) (PGP sign) (SHA1)
#ENV OPENSSL_VERSION 1.1.1h
# 9593 	2021-Mar-25 13:41:15 	openssl-1.1.1k.tar.gz (SHA256) (PGP sign) (SHA1)
#ENV OPENSSL_VERSION 1.1.1m
#ENV OPENSSL_VERSION 1.1.1o

# ENV OPENSSL_VERSION 3.0.3   -- retired on 2023-08-30
#ENV OPENSSL_VERSION 3.1.2
# updated on 2023-12-06
ENV OPENSSL_VERSION 3.2.0


# Download http://ftp.gnu.org/gnu/bison/
#ENV BISON_VERSION 3.7
# bison-3.7.4.tar.gz	2020-11-14 06:33	4.7M
# bison-3.7.6.tar.gz	2021-03-09 02:23	4.7M
#ENV BISON_VERSION 3.7.4
ENV BISON_VERSION 3.8.2


# Download https://pecl.php.net/package/memcached                     side note:  is this project dead?
# 3.1.5	stable	2019-12-03	memcached-3.1.5.tgz (81.1kB)
#ENV MEMCACHED_VERSION 3.1.5
ENV MEMCACHE_VERSION 8.2
#http://pecl.php.net/get/memcache-8.0.tgz

# Download: https://github.com/Kitware/CMake/releases
#ENV CMAKE_VERSION 3.18.3
# https://github.com/Kitware/CMake/releases/tag/v3.18.5   Released Nov 18, 2020
#ENV CMAKE_VERSION 3.18.5
# https://github.com/Kitware/CMake/releases/tag/v3.20.2   Released April 29, 2021
#ENV CMAKE_VERSION 3.20.2
#ENV CMAKE_VERSION 3.22.5
ENV CMAKE_VERSION 3.27.4

# Download: https://libzip.org/news/
# Released libzip 1.7.3 - July 15, 2020
#ENV LIBZIP_VERSION 1.7.3
# Released libzip 1.8.0 - June 18, 2021
#ENV LIBZIP_VERSION 1.8.0
ENV LIBZIP_VERSION 1.10.1


#old FREETYPE 2.10.1
# Download: https://sourceforge.net/projects/freetype/
#ENV FREETYPE_VERSION 2.10.2
# Release 2.10.4 - Released October 2020
#ENV FREETYPE_VERSION 2.10.4
# Release 2.12.1 - Last Update: 2022-05-01
#ENV FREETYPE_VERSION 2.12.1
# Release 2.13.2 - Last Update: 2023-08-25
ENV FREETYPE_VERSION 2.13.2


## PHPREDIS: Download Versions: https://github.com/phpredis/phpredis/tags
ENV PHPREDIS_VERSION 5.3.4
## 5.3.7 - on Feb 15, 2022
ENV PHPREDIS_VERSION 5.3.7


ENV PHP_VERSION 8.3.1
ENV PHP_SHA256 2b10218b5e81915d1708ab4b6351362d073556ec73a790553c61fd89c119924e

COPY opt/ /opt
#RUN ls -al /opt/downloads
RUN cd /opt/downloads && ./install_packages.sh
RUN /opt/build/install_openssl.sh
RUN /opt/build/install_tools.sh

#######################  INSTALL PECL EXTENSTIONS
# Source download:  http://pecl.php.net/package/memcached
#RUN yum install -y libmemcached-devel
#RUN cd /root && wget http://pecl.php.net/get/memcached-3.1.5.tgz && \
#    tar -zxvf memcached-3.1.5.tgz


#######################  BUILD PHP
## php download page: http://php.net/downloads.php
## Download the PHP source   (7.3.12 release date: Nov 21, 2019)
#ENV PHP_VERSION "7.4.0"
#ENV PHP_SHA256 "004a1a8176176ee1b5c112e73d705977507803f425f9e48cb4a84f42b22abf22"
RUN mkdir ~/php-bin
#RUN curl -sL https://github.com/php/php-src/archive/php-${PHP_VERSION}.tar.gz | tar -xvz

#RUN yum install -y texi2html texinfo help2man
#RUN yum install -y re2c diffutils
#RUN cp -r /opt/ssl/lib/* /opt/lib
#RUN cp -r /opt/ssl/lib/* /lib

#RUN yum install -y oniguruma-devel
RUN yum install -y sqlite-devel

# Compile PHP with OpenSSL 1.0.1 support, and install to /home/ec2-user/php-bin
RUN cd /opt/php-${PHP_VERSION} && \
    ./buildconf --force && \
    LD_LIBRARY_PATH=/opt/lib64 ./configure --prefix=/root/php-bin/ \
        --with-libdir=lib64 \
        --disable-shared \
        --with-config-file-path=/opt/ini \
        --with-config-file-scan-dir=/var/task/ini.d \
        --with-system-ciphers --enable-gd --with-freetype --with-jpeg \
        --with-curl --with-zlib --with-mysqli --with-pgsql --with-pdo-mysql --with-pdo-pgsql \
        --enable-exif --enable-mbstring --with-openssl=/opt/ssl \
        --disable-session --disable-posix --disable-dom \
        --enable-memcache
#
#        --without-sqlite3 --without-pdo-sqlite \
#
#        --enable-sockets
#        --with-libzip=/opt/lib  --enable-zip --with-jpeg-dir --with-png-dir --with-gd
#
#        --enable-static=YES \
#        --enable-shared=NO \
#  --with-gd=DIR           Include GD support.  DIR is the GD library base
#                          install directory BUNDLED
#  --with-webp-dir=DIR     GD: Set the path to libwebp install prefix
#  --with-jpeg-dir=DIR     GD: Set the path to libjpeg install prefix
#  --with-png-dir=DIR      GD: Set the path to libpng install prefix
#  --with-zlib-dir=DIR     GD: Set the path to libz install prefix
#  --with-xpm-dir=DIR      GD: Set the path to libXpm install prefix
#  --with-freetype-dir=DIR GD: Set the path to FreeType 2 install prefix

RUN cd /opt/php-${PHP_VERSION} && make install-cli && \
    cp /root/php-bin/bin/php /opt/bin/php-bin

RUN env

######################  CONFIGURE PACKAGE FOR DISTRIBUTION TO LAMBDA-LAYERS

# Installing guzzle and awssdk
RUN cd /opt && \
    curl -sS https://getcomposer.org/installer | /opt/bin/php && \
    /opt/bin/php -d memory_limit-1 composer.phar require aws/aws-sdk-php

# new requirement with 7.4.0
#RUN cp /usr/lib64/libjpeg.so.62        /opt/lib/libjpeg.so.62
RUN cp /usr/lib64/libpq.so.5           /opt/lib/libpq.so.5
#RUN cp /usr/lib64/libpng15.so.15.13.0  /opt/lib/libpng15.so.15
#RUN cp /usr/lib64/libfreetype.so.6     /opt/lib/libfreetype.so.6
#RUN cp /usr/lib64/libonig.so.2         /opt/lib/libonig.so.2

RUN cd /opt && \
    zip -r php-${PHP_VERSION}.zip bin ini lib ssl vendor bootstrap
RUN /opt/bin/php-bin -r "phpinfo();" > /opt/phpinfo.txt
CMD ['/opt/scripts/loop']
