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

# Select your compiler here
DV_COMPILER	?=	gnu
DV_GNU_D	?=	/usr/local/gcc

# Include the board Make selections
include $(DV_BOARD_D)/dv-board.make

include make/dv-defs.make

.PHONY:		default all help klib ulib clean

default:	help

all:		klib

klib:		$(DV_LIB_D)/lib$(DV_KLIB_NAME).$(DV_A)

uib:		$(DV_LIB_D)/lib$(DV_ULIB_NAME).$(DV_A)

clean:
	-rm	$(DV_LIB_D)/*.$(DV_A)
	-rm	$(DV_OBJ_D)/*.$(DV_O)
	

help:
	@echo ""
	@echo "Make targets:"
	@echo "  default:         show this message"
	@echo "  all:             make everything"
	@echo "  klib:            make the kernel library"
	@echo "  ulib:            make the userland library"
	@echo ""
	@echo "Settings:"
	@echo "  DV_ROOT = $(DV_ROOT)"
	@echo "  DV_BOARD_D = $(DV_BOARD_D)"
	@echo "  DV_O = $(DV_O)"
	@echo "  DV_C = $(DV_C)"
	@echo "  DV_S = $(DV_S)"
	@echo "  DV_A = $(DV_A)"
	@echo "  DV_KLIB_NAME = $(DV_KLIB_NAME)"
	@echo "  DV_ULIB_NAME = $(DV_ULIB_NAME)"
	@echo "  DV_OBJ_D = $(DV_OBJ_D)"
	@echo "  DV_LIB_D = $(DV_LIB_D)"
	@echo "  DV_KSRC_D = $(DV_KSRC_D)"
	@echo "  DV_USRC_D = $(DV_USRC_D)"
	@echo ""

include make/$(DV_FAMILY)/dv-$(DV_FAMILY)-$(DV_COMPILER).make
include make/dv-rules.make
include make/dv-klib.make
