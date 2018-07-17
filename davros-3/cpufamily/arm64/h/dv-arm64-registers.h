/*	dv-arm64-registers.h - ARM64 registers structure for davros
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
#ifndef dv_arm64_registers_h
#define dv_arm64_registers_h	1

#include <kernel/h/dv-kconfig.h>
#include <kernel/h/dv-types.h>
#include <kernel/h/dv-thread.h>
#include <kernel/h/dv-executable.h>
#include <kernel/h/dv-api.h>

/* Bit fields in PSR
 *
 * ToDo: this is duplicated in dv-arm64-core.h - remove duplication
*/
#define DV_A64_PSR_MODE		0x0000000f
#define DV_A64_PSR_M4		0x00000010		/* 0 = 64-bit, 1 = 32-bit */
#define DV_A64_PSR_F		0x00000020		/* 0 = FIQ enabled */
#define DV_A64_PSR_I		0x00000040		/* 0 = IRQ enabled */
#define DV_A64_PSR_A		0x00000080		/* 0 = SError enabled */
#define DV_A64_PSR_D		0x00000100		/* 0 = Debug enabled */
#define DV_A64_PSR_IL		0x00100000		/* Illegal execution state */
#define DV_A64_PSR_SS		0x00200000		/* Software step */
#define DV_A64_PSR_V		0x10000000		/* Overflow */
#define DV_A64_PSR_C		0x20000000		/* Carry */
#define DV_A64_PSR_Z		0x40000000		/* Zero */
#define DV_A64_PSR_N		0x80000000		/* Negative */

#define DV_A64_MODE_EL1t	0x4		/* "Thread mode" EL1 (uses EL0_SP) */
#define DV_A64_MODE_EL0		0x0

#if !DV_ASM

/* The structure holding an executable's registers while it isn't running
*/
struct dv_registers_s
{
	dv_u64_t gpr[31];	/* lr is gpr[30] */
	dv_u64_t pc;		/* PC = exception return address ELR_EL1 */
	dv_u64_t psr;		/* Program state register (SPSR_EL1) */
	dv_u64_t sp;		/* Stack pointer (SP_EL0) */
						/* ToDo: floating point registers */
	dv_u32_t esr;		/* Exception syndrome register. Contains syscall index (among other things) */
	int n_exe;			/* No of executables sharing this register store. Required by common code. */
};

/* dv_get_xxx() - get the value of a register
*/
static inline dv_u64_t dv_get_p0(dv_registers_t *regs)
{
	return regs->gpr[0];
}

static inline dv_u64_t dv_get_p1(dv_registers_t *regs)
{
	return regs->gpr[1];
}

static inline dv_u64_t dv_get_p2(dv_registers_t *regs)
{
	return regs->gpr[2];
}

static inline dv_u64_t dv_get_p3(dv_registers_t *regs)
{
	return regs->gpr[3];
}

/* dv_set_registers() - initialise the registers.
 *
 * Fills the registers with the initial values for the executable.
 * This function fills just the standard registers. See also dv_set_parameters().
 *
 * Return value is in same registers as parameters 1 and 2, so we use the standard
 * C function here.
*/
static inline void dv_set_registers(dv_registers_t *regs, dv_executable_t *exe)
{
	regs->pc = (dv_u64_t)exe->function;
	regs->psr = DV_A64_MODE_EL1t | DV_A64_PSR_F;		/* ToDo: from field in exe */
	regs->sp = (dv_u64_t)exe->initial_sp;
	regs->gpr[30] = (dv_u64_t)dv_exit;
}

/* dv_set_xxx() - set the value of a register
*/
static inline void dv_set_rv0(dv_registers_t *regs, dv_u64_t v)
{
	regs->gpr[0] = v;
}

static inline void dv_set_rv1(dv_registers_t *regs, dv_u64_t v)
{
	regs->gpr[1] = v;
}

#endif

#define DV_OFFSET_regs_gpr0		0
#define DV_OFFSET_regs_pc		248		/* (31*8)	*/
#define DV_OFFSET_regs_psr		256
#define DV_OFFSET_regs_sp		264
#define DV_OFFSET_regs_esr		272

#endif
