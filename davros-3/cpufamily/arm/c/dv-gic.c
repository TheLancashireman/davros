/*	dv-gic.c - arm GIC functions
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
#include <cpufamily/arm/h/dv-arm-gic.h>
#include <cpufamily/arm/h/dv-arm-dispatch.h>

/* dv_init_gic() - initialise the interrupt controller
*/
void dv_init_gic(void)
{
	dv_gicc_t *icc = dv_get_config_base(DV_GICC_OFFSET);
	dv_gicd_t *icd = dv_get_config_base(DV_GICD_OFFSET);
	int i;

	/* The ICC is banked, so each core must initialise its own copy.
	*/
	icc->iccicr = DV_ICCICR_ENABLE;		/* Enable the controller. */
	icc->iccpmr = 0;					/* Set level to disable all interrupts. */

	/* The ICD is mostly common to all cores.
	 * Initialisation just writes fixed values to registers, so it should be OK
	 * to do it on all cores.
	*/
	icd->icdctlr = DV_ICDCTLR_ENABLE;	/* Enable the distributor. */

	/* Clear all the target cores and set priorities to the lowest possible.
	*/
	for ( i = 0; i < 1020; i++ )
	{
		/* The ARM docs describe these registers as 32-bit registers each of which contains priority
		 * or target core bits for 4 IRQs. However, they are accessible with 8-bit writes, so we define them
		 * as 8-bit registers for simplicity.
		 * Removing cores from the target set does not affect the active flag on that core,
		 * so we clear the active flags later.
		*/
		icd->icditargetsr[i] = 0;				/* Clear all the target core bits. */
		icd->icdipriorityr[i] = 0xff;			/* Set priority to lowest possible. */
	}

	/* Disable, clear pending and active flags for all interrupt sources.
	*/
	for ( i = 0; i < 32; i++ )
	{
		/* Each register contains bits for 32 interrupt sources.
		 * These registers are "write 1 to clear".
		*/
		icd->icdicenabler[i] = 0xffffffff;		/* Disable interrupt sources. */
		icd->icdicpendr[i] = 0xffffffff;		/* Clear the pending state of interrupt sources. */
		icd->icdicactiver[i] = 0xffffffff;		/* Set interrupt sources to inactive. */
	}

	/* Clear the pending flags for software-generated interrupts (SGI).
	 * This MUST be done on all cores.
	*/
	for ( i = 0; i < 16; i++ )
	{
		/* The ARM docs describe these registers as 32-bit registers each of which contains pending
		 * bits for 4 SGIs. However, they are accessible with 8-bit writes, so we define them
		 * as 8-bit registers for simplicity.
		 * Each register has a bit for each core. However, a core can only affect its own SGIs.
		 * These registers are "write 1 to clear".
		*/
		icd->icdcpendsgir[i] = 0xff;			/* Clear pending state for SGIs */
	}
}

void dv_irq_handler(dv_kernel_t *kvars)
{
	dv_gicc_t *icc = dv_get_config_base(DV_GICC_OFFSET);
	unsigned vec, iid;

	/* Read and acknowledge, then immediately signal EOI.
	*/
	vec = icc->icciar;
	icc->icceoir = vec;

	iid = vec & DV_ICCIAR_INTERRUPT_ID;		/* Discard the CPU ID (and other bits) */

	if ( iid < DV_N_IID )
	{
		dv_dispatch_interrupt(kvars, vec);
	}
	else if ( iid == DV_IID_SPURIOUS )
	{
		/* Spurious interrupt. Silently return to interrupted thread as quickly as possible.
		*/
		dv_resume_thread(kvars, kvars->current_thread);
	}
	else
	{
		/* Interrupt that cannot occur (according to documents).
		 * Note: iid 1020-1022 have special meanings sometimes - let's see what happens.
		*/
		dv_unknown_interrupt(kvars, vec);
	}
}
