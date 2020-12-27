/*  dv-usb-ep0-rxtx.c
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
#include "dv-devices.h"
#include DV_USB_CONFIG
#include <dv-usb.h>
#include <dv-usb-log.h>

/* dv_usb_ep0_ev_tx() - handle a tx event on EP0
*/
void dv_usb_ep0_ev_tx(dv_u16_t unused_ep)
{
	dv_printf("dv_usb_ep0_ev_tx() : ep = %d\n", unused_ep);
	if ( (dv_setup_packet.b[DV_USB_SETUPPKT_bmRequestType] & DV_USB_SETUPPKT_DIR) == 0 )
	{
		/* If there's a new address, program it into the hardware before anything else happens.
		*/
		if ( (dv_usb_dev.device_address & DV_USB_NEW_ADDRESS) != 0 )
		{
			dv_usb_dev.device_address &= DV_USB_ADDRESS;
			dv_printf("dv_usb_ep0_ev_tx() : ep = %d, new address %d\n", unused_ep, dv_usb_dev.device_address);
			dv_usbdrv_set_address(dv_usb_dev.device_address);
		}
		else
		{
			dv_printf("dv_usb_ep0_ev_tx() : ep = %d nothing to do\n", unused_ep);
		}
	}
	else
	{
		dv_printf("dv_usb_ep0_ev_tx() : ep = %d, calling dv_usb_stage_tx()\n", unused_ep);
		dv_usb_stage_tx(0);
	}
}

/* dv_usb_ep0_ev_rx() - handle an rx event on EP0
*/
void dv_usb_ep0_ev_rx(dv_u16_t unused_ep)
{
	dv_printf("dv_usb_ep0_ev_rx() : ep = %d\n", unused_ep);
	if ( (dv_setup_packet.b[DV_USB_SETUPPKT_bmRequestType] & DV_USB_SETUPPKT_DIR) == 0 )
	{
		if ( dv_ep0_state.count == 0 )
		{
			/* Nothing to do */
		}
		else
		{
			if ( dv_ep0_state.data == DV_NULL )
			{
				dv_printf("dv_usb_ep0_ev_rx(): ERROR !!! dv_ep0_state.data is NULL\n");
				dv_usbdrv_stall_tx(0);
				return;
			}
			dv_u16_t c = dv_usbdrv_read_ep(0, (dv_u8_t *)dv_ep0_state.data, DV_USB_SETUPPKT_LEN);
			dv_ep0_state.data += c;
			dv_ep0_state.count -= c;

			if ( dv_ep0_state.count == 0 )
			{
				switch ( dv_setup_packet.b[DV_USB_SETUPPKT_bmRequestType] & DV_USB_SETUPPKT_TYPE )
				{
				case DV_USB_SETUPPKT_Class:
					/* ToDo: HID/AUDIO stuff */
				case DV_USB_SETUPPKT_Standard:
				default:
					dv_usbdrv_stall_tx(0);
					break;
				}
			}
		}
	}
	else
	{
		dv_usbdrv_read_ep(0, &dv_ep0_buffer[0], DV_CFG_EP0_SIZE);
	}
}
