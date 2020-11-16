/*  dv-usb-hid.h
 *
 *  Copyright David Haworth
 *
 *  This file is part of davros.
 *
 *  davros is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  davros is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with davros.  If not, see <http://www.gnu.org/licenses/>.
*/
#ifndef DV_USB_HID_H
#define DV_USB_HID_H

#include "dv-devices.h"
#include DV_USB_CONFIG
#include DV_CFG_USBDRV
#include "dv-usb-util.h"
#include "dv-usb.h"

/* USB HID descriptor types
*/
#define DV_USB_HID_DESCRIPTOR_CLASS					0x21
#define DV_USB_HID_DESCRIPTOR_REPORT				0x22
#define DV_USB_HID_DESCRIPTOR_PHYSICAL				0x23

/* USB HID class descriptor  --- ToDo: better descriptions
 * 9 bytes
*/
#define DV_USB_HID_CLASSDESC_LEN					9
#define DV_USB_HID_CLASSDESC_IDX_bcdHID				2	/* 2 bytes	: version */
#define DV_USB_HID_CLASSDESC_IDX_bCountryCode		4	/* 1 byte	: country code */
#define DV_USB_HID_CLASSDESC_IDX_bNumDescriptors	5	/* 1 byte	: number of descriptors */
#define DV_USB_HID_CLASSDESC_IDX_bDescriptorType	6	/* 1 byte	: descriptor type */
#define DV_USB_HID_CLASSDESC_IDX_wDescriptorLength	7	/* 2 bytes	: descriptor length */

/* HID subclasses
*/
#define DV_USB_HID_SUBCLASS_NONE		0
#define DV_USB_HID_SUBCLASS_BOOT		1

/* HID protocols
*/
#define DV_USB_HID_PROTOCOL_NONE		0
#define DV_USB_HID_PROTOCOL_KEYBOARD	1
#define DV_USB_HID_PROTOCOL_MOUSE		2

#endif
