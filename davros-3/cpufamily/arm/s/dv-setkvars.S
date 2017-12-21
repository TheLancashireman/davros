/*	dv-setkvars.s - store the kvars address in a register
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
#include <kernel/h/dv-asm.h>
#include <kernel/h/dv-kernel.h>
#include <kernel/h/dv-thread.h>
#include <cpufamily/arm/h/dv-arm-registers.h>

	.globl	dv_set_kvars
	.text

dv_set_kvars:
	mcr		p15, 0, r0, c13, c0, 3						/* Store r0 to user read/only pid register */
	bx		lr
