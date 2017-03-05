/*	dv-types.h - data types for davros
 *
 *	Copyright 2017 David Haworth
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
#ifndef dv_types_h
#define dv_types_h	1

#include DV_CPU_CHARACTERISTICS

typedef DV_I8_T				dv_i8_t;
typedef DV_I16_T			dv_i16_t;
typedef DV_I32_T			dv_i32_t;
typedef DV_I64_T			dv_i64_t;

typedef DV_U8_T				dv_uint8_t;
typedef DV_U16_T			dv_uint16_t;
typedef DV_U32_T			dv_uint32_t;
typedef DV_U64_T			dv_uint64_t;

typedef DV_ADDRESS_T		dv_address_t;
typedef DV_MACHINEWORD_T	dv_machineword_t;
typedef DV_SIZE_T			dv_size_t;
typedef DV_STACKWORD_T		dv_stackword_t;

#endif
