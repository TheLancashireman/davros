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

/* USB HID report items
 *
 * Each item is an identifier code followed by some data.
 *
 * The macros are sorted in alphabetical order
*/
#define DV_USB_HID_Collection(x)			0xa1, dv_usb_init_8_c90(x)
#define DV_USB_HID_EndCollection()			0xc0
#define DV_USB_HID_Feature(x)				0xb1, dv_usb_init_8_c90(x)
#define DV_USB_HID_Input(x)					0x81, dv_usb_init_8_c90(x)
#define DV_USB_HID_LogicalMaxL(x)			0x27, dv_usb_init_32_c90(x)
#define DV_USB_HID_LogicalMaxS(x)			0x26, dv_usb_init_16_c90(x)
#define DV_USB_HID_LogicalMax(x)			0x25, dv_usb_init_8_c90(x)
#define DV_USB_HID_LogicalMinL(x)			0x17, dv_usb_init_32_c90(x)
#define DV_USB_HID_LogicalMinS(x)			0x16, dv_usb_init_16_c90(x)
#define DV_USB_HID_LogicalMin(x)			0x15, dv_usb_init_8_c90(x)
#define DV_USB_HID_Output(x)				0x91, dv_usb_init_8_c90(x)
#define DV_USB_HID_PhysicalMaxL(x)			0x47, dv_usb_init_32_c90(x)
#define DV_USB_HID_PhysicalMaxS(x)			0x46, dv_usb_init_16_c90(x)
#define DV_USB_HID_PhysicalMax(x)			0x45, dv_usb_init_8_c90(x)
#define DV_USB_HID_PhysicalMinL(x)			0x37, dv_usb_init_32_c90(x)
#define DV_USB_HID_PhysicalMinS(x)			0x36, dv_usb_init_16_c90(x)
#define DV_USB_HID_PhysicalMin(x)			0x35, dv_usb_init_8_c90(x)
#define DV_USB_HID_Pop()					0xb0
#define DV_USB_HID_Push()					0xa0
#define DV_USB_HID_ReportCount(x)			0x95, dv_usb_init_8_c90(x)
#define DV_USB_HID_ReportID(x)				0x85, dv_usb_init_8_c90(x)
#define DV_USB_HID_ReportSize(x)			0x75, dv_usb_init_8_c90(x)
#define DV_USB_HID_UnitExponent(x)			0x55, dv_usb_init_8_c90(x)
#define DV_USB_HID_UnitL(x)					0x67, dv_usb_init_32_c90(x)
#define DV_USB_HID_UnitS(x)					0x66, dv_usb_init_16_c90(x)
#define DV_USB_HID_Unit(x)					0x65, dv_usb_init_8_c90(x)
#define DV_USB_HID_UsageMax(x)				0x29, dv_usb_init_8_c90(x)
#define DV_USB_HID_UsageMin(x)				0x19, dv_usb_init_8_c90(x)
#define DV_USB_HID_UsagePageVendor(x)		0x06, dv_usb_init_8_c90(x), 0xff
#define DV_USB_HID_UsagePage(x)				0x05, dv_usb_init_8_c90(x)
#define DV_USB_HID_Usage(x)					0x09, dv_usb_init_8_c90(x)

/* HID request codes
*/
#define DV_USB_HID_REQ_GET_REPORT			1
#define DV_USB_HID_REQ_GET_IDLE				2
#define DV_USB_HID_REQ_GET_PROTOCOL			3
#define DV_USB_HID_REQ_SET_REPORT			9
#define DV_USB_HID_REQ_SET_IDLE				10
#define DV_USB_HID_REQ_SET_PROTOCOL			11

/* HID report types
*/
#define DV_USB_HID_REPORT_INPUT				1
#define DV_USB_HID_REPORT_OUTPUT			2
#define DV_USB_HID_REPORT_FEATURE			3

/* HID data (input, output, feature)
*/
#define DV_USB_HID_Data						0x00
#define DV_USB_HID_Constant					0x01
#define DV_USB_HID_Array					0x00
#define DV_USB_HID_Variable					0x02
#define DV_USB_HID_Absolute					0x00
#define DV_USB_HID_Relative					0x04
#define DV_USB_HID_NoWrap					0x00
#define DV_USB_HID_Wrap						0x08
#define DV_USB_HID_Linear					0x00
#define DV_USB_HID_NonLinear				0x10
#define DV_USB_HID_PreferredState		 	0x00
#define DV_USB_HID_NoPreferred				0x20
#define DV_USB_HID_NoNullPosition		 	0x00
#define DV_USB_HID_NullState				0x40
#define DV_USB_HID_NonVolatile				0x00
#define DV_USB_HID_Volatile					0x80

