/* demo.c
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
*/
#include <kernel/h/dv-kconfig.h>
#include <dv-types.h>
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

#include <dv-arm-bcm2835-gpio.h>

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
	0			/* Flags */
};
const dv_execonfig_t task_fot_cfg =
{   "task_fot",
	Task_Fot,
	0,			/* Core */
	200,		/* Stacksize (words) */
	0,			/* Priority */
	0			/* Flags */
};
const dv_execonfig_t task_bar_cfg =
{   "task_bar",
	Task_Bar,
	0,			/* Core */
	200,		/* Stacksize (words) */
	1,			/* Priority */
	DV_EXEFLAG_BLOCKING	/* Flags */
};
const dv_execonfig_t task_qxx_cfg =
{   "task_qxx",
	Task_Qxx,
	0,			/* Core */
	200,		/* Stacksize (words) */
	2,			/* Priority */
	DV_EXEFLAG_BLOCKING	/* Flags */
};

/* This function does the job of main(). It runs in a thread, so it really shouldn't call dv_kprintf()
*/
void prj_init(void)
{
	dv_errorid_t e;
	dv_kernel_t *kvars;

	dv_kprintf("prj_init: started.\n");

	kvars = dv_get_kvars();

	/* Four GPIO pins for the LEDs.
	*/
	dv_arm_bcm2835_gpio_pinconfig(17, DV_pinfunc_output, DV_pinpull_none);
	dv_arm_bcm2835_gpio_pinconfig(18, DV_pinfunc_output, DV_pinpull_none);
	dv_arm_bcm2835_gpio_pinconfig(27, DV_pinfunc_output, DV_pinpull_none);
	dv_arm_bcm2835_gpio_pinconfig(22, DV_pinfunc_output, DV_pinpull_none);

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
