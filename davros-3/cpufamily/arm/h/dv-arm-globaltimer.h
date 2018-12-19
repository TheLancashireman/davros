/*	dv-arm-globaltimer.h - ARM global timer header file for davros
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
#ifndef dv_arm_globaltimer_h
#define dv_arm_globaltimer_h	1

#include <dv-types.h>
#include <cpufamily/arm/h/dv-arm-configbase.h>

#if !DV_ASM

typedef struct dv_arm_globaltimer_s dv_arm_globaltimer_t;

struct dv_arm_globaltimer_s
{	dv_reg32_t	timer_lo;
	dv_reg32_t	timer_hi;
	dv_u32_t	ctrl;
	dv_reg32_t	status;
	dv_u32_t	compare_lo;
	dv_u32_t	compare_hi;
	dv_u32_t	compare_increment;
};

#define DV_GT_TEN	0x0001	/* ctrl: Timer enable */
#define DV_GT_CEN	0x0002	/* ctrl: Comparator enable */
#define DV_GT_IEN	0x0004	/* ctrl: IRQ enable */
#define DV_GT_INC	0x0008	/* ctrl: Auto-increment enable */
#define DV_GT_PRE	0xff00	/* ctrl: Prescaler */

#define DV_GT_IRQ	0x0001	/* status: irq occurred. Write 1 to clear */

#define DV_GT_IRQID	27		/* Interrupt index in GIC */

static inline dv_u64_t dv_readtime(void)
{
	dv_arm_globaltimer_t *gtimer = dv_get_config_base(DV_GTIMER_OFFSET);
	dv_u32_t h, l, x;

	x = gtimer->timer_hi;
	do {
		h = x;
		l = gtimer->timer_lo;
		x = gtimer->timer_hi;
	} while ( h != x );
	return (((dv_u64_t)h) << 32) + l;
}

/* Interrupt handler function
*/
void dv_gtimer_interrupt(dv_kernel_t *, unsigned);

#endif

#endif
