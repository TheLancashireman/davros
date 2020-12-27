/*  dv-usb.c
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

dv_usb_device_t dv_usb_dev;
dv_u8_t dv_ep0_buffer[DV_CFG_EP0_SIZE];

/* dv_usb_nullepfunc() - do nothing
 *
 * Used for filling unused slots in the epstate structures
*/
void dv_usb_nullepfunc(dv_u16_t ep)
{
	dv_printf("dv_usb_nullepfunc() : ep = %d\n", ep);
	dv_usb_log(0xee, 0x02, ep, 0x0000, 0x0000);
}

/* dv_usb_init_ep() - initialize an endpoint
*/
void dv_usb_init_ep(dv_i32_t ep, dv_u16_t len, void (*sfn)(dv_u16_t), void (*rfn)(dv_u16_t), void (*tfn)(dv_u16_t))
{
	dv_usb_dev.epstate[ep].setup_func = sfn;
	dv_usb_dev.epstate[ep].rx_func = rfn;
	dv_usb_dev.epstate[ep].tx_func = tfn;
	dv_usb_dev.epstate[ep].max_tx = len;
	dv_usb_dev.epstate[ep].max_rx = len;
	dv_usb_dev.epstate[ep].data = DV_NULL;
	dv_usb_dev.epstate[ep].count = 0;
	dv_usb_dev.epstate[ep].status = 0;
}

/* dv_usb_init() - initialise the USB stack
*/
void dv_usb_init(void)
{
	int i;
	dv_usb_dev.device_status = 0;
	dv_usb_dev.device_address = 0;
	dv_usb_dev.configuration = 0;
	dv_usb_dev.n_interfaces = DV_CFG_USB_N_INTERFACES;
	for ( i = 1; i < DV_CFG_USB_N_ENDPOINTS; i++ )
	{
		dv_usb_init_ep(i, 0, &dv_usb_nullepfunc, &dv_usb_nullepfunc, &dv_usb_nullepfunc);
	}

	dv_usb_init_ep(0, DV_CFG_EP0_SIZE, &dv_usb_ep0_ev_setup, &dv_usb_ep0_ev_rx, &dv_usb_ep0_ev_tx);
}
