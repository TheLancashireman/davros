
#include <kernel/h/dv-kconfig.h>
#include <kernel/h/dv-types.h>
#include <kernel/h/dv-error.h>
#include <kernel/h/dv-syscall.h>
#include <kernel/h/dv-thread.h>
#include <kernel/h/dv-stdio.h>
#include <kernel/h/dv-kernel.h>
#include <kernel/h/dv-coreconfig.h>
#include DV_REGISTERS
#include <kernel/h/dv-api.h>
#include <kernel/h/dv-error.h>
#include <cpufamily/arm/h/dv-arm-globaltimer.h>

/* Task id variables
*/
dv_index_t task_foo;
dv_index_t task_bar;

/* Task configurations
*/
void Task_Foo(void);
void Task_Bar(void);
const dv_execonfig_t task_foo_cfg =
{   "task_foo",
	Task_Foo,		
	0,			/* Core */
	200,		/* Stacksize (words) */
	0,			/* Priority */
	1,			/* Max instances */
	0			/* Flags */
};
const dv_execonfig_t task_bar_cfg =
{   "task_bar",
	Task_Bar,		
	0,			/* Core */
	200,		/* Stacksize (words) */
	1,			/* Priority */
	1,			/* Max instances */
	0			/* Flags */
};




void dv_panic(dv_panic_t reason, char *function, char *message)
{
	dv_kprintf("Davros panic %d in %s: %s\n", reason, function, message);
	for (;;) {}
}

void prj_init(void)
{
	dv_errorid_t e;
	dv_kernel_t *kvars;

	dv_kprintf("prj_init: started.\n");

	dv_nullsc(0x42, 0xdeadbeef, 0x12345678, 0x98765432);

	dv_kprintf("prj_init: returned from null system call.\n");

	e = dv_spawn(1000);
	dv_kprintf("prj_init: dv_spawn(1000) returned %d\n", e);

	e = dv_spawn(task_foo+1);
	dv_kprintf("prj_init: dv_spawn(%d) returned %d\n", task_bar+1, e);

	e = dv_spawn(0);
	dv_kprintf("prj_init: dv_spawn(0) returned %d\n", e);

	kvars = dv_get_kvars();

	if ( kvars->core_index == 0 )
	{
		dv_dual_t rv;
		rv = dv_create_exe(&task_foo_cfg);

		if ( rv.rv0 == dv_eid_None )
		{
			dv_kprintf("prj_init: created task_foo (%d) on core %d\n", task_foo, kvars->core_index);
			task_foo = (dv_index_t)rv.rv1;
			e = dv_spawn(task_foo);
			dv_kprintf("prj_init: dv_spawn(task_foo) returned %d\n", e);
		}
		else
		{
			dv_kprintf("prj_init: failed to create task_foo (%d, %d) on core %d\n", rv.rv0, rv.rv1, kvars->core_index);
		}
	}
}

const char *tstates[dv_thread_nstates] = { DV_THREADSTATES };

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

/* This function is called at startup so the system-call API is not available.
 * Use direct function calls to internal functions instead.
*/
void prj_startup(dv_kernel_t *kvars)
{
	dv_kprintf("prj_startup: called on core %d\n", kvars->core_index);
}

void Task_Foo(void)
{
	int i;
	dv_errorid_t e;
	dv_dual_t rv;
	dv_u64_t t;
	dv_arm_globaltimer_t *gt;

	dv_kprintf("Task_Foo: started\n");

	rv = dv_create_exe(&task_bar_cfg);

	if ( rv.rv0 == dv_eid_None )
	{
		task_bar = (dv_index_t)rv.rv1;

		dv_kprintf("Task_Foo: created task_bar, id = %d\n", task_bar);
		for ( i = 0; i < 5; i++ )
		{
			e = dv_spawn(task_bar);
			dv_kprintf("Task_Foo: dv_spawn(task_bar) (%d) returned %d\n", i, e);
		}
	}
	else
	{
		dv_kprintf("Task_Foo: dv_create_exe() returned error %d (rv1 = 0x%08x)\n", rv.rv0, rv.rv1);
	}

	dv_kprintf("Task_Foo: calling dv_get_config_base()\n");
	gt = dv_get_config_base(DV_GTIMER_OFFSET);
	dv_kprintf("Task_Foo: gtimer = 0x%08x\n", (unsigned)gt);

#if 1
#define delay 100000000
	
	t = dv_readtime();
	dv_kprintf("Task_Foo: initial t = 0x%08x%08x\n", (unsigned)(t>>32), (unsigned)t);
	t += delay;
	gt->compare_hi = (unsigned)(t >> 32);
	gt->compare_lo = (unsigned)t;
	gt->compare_increment = delay;
	gt->status = DV_GT_IRQ;
	gt->ctrl = (DV_GT_INC | DV_GT_IEN | DV_GT_CEN | DV_GT_TEN);

	for ( i = 0; i < 60; i++ )
	{
		while ( (gt->status & DV_GT_IRQ) == 0 )
		{
		}
		gt->status = DV_GT_IRQ;
		t = dv_readtime();
		dv_kprintf("Task_Foo: t = 0x%08x%08x\n", (unsigned)(t>>32), (unsigned)t);
	}

#endif
}

int bar_count = 0;

void Task_Bar(void)
{
	dv_kprintf("Task_Bar: started\n");
	bar_count++;
	dv_kprintf("Task_Bar: executed %d times\n", bar_count);
}
