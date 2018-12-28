/* demo.c - demo program to test early davroska stuff on linux host
 *
 * (c) David Haworth
*/
#define DV_ASM	0
#include <dv-config.h>
#include <davroska.h>
#include <dv-stdio.h>
#include <dv-string.h>

#include <dv-arm-bcm2835-uart.h>
#include <dv-arm-bcm2835-aux.h>
#include <dv-arm-bcm2835-interruptcontroller.h>
#include <dv-armv6-mmu.h>
#include <dv-arm-cp15.h>
#include <dv-arm-bcm2835-systimer.h>

dv_u64_t start_time;

void main_Init(void);
void main_Loop(void);
void main_Ping(void);
void main_Pong(void);
void main_Uart(void);
void main_Cyclic(void);
static dv_u32_t acb_Cyclist(dv_id_t a);

dv_id_t Init, Loop, Ping, Pong, Cyclic;		/* Tasks */
dv_id_t Uart;								/* ISRs */
dv_id_t Lock1;								/* Locks */
dv_id_t Ticker;								/* Counters */
dv_id_t Cyclist;							/* Alarms */

/* Temporary: abt and undef stacks
*/
dv_u32_t dv_abtstack[256];
dv_u32_t dv_undstack[256];

int main(int argc, char **argv)
{
	dv_printf("davroska starting ...\n");

	dv_startos(0);

	return 0;
}

void callout_addtasks(dv_id_t mode)
{
	dv_printf("callout_addtasks()\n");
	Init = dv_addtask("Init", &main_Init, 3, 1);
	Loop = dv_addtask("Loop", &main_Loop, 1, 1);
	Ping = dv_addtask("Ping", &main_Ping, 2, 1);
	Pong = dv_addtask("Pong", &main_Pong, 3, 3);
	Cyclic = dv_addtask("Cyclic", &main_Cyclic, 3, 1);
	dv_printf("callout_addtasks() - done\n");
}

void callout_addisrs(dv_id_t mode)
{
	dv_printf("callout_addisrs()\n");
	Uart = dv_addisr("Uart", &main_Uart, dv_iid_aux, 9);
	dv_printf("callout_addisrs() - done\n");
}

void callout_addlocks(dv_id_t mode)
{
	Lock1 = dv_addlock("Lock1", 1);
	{
		dv_addlockuser(Lock1, Ping);
		dv_addlockuser(Lock1, Pong);
	}
}

void callout_addcounters(dv_id_t mode)
{
	Ticker = dv_addcounter("Ticker");
}

void callout_addalarms(dv_id_t mode)
{
	Cyclist = dv_addalarm("Cyclist", &acb_Cyclist);
}

void callout_autostart(dv_id_t mode)
{
	dv_activatetask(Init);
	dv_setalarm_rel(Ticker, Cyclist, 2);
}

void main_Init(void)
{
	dv_statustype_t ee;

	dv_printf("Task Init starting ...\n");

	if ( (ee = dv_activatetask(Loop)) != dv_e_ok )
		dv_printf("Task Init: dv_activatetask(Loop) returned %d\n", ee);

	if ( (ee = dv_activatetask(Ping)) != dv_e_ok )
		dv_printf("Task Init: dv_activatetask(Ping) returned %d\n", ee);

	dv_printf("Task Init terminating ...\n");

	dv_printf("Task Init: Info: diff time in task Ping is termination time ...\n");
	start_time = dv_readtime();
	(void)dv_terminatetask();
}

void main_Loop(void)
{
	dv_statustype_t ee;

	dv_printf("Task Loop starting ...\n");

	dv_printf("Task Init: Info: diff time in task Ping is activation_high time ...\n");
	start_time = dv_readtime();
	if ( (ee = dv_activatetask(Ping)) != dv_e_ok )
		 dv_printf("Task Loop: dv_activatetask(Ping) returned %d\n", ee);

#if 1
	dv_printf("Task Loop: enabling UART RxInt. Old ier value: 0x%02x\n", dv_arm_bcm2835_uart.ier);
	dv_arm_bcm2835_uart.ier |= DV_IER_RxInt;
#endif
#if 1
	dv_printf("Task Loop: dv_enable_irq() %d\n", dv_iid_aux);
	dv_enable_irq(dv_iid_aux);
#endif

	dv_printf("Task Loop terminating ...\n");
	(void)dv_terminatetask();
}

