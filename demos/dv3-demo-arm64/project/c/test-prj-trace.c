
#include <kernel/h/dv-kconfig.h>
#include <dv-types.h>
#include <kernel/h/dv-syscall.h>
#include <kernel/h/dv-thread.h>
#include <kernel/h/dv-stdio.h>
#include <kernel/h/dv-trace.h>
#include DV_H_REGISTERS
#include <kernel/h/dv-error.h>

void print64(char *s, dv_u64_t v64);

void prj_dumpregs(const char *str, const dv_registers_t *r)
{
	int i;

	dv_kprintf("dumpregs: %s\n", str);

	print64("  registers", (dv_u64_t)r);

	for ( i=0; i<31; i++ )
	{
		dv_kprintf("  r%-2d  : 0x%x%08x\n", i, (dv_u32_t)(r->gpr[i]>>32), (dv_u32_t)r->gpr[i]);
	}
	print64("  pc   ", r->pc);
	print64("  psr  ", r->psr);
	print64("  sp   ", r->sp);
}

#if DV_TRACE

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

void dv_trace_api_done(dv_thread_t *thread, dv_index_t sci, const dv_syscall_t *sc)
{
	dv_kprintf("TRACE: API %d (%s) returned\n", sci, sc->name);
}

void dv_trace_dumpregs(const char *str, const dv_registers_t *r)
{
	prj_dumpregs(str, r);
}

void dv_trace_dumpcpuregs(void)
{
	dv_registers_t r;

	dv_saveregs(&r);
	dv_trace_dumpregs("Current registers", &r);
}
#endif
