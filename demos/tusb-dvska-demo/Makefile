#	Makefile - demo Makefile for a USB-MIDI device using tinyusb with davroska
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
#		make [DV_BOARD=blue-pill|pi-pico|...] [DV_COMPILER=...]
#
#	Targets:
#		clean: removes all object and binary files
#		default: compiles and links
#		install: objcopy the ELF file to a binary (img) file IMAGE_FILE
#		srec: objcopy the ELF to an S-record file in the bin directory
#		uf2: create a UF2 file from the ELf file

# Find out where we are :-)
DEMO_D		?= $(shell pwd)
DV_D		?= $(shell (cd $(DEMO_D)/../../ ; pwd))
PRJ_D		?= $(shell (cd $(DEMO_D)/../../../ ; pwd))
GNU_D		?= $(shell (cd $(DEMO_D)/../../../../gnu ; pwd))

DAVROSKA_D	= $(DV_D)/davroska

# 3rd party libraries etc.
STM32CUBE	?= $(PRJ_D)/OtherPeople/ST/STM32CubeF1
TINYUSB_D	?= $(PRJ_D)/OtherPeople/tinyusb
AEABI_V6M_D	?= $(PRJ_D)/libaeabi-cortexm0

# Select your hardware here. TinyUSB has no support for pi-zero or pi3
#DV_BOARD	?= blue-pill
DV_BOARD	?= pi-pico
#DV_BOARD	?= pi3-arm64
#DV_BOARD	?= pi-zero

ifeq ($(DV_BOARD), pi3-arm64)
$(error  No TinyUSB support for pi3)
DEMO_BOARD		?=	DEMO_PI3_ARM64
DV_COMPILER		?=	linaro-aarch64
DV_LDSCRIPT		?=	$(DAVROSKA_D)/hardware/arm64/ld/dv-pi3.ldscript
DV_ENTRY		?=	-e dv_reset
CC_OPT			+= -mfloat-abi=hard
CC_OPT			+= -mfpu=vfp

default:		srec

endif

ifeq ($(DV_BOARD), pi-zero)
$(error  No TinyUSB support for pi zero)
DEMO_BOARD		?=	DEMO_PI_ZERO
DV_COMPILER		?=	linaro-arm
DV_LDSCRIPT		?=	$(DAVROSKA_D)/hardware/arm/ld/dv-pi-zero.ldscript
DV_ENTRY		?=	-e dv_trap_reset
IMAGE_FILE		?=	$(PRJ_D)/raspberry-pi/usb-zero/msd/kernel.img
CC_OPT			+= -mfloat-abi=hard
CC_OPT			+= -mfpu=vfp

default:		install

endif

ifeq ($(DV_BOARD), blue-pill)
DEMO_BOARD		?=	DEMO_BLUE_PILL
DV_COMPILER		?=	arm-gcc
DV_LDSCRIPT		?=	$(DAVROSKA_D)/hardware/arm/ld/dv-blue-pill.ldscript
DV_ENTRY		?=	--thumb-entry dv_reset
#LD_OPT			+=	-L $(DV_GNU_D)/lib/gcc/arm-none-eabi/10.3.1/thumb/v7-m/nofp
#LD_OPT			+=	-L /usr/lib/arm-none-eabi/newlib/thumb/v7-m/nofp/
LD_OPT			+=	-L $(DV_GNU_D)/arm-none-eabi/lib/thumb/v7-m/nofp
LD_OPT			+=	-L $(DV_GNU_D)/lib/gcc/arm-none-eabi/9.2.1/thumb/v7-m/nofp
CC_OPT			+=	-mcpu=cortex-m3
CC_OPT			+=	-mthumb
CC_OPT			+=	-I $(STM32CUBE)/Drivers/CMSIS/Device/ST/STM32F1xx/Include
CC_OPT			+=	-I $(STM32CUBE)/Drivers/CMSIS/Core/Include

default:		openocd

endif

ifeq ($(DV_BOARD), pi-pico)
DEMO_BOARD		?=	DEMO_PI_PICO
DV_COMPILER		?=	host-arm
DV_LDSCRIPT		?=	$(DAVROSKA_D)/hardware/arm/ld/dv-pi-pico-ram.ldscript
DV_ENTRY		?=	--thumb-entry dv_boot
CC_OPT			+=	-mcpu=cortex-m0plus
CC_OPT			+=	-mthumb
# A compatibility gasket for tinyusb to avoid pulling in the entire pico-sdk
CC_OPT			+=	-I $(DV_D)/lib/h/tinyusb-picosdk-gasket
CC_OPT			+=	-I $(PRJ_D)/pico/pico-sdk/src

