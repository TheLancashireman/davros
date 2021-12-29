/* demo-hid.c - HID functionality for OSEK/TinyUSB demo
 *
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
 * (c) David Haworth
*/
#define DV_ASM	0
#include "demo.h"
#include "tusb.h"

/* send_hid_report() - the application calls this function to send an event (keypress, etc.) to the host
 *
 * Parameters:	report_id - keyboard/mouse/etc.
 *				btn - key pressed/released
*/
void send_hid_report(uint8_t report_id, uint32_t btn)
{
	/* Ignore if USB device isn't ready.
	*/
	if ( !tud_hid_ready() ) return;

	switch(report_id)
	{
		case REPORTID_KEYBOARD:
			if ( btn )
			{
				uint8_t keycode[6] = { 0 };
				keycode[0] = HID_KEY_A;

				tud_hid_keyboard_report(REPORTID_KEYBOARD, 0, keycode);
			}
			else
			{
				tud_hid_keyboard_report(REPORTID_KEYBOARD, 0, NULL);
			}
			break;

#if 0
		case REPORTID_MOUSE:
			break;

		case REPORTID_CONSUMER_CONTROL:
			break;

		case REPORTID_GAMEPAD:
			break;
#endif

		default: break;
	}
}

/* tud_hid_report_complete_cb() - invoked when REPORT successfully sent to host
 *
 * The application can use this to send the next report.
 * For composite reports, report[0] is the report ID.
*/
void tud_hid_report_complete_cb(uint8_t instance, uint8_t const* report, uint8_t len)
{
	uint8_t next_report_id = report[0] + 1;

	if (next_report_id <= REPORTID_LAST)
	{
		send_hid_report(next_report_id, 0);
	}
}


/* tud_hid_get_report_cb() - invoked when GET_REPORT received
 *
 * The application must fill buffer report's content and return its length.
 * Returning zero will cause the stack to send a STALL request
*/
uint16_t tud_hid_get_report_cb
(	uint8_t instance,
	uint8_t report_id,
	hid_report_type_t report_type,
	uint8_t* buffer,
	uint16_t reqlen
)
{
	return 0;
}

/* tud_hid_set_report_cb() - invoked when a SET_REPORT received.
 * Also when data is received on an OUT endpoint.
*/
void tud_hid_set_report_cb
(	uint8_t instance,
	uint8_t report_id,
	hid_report_type_t report_type,
	uint8_t const* buffer,
	uint16_t bufsize
)
{
	if ( report_type == HID_REPORT_TYPE_OUTPUT )
	{
		if (report_id == REPORTID_KEYBOARD)
		{
			/* Set keyboard LEDs
			*/
			if ( bufsize < 1 ) return;		/* bufsize should be (at least) 1 */

			uint8_t const kbd_leds = buffer[0];

			if (kbd_leds & KEYBOARD_LED_CAPSLOCK)
			{
				/* Capslock on
				*/
			}else
			{
				/* Capslock off
				*/
			}
		}
	}
}

/* tud_mount_cb() - invoked when device is mounted
*/
void tud_mount_cb(void)
{
}

/* tud_umount_cb() - invoked when device is unmounted
*/
void tud_umount_cb(void)
{
}

/* tud_suspend_cb() - invoked when usb bus is suspended
 *
 * Parameter: remote_wakeup_en - true if host allows us to perform remote wakeup
 *
 * Within 7ms, device must draw an average of current less than 2.5 mA from bus
*/
void tud_suspend_cb(bool remote_wakeup_en)
{
}

/* tud_resume_cb() - invoked when usb bus is resumed
*/
void tud_resume_cb(void)
{
}

/* A combination of interfaces must have a unique product id, since PC will save device driver after the first plug.
 * Same VID/PID with different interface e.g MSC (first), then CDC (later) will possibly cause system error on PC.
 *
 * Auto ProductID layout's Bitmap:
 *	 [MSB]				 HID | MSC | CDC					[LSB]
*/
#define _PID_MAP(itf, n)	( (CFG_TUD_##itf) << (n) )
#define USB_PID					 (0x4000 | _PID_MAP(CDC, 0) | _PID_MAP(MSC, 1) | _PID_MAP(HID, 2) | \
													 _PID_MAP(MIDI, 3) | _PID_MAP(VENDOR, 4) )