/* HID collection data
*/
#define DV_USB_HID_Physical					0
#define DV_USB_HID_Application				1
#define DV_USB_HID_Logical					2
#define DV_USB_HID_Report					3
#define DV_USB_HID_NamedArray				4
#define DV_USB_HID_UsageSwitch				5
#define DV_USB_HID_UsageModifier			6

/* HID usage pages
*/
#define DV_USB_HID_UPAGE_UNDEFINED			0
#define DV_USB_HID_UPAGE_GENERIC			1
#define DV_USB_HID_UPAGE_SIMULATION			2
#define DV_USB_HID_UPAGE_VR					3
#define DV_USB_HID_UPAGE_SPORT				4
#define DV_USB_HID_UPAGE_GAME				5
#define DV_USB_HID_UPAGE_DEV_CONTROLS		6
#define DV_USB_HID_UPAGE_KEYBOARD			7
#define DV_USB_HID_UPAGE_LED				8
#define DV_USB_HID_UPAGE_BUTTON				9
#define DV_USB_HID_UPAGE_ORDINAL			10
#define DV_USB_HID_UPAGE_TELEPHONY			11
#define DV_USB_HID_UPAGE_CONSUMER			12
#define DV_USB_HID_UPAGE_DIGITIZER			13
#define DV_USB_HID_UPAGE_UNICODE			16
#define DV_USB_HID_UPAGE_ALPHANUMERIC		20

/* Generic desktop page (0x01)
*/
#define DV_USB_HID_UGENERIC_POINTER							0x01
#define DV_USB_HID_UGENERIC_MOUSE							0x02
#define DV_USB_HID_UGENERIC_JOYSTICK						0x04
#define DV_USB_HID_UGENERIC_GAMEPAD							0x05
#define DV_USB_HID_UGENERIC_KEYBOARD						0x06
#define DV_USB_HID_UGENERIC_KEYPAD							0x07
#define DV_USB_HID_UGENERIC_X								0x30
#define DV_USB_HID_UGENERIC_Y								0x31
#define DV_USB_HID_UGENERIC_Z								0x32
#define DV_USB_HID_UGENERIC_RX								0x33
#define DV_USB_HID_UGENERIC_RY								0x34
#define DV_USB_HID_UGENERIC_RZ								0x35
#define DV_USB_HID_UGENERIC_SLIDER							0x36
#define DV_USB_HID_UGENERIC_DIAL							0x37
#define DV_USB_HID_UGENERIC_WHEEL							0x38
#define DV_USB_HID_UGENERIC_HATSWITCH						0x39
#define DV_USB_HID_UGENERIC_COUNTED_BUFFER					0x3a
#define DV_USB_HID_UGENERIC_BYTE_COUNT						0x3b
#define DV_USB_HID_UGENERIC_MOTION_WAKEUP					0x3c
#define DV_USB_HID_UGENERIC_VX								0x40
#define DV_USB_HID_UGENERIC_VY								0x41
#define DV_USB_HID_UGENERIC_VZ								0x42
#define DV_USB_HID_UGENERIC_VBRX							0x43
#define DV_USB_HID_UGENERIC_VBRY							0x44
#define DV_USB_HID_UGENERIC_VBRZ							0x45
#define DV_USB_HID_UGENERIC_VNO								0x46
#define DV_USB_HID_UGENERIC_SYSTEM_CTL						0x80
#define DV_USB_HID_UGENERIC_SYSCTL_POWER					0x81
#define DV_USB_HID_UGENERIC_SYSCTL_SLEEP					0x82
#define DV_USB_HID_UGENERIC_SYSCTL_WAKE						0x83
#define DV_USB_HID_UGENERIC_SYSCTL_CONTEXT_MENU				0x84
#define DV_USB_HID_UGENERIC_SYSCTL_MAIN_MENU				0x85
#define DV_USB_HID_UGENERIC_SYSCTL_APP_MENU					0x86
#define DV_USB_HID_UGENERIC_SYSCTL_HELP_MENU				0x87
#define DV_USB_HID_UGENERIC_SYSCTL_MENU_EXIT				0x88
#define DV_USB_HID_UGENERIC_SYSCTL_MENU_SELECT				0x89
#define DV_USB_HID_UGENERIC_SYSCTL_MENU_RIGHT				0x8a
#define DV_USB_HID_UGENERIC_SYSCTL_MENU_LEFT				0x8b
#define DV_USB_HID_UGENERIC_SYSCTL_MENU_UP					0x8c
#define DV_USB_HID_UGENERIC_SYSCTL_MENU_DOWN				0x8d

/* Simulation controls page (0x02)
*/
#define DV_USB_HID_USAGE_SIMULATION_RUDDER					0xba
#define DV_USB_HID_USAGE_SIMULATION_THROTTLE				0xbb

/* Virtual reality controls page (0x03)
*/

/* Sport controls page (0x04)
*/

