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

CC_OPT	+= -std=c99
CC_OPT	+= -Wall
CC_OPT	+= -Wextra
CC_OPT	+= -Wno-unused-parameter
CC_OPT	+= -g
CC_OPT	+= -fno-common
CC_OPT	+= -fno-builtin
CC_OPT	+= -mcpu=$(DV_GNU_CPU)

AS_OPT	+= -mcpu=$(DV_GNU_CPU)

LD_OPT	+= -L $(DV_LIB_D)
LD_LIB	+= -ldavroskernel -ldavrosuser

# Make an object file from a C source file
ifndef dv_cc
define dv_cc
	$(DV_GNU_D)/bin/$(DV_GNU_PRE)gcc $(CC_OPT) $(CC_INC) -c -o $@ $<
endef
endif

# Make an object file from an assembler source file
ifndef dv_as
define dv_as
	$(DV_GNU_D)/bin/$(DV_GNU_PRE)gcc $(AS_OPT) $(AS_INC) -c -o $@ $<
endef
endif

# Make a library from a list of object files.
ifndef dv_ar
define dv_ar
	-rm $@
    $(DV_GNU_D)/bin/$(DV_GNU_PRE)ar -crus $@ $^
endef
endif

# Make an ELF binary from a list of object files.
ifndef dv_ld
define dv_ld
    $(DV_GNU_D)/bin/$(DV_GNU_PRE)ld -o $@ $(DV_LD_OBJS) $(LD_LIB) $(LD_OPT)
endef
endif

# Make a raw binary from an ELF file
ifndef dv_elf2bin
define dv_elf2bin
    $(DV_GNU_D)/bin/$(DV_GNU_PRE)objcopy $< -O binary $@
endef
endif

# Make a raw binary from an ELF file
ifndef dv_elf2srec
define dv_elf2srec
    $(DV_GNU_D)/bin/$(DV_GNU_PRE)objcopy $< -O srec --srec-forceS3 /dev/stdout | dos2unix | egrep -v '^S3..........00*..$$' > $@
endef
endif
