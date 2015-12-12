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

default:	all

all:		klib

klib:		$(DV_LIB_D)/lib$(DV_KLIB_NAME).$(DV_A)

uib:		$(DV_LIB_D)/lib$(DV_ULIB_NAME).$(DV_A)

include make/dv-commontargets.make
include make/$(DV_FAMILY)/dv-$(DV_FAMILY)-$(DV_COMPILER).make
include make/dv-rules.make
include make/dv-klib.make
