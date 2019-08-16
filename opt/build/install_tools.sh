#!/usr/bin/env bash


#install cmake from source
mkdir /libzip && cd /libzip && wget https://cmake.org/files/v3.14/cmake-3.14.0.tar.gz && \
    tar zxvf cmake-3.14.0.tar.gz && \
    cd cmake-3.14.0 && \
    ./bootstrap && \
    make -j -l 2.5 && \
    make install


# Install AWS CLI
curl -O https://bootstrap.pypa.io/get-pip.py && \
    python get-pip.py && \
    pip install awscli

# Build libzip from source, as the yum repos have an older version: 0.10.1, requires >= 0.11
cd /libzip && wget https://libzip.org/download/libzip-1.5.2.tar.gz && \
    tar zxvf libzip-1.5.2.tar.gz && \
    cd libzip-1.5.2 && \
    mkdir build && \
    cd build && \
    CFLAGS=-DBUILD_SHARED_LIBS=OFF /usr/local/bin/cmake .. && \
    make -j -l 2.5 && \
    make -j -l 2.5  test && \
    make -j -l 2.5  install
cp /usr/local/lib64/libzip* /opt/lib/

