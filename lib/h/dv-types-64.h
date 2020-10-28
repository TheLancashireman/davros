/*	dv-types-64.h - standard data types for 64-bit processors
 *
 *	Copyright David Haworth
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
#ifndef dv_types_64_h
#define dv_types_64_h	1

/* On 64-bit CPUs there are several programming models that give different sizes for variables.
 *
 * For davros, LP64 is assumed. int is 32-bit, long is 64-bit. The long long qualifier isn't needed.
*/

#define DV_I8_T				signed char
#define DV_I16_T			signed short
#define DV_I32_T			signed int
#define DV_I64_T			signed long

#define DV_U8_T				unsigned char
#define DV_U16_T			unsigned short
#define DV_U32_T			unsigned int
#define DV_U64_T			unsigned long

#ifndef DV_BOOLEAN_T
#define DV_BOOLEAN_T		int
#endif

#ifndef DV_ADDRESS_T
#define DV_ADDRESS_T		DV_U64_T
#endif

#ifndef DV_MACHINEWORD_T
#define DV_MACHINEWORD_T	DV_U64_T
#endif

#ifndef DV_SIZE_T
#define DV_SIZE_T			DV_U64_T
#endif

#ifndef DV_STACKWORD_T
#define DV_STACKWORD_T		DV_U64_T
#endif

#ifndef DV_SIZE_PTR
#define DV_SIZE_PTR			8
#endif

#ifndef DV_LONG_TO_INT
#define DV_LONG_TO_INT		0xffffffff
#endif

#endif
