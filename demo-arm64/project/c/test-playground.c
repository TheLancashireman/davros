
#include <kernel/h/dv-kconfig.h>
#include <kernel/h/dv-types.h>
#include <kernel/h/dv-error.h>
#include <kernel/h/dv-syscall.h>
#include <kernel/h/dv-thread.h>
#include <kernel/h/dv-stdio.h>
#include <kernel/h/dv-kernel.h>
#include <kernel/h/dv-trace.h>
#include <kernel/h/dv-coreconfig.h>
#include DV_H_REGISTERS
#include <kernel/h/dv-api.h>
#include <kernel/h/dv-error.h>

#include DV_H_SYSTEMTIMER
#include DV_H_CONSOLEUART
#include DV_H_INTERRUPTCONTROLLER
#define CMP 1		/* Which comparator of the timer module to monitor */

#include <kernel/h/dv-interrupt.h>

#include <devices/h/dv-arm-bcm2835-gpio.h>
#include <cpufamily/arm64/h/dv-arm64-core.h>

extern dv_u64_t prj_vectors;

void prj_dumpregs(const char *str, const dv_registers_t *r);

/* Task id variables
*/
dv_index_t task_foo;
dv_index_t task_fot;
dv_index_t task_bar;
dv_index_t task_qxx;

/* Task configurations
*/
void Task_Foo(void);
void Task_Fot(void);
void Task_Bar(void);
void Task_Qxx(void);
const dv_execonfig_t task_foo_cfg =
{   "task_foo",
	Task_Foo,		
	0,			/* Core */
	200,		/* Stacksize (words) */
	0,			/* Priority */
	1,			/* Max instances */
	0			/* Flags */
};
const dv_execonfig_t task_fot_cfg =
{   "task_fot",
	Task_Fot,		
	0,			/* Core */
	200,		/* Stacksize (words) */
	0,			/* Priority */
	2,			/* Max instances */
	0			/* Flags */
};
const dv_execonfig_t task_bar_cfg =
{   "task_bar",
	Task_Bar,		
	0,			/* Core */
	200,		/* Stacksize (words) */
	1,			/* Priority */
	1,			/* Max instances */
	DV_EXEFLAG_BLOCKING	/* Flags */
};
const dv_execonfig_t task_qxx_cfg =
{   "task_qxx",
	Task_Qxx,		
	0,			/* Core */
	200,		/* Stacksize (words) */
	2,			/* Priority */
	1,			/* Max instances */
	DV_EXEFLAG_BLOCKING	/* Flags */
};

void print64(char *s, dv_u64_t v64)
{
	dv_kprintf("%s = 0x%x%08x\n", s, (dv_u32_t)(v64>>32), (dv_u32_t)v64);
}

#if 0
void dv_catch_thread_synchronous_exception(dv_kernel_t *kvars)
{
	dv_panic(dv_panic_unimplemented, "dv_catch_thread_synchronous_exception", "Oops! An exception occurred");
}
#endif

void dv_catch_thread_irq(dv_kernel_t *kvars)
{
#if 1
	dv_kprintf("dv_catch_thread_irq() - calling dv_irq_handler()\n");
	dv_irq_handler(kvars);
#else
	dv_panic(dv_panic_unimplemented, "dv_catch_thread_irq", "Oops! An exception occurred");
#endif
}

void dv_catch_thread_fiq(dv_kernel_t *kvars)
{
	dv_panic(dv_panic_unimplemented, "dv_catch_thread_fiq", "Oops! An exception occurred");
}

void dv_catch_thread_syserror(dv_kernel_t *kvars)
{
	dv_panic(dv_panic_unimplemented, "dv_catch_thread_syserror", "Oops! An exception occurred");
}

void dv_catch_kernel_synchronous_exception(dv_kernel_t *kvars)
{
	dv_panic(dv_panic_unimplemented, "dv_catch_kernel_synchronous_exception", "Oops! An exception occurred");
}

