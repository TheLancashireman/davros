
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

#if 0
#include <cpufamily/arm/h/dv-arm-globaltimer.h>
#include <cpufamily/arm/h/dv-arm-gic.h>
#endif

#if DV_CPU == DV_ARM1176
#include <cpufamily/arm/h/dv-arm-bcm2835-timer.h>
#endif

#include <kernel/h/dv-interrupt.h>

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

void dv_trap_unimplemented(void)
{
	dv_panic(dv_panic_unimplemented, "dv_trap_unimplemented", "Oops! An exception occurred");
}


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
	dv_kprintf("prj_init: dv_spawn(%d) returned %d\n", task_foo+1, e);

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

	{
#define CMP 1
		dv_u64_t then = 0;
		dv_u64_t now = 0;
		int m;

		dv_setcmp(CMP, (dv_readtime() & 0xffffffff) + 10000000);
		dv_clrmatch(CMP);

		for  (;;)
		{
#if 1
			then = now;

			do {
				now = dv_readtime();
			} while ( (now - then) < 1000000 );

			dv_kprintf("Time: 0x%08x%08x cmp = 0x%08x int = %d\n",
				(dv_u32_t)(now / 0x100000000), (dv_u32_t)(now % 0x100000000),
				dv_getcmp(CMP), m = dv_getmatch(CMP));
#if 1
			if ( m )
			{
				dv_setcmp(CMP, dv_getcmp(CMP) + 10000000);
				dv_clrmatch(CMP);
			}
#endif
#else
			now = dv_readtime();
			dv_kprintf("Time: 0x%08x%08x\n", (dv_u32_t)(now / 0x100000000), (dv_u32_t)(now % 0x100000000));
#endif
		}
	}
}

int bar_count = 0;

void Task_Bar(void)
{
	dv_kprintf("Task_Bar: started\n");
	bar_count++;
	dv_kprintf("Task_Bar: executed %d times\n", bar_count);
}

#if 0
void cortex_a_stuff(void)
{
	gt = dv_get_config_base(DV_GTIMER_OFFSET);
	icc = dv_get_config_base(DV_GICC_OFFSET);
	icd = dv_get_config_base(DV_GICD_OFFSET);

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

#if 0
	dv_config_irq(27, 0, 254);
	dv_attach_irq(27, dv_gtimer_interrupt, 0);
	dv_enable_irq(27);
#endif

	dv_set_level(255);
	dv_kprintf("Task_Foo: gicc.iccpmr = %d, gicd.icdipriorityr[27] = %d\n", icc->iccpmr, icd->icdipriorityr[27]);

#if 0
	for ( i = 0; i < 60; i++ )
	{
		p1 = icc->icchpir;
		while ( (gt->status & DV_GT_IRQ) == 0 )
		{
		}
		p2 = icc->icchpir;
		gt->status = DV_GT_IRQ;
		t = dv_readtime();
		dv_kprintf("Task_Foo: t = 0x%08x%08x p1=%d, p2=%d\n", (unsigned)(t>>32), (unsigned)t, p1, p2);
	}
#endif

#endif
}
#endif