DV_LDLIB		?=	-lc -laeabi-cortexm0
LD_OPT			?=	-L $(DV_GNU_D)/lib/arm-none-eabi/newlib/thumb/v6-m/nofp/ -L $(PRJ_D)/OtherPeople/libaeabi-cortexm0

default:		uf2

endif

ifeq ($(DEMO_BOARD), )
$(error Unknown board)
endif

ifeq ($(DV_COMPILER), arm-gcc)
# gcc from arm.com
DV_GNU_D		?=	$(GNU_D)/gcc-arm-none-eabi-9-2019-q4-major
DV_GNU_INFIX	?=	arm-none-eabi
endif

ifeq ($(DV_COMPILER), host-arm)
# arm gcc install on host
DV_GNU_D		?=	/usr
DV_GNU_INFIX	?=	arm-none-eabi
endif

ifeq ($(DV_COMPILER), linaro-arm)
DV_GNU_D		?=	$(GNU_D)/gcc-linaro-7.5.0-2019.12-x86_64_arm-eabi
#DV_GNU_D		?=	$(GNU_D)/gcc-linaro-6.3.1-2017.02-x86_64_arm-eabi
DV_GNU_INFIX	?=	arm-eabi
endif

ifeq ($(DV_COMPILER), linaro-aarch64)
DV_GNU_D		?=	$(GNU_D)/gcc-linaro-7.4.1-2019.02-x86_64_aarch64-elf
#DV_GNU_D		?=	$(GNU_D)/gcc-linaro-6.3.1-2017.02-x86_64_aarch64-elf
DV_GNU_INFIX	?=	aarch64-elf
endif

ifeq ($(DV_GNU_INFIX), )
$(error Unknown compiler)
endif

# Programs
DV_GCC		?=	$(DV_GNU_D)/bin/$(DV_GNU_INFIX)-gcc
DV_LD		?=	$(DV_GNU_D)/bin/$(DV_GNU_INFIX)-ld
DV_OBJCOPY	?=	$(DV_GNU_D)/bin/$(DV_GNU_INFIX)-objcopy
DV_AR		?=	$(DV_GNU_D)/bin/$(DV_GNU_INFIX)-ar
DV_ELF2UF2	?=	elf2uf2

# Build locations
DV_BIN_D	=	bin
DV_OBJ_D	=	obj
DV_LIB_D	=	lib

# Build files
ELF_FILE	?=	$(DV_BIN_D)/demo.elf
IMAGE_FILE	?=	$(DV_BIN_D)/demo.bin
SREC_FILE	?=	$(DV_BIN_D)/demo.srec
UF2_FILE	?=	$(DV_BIN_D)/demo.uf2
LIBUSB_FILE	?=	$(DV_LIB_D)/libtusb.a


CC_OPT		+=	-D DV_DAVROSKA=1
CC_OPT		+=	-D DEMO_BOARD=$(DEMO_BOARD)
CC_OPT		+= -I h
CC_OPT		+= -I ../davroska-common/h
CC_OPT		+= -I $(DAVROSKA_D)/h
CC_OPT		+= -I $(DAVROSKA_D)/hardware
CC_OPT		+= -I $(DV_D)/devices/h
CC_OPT		+= -I $(DV_D)/lib/h
CC_OPT		+= -I $(TINYUSB_D)/src
CC_OPT		+= -Wall
CC_OPT		+= -fno-common
CC_OPT		+= -g
CC_OPT		+= -O2


DV_LDLIB	?= -lc -lgcc
LD_OPT		+= $(DV_ENTRY)
LD_OPT		+= -T $(DV_LDSCRIPT)
LD_OPT		+= -L $(DV_LIB_D)

# The demo code
DV_LD_OBJS	+= $(DV_OBJ_D)/demo.o
DV_LD_OBJS	+= $(DV_OBJ_D)/usb-midi.o

# davroska and associated library files
DV_LD_OBJS	+= $(DV_OBJ_D)/davroska.o
DV_LD_OBJS	+= $(DV_OBJ_D)/davroska-time.o
DV_LD_OBJS	+= $(DV_OBJ_D)/davroska-extended.o
DV_LD_OBJS	+= $(DV_OBJ_D)/dv-printf.o
DV_LD_OBJS	+= $(DV_OBJ_D)/dv-xprintf.o
DV_LD_OBJS	+= $(DV_OBJ_D)/dv-memset32.o

