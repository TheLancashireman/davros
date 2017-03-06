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

#include <kernel/h/dv-types.h>

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
/* Todo: floating point registers */
};

#endif

#define DV_OFFSET_regs_gpr0		0
#define DV_OFFSET_regs_gpr8		32		/* (8*4)	*/
#define DV_OFFSET_regs_pc		52		/* (13*4)	*/
#define DV_OFFSET_regs_cpsr		56
#define DV_OFFSET_regs_sp		60

#endif
