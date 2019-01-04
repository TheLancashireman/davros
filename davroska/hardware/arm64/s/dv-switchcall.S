/*	dv-switchcall.S - stack switch and call
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
#define DV_ASM	1
#include <arm/h/dv-arm-registers.h>

	.globl	dv_switchcall
	.extern	dv_panic_return_from_switchcall_function

	.text

/* dv_switchcall() - switch stack and jump to a function
 *
 * Parameters:
 *	x0	- parameter to called function
 *	x1	- parameter to called function
 *	x2	- new stack pointer
 *	x3	- function to call - must never return
*/

dv_switchcall:
	mov		sp, x2
	blr		x3
	b		dv_panic_return_from_switchcall_function
