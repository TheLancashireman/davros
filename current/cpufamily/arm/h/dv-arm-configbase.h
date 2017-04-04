/*	dv-arm-configbase.h - ARM perpheral address header file for davros
 *
 *	Copyright 2017 David Haworth
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
#ifndef dv_arm_configbase_h
#define dv_arm_configbase_h	1

#if !DV_ASM

static inline void *dv_get_config_base(unsigned offset)
{
	unsigned x;
	__asm__ volatile ("mrc	p15, 4, %0, c15, c0, 0" : "=r"(x) : );
	return (void *)(x+offset);
}

#endif

#define DV_GTIMER_OFFSET	0x0200

#endif
