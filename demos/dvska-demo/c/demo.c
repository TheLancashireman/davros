/* demo.c - demo for davroska
 *
 * This file contains all the target-independent stuff - tasks, ISRs etc.
 * The target-dependent stuff (startup, main, timer & uart functions) can be found in
 * hardware specific header/source files.
 *
 * (c) David Haworth
*/
#define DV_ASM	0
#include <dv-config.h>
#include <davroska.h>
#include <dv-stdio.h>
#include <dv-string.h>

/* This include file selects the hardware type
*/
#include DV_DEMO_TARGET

/* Task main functions, along with a description of how they interact.
 *
 * The hardware is assumed to have 4 LEDs that can be driven by output ports.
 *
 * The task Led is responsible for updating the set of LEDs. It is an extended task running at a fairly low
 * priority. It is the only task that actually modifies the state of the ports that drive the LEDs, so
 * no co-ordination is required for the hardware. However, it needs to get a consistent state of the
 * LED bits, so it uses a mutex that is shared with the three bit-flip tasks.
 *
 * The tasks Bit0, Bit1 and Bit2 are each responsible for "incrementing" a single bit of an LED state counter.
 * 		"Incrementing" means inverting the state:
 *			if the new state is 0 (carry) the next bit task is chained.
 *			if the new state is 1 an update event is sent to the Led task.
 *		Bit0 is activated by an alarm once per second.
 *		Bit2 has no carry, so it always sends the event.
 *
 * The final Led's state is controlled by sending flip messages to the Led task.
 * These messages are sent as a somewhat irregular rate that is generated by changing the return value
 * of the alarm trigger function that sends the event.
*/
/* Object identifiers
*/
dv_id_t Init, Led, Bit0, Bit1, Bit2;		/* Tasks */
dv_id_t Uart, Timer;						/* ISRs */
dv_id_t mx_Gpio;							/* Mutexes */
dv_id_t Ticker;								/* Counters */
dv_id_t BitDriver, FlickerDriver;			/* Alarms */

dv_boolean_t ledstate[4];
const dv_eventmask_t ev_Flip3 = 0x01;	/* Flips ledstate[3] */
const dv_eventmask_t ev_Update = 0x08;	/* Transfers led states to I/O pins */

/* main_Led() - task body function for the Led task
*/
void main_Led(void)
{
	dv_statustype_t ee;
	dv_eventmask_t events;

	dv_printf("Led : SP = 0x%08x\n", dv_arm_get_SP());

	for (;;)
	{
		if ( (ee = dv_waitevent(ev_Flip3 | ev_Update)) != dv_e_ok )
			dv_shutdown(ee);
		if ( (ee = dv_getevent(Led, &events)) != dv_e_ok )
			dv_shutdown(ee);
		if ( (ee = dv_clearevent(events)) != dv_e_ok )
			dv_shutdown(ee);

		if ( (events & ev_Flip3) != 0 )
		{
			/* Flip led 3 and force an update
			*/
			ledstate[3] = !ledstate[3];
			events |= ev_Update;
		}

		if ( (events & ev_Update) != 0 )
		{
			if ( (ee = dv_takemutex(mx_Gpio)) != dv_e_ok )
				dv_shutdown(ee);

			for (int i = 0; i < 3; i++ )
				hw_SetLed(i, ledstate[i]);

			if ( (ee = dv_dropmutex(mx_Gpio)) != dv_e_ok )
				dv_shutdown(ee);
		}
	}
}

/* main_Bit0() - task body function for the Bit0 task
*/
void main_Bit0(void)
{
	dv_statustype_t ee;

	ledstate[0] = !ledstate[0];

	dv_printf("Bit0: %d\n", ledstate[0]);

	if ( ledstate[0] )
	{
		if ( (ee = dv_setevent(Led, ev_Update)) != dv_e_ok )
			dv_shutdown(ee);
		ee = dv_terminatetask();
		dv_shutdown(ee);
	}
	else
	{
		ee = dv_chaintask(Bit1);
		dv_shutdown(ee);
	}
}

/* main_Bit1() - task body function for the Bit1 task
*/
void main_Bit1(void)
{
	dv_statustype_t ee;

	ledstate[1] = !ledstate[1];

	dv_printf("Bit1: %d\n", ledstate[1]);

	if ( ledstate[1] )
	{
		if ( (ee = dv_setevent(Led, ev_Update)) != dv_e_ok )
			dv_shutdown(ee);
		ee = dv_terminatetask();
		dv_shutdown(ee);
	}
	else
	{
		ee = dv_chaintask(Bit2);
		dv_shutdown(ee);
	}
}

/* main_Bit2() - task body function for the Bit2 task
*/
void main_Bit2(void)
{
	dv_statustype_t ee;

	ledstate[2] = !ledstate[2];

	dv_printf("Bit2: %d\n", ledstate[2]);

	if ( (ee = dv_setevent(Led, ev_Update)) != dv_e_ok )
		dv_shutdown(ee);
	ee = dv_terminatetask();
	dv_shutdown(ee);
}

/* main_Init() - start the ball rolling
*/
void main_Init(void)
{
	dv_printf("Init : SP = 0x%08x\n", dv_arm_get_SP());
}

