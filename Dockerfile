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

COPY opt/build/update_os.sh /opt/build/update_os.sh
RUN /opt/build/update_os.sh
COPY opt/build/install_tools.sh /opt/build/install_tools.sh
RUN /opt/build/install_tools.sh
COPY opt/build/install_openssl.sh /opt/build/install_openssl.sh
RUN /opt/build/install_openssl.sh

#######################  INSTALL PECL EXTENSTIONS
# Source download:  http://pecl.php.net/package/memcached
RUN yum install -y libmemcached-devel
RUN cd /root && wget http://pecl.php.net/get/memcached-3.1.4.tgz && \
    tar -zxvf memcached-3.1.4.tgz
#//    cd memcached-3.1.3 && phpize && \


#######################  BUILD PHP
## php download page: http://php.net/downloads.php
## Download the PHP source   (7.3.12 release date: Nov 21, 2019)
ENV PHP_VERSION "7.3.12"
ENV PHP_SHA256 "d617e5116f8472a628083f448ebe4afdbc4ac013c9a890b08946649dcbe61b34
"
RUN mkdir ~/php-7-bin
RUN curl -sL https://github.com/php/php-src/archive/php-${PHP_VERSION}.tar.gz | tar -xvz

## Compile PHP with OpenSSL 1.0.1 support, and install to /home/ec2-user/php-7-bin
RUN cd php-src-php-${PHP_VERSION} && \
    ./buildconf --force && \
    LD_LIBRARY_PATH=/opt/lib ./configure --prefix=/root/php-7-bin/ \
        --with-config-file-path=/opt/ini \
        --with-config-file-scan-dir=/var/task/ini.d \
        --with-system-ciphers \
        --with-libzip=/opt/lib \
        --with-curl --with-zlib --with-mysqli --enable-zip --enable-exif \
        --without-sqlite3 --without-pdo-sqlite --disable-session \
        --with-gd --with-jpeg-dir --with-png-dir \
        --enable-sockets --enable-mbstring --with-gmp \
        --with-openssl=/opt/ssl

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

RUN cd php-src-php-${PHP_VERSION} && make install-cli && \
    cp /root/php-7-bin/bin/php /opt/bin/php-bin
COPY opt/ /opt

RUN env
RUN cp -r /opt/aws /root/.aws
RUN cp /opt/scripts/* /usr/local/bin

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

RUN cd /opt && \
    zip -r php73.zip bin ini lib ssl vendor bootstrap
