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
DV_ROOT		= ../../davroska

# Select your compiler here
DV_COMPILER	?=	gnu
#DV_GNU_D	?=	/data1/gnu/gcc-linaro-6.3.1-2017.02-x86_64_aarch64-elf
#DV_GNU_D	?=	/data1/gnu/gcc-linaro-6.3.1-2017.02-x86_64_arm-eabi

DV_BIN_D	= bin
DV_OBJ_D	= obj

CC_OPT		+= -I h
CC_OPT		+= -I $(DV_ROOT)/h
CC_OPT		+= -Wall

DV_LD_OBJS	+= $(DV_OBJ_D)/demo.o
DV_LD_OBJS	+= $(DV_OBJ_D)/davroska.o

#CC_OPT		+= -O2

VPATH 		+=	c
VPATH		+=	$(DV_ROOT)/c

.PHONY:		default all help clean

default:	all

clean:
	-rm -rf $(DV_OBJ_D) $(DV_BIN_D)

all:		$(DV_OBJ_D) $(DV_BIN_D) elf

elf:		$(DV_BIN_D)/davroska.elf

$(DV_BIN_D)/davroska.elf:	$(DV_LD_OBJS)
	gcc -o $@ $(DV_LD_OBJS) $(LD_LIB) $(LD_OPT)

$(DV_OBJ_D)/%.o:  %.c
	gcc $(CC_OPT) -o $@ -c $<

$(DV_BIN_D):
	mkdir -p bin

$(DV_OBJ_D):
	mkdir -p obj
