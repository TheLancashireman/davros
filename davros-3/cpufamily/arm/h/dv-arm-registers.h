/*	dv-arm-registers.h - ARM registers structure for davros
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
#ifndef dv_arm_registers_h
#define dv_arm_registers_h	1

#include <kernel/h/dv-kconfig.h>
#include <kernel/h/dv-types.h>
#include <kernel/h/dv-thread.h>
#include <kernel/h/dv-executable.h>
#include <kernel/h/dv-api.h>

#define DV_ARM_MODE_USR		0x10
#define DV_ARM_MODE_FIQ		0x11
#define DV_ARM_MODE_IRQ		0x12
#define DV_ARM_MODE_SVC		0x13
#define DV_ARM_MODE_ABT		0x17
#define DV_ARM_MODE_UNDEF	0x1B
#define DV_ARM_MODE_SYS		0x1F	/* available on ARM Arch 4 and later */

#define DV_ARM_IRQ_DIS		0x80	/* when I bit is set, IRQ is disabled */
#define DV_ARM_FIQ_DIS		0x40	/* when F bit is set, FIQ is disabled */

#if !DV_ASM

/* The structure holding an executable's registers while it isn't running
*/
struct dv_registers_s
{
	dv_u32_t gpr[13];
	dv_u32_t pc;
	dv_u32_t cpsr;
	dv_u32_t sp;
	dv_u32_t lr;
/* ToDo: floating point registers */
	int n_exe;
};

/* dv_get_xxx() - get the value of a register
*/
static inline dv_u32_t dv_get_p0(dv_registers_t *regs)
{
	return regs->gpr[0];
}

static inline dv_u32_t dv_get_p1(dv_registers_t *regs)
{
	return regs->gpr[1];
}

static inline dv_u32_t dv_get_p2(dv_registers_t *regs)
{
	return regs->gpr[2];
}

static inline dv_u32_t dv_get_p3(dv_registers_t *regs)
{
	return regs->gpr[3];
}

/* dv_set_registers() - initialise the registers.
 *
 * Fills the registers with the initial values for the executable.
 * This function fills just the standard registers. See also dv_set_parameters().
 *
 * Return value (32- and 64-bit) is in same registers as parameters 1 and 2, so we use the standard
 * C function here.
*/
static inline void dv_set_registers(dv_registers_t *regs, dv_executable_t *exe)
{
	regs->pc = (dv_u32_t)exe->function;
	regs->cpsr = DV_ARM_MODE_SYS | DV_ARM_FIQ_DIS;		/* ToDo: from field in exe */
	regs->sp = (dv_u32_t)exe->initial_sp;
	regs->lr = (dv_u32_t)dv_exit;
}

/* dv_set_xxx() - set the value of a register
*/
static inline void dv_set_rv0(dv_registers_t *regs, dv_u32_t v)
{
	regs->gpr[0] = v;
}

static inline void dv_set_rv1(dv_registers_t *regs, dv_u32_t v)
{
	regs->gpr[1] = v;
}

#endif

#define DV_OFFSET_regs_gpr0		0
#define DV_OFFSET_regs_gpr8		32		/* (8*4)	*/
#define DV_OFFSET_regs_pc		52		/* (13*4)	*/
#define DV_OFFSET_regs_cpsr		56
#define DV_OFFSET_regs_sp		60

#endif
