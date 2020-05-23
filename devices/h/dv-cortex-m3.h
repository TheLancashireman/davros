/*  dv-cortex-m3.h
 *
 *  Copyright David Haworth
 *
 *  This file is part of davros.
 *
 *  davros is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  davros is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with davros.  If not, see <http://www.gnu.org/licenses/>.
*/
#ifndef DV_CORTEX_M3_H
#define DV_CORTEX_M3_H

#include <dv-devices.h>

/* Aux control registers
*/
typedef struct dv_cortexm3acr_s dv_cortexm3acr_t;

struct dv_cortexm3acr_s
{
	dv_reg32_t unknown;		/* What's this */
	dv_reg32_t ictr;		/* Interrupt controller type */
	dv_reg32_t actlr;		/* Aux control register */
};

#define DV_CORTEXM3ACR_BASE		0xe000e000
#define dv_m3acr				((dv_cortexm3acr_t *)DV_CORTEXM3ACR_BASE)[0]

/* Systick registers
*/
typedef struct dv_cortexm3systick_s dv_cortexm3systick_t;

struct dv_cortexm3systick_s
{
	dv_reg32_t stcsr;		/* SysTick control/status */
	dv_reg32_t strvr;		/* SysTick reload */
	dv_reg32_t stcvr;		/* SysTick current value */
	dv_reg32_t stcr;		/* SysTick calibration value */
};

#define DV_CORTEXM3SYSTICK_BASE	0xe000e010
#define dv_systick				((dv_cortexm3systick_t *)DV_CORTEXM3SYSTICK_BASE)[0]


/* NVIC has its own header file, but the base address is defined here.
*/
#define DV_NVIC_BASE	0xe000e100

/* System control registers
*/
typedef struct dv_cortexm3scr_s dv_cortexm3scr_t;

struct dv_cortexm3scr_s
{
	dv_reg32_t cpuid;		/* RO - CPU identification */
	dv_reg32_t icsr;		/* Interrupt control/state */
	dv_reg32_t vtor;		/* Vector table offset */
	dv_reg32_t aircr;		/* Application interrupt and reset control */
	dv_reg32_t scr;			/* System control */
	dv_reg32_t ccr;			/* Configuration and control */
	dv_reg32_t shpr[3];		/* System handler priority */
	dv_reg32_t shcsr;		/* System handler control and state */
	dv_reg32_t cfsr;		/* Configurable fault status */
	dv_reg32_t hfsr;		/* Hardfault status */
	dv_reg32_t dfsr;		/* Debug fault status */
	dv_reg32_t mmfar;		/* MemManage fault address */
	dv_reg32_t bfar;		/* Bus fault address*/
	dv_reg32_t afsr;		/* Auxiliary fault status */
	dv_reg32_t id_pfr[2];	/* RO */
	dv_reg32_t id_dfr0;		/* RO */
	dv_reg32_t id_afr0;		/* RO */
	dv_reg32_t id_mmfr[4];	/* RO */
	dv_reg32_t id_isar[5];	/* RO */
};

#define DV_CORTEXM3SCR_BASE		0xe000ed00
#define dv_m3scr				((dv_cortexm3scr_t *)DV_CORTEXM3SCR_BASE)[0]

/* CPACR and STIR are well separated from the SCR
*/
#define DV_CORTEXM3_CPACR		0xe000ed88
#define DV_m3cpacr				((dv_reg32_t *)DV_CORTEXM3_CPACR)[0]

#define DV_CORTEXM3_STIR		0xe000ef00
#define DV_m3stir				((dv_reg32_t *)DV_CORTEXM3_STIR)[0]

#define DV_SYST_COUNTFLAG		0x00010000		/* 1 ==> counter has counted to zero (read-only, clear by reading) */
#define DV_SYST_CLKSRC			0x00000004		/* 1 = CPU, 0 = ext. ref */
#define DV_SYST_TICKINT			0x00000002		/* 1 = trigger exception when timer reaches 0 */
#define DV_SYST_ENABLE			0x00000001		/* 1 = counter is enabled */

#define DV_SYST_MASK			0x00ffffff		/* Max value mask */

#define DV_XPSR_IPSR			0x000001ff		/* Mask for IPSR in XPSR */

/* dv_get_msp()/dv_set_msp() - get and set the MSP register (main SP)
 *
 * WARNING: dv_set_msp() results in undefined behaviour if main SP is the current SP.
*/
static inline dv_u32_t dv_get_msp(void)
{
	dv_u32_t msp;
	__asm__ volatile("mrs %[reg], MSP" : [reg] "=r" (msp) : : );
	return msp;
}
static inline void dv_set_msp(dv_u32_t msp)
{
	__asm__ volatile ("msr MSP, %[reg]" : : [reg] "r" (msp) : );
}

