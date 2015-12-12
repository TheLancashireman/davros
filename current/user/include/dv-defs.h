/*	dv-defs.h - definitions (families, cpus etc.) for davros
 *
 *	Copyright 2015 David Haworth
 *
 *	This file is part of davros.
 *
 *	davros is free software: you can redistribute it and/or modify
 *	it under the terms of the GNU General Public License as published by
 *	the Free Software Foundation, either version 3 of the License, or
 *	(at your option) any later version.
 *
 *	davros is distributed in the hope that it will be useful,
 *	but WITHOUT ANY WARRANTY; without even the implied warranty of
 *	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *	GNU General Public License for more details.
 *
 *	You should have received a copy of the GNU General Public License
 *	along with davros.  If not, see <http://www.gnu.org/licenses/>.
*/
#ifndef dv_defs_h
#define dv_defs_h	1

/* Macros to specify the CPU family. DV_FAMILY is defined to be one of these.
*/
#define DV_FAMILY_ARM		1
#define DV_FAMILY_ARM64		2
#define DV_FAMILY_MIPS		3
#define DV_FAMILY_TRICORE	4

/* Macros to specify the compiler. DV_COMPILER is defined to be one of these.
*/
#define DV_COMPILER_GNU		1
#define DV_COMPILER_GHS		2	/* Green Hills */
#define DV_COMPILER_WR		3	/* Wind River (diab) */

/* Macros to define header file selection. DV_HEADERSEL is defined to be one of these.
*/
#define DV_HEADERSEL_ARM	<user/include/arm/dv-arm-headers.h>

#endif
