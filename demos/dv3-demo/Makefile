#	Makefile - demo Makefile for testing parts of davros-3 on raspberry pi (zero, 3) or Blue Pill
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

#	Usage:
#		make [DV_BOARD=pi3-arm64|pi-zero] [DV_GNU_D=</path/to/gcc>] [IMAGE_FILE=</place/to/install/file.img>]
#	Alternatively, you can set DV_BOARD DV_GNU_D and IMAGE_FILE as environment variables.
#
#	Targets:
#		clean: removes all object and binary files
#		default: compiles and links
#		install: objcopy the ELF file to a binary (img) file IMAGE_FILE
#		srec: objcopy the ELF to an S-record file in the bin directory

PRJ_ROOT	?= $(shell pwd)
DV_ROOT		?= $(shell (cd $(PRJ_ROOT)/../../ ; pwd))
DV3_ROOT	?= $(DV_ROOT)/davros-3

# Select your hardware here
#DV_BOARD	?= blue-pill
#DV_BOARD	?= pi3-arm64
DV_BOARD	?= pi-zero

# Select your compiler here
ifeq ($(DV_BOARD), pi3-arm64)

DEMO_BOARD	?=	DEMO_PI3_ARM64
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

endif

ifeq ($(DV_BOARD), pi-zero)

DEMO_BOARD	?=	DEMO_PI_ZERO
#DV_GNU_D	?=	/data1/gnu/gcc-linaro-6.3.1-2017.02-x86_64_arm-eabi
DV_GNU_D	?=	/data1/gnu/gcc-linaro-7.5.0-2019.12-x86_64_arm-eabi

DV_GCC		?=	$(DV_GNU_D)/bin/arm-eabi-gcc
DV_LD		?=	$(DV_GNU_D)/bin/arm-eabi-ld
DV_AR		?=	$(DV_GNU_D)/bin/arm-eabi-ar
DV_OBJCOPY	?=	$(DV_GNU_D)/bin/arm-eabi-objcopy
DV_LDLIB_D	?=	$(DV_GNU_D)/arm-eabi/libc/usr/lib/
DV_LDSCRIPT	?=	$(DVSK_ROOT)/hardware/arm/ld/dv-pi-zero.ldscript

DV_ENTRY	?=	dv_trap_reset

# The directory where 'make install' puts the binary file could be a mounted sd card,
# or the msd directory of rpi-boot
IMAGE_FILE	?=	/data1/projects/raspberry-pi/usb-zero/msd/kernel.img

VPATH		+= $(DV3_ROOT)/cpufamily/arm/c
VPATH		+= $(DV3_ROOT)/cpufamily/arm/s

# Selected files from the devices directory
DV_LIB_OBJS	+= $(DV_OBJ_D)/dv-arm-bcm2835-interruptcontroller.o

# Selected files from the cpufamily/arm directory
DV_LIB_OBJS	+= $(DV_OBJ_D)/dv-armv6-mmu.o
DV_LIB_OBJS	+= $(DV_OBJ_D)/dv-inithardware.o
DV_LIB_OBJS	+= $(DV_OBJ_D)/dv-initmemorymanagement.o
DV_LIB_OBJS	+= $(DV_OBJ_D)/dv-initvectors.o

endif

ifeq ($(DV_BOARD), blue-pill)

DEMO_BOARD	?=	DEMO_BLUE_PILL

# ARM gcc. Linaro gcc doesn't have library support for Cortex-M
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

endif

ifeq ($(DEMO_BOARD), )

$(error Unknown board)

endif

DV_BIN_D	= bin
DV_OBJ_D	= obj
DV_LIB_D	= lib

CC_OPT		+=	-D DV_DAVROSKA=0
CC_OPT		+=	-D DEMO_BOARD=$(DEMO_BOARD)
CC_OPT		+= -I h
CC_OPT		+= -I $(DV3_ROOT)
CC_OPT		+= -I $(DV_ROOT)/devices/h
CC_OPT		+= -I $(DV_ROOT)/lib/h
CC_OPT		+= -Wall
CC_OPT		+= -fno-common
CC_OPT		+= -g

CC_OPT		+= -O2

#CC_OPT		+= -mfloat-abi=hard
#CC_OPT		+= -mfpu=vfp

LD_OPT		+= -e $(DV_ENTRY)
LD_OPT		+= -T $(DV_LDSCRIPT)
LD_OPT		+=	-L $(DV_LDLIB_D)
LD_OPT		+=	-lc

DV_LIBS		+=	$(DV_LIB_D)/libdavros3.a

# The demo code
DV_LD_OBJS	+= $(DV_OBJ_D)/demo.o

# davros-3 kernel library files
VPATH 		+= $(DV3_ROOT)/kernel/c
VPATH 		+= $(DV_ROOT)/devices/c
VPATH 		+= $(DV_ROOT)/devices/s

