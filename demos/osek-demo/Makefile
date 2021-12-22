#	Makefile - demo Makefile for testing parts of the davroska OSEK compatibility layer
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

#	Usage:
#		make [DV_BOARD=pi3-arm64|pi-zero] [DV_GNU_D=</path/to/gcc>] [IMAGE_FILE=</place/to/install/file.img>]
#	Alternatively, you can set DV_BOARD DV_GNU_D and IMAGE_FILE as environment variables.
#
#	Targets:
#		clean: removes all object and binary files
#		default: compiles and links
#		install: objcopy the ELF file to a binary (img) file IMAGE_FILE
#		srec: objcopy the ELF to an S-record file in the bin directory
#		uf2: create a UF2 file from the ELf file

# Find out where we are :-)
PRJ_ROOT	?= $(shell pwd)
DV_ROOT		?= $(shell (cd $(PRJ_ROOT)/../../ ; pwd))
DVSK_ROOT	= $(DV_ROOT)/davroska

# Select your hardware here
#DV_BOARD	?= blue-pill
#DV_BOARD	?= pi3-arm64
#DV_BOARD	?= pi-zero
DV_BOARD	?= pi-pico

# Select your compiler here
ifeq ($(DV_BOARD), pi3-arm64)

DEMO_BOARD	?=	DEMO_PI3_ARM64
#DV_GNU_D	?=	/data1/gnu/gcc-linaro-6.3.1-2017.02-x86_64_aarch64-elf
DV_GNU_D	?=	/data1/gnu/gcc-linaro-7.4.1-2019.02-x86_64_aarch64-elf

DV_GCC		?=	$(DV_GNU_D)/bin/aarch64-elf-gcc
DV_LD		?=	$(DV_GNU_D)/bin/aarch64-elf-ld
DV_OBJCOPY	?=	$(DV_GNU_D)/bin/aarch64-elf-objcopy
DV_LDLIB_D	?=	$(DV_GNU_D)/aarch64-elf/libc/usr/lib/
DV_LDSCRIPT	?=	$(DVSK_ROOT)/hardware/arm64/ld/dv-pi3.ldscript

DV_ENTRY	?=	-e dv_reset

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
DV_OBJCOPY	?=	$(DV_GNU_D)/bin/arm-eabi-objcopy
DV_LDLIB_D	?=	$(DV_GNU_D)/arm-eabi/libc/usr/lib/
DV_LDSCRIPT	?=	$(DVSK_ROOT)/hardware/arm/ld/dv-pi-zero.ldscript
LD_LIB_ARCH	?=	-lgcc

DV_ENTRY	?=	-e dv_trap_reset

# The directory where 'make install' puts the binary file could be a mounted sd card,
# or the msd directory of rpi-boot
IMAGE_FILE	?=	/data1/projects/raspberry-pi/usb-zero/msd/kernel.img

endif

ifeq ($(DV_BOARD), blue-pill)

DEMO_BOARD	?=	DEMO_BLUE_PILL

ifeq (A, A)

# ARM gcc
DV_GNU_D	?=	/data1/gnu/gcc-arm-none-eabi-9-2019-q4-major

DV_GCC		?=	$(DV_GNU_D)/bin/arm-none-eabi-gcc
DV_LD		?=	$(DV_GNU_D)/bin/arm-none-eabi-ld
DV_OBJCOPY	?=	$(DV_GNU_D)/bin/arm-none-eabi-objcopy
DV_LDLIB_D	?=	$(DV_GNU_D)/arm-none-eabi/lib/thumb/v7-m/nofp

else

# linaro gcc
DV_GNU_D	?=	/data1/gnu/gcc-linaro-7.5.0-2019.12-x86_64_arm-eabi

DV_GCC		?=	$(DV_GNU_D)/bin/arm-eabi-gcc
DV_LD		?=	$(DV_GNU_D)/bin/arm-eabi-ld
DV_OBJCOPY	?=	$(DV_GNU_D)/bin/arm-eabi-objcopy
DV_LDLIB_D	?=	$(DV_GNU_D)/arm-eabi/libc/usr/lib/thumb

endif

LD_LIB_ARCH	?=	-lg

DV_LDSCRIPT	?=	$(DVSK_ROOT)/hardware/arm/ld/dv-blue-pill.ldscript

DV_ENTRY	?=	-e dv_reset

IMAGE_FILE	?=	demo.bin
SREC_FILE	?=	demo.srec

CC_OPT		+= -mcpu=cortex-m3

endif

ifeq ($(DV_BOARD), pi-pico)

DEMO_BOARD	?=	DEMO_PI_PICO

ifeq (A, A)

# Host arm-gcc
DV_GNU_D	?=	/usr

else

# ARM gcc
DV_GNU_D	?=	/data1/gnu/gcc-arm-none-eabi-9-2019-q4-major

endif

