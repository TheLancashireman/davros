/*	dv-vectors.s - ARM exception and interrupt vectors
 *
 *	Copyright 2015 David Haworth
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

	.globl	dv_vectortable
	.globl	dv_vectortable_end
	.extern	dv_trap_reset
	.extern	dv_trap_undef
	.extern	dv_trap_sbreak
	.extern	dv_trap_prefetch_abort
	.extern	dv_trap_data_abort
	.extern	dv_trap_reserved
	.extern	dv_trap_irq
	.extern	dv_trap_fiq

/* dv_vectortable - a standard ARM vector table
 *     
 * Each vector loads the handler address directly into the PC using PC-relative addressing
*/
	.text

dv_vectortable:
	ldr		pc, dv_hndlraddr_reset
	ldr		pc, dv_hndlraddr_undef
	ldr		pc, dv_hndlraddr_sbreak
	ldr		pc, dv_hndlraddr_prefetch_abort
	ldr		pc, dv_hndlraddr_data_abort
	ldr		pc, dv_hndlraddr_reserved
	ldr		pc, dv_hndlraddr_irq
	ldr		pc, dv_hndlraddr_fiq

dv_hndlraddr_reset:				.word		dv_trap_reset
dv_hndlraddr_undef:				.word		dv_trap_undef
dv_hndlraddr_sbreak:			.word		dv_trap_sbreak
dv_hndlraddr_prefetch_abort:	.word		dv_trap_prefetch_abort
dv_hndlraddr_data_abort:		.word		dv_trap_data_abort
dv_hndlraddr_reserved:			.word		dv_trap_reserved
dv_hndlraddr_irq:				.word		dv_trap_irq
dv_hndlraddr_fiq:				.word		dv_trap_fiq

dv_vectortable_end:

/* Temporary vectors for stuff that isn't handled yet
*/
dv_trap_undef:
dv_trap_prefetch_abort:
dv_trap_data_abort:
dv_trap_reserved:
dv_trap_irq:
dv_trap_fiq:
dv_trap_stop:
	b		dv_trap_stop   
