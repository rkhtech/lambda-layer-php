FROM amazonlinux:1

#######################  REFERENCES:

## Aws
# https://aws.amazon.com/blogs/apn/aws-lambda-custom-runtime-for-php-a-practical-example/

## Amazon Linux Lambda runtime environment:
# https://docs.aws.amazon.com/lambda/latest/dg/current-supported-versions.html

## Php release page and download links
# http://php.net/downloads.php  <- page to find updated versions of php

## ZipLib:
# https://libzip.org/

COPY installation/update_os.sh /opt/installation/update_os.sh
RUN /opt/installation/update_os.sh

ENV OPENSSL_VERSION 1.0.2t
ENV BISON_VERSION 3.4
ENV MEMCACHED_VERSION 3.1.5
ENV PHP_VERSION 7.4.0
ENV CMAKE_VERSION 3.16.0
ENV LIBZIP_VERSION 1.5.2

COPY installation/download_packages.sh /opt/installation/download_packages.sh
RUN /opt/installation/download_packages.sh


COPY opt/ /opt
##COPY opt/build/download_packages.sh /opt/build/download_packages.sh
#RUN /opt/build/download_packages.sh
##COPY opt/build/update_os.sh /opt/build/update_os.sh
#RUN /opt/build/update_os.sh
#COPY opt/build/install_openssl.sh /opt/build/install_openssl.sh
RUN /opt/build/install_openssl.sh
#COPY opt/build/install_tools.sh /opt/build/install_tools.sh
RUN /opt/build/install_tools.sh

#######################  INSTALL PECL EXTENSTIONS
# Source download:  http://pecl.php.net/package/memcached
RUN yum install -y libmemcached-devel
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

RUN yum install -y oniguruma-devel
## Compile PHP with OpenSSL 1.0.1 support, and install to /home/ec2-user/php-bin
RUN cd /opt/php-src-php-${PHP_VERSION} && \
    ./buildconf --force && \
    LD_LIBRARY_PATH=/opt/lib ./configure --prefix=/root/php-bin/ \
        --disable-shared \
        --with-config-file-path=/opt/ini \
        --with-config-file-scan-dir=/var/task/ini.d \
        --with-system-ciphers \
        --with-curl --with-zlib --with-mysqli --enable-exif \
        --without-sqlite3 --without-pdo-sqlite --disable-session \
        --enable-sockets --enable-mbstring --with-gmp --with-openssl=/opt/ssl
#        --with-libzip=/opt/lib  --enable-zip --with-jpeg-dir --with-png-dir --with-gd

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

RUN cd /opt/php-src-php-${PHP_VERSION} && make install-cli && \
    cp /root/php-bin/bin/php /opt/bin/php-bin


RUN env

#######################  CONFIGURE PACKAGE FOR DISTRIBUTION TO LAMBDA-LAYERS

# Installing guzzle and awssdk
RUN cd /opt && \
    curl -sS https://getcomposer.org/installer | /opt/bin/php && \
    /opt/bin/php composer.phar require guzzlehttp/guzzle && \
    /opt/bin/php composer.phar require aws/aws-sdk-php
#    /opt/bin/php composer.phar require codercat/jwk-to-pem
#    /opt/bin/php composer.phar require web-token/jwt-core && \
#    /opt/bin/php composer.phar require web-token/jwt-checker && \
#    /opt/bin/php composer.phar require web-token/jwt-framework

# new requirement with 7.4.0
RUN cp /usr/lib64/libonig.so.2 /opt/lib/libonig.so.2

RUN cd /opt && \
    zip -r php74.zip bin ini lib ssl vendor bootstrap