/* dv_get_psp()/dv_set_psp() - get and set the PSP register (process SP)
 *
 * WARNING: dv_set_psp() results in undefined behaviour if process SP is the current SP.
*/
static inline dv_u32_t dv_get_psp(void)
{
	dv_u32_t psp;
	__asm__ volatile("mrs %[reg], PSP" : [reg] "=r" (psp) : : );
	return psp;
}
static inline void dv_set_psp(dv_u32_t psp)
{
	__asm__ volatile ("msr PSP, %[reg]" : : [reg] "r" (psp) : );
}

/* dv_get_primask()/dv_set_primask() - get and set the PRIMASK register
*/
static inline dv_u32_t dv_get_primask(void)
{
	dv_u32_t primask;
	__asm__ volatile("mrs %[reg], PRIMASK" : [reg] "=r" (primask) : : );
	return primask;
}
static inline void dv_set_primask(dv_u32_t primask)
{
	__asm__ volatile ("msr PRIMASK, %[reg]" : : [reg] "r" (primask) : );
}

/* dv_get_faultmask()/dv_set_faultmask() - get and set the FAULTMASK register
*/
static inline dv_u32_t dv_get_faultmask(void)
{
	dv_u32_t faultmask;
	__asm__ volatile("mrs %[reg], FAULTMASK" : [reg] "=r" (faultmask) : : );
	return faultmask;
}
static inline void dv_set_faultmask(dv_u32_t faultmask)
{
	__asm__ volatile ("msr FAULTMASK, %[reg]" : : [reg] "r" (faultmask) : );
}

/* dv_get_basepri()/dv_set_basepri() - get and set the BASEPRI register
*/
static inline dv_u32_t dv_get_basepri(void)
{
	dv_u32_t basepri;
	__asm__ volatile("mrs %[reg], BASEPRI" : [reg] "=r" (basepri) : : );
	return basepri;
}
static inline void dv_set_basepri(dv_u32_t basepri)
{
	__asm__ volatile ("msr BASEPRI, %[reg]" : : [reg] "r" (basepri) : );
}

/* dv_get_control()/dv_set_control() - get and set the CONTROL register
*/
static inline dv_u32_t dv_get_control(void)
{
	dv_u32_t control;
	__asm__ volatile("mrs %[reg], CONTROL" : [reg] "=r" (control) : : );
	return control;
}
static inline void dv_set_control(dv_u32_t control)
{
	__asm__ volatile ("msr CONTROL, %[reg]" : : [reg] "r" (control) : );
}

/* dv_get_xpsr()/dv_set_xpsr() - get and set the XPSR register
*/
static inline dv_u32_t dv_get_xpsr(void)
{
	dv_u32_t xpsr;
	__asm__ volatile("mrs %[reg], XPSR" : [reg] "=r" (xpsr) : : );
	return xpsr;
}
static inline void dv_set_xpsr(dv_u32_t xpsr)
{
	__asm__ volatile ("msr XPSR, %[reg]" : : [reg] "r" (xpsr) : );
}

/* dv_get_ipsr() - get the IPSR register
 *
 * IPSR is part of XPSR (bits 0..8). During interrupt/exception handling it contains the vector number.
 * IPSR cannot be written.
*/
static inline dv_u32_t dv_get_ipsr(void)
{
	dv_u32_t ipsr;
	__asm__ volatile("mrs %[reg], IPSR" : [reg] "=r" (ipsr) : : );
	return ipsr;
}
/* dv_get_sp() - get the current stack pointer
 *
 * There is no "set" function for the sp. Changing the SP from C code would result in undefined behaviour.
*/
static inline dv_u32_t dv_get_sp(void)
{
	dv_u32_t sp;
	__asm__ volatile("mov %[reg], sp" : [reg] "=r" (sp) : : );
	return sp;
}

/* Interrupt status, locking and unlocking
*/
#define DV_INTENABLED	0x0		/* PRIMASK value to enable interrupts */
#define DV_INTDISABLED	0x1		/* PRIMASK value to disable interrupts */

static inline dv_intstatus_t dv_disable(void)
{
	dv_intstatus_t old = dv_get_primask();
	dv_set_primask(0x1);
	return old;
}

static inline dv_intstatus_t dv_restore(dv_intstatus_t x)
{
	dv_intstatus_t old = dv_get_primask();
	dv_set_primask(x);
	return old;
}

#endif
