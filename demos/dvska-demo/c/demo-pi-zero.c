/* demo-pi-zero.c - hardware-specific functions for davroska demo
 *
 * (c) David Haworth
*/
#define DV_ASM	0
#include <dv-config.h>
#include <davroska.h>
#include <dv-stdio.h>
#include <dv-string.h>

#include <dv-arm-bcm2835-uart.h>
#include <dv-arm-bcm2835-aux.h>
#include <dv-arm-bcm2835-gpio.h>
#include <dv-arm-bcm2835-interruptcontroller.h>
#include <dv-armv6-mmu.h>
#include <dv-arm-cp15.h>
#include <dv-arm-bcm2835-systimer.h>
#include <dv-arm-bcm2835-armtimer.h>

extern int main(int argc, char **argv);

/* abt and undef stacks
*/
dv_u32_t dv_abtstack[256];
dv_u32_t dv_undstack[256];

/* Startup and exception handling
*/
extern dv_u32_t dv_start_bss, dv_end_bss, dv_vectortable, dv_vectortable_end;

const dv_u32_t dv_initialsp_abt = (dv_u32_t)&dv_abtstack[256];
const dv_u32_t dv_initialsp_und = (dv_u32_t)&dv_undstack[256];

void dv_board_start(void)
{
	/* Initialise bss
	*/
	dv_memset32(&dv_start_bss, 0,
		((dv_address_t)&dv_end_bss - (dv_address_t)&dv_start_bss + sizeof(dv_u32_t) - 1) / sizeof(dv_u32_t));

	/* Initialise uart and connect it to the stdio functions
	*/
	dv_arm_bcm2835_uart_init(115200, 8, 0);
	dv_arm_bcm2835_uart_console();

	dv_printf("pi-zero starting ...\n");

	/* Copy the vector table to 0
	*/
	dv_memcpy32(0, &dv_vectortable, &dv_vectortable_end - &dv_vectortable);

	/* Set up the MMU
	*/
	dv_armv6_mmu_setup();

	/* Caches
	*/
	dv_printf("CP15 cache type 0x%08x\n", dv_read_cp15_cache_type());

	/* Enable both caches and the write buffer
	*/
	dv_printf("Enabling caches ...\n");
	dv_write_cp15_control(dv_read_cp15_control() | DV_CP15_CTRL_C | DV_CP15_CTRL_W | DV_CP15_CTRL_I);

	/* Enable branch prediction
	*/
	dv_printf("Enabling branch prediction ...\n");
	dv_write_cp15_control(dv_read_cp15_control() | DV_CP15_CTRL_Z);

	/* Enable four GPIO pins for the LEDs.
    */
	dv_arm_bcm2835_gpio_pinconfig(17, DV_pinfunc_output, DV_pinpull_none);
	dv_arm_bcm2835_gpio_pinconfig(18, DV_pinfunc_output, DV_pinpull_none);
	dv_arm_bcm2835_gpio_pinconfig(27, DV_pinfunc_output, DV_pinpull_none);
	dv_arm_bcm2835_gpio_pinconfig(22, DV_pinfunc_output, DV_pinpull_none);

	/* All LEDs off
	*/
	dv_arm_bcm2835_gpio_pin_set(17);
	dv_arm_bcm2835_gpio_pin_set(18);
	dv_arm_bcm2835_gpio_pin_set(27);
	dv_arm_bcm2835_gpio_pin_set(22);

	main(0, 0);
}

void dv_catch_data_abort(void)
{
    dv_printf("%s --- %s\n", "dv_catch_data_abort", "Oops! An exception occurred");
	for (;;) {}
}

void dv_catch_prefetch_abort(void)
{
    dv_printf("%s --- %s\n", "dv_catch_prefetch_abort", "Oops! An exception occurred");
	for (;;) {}
}

void dv_catch_reserved(void)
{
    dv_printf("%s --- %s\n", "dv_catch_reserved", "Oops! An exception occurred");
	for (;;) {}
}

void dv_catch_undef(void)
{
    dv_printf("%s --- %s\n", "dv_catch_undef", "Oops! An exception occurred");
	for (;;) {}
}

void dv_catch_unimplemented(void)
{
    dv_printf("%s --- %s\n", "dv_trap_unimplemented", "Oops! An exception occurred");
	for (;;) {}
}

void dv_catch_sbreak(void)
{
    dv_printf("%s --- %s\n", "dv_trap_sbreak", "Oops! An exception occurred");
	for (;;) {}
}

void dv_catch_fiq(void)
{
    dv_printf("%s --- %s\n", "dv_trap_fiq", "Oops! An exception occurred");
	for (;;) {}
}

void dv_catch_reset(void)
{
    dv_printf("%s --- %s\n", "dv_trap_reset", "Oops! Return from dv_board_start");
	for (;;) {}
}

void dv_panic_return_from_switchcall_function(void)
{
	dv_printf("%s --- %s\n", "dv_switchcall", "Oops! The task wrapper returned");
	dv_panic(dv_panic_UnknownPanic);
}

void dv_panic_failed_return_from_irq(void)
{
	dv_printf("%s --- %s\n", "dv_trap_irq", "Oops! Failed to return from an IRQ");
	dv_panic(dv_panic_UnknownPanic);
}
