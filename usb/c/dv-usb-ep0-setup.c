/*  dv-usb-ep0-setup.c
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

dv_i32_t dv_usb_setup_request_device_standard(dv_usb_setup_packet_t *pkt);
dv_i32_t dv_usb_setup_request_interface_standard(dv_usb_setup_packet_t *pkt);
dv_i32_t dv_usb_setup_request_endpoint_standard(dv_usb_setup_packet_t *pkt);

dv_i32_t dv_usb_setup_request_device_class(dv_usb_setup_packet_t *pkt);
dv_i32_t dv_usb_setup_request_interface_class(dv_usb_setup_packet_t *pkt);
dv_i32_t dv_usb_setup_request_endpoint_class(dv_usb_setup_packet_t *pkt);

dv_i32_t dv_usb_GetStatus(dv_usb_setup_packet_t *pkt);

/* dv_usb_ep0_ev_setup() - handle a setup event on EP0
*/
void dv_usb_ep0_ev_setup(void)
{
	dv_usb_setup_packet_t setup_packet;
	dv_i32_t ok = 0;

	/* Read the setup packet from the endpoint
	*/
	dv_usbdrv_read_ep(0, &setup_packet.b[0], DV_USB_SETUPPKT_LEN);

	dv_usbdrv_set_ep_direction(setup_packet.b[DV_USB_SETUPPKT_bmRequestType] & DV_USB_SETUPPKT_DIR);

	/* Get packet type and recipient and call corresponding subfunction
	*/
	switch ( setup_packet.b[DV_USB_SETUPPKT_bmRequestType] & (DV_USB_SETUPPKT_RECIPIENT | DV_USB_SETUPPKT_TYPE) )
	{
	case DV_USB_SETUPPKT_Device | DV_USB_SETUPPKT_Standard:
		ok = dv_usb_setup_request_device_standard(&setup_packet);
		break;

	case DV_USB_SETUPPKT_Interface | DV_USB_SETUPPKT_Standard:
		ok = dv_usb_setup_request_interface_standard(&setup_packet);
		break;

	case DV_USB_SETUPPKT_Endpoint | DV_USB_SETUPPKT_Standard:
		ok = dv_usb_setup_request_endpoint_standard(&setup_packet);
		break;

	case DV_USB_SETUPPKT_Device | DV_USB_SETUPPKT_Class:
		ok = dv_usb_setup_request_device_class(&setup_packet);
		break;

	case DV_USB_SETUPPKT_Interface | DV_USB_SETUPPKT_Class:
		ok = dv_usb_setup_request_interface_class(&setup_packet);
		break;

	case DV_USB_SETUPPKT_Endpoint | DV_USB_SETUPPKT_Class:
		ok = dv_usb_setup_request_endpoint_class(&setup_packet);
		break;

/* Ignored requests:
 *	- type = VENDOR
 *	- recipient = OTHER
 *	- all "reserved" types and recipients
*/
	default:
		break;
	}

	if ( ok )
		dv_usb_stage_tx(0);
}

/* dv_usb_setup_request_device_standard() - handle a standard device setup request
*/
dv_i32_t dv_usb_setup_request_device_standard(dv_usb_setup_packet_t *pkt)
{
	dv_i32_t ok = 0;
	dv_u16_t rqlen = dv_usb_load_16(&pkt->b[DV_USB_SETUPPKT_wLength]); 	/* Requested data length */
	dv_u16_t rqval = dv_usb_load_16(&pkt->b[DV_USB_SETUPPKT_wValue]);	/* Request value */

	switch ( pkt->b[DV_USB_SETUPPKT_bRequest] )
	{
	case DV_USB_REQ_GetStatus:
		if ( rqlen == 2 )
		{
			dv_usb_store_16(&dv_ep0_buffer[0], dv_usb_dev.device_status);
			dv_ep0_state.data = &dv_ep0_buffer[0];
			dv_ep0_state.count = 2;
			ok = 1;
		}
		break;

	case DV_USB_REQ_ClearFeature:
		if ( rqlen == 0 && rqval == DV_USB_FEATURE_REMOTE_WAKEUP )
		{
			dv_usbdrv_disable_wakeup();
			dv_usb_dev.device_status &= ~DV_USB_DEVSTATUS_REMOTE_WAKEUP;
			ok = 1;
#if DV_CFG_USB_FEATURE_EVENT
			USB_Feature_Event();
#endif
		}
		break;

	case DV_USB_REQ_SetFeature:
		if ( rqlen == 0 && rqval == DV_USB_FEATURE_REMOTE_WAKEUP )
		{
			dv_usbdrv_enable_wakeup();
			dv_usb_dev.device_status |= DV_USB_DEVSTATUS_REMOTE_WAKEUP;
			ok = 1;
#if DV_CFG_USB_FEATURE_EVENT
			USB_Feature_Event();
#endif
		}
		break;

	case DV_USB_REQ_SetAddress:
		if ( rqlen == 0 )
		{
			dv_usb_dev.device_address = DV_USB_NEW_ADDRESS | rqval;
			ok = 1;
		}
		break;

	case DV_USB_REQ_GetDescriptor:
		if ( USB_GetDescriptor_Device() ) 	/* TODO */
		{
			ok = 1;
		}
		break;

	case DV_USB_REQ_SetDescriptor:
		dv_usbdrv_stall_rx(0);
		dv_ep0_state.data = DV_NULL;
		dv_ep0_state.count = 0;
		return 0;		/* Quit without stalling the tx */
		break;

	case DV_USB_REQ_GetConfiguration:
		if ( rqlen == 1 )
		{
			dv_ep0_state.data = &dv_usb_dev.configuration;
			dv_ep0_state.count = 1;
			ok = 1;
		}
		break;

	case DV_USB_REQ_SetConfiguration:
		ok = USB_SetConfiguration_Device();	/* TODO */
#if DV_CFG_USB_CONFIGURE_EVENT
		USB_Configure_Event();
#endif
		break;

	default:
		break;
	}

	if ( !ok )
	{
		dv_usbdrv_stall_tx(0);
		dv_ep0_state.data = DV_NULL;
		dv_ep0_state.count = 0;
	}

	return ok;
}

