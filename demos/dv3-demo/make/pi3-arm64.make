#	pi3-arm64.make - demo makefile fragment for building davros-3 for raspberry pi 3 (aarch64)
#
#	Copyright David Haworth
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
DV_LDSCRIPT	?=	$(DV3_ROOT)/board/arm64/raspberry-pi-3/ldscript/rpi3-0based.ldscript

DV_ENTRY	?=	dv_reset

CC_OPT		+= -mcpu=cortex-a53

VPATH		+= $(DV3_ROOT)/cpufamily/arm64/c
VPATH		+= $(DV3_ROOT)/cpufamily/arm64/s
VPATH		+= $(DV3_ROOT)/board/arm64/raspberry-pi-3/c
VPATH		+= $(DV_ROOT)/devices/c

# Selected files from the cpufamily/arm64 directory
DV_LIB_OBJS	+= $(DV_OBJ_D)/dv-catch-thread-synchronous-exception.o
DV_LIB_OBJS	+= $(DV_OBJ_D)/dv-init-core.o
DV_LIB_OBJS	+= $(DV_OBJ_D)/dv-inithardware.o
DV_LIB_OBJS	+= $(DV_OBJ_D)/dv-initmemorymanagement.o
DV_LIB_OBJS	+= $(DV_OBJ_D)/dv-initvectors.o

DV_USR_OBJS	+= $(DV_OBJ_D)/dv-nullsc.o
DV_USR_OBJS	+= $(DV_OBJ_D)/dv-exit.o
DV_USR_OBJS	+= $(DV_OBJ_D)/dv-createexe.o
DV_USR_OBJS	+= $(DV_OBJ_D)/dv-spawn.o
DV_USR_OBJS	+= $(DV_OBJ_D)/dv-sleep.o
DV_USR_OBJS	+= $(DV_OBJ_D)/dv-sleepuntil.o

# Selected files from the devices directory
DV_LIB_OBJS	+= $(DV_OBJ_D)/dv-arm-bcm2836-interruptcontroller.o
DV_LIB_OBJS	+= $(DV_OBJ_D)/dv-arm-bcm2835-interruptcontroller.o
DV_LIB_OBJS	+= $(DV_OBJ_D)/dv-arm-bcm2835-uart.o
DV_LIB_OBJS	+= $(DV_OBJ_D)/dv-arm-bcm2835-gpio.o
DV_LIB_OBJS	+= $(DV_OBJ_D)/dv-arm-bcm2835-systimer.o

# Files to feed to the linker
DV_LD_OBJS	+= $(DV_OBJ_D)/dv-raspberrypi-board.o
