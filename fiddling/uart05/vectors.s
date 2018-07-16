

.globl _start
_start:
    b skip

.space 0x8000-0x0004,0

skip:
	mov		x0, xzr			/* Clear all registers */
	mov		x1, xzr
	mov		x2, xzr
	mov		x3, xzr
	mov		x4, xzr
	mov		x5, xzr
	mov		x6, xzr
	mov		x7, xzr
	mov		x8, xzr
	mov		x9, xzr
	mov		x10, xzr
	mov		x11, xzr
	mov		x12, xzr
	mov		x13, xzr
	mov		x14, xzr
	mov		x15, xzr
	mov		x16, xzr
	mov		x17, xzr
	mov		x18, xzr
	mov		x19, xzr
	mov		x20, xzr
	mov		x21, xzr
	mov		x22, xzr
	mov		x23, xzr
	mov		x24, xzr
	mov		x25, xzr
	mov		x26, xzr
	mov		x27, xzr
	mov		x28, xzr
	mov		x29, xzr
	mov		x30, xzr

	fmov	d0, xzr
	fmov	d1, xzr
	fmov	d2, xzr
	fmov	d3, xzr
	fmov	d4, xzr
	fmov	d5, xzr
	fmov	d6, xzr
	fmov	d7, xzr
	fmov	d8, xzr
	fmov	d9, xzr
	fmov	d10, xzr
	fmov	d11, xzr
	fmov	d12, xzr
	fmov	d13, xzr
	fmov	d14, xzr
	fmov	d15, xzr
	fmov	d16, xzr
	fmov	d17, xzr
	fmov	d18, xzr
	fmov	d19, xzr
	fmov	d20, xzr
	fmov	d21, xzr
	fmov	d22, xzr
	fmov	d23, xzr
	fmov	d24, xzr
	fmov	d25, xzr
	fmov	d26, xzr
	fmov	d27, xzr
	fmov	d28, xzr
	fmov	d29, xzr
	fmov	d30, xzr
	fmov	d31, xzr
	
    mrs x0,mpidr_el1
    mov x1,#0xFF000000
    bic x0,x0,x1
    cbz x0,core_zero
    sub x1,x0,#1
    cbz x1,core_one
    sub x1,x0,#2
    cbz x1,core_two
    sub x1,x0,#3
    cbz x1,core_three

    mrs x0,mpidr_el1
    ;mov x1,#0x40000
    ;str w0,[x1]
    b hang

core_zero:
    mov sp,#0x8000
    bl notmain
hang: b hang

core_one:
    mov x1,#0x6000
	sub	x1,x1,#16
	mov	sp,x1
    mov x1,#0
    str w1,[sp]
    mov x0,#0
core_one_loop:
    ldr w0,[sp]
    cbz w0,core_one_loop
    bl hopper
    b hang

core_two:
    mov x1,#0x4000
	sub	x1,x1,#16
	mov	sp,x1
    mov x1,#0
    str w1,[sp]
    mov x0,#0
core_two_loop:
    ldr w0,[sp]
    cbz w0,core_two_loop
    bl hopper
    b hang

core_three:
    mov x1,#0x2000
	sub	x1,x1,#16
	mov	sp,x1
    mov x1,#0
    str w1,[sp]
    mov x0,#0
core_three_loop:
    ldr w0,[sp]
    cbz w0,core_three_loop
    bl hopper
    b hang

hopper:
    br x0

.globl GETPC
GETPC:
    mov x0,x30
    ret

.globl go_el3_el1
go_el3_el1:
	mov		x0, sp			/* All stack pointers same as EL3 */
	msr		SP_EL2, x0
#if 0
	msr		SP_EL1, x0
	msr		SP_EL0, x0
#endif

	msr		ELR_EL3, x30	/* Exception return address = function return address */
	mov		x0, #5			/* PSTATE = EL1h */
	msr		SPSR_EL3, x0
	eret

.globl go_el3_el2
go_el3_el2:
	mov		x0, sp			/* All stack pointers same as EL3 */
	msr		SP_EL2, x0
#if 0
	msr		SP_EL1, x0
	msr		SP_EL0, x0
#endif

	msr		ELR_EL3, x30	/* Exception return address = function return address */
	mov		x0, #9			/* PSTATE = EL2 */
	msr		SPSR_EL3, x0
	eret

.globl go_el2_el1
go_el2_el1:
	mov		x0, sp			/* All stack pointers same as EL2 */
	msr		SP_EL1, x0
#if 0
	msr		SP_EL0, x0
#endif

	msr		ELR_EL2, x30	/* Exception return address = function return address */
	mov		x0, #5			/* PSTATE = EL2 */
	msr		SPSR_EL2, x0
	eret

//-------------------------------------------------------------------------
//
// Copyright (c) 2016 David Welch dwelch@dwelch.com
// Modified by David Haworth 2018
//
// Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
//
//-------------------------------------------------------------------------
