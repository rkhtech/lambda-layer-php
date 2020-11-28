#!/bin/bash

rm bootstrap
gcc bootstrap.c -lcurl -std=gnu99 -o bootstrap -lrt
