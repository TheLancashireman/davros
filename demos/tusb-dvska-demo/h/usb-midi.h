/* usb-midi.h - header file for a USB-midi devicb_midi_h
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
#ifndef usb_midi_h
#define usb_midi_h	1

#include <dv-config.h>
#include <davroska.h>
#include <tusb.h>

extern dv_id_t tusb_Mutex, tusb_DeviceTask;
extern dv_id_t tusb_Isr1, tusb_Isr2, tusb_Isr3;
extern dv_id_t tusb_DeviceAlarm;

extern void main_tusb_DeviceTask(void);
extern void main_tusb_Isr1(void);
extern void main_tusb_Isr2(void);
extern void main_tusb_Isr3(void);
extern dv_u64_t tusb_Expiry(dv_id_t, dv_param_t);

#endif
