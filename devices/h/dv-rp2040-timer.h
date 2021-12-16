/* dv-rp2040-timer.h
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
#ifndef DV_RP2040_TIMER_H
#define DV_RP2040_TIMER_H

#include <dv-devices.h>
#include "dv-rp2040.h"

#define DV_RP2040_TIMER_BASE		0x40054000

/* The armed and intx registers have bits for each alarm ( 1<<alarm_index )
 * The alarm registers are one-shot - they arm on write and automatically disarm in match with
 * time_l.
*/

typedef struct dv_rp2040_timer_s
{
	volatile dv_u32_t	time_hw;	/* 0x00 - write high word and transfer low word buffer */
	volatile dv_u32_t	time_lw;	/* 0x04 - write low word buffer */
	volatile dv_u32_t	time_hr;	/* 0x08 - read latched high word */
	volatile dv_u32_t	time_lr;	/* 0x0c - read low word and latch high word */
	volatile dv_u32_t	alarm[4];	/* 0x10 - alarm registers */
	volatile dv_u32_t	armed;		/* 0x20 - armed state of alarms */
	volatile dv_u32_t	time_hraw;	/* 0x24 - read high word */
	volatile dv_u32_t	time_lraw;	/* 0x28 - read low word */
	volatile dv_u32_t	dbgpause;	/* 0x2c - pause timer while debugging */
	volatile dv_u32_t	pause;		/* 0x30 - pause timer in software */
	volatile dv_u32_t	intr;		/* 0x34 - raw interrupt status  */
	volatile dv_u32_t	inte;		/* 0x38 - interrupt enable */
	volatile dv_u32_t	intf;		/* 0x3c - interrupt force  */
	volatile dv_u32_t	ints;		/* 0x40 - interrupt status after mask/force */
} dv_rp2040_timer_t;

#define dv_rp2040_timer		(((dv_rp2040_timer_t *)DV_RP2040_TIMER_BASE)[0])
#define dv_rp2040_timer_xor	(((dv_rp2040_timer_t *)(DV_RP2040_TIMER_BASE+DV_OFFSET_XOR))[0])
#define dv_rp2040_timer_w1s	(((dv_rp2040_timer_t *)(DV_RP2040_TIMER_BASE+DV_OFFSET_W1S))[0])
#define dv_rp2040_timer_w1c	(((dv_rp2040_timer_t *)(DV_RP2040_TIMER_BASE+DV_OFFSET_W1C))[0])

/* dv_rp2040_read_time() - read and return the timer value
 *
 * The time_hr/_lr registers suffer from the usual concurrency issues, so this function
 * uses a standard high-low-high algorithm reading the raw registers.
*/
static inline dv_u64_t dv_rp2040_read_time(void)
{
	dv_u32_t h1, h2, l;

	do {
		h1 = dv_rp2040_timer.time_hraw;
		l  = dv_rp2040_timer.time_lraw;
		h1 = dv_rp2040_timer.time_hraw;
	} while ( h1 != h2 );

	return (((dv_u64_t)h1) << 32) + ((dv_u64_t)l);
}

#endif
