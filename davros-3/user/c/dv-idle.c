/*	dv-idle.c - standard idle loop for davros
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
#include <kernel/h/dv-kconfig.h>
#include <kernel/h/dv-types.h>
#include <kernel/h/dv-coverage.h>

/* Todo: temporary, use kernel printf.
*/
#include <kernel/h/dv-stdio.h>
#define dv_uprintf	dv_kprintf

DV_COVDEF(idle);

/* dv_idle() - idle loop
*/
void dv_idle(void)
{
	dv_uprintf("dv_idle() reached()\n");
	for (;;)
	{
		dv_hw_wait();
	}
}

/* man-page-generation - to be defined
*/
