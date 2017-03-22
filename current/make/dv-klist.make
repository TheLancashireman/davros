#	dv-klist.make - davros kernel library files
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

# DV_KLIST_C is a list of all the C source files that are in the kernel library.
# Without the .c suffix.
DV_KLIST_C	+=	dv-allocatethread
DV_KLIST_C	+=	dv-allocateobj
DV_KLIST_C	+=	dv-allocateregisters
DV_KLIST_C	+=	dv-allocatestack
DV_KLIST_C	+=	dv-cores
DV_KLIST_C	+=	dv-createexecutable
DV_KLIST_C	+=	dv-dispatch
DV_KLIST_C	+=	dv-dllinit
DV_KLIST_C	+=	dv-dllinsertaftersame
DV_KLIST_C	+=	dv-dllinsertbeforesame
DV_KLIST_C	+=	dv-initkvars
DV_KLIST_C	+=	dv-spawnexecutable
DV_KLIST_C	+=	dv-spawnexecutableinthread
DV_KLIST_C	+=	dv-start
DV_KLIST_C	+=	dv-syscall

DV_KLIST_C	+=	dv-c0

DV_KLIST_C	+=	dv-xprintf
DV_KLIST_C	+=	dv-kprintf
DV_KLIST_C	+=	dv-strlen

include cpufamily/$(DV_FAMILY)/make/dv-klist.make
