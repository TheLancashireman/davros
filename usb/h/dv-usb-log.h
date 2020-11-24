/*	dv-usb-log.h - logs usb requests for debugging
 *
 *	Copyright David Haworth
 *
 *	This file is part of davros.
 *
 *	davros is free software: you can redistribute it and/or modify
 *	it under the terms of the GNU General Public License as published by
 *	the Free Software Foundation, either version 3 of the License, or
 *	(at your option) any later version.
 *
 *	davros is distributed in the hope that it will be useful,
 *	but WITHOUT ANY WARRANTY; without even the implied warranty of
 *	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *	GNU General Public License for more details.
 *
 *	You should have received a copy of the GNU General Public License
 *	along with davros.  If not, see <http://www.gnu.org/licenses/>.
*/
#ifndef dv_usb_log_h
#define dv_usb_log_h	1

#include <dv-logger.h>

#define DV_USB_LOG_LENGTH	256

typedef struct dv_usb_log_setup_s
{
	dv_u8_t bmRequestType;
	dv_u8_t bRequest;
	dv_u16_t wValue;
	dv_u16_t wIndex;
	dv_u16_t wLength;
} dv_usb_log_setup_t;

extern dv_logger_t dv_usb_logger;
extern dv_usb_log_setup_t dv_usb_log_buffer[DV_USB_LOG_LENGTH];

static inline void dv_usblog_init()
{
	dv_log_init(&dv_usb_logger, DV_USB_LOG_LENGTH);
}

static inline void dv_usb_log(dv_u8_t T, dv_u8_t R, dv_u16_t V, dv_u16_t I, dv_u16_t L)
{
#if 0
	dv_i16_t i = dv_log_get_write_idx(&dv_usb_logger);
	if ( i < 0 )
		return;

	dv_usb_log_buffer[i].bmRequestType = T;
	dv_usb_log_buffer[i].bRequest = R;
	dv_usb_log_buffer[i].wValue = V;
	dv_usb_log_buffer[i].wIndex = I;
	dv_usb_log_buffer[i].wLength = L;

	dv_log_upd_write_idx(&dv_usb_logger);
#endif
}

static inline void dv_usb_print(void)
{
	for (;;)
	{
		dv_i16_t i = dv_log_get_read_idx(&dv_usb_logger);
		if ( i < 0 )
			return;

		dv_printf("USB: %02x %02x %04x %04x %04x\n",
						dv_usb_log_buffer[i].bmRequestType,
						dv_usb_log_buffer[i].bRequest,
						dv_usb_log_buffer[i].wValue,
						dv_usb_log_buffer[i].wIndex,
						dv_usb_log_buffer[i].wLength);

		dv_log_upd_read_idx(&dv_usb_logger);
	}
}

#endif
