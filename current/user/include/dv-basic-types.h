/*	dv-basic-types.h - basic data types for davros
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
#ifndef dv_basic_types_h
#define dv_basic_types_h	1

/* This file defines the basic data types for davros
 *
 * The types are defined using macros to permit the processor families to define their own
 * types if necessary.
 *
 * The macros will be used in dv-typedefs.h to define C data types.
*/

/* Natural data types (useful for some purposes)
*/
#ifndef DV_CHAR_T
#define DV_CHAR_T				char
#endif

#ifndef DV_SHORT_T
#define DV_SHORT_T				short
#endif

#ifndef DV_INT_T
#define DV_INT_T				int
#endif

#ifndef DV_LONG_T
#define DV_LONG_T				long
#endif

#ifndef DV_LLONG_T
#define DV_LLONG_T				long long
#endif

#ifndef DV_UCHAR_T
#define DV_UCHAR_T				unsigned char
#endif

#ifndef DV_USHORT_T
#define DV_USHORT_T				unsigned short
#endif

#ifndef DV_UNSIGNED_T
#define DV_UNSIGNED_T			unsigned
#endif

#ifndef DV_ULONG_T
#define DV_ULONG_T				unsigned long
#endif

#ifndef DV_ULLONG_T
#define DV_ULLONG_T				unsigned long long
#endif


/* Signed scalar types of fixed size.
*/
#ifndef DV_INT8_T
#define DV_INT8_T				char
#endif

#ifndef DV_INT16_T
#define DV_INT16_T				short
#endif

#ifndef DV_INT32_T
#define DV_INT32_T				long
#endif

#ifndef DV_INT64_T
#define DV_INT64_T				long long
#endif


/* Unsigned scalar types of fixed size.
*/
#ifndef DV_UINT8_T
#define DV_UINT8_T				unsigned char
#endif

#ifndef DV_UINT16_T
#define DV_UINT16_T				unsigned short
#endif

#ifndef DV_UINT32_T
#define DV_UINT32_T				unsigned long
#endif

#ifndef DV_UINT64_T
#define DV_UINT64_T				unsigned long long
#endif

/* Fastest signed scalar types of at least given size.
*/
#ifndef DV_FINT8_T
#define DV_FINT8_T				DV_INT_T
#endif

#ifndef DV_FINT16_T
#define DV_FINT16_T				DV_INT_T
#endif

#ifndef DV_FINT32_T
/* FIXME - word-length dependent */
#define DV_FINT32_T				DV_LONG_T
#endif

#ifndef DV_FINT64_T
#define DV_FINT64_T				DV_LLONG_T
#endif


/* Fastest unsigned scalar types of at least given size.
*/
#ifndef DV_FUINT8_T
#define DV_FUINT8_T				DV_UNSIGNED_T
#endif

#ifndef DV_FUINT16_T
#define DV_FUINT16_T			DV_UNSIGNED_T
#endif

#ifndef DV_FUINT32_T
/* FIXME - word-length dependent */
#define DV_FUINT32_T			DV_ULONG_T
#endif

#ifndef DV_FUINT64_T
#define DV_FUINT64_T			DV_ULLONG_T
#endif

/* Memory-mapped hardware registers.
*/
#ifndef DV_REG8_T
#define DV_REG8_T				volatile dv_uint8_t
#endif

#ifndef DV_REG16_T
#define DV_REG16_T				volatile dv_uint16_t
#endif

#ifndef DV_REG32_T
#define DV_REG32_T				volatile dv_uint32_t
#endif


/* Memory sizes and addresses
*/
#ifndef DV_SIZE_T
#define DV_SIZE_T				dv_uint32_t
#endif

#ifndef DV_ADDRESS_T
#define DV_ADDRESS_T			dv_uint32_t
#endif


/* Stack elements
*/
#ifndef DV_STACKELEMENT_T

#ifndef DV_SIZEOF_STACKELEMENT
#define DV_SIZEOF_STACKELEMENT	4
#endif

#if		DV_SIZEOF_STACKELEMENT == 1
#define DV_STACKELEMENT_T		dv_uint8_t
#elif	DV_SIZEOF_STACKELEMENT == 2
#define DV_STACKELEMENT_T		dv_uint16_t
#elif	DV_SIZEOF_STACKELEMENT == 4
#define DV_STACKELEMENT_T		dv_uint32_t
#elif	DV_SIZEOF_STACKELEMENT == 8
#define DV_STACKELEMENT_T		dv_uint64_t
#else
#error "Unsupported value of DV_SIZEOF_STACKELEMENT. Define your own DV_STACKELEMENT_T"
#endif

#endif

/* Boolean
*/
#ifndef DV_BOOLEAN_T
#define	DV_BOOLEAN_T		dv_int_t
#endif

/* Status
*/
#ifndef DV_STATUS_T
#define	DV_STATUS_T			dv_int_t
#endif


/* Below here we define the C data types
 *
 * The types are defined using the macros defined above and/or in the CPU files
*/
#ifndef DV_ASM

typedef DV_CHAR_T			dv_char_t;
typedef DV_SHORT_T			dv_short_t;
typedef DV_INT_T			dv_int_t;
typedef DV_LONG_T			dv_long_t;
typedef DV_LLONG_T			dv_llong_t;

typedef DV_UCHAR_T			dv_uchar_t;
typedef DV_USHORT_T			dv_ushort_t;
typedef DV_UNSIGNED_T		dv_unsigned_t;
typedef DV_ULONG_T			dv_ulong_t;
typedef DV_ULLONG_T			dv_ullong_t;

typedef DV_INT8_T			dv_int8_t;
typedef DV_INT16_T			dv_int16_t;
typedef DV_INT32_T			dv_int32_t;
typedef DV_INT64_T			dv_int64_t;

typedef DV_UINT8_T			dv_uint8_t;
typedef DV_UINT16_T			dv_uint16_t;
typedef DV_UINT32_T			dv_uint32_t;
typedef DV_UINT64_T			dv_uint64_t;

typedef DV_FINT8_T			dv_fint8_t;
typedef DV_FINT16_T			dv_fint16_t;
typedef DV_FINT32_T			dv_fint32_t;
typedef DV_FINT64_T			dv_fint64_t;

typedef DV_FUINT8_T			dv_fuint8_t;
typedef DV_FUINT16_T		dv_fuint16_t;
typedef DV_FUINT32_T		dv_fuint32_t;
typedef DV_FUINT64_T		dv_fuint64_t;

typedef DV_REG8_T			dv_reg8_t;
typedef DV_REG16_T			dv_reg16_t;
typedef DV_REG32_T			dv_reg32_t;

typedef DV_SIZE_T			dv_size_t;
typedef DV_ADDRESS_T		dv_address_t;
typedef DV_STACKELEMENT_T	dv_stackelement_t;
typedef DV_BOOLEAN_T		dv_boolean_t;
typedef DV_STATUS_T			dv_status_t;

#endif

#endif
