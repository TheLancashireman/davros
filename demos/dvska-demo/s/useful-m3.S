/* useful-m3.S - hardware-specific assembly functions for davroska demo
 *
 * Copyright David Haworth
 *
 * This file is part of davros.
 *
 * davros is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * davros is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with davros.  If not, see <http://www.gnu.org/licenses/>.
 *
 *
 *
*/

/* Some of these functions will move to the davroska or devices subdir eventually
*/

/* 
void sysinfo(void)
{
	dv_printf("Sysinfo:\n");
	dv_printf("SP        = ", getSp());
	dv_printf("mainSP    = ", getMainSp());
	dv_printf("threadSP  = ", getThreadSp());
	dv_printf("PRIMASK   = ", getPrimask());
	dv_printf("FAULTMASK = ", getFaultmask());
	dv_printf("BASEPRI   = ", getBasepri());
	dv_printf("CONTROL   = ", getControl());
	dv_printf("XPSR      = ", getXpsr());
}

void switchToPsp(unsigned *psp, unsigned *msp, unsigned control, void (*fp)(void));
*/

	.text
	.globl	getSp
	.globl	getMainSp
	.globl	getThreadSp
	.globl	getPrimask
	.globl	getFaultmask
	.globl	getBasepri
	.globl	getControl
	.globl	getXpsr
	.globl	switchToPsp

getSp:
	mov		r0, sp
	bx		lr

getMainSp:
	mrs		r0, MSP
	bx		lr

getThreadSp:
	mrs		r0, PSP
	bx		lr

getPrimask:
	mrs		r0, PRIMASK
	bx		lr

getFaultmask:
	mrs		r0, FAULTMASK
	bx		lr

getBasepri:
	mrs		r0, BASEPRI
	bx		lr

getControl:
	mrs		r0, CONTROL
	bx		lr

getXpsr:
	mrs		r0, XPSR
	bx		lr

switchToPsp:
	msr		PSP, r0
	msr		CONTROL, r2
	msr		MSP, r1
	bx		r3
