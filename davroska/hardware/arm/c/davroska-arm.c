/* davroska-arm.c - OSEK-like member of the davros family; ARM specific code
 *
 * (c) David Haworth
*/
#define DV_ASM  0
#include <dv-config.h>
#include <davroska-api.h>
#include <davroska.h>
#include <dv-string.h>

#include DV_INCLUDE_INTERRUPTCONTROLLER


/* Kernel stack starts 32 bytes below top of RAM
*/
extern dv_u32_t dv_end_ram;
const dv_u32_t dv_initialsp_svc = (dv_u32_t)&dv_end_ram - 32;

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

	/* Raise priority to maximum possible; save previous priority
	*/
	dv_prio_t p = dv_currentprio;
	dv_currentprio = dv_highestprio;

	/* Call all interrupt functions; most will activate executables
	*/
	dv_dispatch_interrupts();

	/* Now run all queued executables down to saved priority
	*/
	dv_runqueued(p, DV_INTENABLED);

	/* Restore the saved irq context
	*/
	dv_memcpy32(dv_irqstack, context, 8);
}