void dv_catch_kernel_irq(dv_kernel_t *kvars)
{
	dv_panic(dv_panic_unimplemented, "dv_catch_kernel_irq", "Oops! An exception occurred");
}

void dv_catch_kernel_fiq(dv_kernel_t *kvars)
{
	dv_panic(dv_panic_unimplemented, "dv_catch_kernel_fiq", "Oops! An exception occurred");
}

void dv_catch_kernel_syserror(dv_kernel_t *kvars)
{
	dv_panic(dv_panic_unimplemented, "dv_catch_kernel_syserror", "Oops! An exception occurred");
}

void dv_catch_aarch32_synchronous_exception(dv_kernel_t *kvars)
{
	dv_panic(dv_panic_unimplemented, "dv_catch_aarch32_synchronous_exception", "Oops! An exception occurred");
}

void dv_catch_aarch32_irq(dv_kernel_t *kvars)
{
	dv_panic(dv_panic_unimplemented, "dv_catch_aarch32_irq", "Oops! An exception occurred");
}

void dv_catch_aarch32_fiq(dv_kernel_t *kvars)
{
	dv_panic(dv_panic_unimplemented, "dv_catch_aarch32_fiq", "Oops! An exception occurred");
}

void dv_catch_aarch32_syserror(dv_kernel_t *kvars)
{
	dv_panic(dv_panic_unimplemented, "dv_catch_aarch32_syserror", "Oops! An exception occurred");
}

void dv_panic(dv_panic_t reason, char *function, char *message)
{
	dv_kprintf("Davros panic %d in %s: %s\n", reason, function, message);
	for (;;) {}
}

/* This function does the job of main(). It runs in a thread, so it really shouldn't call dv_kprintf()
*/
void prj_init(void)
{
	dv_errorid_t e;
	dv_kernel_t *kvars;
	dv_u64_t v;

	dv_kprintf("prj_init: started.\n");
	dv_kprintf("Current EL = %d\n", dv_get_el());

	kvars = dv_get_kvars();
	dv_kprintf("kvars : 0x%08lx\n", (dv_u64_t)kvars);
	dv_kprintf("kvars->current_thread : 0x%08lx\n", (dv_u64_t)kvars->current_thread);
	dv_kprintf("kvars->kernel_sp : 0x%08lx\n", (dv_u64_t)kvars->kernel_sp);
	dv_kprintf("kvars->core_index = %d\n", kvars->core_index);
	prj_dumpregs(kvars->current_thread->executable->name, kvars->current_thread->regs);

	v = dv_arm64_mrs(VBAR_EL1);
	dv_kprintf("VBAR_EL1 : 0x%08lx\n", v);

	v = dv_arm64_mrs(S3_1_C15_C3_0);		/* CBAR_EL1 */
	dv_kprintf("CBAR_EL1 : 0x%08lx (PeriphBase)\n", v);

#if 0
	/* Four GPIO pins for the LEDs.
	*/
	dv_arm_bcm2835_gpio_pinconfig(17, DV_pinfunc_output, DV_pinpull_none);
	dv_arm_bcm2835_gpio_pinconfig(18, DV_pinfunc_output, DV_pinpull_none);
	dv_arm_bcm2835_gpio_pinconfig(27, DV_pinfunc_output, DV_pinpull_none);
	dv_arm_bcm2835_gpio_pinconfig(22, DV_pinfunc_output, DV_pinpull_none);
#endif

	dv_kprintf("prj_init: calling null system call.\n");
	dv_nullsc(0x42, 0xdeadbeef, 0x12345678, 0x98765432);
	dv_kprintf("prj_init: returned from null system call.\n");

	e = dv_spawn(1000);
	dv_kprintf("prj_init: dv_spawn(1000) returned %d\n", e);

	e = dv_spawn(task_foo+1);
	dv_kprintf("prj_init: dv_spawn(%d) returned %d\n", task_foo+1, e);

	e = dv_spawn(0);
	dv_kprintf("prj_init: dv_spawn(0) returned %d\n", e);

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

		rv = dv_create_exe(&task_fot_cfg);

		if ( rv.rv0 == dv_eid_None )
		{
			dv_kprintf("prj_init: created task_fot (%d) on core %d\n", task_fot, kvars->core_index);
			task_fot = (dv_index_t)rv.rv1;
			e = dv_spawn(task_fot);
			dv_kprintf("prj_init: dv_spawn(task_fot) returned %d\n", e);
		}
		else
		{
			dv_kprintf("prj_init: failed to create task_fot (%d, %d) on core %d\n", rv.rv0, rv.rv1, kvars->core_index);
		}
	}
}

