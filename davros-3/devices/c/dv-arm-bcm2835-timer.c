/*	dv-arm-bcm2835-timer.c - system timer on bcm2835 (raspberry pi)
 *
 *	Copyright 2017 David Haworth
 *
 *	This file is part of davros.
 *
 *	davros is free software: you can redistribute it and/or modify
 *	it under the terms of the GNU General Public License as published by
 *	the Free Software Foundation, either version 3 of the License, or
 *	(at your option) any later version.
 *
 *	davros is distributed in the hope that it will be useful,
 *	but WITHOUT ANY WARRANTY; without even the implied warranty of
 *	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *	GNU General Public License for more details.
 *
 *	You should have received a copy of the GNU General Public License
 *	along with davros.  If not, see <http://www.gnu.org/licenses/>.
*/
#include <kernel/h/dv-kconfig.h>
#include <kernel/h/dv-types.h>
#include <cpufamily/arm/h/dv-arm-bcm2835-timer.h>
#include <cpufamily/arm/h/dv-arm-bcm2835-interruptcontroller.h>
#include <kernel/h/dv-interrupt.h>
#include <kernel/h/dv-executable.h>

/* dv_arm_bcm2835_timerinterrupt() - handle a timer interrupt from the BCM2835 system timer
 *
 * This function is intended to be used as a "software vectoring" function for an
 * interrupt from the system timer.
 * Counter match registers CM1 and CM3 can be used - CM0 and CM2 appear to be used by the GPU.
 *
 * On entry, the interrupt has been disabled by the vectoring functions, so will need to be re-enabled.
 * The "param" parameter will be either 1 or 3.
 *
 * In the absence of any programmed timing requirements (e.g. from dv_sleep() etc.), a regular
 * 1 second tick is generated.
*/
void dv_arm_bcm2835_timerinterrupt(dv_kernel_t *kvars, dv_address_t param)
{
	int chan = (int)param;
	dv_u64_t next;

	DV_DBG(dv_kprintf("dv_arm_bcm2835_timerinterrupt(): param = %d\n", chan));

	dv_clrmatch(chan);

	next = dv_wakeup(kvars);

	dv_set_system_timer_alarm(next);
}

/* dv_init_system_timer() - initialise the system timer.
 *
 * This function initialises a channel of the BCM2835 system timer module for use as
 * Davros's system timer.
 *
 * The timer appears to tick at 1 MHz.
*/
#if 1			/* Use timer channel 1 */
#define IID		dv_iid_syst_cm1
#define CMP		1
#else			/* Use timer channel 3 */
#define IID		dv_iid_syst_cm3
#define CMP		3
#endif

void dv_init_system_timer(dv_kernel_t *kvars)
{
	dv_u64_t t = dv_readtime();
	dv_setcmp(CMP, (t & 0xffffffff) + 1000000);
	dv_attach_irq(IID, dv_arm_bcm2835_timerinterrupt, CMP);
	dv_enable_irq(IID);
}

/* dv_set_system_timer_alarm() - sets the system timer compare register
 *
 * This function sets the system timer's compare register so that a match will
 * occur when the timer reaches the specified value.
 * If the value is more than 0xc0000000 in the future, the compare register is set
 * to 0x80000000 in the future.
*/
void dv_set_system_timer_alarm(dv_u64_t t)
{
	dv_u64_t now = dv_readtime();

	if ( t <= (now + DV_MIN_SLEEP) )
		t = now + DV_MIN_SLEEP;
	else
	if ( (t - now) > 0xc0000000 )
		t = now + 0x80000000;

	dv_setcmp(CMP, (dv_u32_t)t);
	dv_enable_irq(IID);
}