/* Game controls page (0x05)
*/

/* Generic device controls page (0x06)
*/

/* Keyboard/keypad page (0x07)
*/

/* Error "keys"
*/
#define DV_USB_HID_USAGE_KEYBOARD_NOEVENT					0x00
#define DV_USB_HID_USAGE_KEYBOARD_ROLLOVER					0x01
#define DV_USB_HID_USAGE_KEYBOARD_POSTFAIL					0x02
#define DV_USB_HID_USAGE_KEYBOARD_UNDEFINED					0x03

/* Letters
*/
#define DV_USB_HID_USAGE_KEYBOARD_aA						0x04
#define DV_USB_HID_USAGE_KEYBOARD_zZ						0x1d

/* Numbers
*/
#define DV_USB_HID_USAGE_KEYBOARD_ONE						0x1e
#define DV_USB_HID_USAGE_KEYBOARD_ZERO						0x27

#define DV_USB_HID_USAGE_KEYBOARD_RETURN					0x28
#define DV_USB_HID_USAGE_KEYBOARD_ESCAPE					0x29
#define DV_USB_HID_USAGE_KEYBOARD_DELETE					0x2a

/* Function keys
*/
#define DV_USB_HID_USAGE_KEYBOARD_F1						0x3a
#define DV_USB_HID_USAGE_KEYBOARD_F12						0x45

#define DV_USB_HID_USAGE_KEYBOARD_PRINT_SCREEN				0x46

/* Modifier keys
*/
#define DV_USB_HID_USAGE_KEYBOARD_LCTRL						0xe0
#define DV_USB_HID_USAGE_KEYBOARD_LSHFT						0xe1
#define DV_USB_HID_USAGE_KEYBOARD_LALT						0xe2
#define DV_USB_HID_USAGE_KEYBOARD_LGUI						0xe3
#define DV_USB_HID_USAGE_KEYBOARD_RCTRL						0xe4
#define DV_USB_HID_USAGE_KEYBOARD_RSHFT						0xe5
#define DV_USB_HID_USAGE_KEYBOARD_RALT						0xe6
#define DV_USB_HID_USAGE_KEYBOARD_RGUI						0xe7
#define DV_USB_HID_USAGE_KEYBOARD_SCROLL_LOCK				0x47
#define DV_USB_HID_USAGE_KEYBOARD_NUM_LOCK					0x53
#define DV_USB_HID_USAGE_KEYBOARD_CAPS_LOCK					0x39

