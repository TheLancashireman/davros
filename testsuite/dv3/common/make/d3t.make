#	d3t.make - common makefile fragment for davros-3 test suite
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

DV_ROOT		?= $(shell (cd $(TEST_COMMON_D)/../../../ ; pwd))
DV3_ROOT	?= $(DV_ROOT)/davros-3

DV_BIN_D	= bin
DV_OBJ_D	= o
DV_LIB_D	= a

DV_ELF		?= $(DV_BIN_D)/dv3-test-$(TEST_NAME).elf
DV_SREC		?= $(DV_ELF:.elf=.srec)

include $(TEST_COMMON_D)/make/d3t-$(DV_BOARD).make
include $(TEST_COMMON_D)/make/d3t-davros-3.make

VPATH			+= $(TEST_COMMON_D)/c

D3T_LIB_OBJS	+= $(DV_OBJ_D)/d3t-panic.o
D3T_LIB_OBJS	+= $(DV_OBJ_D)/d3t-init.o

# Standard compiler options
CC_OPT		+= -D D3T_H_BOARDCFG="$(D3T_H_BOARDCFG)"
CC_OPT		+= -I h
CC_OPT		+= -I $(TEST_COMMON_D)/h
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

.PHONY:		default elf libs srec image dirs clean help

default:	elf

clean:
	-rm -rf $(DV_OBJ_D) $(DV_BIN_D) $(DV_LIB_D)

dirs:	$(DV_BIN_D) $(DV_LIB_D) $(DV_OBJ_D)

elf:	dirs $(DV_ELF)

libs:	dirs $(DV_LIBS)

image:	dirs $(DV_IMAGE)

srec:	dirs $(DV_SREC)

$(DV_ELF):	$(DV_BIN_D) $(DV_LD_OBJS) $(DV_LIBS)
	$(DV_LD) -o $@ $(DV_LD_OBJS)  -ldavros3 $(LD_OPT)

$(DV_LIB_D)/libdavros3.a:	$(DV_LIB_D) $(DV_LIB_OBJS) $(DV_USR_OBJS) $(D3T_LIB_OBJS)
	-rm $@
	$(DV_AR) cqs $@ $(DV_LIB_OBJS) $(DV_USR_OBJS) $(D3T_LIB_OBJS)
	
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

ifneq ($(DV_IMAGE),)
$(DV_IMAGE):$(DV_ELF)
	$(DV_OBJCOPY) $(DV_ELF) -O binary $(DV_IMAGE)
endif

$(DV_SREC):	$(DV_ELF)
	$(DV_OBJCOPY) $< -O srec --srec-forceS3 /dev/stdout | dos2unix | egrep -v '^S3..........00*..$$' > $@
