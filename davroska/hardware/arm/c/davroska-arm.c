/* davroska-arm.c - OSEK-like member of the davros family; ARM specific code
 *
 * (c) David Haworth
*/
#define DV_ASM  0
#include <dv-config.h>
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

	dv_printf(" irqstack: 0x%08x 0x%08x 0x%08x 0x%08x 0x%08x 0x%08x 0x%08x 0x%08x\n",
		dv_irqstack[0], dv_irqstack[1], dv_irqstack[2], dv_irqstack[3],
		dv_irqstack[4], dv_irqstack[5], dv_irqstack[6], dv_irqstack[7]);

	/* Save irq context on stack
	*/
	dv_memcpy32(context, dv_irqstack, 8);

	/* For an irq, have to adjust the return address by -4
	*/
	context[5] -= 4;

	dv_printf("     ctxt: 0x%08x 0x%08x 0x%08x 0x%08x 0x%08x 0x%08x 0x%08x 0x%08x\n",
		context[0], context[1], context[2], context[3],
		context[4], context[5], context[6], context[7]);

	/* Raise priority to maximum possible; save previous priority
	*/
	dv_id_t me = dv_currentexe;
	dv_prio_t my_p = dv_exe[me].currprio;
	dv_printf("dv_catch_irq() : prio %d --> %d\n", my_p, dv_maxprio+1);
	dv_exe[dv_currentexe].currprio = dv_maxprio+1;

	/* Call all interrupt functions; most will activate executables
	*/
	dv_printf("dv_catch_irq() : dv_dispatch_interrupts()\n");
	dv_dispatch_interrupts();

	/* Sanity check
	*/
	if ( dv_currentexe != me )
		dv_panic(dv_panic_UnknownPanic);		/* ToDo ...*/

	/* Restore priority of current executable
	*/
	dv_exe[me].currprio = my_p;

	/* Now run all queued executables down to saved priority
	*/
	dv_printf("dv_catch_irq() : dv_runqueued() %d\n", my_p);
	dv_runqueued(dv_maxprio, dv_exe[dv_currentexe].currprio, DV_INTENABLED);

	/* When all higher-priority activity is done, back to the original caller
	*/
	dv_setqueueirqlevel(my_p);

	dv_printf("     ctxt: 0x%08x 0x%08x 0x%08x 0x%08x 0x%08x 0x%08x 0x%08x 0x%08x\n",
		context[0], context[1], context[2], context[3],
		context[4], context[5], context[6], context[7]);

	dv_printf("dv_catch_irq() : restore context\n");
	/* Restore the saved irq context
	*/
	dv_memcpy32(dv_irqstack, context, 8);

	dv_printf(" irqstack: 0x%08x 0x%08x 0x%08x 0x%08x 0x%08x 0x%08x 0x%08x 0x%08x\n",
		dv_irqstack[0], dv_irqstack[1], dv_irqstack[2], dv_irqstack[3],
		dv_irqstack[4], dv_irqstack[5], dv_irqstack[6], dv_irqstack[7]);

	dv_printf("dv_catch_irq() : return\n");
}
