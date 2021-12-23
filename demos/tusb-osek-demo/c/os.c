/* os.c -  "generated" OS source file
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
#include <davroska-osek.h>
#include <os.h>

#include DV_DEMO_TARGET

/* Object identifiers
*/
TaskType Init, Led, Bit0, Bit1, Bit2, Bit3;		/* Tasks */
ResourceType mx_Gpio;							/* Mutexes */
AlarmType BitDriver, FlickerDriver;				/* Alarms */
dv_id_t Uart, Timer;							/* ISRs */
dv_id_t Ticker;									/* Davroska counters */
dv_id_t OsekCounter;							/* OSEK counters */

const EventMaskType ev_Flicker = 0x01;
const EventMaskType ev_Update = 0x08;

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
	Uart = dv_addisr("Uart", &main_Uart, hw_UartInterruptId, 7);
	Timer = dv_addisr("Timer", &main_Timer, hw_TimerInterruptId, 8);
}

/* callout_addgroups() - configure the executable groups
*/
void callout_addgroups(dv_id_t mode)
{
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

	OsekCounter = dv_addosekcounter("OsekCounter", Ticker, 9999, 10);
}

/* callout_addalarms() - configure the alarms
*/
void callout_addalarms(dv_id_t mode)
{
	BitDriver = dv_addosekalarm_task("BitDriver", OsekCounter, Bit0, 0);
	FlickerDriver = dv_addosekalarm_task("FlickerDriver", OsekCounter, Bit3, ev_Flicker);
}

/* callout_autostart() - start the objects that need to be running after StartOS()
*/
void callout_autostart(dv_id_t mode)
{
	ActivateTask(Init);
	ActivateTask(Led);
	ActivateTask(Bit3);
	SetRelAlarm(BitDriver, 1000, 1000);
	SetRelAlarm(FlickerDriver, 1700, 89);

	/* Enable interrupts from the UART
	*/
	hw_EnableUartRxInterrupt();
	dv_enable_irq(hw_UartInterruptId);

	hw_InitialiseMillisecondTicker();
	dv_enable_irq(hw_TimerInterruptId);
}