static unsigned char x;
static int pongCount;

void main_Ping(void)
{
	dv_statustype_t ee;
	dv_u64_t end_time = dv_readtime();
	dv_u32_t diff = (dv_u32_t)(end_time - start_time);

	dv_printf("Task Ping starting ... diff time = %u\n", diff);

	x++;
	if ( x & 1 )
	{
		dv_printf("Task Ping chaining Ping ...\n");
		dv_printf("Task Ping: Info: diff time in task Ping is chain self time ...\n");
		start_time = dv_readtime();
		ee = dv_chaintask(Ping);
		dv_printf("Task Ping: dv_chaintask(Ping) returned unexpectedly : %d\n", ee);
	}
	else
	{
		pongCount = 0;

		start_time = dv_readtime();
		if ( (ee = dv_takelock(Lock1)) != dv_e_ok )
			dv_printf("Task Ping: dv_takelock(Lock1) returned %d\n", ee);
		else
		{
			dv_u32_t diff = (dv_u32_t)(dv_readtime() - start_time);
			dv_printf("Task Ping: time to take a lock = %u\n", diff);
			for ( int i = 0; i < 3; i++ )
			{
				start_time = dv_readtime();
				if ( (ee = dv_activatetask(Pong)) != dv_e_ok )
				{
					dv_printf("Task Ping: dv_activatetask(Pong) returned %d\n", ee);
				}
				else
				{
					diff = (dv_u32_t)(dv_readtime() - start_time);
					dv_printf("Task Ping: dv_activatetask(Pong) successful\n");
					dv_printf("Task Ping: activation time (lower) = %u\n", diff);
				}
			}

			if ( (ee = dv_droplock(Lock1)) != dv_e_ok )
				dv_printf("Task Ping: dv_droplock(Lock1) returned %d\n", ee);
		}
	}

	dv_printf("Task Ping terminating ...\n");
	(void)dv_terminatetask();
}

void main_Pong(void)
{
	dv_printf("Task Pong ... %d %ld\n", pongCount++, dv_readtime());

	(void)dv_terminatetask();
}

static int cycleCount;
void main_Cyclic(void)
{
	dv_printf("Task Cyclic ... %d %ld\n", cycleCount++, dv_readtime());

	(void)dv_terminatetask();
}

void main_Uart(void)
{
	dv_printf("ISR Uart start\n");

	while ( dv_consoledriver.isrx() )
	{
		int c = dv_consoledriver.getc();

		dv_printf("ISR Uart - 0x%02x\n", c);

		if ( c == 'P' )
		{
			dv_statustype_t ee = dv_activatetask(Ping);
			if ( ee != dv_e_ok )
				dv_printf("ISR Uart : dv_activatetask(Ping) returned %d\n", ee);
		}
		if ( c == 'Q' )
		{
			dv_statustype_t ee = dv_activatetask(Pong);
			if ( ee != dv_e_ok )
				dv_printf("ISR Uart : dv_activatetask(Pong) returned %d\n", ee);
		}
		if ( c == 'T' )
		{
			dv_statustype_t ee = dv_advancecounter(Ticker, 1);
			if ( ee != dv_e_ok )
				dv_printf("ISR Uart : dv_advancecounter(Ticker) returned %d\n", ee);
		}
	}
	dv_printf("ISR Uart return\n");

		
}

static dv_u32_t acb_Cyclist(dv_id_t a)
{
	dv_activatetask(Cyclic);
	return 5;
}

void callout_error(dv_statustype_t e)
{
	dv_printf("callout_error(%d) called\n", e);
}

/* Startup and exception handling
*/
extern dv_u32_t dv_start_bss, dv_end_bss, dv_vectortable, dv_vectortable_end;

const dv_u32_t dv_initialsp_abt = (dv_u32_t)&dv_abtstack[256];
const dv_u32_t dv_initialsp_und = (dv_u32_t)&dv_undstack[256];

