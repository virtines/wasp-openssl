#!/bin/bash

make clean
CC=/usr/local/bin/vcc ./Configure no-hw
make -j
