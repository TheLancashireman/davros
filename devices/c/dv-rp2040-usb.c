/*  dv-rp2040-usb.c
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
#include "dv-rp2040-usb.h"
#include "dv-rp2040-resets.h"

/* dv_rp2040_usb_init() - initialise USB controller. Return 0 if OK.
*/
int dv_rp2040_usb_init(void)
{
	/* If the USB controller is in reset, release the reset and wait till ready.
	*/
	dv_rp2040_release(DV_RESETS_usbctrl);

	return 0;
}
