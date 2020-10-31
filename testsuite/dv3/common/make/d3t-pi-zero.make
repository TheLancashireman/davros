#	d3t-pi-zero.make - makefile fragment for building davros-3 test suite for raspberry pi zero
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

D3T_H_BOARDCFG	?= <d3t-cfg-pi-zero.h>

# Compiler selection.
#DV_GNU_D	?= /data1/gnu/gcc-linaro-6.3.1-2017.02-x86_64_arm-eabi
DV_GNU_D	?= /data1/gnu/gcc-linaro-7.5.0-2019.12-x86_64_arm-eabi

DV_GCC		?= $(DV_GNU_D)/bin/arm-eabi-gcc
DV_LD		?= $(DV_GNU_D)/bin/arm-eabi-ld
DV_AR		?= $(DV_GNU_D)/bin/arm-eabi-ar
DV_OBJCOPY	?= $(DV_GNU_D)/bin/arm-eabi-objcopy
DV_LDLIB_D	?= $(DV_GNU_D)/arm-eabi/libc/usr/lib/
DV_LDSCRIPT	?= $(DV3_ROOT)/board/arm/raspberry-pi/ldscript/pi-zero.ldscript

DV_ENTRY	?= dv_trap_reset

# The directory where 'make install' puts the binary file could be a mounted sd card,
# or the msd directory of rpi-boot
DV_IMAGE	?= /data1/projects/raspberry-pi/usb-zero/msd/kernel.img

CC_OPT		+= -mcpu=arm1176jzf-s

VPATH		+= $(DV3_ROOT)/cpufamily/arm/c
VPATH		+= $(DV3_ROOT)/cpufamily/arm/s
VPATH		+= $(DV3_ROOT)/board/arm/raspberry-pi/c
VPATH		+= $(DV_ROOT)/devices/c

# Selected files from the cpufamily/arm directory
DV_LIB_OBJS	+= $(DV_OBJ_D)/dv-armv6-mmu.o
DV_LIB_OBJS	+= $(DV_OBJ_D)/dv-inithardware.o
DV_LIB_OBJS	+= $(DV_OBJ_D)/dv-initmemorymanagement.o
DV_LIB_OBJS	+= $(DV_OBJ_D)/dv-initvectors.o
DV_LIB_OBJS	+= $(DV_OBJ_D)/dv-saveregs.o
DV_LIB_OBJS	+= $(DV_OBJ_D)/dv-arm-returntothread.o
DV_LIB_OBJS	+= $(DV_OBJ_D)/dv-getconfigbase.o
DV_LIB_OBJS	+= $(DV_OBJ_D)/dv-getkvars.o
DV_LIB_OBJS	+= $(DV_OBJ_D)/dv-setkvars.o
DV_LIB_OBJS	+= $(DV_OBJ_D)/dv-dabort.o
DV_LIB_OBJS	+= $(DV_OBJ_D)/dv-pfabort.o
DV_LIB_OBJS	+= $(DV_OBJ_D)/dv-irq.o
DV_LIB_OBJS	+= $(DV_OBJ_D)/dv-reserved.o
DV_LIB_OBJS	+= $(DV_OBJ_D)/dv-sbreak.o
DV_LIB_OBJS	+= $(DV_OBJ_D)/dv-undef.o
DV_LIB_OBJS	+= $(DV_OBJ_D)/dv-reset.o

DV_USR_OBJS	+= $(DV_OBJ_D)/dv-nullsc.o
DV_USR_OBJS	+= $(DV_OBJ_D)/dv-exit.o
DV_USR_OBJS	+= $(DV_OBJ_D)/dv-createexe.o
DV_USR_OBJS	+= $(DV_OBJ_D)/dv-spawn.o
DV_USR_OBJS	+= $(DV_OBJ_D)/dv-sleep.o
DV_USR_OBJS	+= $(DV_OBJ_D)/dv-sleepuntil.o
DV_USR_OBJS	+= $(DV_OBJ_D)/dv-suspend.o
DV_USR_OBJS	+= $(DV_OBJ_D)/dv-resume.o
DV_USR_OBJS	+= $(DV_OBJ_D)/dv-createsemaphore.o
DV_USR_OBJS	+= $(DV_OBJ_D)/dv-destroysemaphore.o
DV_USR_OBJS	+= $(DV_OBJ_D)/dv-usessemaphore.o
DV_USR_OBJS	+= $(DV_OBJ_D)/dv-wait.o
DV_USR_OBJS	+= $(DV_OBJ_D)/dv-signal.o

# Selected files from the devices directory
DV_LIB_OBJS	+= $(DV_OBJ_D)/dv-arm-bcm2835-interruptcontroller.o
DV_LIB_OBJS	+= $(DV_OBJ_D)/dv-arm-bcm2835-uart.o
DV_LIB_OBJS	+= $(DV_OBJ_D)/dv-arm-bcm2835-gpio.o
DV_LIB_OBJS	+= $(DV_OBJ_D)/dv-arm-bcm2835-systimer.o

# Files needed by the test suite
D3T_LIB_OBJS	+= $(DV_OBJ_D)/d3t-traps-arm.o

# Files to feed to the linker
DV_LD_VECT	+= $(DV_OBJ_D)/dv-vectors.o
DV_LD_OBJS	+= $(DV_OBJ_D)/dv-raspberrypi-board.o
