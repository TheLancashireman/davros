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
#include <dv-usb-log.h>

dv_i32_t dv_usb_setup_request_device_standard(dv_usb_setup_packet_t *pkt);
dv_i32_t dv_usb_setup_request_interface_standard(dv_usb_setup_packet_t *pkt);
dv_i32_t dv_usb_setup_request_endpoint_standard(dv_usb_setup_packet_t *pkt);

dv_i32_t dv_usb_setup_request_device_class(dv_usb_setup_packet_t *pkt);
dv_i32_t dv_usb_setup_request_interface_class(dv_usb_setup_packet_t *pkt);
dv_i32_t dv_usb_setup_request_endpoint_class(dv_usb_setup_packet_t *pkt);

dv_i32_t dv_usb_GetStatus(dv_usb_setup_packet_t *pkt);

/* dv_usb_ep0_ev_setup() - handle a setup event on EP0
*/
void dv_usb_ep0_ev_setup(dv_u16_t unused_ep)
{
	dv_usb_setup_packet_t setup_packet;
	dv_i32_t ok = 0;

	/* Read the setup packet from the endpoint
	*/
	dv_usbdrv_read_ep(0, &setup_packet.b[0], DV_USB_SETUPPKT_LEN);

	dv_printf("dv_usb_ep0_ev_setup() : ep = %d, setup_packet = %02x, %02x, %04x %04x %04x\n", unused_ep,
				setup_packet.b[DV_USB_SETUPPKT_bmRequestType],
				setup_packet.b[DV_USB_SETUPPKT_bRequest],
				dv_usb_load_16(&setup_packet.b[DV_USB_SETUPPKT_wValue]),
				dv_usb_load_16(&setup_packet.b[DV_USB_SETUPPKT_wIndex]),
				dv_usb_load_16(&setup_packet.b[DV_USB_SETUPPKT_wLength]) );

	dv_usb_log(	setup_packet.b[DV_USB_SETUPPKT_bmRequestType],
				setup_packet.b[DV_USB_SETUPPKT_bRequest],
				dv_usb_load_16(&setup_packet.b[DV_USB_SETUPPKT_wValue]),
				dv_usb_load_16(&setup_packet.b[DV_USB_SETUPPKT_wIndex]),
				dv_usb_load_16(&setup_packet.b[DV_USB_SETUPPKT_wLength]) );

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

/* dv_usb_setup_request_device_get_descriptor() - send the requested descriptor
 *
 * Descriptor can be one of DV_USB_DESCRIPTOR_DEVICE, DV_USB_DESCRIPTOR_CONFIGURATION, DV_USB_DESCRIPTOR_STRING.
*/
static dv_i32_t dv_usb_setup_request_device_get_descriptor(dv_u16_t rqlen, dv_u16_t rqval)
{
	dv_i32_t ok = 0;
	dv_u16_t dtype = rqval >> 8;
	dv_u16_t dnum;
	dv_u16_t i;
	dv_u16_t idx;
	dv_u16_t len;

	switch ( dtype )
	{
	case DV_USB_DESCRIPTOR_DEVICE:
		dv_ep0_state.data = &DV_USB_DEVICE_DESCRIPTOR[0];
		dv_ep0_state.count = (rqlen > DV_USB_DEVDESC_LEN) ? DV_USB_DEVDESC_LEN : rqlen;
		ok = 1;
		break;

	case DV_USB_DESCRIPTOR_CONFIGURATION:
		/* There might be several configuration descriptors. They're stored in a single
		 * array. Q: Why not have an array of references to them, rather than walking the
		 * list?
		*/
		if ( DV_USB_CONFIGURATION_DESCRIPTOR[0] == 0 )
			return 0;		/* No descriptors! Can this ever happen? */
		dnum = rqval & 0xff;
		idx = 0;					/* Index of the bLength of the descriptor */
		for ( i = 0; i < dnum; i++ )
		{
			idx += dv_usb_load_16(&DV_USB_CONFIGURATION_DESCRIPTOR[idx+DV_USB_CFGDESC_IDX_wTotalLength]);

			if ( DV_USB_CONFIGURATION_DESCRIPTOR[idx] == 0 )
				return 0;		/* Descriptor doesn't exist */
		}
		dv_ep0_state.data = &DV_USB_CONFIGURATION_DESCRIPTOR[idx];
		len = dv_usb_load_16(&DV_USB_CONFIGURATION_DESCRIPTOR[idx+DV_USB_CFGDESC_IDX_wTotalLength]);
		dv_ep0_state.count = (rqlen > len) ? len : rqlen;
		ok = 1;
		break;

	case DV_USB_DESCRIPTOR_STRING:
		/* The various other descriptors (device etc.) contain the byte offsets of the
		 * string descriptors, so error checking is difficult.
		 * Heuristics:
		 *	- index is in range AND
		 *	- bLength of descriptor is non-zero AND
		 *	- bDescriptorType of descriptor is correct
		*/
		idx = rqval & 0xff;
		if ( idx >= DV_USB_STRING_DESCRIPTOR_LENGTH )
			return 0;				/* Totally out of range */

		len = DV_USB_STRING_DESCRIPTOR[idx+DV_USB_DESC_IDX_bLength];
		if ( len == 0 )
			return 0;				/* Descriptor length zero - terminator */

		if ( DV_USB_STRING_DESCRIPTOR[idx+DV_USB_DESC_IDX_bDescriptorType] != DV_USB_DESCRIPTOR_STRING )
			return 0;				/* Not a string descriptor */

		dv_ep0_state.data = &DV_USB_STRING_DESCRIPTOR[idx];
		dv_ep0_state.count = (rqlen > len) ? len : rqlen;
		ok = 1;
		break;

	default:
		break;
	}

	return ok;
}

/* dv_usb_setup_request_device_standard() - handle a standard device setup request
*/
dv_i32_t dv_usb_setup_request_device_standard(dv_usb_setup_packet_t *pkt)
{
	dv_i32_t ok = 0;
	dv_u8_t rq = pkt->b[DV_USB_SETUPPKT_bRequest];
	dv_u16_t rqlen = dv_usb_load_16(&pkt->b[DV_USB_SETUPPKT_wLength]); 	/* Requested data length */
	dv_u16_t rqval = dv_usb_load_16(&pkt->b[DV_USB_SETUPPKT_wValue]);	/* Request value */

	switch ( rq )
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
			callout_feature_event();
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
			callout_feature_event();
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
		ok = dv_usb_setup_request_device_get_descriptor(rqlen, rqval);
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
#if 0
		ok = USB_SetConfiguration_Device();	/* TODO */
#endif
#if DV_CFG_USB_CONFIGURE_EVENT
		callout_configure_event();
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

/* dv_usb_setup_request_interface_get_descriptor() - send the requested descriptor
 *
 * Descriptor can be one of DV_USB_DESCRIPTOR_DEVICE, DV_USB_DESCRIPTOR_CONFIGURATION, DV_USB_DESCRIPTOR_STRING.
*/
static dv_i32_t dv_usb_setup_request_interface_get_descriptor(dv_u16_t rqlen, dv_u16_t rqval, dv_u16_t rqidx)
{
	dv_i32_t ok = 0;
	dv_u16_t dtype = rqval >> 8;
	dv_u16_t dnum;
	dv_u16_t i;
	dv_u16_t idx;
	dv_u16_t len;

	switch ( dtype )
	{
#if 0	/* ToDo: have an interface request handler for each interface? */
	case DV_USB_HID_DESCRIPTOR_CLASS:
		ok = dv_usb_get_hid_class_descriptor(rqlen, rqidx);
		break;

	case DV_USB_HID_DESCRIPTOR_REPORT:
		ok = dv_usb_get_hid_class_descriptor(rqlen, rqidx);
		break;

	case DV_USB_HID_DESCRIPTOR_PHYSICAL:
		ok = dv_usb_get_hid_class_descriptor(rqlen, rqidx);
		break;
#endif

	default:
		break;
	}

	return ok;
}

/* dv_usb_setup_request_interface_standard() - handle a standard interface setup request
*/
dv_i32_t dv_usb_setup_request_interface_standard(dv_usb_setup_packet_t *pkt)
{
	dv_i32_t ok = 0;
	dv_u8_t rq = pkt->b[DV_USB_SETUPPKT_bRequest];
	dv_u16_t rqlen = dv_usb_load_16(&pkt->b[DV_USB_SETUPPKT_wLength]); 	/* Requested data length */
	dv_u16_t rqidx = dv_usb_load_16(&pkt->b[DV_USB_SETUPPKT_wIndex]);	/* Request index */
	dv_u16_t rqval = dv_usb_load_16(&pkt->b[DV_USB_SETUPPKT_wValue]);	/* Request value */

	switch ( rq )
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
		ok = dv_usb_setup_request_interface_get_descriptor(rqlen, rqval, rqidx);
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
#if 0
		ok = USB_SetInterface();	/* TODO */
#endif
#if DV_CFG_USB_INTERFACE_EVENT
		if ( ok )
			callout_interface_event();
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
