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
#include DV_H_INTERRUPTCONTROLLER
#include DV_H_SYSTEMTIMER

/* dv_init_hardware() - initialise the processor etc.
*/
void dv_init_hardware(dv_kernel_t *kvars)
{
	dv_kprintf("dv_init_hardware() called\n");

	dv_init_interrupt_controller();
}

/* dv_init_peripherals() - initisalise the peripherals used by davros.
 *
 * Modifying vectors for reserved and banked interrupts is done here too.
*/
void dv_init_peripherals(dv_kernel_t *kvars)
{
	dv_kprintf("dv_init_peripherals()\n");

	dv_init_system_timer(kvars);

/* The stuff below here will have to be moved to a cortex-a file eventually
*/
#if 0	/* Multi-core */
	dv_arm_globaltimer_t *gt;

	dv_attach_irq(DV_IID_SGI0, dv_xcore_interrupt, 0);

	for ( i = DV_IID_SGI15+1; i < DV_IID_GTIMER ; i++ )
		dv_attach_irq(i, dv_unknown_interrupt, i);
#endif

#if 0
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