#define USB_VID	 0xCafe
#define USB_BCD	 0x0200

/* Device descriptor
*/
tusb_desc_device_t const desc_device =
{
		.bLength			= sizeof(tusb_desc_device_t),
		.bDescriptorType	= TUSB_DESC_DEVICE,
		.bcdUSB				= USB_BCD,
		.bDeviceClass		= 0x00,
		.bDeviceSubClass	= 0x00,
		.bDeviceProtocol	= 0x00,
		.bMaxPacketSize0	= CFG_TUD_ENDPOINT0_SIZE,

		.idVendor			= USB_VID,
		.idProduct			= USB_PID,
		.bcdDevice			= 0x0100,

		.iManufacturer		= 0x01,
		.iProduct			= 0x02,
		.iSerialNumber		= 0x03,

		.bNumConfigurations	= 0x01
};

/* tud_descriptor_device_cb() - invoked when GET DEVICE DESCRIPTOR received
 *
 * Returns a pointer to the descriptor
*/
uint8_t const * tud_descriptor_device_cb(void)
{
	return (uint8_t const *) &desc_device;
}

/* HID report descriptor
*/
uint8_t const desc_hid_report[] =
{
	TUD_HID_REPORT_DESC_KEYBOARD(HID_REPORT_ID(REPORTID_KEYBOARD)),
#ifdef REPORTID_MOUSE
	TUD_HID_REPORT_DESC_MOUSE(HID_REPORT_ID(REPORTID_MOUSE)),
#endif
#ifdef REPORTID_CONSUMER_CONTROL
	TUD_HID_REPORT_DESC_CONSUMER(HID_REPORT_ID(REPORTID_CONSUMER_CONTROL)),
#endif
#ifdef REPORTID_GAMEPAD
	TUD_HID_REPORT_DESC_GAMEPAD(HID_REPORT_ID(REPORTID_GAMEPAD))
#endif
};

/* tud_hid_descriptor_report_cb() - invoked when GET HID REPORT DESCRIPTOR received
 *
 * Returns pointer to descriptor
 * Descriptor contents must exist long enough for transfer to complete
*/
uint8_t const * tud_hid_descriptor_report_cb(uint8_t instance)
{
	return desc_hid_report;
}

/* Configuration Descriptor
*/
enum
{
	ITF_NUM_HID,
	ITF_NUM_TOTAL
};

#define	CONFIG_TOTAL_LEN	(TUD_CONFIG_DESC_LEN + TUD_HID_DESC_LEN)

#define EPNUM_HID	 0x81

uint8_t const desc_configuration[] =
{
	TUD_CONFIG_DESCRIPTOR
	(	1,										/* Config number */
		ITF_NUM_TOTAL,							/* Interface count */
		0,										/* String index */
		CONFIG_TOTAL_LEN,						/* Total length */
		TUSB_DESC_CONFIG_ATT_REMOTE_WAKEUP,		/* Attribute */
		100										/* Power in mA */
	),
	TUD_HID_DESCRIPTOR
	(	ITF_NUM_HID,							/* Interface number */
		0,										/* String index */
		HID_ITF_PROTOCOL_NONE,					/* Protocol */
		sizeof(desc_hid_report),				/* Report descriptor len */
		EPNUM_HID,								/* EP IN address */
		CFG_TUD_HID_EP_BUFSIZE,					/* size */
		5										/* Polling interval */
	)
};

#if TUD_OPT_HIGH_SPEED
/* High speed capable device must report device_qualifier and other_speed_configuration
*/

/* Other speed configuration
*/
uint8_t desc_other_speed_config[CONFIG_TOTAL_LEN];

