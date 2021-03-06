/*  dv-usb-util.h
 *
 *  Copyright David Haworth
 *
 *  This file is part of davros.
 *
 *  davros is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  davros is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with davros.  If not, see <http://www.gnu.org/licenses/>.
*/
#ifndef DV_USB_UTIL_H
#define DV_USB_UTIL_H

/* Data access
 * ===========
 *
 * To avoid endianness and alignment problems, all USB descriptors and packets are defined as arrays of bytes.
 * Storing a multi-byte value into consecutive bytes in the array is done in two ways
 * 	- a macro for for initialising a constant or variable at compile time
 *	- a function for storing into a variable at runtime
 *
 * dv_usb_init_8()		- used for initializing one byte. Expands to a c99-style initialiser
 * dv_usb_init_16()		- used for initializing two consecutive bytes. Expands to two c99-style initialisers
 * dv_usb_init_8_c90()	- used for initializing one byte. Expands to a c90-style initialiser
 * dv_usb_init_16_c90()	- used for initializing two consecutive bytes. Expands to two c90-style initialisers
 * dv_usb_store_16()	- used for storing into two consecutive bytes at runtime
 * dv_usb_load_16()		- used for loading from two consecutive bytes at runtime
*/
#define dv_usb_init_8(b, x)		[b] = (x)
#define dv_usb_init_16(b, x)	[b]   = (((dv_u16_t)(x)) & 0xff), \
								[b+1] = ((((dv_u16_t)(x)) >> 8) & 0xff)
#define dv_usb_init_32(b, x)	[b]   = (((dv_u32_t)(x)) & 0xff), \
								[b+1] = ((((dv_u32_t)(x)) >> 8) & 0xff), \
								[b+2] = ((((dv_u32_t)(x)) >> 16) & 0xff), \
								[b+3] = ((((dv_u32_t)(x)) >> 24) & 0xff)

#define dv_usb_init_8_c90(x)	(x)
#define dv_usb_init_16_c90(x)	(((dv_u16_t)(x)) & 0xff), \
								((((dv_u16_t)(x)) >> 8) & 0xff)
#define dv_usb_init_32_c90(x)	(((dv_u32_t)(x)) & 0xff), \
								((((dv_u32_t)(x)) >> 8) & 0xff), \
								((((dv_u32_t)(x)) >> 16) & 0xff), \
								((((dv_u32_t)(x)) >> 24) & 0xff)

static inline void dv_usb_store_16(dv_u8_t *b0, dv_u16_t w)
{
	b0[0] = w & 0xff;
	b0[1] = (w >> 8) & 0xff;
}

static inline dv_u16_t dv_usb_load_16(const dv_u8_t *b0)
{
	return (((dv_u16_t)b0[1]) << 8) | (dv_u16_t)b0[0];
}


#endif
