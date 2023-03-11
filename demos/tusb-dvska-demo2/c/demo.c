/* demo.c - USB-serial demo for davroska
 *
 * This file contains all the target-independent stuff - tasks, ISRs etc.
 * The target-dependent stuff (startup, main, timer & uart functions) can be found in
 * hardware specific header/source files.
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
#define DV_ASM	0
#include <dv-config.h>
#include <davroska.h>
#include <dv-stdio.h>
#include <dv-string.h>
#include <dv-ringbuf.h>
#include <dv-tusb-device.h>

/* This include file selects the hardware type
*/
#include DV_DEMO_TARGET

/* This includes the USB functionality
*/
#include "usb-serial.h"

/* Task main functions, along with a description of how they interact.
 *
 * The hardware is assumed to have an LED that can be driven by an output port.
 *
 * The task Led is responsible for blinking the LED at a steady rate of 20ms on every 2 seconds.
*/
/* Object identifiers
*/
dv_id_t Init, Led, UsbRead0, UsbRead1;	/* Tasks */
dv_id_t Uart, Timer;					/* ISRs */
dv_id_t mx_Gpio;						/* Mutexes */
dv_id_t Ticker;							/* Counters */
dv_id_t LedDriver;						/* Alarms */

#define ev_uart1_rx		0x01

dv_boolean_t ledstate;

/* Ringbuffer for UART1 rx
*/
#define UART1_RX_RBLEN	128
dv_rbm_t uart1_rx_rbm;
dv_u8_t uart1_rx_ringbuf[UART1_RX_RBLEN];

/* main_Led() - task body function for the Led task
*/
void main_Led(void)
{
	dv_putc('*');		/* FIXME: temp */
	ledstate = !ledstate;
	hw_SetLed(3, ledstate);
	dv_setalarm_rel(Ticker, LedDriver, ledstate ? 20 : 1980);
}

/* main_Init() - start the ball rolling
*/
void main_Init(void)
{
	dv_printf("main_Init() reached\n");
	sysinfo();
	for ( int i = 0; i < dv_nexe; i++ )
	{
		dv_printf("%d %s  b=%d r=%d c=%d %d\n", i, dv_exe[i].name, dv_exe[i].baseprio, dv_exe[i].runprio,
													dv_exe[i].currprio, dv_exe[i].state);
	}

	callout_tusbd_activate();
}

/* main_UsbRead0() - task body function for the UsbRead0 task
 * Activated when there's data to read from CDC0
*/
void main_UsbRead0(void)
{
	dv_printf("UsbRead0\n");

	while ( tud_cdc_n_available(0) )
	{
		dv_u8_t buf[64];
		dv_u32_t count = tud_cdc_n_read(0, buf, sizeof(buf));
		tud_cdc_n_write(1, buf, count);
	}
	tud_cdc_n_write_flush(1);
}

/* main_UsbRead1() - task body function for the UsbRead1 task
 * Activated when there's data to read from CDC1
*/
void main_UsbRead1(void)
{
	dv_printf("UsbRead1\n");

	while ( tud_cdc_n_available(1) )
	{
		dv_u8_t buf[64];
		dv_u32_t count = tud_cdc_n_read(1, buf, sizeof(buf));
		tud_cdc_n_write(0, buf, count);
	}
	tud_cdc_n_write_flush(0);
}

/* tud_cdc_rx_cb() - called when there's data on a CDC interface
*/
void tud_cdc_rx_cb(dv_u8_t itf)
{
	if ( itf == 0 )
		dv_activatetask(UsbRead0);
	else
		dv_activatetask(UsbRead1);
}

/* main_Uart() - body of ISR to handle uart rx interrupt
*/
void main_Uart(void)
{
#if 0
	dv_printf("main_Uart() called\n");
#endif
	while ( dv_consoledriver.isrx() )
	{
		int c = dv_consoledriver.getc();
#if 0
		dv_printf("uart rx : 0x%02x\n", c);
#endif

		if ( !dv_rb_full(&uart1_rx_rbm) )
		{
			uart1_rx_ringbuf[uart1_rx_rbm.tail] = (dv_u8_t)c;
			uart1_rx_rbm.tail = dv_rb_add1(&uart1_rx_rbm, uart1_rx_rbm.tail);
		}
	}
}

/* main_Timer() - body of ISR to handle interval timer interrupt
*/
void main_Timer(void)
{
	hw_ClearTimer();

	dv_statustype_t ee = dv_advancecounter(Ticker, 1);
	if ( ee != dv_e_ok )
		dv_shutdown(ee);
}

