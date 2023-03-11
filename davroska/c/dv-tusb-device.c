/*	dv-tusb-device.c - standard tinyusb device infrastructure for davroska
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
#define DV_ASM	0
#include <dv-config.h>
#include <davroska.h>
#include <tusb.h>
#include <dv-tusb-device.h>

/* Object identifiers for tasks, ISRs etc.
 *
 * A mutex, an extended task and an alarm are always needed.
 * The number of ISRs needed depends on the hardware. Configure using hw_UsbInterruptIdn
*/
dv_id_t tusb_Mutex;
dv_id_t tusb_DeviceTask;
dv_id_t tusb_DeviceAlarm;

#ifdef hw_UsbInterruptId1
dv_id_t tusb_Isr1;
#endif
#ifdef hw_UsbInterruptId2
dv_id_t tusb_Isr2;
#endif
#ifdef hw_UsbInterruptId3
dv_id_t tusb_Isr3;
#endif

/* main_tusb_DeviceTask() - the body of the tinyusb device task.
 *
 * The task is an extended task.
 * tud_task() only returns if there's something wrong. It can happen normally at startup, so
 * there's a loop that calls osal_delay() with a delay of 5 ms.
*/
void main_tusb_DeviceTask(void)
{
	tud_init(BOARD_TUD_RHPORT);
	for (;;)
	{
		tud_task();
		osal_delay(0, 5);		/* Might happen during startup. Avoids busy-wait. */
	}
}

/* main_tusb_Isrn() - bodies of up to three ISRs
 *
 * Each ISR calls a configurable TUSB handler that depends on the hardware.
*/
#ifdef hw_UsbInterruptId1
void main_tusb_Isr1(void)
{
	hw_UsbIsr1();
}
#endif

#ifdef hw_UsbInterruptId2
void main_tusb_Isr2(void)
{
	hw_UsbIsr2();
}
#endif

#ifdef hw_UsbInterruptId3
void main_tusb_Isr3(void)
{
	hw_UsbIsr3();
}
#endif

/* tusb_Expiry() - the expiry function used with tusb_DeviceAlarm
 *
 * The alarm expiry causes the event tusb_EvTimeout to be sent to the task specified in the parameter.
*/
dv_u64_t tusb_Expiry(dv_id_t a, dv_param_t p)
{
	(void)dv_setevent((dv_id_t)p, tusb_EvTimeout);
	return 0;
}
