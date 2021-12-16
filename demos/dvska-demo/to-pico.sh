#!/bin/sh
#
# Copy a (UF2) file to the Raspberry Pi pico
# You may have to enter your password for the sudo parts. Read the script to understand what it does before
# trusting it with supervisor privileges.
#
# Assumptions:
#	* Hot-plugged filesystems don't automatically get mounted
#	* The pico shows up reliably at /dev/sdc --> change PICODEV if not
#	* The mount point is /media/pico --> change PICOMNT if not
#	* You have sudo permissions to mount and umount filesystems  --> hard luck if not ;-)
#
# (c) David Haworth

PICODEV=/dev/sdc1
PICOMNT=/media/pico

set -e

if [ -z "$1" ] ; then
	echo "Usage: to-pico.sh <UF2-file>"
	exit 1
fi

sudo mount -o uid=${USER} ${PICODEV} ${PICOMNT}
if [ -e ${PICOMNT}/INFO_UF2.TXT ]; then
	if diff ${PICOMNT}/INFO_UF2.TXT INFO_UF2.TXT ; then
		cp $1 ${PICOMNT}
	else
		echo "The mounted UF2 bootloader is not a Raspberry Pi pico"
		exit 1
	fi
else
	echo "The mounted filesystem is not a UF2 bootloader"
	exit 1
fi
sudo umount ${PICOMNT}
