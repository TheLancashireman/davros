#	dv-defs.make - general make definitions for davros
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

# Default file suffixes: object (.o), c source (.c), assembler source (.s) library (.a)
DV_O		?= o
DV_C		?= c
DV_S		?= S
DV_A		?= a

# The name of the kernel library (stuff that hides behind the system call)
# Without the lib prefix and .a suffix.
DV_KLIB_NAME	?=	davroskernel

# The name of the userland library (stuff that runs in threads)
# Without the lib prefix and .a suffix.
DV_ULIB_NAME	?=	davrosuser

# Standard directories
# The top level makefile defines DV_BOARD_D.
DV_OBJ_D	?= bin/obj
DV_LIB_D	?= bin
DV_BIN_D	?= bin

DV_CPUFAM_D	?= $(DV_ROOT)/cpufamily/$(DV_FAMILY)
DV_CPUFAM_D	?= $(DV_ROOT)/cpufamily/$(DV_FAMILY)

DV_KERNEL_D	?= $(DV_ROOT)/kernel
DV_USER_D	?= $(DV_ROOT)/user

# Add source directories to the search path
VPATH += $(DV_BOARD_D)/c
VPATH += $(DV_BOARD_D)/s
VPATH += $(DV_CPUFAM_D)/c
VPATH += $(DV_CPUFAM_D)/s
VPATH += $(DV_KERNEL_D)/c
VPATH += $(DV_USER_D)/c

# Files to put into the include path
DV_INCLUDE_PATH	+= $(DV_BOARD_D)/h
DV_INCLUDE_PATH	+= $(DV_ROOT)
