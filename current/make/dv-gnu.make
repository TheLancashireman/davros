#	dv-gnu.make - gnu compiler settings
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

CC_INC	?=	$(addprefix -I,$(DV_INCLUDE_PATH))
AS_INC	?=	$(addprefix -I,$(DV_INCLUDE_PATH))

CC_OPTS += -std=c99
CC_OPTS += -Wall
CC_OPTS += -Wextra
CC_OPTS += -g
CC_OPTS += -fcommon

# Make an object file from a C source file
ifndef dv_cc
define dv_cc
	$(DV_GNU_D)/bin/$(DV_GNU_PRE)gcc $(CC_OPTS) $(CC_INC) -c -o $@ $<
endef
endif

# Make an object file from an assembler source file
ifndef dv_as
define dv_as
	$(DV_GNU_D)/bin/$(DV_GNU_PRE)gcc $(AS_OPTS) $(AS_INC) -c -o $@ $<
endef
endif

# Make a library from a list of object files.
ifndef dv_ar
define dv_ar
	-rm $@
    $(DV_GNU_D)/bin/$(DV_GNU_PRE)ar -crus $@ $^
endef
endif

# Make a binary from a list of object files.
ifndef dv_ld
define dv_ld
    $(DV_GNU_D)/bin/$(DV_GNU_PRE)ld $(LD_LIB) $(LD_OPT) -o $@ $^
endef
endif
