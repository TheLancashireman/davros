	.text

/* If all cores jump here we need to sort them out.
*/
.globl _start
_start:
	msr		DAIFSet, 0xf	/* Enable all exceptions and interrupts */

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

	msr		ELR_EL1, xzr
	msr		ELR_EL2, xzr
	msr		ELR_EL3, xzr
	msr		SPSR_EL1, xzr

	mrs		x1, MPIDR_EL1
	and		x1, x1, #3
	cbz		x1, _start0

qqq:
	wfe
	b	qqq

	sub x1, x1, #1
	cbz x1, _start1

	sub x1, x1, #1
	cbz x1, _start2

	sub x1, x1, #1
	cbz x1, _start3

    mov	sp,#0x8000			/* If the core identification is wrong ... */
	sub	sp,sp,#0x1000
	b	play

.globl _start0
_start0:
    mov	sp,#0x8000
    b	play

.globl _start1
_start1:
    mov	sp,#0x8000
	sub	sp,sp,#0x400
    b	play

.globl _start2
_start2:
    mov	sp,#0x8000
	sub	sp,sp,#0x800
    b	play

.globl _start3
_start3:
    mov	sp,#0x8000
	sub	sp,sp,#0xc00
    b	play

/* go_el1(entry, psr)
 *
 *	entry = address of startup code
 *	psr   = program state to use
*/
.globl go_el1
go_el1:
	msr	spsr_el2, x1
	msr	elr_el2, x0
	eret

