/*  dv-rp2040-usb.h
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
#ifndef DV_RP2040_USB_H
#define DV_RP2040_USB_H

#include <dv-devices.h>
#include "dv-rp2040.h"

#define DV_RP2040_USB_BASE		0x40034000

extern int dv_rp2040_usb_init(void);

#endif
