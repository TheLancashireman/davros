/*  dv-usb-ep0.c
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

#if 0
void USB_EndPoint0 (DWORD event) {

  switch (event) {

    case USB_EVT_SETUP:
#endif

/* dv_usb_ep0_ev_setup() - handle a setup event on EP0
*/
void dv_usb_ep0_ev_setup(void)
{
	dv_usb_setup_packet_t setup_packet;

	/* Read the setup packet from the endpoint
	*/
	dv_usb_read_ep(0, &setup_packet.b[0], DV_USB_SETUPPKT_LEN);

#if 0		/* TODO - not needed on stm32 */
	USB_DirCtrlEP(SetupPacket.bmRequestType.BM.Dir);
#endif

	EP0Data.Count = SetupPacket.wLength;					/* TODO - move to subfunction ?*/

	/* Get packet type and call corresponding subfunction
	*/
	switch ( setup_packet.b[DV_USB_SETUPPKT_bmRequestType] & DV_USB_SETUPPKT_TYPE )
	{
	case DV_USB_SETUPPKT_Standard:
		dv_usb_setup_packet_standard(&setup_packet);
		break;

	case DV_USB_SETUPPKT_Class:
		dv_usb_setup_packet_class(&setup_packet);
		break;

	case DV_USB_SETUPPKT_Vendor:
		/* Fall through */

	default:
		/* stall_i */
		USB_SetStallEP(0x80);		/* TODO */
		EP0Data.Count = 0;			/* TODO */
		break;
	}
}

/* dv_usb_setup_packet_standard() - handle a standard setup request on EP0
*/
void dv_usb_setup_packet_standard(dv_usb_setup_packet_t *pkt)
{
	switch ( pkt->b[DV_USB_SETUPPKT_bRequest] )
	{
	case DV_USB_REQ_GetStatus:
		if ( !dv_usb_GetStatus(pkt) )
		{
			/* stall_i */
			USB_SetStallEP(0x80);		/* TODO */
			EP0Data.Count = 0;			/* TODO */
		}
		break;

	case DV_USB_REQ_ClearFeature:
		if (!USB_SetClrFeature(0)) {
                goto stall_i;
              }
              USB_StatusInStage();
#if USB_FEATURE_EVENT
              USB_Feature_Event();
#endif
              break;

	case DV_USB_REQ_SetFeature:
              if (!USB_SetClrFeature(1)) {
                goto stall_i;
              }
              USB_StatusInStage();
#if USB_FEATURE_EVENT
              USB_Feature_Event();
#endif
		break;

	case DV_USB_REQ_SetAddress:
              switch (SetupPacket.bmRequestType.BM.Recipient) {
                case REQUEST_TO_DEVICE:
                  USB_DeviceAddress = 0x80 | SetupPacket.wValue.WB.L;
                  USB_StatusInStage();
                  break;
                default:
                  goto stall_i;
              }
		break;

	case DV_USB_REQ_GetDescriptor:
		if (!USB_GetDescriptor()) {
                goto stall_i;
              }
		break;

	case DV_USB_REQ_SetDescriptor:
		USB_SetStallEP(0x00);
		EP0Data.Count = 0;
		break;

	case DV_USB_REQ_GetConfiguration:
              switch (SetupPacket.bmRequestType.BM.Recipient) {
                case REQUEST_TO_DEVICE:
                  EP0Data.pData = &USB_Configuration;
                  USB_DataInStage();
                  break;
                default:
                  goto stall_i;
              }
		break;

	case DV_USB_REQ_SetConfiguration:
              switch (SetupPacket.bmRequestType.BM.Recipient) {
                case REQUEST_TO_DEVICE:
                  if (!USB_SetConfiguration()) {
                    goto stall_i;
                  }
                  USB_StatusInStage();
#if USB_CONFIGURE_EVENT
                  USB_Configure_Event();
#endif
                  break;
                default:
                  goto stall_i;
              }
		break;

	case DV_USB_REQ_GetInterface:
              switch (SetupPacket.bmRequestType.BM.Recipient) {
                case REQUEST_TO_INTERFACE:
                  if ((USB_Configuration != 0) &&
                      (SetupPacket.wIndex.WB.L < USB_NumInterfaces)) {
                    EP0Data.pData = USB_AltSetting + SetupPacket.wIndex.WB.L;
                    USB_DataInStage();
                  } else {
                    goto stall_i;
                  }
                  break;
                default:
                  goto stall_i;
              }
		break;

	case DV_USB_REQ_SetInterface:
              switch (SetupPacket.bmRequestType.BM.Recipient) {
                case REQUEST_TO_INTERFACE:
                  if (!USB_SetInterface()) {
                    goto stall_i;
                  }
                  USB_StatusInStage();
#if USB_INTERFACE_EVENT
                  USB_Interface_Event();
#endif
                  break;
                default:
                  goto stall_i;
              }
		break;

	default:
		USB_SetStallEP(0x80);		/* TODO */
		EP0Data.Count = 0;			/* TODO */
		break;
	}
}

