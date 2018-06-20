/*  dv-arm-bcm2835-systimer.h - system timer on bcm2835 (raspberry pi)
 *
 *  Copyright 2017 David Haworth
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
#ifndef dv_arm_bcm2835_systimer_h
#define dv_arm_bcm2835_systimer_h	1

#include <kernel/h/dv-kconfig.h>
#include <kernel/h/dv-types.h>

#ifndef DV_PBASE
#error	"No definition of DV_PBASE in the board headers. Please fix!"
#endif

/* It looks like the GPU is using compare registers 0 and 2, so only 1 and 3 are available for the ARM
*/

typedef struct dv_arm_bcm2835_systimer_s dv_arm_bcm2835_systimer_t;

struct dv_arm_bcm2835_systimer_s
{
	dv_reg32_t cs;		/* Status. Write 1 to clear a bit. */
	dv_reg32_t clo;		/* Counter (low) */
	dv_reg32_t chi;		/* Counter (high) */
	dv_reg32_t c[4];	/* Compare 0..3  */
};

#define dv_arm_bcm2835_systimer	((dv_arm_bcm2835_systimer_t *)(DV_PBASE+0x003000))[0]

static inline dv_u64_t dv_readtime(void)
{
	dv_u32_t u1, u2, l;

	u2 = dv_arm_bcm2835_systimer.chi;
	do {
		u1 = u2;
		l = dv_arm_bcm2835_systimer.clo;
		u2 = dv_arm_bcm2835_systimer.chi;
	} while ( u1 != u2 );

	return (((dv_u64_t)u2) << 32) + (dv_u64_t)l;
}

static inline dv_u32_t dv_getcmp(int i)
{
	return dv_arm_bcm2835_systimer.c[i];
}

static inline void dv_setcmp(int i, dv_u32_t v)
{
	dv_arm_bcm2835_systimer.c[i] = v;
}

static inline int dv_getmatch(int i)
{
	return (dv_arm_bcm2835_systimer.cs & (1<<i)) != 0;
}

static inline void dv_clrmatch(int i)
{
	dv_arm_bcm2835_systimer.cs = (1<<i);
}

void dv_arm_bcm2835_systimerinterrupt(dv_kernel_t *kvars, dv_address_t param);
void dv_init_systimer(dv_kernel_t *kvars);
void dv_set_system_timer_alarm(dv_u64_t);

/* Bits in cs register. 1 means "matched".
 * Write 1 to clear.
*/
#define DV_CS_M0	0x01
#define DV_CS_M1	0x02
#define DV_CS_M2	0x04
#define DV_CS_M3	0x08

/* Minimum sleep time. Any time difference less than this is considered zero
 * for the purpose of the sleep queue.
*/
#define DV_MIN_SLEEP	10

#endif
