/* usb-midi.c - source file for davroska USB-MIDI executables
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
*/
#define DV_ASM	0
#include <dv-config.h>
#include <davroska.h>
#include <usb-midi.h>
#include DV_DEMO_TARGET
#include <tusb.h>

extern dv_id_t UsbRead;

/* A combination of interfaces must have a unique product id, since PC will save device driver after the first plug.
 * Same VID/PID with different interface e.g MSC (first), then CDC (later) will possibly cause system error on PC.
 *
 * Auto ProductID layout's Bitmap:
 *   [MSB]       MIDI | HID | MSC | CDC          [LSB]
 */
#define _PID_MAP(itf, n)  ( (CFG_TUD_##itf) << (n) )
#define USB_PID           (0x4000 | _PID_MAP(CDC, 0) | _PID_MAP(MSC, 1) | _PID_MAP(HID, 2) | \
                           _PID_MAP(MIDI, 3) | _PID_MAP(VENDOR, 4) )

/* Device descriptors
*/
tusb_desc_device_t const desc_device =
{
	.bLength				= sizeof(tusb_desc_device_t),
	.bDescriptorType		= TUSB_DESC_DEVICE,
	.bcdUSB					= 0x0200,
	.bDeviceClass			= 0x00,
	.bDeviceSubClass		= 0x00,
	.bDeviceProtocol		= 0x00,
	.bMaxPacketSize0		= CFG_TUD_ENDPOINT0_SIZE,

	.idVendor				= 0xCafe,
	.idProduct				= USB_PID,
	.bcdDevice				= 0x0100,

	.iManufacturer			= 0x01,
	.iProduct				= 0x02,
	.iSerialNumber			= 0x03,

	.bNumConfigurations 	= 0x01
};

/* tud_descriptor_device_cb() - invoked when GET DEVICE DESCRIPTOR received
 *
 * Returns pointer to descriptor
*/
uint8_t const * tud_descriptor_device_cb(void)
{
	return (uint8_t const *) &desc_device;
}


/* Configuration Descriptor
*/
enum
{
	ITF_NUM_MIDI = 0,
	ITF_NUM_MIDI_STREAMING,
	ITF_NUM_TOTAL
};

#define CONFIG_TOTAL_LEN	(TUD_CONFIG_DESC_LEN + TUD_MIDI_DESC_LEN)

#if CFG_TUSB_MCU == OPT_MCU_LPC175X_6X || CFG_TUSB_MCU == OPT_MCU_LPC177X_8X || CFG_TUSB_MCU == OPT_MCU_LPC40XX

/* LPC 17xx and 40xx endpoint type (bulk/interrupt/iso) are fixed by its number
 * 0 control, 1 In, 2 Bulk, 3 Iso, 4 In etc ...
*/
#define EPNUM_MIDI	0x02

#else
#define EPNUM_MIDI	0x01
#endif

uint8_t const desc_fs_configuration[] =
{
	/* Config number, interface count, string index, total length, attribute, power in mA
	*/
	TUD_CONFIG_DESCRIPTOR(1, ITF_NUM_TOTAL, 0, CONFIG_TOTAL_LEN, 0x00, 100),

	/* Interface number, string index, EP Out & EP In address, EP size
	*/
	TUD_MIDI_DESCRIPTOR(ITF_NUM_MIDI, 0, EPNUM_MIDI, 0x80 | EPNUM_MIDI, 64)
};

#if TUD_OPT_HIGH_SPEED
uint8_t const desc_hs_configuration[] =
{
	/* Config number, interface count, string index, total length, attribute, power in mA
	*/
	TUD_CONFIG_DESCRIPTOR(1, ITF_NUM_TOTAL, 0, CONFIG_TOTAL_LEN, 0x00, 100),

	/* Interface number, string index, EP Out & EP In address, EP size
	*/
	TUD_MIDI_DESCRIPTOR(ITF_NUM_MIDI, 0, EPNUM_MIDI, 0x80 | EPNUM_MIDI, 512)
};
#endif

/* tud_descriptor_configuration_cb() - invoked when GET CONFIGURATION DESCRIPTOR received
 *
 * Returns pointer to descriptor
 * Descriptor contents must exist long enough for transfer to complete
*/
uint8_t const * tud_descriptor_configuration_cb(uint8_t unused_index)
{
#if TUD_OPT_HIGH_SPEED
	/* Although we are highspeed, host may be fullspeed.
	*/
	return (tud_speed_get() == TUSB_SPEED_HIGH) ?	desc_hs_configuration : desc_fs_configuration;
#else
	return desc_fs_configuration;
#endif
}

/* String Descriptors
 *
 * array of pointer to string descriptors
*/
char const* string_desc_arr [] =
{
	(const char[]) { 0x09, 0x04 },	/* 0: is supported language is English (0x0409)	*/
	"Dave",							/* 1: Manufacturer */
	"MidiMagic",					/* 2: Product */
	"123456",						/* 3: Serials, should use chip ID */
};

static uint16_t _desc_array[32];

/* tud_descriptor_string_cb() - invoked when GET STRING DESCRIPTOR received
 *
 * Returns pointer to utf-16 string descriptor
 * Descriptor must exist long enough for transfer to complete
 *
 * Is there an endianness-dependency here? Need to check the USB spec.
*/
uint16_t const* tud_descriptor_string_cb(uint8_t index, uint16_t unused_langid)
{
	uint8_t chr_count;

	if ( index == 0)
	{
		memcpy(&_desc_array[1], string_desc_arr[0], 2);
		chr_count = 1;
	}
	else
	{
		/* Note: the 0xEE index string is a Microsoft OS 1.0 Descriptors.
		 * https://docs.microsoft.com/en-us/windows-hardware/drivers/usbcon/microsoft-defined-usb-descriptors
		*/
		if ( !(index < sizeof(string_desc_arr)/sizeof(string_desc_arr[0])) ) return NULL;

		const char* str = string_desc_arr[index];

		/* Cap at max char
		*/
		chr_count = strlen(str);
		if ( chr_count > 31 )	chr_count = 31;

		/* Convert ASCII string into UTF-16
		*/
		for ( int i = 0; i < chr_count; i++ )
		{
			_desc_array[1+i] = str[i];
		}
	}

	/* first byte is length (including header), second byte is string type
	*/
	_desc_array[0] = (TUSB_DESC_STRING << 8 ) | (2*chr_count + 2);

	return _desc_array;
}

/* tud_midi_rx_cb() - called whenever MIDI is received
*/
void tud_midi_rx_cb(uint8_t itf)
{
	dv_activatetask(UsbRead);
}
