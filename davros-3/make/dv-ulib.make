#	dv-ulib.make - makefile for davros kernel library
#
#	Copyright David Haworth
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

include $(DV_ROOT)/make/dv-ulist.make

DV_ULIB_O 	=	$(addsuffix .$(DV_O),$(DV_ULIST_C) $(DV_ULIST_S))
DV_ULIB_DO	=	$(addprefix $(DV_OBJ_D)/,$(DV_ULIB_O))

$(DV_LIB_D)/lib$(DV_ULIB_NAME).$(DV_A):	$(DV_ULIB_DO)
	$(dv_ar)
