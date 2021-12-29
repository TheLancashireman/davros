#!/bin/sh
# Generate a list of all the TinyUSB object files
#
# Called from the Makefile
cd $1
find src -name \*.c -exec basename \{\} \; | sed -e 's/\.c$/\.o/' | sed -e "s/^/$2\//"