/* This function is called at startup so the system-call API is not available.
 * Use direct function calls to internal functions instead.
*/
void prj_startup(dv_kernel_t *kvars)
{
	dv_kprintf("prj_startup(): called on core %d\n", kvars->core_index);
}

void Task_Foo(void)
{
	dv_errorid_t e;
	dv_dual_t rv;

	dv_kprintf("Task_Foo: started\n");

	e = dv_spawn(task_fot);		/* should run when foo finishes */
	dv_kprintf("Task_Foo: dv_spawn(task_fot) returned %d\n", e);
	e = dv_spawn(task_fot);		/* should fail: max instances exceeded */
	dv_kprintf("Task_Foo: dv_spawn(task_fot) returned %d\n", e);

	rv = dv_create_exe(&task_bar_cfg);

	if ( rv.rv0 == dv_eid_None )
	{
		task_bar = (dv_index_t)rv.rv1;

		dv_kprintf("Task_Foo: created task_bar, id = %d\n", task_bar);
		e = dv_spawn(task_bar);
		dv_kprintf("Task_Foo: dv_spawn(task_bar) returned %d\n", e);
	}
	else
	{
		dv_kprintf("Task_Foo: dv_create_exe() returned error %d (rv1 = 0x%08x)\n", rv.rv0, rv.rv1);
	}

	rv = dv_create_exe(&task_qxx_cfg);

	if ( rv.rv0 == dv_eid_None )
	{
		task_qxx = (dv_index_t)rv.rv1;

		dv_kprintf("Task_Foo: created task_qxx, id = %d\n", task_qxx);
		e = dv_spawn(task_qxx);
		dv_kprintf("Task_Foo: dv_spawn(task_qxx) returned %d\n", e);
	}
	else
	{
		dv_kprintf("Task_Foo: dv_create_exe() returned error %d (rv1 = 0x%08x)\n", rv.rv0, rv.rv1);
	}

#if 0
	dv_u64_t then = 0;
	dv_u64_t now = 0;

	for  (;;)
	{
		int nchar = 0;
		then = now;

		do {
			if ( dv_consoledriver.isrx() )
			{
				dv_kputc(dv_consoledriver.getc());
				nchar++;
			}
			now = dv_readtime();
		} while ( (now - then) < 1000000 );

		if ( nchar != 0 )
		{
			nchar = 0;
			dv_kputc('\n');
		}
		dv_kprintf("Time: 0x%08x%08x cmp = 0x%08x int = %d\n",
			(dv_u32_t)(now / 0x100000000), (dv_u32_t)(now % 0x100000000),
			dv_getcmp(CMP), dv_getmatch(CMP));
	}
#endif
}

void Task_Fot(void)
{
	dv_kprintf("Task_Fot: started\n");
}

const dv_u64_t led_mask = (1<<17)|(1<<18)|(1<<27)|(1<<22);

