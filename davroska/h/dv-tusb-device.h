/*	dv-tusb-device.h - standard tinyusb device infrastructure for davroska
 *
 *	Copyright David Haworth
 *
 *	This file is part of davros.
 *
 *	davros is free software: you can redistribute it and/or modify
 *	it under the terms of the GNU General Public License as published by
 *	the Free Software Foundation, either version 3 of the License, or
 *	(at your option) any later version.
 *
 *	davros is distributed in the hope that it will be useful,
 *	but WITHOUT ANY WARRANTY; without even the implied warranty of
 *	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *	GNU General Public License for more details.
 *
 *	You should have received a copy of the GNU General Public License
 *	along with davros.  If not, see <http://www.gnu.org/licenses/>.
*/
#ifndef dv_tusb_device_h
#define dv_tusb_device_h	1
#include <dv-config.h>
#include <davroska.h>
#include <tusb.h>

/* Default configuration of stack sizes, priorities, etc.
*/
#ifndef DV_TUSB_TASKPRIO
#define DV_TUSB_TASKPRIO	3
#endif
#ifndef DV_TUSB_ISRPRIO
#define DV_TUSB_ISRPRIO		8
#endif
#ifndef DV_TUSB_TASKSTACK
#define DV_TUSB_TASKSTACK	1024
#endif

/* Object identifiers for tasks, ISRs etc.
 *
 * A mutex, an extended task and an alarm are always needed.
 * The number of ISRs needed depends on the hardware.
 * Unused identifiers and functions are deeclared anyway - this is harmless
*/
extern dv_id_t tusb_Mutex;
extern dv_id_t tusb_DeviceTask;
extern dv_id_t tusb_DeviceAlarm;

extern dv_id_t tusb_Isr1;
extern dv_id_t tusb_Isr2;
extern dv_id_t tusb_Isr3;

extern void main_tusb_DeviceTask(void);
extern void main_tusb_Isr1(void);
extern void main_tusb_Isr2(void);
extern void main_tusb_Isr3(void);

extern dv_u64_t tusb_Expiry(dv_id_t a, dv_param_t p);

/* Functions to create all the objects.
 * These functions are called from callout_createXXX
*/
static inline void callout_tusbd_addtasks(void)
{
	tusb_DeviceTask = dv_addextendedtask("tusb_DeviceTask", &main_tusb_DeviceTask, DV_TUSB_TASKPRIO, DV_TUSB_TASKSTACK);
}

static inline void callout_tusbd_addisrs(void)
{
#ifdef hw_UsbInterruptId1
    tusb_Isr1 = dv_addisr("tusb_Isr1", &main_tusb_Isr1, hw_UsbInterruptId1, DV_TUSB_ISRPRIO);
#endif
#ifdef hw_UsbInterruptId2
    tusb_Isr2 = dv_addisr("tusb_Isr2", &main_tusb_Isr2, hw_UsbInterruptId2, DV_TUSB_ISRPRIO);
#endif
#ifdef hw_UsbInterruptId3
    tusb_Isr3 = dv_addisr("tusb_Isr3", &main_tusb_Isr3, hw_UsbInterruptId3, DV_TUSB_ISRPRIO);
#endif
}

static inline void callout_tusbd_addmutexes(void)
{
	tusb_Mutex = dv_addmutex("tusb_Mutex", 1);
	dv_addmutexuser(tusb_Mutex, tusb_DeviceTask);
#ifdef hw_UsbInterruptId1
	dv_addmutexuser(tusb_Mutex, tusb_Isr1);
#endif
#ifdef hw_UsbInterruptId2
	dv_addmutexuser(tusb_Mutex, tusb_Isr2);
#endif
#ifdef hw_UsbInterruptId3
	dv_addmutexuser(tusb_Mutex, tusb_Isr3);
#endif
}

static inline void callout_tusbd_addalarms(void)
{
	tusb_DeviceAlarm = dv_addalarm("tusb_DeviceAlarm", &tusb_Expiry, (dv_param_t)tusb_DeviceTask);
}

/* callout_tusbd_activate() - activate the tusb task(s)
*/
static inline void callout_tusbd_activate(void)
{
	dv_activatetask(tusb_DeviceTask);
}

/* dv_tusb_enableirqs() - enable all the IRQs for USB
*/
#ifdef hw_UsbInterruptId1
#define DV_ENABLEUSBIRQ1()		dv_enable_irq(hw_UsbInterruptId1)
#else
#define DV_ENABLEUSBIRQ1()		do { } while (0)
#endif

#ifdef hw_UsbInterruptId2
#define DV_ENABLEUSBIRQ2()		dv_enable_irq(hw_UsbInterruptId2)
#else
#define DV_ENABLEUSBIRQ2()		do { } while (0)
#endif

#ifdef hw_UsbInterruptId3
#define DV_ENABLEUSBIRQ3()		dv_enable_irq(hw_UsbInterruptId3)
#else
#define DV_ENABLEUSBIRQ3()		do { } while (0)
#endif

/* Defined as a macro to avoid having to include hardware-specific header files here
*/
#define dv_tusbd_enableirqs()	\
do {						\
	DV_ENABLEUSBIRQ1();		\
	DV_ENABLEUSBIRQ2();		\
	DV_ENABLEUSBIRQ3();		\
} while (0)

#endif
