#	dv-rules.make - rules for making things
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

# Make an object file from a C source file
ifndef dv_cc
define dv_cc
	$(CC) $(CC_OPT) $(CC_O) $@ $<
endef
endif

$(DV_OBJ_D)/%.$(DV_O):	%.$(DV_C)
	$(dv_cc)

# Make an object file from an assembler source file
ifndef dv_as
define dv_as
	$(AS) $(AS_OPT) $(AS_O) $@ $<
endef
endif

$(DV_OBJ_D)/%.$(DV_O):	%.$(DV_S)
	$(dv_as)

# Make a library from a list of object files.
# No rule can be given for this
ifndef dv_ar
define dv_ar
    $(AR) $(AR_OPT) $@ $^
endef
endif

# Make a binary from a list of object files.
# No rule can be given for this
ifndef dv_ld
define dv_ld
    $(LD) $(LD_LIB) $@ $^ $(LD_OPT) $(LD_O)
endef
endif
