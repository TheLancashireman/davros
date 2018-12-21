/* davroska-arm.c - OSEK-like member of the davros family; ARM specific code
 *
 * (c) David Haworth
*/
#define DV_ASM  0
#include <dv-config.h>
#include <davroska-api.h>
#include <davroska.h>


/* Kernel stack starts 32 bytes below top of RAM
*/
extern dv_u32_t dv_end_ram;
const dv_u32_t dv_initialsp_svc = (dv_u32_t)&dv_end_ram - 32;

/* Only a very small stack is needed for irq and fiq because we switch to svc.
 * The stack is not pushed, just stored, so the initialsp constants contain the address of element 0
 *
 * The kernel copies the stack to handle nested interrupts.
*/
dv_u32_t dv_irqstack[32];
dv_u32_t dv_fiqstack[32];

const dv_u32_t dv_initialsp_irq = (dv_u32_t)&dv_irqstack[0];
const dv_u32_t dv_initialsp_fiq = (dv_u32_t)&dv_fiqstack[0];