DV_GCC		?=	$(DV_GNU_D)/bin/arm-none-eabi-gcc
DV_LD		?=	$(DV_GNU_D)/bin/arm-none-eabi-ld
DV_OBJCOPY	?=	$(DV_GNU_D)/bin/arm-none-eabi-objcopy
DV_LDLIB_D	?=	$(DV_GNU_D)/lib/arm-none-eabi/newlib/thumb/v6-m/nofp/
DV_LDEABI_D	?=	/data1/projects/OtherPeople/libaeabi-cortexm0
DV_ELF2UF2	?=	elf2uf2

DV_LDSCRIPT	?=	$(DVSK_ROOT)/hardware/arm/ld/dv-pi-pico-ram.ldscript

DV_ENTRY	?=	--thumb-entry dv_boot

IMAGE_FILE	?=	demo.bin
SREC_FILE	?=	demo.srec
UF2_FILE	?=	demo.uf2

CC_OPT		+= -mcpu=cortex-m0plus
CC_OPT		+= -mthumb

LD_LIBDIR_ARCH	= -L $(DV_LDEABI_D)
LD_LIB_ARCH		= -laeabi-cortexm0

endif

ifeq ($(DEMO_BOARD), )

$(error Unknown board)

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
CC_OPT		+= -g

CC_OPT		+= -O2

#CC_OPT		+= -mfloat-abi=hard
#CC_OPT		+= -mfpu=vfp

LD_OPT		+= $(DV_ENTRY)
LD_OPT		+= -T $(DV_LDSCRIPT)
LD_OPT		+=	-L $(DV_LDLIB_D)
LD_OPT		+=	$(LD_LIBDIR_ARCH)
LD_OPT		+=	-lc
LD_OPT		+=	$(LD_LIB_ARCH)

# The demo code
DV_LD_OBJS	+= $(DV_OBJ_D)/demo.o

# davroska and associated library files
DV_LD_OBJS	+= $(DV_OBJ_D)/davroska.o
DV_LD_OBJS	+= $(DV_OBJ_D)/davroska-time.o
DV_LD_OBJS	+= $(DV_OBJ_D)/davroska-extended.o
DV_LD_OBJS	+= $(DV_OBJ_D)/davroska-osek.o
DV_LD_OBJS	+= $(DV_OBJ_D)/dv-printf.o
DV_LD_OBJS	+= $(DV_OBJ_D)/dv-xprintf.o
DV_LD_OBJS	+= $(DV_OBJ_D)/dv-memset32.o
DV_LD_OBJS	+= $(DV_OBJ_D)/os.o

VPATH 		+=	c
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

DV_LD_OBJS	+= $(DV_OBJ_D)/davroska-cortexm.o
DV_LD_OBJS	+= $(DV_OBJ_D)/dv-cortexm-vectors.o
DV_LD_OBJS	+= $(DV_OBJ_D)/dv-switchcall.o
DV_LD_OBJS	+= $(DV_OBJ_D)/dv-cortexm-ctxsw.o
DV_LD_OBJS	+= $(DV_OBJ_D)/dv-nvic.o

DV_LD_OBJS	+= $(DV_OBJ_D)/dv-stm32-rcc.o
DV_LD_OBJS	+= $(DV_OBJ_D)/dv-stm32-uart.o
DV_LD_OBJS	+= $(DV_OBJ_D)/dv-stm32-timx.o

DV_LD_OBJS	+= $(DV_OBJ_D)/demo-blue-pill.o

VPATH		+=	$(DVSK_ROOT)/hardware/arm/c
VPATH		+=	$(DVSK_ROOT)/hardware/arm/s

endif

ifeq ($(DV_BOARD), pi-pico)

DV_LD_OBJS	+= $(DV_OBJ_D)/davroska-cortexm.o
DV_LD_OBJS	+= $(DV_OBJ_D)/dv-cortexm-vectors.o
DV_LD_OBJS	+= $(DV_OBJ_D)/dv-switchcall.o
DV_LD_OBJS	+= $(DV_OBJ_D)/dv-cortexm-ctxsw.o
DV_LD_OBJS	+= $(DV_OBJ_D)/dv-nvic.o

DV_LD_OBJS	+= $(DV_OBJ_D)/dv-rp2040-uart.o
DV_LD_OBJS	+= $(DV_OBJ_D)/dv-rp2040-clocks.o

DV_LD_OBJS	+= $(DV_OBJ_D)/dv-pico-boot.o
DV_LD_OBJS	+= $(DV_OBJ_D)/demo-pi-pico.o

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

uf2:			$(DV_OBJ_D) $(DV_BIN_D) $(UF2_FILE)

$(UF2_FILE):	$(DV_BIN_D)/davroska.elf
	$(DV_ELF2UF2) -v $(DV_BIN_D)/davroska.elf $(DV_BIN_D)/davroska.uf2
