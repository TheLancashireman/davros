	.text

/* If all cores jump here we need to sort them out.
*/
.globl _start
_start:

	mrs	x1, MPIDR_EL1
	and	x1, x1, #3
	cbz	x1, _start0

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

/* Flush and invalidate cache
 *
 * Stolen from DAI0527A_baremetal_boot_code_for_ARMv8_A_processors.pdf
*/
.globl flush_cache
flush_cache:
	mov		x0, #0x0				/* x0 = Cache level */
	msr		CSSELR_EL1, x0			/* 0x0 for L1 Dcache  0x2 for L2 Dcache. */
	mrs		x4, CCSIDR_EL1			/* Read Cache Size ID. */
	and		x1, x4, #0x7
	add		x1, x1, #0x4			/* x1 = Cache Line Size. */
	ldr		x3, =0x7FFF
	and		x2, x3, x4, LSR #13		/* x2 = Cache Set Number - 1. */
	ldr		x3, =0x3FF
	and		x3, x3, x4, LSR #3		/* x3 = Cache Associativity Number - 1. */
	clz		x4, x3					/* x4 = way position in the CISW instruction. */
	mov		x5, #0					/* x5 = way counter way_loop. */
way_loop:
	mov		x6, #0					/* x6 = set counter set_loop. */
set_loop:
	lsl		x7, x5, x4
	orr		x7, x0, x7				/* Set way. */
	lsl		x8, x6, x1
	orr		x7, x7, x8				/* Set set. */
	dc		cisw, x7				/* Clean and Invalidate cache line. */
	add		x6, x6, #1				/* Increment set counter. */
	cmp		x6, x2					/* Last set reached yet? */
	ble		set_loop				/* If not, iterate set_loop, */
	add		x5, x5, #1				/* else, next way. */
	cmp		x5, x3					/* Last way reached yet? */
	ble		way_loop				/* If not, iterate way_loop. */
	ret
