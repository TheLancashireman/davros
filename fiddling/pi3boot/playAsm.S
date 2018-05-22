	.text

/* If all cores jump here we need to sort them out.
*/
.globl _start
_start:

	mrs	x1, MPIDR_EL1
	and	x1, x1, #3
	cbz	x1, _start0

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

