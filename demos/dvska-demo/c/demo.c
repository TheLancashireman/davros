/* demo.c - demo for davroska
 *
 * This file contains all the target-independent stuff - tasks, ISRs etc.
 * The target-dependent stuff (startup, main, timer & uart functions) can be found in
 * hardware specific header/source files.
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
 *
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
 * The final Led's state is controlled by a pseudo-random noise generator in the Bit3 task.
 * Bit3 is an extended task that expects to be sent the 
 * These messages are sent as a somewhat irregular rate that is generated by changing the return value
 * of the alarm trigger function that sends the event.
*/
/* Object identifiers
*/
dv_id_t Init, Led, Bit0, Bit1, Bit2, Bit3;	/* Tasks */
dv_id_t Uart, Timer;						/* ISRs */
dv_id_t mx_Gpio;							/* Mutexes */
dv_id_t Ticker;								/* Counters */
dv_id_t BitDriver, FlickerDriver;			/* Alarms */

dv_boolean_t ledstate[4];
const dv_eventmask_t ev_Flicker = 0x01;	/* Calculates ledstate[3] */
const dv_eventmask_t ev_Update = 0x08;	/* Transfers led states to I/O pins */

/* main_Led() - task body function for the Led task
*/
void main_Led(void)
{
	dv_statustype_t ee;
	dv_eventmask_t events;

	for (;;)
	{
		if ( (ee = dv_waitevent(ev_Update)) != dv_e_ok )
			dv_shutdown(ee);
		if ( (ee = dv_getevent(Led, &events)) != dv_e_ok )
			dv_shutdown(ee);
		if ( (ee = dv_clearevent(events)) != dv_e_ok )
			dv_shutdown(ee);

		if ( (events & ev_Update) != 0 )
		{
			if ( (ee = dv_takemutex(mx_Gpio)) != dv_e_ok )
				dv_shutdown(ee);

			char cc[4];

			for (int i = 0; i < 4; i++ )
			{
				hw_SetLed(i, ledstate[i]);
				cc[i] = ledstate[i] ? '*' : '-';
			}

#if DEMO_BOARD==DEMO_PI_ZERO
			dv_printf("    %c %c %c %c\r", cc[3], cc[2], cc[1], cc[0]);
#elif DEMO_BOARD==DEMO_PI3_ARM64
			extern volatile char core_state[];
			dv_printf("    %c %c %c %c %c %c %c\r", cc[3], cc[2], cc[1], cc[0],
													core_state[1], core_state[2], core_state[3]);
#elif DEMO_BOARD==DEMO_BLUE_PILL
			dv_printf("    %c %c %c %c\r", cc[3], cc[2], cc[1], cc[0]);
#endif

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

	if ( (ee = dv_setevent(Led, ev_Update)) != dv_e_ok )
		dv_shutdown(ee);

	dv_takemutex(mx_Gpio);		/* ToDo: remove this test thing */
	ee = dv_terminatetask();
	dv_shutdown(ee);
}

/* main_Bit3() - task body function for the Bit3 task
*/
void main_Bit3(void)
{
	dv_statustype_t ee;
	dv_eventmask_t events;
	unsigned shiftreg = 0xcccc;

	for (;;)
	{
		if ( (ee = dv_waitevent(ev_Flicker)) != dv_e_ok )
			dv_shutdown(ee);
		if ( (ee = dv_getevent(Bit3, &events)) != dv_e_ok )
			dv_shutdown(ee);
		if ( (ee = dv_clearevent(events)) != dv_e_ok )
			dv_shutdown(ee);

		if ( events & ev_Flicker )
		{
			if ( (shiftreg & 1) != 0 )
			{
				if ( ledstate[3] == 0 )
				{
					ledstate[3] = 1;
					if ( (ee = dv_setevent(Led, ev_Update)) != dv_e_ok )
						dv_shutdown(ee);
				}

				if ( (shiftreg & 8) == 0 )
					shiftreg |= 0x20000;
			}
			else
			{
				if ( ledstate[3] != 0 )
				{
					ledstate[3] = 0;
					if ( (ee = dv_setevent(Led, ev_Update)) != dv_e_ok )
						dv_shutdown(ee);
				}

				if ( (shiftreg & 8) != 0 )
					shiftreg |= 0x20000;
			}

			shiftreg = shiftreg >> 1;
		}
	}
}

/* main_Init() - start the ball rolling
*/
void main_Init(void)
{
	dv_printf("main_Init() reached\n");
	sysinfo();
	for ( int i = 0; i < dv_nexe; i++ )
	{
		dv_printf("%d %s  b=%d r=%d c=%d %d\n", i, dv_exe[i].name, dv_exe[i].baseprio, dv_exe[i].runprio,
													dv_exe[i].currprio, dv_exe[i].state);
	}
#if DEMO_BOARD == DEMO_BLUE_PILL
	dv_nvic_triggerirq(0);
#endif
}

/* main_Uart() - body of ISR to handle uart rx interrupt
*/
void main_Uart(void)
{
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
	hw_ClearTimer();

	dv_statustype_t ee = dv_advancecounter(Ticker, 1);
	if ( ee != dv_e_ok )
		dv_shutdown(ee);
}

/* af_BitDriver() - alarm function to activate the Bit0 task every 1000 ticks
*/
dv_u64_t af_BitDriver(dv_id_t a, dv_param_t unused_d)
{
	dv_activatetask(Bit0);
	return 1000;
}

/* af_FlickerDriver() - alarm function to send the event to calculate Led3 every 89 ticks
*/
dv_u64_t af_FlickerDriver(dv_id_t a, dv_param_t unused_d)
{
	dv_setevent(Bit3, ev_Flicker);
	return 89;
}

/* callout_addtasks() - configure the tasks
*/
void callout_addtasks(dv_id_t mode)
{
	Init = dv_addtask("Init", &main_Init, 4, 1);
	Led = dv_addextendedtask("Led", &main_Led, 1, 8192);
	Bit0 = dv_addtask("Bit0", &main_Bit0, 2, 1);
	Bit1 = dv_addtask("Bit1", &main_Bit1, 2, 1);
	Bit2 = dv_addtask("Bit2", &main_Bit2, 2, 1);
	Bit3 = dv_addextendedtask("Bit3", &main_Bit3, 3, 8192);
}

/* callout_addisrs() - configure the isrs
*/
void callout_addisrs(dv_id_t mode)
{
#if DEMO_BOARD == DEMO_BLUE_PILL
/* Interrupts not implemented yet
*/
#else
	Uart = dv_addisr("Uart", &main_Uart, hw_UartInterruptId, 7);
	Timer = dv_addisr("Timer", &main_Timer, hw_TimerInterruptId, 8);
#endif
}

/* callout_addgroups() - configure the executable groups
 *
 * ToDo: remove the contents - just for testing
*/
void callout_addgroups(dv_id_t mode)
{
	dv_startgroup("NONPRE", 1);
	{
		dv_addtogroup(Init);
		dv_addtogroup(Bit0);
		dv_finishgroup();
	}

	dv_startgroup("Silly", 0);
	{
		dv_addtogroup(Bit0);
		dv_addtogroup(Bit1);
		dv_addtogroup(Bit3);
		dv_finishgroup();
	}
#if DEMO_BOARD == DEMO_BLUE_PILL
/* Interrupts not implemented yet
*/
#else
	dv_startgroup("Sillier", 0);
	{
		dv_addtogroup(Init);
		dv_addtogroup(Uart);
		dv_finishgroup();
	}
#endif
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
	BitDriver = dv_addalarm("BitDriver", &af_BitDriver, 0);
	FlickerDriver = dv_addalarm("FlickerDriver", &af_FlickerDriver, 0);
}

/* callout_autostart() - start the objects that need to be running after dv_startos()
*/
void callout_autostart(dv_id_t mode)
{
	dv_activatetask(Init);
	dv_activatetask(Led);
	dv_activatetask(Bit3);
	dv_setalarm_rel(Ticker, BitDriver, 1000);
	dv_setalarm_rel(Ticker, FlickerDriver, 1700);

#if DEMO_BOARD == DEMO_BLUE_PILL
	/* Interrupt handling not implemented yet. Temporary test stuff
	*/
	dv_nvic_setprio(0, 12);
	dv_nvic_enableirq(0);
#else
	/* Enable interrupts from the UART
	*/
	hw_EnableUartRxInterrupt();
	dv_enable_irq(hw_UartInterruptId);

	hw_InitialiseMillisecondTicker();
	dv_enable_irq(hw_TimerInterruptId);
#endif
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

	for (;;) {} /* Temporary */
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
	dv_printf("Idle loop reached\n");
	for (;;) {}
}

/* callout_panic() - called from dv_panic
*/
void callout_panic(dv_panic_t p, dv_sid_t sid, char *fault)
{
	dv_printf("Panic %d in %d : %s\n", p, sid, fault);
}

/* main() - well, it's main, innit?
*/
int main(int argc, char **argv)
{
	dv_printf("davroska starting ...\n");

	sysinfo();

	dv_startos(0);

	return 0;
}
