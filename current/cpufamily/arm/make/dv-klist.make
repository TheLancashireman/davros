#	dv-klist.make - davros kernel library files for ARM
#
#	Copyright 2017 David Haworth
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

# DV_KLIST_C is a list of all the C source files that are in the kernel library.
# Without the .c suffix.
DV_KLIST_C	+= dv-initvectors

# DV_KLIST_S is a list of all the assembler source files that are in the kernel library.
# Without the .s suffix.
DV_KLIST_S	+= dv-vectors
DV_KLIST_S	+= dv-reset
DV_KLIST_S	+= dv-sbreak
DV_KLIST_S	+= dv-resume
DV_KLIST_S	+= dv-exit
DV_KLIST_S	+= dv-nullsc
DV_KLIST_S	+= dv-setkvars
