/* davroska-arm.c - OSEK-like member of the davros family; ARM specific code
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
#define DV_ASM  0
#include <dv-config.h>
#include <davroska.h>
#include <dv-string.h>

#include DV_INCLUDE_INTERRUPTCONTROLLER

#include <davroska-inline.h>

#ifndef DV_DEBUG
#define DV_DEBUG	0
#endif


/* Kernel stack starts DV_CANARY bytes below top of memory
*/
extern dv_u32_t dv_end_mem;
const dv_u32_t dv_initialsp_svc = (dv_u32_t)&dv_end_mem - DV_CANARY;

/* Only a very small stack is needed for irq and fiq because we switch to svc.
 * The stack is not pushed, just stored, so the initialsp constants contain the address of element 0
 *
 * The kernel copies the stack to handle nested interrupts.
 * For irq, 8 registers are saved: see dv-trap-irq.S
 * For fiq, not implemented yet.
*/
dv_u32_t dv_irqstack[8];
dv_u32_t dv_fiqstack[8];

const dv_u32_t dv_initialsp_irq = (dv_u32_t)&dv_irqstack[0];
const dv_u32_t dv_initialsp_fiq = (dv_u32_t)&dv_fiqstack[0];

/* dv_catch_irq() - catch IRQ, dispatch interrupts depending on exact interrupt controller
*/
void dv_catch_irq(void)
{
	dv_u32_t context[8];

	/* Save irq context on stack
	*/
	dv_memcpy32(context, dv_irqstack, 8);

	/* For an irq, have to adjust the return address by -4
	*/
	context[5] -= 4;

	/* Raise priority to maximum possible; save previous priority
	*/
	dv_id_t me = dv_currentexe;
	dv_prio_t my_p = dv_exe[me].currprio;
	dv_exe[dv_currentexe].currprio = dv_maxprio+1;

	/* Call all interrupt functions; most will activate executables
	*/
	dv_bcm2835_interrupt_handler();

	/* Sanity check
	*/
	if ( dv_currentexe != me )
		dv_panic(dv_panic_QueueCorrupt, dv_sid_interruptdispatcher, "current executable changed");

	/* Restore priority of current executable
	*/
	dv_exe[me].currprio = my_p;

	/* Now run all queued executables down to saved priority
	*/
	dv_runqueued_onkernelstack(dv_maxprio, dv_exe[dv_currentexe].currprio, DV_INTENABLED);

	/* When all higher-priority activity is done, back to the original caller
	*/
	dv_setqueueirqlevel(my_p);

	/* Restore the saved irq context
	*/
	dv_memcpy32(dv_irqstack, context, 8);
}
