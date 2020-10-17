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
#		make [DV_BOARD=pi3-arm64|pi-zero] [DV_GNU_D=</path/to/gcc>] [DV_IMAGE=</place/to/install/file.img>]
#	Alternatively, you can set DV_BOARD DV_GNU_D and IMAGE_FILE as environment variables.
#
#	Targets:
#		clean: removes all object and binary files
#		default: compiles and links
#		install: objcopy the ELF file to a binary (img) file DV_IMAGE
#		srec: objcopy the ELF to an S-record file in the bin directory

PRJ_ROOT	?= $(shell pwd)
DV_ROOT		?= $(shell (cd $(PRJ_ROOT)/../../ ; pwd))
DV3_ROOT	?= $(DV_ROOT)/davros-3

# Select your hardware here
#DV_BOARD	?= blue-pill
#DV_BOARD	?= pi3-arm64
DV_BOARD	?= pi-zero

DV_BIN_D	= bin
DV_OBJ_D	= o
DV_LIB_D	= a

DV_ELF		?= $(DV_BIN_D)/dv3-demo.elf
DV_SREC		?= $(DV_ELF:.elf=.srec)

include make/$(DV_BOARD).make
include make/davros-3.make

# Standard compiler options
CC_OPT		+= -D DEMO_H_BOARDCFG="$(DEMO_H_BOARDCFG)"
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

# Standard linker options
LD_OPT		+= -e $(DV_ENTRY)
LD_OPT		+= -T $(DV_LDSCRIPT)
LD_OPT		+= -L $(DV_LDLIB_D)
LD_OPT		+= -L $(DV_LIB_D)
LD_OPT		+= -lc

# The demo code
VPATH	+= c

DV_LD_OBJS	+= $(DV_OBJ_D)/demo.o
DV_LD_OBJS	+= $(DV_OBJ_D)/demo-errorhandling.o

.PHONY:		default elf libs srec image dirs clean help

default:	elf

clean:
	-rm -rf $(DV_OBJ_D) $(DV_BIN_D) $(DV_LIB_D)

dirs:	$(DV_BIN_D) $(DV_LIB_D) $(DV_OBJ_D)

elf:	dirs $(DV_ELF)

libs:	dirs $(DV_LIBS)

image:	dirs $(DV_IMAGE)

srec:	dirs $(DV_SREC)


$(DV_ELF):	$(DV_BIN_D) $(DV_LD_OBJS) libs
	$(DV_LD) -o $@ $(DV_LD_OBJS)  -ldavros3 $(LD_OPT)

$(DV_LIB_D)/libdavros3.a:	$(DV_LIB_D) $(DV_LIB_OBJS) $(DV_USR_OBJS)
	-rm $@
	$(DV_AR) cqs $@ $(DV_LIB_OBJS) $(DV_USR_OBJS)
	
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

$(DV_IMAGE):$(DV_ELF)
	$(DV_OBJCOPY) $(DV_ELF) -O binary $(DV_IMAGE)

$(DV_SREC):	$(DV_ELF)
	$(DV_OBJCOPY) $< -O srec --srec-forceS3 /dev/stdout | dos2unix | egrep -v '^S3..........00*..$$' > $@
