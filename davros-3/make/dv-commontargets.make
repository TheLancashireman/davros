#	dv-commonargets.make - common make targets (clean, help etc.)
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