const dv_u64_t led_vals[16] =
{	0,
	(1<<17),
	(1<<18),
	(1<<17)|(1<<18),
	(1<<27),
	(1<<17)|(1<<27),
	(1<<18)|(1<<27),
	(1<<17)|(1<<18)|(1<<27),
	(1<<22),
	(1<<17)|(1<<22),
	(1<<18)|(1<<22),
	(1<<17)|(1<<18)|(1<<22),
	(1<<27)|(1<<22),
	(1<<17)|(1<<27)|(1<<22),
	(1<<18)|(1<<27)|(1<<22),
	(1<<17)|(1<<18)|(1<<27)|(1<<22)
};

void Task_Bar(void)
{
	dv_errorid_t e;
	int bar_count = 0;
	dv_kprintf("Task_Bar: started\n");

	for (;;)
	{
		dv_kprintf("Task_Bar: sleeping\n");
		dv_sleep(1000000);
		bar_count++;
		dv_kprintf("Task_Bar: woken up %d\n", bar_count);
		e = dv_spawn(task_fot);		/* should run when bar finishes */
		dv_kprintf("Task_Bar dv_spawn(task_fot) returned %d\n", e);

		/* Temporary: try some bad things */
#if 0
		dv_kprintf("Task_Bar trying to write to 0x4000\n");
		*(dv_u32_t *)0x4000 = 0;
#endif

#if 0
		dv_kprintf("Task_Bar trying to write misaligned\n");
		*(dv_u32_t *)(((dv_u32_t)(&bar_count))+1) = 0;
#endif
	}
}

void Task_Qxx(void)
{
	dv_errorid_t e;
	int qxx_count = 0;
	dv_kprintf("Task_Qxx: started\n");
	dv_arm_bcm2835_gpio_pin_set_group(led_mask);	/* All LEDs off */

	dv_u64_t next = dv_readtime();
	dv_kprintf("Task_Qxx() next = %ld\n", next);
	for (;;)
	{
		next += 1000000;
		dv_kprintf("Task_Qxx: sleeping\n");
		dv_sleep_until(next);
		dv_arm_bcm2835_gpio_pin_clear_group(led_vals[qxx_count%16]);
		dv_arm_bcm2835_gpio_pin_set_group(led_vals[qxx_count%16]^led_mask);
		//	dv_arm_bcm2835_gpio_pin_clear(17);
		qxx_count++;
		dv_kprintf("Task_Qxx: woken up %d\n", qxx_count);
		e = dv_spawn(task_fot);		/* should run when qxx finishes */
		dv_kprintf("Task_Qxx dv_spawn(task_fot) returned %d\n", e);
	}
}

void prj_exc_handler(dv_u64_t x0, dv_u64_t x1, dv_u64_t x2, dv_u64_t x3)
{
	int el;
	dv_u64_t v;

	dv_kprintf("prj_exc_handler() invoked\n");

	el = dv_get_el();
	dv_kprintf("Currentl EL : %d\n", el);

	if ( el == 1 )
	{
		v = dv_arm64_mrs(ELR_EL1);
		dv_kprintf(" ELR_EL1  : 0x%08lx\n", v);
		v = dv_arm64_mrs(SPSR_EL1);
		dv_kprintf(" SPSR_EL1 : 0x%08lx\n", v);
		v = dv_arm64_mrs(ESR_EL1);
		dv_kprintf(" ESR_EL1 : 0x%08lx\n", v);
	}

	v = dv_arm64_mrs(TPIDRRO_EL0);
	dv_kernel_t *kvars = (dv_kernel_t *)v;

	dv_kprintf("  kvars : 0x%08lx\n", (dv_u64_t)kvars);
	dv_kprintf("  kvars->current_thread : 0x%08lx\n", (dv_u64_t)(kvars->current_thread));
	dv_kprintf("  kvars->current_thread->regs : 0x%08lx\n", (dv_u64_t)(kvars->current_thread->regs));

	dv_kprintf("  x0 : 0x%08lx\n", x0);
	dv_kprintf("  x1 : 0x%08lx\n", x1);
	dv_kprintf("  x2 : 0x%08lx\n", x2);
	dv_kprintf("  x3 : 0x%08lx\n", x3);

	for (;;) { }
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
