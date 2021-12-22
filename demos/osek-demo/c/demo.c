/* demo.c - demo for davroska (OSEK version)
 *
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
 * This file contains all the target-independent stuff - tasks, ISRs etc.
 * The target-dependent stuff (startup, main, timer & uart functions) can be found in
 * hardware specific header/source files.
 *
 * (c) David Haworth
*/
#define DV_ASM	0
#include <os.h>

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
DeclareTask(Led);
DeclareTask(Bit0);
DeclareTask(Bit1);
DeclareTask(Bit2);
DeclareTask(Bit3);
DeclareResource(mx_Gpio);

dv_boolean_t ledstate[4];

/* TASK(Led) - task body function for the Led task
*/
TASK(Led)
{
	StatusType ee;
	EventMaskType events;

	for (;;)
	{
		if ( (ee = WaitEvent(ev_Update)) != E_OK )
			ShutdownOS(ee);
		if ( (ee = GetEvent(Led, &events)) != E_OK )
			ShutdownOS(ee);
		if ( (ee = ClearEvent(events)) != E_OK )
			ShutdownOS(ee);

		if ( (events & ev_Update) != 0 )
		{
			if ( (ee = GetResource(mx_Gpio)) != E_OK )
				ShutdownOS(ee);

			char cc[4];

			for (int i = 0; i < 4; i++ )
			{
				hw_SetLed(i, ledstate[i]);
				cc[i] = ledstate[i] ? '*' : '-';
			}

#if DEMO_BOARD==DEMO_PI3_ARM64
			extern volatile char core_state[];
			dv_printf("    %c %c %c %c %c %c %c\r", cc[3], cc[2], cc[1], cc[0],
													core_state[1], core_state[2], core_state[3]);
#else
			dv_printf("    %c %c %c %c\r", cc[3], cc[2], cc[1], cc[0]);
#endif

			if ( (ee = ReleaseResource(mx_Gpio)) != E_OK )
				ShutdownOS(ee);
		}
	}
}

/* TASK(Bit0) - task body function for the Bit0 task
*/
TASK(Bit0)
{
	StatusType ee;

	ledstate[0] = !ledstate[0];

	if ( ledstate[0] )
	{
		if ( (ee = SetEvent(Led, ev_Update)) != E_OK )
			ShutdownOS(ee);
		ee = TerminateTask();
		ShutdownOS(ee);
	}
	else
	{
		ee = ChainTask(Bit1);
		ShutdownOS(ee);
	}
}

/* TASK(Bit1) - task body function for the Bit1 task
*/
TASK(Bit1)
{
	StatusType ee;

	ledstate[1] = !ledstate[1];

	if ( ledstate[1] )
	{
		if ( (ee = SetEvent(Led, ev_Update)) != E_OK )
			ShutdownOS(ee);
		ee = TerminateTask();
		ShutdownOS(ee);
	}
	else
	{
		ee = ChainTask(Bit2);
		ShutdownOS(ee);
	}
}

/* TASK(Bit2) - task body function for the Bit2 task
*/
TASK(Bit2)
{
	StatusType ee;

	ledstate[2] = !ledstate[2];

	if ( (ee = SetEvent(Led, ev_Update)) != E_OK )
		ShutdownOS(ee);

	GetResource(mx_Gpio);		/* ToDo: remove this test thing */
	ee = TerminateTask();
	ShutdownOS(ee);
}

/* TASK(Bit3) - task body function for the Bit3 task
*/
TASK(Bit3)
{
	StatusType ee;
	EventMaskType events;
	unsigned shiftreg = 0xcccc;

	for (;;)
	{
		if ( (ee = WaitEvent(ev_Flicker)) != E_OK )
			ShutdownOS(ee);
		if ( (ee = GetEvent(Bit3, &events)) != E_OK )
			ShutdownOS(ee);
		if ( (ee = ClearEvent(events)) != E_OK )
			ShutdownOS(ee);

		if ( events & ev_Flicker )
		{
			if ( (shiftreg & 1) != 0 )
			{
				if ( ledstate[3] == 0 )
				{
					ledstate[3] = 1;
					if ( (ee = SetEvent(Led, ev_Update)) != E_OK )
						ShutdownOS(ee);
				}

				if ( (shiftreg & 8) == 0 )
					shiftreg |= 0x20000;
			}
			else
			{
				if ( ledstate[3] != 0 )
				{
					ledstate[3] = 0;
					if ( (ee = SetEvent(Led, ev_Update)) != E_OK )
						ShutdownOS(ee);
				}

				if ( (shiftreg & 8) != 0 )
					shiftreg |= 0x20000;
			}

			shiftreg = shiftreg >> 1;
		}
	}

	ee = TerminateTask();
	ShutdownOS(ee);
}

/* TASK(Init) - start the ball rolling
*/
TASK(Init)
{
	for ( int i = 0; i < dv_nexe; i++ )
	{
		dv_printf("%d %s  b=%d r=%d c=%d %d\n", i, dv_exe[i].name, dv_exe[i].baseprio, dv_exe[i].runprio,
													dv_exe[i].currprio, dv_exe[i].state);
	}
}

/* ISR(Uart) - body of ISR to handle uart rx interrupt
*/
ISR(Uart)
{
	while ( dv_consoledriver.isrx() )
	{
		int c = dv_consoledriver.getc();

		dv_printf("uart rx : 0x%02x\n", c);
	}
}

/* ISR(Timer) - body of ISR to handle interval timer interrupt
*/
ISR(Timer)
{
	hw_ClearTimer();

	dv_statustype_t ee = dv_advancecounter(Ticker, 1);
	if ( ee != dv_e_ok )
		ShutdownOS(ee);
}

#if 1
/* ErrorHook() - called if an error is detected
*/
void ErrorHook(StatusType e)
{
	dv_printf("ErrorHook(%d) called.\n", e);
	dv_printf("    - sid = %d\n", dv_lasterror.sid);
	for (int i = 0; i < dv_lasterror.n_param; i++ )
	{
		/* Only print the lower 32 bits of the parameters
		*/
		dv_printf("    param[%d] = %d (0x%08x)\n", i, (dv_u32_t)dv_lasterror.p[i], (dv_u32_t)dv_lasterror.p[i]);
	}
}

#else
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
#endif

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

	StartOS(OSDEFAULTAPPMODE);

	return 0;
}
