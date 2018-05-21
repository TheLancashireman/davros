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
