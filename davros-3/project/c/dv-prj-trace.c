
#include <kernel/h/dv-kconfig.h>
#include <kernel/h/dv-types.h>
#include <kernel/h/dv-syscall.h>
#include <kernel/h/dv-thread.h>
#include <kernel/h/dv-stdio.h>
#include <kernel/h/dv-trace.h>
#include DV_H_REGISTERS
#include <kernel/h/dv-error.h>

const char *tstates[dv_thread_nstates] = { DV_THREADSTATES };

void dv_saveregs(dv_registers_t *r);

void dv_trace_threadstate(dv_thread_t *thread, dv_threadstate_t newstate)
{
	dv_kprintf("TRACE: thread 0x%08x (%s) changed from %s to %s\n",
			(unsigned)thread, thread->executable->name, tstates[thread->state], tstates[newstate]);
}

void dv_trace_api(dv_thread_t *thread, dv_index_t sci, const dv_syscall_t *sc)
{
	dv_kprintf("TRACE: thread 0x%08x (%s) called API %d (%s) with parameters 0x%08x, 0x%08x, , 0x%08x, 0x%08x\n",
			(unsigned)thread, thread->executable->name, sci, sc->name,
			dv_get_p0(thread->regs), dv_get_p1(thread->regs), dv_get_p2(thread->regs), dv_get_p3(thread->regs));
}

void dv_trace_dumpregs(const char *str, const dv_registers_t *r)
{
	int i;

	dv_kprintf("dumpregs: %s r = 0x%08x\n", str, r);

	for ( i=0; i<13; i++ )
	{
		dv_kprintf("r%-2d  = 0x%08x\n", i, r->gpr[i]);
	}
	dv_kprintf("pc   = 0x%08x\n", r->pc);
	dv_kprintf("cpsr = 0x%08x\n", r->cpsr);
	dv_kprintf("sp   = 0x%08x\n", r->sp);
	dv_kprintf("lr   = 0x%08x\n", r->lr);
}

void dv_trace_dumpcpuregs(void)
{
	dv_registers_t r;

	dv_saveregs(&r);
	dv_trace_dumpregs("Current registers", &r);
}