/* Device qualifier is mostly similar to device descriptor since we don't change configuration based on speed
*/
tusb_desc_device_qualifier_t const desc_device_qualifier =
{
	.bLength			= sizeof(tusb_desc_device_qualifier_t),
	.bDescriptorType	= TUSB_DESC_DEVICE_QUALIFIER,
	.bcdUSB				= USB_BCD,

	.bDeviceClass		= 0x00,
	.bDeviceSubClass	= 0x00,
	.bDeviceProtocol	= 0x00,

	.bMaxPacketSize0	= CFG_TUD_ENDPOINT0_SIZE,
	.bNumConfigurations	= 0x01,
	.bReserved			= 0x00
};

/* tud_descriptor_device_qualifier_cb() - invoked when GET DEVICE QUALIFIER DESCRIPTOR received
 *
 * Returns pointer to descriptor, whose contents must exist long enough for transfer to complete.
 * Device_qualifier descriptor describes information about a high-speed capable device that would
 * change if the device were operating at the other speed. If not highspeed capable stall this request.
*/
uint8_t const* tud_descriptor_device_qualifier_cb(void)
{
	return (uint8_t const*) &desc_device_qualifier;
}

/* tud_descriptor_other_speed_configuration_cb() - invoked when GET OTHER SPEED CONFIGURATION DESCRIPTOR received
 *
 * Returns pointer to descriptor, whose contents must exist long enough for transfer to complete
 * Configuration descriptor in the other speed e.g if high speed then this is for full speed and vice versa
*/
uint8_t const* tud_descriptor_other_speed_configuration_cb(uint8_t index)
{
	/* Other speed config is same as configuration but with type = OTHER_SPEED_CONFIG
	*/
	memcpy(desc_other_speed_config, desc_configuration, CONFIG_TOTAL_LEN);
	desc_other_speed_config[1] = TUSB_DESC_OTHER_SPEED_CONFIG;

	return desc_other_speed_config;
}

#endif

/* tud_descriptor_configuration_cb() - invoked when GET CONFIGURATION DESCRIPTOR received
 *
 * Returns pointer to descriptor
 * Descriptor contents must exist long enough for transfer to complete
*/
uint8_t const * tud_descriptor_configuration_cb(uint8_t index)
{
	return desc_configuration;
}

/* String Descriptors
*/
char const* string_desc_arr [] =
{
	(const char[]) { 0x09, 0x04 },	/* 0: is supported language is English (0x0409) */
	"TinyUSB",						/* 1: Manufacturer */
	"TinyUSB Device",				/* 2: Product */
	"123456",						/* 3: Serials, should use chip ID */
};

/* USB string descriptors are wide characters (UTF-16)
*/
static uint16_t utf16_buf[32];

/* tud_descriptor_string_cb() - invoked when GET STRING DESCRIPTOR received
 *
 * Returns pointer to descriptor, whose contents must exist long enough for transfer to complete
 *
 * Note: the 0xEE index string is a Microsoft OS 1.0 Descriptor.
 * https://docs.microsoft.com/en-us/windows-hardware/drivers/usbcon/microsoft-defined-usb-descriptors
*/
uint16_t const* tud_descriptor_string_cb(uint8_t index, uint16_t langid)
{
	uint16_t chr_count;

	if ( index == 0)
	{
		memcpy(&utf16_buf[1], string_desc_arr[0], 2);
		chr_count = 1;
	}
	else
	{
		if ( index >= sizeof(string_desc_arr)/sizeof(string_desc_arr[0]) )
			return NULL;		/* String index out of range */

		const char* str = string_desc_arr[index];

		/* Limit the number of characters
		 * The limit is 31 because element 0 of the UTF-16 buffer holds the length and other info.
		*/
		chr_count = strlen(str);
		if ( chr_count > 31 ) chr_count = 31;

		/* Convert ASCII string into UTF-16
		*/
		for ( uint8_t i = 0; i < chr_count; i++ )
		{
			utf16_buf[i+1] = str[i];
		}
	}

	/* First byte is length (including header), second byte is string type
	 * Endianness?
	*/
	utf16_buf[0] = (TUSB_DESC_STRING << 8 ) | (2*(chr_count + 1));

	return utf16_buf;
}
