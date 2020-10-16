#	pi3-arm64.make - demo makefile fragment for building davros-3 for raspberry pi 3 (aarch64)
#
#	Copyright 2020 David Haworth
#
#	This file is part of davros.
#
#	davros is free software: you can redistribute it and/or modify
#	it under the terms of the GNU General Public License as published by
#	the Free Software Foundation, either version 3 of the License, or
#	(at your option) any later version.
#
#	davros is distributed in the hope that it will be useful,
#	but WITHOUT ANY WARRANTY; without even the implied warranty of
#	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#	GNU General Public License for more details.
#
#	You should have received a copy of the GNU General Public License
#	along with davros.  If not, see <http://www.gnu.org/licenses/>.

DEMO_H_BOARDCFG	?= <dv-cfg-pi3-arm64.h>

# Compiler selection.
#DV_GNU_D	?=	/data1/gnu/gcc-linaro-6.3.1-2017.02-x86_64_aarch64-elf
DV_GNU_D	?=	/data1/gnu/gcc-linaro-7.4.1-2019.02-x86_64_aarch64-elf

DV_GCC		?=	$(DV_GNU_D)/bin/aarch64-elf-gcc
DV_LD		?=	$(DV_GNU_D)/bin/aarch64-elf-ld
DV_AR		?=	$(DV_GNU_D)/bin/aarch64-elf-ar
DV_OBJCOPY	?=	$(DV_GNU_D)/bin/aarch64-elf-objcopy
DV_LDLIB_D	?=	$(DV_GNU_D)/aarch64-elf/libc/usr/lib/
DV_LDSCRIPT	?=	$(DVSK_ROOT)/hardware/arm64/ld/dv-pi3.ldscript

DV_ENTRY	?=	dv_reset

# The directory where 'make install' puts the binary file could be a mounted sd card,
# or the msd directory of rpi-boot
IMAGE_FILE	?=	kernel.img
