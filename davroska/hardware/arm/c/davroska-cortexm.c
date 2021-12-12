/*  dv-cortexm.c - interrupt handling for Cortex M devices
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
#define DV_ASM	0
#include <dv-config.h>
#include <davroska.h>

#include DV_INCLUDE_INTERRUPTCONTROLLER

#include <davroska-inline.h>

#define DV_PENDSVSET	0x10000000
#define DV_PENDSVCLR	0x08000000

extern void dv_preemptframe(void);
extern void dv_preemptframe_end(void);
void dv_preempt(void);

/* dv_irq() - vector handler for all NVIC interrupt requests.
 *
 * Entry conditions (enforced by hardware):
 *	- running on main stack
 *	- handler mode
 *	- privileged
 *
 * Cortex M's return-from-interrupt mechanism and the NVIC are tightly coupled, with the unfortunate
 * result that if you drop from handler mode to thread mode to execute an ISR, the interrupt will
 * retrigger.
 * There are ways around it, but they're unpleasant. Since davroska is all privileged anyway, ISRs
 * are handled directly at handler level on the main stack.
 * Note: davroska's "kernel stack" and "extended stacks" both use Cortex M's process stack.
 * Theoretically it would be possible to put ISRs directly into the vector table. This has some disadvantages:
 *	- the vector table would occupy 336 bytes of RAM.
 *	- ISRs would no longer be "executables" in davroska.
 *	- davroska would need some modifications to prevent tasks that get activated during ISRs from running
 *	  immediately. Some hardware heuristic would be needed.
 *	- davroska assumes that ISRs are manage like tasks using the queue mechanism. Changing that would also
 *	  require changes in davroska
 *
 * The implementation adopted activates the ISRs as executables and runs them immediately on the main stack.
 * The dv_runqueued function is prevented from scheduling tasks by raising the current executable's priority to
 * the highest task priority.
 * Stack switching is inhibited by setting the onkernelstack flag to 1.
 * The original values are restored at the end of the processing.
 * If an ISR wakes up a task that has a higher priority than the current task, the "cortex m" method of
 * triggering a pendsv interrupt is used. When all interrupt processing is finished (including
 * nested interrupts) the pendsv handler pushes a preemption frame onto the interrupted process stack (which
 * might be the "kernel" stack or the private stack of an extended task). See dv_pendsvtrap() for details.
*/
void dv_irq(void)
{
	/* Get the interrupt request index from the hardware. NVIC interrupts start at 16.
	*/
	int irq = ((int)dv_get_ipsr()) - 16;

	/* Sanity check
	*/
	if ( (irq < 0) || (irq >= DV_NVECTOR) )
		dv_panic(dv_panic_UnexpectedHardwareResponse, dv_sid_interruptdispatcher, "irq number out of range");

	/* Raise priorty of current executable to highest task priority; save previous priority.
	 * This causes ISR executables to run immediately on the main (interrupt) stack, but any tasks
	 * that get woken up as a result get enqueued and run later on the appropriate extended/kernel stack.
	 * Ensure that curren executable's priority doesn't go lower as a result; it might be an ISR, or running
	 * occupying a mutex.
	 * There's no need to mess around with interrupt levels because the hardware sorts that out.
	 * Setting BASEPRI to a lower level than the IRQ has no effect.
	*/
	dv_id_t me = dv_currentexe;
	dv_prio_t my_p = dv_exe[me].currprio;

	if ( dv_exe[dv_currentexe].currprio < dv_maxtaskprio )
		dv_exe[dv_currentexe].currprio = dv_maxtaskprio;

#if DV_CFG_MAXEXTENDED > 0
	/* We're on the main stack, so inhibit switching stack to run ISRs.
	 * Only needed if there are extended tasks.
	*/
	extern dv_boolean_t dv_onkernelstack;
	dv_boolean_t my_onkernelstack = dv_onkernelstack;
	dv_onkernelstack = 1; 
#endif

	/* Call the vector function. If this activates an ISR executable, the ISR should run before
	 * returning.
	*/
	dv_softvector(irq);
	
	/* Sanity check
	*/
	if ( dv_currentexe != me )
		dv_panic(dv_panic_QueueCorrupt, dv_sid_interruptdispatcher, "current executable changed");

#if DV_CFG_MAXEXTENDED > 0
	/* Restore the kernel stack flag.
	 * Only needed if there are extended tasks.
	*/
	dv_onkernelstack = my_onkernelstack; 
#endif

	/* Restore priority of current executable
	*/
	dv_exe[me].currprio = my_p;

	/* Determine whether to trigger pendsv to perform context switch.
	 * This is a kind of stub version of dv_runqueued(). Instead of running the higher-priority executable,
	 * pendsv is triggered to do it when all interrupt handling is finished.
	*/
	dv_prio_t p = dv_maxtaskprio;

	while ( p > my_p )
	{
		dv_id_t e = dv_qhead(p);

		if ( e > 0 )
		{
			/* Executable with higher priority than current executable found.
			 * Trigger PENDSV to run higher-priority tasks that the ISR woke up.
			 * Once triggered - no need to carry on looking.
			*/
			dv_ctxm_scr.icsr = DV_PENDSVSET;
			break;
		}
		p--;
	}
}

