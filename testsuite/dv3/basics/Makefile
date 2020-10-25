#	Makefile - for dv3 "basics" test
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

TEST_NAME		:= basics

TEST_D			?= $(shell pwd)
TEST_COMMON_D	?= $(shell (cd ../common; pwd))

# Select your hardware here
#DV_BOARD	?= blue-pill
#DV_BOARD	?= pi3-arm64
DV_BOARD	?= pi-zero

VPATH	+= c

DV_LD_OBJS	+= $(addprefix $(DV_OBJ_D)/,$(addsuffix .o,$(basename $(notdir $(wildcard c/*.c)))))

include $(TEST_COMMON_D)/make/d3t.make

