#	Makefile - top level Makefile for davros
#
#	Copyright 2015 David Haworth
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
DV_ROOT		?= $(shell pwd)

# Select your board directory here
DV_BOARD_D 	?=	board/arm/raspberry-pi
#DV_BOARD_D 	?=	board/arm/qemu

# Select your compiler here
DV_COMPILER	?=	gnu
#DV_GNU_D	?=	/data1/gnu/gcc-linaro-6.3.1-2017.02-x86_64_aarch64-elf
DV_GNU_D	?=	/data1/gnu/gcc-linaro-6.3.1-2017.02-x86_64_arm-eabi

DV_LD_OBJS	+= $(DV_OBJ_D)/dv-vectors.$(DV_O)
DV_LD_OBJS	+= $(DV_OBJ_D)/dv-c0.$(DV_O)
DV_LD_OBJS	+= $(DV_OBJ_D)/dv-dummy.$(DV_O)
DV_LD_OBJS	+= $(DV_OBJ_D)/dv-prj-trace.$(DV_O)

# Include the board's file lists
include $(DV_BOARD_D)/make/dv-board-list.make

LD_OPT		+= -e dv_trap_reset
LD_OPT		+= -T $(DV_BOARD_D)/ldscript/pi-zero.ldscript

VPATH 		+= project/c

DV_KLIB	= $(DV_LIB_D)/lib$(DV_KLIB_NAME).$(DV_A)
DV_ULIB = $(DV_LIB_D)/lib$(DV_ULIB_NAME).$(DV_A)

# Include the board's make selections
include $(DV_BOARD_D)/make/dv-board.make

include make/dv-defs.make

.PHONY:		default all help klib ulib clean

default:	all

all:		klib ulib bin

klib:		$(DV_KLIB)

ulib:		$(DV_ULIB)

bin:		$(DV_BIN_D)/davros.bin

$(DV_BIN_D)/davros.bin:	 $(DV_BIN_D)/davros.elf
	$(dv_elf2bin)

$(DV_BIN_D)/davros.elf:	$(DV_LD_OBJS) $(DV_KLIB) $(DV_ULIB)
	$(dv_ld)

install:	$(DV_BIN_D)/davros.bin
	cp $(DV_BIN_D)/davros.bin ../../raspberry-pi/usb-zero/msd/kernel.img


include make/dv-commontargets.make
include cpufamily/$(DV_FAMILY)/make/dv-$(DV_FAMILY)-$(DV_COMPILER).make
include make/dv-rules.make
include make/dv-klib.make
include make/dv-ulib.make
