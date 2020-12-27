/*  dv-usb.h
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
#ifndef DV_USB_H
#define DV_USB_H

#include "dv-devices.h"
#include DV_USB_CONFIG
#include DV_CFG_USBDRV
#include "dv-usb-util.h"

/* dv_usb_epstate_t - endpoint data buffering control and status
*/
typedef struct dv_usb_epstate_s
{
	const dv_u8_t *data;				/* Current data pointer */
	dv_u16_t count;						/* No of bytes left to transmit */
	dv_u16_t status;					/* Current status: HALT etc. */
	dv_u16_t max_tx;					/* Biggest tx packet */
	dv_u16_t max_rx;					/* Biggest rx packet */
	void (*setup_func)(dv_u16_t ep);	/* Function to handle setup requests */
	void (*rx_func)(dv_u16_t ep);		/* Function to handle received data (OUT) */
	void (*tx_func)(dv_u16_t ep);		/* Function to handle transmit requests (IN) */
} dv_usb_epstate_t;

/* Bits in dv_usb_epstate_t.status
*/
#define DV_USB_EP_TX_HALT	0x0001
#define DV_USB_EP_RX_HALT	0x0002

/* dv_usb_device_t - usb device status
*/
typedef struct dv_usb_device_s
{
	dv_u16_t device_status;
	dv_u8_t device_address;
	dv_u8_t configuration;
	dv_u8_t n_interfaces;
	dv_u8_t alt_setting[DV_CFG_USB_N_INTERFACES];
	dv_usb_epstate_t epstate[DV_CFG_USB_N_ENDPOINTS];
} dv_usb_device_t;

/* USB address bits. The NEW_ADDRESS flag is specific to davros
*/
#define DV_USB_ADDRESS			0x7f	/* Assigned device address 1..127. 0 --> not yet assigned */
#define DV_USB_NEW_ADDRESS		0x80	/* The address is new and needs to be written into the device */

/* USB device status bits defined by USB standard
*/
#define DV_USB_DEVSTATUS_SELF_POWERED	0x01
#define DV_USB_DEVSTATUS_REMOTE_WAKEUP	0x02

/* USB endpoint status bits defined by USB standard
*/
#define DV_USB_EPSTATUS_STALL			0x01

/* Setup packet
 * ============
 *
 * The meaning of the wValue, wIndex and wLength parameters depends on the request type and ID
*/
#define DV_USB_SETUPPKT_LEN				8
#define DV_USB_SETUPPKT_bmRequestType	0	/* 1 byte	: recipient, type & dir */
#define DV_USB_SETUPPKT_bRequest		1	/* 1 byte	: request ID */
#define	DV_USB_SETUPPKT_wValue			2	/* 2 bytes	: value */
#define	DV_USB_SETUPPKT_wIndex			4	/* 2 bytes	: index */
#define	DV_USB_SETUPPKT_wLength			6	/* 2 bytes	: length */

/* bmRequestType bit fields
*/
#define DV_USB_SETUPPKT_DIR				0x80
#define DV_USB_SETUPPKT_TYPE			0x60
#define DV_USB_SETUPPKT_RECIPIENT		0x1f

/* Values in DIR field of bmRequestType
*/
#define DV_USB_SETUPPKT_HtoD			0x00	/* Direction : host to device */
#define DV_USB_SETUPPKT_DtoH			0x80	/* Direction : device to host */

/* Values in TYPE field of bmRequestType. Other values are "reserved"
*/
#define DV_USB_SETUPPKT_Standard		0x00	/* Type : standard */
#define DV_USB_SETUPPKT_Class			0x20 	/* Type : class */
#define DV_USB_SETUPPKT_Vendor			0x40 	/* Type : vendor */

/* Values of RECIPIENT field of bmRequestType. Other values are "reserved"
*/
#define DV_USB_SETUPPKT_Device			0x00	/* Recipient : device */
#define DV_USB_SETUPPKT_Interface		0x01	/* Recipient : interface */
#define DV_USB_SETUPPKT_Endpoint		0x02	/* Recipient : endpoint */
#define DV_USB_SETUPPKT_Other			0x03	/* Recipient : other */

/* bRequest - standard request codes
*/
#define DV_USB_REQ_GetStatus			0
#define DV_USB_REQ_ClearFeature			1
#define DV_USB_REQ_SetFeature			3
#define DV_USB_REQ_SetAddress			5
#define DV_USB_REQ_GetDescriptor		6
#define DV_USB_REQ_SetDescriptor		7
#define DV_USB_REQ_GetConfiguration		8
#define DV_USB_REQ_SetConfiguration		9
#define DV_USB_REQ_GetInterface			10
#define DV_USB_REQ_SetInterface			11
#define DV_USB_REQ_SyncFrame			12

