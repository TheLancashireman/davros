/*	dv-arm64-exceptions.h - ARM64 exception handling
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
#ifndef dv_arm64_exceptions_h
#define dv_arm64_exceptions_h	1

#include <kernel/h/dv-kconfig.h>
#include <dv-types.h>

/* ESR_ELx - exception syndrome register
*/
#define DV_ESR_EC	0xfc000000		/* Exception class (see below) */
#define DV_ESR_IL	0x02000000		/* Instruction length */
#define DV_ESR_ISS	0x01ffffff		/* Instruction-specific syndrome */

#define DV_ESR_ISS_SVC	0xffff		/* System-call number from instruction */

/* Enumerated tyoe for the exception class
 *
 * See DDI0487C_a_armv8_arm.pdf
*/
typedef enum dv_arm64_ec_e
{
	dv_ec_unknown	= 0x00,
	dv_ec_wfi_wfe	= 0x01,
	dv_ec_cp15		= 0x03,
	dv_ec_cp15_2	= 0x04,
	dv_ec_cp14		= 0x05,
	dv_ec_ldc_stc	= 0x06,
	dv_ec_fpudis	= 0x07,
	dv_ec_vmrs		= 0x08,
	dv_ec_ptr		= 0x09,
	dv_ec_cp14_2	= 0x0c,
	dv_ec_exstate	= 0x0e,
	dv_ec_svc32		= 0x11,
	dv_ec_hvc32		= 0x12,
	dv_ec_smc32		= 0x13,
	dv_ec_svc		= 0x15,
	dv_ec_hvc		= 0x16,
	dv_ec_smc		= 0x17,
	dv_ec_msrmrs	= 0x18,
	dv_ec_svedis	= 0x19,
	dv_ec_eret		= 0x1a,
	dv_ec_impl_el3	= 0x1f,
	dv_ec_iabt_low	= 0x20,
	dv_ec_iabt_same	= 0x21,
	dv_ec_pc_align	= 0x22,
	dv_ec_dabt_low	= 0x24,
	dv_ec_dabt_same	= 0x25,
	dv_ec_sp_align	= 0x26,
	dv_ec_fpu32		= 0x28,
	dv_ec_fpu		= 0x2c,
	dv_ec_serror	= 0x2f,
	dv_ec_bp_low	= 0x30,
	dv_ec_bp_same	= 0x31,
	dv_ec_step_low	= 0x32,
	dv_ec_step_same	= 0x33,
	dv_ec_wp_low	= 0x34,
	dv_ec_wp_same	= 0x35,
	dv_ec_bkpt32	= 0x38,
	dv_ec_vcatch32	= 0x3a,
	dv_ec_brk		= 0x3c
} dv_arm64_ec_t;

/* Exception catchers.
*/
void dv_catch_thread_synchronous_exception(dv_kernel_t *kvars) __attribute__((noreturn));

#endif
