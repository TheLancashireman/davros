#	Makefile - demo Makefile for testing parts of davroska on raspberry pi (zero, 3)
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

# Select your hardware here
DV_BOARD	?= pi3-arm64
#DV_BOARD	?= pi-zero

# Select your compiler here
ifeq ($(DV_BOARD), pi3-arm64)

DEMO_BOARD	?=	DEMO_PI3_ARM64
DV_GNU_D	?=	/data1/gnu/gcc-linaro-6.3.1-2017.02-x86_64_aarch64-elf

DV_GCC		?=	$(DV_GNU_D)/bin/aarch64-elf-gcc
DV_LD		?=	$(DV_GNU_D)/bin/aarch64-elf-ld
DV_OBJCOPY	?=	$(DV_GNU_D)/bin/aarch64-elf-objcopy
DV_LDLIB_D	?=	$(DV_GNU_D)/aarch64-elf/libc/usr/lib/
DV_LDSCRIPT	?=	$(DVSK_ROOT)/hardware/arm64/ld/dv-pi3.ldscript

DV_ENTRY	?=	dv_reset

else

DEMO_BOARD	?=	DEMO_PI_ZERO
DV_GNU_D	?=	/data1/gnu/gcc-linaro-6.3.1-2017.02-x86_64_arm-eabi

DV_GCC		?=	$(DV_GNU_D)/bin/arm-eabi-gcc
DV_LD		?=	$(DV_GNU_D)/bin/arm-eabi-ld
DV_OBJCOPY	?=	$(DV_GNU_D)/bin/arm-eabi-objcopy
DV_LDLIB_D	?=	$(DV_GNU_D)/arm-eabi/libc/usr/lib/
DV_LDSCRIPT	?=	$(DVSK_ROOT)/hardware/arm/ld/dv-pi-zero.ldscript

DV_ENTRY	?=	dv_trap_reset

endif

DV_BIN_D	= bin
DV_OBJ_D	= obj

CC_OPT		+=	-D DV_DAVROSKA=1
CC_OPT		+=	-D DEMO_BOARD=$(DEMO_BOARD)
CC_OPT		+= -I h
CC_OPT		+= -I $(DVSK_ROOT)/h
CC_OPT		+= -I $(DVSK_ROOT)/hardware
CC_OPT		+= -I $(DV_ROOT)/devices/h
CC_OPT		+= -I $(DV_ROOT)/lib/h
CC_OPT		+= -Wall
CC_OPT		+= -fno-common
#CC_OPT		+= -mfloat-abi=hard
#CC_OPT		+= -mfpu=vfp
#CC_OPT		+= -O2

LD_OPT		+= -e $(DV_ENTRY)
LD_OPT		+= -T $(DV_LDSCRIPT)
LD_OPT		+=	-L $(DV_LDLIB_D)
LD_OPT		+=	-lc

# The demo code
DV_LD_OBJS	+= $(DV_OBJ_D)/demo.o

# davroska and associated library files
DV_LD_OBJS	+= $(DV_OBJ_D)/davroska.o
DV_LD_OBJS	+= $(DV_OBJ_D)/davroska-time.o
DV_LD_OBJS	+= $(DV_OBJ_D)/davroska-extended.o
DV_LD_OBJS	+= $(DV_OBJ_D)/dv-printf.o
DV_LD_OBJS	+= $(DV_OBJ_D)/dv-xprintf.o

# Hardware-specific files
ifeq ($(DV_BOARD), pi3-arm64)

DV_LD_OBJS	+= $(DV_OBJ_D)/davroska-arm64.o
DV_LD_OBJS	+= $(DV_OBJ_D)/demo-pi3-arm64.o

DV_LD_OBJS	+= $(DV_OBJ_D)/dv-arm-bcm2835-uart.o
DV_LD_OBJS	+= $(DV_OBJ_D)/dv-arm-bcm2835-gpio.o
DV_LD_OBJS	+= $(DV_OBJ_D)/dv-arm-bcm2835-interruptcontroller.o
DV_LD_OBJS	+= $(DV_OBJ_D)/dv-arm-bcm2836-interruptcontroller.o

DV_LD_OBJS	+= $(DV_OBJ_D)/dv-armv8-mmu.o
DV_LD_OBJS	+= $(DV_OBJ_D)/dv-arm64-reset.o
DV_LD_OBJS	+= $(DV_OBJ_D)/dv-switchcall.o
DV_LD_OBJS	+= $(DV_OBJ_D)/dv-switch-el3el2.o
DV_LD_OBJS	+= $(DV_OBJ_D)/dv-switch-el2el1.o
DV_LD_OBJS	+= $(DV_OBJ_D)/dv-vectors.o

else

DV_LD_OBJS	+= $(DV_OBJ_D)/davroska-arm.o
DV_LD_OBJS	+= $(DV_OBJ_D)/demo-pi-zero.o

DV_LD_OBJS	+= $(DV_OBJ_D)/dv-arm-bcm2835-uart.o
DV_LD_OBJS	+= $(DV_OBJ_D)/dv-arm-bcm2835-gpio.o
DV_LD_OBJS	+= $(DV_OBJ_D)/dv-arm-bcm2835-interruptcontroller.o

DV_LD_OBJS	+= $(DV_OBJ_D)/dv-armv6-mmu.o
DV_LD_OBJS	+= $(DV_OBJ_D)/dv-vectors.o
DV_LD_OBJS	+= $(DV_OBJ_D)/dv-trap-irq.o
DV_LD_OBJS	+= $(DV_OBJ_D)/dv-switchcall.o
DV_LD_OBJS	+= $(DV_OBJ_D)/dv-arm-reset.o
DV_LD_OBJS	+= $(DV_OBJ_D)/dv-arm-traps.o

endif

VPATH 		+=	c
VPATH 		+=	s
VPATH		+=	$(DVSK_ROOT)/c
VPATH		+=	$(DVSK_ROOT)/c

ifeq ($(DV_BOARD), pi3-arm64)

VPATH		+=	$(DVSK_ROOT)/hardware/arm64/c
VPATH		+=	$(DVSK_ROOT)/hardware/arm64/s

else

VPATH		+=	$(DVSK_ROOT)/hardware/arm/c
VPATH		+=	$(DVSK_ROOT)/hardware/arm/s

endif

VPATH		+=	$(DVSK_ROOT)/hardware/common/c
VPATH		+=	$(DV_ROOT)/lib/c
VPATH		+=	$(DV_ROOT)/devices/c


.PHONY:		default all help clean install srec

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

install:	all
	$(DV_OBJCOPY) bin/davroska.elf -O binary /data1/projects/raspberry-pi/usb-zero/msd/kernel.img

srec:		all
	$(DV_OBJCOPY) bin/davroska.elf -O srec --srec-forceS3 /dev/stdout | dos2unix | egrep -v '^S3..........00*..$$' > bin/davroska.srec