/* Feature selectors
*/
#define DV_USB_FEATURE_ENDPOINT_STALL	0
#define DV_USB_FEATURE_REMOTE_WAKEUP	1

typedef struct dv_usb_setup_packet_s
{
	dv_u8_t b[DV_USB_SETUPPKT_LEN];
} dv_usb_setup_packet_t;

extern dv_usb_device_t dv_usb_dev;
extern dv_u8_t dv_ep0_buffer[DV_CFG_EP0_SIZE];
extern dv_usb_setup_packet_t dv_setup_packet;

#define dv_ep0_state	dv_usb_dev.epstate[0]

/* Descriptors
 * ===========
 *
 * All descriptors start with the following two bytes:
 *	- Index 0 = length
 *	- Index 1 = type
*/
#define DV_USB_DESC_IDX_bLength					0	/* Total length including bLength and bDescriptorType */
#define DV_USB_DESC_IDX_bDescriptorType			1

/* Descriptor types
 *
 * In the GetDescriptor request, the descriptor type can be found in the high byte of the wValue field
*/
#define DV_USB_DESCRIPTOR_DEVICE				1
#define DV_USB_DESCRIPTOR_CONFIGURATION			2
#define DV_USB_DESCRIPTOR_STRING				3
#define DV_USB_DESCRIPTOR_INTERFACE				4
#define DV_USB_DESCRIPTOR_ENDPOINT				5
#define DV_USB_DESCRIPTOR_DEVICE_QUALIFIER		6
#define DV_USB_DESCRIPTOR_OTHER_SPEED_CONFIG	7
#define DV_USB_DESCRIPTOR_INTERFACE_POWER		8
#define DV_USB_DESCRIPTOR_ON_THE_GO				9

/* USB device descriptor
 * 18 bytes
*/
#define DV_USB_DEVDESC_LEN						18
#define DV_USB_DEVDESC_IDX_bcdUSB				2	/* 2 bytes	: USB spec version */
#define DV_USB_DEVDESC_IDX_bDeviceClass 		4	/* 1 byte	: Class code (assigned by USB org) */
#define DV_USB_DEVDESC_IDX_bDeviceSubClass		5	/* 1 byte	: Subclass code (assigned by USB org) */
#define DV_USB_DEVDESC_IDX_bDeviceProtocol		6	/* 1 byte	: Protocol code (assigned by USB org) */
#define DV_USB_DEVDESC_IDX_bMaxPacketSize		7	/* 1 byte	: Maximum packet size for EP0. (8, 16, 32, 64) */
#define DV_USB_DEVDESC_IDX_idVendor				8	/* 2 bytes	: Vendor ID (assigned by USB org) */
#define DV_USB_DEVDESC_IDX_idProduct			10	/* 2 bytes	: Product ID (assigned by manufacturer) */
#define DV_USB_DEVDESC_IDX_bcdDevice			12	/* 2 bytes	: Device release number */
#define DV_USB_DEVDESC_IDX_iManufacturer		14	/* 1 byte	: Index of manufacturer string descriptor */
#define DV_USB_DEVDESC_IDX_iProduct				15	/* 1 byte	: Index of product string descriptor */
#define DV_USB_DEVDESC_IDX_iSerialNumber		16	/* 1 byte	: Index of serial number string descriptor */
#define DV_USB_DEVDESC_IDX_bNumConfigurations	17	/* 1 byte	: Number of possible configurations */

/* USB string descriptor
 * Variable. Strings are wide characters
*/
#define DV_USB_STRDESC_IDX_string				2	/* n*2 bytes : wide characters */

/* USB configuration descriptor
 * 9 bytes
*/
#define DV_USB_CFGDESC_LEN						9
#define DV_USB_CFGDESC_IDX_wTotalLength			2	/* 2 bytes	: total length of all config descriptors */
#define DV_USB_CFGDESC_IDX_bNumInterfaces		4	/* 1 byte 	: number of interfaces */
#define DV_USB_CFGDESC_IDX_bConfigurationValue	5	/* 1 byte	: value to select this configuration */
#define DV_USB_CFGDESC_IDX_iConfiguration		6	/* 1 byte	: index of string descriptor describing this cfg */
#define DV_USB_CFGDESC_IDX_bmAttributes			7	/* 1 byte	: attributes (see below) */
#define DV_USB_CFGDESC_IDX_bMaxPower			8	/* 1 byte	: max power in units of 2mA */

/* Config descriptor bmAttributes
 * Remaining bits: reserved, set to zero
*/
#define DV_USB_CFGDESC_ATTR_BUS_POWERED			0x80	/* USB 1.0; for 1.1 and above, reserved, set to 1 */
#define DV_USB_CFGDESC_ATTR_SELF_POWERED		0x40
#define DV_USB_CFGDESC_ATTR_REMOTE_WAKEUP		0x20

