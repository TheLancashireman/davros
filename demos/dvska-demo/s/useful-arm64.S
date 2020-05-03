
#include <kernel/h/dv-asm.h>

    .globl  dv_invalidate_setway
    .text

dv_invalidate_setway:
    dc		cisw, x0
	ret

