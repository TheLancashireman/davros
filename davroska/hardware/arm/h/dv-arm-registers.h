/*	dv-arm-registers.h - ARM registers for davroska
 *
 *	Copyright 2018 David Haworth
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
#ifndef dv_arm_registers_h
#define dv_arm_registers_h	1

#define DV_ARM_MODE_USR		0x10
#define DV_ARM_MODE_FIQ		0x11
#define DV_ARM_MODE_IRQ		0x12
#define DV_ARM_MODE_SVC		0x13
#define DV_ARM_MODE_ABT		0x17
#define DV_ARM_MODE_UNDEF	0x1B
#define DV_ARM_MODE_SYS		0x1F	/* available on ARM Arch 4 and later */

#define DV_ARM_IRQ_DIS		0x80	/* when I bit is set, IRQ is disabled */
#define DV_ARM_FIQ_DIS		0x40	/* when F bit is set, FIQ is disabled */

#endif