/* Convert mA to bMaxPower
*/
#define DV_USB_mA_to_bMaxPower(x)				(((x)+1)/2)		/* Round up */

/* USB interface descriptor
 * 9 bytes
*/
#define DV_USB_IFDESC_LEN						9
#define DV_USB_IFDESC_IDX_bInterfaceNumber		2
#define DV_USB_IFDESC_IDX_bAlternateSetting		3
#define DV_USB_IFDESC_IDX_bNumEndpoints			4
#define DV_USB_IFDESC_IDX_bInterfaceClass		5
#define DV_USB_IFDESC_IDX_bInterfaceSubClass	6
#define DV_USB_IFDESC_IDX_bInterfaceProtocol	7
#define DV_USB_IFDESC_IDX_iInterface			8

/* USB endpoint descriptor
 * 7 bytes
*/
#define DV_USB_EPDESC_LEN						7
#define DV_USB_EPDESC_IDX_bEndpointAddress		2	/* 1 byte	: EP number + direction */
#define DV_USB_EPDESC_IDX_bmAttributes			3	/* 1 byte	: attributes (see below) */
#define DV_USB_EPDESC_IDX_wMaxPacketSize		4	/* 2 bytes	: maximum packet size */
#define DV_USB_EPDESC_IDX_bInterval				6	/* 1 byte	: polling interval in milliseconds */

/* IN (tx) and OUT (rx) endpoint addresses are differentiated by the DIR bit (0x80)
*/
#define DV_USB_EP_IN(x)		(0x80 | (x))
#define DV_USB_EP_OUT(x)	(0x00 | (x))

/* EP descriptor bmAttributes
*/
#define DV_USB_EPDESC_ATTR_TYPE					0x03
#define DV_USB_EPDESC_ATTR_CONTROL				0x00
#define DV_USB_EPDESC_ATTR_ISOCHRONOUS			0x01
#define DV_USB_EPDESC_ATTR_BULK					0x02
#define DV_USB_EPDESC_ATTR_INTERRUPT			0x03
#define DV_USB_EPDESC_ATTR_SYNC					0x0c
#define DV_USB_EPDESC_ATTR_NOSYNC				0x00
#define DV_USB_EPDESC_ATTR_ASYNCHRONOUS			0x04
#define DV_USB_EPDESC_ATTR_ADAPTIVE				0x08
#define DV_USB_EPDESC_ATTR_SYNCHRONOUS			0x0c
#define DV_USB_EPDESC_ATTR_USAGE				0x30
#define DV_USB_EPDESC_ATTR_DATA					0x00
#define DV_USB_EPDESC_ATTR_FEEDBACK				0x10
#define DV_USB_EPDESC_ATTR_IMPLICIT_FEEDBACK	0x20
#define DV_USB_EPDESC_ATTR_RESERVED				0x30


/* USB device classes defined by the USB standard
*/
#define DV_USB_CLASS_0							0x00	/* "Reserved" */
#define DV_USB_CLASS_AUDIO						0x01
#define DV_USB_CLASS_COMMUNICATIONS				0x02
#define DV_USB_CLASS_HUMAN_INTERFACE			0x03
#define DV_USB_CLASS_MONITOR					0x04
#define DV_USB_CLASS_PHYSICAL_INTERFACE			0x05
#define DV_USB_CLASS_POWER						0x06
#define DV_USB_CLASS_PRINTER					0x07
#define DV_USB_CLASS_STORAGE					0x08
#define DV_USB_CLASS_HUB						0x09
#define DV_USB_CLASS_VENDOR_SPECIFIC			0xff

extern void dv_usb_init(void);
extern void dv_usb_nullepfunc(dv_u16_t);
extern void dv_usb_ep0_ev_setup(dv_u16_t);
extern void dv_usb_ep0_ev_rx(dv_u16_t);
extern void dv_usb_ep0_ev_tx(dv_u16_t);

/* Assorted inline functions
 * =========================
*/

/* dv_usb_stage_tx() - stage the next data packet for transmission
 *
 * Writes the next packet of data to the endpoint's tx queue and updates the pointer and count
*/
static inline void dv_usb_stage_tx(dv_i32_t ep)
{
	dv_usb_epstate_t *eps = &dv_usb_dev.epstate[ep];
	dv_u16_t n_tx = (eps->count > eps->max_tx) ? eps->max_tx : eps->count;
	dv_printf("dv_usb_stage_tx(): ep = %d, count = %d max_tx = %d\n", ep, eps->count, eps->max_tx);

	dv_usbdrv_write_ep(ep, eps->data, n_tx);
	eps->data += n_tx;
	eps->count -= n_tx;
}

#endif
