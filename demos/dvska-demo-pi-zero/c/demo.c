/* demo.c - demo program to test early davroska stuff on linux host
 *
 * (c) David Haworth
*/
#define DV_ASM	0
#include <dv-config.h>
#include <davroska-api.h>
#include <dv-stdio.h>
#include <dv-string.h>

#include <dv-arm-bcm2835-uart.h>
#include <dv-arm-bcm2835-interruptcontroller.h>

void main_Init(void);
void main_Loop(void);
void main_Ping(void);
void main_Pong(void);

dv_id_t Init, Loop, Ping, Pong;

dv_id_t Lock1;

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
	Init = dv_addtask("Init", &main_Init, 3, 1);
	Loop = dv_addtask("Loop", &main_Loop, 1, 1);
	Ping = dv_addtask("Ping", &main_Ping, 2, 1);
	Pong = dv_addtask("Pong", &main_Pong, 3, 3);
}

void callout_addlocks(dv_id_t mode)
{
	Lock1 = dv_addlock("Lock1", 1);
	{
		dv_addlockuser(Lock1, Ping);
		dv_addlockuser(Lock1, Pong);
	}
}

void callout_autostart(dv_id_t mode)
{
	dv_activatetask(Init);
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
	(void)dv_terminatetask();
}

void main_Loop(void)
{
	dv_statustype_t ee;

	dv_printf("Task Loop starting ...\n");

	for ( int i = 0; i < 10; i++ )
		if ( (ee = dv_activatetask(Ping)) != dv_e_ok )
			 dv_printf("Task Loop: dv_activatetask(Ping) returned %d\n", ee);

	/* Enable UART interrupt
	*/
	dv_init_interrupt_controller();
	dv_arm_bcm2835_uart.ier |= DV_IER_RxInt;
	dv_arm_bcm2835_interruptcontroller.irq_enable[0] = DV_INT_AUX;

	dv_printf("Task Loop terminating ...\n");
	(void)dv_terminatetask();
}

static unsigned char x;
static int pongCount;

void main_Ping(void)
{
	dv_statustype_t ee;

	dv_printf("Task Ping starting ...\n");

	if ( (ee = dv_chaintask(Loop)) != dv_e_limit )
		 dv_printf("Task Ping: dv_chaintask(Loop) returned %d\n", ee);

	x++;
	if ( x & 1 )
	{
		ee = dv_chaintask(Ping);
		dv_printf("Task Ping: dv_chaintask(Ping) returned unexpectedly : %d\n", ee);
	}
	else
	{
		pongCount = 0;

		if ( (ee = dv_takelock(Lock1)) != dv_e_ok )
			dv_printf("Task Ping: dv_takelock(Lock1) returned %d\n", ee);
		else
		{
			for ( int i = 0; i < 3; i++ )
			{
				if ( (ee = dv_activatetask(Pong)) != dv_e_ok )
				{
					dv_printf("Task Ping: dv_activatetask(Pong) returned %d\n", ee);
				}
				else
				{
					dv_printf("Task Ping: dv_activatetask(Pong) successful\n");
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
	dv_printf("Task Pong ... %d\n", pongCount++);

	(void)dv_terminatetask();
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

	dv_printf("pi-pzero starting ...\n");

	/* Copy the vector table to 0
	*/
	dv_memcpy32(0, &dv_vectortable, &dv_vectortable_end - &dv_vectortable);

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
