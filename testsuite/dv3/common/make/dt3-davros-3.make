#	dt3-davros-3.make - makefile fragment for building davros-3 libraries
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

# Where to find the davros-3 source files
VPATH 	+= $(DV3_ROOT)/kernel/c
VPATH 	+= $(DV_ROOT)/devices/c
VPATH 	+= $(DV_ROOT)/devices/s
VPATH 	+= $(DV_ROOT)/lib/c

# Libraries to build
DV_LIBS		+=	$(DV_LIB_D)/libdavros3.a

# Object files to put into the libraries; everything from kernel/c and lib/c
DV_LIB_OBJS	+= $(addprefix $(DV_OBJ_D)/,$(addsuffix .o,$(basename $(notdir $(wildcard $(DV3_ROOT)/kernel/c/*.c)))))
DV_LIB_OBJS	+= $(addprefix $(DV_OBJ_D)/,$(addsuffix .o,$(basename $(notdir $(wildcard $(DV_ROOT)/lib/c/*.c)))))