DV_LIB_OBJS	+= $(addprefix $(DV_OBJ_D)/,$(addsuffix .o,$(basename $(notdir $(wildcard $(DV3_ROOT)/kernel/c/*.c)))))


.PHONY:		default all help clean install srec tempdefault

tempdefault:	libs

default:	elf

clean:
	-rm -rf $(DV_OBJ_D) $(DV_BIN_D) $(DV_LIB_D)

elf:		$(DV_OBJ_D) $(DV_BIN_D)/davros-3.elf

libs:		$(DV_OBJ_D) $(DV_LIBS)

$(DV_BIN_D)/davros-3.elf:	$(DV_BIN_D) $(DV_LD_OBJS) libs
	$(DV_LD) -o $@ $(DV_LD_OBJS) $(LD_LIB) $(LD_OPT)

$(DV_LIB_D)/libdavros3.a:	$(DV_LIB_D) $(DV_LIB_OBJS)
	-rm $@
	$(DV_AR) cqs $@ $(DV_LIB_OBJS)
	

$(DV_OBJ_D)/%.o:  %.c
	$(DV_GCC) $(CC_OPT) -o $@ -c $<

$(DV_OBJ_D)/%.o:  %.S
	$(DV_GCC) $(CC_OPT) -o $@ -c $<

$(DV_BIN_D):
	mkdir -p $(DV_BIN_D)

$(DV_LIB_D):
	mkdir -p $(DV_LIB_D)

$(DV_OBJ_D):
	mkdir -p $(DV_OBJ_D)


ifeq (A,B)

VPATH 		+=	s
VPATH		+=	$(DVSK_ROOT)/c

# Hardware-specific files and paths
ifeq ($(DV_BOARD), pi3-arm64)

DV_LD_OBJS	+= $(DV_OBJ_D)/davroska-arm64.o
DV_LD_OBJS	+= $(DV_OBJ_D)/demo-pi3-arm64.o

DV_LD_OBJS	+= $(DV_OBJ_D)/dv-arm-bcm2835-uart.o
DV_LD_OBJS	+= $(DV_OBJ_D)/dv-arm-bcm2835-gpio.o
DV_LD_OBJS	+= $(DV_OBJ_D)/dv-arm-bcm2835-interruptcontroller.o
DV_LD_OBJS	+= $(DV_OBJ_D)/dv-arm-bcm2836-interruptcontroller.o

DV_LD_OBJS	+= $(DV_OBJ_D)/dv-armv8-mmu.o
DV_LD_OBJS	+= $(DV_OBJ_D)/dv-arm64-setmmuregisters.o
DV_LD_OBJS	+= $(DV_OBJ_D)/dv-arm64-reset.o
DV_LD_OBJS	+= $(DV_OBJ_D)/dv-switchcall.o
DV_LD_OBJS	+= $(DV_OBJ_D)/dv-switch-el3el2.o
DV_LD_OBJS	+= $(DV_OBJ_D)/dv-switch-el2el1.o
DV_LD_OBJS	+= $(DV_OBJ_D)/dv-vectors.o

VPATH		+=	$(DVSK_ROOT)/hardware/arm64/c
VPATH		+=	$(DVSK_ROOT)/hardware/arm64/s

endif

ifeq ($(DV_BOARD), pi-zero)

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

VPATH		+=	$(DVSK_ROOT)/hardware/arm/c
VPATH		+=	$(DVSK_ROOT)/hardware/arm/s

endif

ifeq ($(DV_BOARD), blue-pill)

DV_LD_OBJS	+= $(DV_OBJ_D)/dv-cortexm-vectors.o
DV_LD_OBJS	+= $(DV_OBJ_D)/dv-stm32-rcc.o
DV_LD_OBJS	+= $(DV_OBJ_D)/dv-stm32-uart.o
DV_LD_OBJS	+= $(DV_OBJ_D)/dv-stm32-timx.o
DV_LD_OBJS	+= $(DV_OBJ_D)/dv-nvic.o
DV_LD_OBJS	+= $(DV_OBJ_D)/demo-blue-pill.o
DV_LD_OBJS	+= $(DV_OBJ_D)/dv-switchcall.o
DV_LD_OBJS	+= $(DV_OBJ_D)/davroska-cortexm.o
DV_LD_OBJS	+= $(DV_OBJ_D)/useful-m3.o

VPATH		+=	$(DVSK_ROOT)/hardware/arm/c
VPATH		+=	$(DVSK_ROOT)/hardware/arm/s

endif

VPATH		+=	$(DVSK_ROOT)/hardware/common/c
VPATH		+=	$(DV_ROOT)/lib/c
VPATH		+=	$(DV_ROOT)/devices/c
VPATH		+=	$(DV_ROOT)/devices/s


.PHONY:		default all help clean install srec

default:	elf

clean:
	-rm -rf $(DV_OBJ_D) $(DV_BIN_D)

elf:		$(DV_OBJ_D) $(DV_BIN_D)/davroska.elf

$(DV_BIN_D)/davroska.elf:	$(DV_BIN_D) $(DV_LD_OBJS)
	$(DV_LD) -o $@ $(DV_LD_OBJS) $(LD_LIB) $(LD_OPT)

$(DV_OBJ_D)/%.o:  %.c
	$(DV_GCC) $(CC_OPT) -o $@ -c $<

$(DV_OBJ_D)/%.o:  %.S
	$(DV_GCC) $(CC_OPT) -o $@ -c $<

$(DV_BIN_D):
	mkdir -p bin

$(DV_OBJ_D):
	mkdir -p obj

install:		$(DV_OBJ_D) $(DV_BIN_D) $(IMAGE_FILE)

$(IMAGE_FILE):	$(DV_BIN_D)/davroska.elf
	$(DV_OBJCOPY) $(DV_BIN_D)/davroska.elf -O binary $(IMAGE_FILE)
	echo "flash write_image erase $(IMAGE_FILE) 0x08000000"

srec:			$(DV_OBJ_D) $(DV_BIN_D) $(SREC_FILE)

$(SREC_FILE):	$(DV_BIN_D)/davroska.elf
	$(DV_OBJCOPY) $(DV_BIN_D)/davroska.elf -O srec --srec-forceS3 /dev/stdout | dos2unix | egrep -v '^S3..........00*..$$' > $(SREC_FILE)

endif