VPATH 		+=	c
VPATH 		+=	s
VPATH 		+=	../davroska-common/c
VPATH 		+=	../davroska-common/s
VPATH		+=	$(DAVROSKA_D)/c

# TinyUSB: src and all subdirectories
VPATH		+=	$(shell find $(TINYUSB_D)/src -type d)

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

VPATH		+=	$(DAVROSKA_D)/hardware/arm64/c
VPATH		+=	$(DAVROSKA_D)/hardware/arm64/s

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

VPATH		+=	$(DAVROSKA_D)/hardware/arm/c
VPATH		+=	$(DAVROSKA_D)/hardware/arm/s

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

VPATH		+=	$(DAVROSKA_D)/hardware/arm/c
VPATH		+=	$(DAVROSKA_D)/hardware/arm/s

endif

ifeq ($(DV_BOARD), pi-pico)

DV_LD_OBJS	+= $(DV_OBJ_D)/davroska-cortexm.o
DV_LD_OBJS	+= $(DV_OBJ_D)/dv-cortexm-vectors.o
DV_LD_OBJS	+= $(DV_OBJ_D)/dv-switchcall.o
DV_LD_OBJS	+= $(DV_OBJ_D)/dv-cortexm-ctxsw.o
DV_LD_OBJS	+= $(DV_OBJ_D)/dv-nvic.o

DV_LD_OBJS	+= $(DV_OBJ_D)/dv-rp2040-clocks.o
DV_LD_OBJS	+= $(DV_OBJ_D)/dv-rp2040-uart.o
DV_LD_OBJS	+= $(DV_OBJ_D)/dv-rp2040-usb.o

DV_LD_OBJS	+= $(DV_OBJ_D)/dv-pico-boot.o
DV_LD_OBJS	+= $(DV_OBJ_D)/demo-pi-pico.o

VPATH		+=	$(DAVROSKA_D)/hardware/arm/c
VPATH		+=	$(DAVROSKA_D)/hardware/arm/s

endif

VPATH		+=	$(DAVROSKA_D)/hardware/common/c
VPATH		+=	$(DV_D)/lib/c
VPATH		+=	$(DV_D)/devices/c
VPATH		+=	$(DV_D)/devices/s

# TinyUSB
LIBUSB_OBJS	= $(shell $(DV_D)/scripts/tusb-list.sh $(TINYUSB_D) $(DV_OBJ_D))

.PHONY:		default all help clean install srec uf2 libusb openocd

clean:
	-rm -rf $(DV_OBJ_D) $(DV_BIN_D) $(DV_LIB_D)

elf:			$(DV_OBJ_D) $(DV_BIN_D) libusb $(ELF_FILE) 

$(ELF_FILE):	$(DV_LD_OBJS)
	$(DV_LD) -o $@ $(DV_LD_OBJS) $(LD_OPT) -ltusb $(DV_LDLIB)

$(DV_OBJ_D)/%.o:  %.c
	$(DV_GCC) $(CC_OPT) -o $@ -c $<

$(DV_OBJ_D)/%.o:  %.S
	$(DV_GCC) $(CC_OPT) -o $@ -c $<

$(DV_BIN_D):
	mkdir -p $@

$(DV_OBJ_D):
	mkdir -p $@

$(DV_LIB_D):
	mkdir -p $@

install:		$(IMAGE_FILE)

$(IMAGE_FILE):	elf
	$(DV_OBJCOPY) $(ELF_FILE) -O binary $@

openocd:		install
	@echo "Use these commands in openocd:"
	@echo "reset halt"
	@echo "flash write_image erase $(IMAGE_FILE) 0x08000000"

srec:			$(SREC_FILE)

$(SREC_FILE):	elf
	$(DV_OBJCOPY) $(ELF_FILE) -O srec --srec-forceS3 /dev/stdout | dos2unix | egrep -v '^S3..........00*..$$' > $@

uf2:			$(UF2_FILE)

$(UF2_FILE):	elf
	$(DV_ELF2UF2) -v $(ELF_FILE) $@

libusb:			$(DV_OBJ_D) $(DV_LIB_D) $(LIBUSB_FILE)

$(LIBUSB_FILE):	$(LIBUSB_OBJS)
	-rm $@
	$(DV_AR) -crs $@ $(LIBUSB_OBJS)

