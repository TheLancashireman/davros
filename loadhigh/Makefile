#	Makefile - top level Makefile for loadhigh (arm64)
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
PRJ_ROOT		?= $(shell pwd)
DV_ROOT			?= $(PRJ_ROOT)/../davros-3

PRJ_NAME		?= loader
BIN_NAME		?= hello.bin
BIN_FILE		?= bin/hello.bin
LOAD_ADDR		?= 0x20000000

# Select your board directory here
DV_BOARD_D		?=	$(DV_ROOT)/board/arm64/raspberry-pi-3

# Select your compiler here
DV_COMPILER		?=	gnu
DV_GNU_D		?=	/data1/gnu/gcc-linaro-6.3.1-2017.02-x86_64_aarch64-elf

DV_LD_OBJS		+= $(DV_OBJ_D)/dv-reset.$(DV_O)
DV_LD_OBJS		+= $(DV_OBJ_D)/loadhigh.$(DV_O)
#DV_LD_OBJS		+= $(DV_OBJ_D)/dv-arm-bcm2835-uart.$(DV_O)
#DV_LD_OBJS		+= $(DV_OBJ_D)/dv-arm-bcm2835-gpio.$(DV_O)
DV_LD_OBJS		+= $(DV_OBJ_D)/dv-kprintf.$(DV_O)
DV_LD_OBJS		+= $(DV_OBJ_D)/dv-xprintf.$(DV_O)
DV_LD_OBJS		+= $(DV_OBJ_D)/dv-strlen.$(DV_O)
DV_LD_OBJS		+= $(DV_OBJ_D)/loadbin.$(DV_O)

HELLO_OBJS		+= $(DV_OBJ_D)/hello.$(DV_O)
HELLO_OBJS		+= $(DV_OBJ_D)/dv-kprintf.$(DV_O)
HELLO_OBJS		+= $(DV_OBJ_D)/dv-xprintf.$(DV_O)
HELLO_OBJS		+= $(DV_OBJ_D)/dv-strlen.$(DV_O)
HELLO_OBJS		+= $(DV_OBJ_D)/dv-arm-bcm2835-uart.$(DV_O)
HELLO_OBJS		+= $(DV_OBJ_D)/dv-arm-bcm2835-gpio.$(DV_O)

# Include the board's file lists
include $(DV_BOARD_D)/make/dv-board-list.make

#CC_OPT		+= -O2
LD_OPT		+= -e dv_reset
LD_OPT		+= -T project/ldscript/loadhigh.ldscript

HELLO_LD_OPT	+= -e hello
HELLO_LD_OPT	+= -T project/ldscript/hello.ldscript

VPATH 		+= project/c
VPATH		+= bin

DV_KLIB	= $(DV_LIB_D)/lib$(DV_KLIB_NAME).$(DV_A)
DV_ULIB = $(DV_LIB_D)/lib$(DV_ULIB_NAME).$(DV_A)

# Include the board's make selections
include $(DV_BOARD_D)/make/dv-board.make

include $(DV_ROOT)/make/dv-defs.make

.PHONY:		default all help klib ulib clean

default:	all

all:		image

shiny:
	-rm bin/*
	-rm bin/obj/*

klib:		$(DV_KLIB)

ulib:		$(DV_ULIB)

image:		$(DV_BIN_D)/$(PRJ_NAME).bin

$(DV_BIN_D)/$(PRJ_NAME).bin:	 $(DV_BIN_D)/$(PRJ_NAME).elf
	$(dv_elf2bin)

$(DV_BIN_D)/$(PRJ_NAME).elf:	$(DV_LD_OBJS)
	$(DV_GNU_D)/bin/$(DV_GNU_PRE)ld -o $@ $(DV_LD_OBJS) $(LD_OPT)

$(DV_BIN_D)/loadbin.c:		$(BIN_FILE)
	echo "const char bin_name[] = \"$(BIN_NAME)\";" > $@
	echo "const unsigned char bin_array[] = {"  >> $@
	hexdump -e '16/1 "0x%02X, ""\n"""' $(BIN_FILE) | sed -e 's/, 0x .*$///' >> $@
	echo "};" >> $@
	echo "const unsigned bin_length = sizeof(bin_array);" >> $@
	echo "const unsigned bin_loadaddr = $(LOAD_ADDR);" >> $@

$(DV_BIN_D)/hello.bin:	 	$(DV_BIN_D)/hello.elf
	$(dv_elf2bin)

$(DV_BIN_D)/hello.elf:		$(HELLO_OBJS)
	$(DV_GNU_D)/bin/$(DV_GNU_PRE)ld -o $@ $(HELLO_OBJS) $(HELLO_LD_OPT)

include $(DV_ROOT)/make/dv-commontargets.make
include $(DV_ROOT)/cpufamily/$(DV_FAMILY)/make/dv-$(DV_FAMILY)-$(DV_COMPILER).make
include $(DV_ROOT)/make/dv-rules.make
include $(DV_ROOT)/make/dv-klib.make
include $(DV_ROOT)/make/dv-ulib.make

# cat bin/hello.bin | hexdump -e '16/1 "0x%02X, ""\n"""' | sed -e 's/, 0x .*$//'