/* dv_usb_setup_packet_class() - handle a class setup packet
*/
void dv_usb_setup_packet_class(dv_usb_setup_packet_t *pkt)
{
#if USB_CLASS
          switch (SetupPacket.bmRequestType.BM.Recipient) {
            case REQUEST_TO_INTERFACE:
#if USB_HID
              if (SetupPacket.wIndex.WB.L == USB_HID_IF_NUM) {
                switch (SetupPacket.bRequest) {
                  case HID_REQUEST_GET_REPORT:
                    if (HID_GetReport()) {
                      EP0Data.pData = EP0Buf;
                      USB_DataInStage();
                      goto class_ok;
                    }
                    break;
                  case HID_REQUEST_SET_REPORT:
                    EP0Data.pData = EP0Buf;
                    goto class_ok;
                  case HID_REQUEST_GET_IDLE:
                    if (HID_GetIdle()) {
                      EP0Data.pData = EP0Buf;
                      USB_DataInStage();
                      goto class_ok;
                    }
                    break;
                  case HID_REQUEST_SET_IDLE:
                    if (HID_SetIdle()) {
                      USB_StatusInStage();
                      goto class_ok;
                    }
                    break;
                  case HID_REQUEST_GET_PROTOCOL:
                    if (HID_GetProtocol()) {
                      EP0Data.pData = EP0Buf;
                      USB_DataInStage();
                      goto class_ok;
                    }
                    break;
                  case HID_REQUEST_SET_PROTOCOL:
                    if (HID_SetProtocol()) {
                      USB_StatusInStage();
                      goto class_ok;
                    }
                    break;
                }
              }
#endif  /* USB_HID */
#if USB_MSC
              if (SetupPacket.wIndex.WB.L == USB_MSC_IF_NUM) {
                switch (SetupPacket.bRequest) {
                  case MSC_REQUEST_RESET:
                    if (MSC_Reset()) {
                      USB_StatusInStage();
                      goto class_ok;
                    }
                    break;
                  case MSC_REQUEST_GET_MAX_LUN:
                    if (MSC_GetMaxLUN()) {
                      EP0Data.pData = EP0Buf;
                      USB_DataInStage();
                      goto class_ok;
                    }
                    break;
                }
              }
#endif  /* USB_MSC */
#if USB_AUDIO
              if ((SetupPacket.wIndex.WB.L == USB_ADC_CIF_NUM)  ||
                  (SetupPacket.wIndex.WB.L == USB_ADC_SIF1_NUM) ||
                  (SetupPacket.wIndex.WB.L == USB_ADC_SIF2_NUM)) {
                if (SetupPacket.bmRequestType.BM.Dir) {
                  if (ADC_IF_GetRequest()) {
                    EP0Data.pData = EP0Buf;
                    USB_DataInStage();
                    goto class_ok;
                  }
                } else {
                  EP0Data.pData = EP0Buf;
                  goto class_ok;
                }
              }
#endif  /* USB_AUDIO */
              goto stall_i;
#if USB_AUDIO
            case REQUEST_TO_ENDPOINT:
              if (SetupPacket.bmRequestType.BM.Dir) {
                if (ADC_EP_GetRequest()) {
                  EP0Data.pData = EP0Buf;
                  USB_DataInStage();
                  goto class_ok;
                }
              } else {
                EP0Data.pData = EP0Buf;
                goto class_ok;
              }
              goto stall_i;
#endif  /* USB_AUDIO */
            default:
              goto stall_i;
          }
class_ok: break;
#else
          goto stall_i;
#endif  /* USB_CLASS */
}




#if 0
/*   Other events
{
  {
*/
    case USB_EVT_OUT:
      if (SetupPacket.bmRequestType.BM.Dir == 0) {
        if (EP0Data.Count) {
          USB_DataOutStage();
          if (EP0Data.Count == 0) {
            switch (SetupPacket.bmRequestType.BM.Type) {
              case REQUEST_STANDARD:
                goto stall_i;
#if (USB_CLASS)
              case REQUEST_CLASS:
                switch (SetupPacket.bmRequestType.BM.Recipient) {
                  case REQUEST_TO_INTERFACE:
#if USB_HID
                    if (SetupPacket.wIndex.WB.L == USB_HID_IF_NUM) {
                      if (!HID_SetReport()) {
                        goto stall_i;
                      }
                      break;
                    }
#endif
#if USB_AUDIO
                    if ((SetupPacket.wIndex.WB.L == USB_ADC_CIF_NUM)  ||
                        (SetupPacket.wIndex.WB.L == USB_ADC_SIF1_NUM) ||
                        (SetupPacket.wIndex.WB.L == USB_ADC_SIF2_NUM)) {
                      if (!ADC_IF_SetRequest()) {
                        goto stall_i;
                      }
                      break;
                    }
#endif
                    goto stall_i;
                  case REQUEST_TO_ENDPOINT:
#if USB_AUDIO
                    if (ADC_EP_SetRequest()) break;
#endif
                    goto stall_i;
                  default:
                    goto stall_i;
                }
                break;
#endif
              default:
                goto stall_i;
            }
            USB_StatusInStage();
          }
        }
      } else {
        USB_StatusOutStage();
      }
      break;

    case USB_EVT_IN:
      if (SetupPacket.bmRequestType.BM.Dir == 1) {
        USB_DataInStage();
      } else {
        if (USB_DeviceAddress & 0x80) {
          USB_DeviceAddress &= 0x7F;
          USB_SetAddress(USB_DeviceAddress);
        }
      }
      break;

    case USB_EVT_IN_STALL:
      USB_ClrStallEP(0x80);
      break;

    case USB_EVT_OUT_STALL:
      USB_ClrStallEP(0x00);
      break;

  }
}
