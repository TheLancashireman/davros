/* dv-cortexm-ctxsw.S - context switch functions for Cortex-M CPUs
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

	.text
	.globl	dv_switchToPsp
	.globl	dv_preemptframe
	.globl	dv_preemptframe_end

/* void dv_switchToPsp(unsigned *psp, unsigned *msp, unsigned control, void (*fp)(void));
 *
 * Switches to the PSP and calls a function.
*/
dv_switchToPsp:
	msr		PSP, r0
	msr		CONTROL, r2
	msr		MSP, r1
	bx		r3

/* void dv_preemptframe(void (*fp)(void));
 *
 * Calls a function in a preemption frame then exits via svc.
*/
dv_preemptframe:
	blx		r0
	svc		#0
dv_preemptframe_end:
	b		dv_preemptframe_end
