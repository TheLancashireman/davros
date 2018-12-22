#	Makefile - demo Makefile for testing parts of davroska on Linux
#
#	Copyright 2018 David Haworth
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

# Find out where we are :-)
DV_ROOT		= ../..
DVSK_ROOT	= ../../davroska

# Select your compiler here
#DV_COMPILER	?=	gnu
#DV_GNU_D	?=	/data1/gnu/gcc-linaro-6.3.1-2017.02-x86_64_aarch64-elf
DV_GNU_D	?=	/data1/gnu/gcc-linaro-6.3.1-2017.02-x86_64_arm-eabi

DV_GCC		?=	$(DV_GNU_D)/bin/arm-eabi-gcc
DV_LD		?=	$(DV_GNU_D)/bin/arm-eabi-ld

DV_BIN_D	= bin
DV_OBJ_D	= obj

CC_OPT		+=	-D DV_DAVROSKA=1
CC_OPT		+= -I h
CC_OPT		+= -I $(DVSK_ROOT)/h
CC_OPT		+= -I $(DVSK_ROOT)/hardware
CC_OPT		+= -I $(DV_ROOT)/devices/h
CC_OPT		+= -I $(DV_ROOT)/lib/h
CC_OPT		+= -Wall
#CC_OPT		+= -O2

LD_OPT		+= -e dv_trap_reset
LD_OPT		+= -T $(DVSK_ROOT)/hardware/arm/ld/dv-pi-zero.ldscript
LD_OPT		+=	-L $(DV_GNU_D)/arm-eabi/libc/usr/lib
LD_OPT		+=	-lc

DV_LD_OBJS	+= $(DV_OBJ_D)/demo.o
DV_LD_OBJS	+= $(DV_OBJ_D)/davroska.o
DV_LD_OBJS	+= $(DV_OBJ_D)/davroska-arm.o
DV_LD_OBJS	+= $(DV_OBJ_D)/dv-printf.o
DV_LD_OBJS	+= $(DV_OBJ_D)/dv-xprintf.o
DV_LD_OBJS	+= $(DV_OBJ_D)/dv-arm-bcm2835-uart.o
DV_LD_OBJS	+= $(DV_OBJ_D)/dv-arm-bcm2835-gpio.o
DV_LD_OBJS	+= $(DV_OBJ_D)/dv-arm-bcm2835-interruptcontroller.o

DV_LD_OBJS	+= $(DV_OBJ_D)/dv-vectors.o
DV_LD_OBJS	+= $(DV_OBJ_D)/dv-reset.o
DV_LD_OBJS	+= $(DV_OBJ_D)/dv-traps.o
DV_LD_OBJS	+= $(DV_OBJ_D)/dv-trap-irq.o

VPATH 		+=	c
VPATH 		+=	s
VPATH		+=	$(DVSK_ROOT)/c
VPATH		+=	$(DVSK_ROOT)/c
VPATH		+=	$(DVSK_ROOT)/hardware/arm/c
VPATH		+=	$(DVSK_ROOT)/hardware/arm/s
VPATH		+=	$(DV_ROOT)/lib/c
VPATH		+=	$(DV_ROOT)/devices/c

.PHONY:		default all help clean install

default:	all

clean:
	-rm -rf $(DV_OBJ_D) $(DV_BIN_D)

all:		$(DV_OBJ_D) $(DV_BIN_D) elf

elf:		$(DV_BIN_D)/davroska.elf

$(DV_BIN_D)/davroska.elf:	$(DV_LD_OBJS)
	$(DV_LD) -o $@ $(DV_LD_OBJS) $(LD_LIB) $(LD_OPT)

$(DV_OBJ_D)/%.o:  %.c
	$(DV_GCC) $(CC_OPT) -o $@ -c $<

$(DV_OBJ_D)/%.o:  %.S
	$(DV_GCC) $(CC_OPT) -o $@ -c $<

$(DV_BIN_D):
	mkdir -p bin

$(DV_OBJ_D):
	mkdir -p obj

install:
	$(DV_GNU_D)/bin/arm-eabi-objcopy bin/davroska.elf -O binary /data1/projects/raspberry-pi/usb-zero/msd/kernel.img

#/data1/gnu/gcc-linaro-6.3.1-2017.02-x86_64_arm-eabi/bin/arm-eabi-ld
#	-ldavroskernel -ldavrosuser
#	-T /data1/projects/davros/demos/dv3-demo-arm/../../davros-3/board/arm/raspberry-pi/ldscript/pi-zero.ldscript
#	-L bin

