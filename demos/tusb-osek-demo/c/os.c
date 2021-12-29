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
ISRType Uart, Timer;							/* ISRs */
dv_id_t Ticker;									/* Davroska counters */
CounterType OsekCounter;						/* OSEK counters */

#if DV_TUSB_HOST
dv_id_t tusb_HostTask;
dv_id_t tusb_HostAlarm;
#endif
#if DV_TUSB_DEVICE
dv_id_t tusb_DeviceTask;
dv_id_t tusb_DeviceAlarm;
#endif

const EventMaskType ev_Flicker = 0x01;
const EventMaskType ev_Update = 0x08;

/* Events for TinyUSB tasks
*/
#if DV_TUSB_HOST || DV_TUSB_DEVICE
ISRType	tusb_Isr1;
#ifdef hw_UsbInterruptId2
ISRType	tusb_Isr2;
#endif
#ifdef hw_UsbInterruptId2
ISRType	tusb_Isr3;
#endif
ISRType	tusb_Isr1;
ResourceType tusb_Mutex;
const EventMaskType ev_Tusb_Timeout	= 0x10;
const EventMaskType ev_Tusb_Notify	= 0x20;
#endif

/* callout_addtasks() - configure the tasks
*/
void callout_addtasks(dv_id_t mode)
{
	Init = dv_addtask("Init", &main_Init, 4, 1);
	Led = dv_addextendedtask("Led", &main_Led, 1, 1024);
	Bit0 = dv_addtask("Bit0", &main_Bit0, 2, 1);
	Bit1 = dv_addtask("Bit1", &main_Bit1, 2, 1);
	Bit2 = dv_addtask("Bit2", &main_Bit2, 2, 1);
	Bit3 = dv_addextendedtask("Bit3", &main_Bit3, 3, 1024);

#if DV_TUSB_HOST
	tusb_HostTask = dv_addextendedtask("tusb_HostTask", &main_tusb_HostTask, 3, 2048);
#endif
#if DV_TUSB_DEVICE
	tusb_DeviceTask = dv_addextendedtask("tusb_DeviceTask", &main_tusb_DeviceTask, 3, 2048);
#endif
}

/* callout_addisrs() - configure the isrs
*/
void callout_addisrs(dv_id_t mode)
{
	Uart = dv_addisr("Uart", &main_Uart, hw_UartInterruptId, 7);
	Timer = dv_addisr("Timer", &main_Timer, hw_TimerInterruptId, 8);

#if DV_TUSB_HOST || DV_TUSB_DEVICE
	tusb_Isr1 = dv_addisr("tusb_Isr1", &main_tusb_Isr1, hw_UsbInterruptId1, 8);
#ifdef hw_UsbInterruptId2
	tusb_Isr2 = dv_addisr("tusb_Isr2", &main_tusb_Isr2, hw_UsbInterruptId2, 8);
#endif
#ifdef hw_UsbInterruptId3
	tusb_Isr3 = dv_addisr("tusb_Isr3", &main_tusb_Isr3, hw_UsbInterruptId3, 8);
#endif
#endif
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
	tusb_Mutex = dv_addmutex("tusb_Mutex", 1);
	{
#if DV_TUSB_HOST
		dv_addmutexuser(tusb_Mutex, tusb_HostTask);
#endif
#if DV_TUSB_DEVICE
		dv_addmutexuser(tusb_Mutex, tusb_DeviceTask);
#endif
		dv_addmutexuser(tusb_Mutex, tusb_Isr1);
#ifdef hw_UsbInterruptId2
		dv_addmutexuser(tusb_Mutex, tusb_Isr2);
#endif
#ifdef hw_UsbInterruptId3
		dv_addmutexuser(tusb_Mutex, tusb_Isr3);
#endif
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

#if DV_TUSB_HOST
	tusb_HostAlarm = dv_addosekalarm_task("tusb_HostAlarm", OsekCounter, tusb_HostTask, ev_Tusb_Timeout);
#endif
#if DV_TUSB_DEVICE
	tusb_DeviceAlarm = dv_addosekalarm_task("tusb_DeviceAlarm", OsekCounter, tusb_DeviceTask, ev_Tusb_Timeout);
#endif
}

/* callout_autostart() - start the objects that need to be running after StartOS()
*/
void callout_autostart(dv_id_t mode)
{
	ActivateTask(Init);
	ActivateTask(Led);
	ActivateTask(Bit3);
#if DV_TUSB_HOST
	ActivateTask(tusb_HostTask);
#endif
#if DV_TUSB_DEVICE
	ActivateTask(tusb_DeviceTask);
#endif

	SetRelAlarm(BitDriver, 1000, 1000);
	SetRelAlarm(FlickerDriver, 1700, 89);

	/* Enable interrupts from the UART
	*/
	hw_EnableUartRxInterrupt();
	dv_enable_irq(hw_UartInterruptId);

	hw_InitialiseMillisecondTicker();
	dv_enable_irq(hw_TimerInterruptId);
}
