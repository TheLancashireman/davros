/* dv-rp2040.h
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
#ifndef DV_RP2040_H
#define DV_RP2040_H

/* The peripherals have "mirror" addresses that allow atomic access
*/
#define	DV_OFFSET_XOR	0x1000		/* XOR (i.e. toggle) */
#define	DV_OFFSET_W1S	0x2000		/* Write 1 to set */
#define	DV_OFFSET_W1C	0x3000		/* Write 1 to clear */

#endif
