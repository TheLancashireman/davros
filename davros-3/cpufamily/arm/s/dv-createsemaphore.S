/*	dv-createsemaphore.S - ARM "create_semaphore" system call
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
#include <kernel/h/dv-syscall.h>

	.globl	dv_create_semaphore
	.text

dv_create_semaphore:
	mov		r12, r0
	mov		r0, r1
	mov		r1, r2
	mov		r2, r3
	swi		#DV_SC_create_semaphore
	str		r0, [r12, #0]
	str		r1, [r12, #4]
	bx		lr