/* af_LedDriver() - alarm function to activate the LedDriver task
*/
dv_u64_t af_LedDriver(dv_id_t unused_a, dv_param_t unused_d)
{
	dv_activatetask(Led);
	return 0;
}

/* callout_addtasks() - configure the tasks
*/
void callout_addtasks(dv_id_t mode)
{
	Init = dv_addtask("Init", &main_Init, 4, 1);
	dv_printf("Init : %d\n", Init);
	Led = dv_addtask("Led", &main_Led, 1, 1);
	dv_printf("Led : %d\n", Led);
	UsbRead0 = dv_addtask("UsbRead0", &main_UsbRead0, 2, 1);
	dv_printf("UsbRead0 : %d\n", UsbRead0);
	UsbRead1 = dv_addtask("UsbRead1", &main_UsbRead1, 2, 1);
	dv_printf("UsbRead1 : %d\n", UsbRead1);

	callout_tusbd_addtasks();
}

/* callout_addisrs() - configure the isrs
*/
void callout_addisrs(dv_id_t mode)
{
	Uart = dv_addisr("Uart", &main_Uart, hw_UartInterruptId, 5);
	Timer = dv_addisr("Timer", &main_Timer, hw_TimerInterruptId, 6);

	callout_tusbd_addisrs();
}

/* callout_addgroups() - configure the executable groups
 *
 * ToDo: remove the contents - just for testing
*/
void callout_addgroups(dv_id_t mode)
{
}

/* callout_addmutexes() - configure the mutexes
*/
void callout_addmutexes(dv_id_t mode)
{
	mx_Gpio = dv_addmutex("mx_Gpio", 1);
		dv_addmutexuser(mx_Gpio, Led);

	callout_tusbd_addmutexes();
}

/* callout_addcounters() - configure the counters
*/
void callout_addcounters(dv_id_t mode)
{
	Ticker = dv_addcounter("Ticker");
}

/* callout_addalarms() - configure the alarms
*/
void callout_addalarms(dv_id_t mode)
{
	LedDriver = dv_addalarm("LedDriver", &af_LedDriver, 0);

	callout_tusbd_addalarms();
}

/* callout_autostart() - start the objects that need to be running after dv_startos()
*/
void callout_autostart(dv_id_t mode)
{
	dv_activatetask(Init);
	dv_setalarm_rel(Ticker, LedDriver, 2000);

	/* Enable interrupts from the UART
	*/
	hw_EnableUartRxInterrupt();
	dv_enable_irq(hw_UartInterruptId);

	/* Initialise timer and enable interrupts
	*/
	hw_InitialiseMillisecondTicker();
	dv_enable_irq(hw_TimerInterruptId);

	/* Enable USB interrupts
	*/
	dv_tusbd_enableirqs();
}

/* callout_reporterror() - called if an error is detected
*/
dv_statustype_t callout_reporterror(dv_sid_t sid, dv_statustype_t e, dv_qty_t nparam, dv_param_t *param)
{
	dv_printf("callout_reporterror(%d, %d, %d, ...) called.\n", sid, e, nparam);
	for (int i = 0; i < nparam; i++ )
	{
		/* Only print the lower 32 bits of the parameters
		*/
		dv_printf("    param[%d] = %d (0x%08x)\n", i, (dv_u32_t)param[i], (dv_u32_t)param[i]);
	}

	for (;;) {} /* Temporary */
	return e;
}

/* callout_shutdown() - called on shutdown
*/
void callout_shutdown(dv_statustype_t e)
{
	dv_printf("callout_shutdown: %d\n", e);
}

/* callout_idle() - called in idle loop
*/
void callout_idle(void)
{
	dv_printf("Idle loop reached\n");
	for (;;) { }
}

/* callout_panic() - called from dv_panic
*/
#ifndef panic_info
#define panic_info()	do { } while (0)
#endif
void callout_panic(dv_panic_t p, dv_sid_t sid, char *fault)
{
	dv_printf("Panic %d in %d : %s\n", p, sid, fault);
	panic_info();
}

/* main() - well, it's main, innit?
*/
int main(int argc, char **argv)
{
	dv_printf("davroska starting ...\n");

	uart1_rx_rbm.length = UART1_RX_RBLEN;

	sysinfo();

	dv_startos(0);

	return 0;
}