void dv_board_start(void)
{
	/* Initialise bss
	*/
	dv_memset32(&dv_start_bss, 0,
		((dv_address_t)&dv_end_bss - (dv_address_t)&dv_start_bss + sizeof(dv_u32_t) - 1) / sizeof(dv_u32_t));

	/* Initialise uart and connect it to the stdio functions
	*/
	dv_arm_bcm2835_uart_init(115200, 8, 0);
	dv_arm_bcm2835_uart_console();

	dv_printf("pi-zero starting ...\n");

	/* Copy the vector table to 0
	*/
	dv_memcpy32(0, &dv_vectortable, &dv_vectortable_end - &dv_vectortable);

	/* Set up the MMU
	*/
	dv_armv6_mmu_setup();

	/* Caches
	*/
	dv_printf("CP15 cache type 0x%08x\n", dv_read_cp15_cache_type());

#if 1
	/* Enable both caches and the write buffer
	*/
	dv_printf("Enabling caches ...\n");
	dv_write_cp15_control(dv_read_cp15_control() | DV_CP15_CTRL_C | DV_CP15_CTRL_W | DV_CP15_CTRL_I);

	/* Enable branch prediction
	*/
	dv_printf("Enabling branch prediction ...\n");
	dv_write_cp15_control(dv_read_cp15_control() | DV_CP15_CTRL_Z);
#endif

	main(0, 0);
}

void dv_catch_data_abort(void)
{
    dv_printf("%s --- %s\n", "dv_catch_data_abort", "Oops! An exception occurred");
	for (;;) {}
}

void dv_catch_prefetch_abort(void)
{
    dv_printf("%s --- %s\n", "dv_catch_prefetch_abort", "Oops! An exception occurred");
	for (;;) {}
}

void dv_catch_reserved(void)
{
    dv_printf("%s --- %s\n", "dv_catch_reserved", "Oops! An exception occurred");
	for (;;) {}
}

void dv_catch_undef(void)
{
    dv_printf("%s --- %s\n", "dv_catch_undef", "Oops! An exception occurred");
	for (;;) {}
}

void dv_catch_unimplemented(void)
{
    dv_printf("%s --- %s\n", "dv_trap_unimplemented", "Oops! An exception occurred");
	for (;;) {}
}

void dv_catch_sbreak(void)
{
    dv_printf("%s --- %s\n", "dv_trap_sbreak", "Oops! An exception occurred");
	for (;;) {}
}

void dv_catch_fiq(void)
{
    dv_printf("%s --- %s\n", "dv_trap_fiq", "Oops! An exception occurred");
	for (;;) {}
}

void dv_catch_reset(void)
{
    dv_printf("%s --- %s\n", "dv_trap_reset", "Oops! Return from dv_board_start");
	for (;;) {}
}

#if 0
void dv_catch_irq(void)
{
    dv_printf("%s --- %s\n", "dv_trap_irq", "An interrupt!");
}
#endif

void dv_panic_failed_return_from_irq(void)
{
	dv_printf("%s --- %s\n", "dv_trap_irq", "Oops! Failed to return from an IRQ");
}

#if 1
void print_interrupt_status(dv_bcm2835_imask_t *enabled)
{
	dv_printf("Interrupt status:\n");
	dv_printf("   address: 0x%08x 0x%08x 0x%08x -- 0x%08x 0x%08x\n",
			(unsigned)&dv_arm_bcm2835_interruptcontroller.irq_pending[0],
			(unsigned)&dv_arm_bcm2835_interruptcontroller.irq_pending[1],
			(unsigned)&dv_arm_bcm2835_interruptcontroller.basic_pending,
			(unsigned)&dv_arm_bcm2835_aux.irq,
			(unsigned)&dv_arm_bcm2835_uart.iir);
	dv_printf("   pending: 0x%08x 0x%08x 0x%08x - 0x%02x 0x%02x\n",
            dv_arm_bcm2835_interruptcontroller.irq_pending[0],
            dv_arm_bcm2835_interruptcontroller.irq_pending[1],
            dv_arm_bcm2835_interruptcontroller.basic_pending,
            dv_arm_bcm2835_aux.irq,
            dv_arm_bcm2835_uart.iir);

	if ( enabled == DV_NULL ) return;

	dv_printf("   enabled: 0x%08x 0x%08x 0x%08x\n",
			enabled->mask[0],
			enabled->mask[1],
			enabled->mask[2]);
}
#endif
