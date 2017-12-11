/*	dv-inithardware.c - ARM hardware initialisation source file for davros
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
#include <kernel/h/dv-interrupt.h>
#include <cpufamily/arm/h/dv-arm-start.h>
#include <cpufamily/arm/h/dv-arm-gic.h>
#include <cpufamily/arm/h/dv-arm-globaltimer.h>
#include <kernel/h/dv-coverage.h>

/* dv_init_hardware() - initialise the processor etc.
*/
void dv_init_hardware(dv_kernel_t *unused_kvars)
{
#if 0	/* ToDo: select interrupt controller based on hardware */
	dv_init_gic();
#endif
}

/* dv_init_peripherals() - initisalise the peripherals used by davros.
 *
 * Modifying vectors for reserved and banked interrupts is done here too.
*/
void dv_init_peripherals(dv_kernel_t *kvars)
{
	int i;
	dv_arm_globaltimer_t *gt;

#if 0	/* Multi-core */
	dv_attach_irq(DV_IID_SGI0, dv_xcore_interrupt, 0);
#endif

	for ( i = DV_IID_SGI15+1; i < DV_IID_GTIMER ; i++ )
		dv_attach_irq(i, dv_unknown_interrupt, i);

	/* ToDo: board function to patch the "reserved" vectors?
	*/

#if 0	/* ToDo: select timer based on hardware */
	/* Set up the globaltimer and its interrupt handling
	*/
	gt = dv_get_config_base(DV_GTIMER_OFFSET);
	gt->status = DV_GT_IRQ;
	gt->ctrl = (DV_GT_IEN | DV_GT_CEN | DV_GT_TEN);
	dv_attach_irq(DV_IID_GTIMER, dv_gtimer_interrupt, 0);
	dv_config_irq(DV_IID_GTIMER, kvars->core_index, DV_LEVEL_GTIMER);
	dv_enable_irq(DV_IID_GTIMER);
#endif
}

/* man-page-generation - to be defined
*/
