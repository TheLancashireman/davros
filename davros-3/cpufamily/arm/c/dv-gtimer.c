/*	dv-gtimer.c - global timer handling for davros
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
#include <kernel/h/dv-kconfig.h>
#include <kernel/h/dv-types.h>
#include <kernel/h/dv-kernel-types.h>
#include <kernel/h/dv-kernel.h>
#include <cpufamily/arm/h/dv-arm-globaltimer.h>
#include <kernel/h/dv-coverage.h>

/* dv_gtimer_interrupt() - handle an interrupt from the global timer
 *
 * The global timer is used to provide davros time features such as sleep etc.
*/
void dv_gtimer_interrupt(dv_kernel_t *kvars, unsigned unused_p)
{
	dv_arm_globaltimer_t *gt = dv_get_config_base(DV_GTIMER_OFFSET);
	dv_u64_t t;

	/* Acknowledge the timer event.
	*/
	gt->status = DV_GT_IRQ;

	t = dv_readtime();
	dv_kprintf("dv_gtimer_interrupt: core %d, t = 0x%08x%08x\n", kvars->core_index, (unsigned)(t>>32), (unsigned)t);
}

/* man-page-generation - to be defined
*/
