#!/bin/sh
# Generate a list of all the TinyUSB object files
#
# Called from the Makefile
#
# $1 is the tinyusb directory
# $2 is the object directory

# Sanitise the object directory - add \ in front of /
O=$(echo "$2" | sed -e 's/\//\\\//g')

cd "$1" || exit
find src -name \*.c -exec basename \{\} \; | sed -e 's/\.c$/\.o/' | sed -e "s/^/${O}\//"
