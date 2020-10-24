#	dt3-blue-pill.make - makefile fragment for building davros-3 test suite for Blue Pill
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

DT3_H_BOARDCFG	?= <dt3-cfg-blue-pill.h>

# Compiler selection. This is a gcc from ARM's website. Linaro gcc doesn't have library support for Cortex-M
DV_GNU_D	?=	/data1/gnu/gcc-arm-none-eabi-9-2019-q4-major

DV_GCC		?=	$(DV_GNU_D)/bin/arm-none-eabi-gcc
DV_LD		?=	$(DV_GNU_D)/bin/arm-none-eabi-ld
DV_AR		?=	$(DV_GNU_D)/bin/arm-none-eabi-ar
DV_OBJCOPY	?=	$(DV_GNU_D)/bin/arm-none-eabi-objcopy
DV_LDLIB_D	?=	$(DV_GNU_D)/arm-none-eabi/lib/thumb/v7-m/nofp

DV_LDSCRIPT	?=	$(DVSK_ROOT)/hardware/arm/ld/dv-blue-pill.ldscript

DV_ENTRY	?=	dv_reset

IMAGE_FILE	?=	demo.bin
SREC_FILE	?=	demo.srec

CC_OPT		+= -mcpu=cortex-m3