/* dv_usb_setup_request_interface_standard() - handle a standard interface setup request
*/
dv_i32_t dv_usb_setup_request_interface_standard(dv_usb_setup_packet_t *pkt)
{
	dv_i32_t ok = 0;
	dv_u16_t rqlen = dv_usb_load_16(&pkt->b[DV_USB_SETUPPKT_wLength]); 	/* Requested data length */
	dv_u16_t rqidx = dv_usb_load_16(&pkt->b[DV_USB_SETUPPKT_wIndex]);	/* Request index */
	dv_u16_t rqval = dv_usb_load_16(&pkt->b[DV_USB_SETUPPKT_wValue]);	/* Request value */

	switch ( pkt->b[DV_USB_SETUPPKT_bRequest] )
	{
	case DV_USB_REQ_GetStatus:
		if ( ( rqlen == 2 ) && ( dv_usb_dev.configuration != 0 ) && ( rqidx < dv_usb_dev.n_interfaces ) )
		{
			dv_ep0_buffer[0] = dv_ep0_buffer[1] = 0;	/* ToDo: interface status always zero? */
			dv_ep0_state.data = &dv_ep0_buffer[0];
			dv_ep0_state.count = 2;
			ok = 1;
		}
		break;

	case DV_USB_REQ_GetDescriptor:
		ok = USB_GetDescriptor_Interface(); 	/* TODO */
		break;

	case DV_USB_REQ_SetDescriptor:
		dv_usbdrv_stall_rx(0);
		dv_ep0_state.data = DV_NULL;
		dv_ep0_state.count = 0;
		return 0;		/* Quit without stalling the tx */
		break;

	case DV_USB_REQ_GetInterface:
		if ( ( rqlen == 1 ) && ( dv_usb_dev.configuration != 0 ) && ( rqidx < dv_usb_dev.n_interfaces ) )
		{
			dv_ep0_state.data = &dv_usb_dev.alt_setting[rqidx];
			dv_ep0_state.count = 1;
			ok = 1;
		}
		break;

	case DV_USB_REQ_SetInterface:
		ok = USB_SetInterface();	/* TODO */
#if DV_CFG_USB_INTERFACE_EVENT
		if ( ok )
			USB_Interface_Event();
#endif
		break;

	default:
		break;
	}

	if ( !ok )
	{
		dv_usbdrv_stall_tx(0);
		dv_ep0_state.data = DV_NULL;
		dv_ep0_state.count = 0;
	}

	return ok;
}

/* Still to implement ...
*/
dv_i32_t dv_usb_setup_request_endpoint_standard(dv_usb_setup_packet_t *pkt)
{
	return 0;
}

dv_i32_t dv_usb_setup_request_device_class(dv_usb_setup_packet_t *pkt)
{
	return 0;
}

dv_i32_t dv_usb_setup_request_interface_class(dv_usb_setup_packet_t *pkt)
{
	return 0;
}

dv_i32_t dv_usb_setup_request_endpoint_class(dv_usb_setup_packet_t *pkt)
{
	return 0;
}