/* LED page (0x08)
*/
#define DV_USB_HID_USAGE_LED_NUM_LOCK						0x01
#define DV_USB_HID_USAGE_LED_CAPS_LOCK						0x02
#define DV_USB_HID_USAGE_LED_SCROLL_LOCK					0x03
#define DV_USB_HID_USAGE_LED_COMPOSE						0x04
#define DV_USB_HID_USAGE_LED_KANA							0x05
#define DV_USB_HID_USAGE_LED_POWER							0x06
#define DV_USB_HID_USAGE_LED_SHIFT							0x07
#define DV_USB_HID_USAGE_LED_DO_NOT_DISTURB					0x08
#define DV_USB_HID_USAGE_LED_MUTE							0x09
#define DV_USB_HID_USAGE_LED_TONE_ENABLE					0x0a
#define DV_USB_HID_USAGE_LED_HIGH_CUT_FILTER				0x0b
#define DV_USB_HID_USAGE_LED_LOW_CUT_FILTER					0x0c
#define DV_USB_HID_USAGE_LED_EQUALIZER_ENABLE				0x0d
#define DV_USB_HID_USAGE_LED_SOUND_FIELD_ON					0x0e
#define DV_USB_HID_USAGE_LED_SURROUND_FIELD_ON				0x0f
#define DV_USB_HID_USAGE_LED_REPEAT							0x10
#define DV_USB_HID_USAGE_LED_STEREO							0x11
#define DV_USB_HID_USAGE_LED_SAMPLING_RATE_DETECT			0x12
#define DV_USB_HID_USAGE_LED_SPINNING						0x13
#define DV_USB_HID_USAGE_LED_CAV							0x14
#define DV_USB_HID_USAGE_LED_CLV							0x15
#define DV_USB_HID_USAGE_LED_RECORDING_FORMAT_DET			0x16
#define DV_USB_HID_USAGE_LED_OFF_HOOK						0x17
#define DV_USB_HID_USAGE_LED_RING							0x18
#define DV_USB_HID_USAGE_LED_MESSAGE_WAITING				0x19
#define DV_USB_HID_USAGE_LED_DATA_MODE						0x1a
#define DV_USB_HID_USAGE_LED_BATTERY_OPERATION				0x1b
#define DV_USB_HID_USAGE_LED_BATTERY_OK						0x1c
#define DV_USB_HID_USAGE_LED_BATTERY_LOW					0x1d
#define DV_USB_HID_USAGE_LED_SPEAKER						0x1e
#define DV_USB_HID_USAGE_LED_HEAD_SET						0x1f
#define DV_USB_HID_USAGE_LED_HOLD							0x20
#define DV_USB_HID_USAGE_LED_MICROPHONE						0x21
#define DV_USB_HID_USAGE_LED_COVERAGE						0x22
#define DV_USB_HID_USAGE_LED_NIGHT_MODE						0x23
#define DV_USB_HID_USAGE_LED_SEND_CALLS						0x24
#define DV_USB_HID_USAGE_LED_CALL_PICKUP					0x25
#define DV_USB_HID_USAGE_LED_CONFERENCE						0x26
#define DV_USB_HID_USAGE_LED_STAND_BY						0x27
#define DV_USB_HID_USAGE_LED_CAMERA_ON						0x28
#define DV_USB_HID_USAGE_LED_CAMERA_OFF						0x29
#define DV_USB_HID_USAGE_LED_ON_LINE						0x2a
#define DV_USB_HID_USAGE_LED_OFF_LINE						0x2b
#define DV_USB_HID_USAGE_LED_BUSY							0x2c
#define DV_USB_HID_USAGE_LED_READY							0x2d
#define DV_USB_HID_USAGE_LED_PAPER_OUT						0x2e
#define DV_USB_HID_USAGE_LED_PAPER_JAM						0x2f
#define DV_USB_HID_USAGE_LED_REMOTE							0x30
#define DV_USB_HID_USAGE_LED_FORWARD						0x31
#define DV_USB_HID_USAGE_LED_REVERSE						0x32
#define DV_USB_HID_USAGE_LED_STOP							0x33
#define DV_USB_HID_USAGE_LED_REWIND							0x34
#define DV_USB_HID_USAGE_LED_FAST_FORWARD					0x35
#define DV_USB_HID_USAGE_LED_PLAY							0x36
#define DV_USB_HID_USAGE_LED_PAUSE							0x37
#define DV_USB_HID_USAGE_LED_RECORD							0x38
#define DV_USB_HID_USAGE_LED_ERROR							0x39
#define DV_USB_HID_USAGE_LED_SELECTED_INDICATOR				0x3a
#define DV_USB_HID_USAGE_LED_IN_USE_INDICATOR				0x3b
#define DV_USB_HID_USAGE_LED_MULTI_MODE_INDICATOR			0x3c
#define DV_USB_HID_USAGE_LED_INDICATOR_ON					0x3d
#define DV_USB_HID_USAGE_LED_INDICATOR_FLASH				0x3e
#define DV_USB_HID_USAGE_LED_INDICATOR_SLOW_BLINK			0x3f
#define DV_USB_HID_USAGE_LED_INDICATOR_FAST_BLINK			0x40
#define DV_USB_HID_USAGE_LED_INDICATOR_OFF					0x41
#define DV_USB_HID_USAGE_LED_FLASH_ON_TIME					0x42
#define DV_USB_HID_USAGE_LED_SLOW_BLINK_ON_TIME				0x43
#define DV_USB_HID_USAGE_LED_SLOW_BLINK_OFF_TIME			0x44
#define DV_USB_HID_USAGE_LED_FAST_BLINK_ON_TIME				0x45
#define DV_USB_HID_USAGE_LED_FAST_BLINK_OFF_TIME			0x46
#define DV_USB_HID_USAGE_LED_INDICATOR_COLOR				0x47
#define DV_USB_HID_USAGE_LED_RED							0x48
#define DV_USB_HID_USAGE_LED_GREEN							0x49
#define DV_USB_HID_USAGE_LED_AMBER							0x4a
#define DV_USB_HID_USAGE_LED_GENERIC_INDICATOR				0x4b

/* Button page (0x09)
 * There is no need to label these usages
*/

/* Ordinal page (0x0a)
 * There is no need to label these usages
*/

/* Telephony device page (0x0b)
*/
#define DV_USB_HID_USAGE_TELEPHONY_PHONE					1
#define DV_USB_HID_USAGE_TELEPHONY_ANSWERING_MACHINE		2
#define DV_USB_HID_USAGE_TELEPHONY_MESSAGE_CONTROLS			3
#define DV_USB_HID_USAGE_TELEPHONY_HANDSET					4
#define DV_USB_HID_USAGE_TELEPHONY_HEADSET					5
#define DV_USB_HID_USAGE_TELEPHONY_KEYPAD					6
#define DV_USB_HID_USAGE_TELEPHONY_PROGRAMMABLE_BUTTON		7

/* Consumer page (0x0c)
*/
#define DV_USB_HID_USAGE_CONSUMER_CONTROL					1

#endif

