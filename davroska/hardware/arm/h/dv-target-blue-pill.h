/* dv-target-blue-pill.h - header file for davroska on a Blue Pill
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
#ifndef dv_target_blue_pill_h
#define dv_target_blue_pill_h	1

#define DV_ARMv7_M		1

#include <arm/h/dv-types-cortexm.h>
#include <dv-cortex-m.h>
#include <dv-stm32-interrupts.h>
#include <dv-stdio.h>

#define DV_INCLUDE_INTERRUPTCONTROLLER	<dv-nvic.h>
#define DV_MAX_INTLEVEL		15
#define DV_LOCKALL_LEVEL	(DV_MAX_INTLEVEL)
#define DV_NVECTOR			68

/* USB interrupts
*/
#define hw_UsbInterruptId1		dv_irq_usb_lp_can_rx0
#define hw_UsbInterruptId2		dv_irq_usb_hp_can_tx
#define hw_UsbInterruptId3		dv_irq_usbwakeup

/* ISR bodies for USB interrupts, defined as macros to avoid having to include tinyusb header here.
*/
#define hw_UsbIsr1()	tud_int_handler(0)
#define hw_UsbIsr2()	tud_int_handler(0)
#define hw_UsbIsr3()	tud_int_handler(0)


#endif
