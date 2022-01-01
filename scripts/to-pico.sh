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
	UF2FILE="bin/davroska.uf2"
else
	UF2FILE="$1"
fi

if [ -e "${UF2FILE}" ]; then
	echo "Uploading ${UF2FILE}"
else
	echo "Usage: to-pico.sh <UF2-file>"
	exit 1
fi

sudo mount -o uid="${USER}" "${PICODEV}" "${PICOMNT}"
if [ -e "${PICOMNT}/INFO_UF2.TXT" ]; then
	LINE=A
	CHECK=OK
	while read -r l ; do
		case ${LINE} in
		A)	if [ "${l}" != "UF2 Bootloader v2.0" ] ; then
				echo "Unexpected line 1 in INFO_UF2.TXT"
				CHECK=BAD
			fi
			LINE=B
			;;
		B)	if [ "${l}" != "Model: Raspberry Pi RP2" ] ; then
				echo "Unexpected line 2 in INFO_UF2.TXT"
				CHECK=BAD
			fi
			LINE=C
			;;
		C)	if [ "${l}" != "Board-ID: RPI-RP2" ] ; then
				echo "Unexpected line 3 in INFO_UF2.TXT"
				CHECK=BAD
			fi
			LINE=D
			;;
		D)	echo "Unexpected extra line in INFO_UF2.TXT"
			CHECK=BAD
			LINE=E
			;;
		E)
			;;
		esac
	done < ${PICOMNT}/INFO_UF2.TXT

	if [ "${LINE}" = "D" ] && [ "${CHECK}" = "OK" ] ; then
		cp "${UF2FILE}" "${PICOMNT}"
	else
		echo "The mounted UF2 bootloader is not a Raspberry Pi pico"
	fi
else
	echo "The mounted filesystem is not a UF2 bootloader"
fi
sudo umount "${PICOMNT}"