/* dv_pendsvtrap() - handle the pendsv interrupt
 *
 * This interrupt is the lowest priority. It is triggered from dv_irq() when a task preemption is needed.
 * It runs when all normal interrupt handling has finished.
 *
 * To force a preemption, an artificial exception stack frame is pushed onto the process stack.
 * This stack frame causes the exception return to "return" to a specially-crafted assembly language
 * stub (dv_preepmptframe) that calls a given C function and then executes a system call (svc) to unwind
 * the stack and return to the preempted executable.
 *
 * IMPORTANT: the assembly stub is used so that the address can be verified by the svc trap handler.
 *
 * The C function is responsible for processing all executables of higher priority than the
 * "current" executable.
 *
 * TODO - the stack frame might need to take FPU operations into account on some cortex M hardware.
*/
void dv_pendsvtrap(void)
{
	/* Prevent further interrupts while preparing the preemption.
	*/
	dv_setirqlevel(DV_LOCKALL_LEVEL);

	/* Clear the pendsv request
	*/
	dv_ctxm_scr.icsr = DV_PENDSVCLR;


	dv_u32_t *psp = (dv_u32_t *)dv_get_psp();
	psp -= 8;
	dv_set_psp((dv_u32_t)psp);

	psp[0] = ((dv_u32_t)&dv_preempt) | 0x01;		/* r0 */
	psp[1] = psp[2] = psp[3] = psp[4] = 0;			/* r1-r3, r12 */
	psp[5] = 0;										/* lr */
	psp[6] = ((dv_u32_t)&dv_preemptframe) | 0x01;	/* pc */
	psp[7] = psp[15];								/* xPSR */

	if ( (psp[7] & 0x1ff) != 0 )
	{
		dv_panic(dv_panic_Exception, dv_sid_exceptionhandler, "PendSV with xSPR[8..0] != 0");
	}
}

/* dv_preempt() - execute all tasks that were awoke dugint interrupt handling
 *
 * This function is called from the stackframe that gets pushed onto
*/
void dv_preempt(void)
{
	(void)dv_disable();
	/* Run all queued executables down to the priority of the current executable.
	*/
	dv_runqueued_onkernelstack(dv_maxtaskprio, dv_exe[dv_currentexe].currprio, DV_INTENABLED);

	/* When all higher-priority activity is done, back to the original caller. To do that
	 * we have to pop the CPU's exception frame off the stack. That's a bit tricky -
	 * the simplest way is to use another exception - i.e. svc
	 * The svc is triggered from the assembly-language stub dv_preempt_frame
	*/
	dv_setirqlevel(15);
	(void)dv_restore(DV_INTENABLED);
}

/* dv_svctrap() - handle the svc trap to finish interrupt preemption.
*/
void dv_svctrap(void)
{
	dv_u32_t *psp = (dv_u32_t *)dv_get_psp();

	if ( (psp[6] & ~0x01) == (dv_u32_t)&dv_preemptframe_end )
	{
		psp += 8;
		dv_set_psp((dv_u32_t)psp);
		dv_setirqlevel(0);
	}
	else
	{
		dv_panic(dv_panic_Exception, dv_sid_exceptionhandler, "Oops! Unexpected SVC");
	}
}

/* dv_systickirq() - handle the systick interrupt
 *
 * TODO - SysTick support
*/
void dv_systickirq(void)
{
	dv_panic(dv_panic_Exception, dv_sid_exceptionhandler, "Oops! An SysTick trap occurred");
}
