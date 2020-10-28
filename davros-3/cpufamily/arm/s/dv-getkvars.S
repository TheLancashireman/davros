/*	dv-getkvars.s - retrieve the kvars address from the special-purpose register
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
#include <kernel/h/dv-asm.h>
#include <kernel/h/dv-kernel.h>
#include <kernel/h/dv-thread.h>
#include <cpufamily/arm/h/dv-arm-registers.h>

	.globl	dv_get_kvars
	.text

dv_get_kvars:
	mrc		p15, 0, r0, c13, c0, 3						/* Load r0 from user read/only pid register */
	bx		lr