/* main_Uart() - body of ISR to handle uart rx interrupt
*/
void main_Uart(void)
{
	dv_printf("Uart : SP = 0x%08x\n", dv_arm_get_SP());

	while ( dv_consoledriver.isrx() )
	{
		int c = dv_consoledriver.getc();

		dv_printf("uart rx : 0x%02x\n", c);
	}
}

/* main_Timer() - body of ISR to handle interval timer interrupt
*/
void main_Timer(void)
{
	dv_u32_t sp = dv_arm_get_SP();
	if ( sp < 0x10000000 )
		dv_printf("Timer : SP = 0x%08x\n", sp);
	hw_ClearTimer();

	dv_statustype_t ee = dv_advancecounter(Ticker, 1);
	if ( ee != dv_e_ok )
		dv_shutdown(ee);
}

/* af_BitDriver() - alarm function to activate the Bit0 task every 1000 ticks
*/
dv_u32_t af_BitDriver(dv_id_t a)
{
	dv_activatetask(Bit0);
	return 1000;
}

/* af_FlickerDriver() - alarm function to send the event to toggle Led3 at irregular intervals
 *
 * The times are chosen sequentially from an array of prime number multiples of 100. The array has an
 * odd number of elements so that the even passes through the array give different flash times from the odd passes.
*/
dv_u32_t af_FlickerDriver(dv_id_t a)
{
	const dv_u32_t flicker_times[7] = {300, 2300, 500, 1300, 700, 1300, 1100};
	static int step;

	dv_setevent(Led, ev_Flip3);
	step++;
	if ( step >= 7 ) step = 0;
	return flicker_times[step];
}

/* callout_addtasks() - configure the tasks
*/
void callout_addtasks(dv_id_t mode)
{
	Init = dv_addtask("Init", &main_Init, 3, 1);
	Led = dv_addextendedtask("Led", &main_Led, 1, 1024);
	Bit0 = dv_addtask("Bit0", &main_Bit0, 4, 1);
	Bit1 = dv_addtask("Bit1", &main_Bit1, 4, 1);
	Bit2 = dv_addtask("Bit2", &main_Bit2, 4, 1);
}

/* callout_addisrs() - configure the isrs
*/
void callout_addisrs(dv_id_t mode)
{
	Uart = dv_addisr("Uart", &main_Uart, hw_UartInterruptId, 7);
	Timer = dv_addisr("Timer", &main_Timer, hw_TimerInterruptId, 8);
}

/* callout_addmutexes() - configure the mutexes
*/
void callout_addmutexes(dv_id_t mode)
{
	mx_Gpio = dv_addmutex("mx_Gpio", 1);
	{
		dv_addmutexuser(mx_Gpio, Led);
		dv_addmutexuser(mx_Gpio, Bit0);
		dv_addmutexuser(mx_Gpio, Bit1);
		dv_addmutexuser(mx_Gpio, Bit2);
	}
}

/* callout_addcounters() - configure the counters
*/
void callout_addcounters(dv_id_t mode)
{
	Ticker = dv_addcounter("Ticker");
}

/* callout_addalarms() - configure the alarms
*/
void callout_addalarms(dv_id_t mode)
{
	BitDriver = dv_addalarm("BitDriver", &af_BitDriver);
	FlickerDriver = dv_addalarm("FlickerDriver", &af_FlickerDriver);
}

/* callout_autostart() - start the objects that need to be running after dv_startos()
*/
void callout_autostart(dv_id_t mode)
{
	dv_activatetask(Init);
	dv_activatetask(Led);
	dv_setalarm_rel(Ticker, BitDriver, 1000);
	dv_setalarm_rel(Ticker, FlickerDriver, 1700);

	/* Enable interrupts from the UART
	*/
	hw_EnableUartRxInterrupt();
	dv_enable_irq(hw_UartInterruptId);

	hw_InitialiseMillisecondTicker();
	dv_enable_irq(hw_TimerInterruptId);
}

/* callout_reporterror() - called if an error is detected
*/
dv_statustype_t callout_reporterror(dv_sid_t sid, dv_statustype_t e, dv_qty_t nparam, dv_param_t *param)
{
	dv_printf("callout_reporterror(%d, %d, %d, ...) called.\n", sid, e, nparam);
	for (int i = 0; i < nparam; i++ )
	{
		/* Only print the lower 32 bits of the parameters
		*/
		dv_printf("    param[%d] = %d (0x%08x)\n", i, (dv_u32_t)param[i], (dv_u32_t)param[i]);
	}
	return e;
}

/* callout_shutdown() - called on shutdown
*/
void callout_shutdown(dv_statustype_t e)
{
	dv_printf("callout_shutdown: %d\n", e);
}

/* callout_idle() - called in idle loop
*/
void callout_idle(void)
{
	dv_intstatus_t is = dv_disable();
	dv_u32_t sp = dv_arm_get_SP();
	dv_printf("callout_idle: SP = 0x%08x\n", sp);
	dv_restore(is);

	for ( volatile int i = 0; i < 3000000; i++ )
	{
	}
}

/* main() - well, it's main, innit?
*/
int main(int argc, char **argv)
{
	dv_printf("davroska starting ...\n");

	dv_startos(0);

	return 0;
}
