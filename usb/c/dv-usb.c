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

/* dv_usb_init() - initialise the USB stack
*/
void dv_usb_init(void)
{
	int i;
	dv_usb_dev.device_status = 0;
	dv_usb_dev.device_address = 0;
	dv_usb_dev.configuration = 0;
	dv_usb_dev.n_interfaces = DV_CFG_USB_N_INTERFACES;
	for ( i = 0; i < DV_CFG_USB_N_ENDPOINTS; i++ )
	{
		dv_usb_dev.epstate[i].data = DV_NULL;
		dv_usb_dev.epstate[i].count = 0;
		dv_usb_dev.epstate[i].status = 0;
		dv_usb_dev.epstate[i].max_tx = 0;
		dv_usb_dev.epstate[i].max_rx = 0;
		dv_usb_dev.epstate[i].setup_func = dv_usb_nullepfunc;
		dv_usb_dev.epstate[i].rx_func = dv_usb_nullepfunc;
		dv_usb_dev.epstate[i].tx_func = dv_usb_nullepfunc;
	}

	dv_usb_dev.epstate[0].setup_func = &dv_usb_ep0_ev_setup;
}
