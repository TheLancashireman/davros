/* pico.h -  compatibility file for tinyusb
 *
 * Copyright David Haworth
 *
 * This file is part of davros.
 *
 * davros is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * davros is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with davros.  If not, see <http://www.gnu.org/licenses/>.
 *
 *
 *
*/
#ifndef pico_h
#define pico_h

#include <dv-config.h>
#include <davroska.h>

#define static_assert(x, y) extern void *xxxxxxxx
#define assert(x)			do {} while (0)
#define hard_assert(x)		do {} while (0)
#define _REG_(x)

#define REG_ALIAS_SET_BITS				(0x2u << 12u)
#define REG_ALIAS_CLR_BITS				(0x3u << 12u)
#define hw_alias_check_addr(addr)		((dv_u32_t)(addr))
#define hw_set_alias_untyped(addr)		((void *)(REG_ALIAS_SET_BITS | hw_alias_check_addr(addr)))
#define hw_set_alias(p)					((typeof(p))hw_set_alias_untyped(p))
#define hw_clear_alias_untyped(addr)	((void *)(REG_ALIAS_CLR_BITS | hw_alias_check_addr(addr)))
#define hw_clear_alias(p)				((typeof(p))hw_clear_alias_untyped(p))

typedef volatile dv_u32_t io_rw_32;
typedef volatile dv_u32_t io_wo_32;
typedef const volatile dv_u32_t io_ro_32;
typedef dv_u32_t uint;

static inline void hw_set_bits(io_rw_32 *addr, dv_u32_t mask)
{
    *(io_rw_32 *) hw_set_alias_untyped((volatile void *) addr) = mask;
}

static inline void hw_clear_bits(io_rw_32 *addr, dv_u32_t mask)
{
    *(io_rw_32 *) hw_clear_alias_untyped((volatile void *) addr) = mask;
}

#define tight_loop_contents()			do {} while (0)

#define panic(...) \
do {						\
	dv_printf(__VA_ARGS__);	\
	for (;;) { }			\
} while (0)

#define _u(x)				(x)
#define USBCTRL_DPRAM_BASE	0x50100000
#define USBCTRL_REGS_BASE	0x50110000
#define RESETS_BASE			0x4000c000

/* ToDo: map to davroska/davros functions?
*/
#define irq_set_enabled(irq, en)			do {} while (0)
#define irq_set_exclusive_handler(irq, fn)	do {} while (0)

#endif
