/*	dv-boardconfig.h - realview-pbx-a9 (qemu) kernel configuration header file for davros
 *
 *	This file defines the configuration of the kernel libraries for the realview-pbx-a9 board emulation
 *	in qemu:
 *	- processor selection
 *	- compiler selection
 *	- instrumentation for test coverage
 *	- debug and trace macros
 *
 *	Copyright 2015 David Haworth
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
#ifndef dv_boardconfig_h
#define dv_boardconfig_h	1

#include <board/arm/qemu/h/dv-qemu-uart.h>

/* Select the family, CPU and compiler
*/
#define DV_FAMILY		DV_ARM
#define DV_CPU			DV_CORTEXA9
#define DV_COMPILER		DV_COMPILER_GNU

/* DV_COVERAGE
 *	0		- don't trace code coverage
 *	other	- trace code coverage
*/
#define DV_COVERAGE		0

/* The "kputc" function for this board.
*/
#define DV_KPUTC(c)	dv_uart0_putc(c)

/* Interrupt IDs for this board.
 * IDs 0-31 are defined by the GIC.
 * IDs 41 and 58 are "reserved".
*/
#define DV_IID_WDG			32		/* Watchdog timer */
#define DV_IID_SOFT			33		/* Software interrupt */
#define DV_IID_COMMRX		34		/* Debug communications receive interrupt. */
#define DV_IID_COMMTX		35		/* Debug communications transmit interrupt. */
#define DV_IID_TIMER_0_1	36		/* Timers */
#define DV_IID_TIMER_2_3	37		/* Timers */
#define DV_IID_GPIO0		38		/* GPIO controller */
#define DV_IID_GPIO1		39		/* GPIO controller */
#define DV_IID_GPIO2		40		/* GPIO controller (various board I/O signals) */
#define DV_IID_RTC			42		/* Real time clock */
#define DV_IID_SSP			43		/* Synchronous serial port */
#define DV_IID_UART0		44		/* UART0 */
#define DV_IID_UART1		45		/* UART1 */
#define DV_IID_UART2		46		/* UART2 */
#define DV_IID_UART3		47		/* UART3 */
#define DV_IID_SCI			48		/* Smart Card interface */
#define DV_IID_MCIa			49		/* Multimedia Card Interface interrupt a */
#define DV_IID_MCIb			50		/* Multimedia Card Interface interrupt b */
#define DV_IID_AACI			51		/* CODEC controller interrupt */
#define DV_IID_KMI0			52		/* Keyboard/Mouse Interface */
#define DV_IID_KMI1			53		/* Keyboard/Mouse Interface */
#define DV_IID_LCD			54		/* Character LCD display */
#define DV_IID_CLCD			55		/* CLCD display (from adapter board) */
#define DV_IID_DMAC			56		/* DMA controller */
#define DV_IID_PWRFAIL		57		/* Power failure signal from FPGA */
#define DV_IID_PISMO		59		/* Memory expansion board */
#define DV_IID_ETHER		60		/* Interrupt from Ethernet controller IC */
#define DV_IID_USB			61		/* Interrupt from USB controller IC */
#define DV_IID_TSnPENIRQ	62		/* Touch screen keypad interrupt */
#define DV_IID_TSnKPADIRQ	63		/* Touch screen pen interrupt */
#define DV_IID_T1_INT0		64		/* Interrupts from tile site 1 */
#define DV_IID_T1_INT1		65
#define DV_IID_T1_INT2		66
#define DV_IID_T1_INT3		67
#define DV_IID_T1_INT4		68
#define DV_IID_T1_INT5		69
#define DV_IID_T1_INT6		70
#define DV_IID_T1_INT7		71
#define DV_IID_T2_INT0		72		/* Interrupts from tile site 2 */
#define DV_IID_T2_INT1		73
#define DV_IID_T2_INT2		74
#define DV_IID_T2_INT3		75
#define DV_IID_T2_INT4		76
#define DV_IID_T2_INT5		77
#define DV_IID_T2_INT6		78
#define DV_IID_T2_INT7		79
#define DV_IID_PCI0			80		/* Interrupts from PCI expansion bus */
#define DV_IID_PCI1			81
#define DV_IID_PCI2			82
#define DV_IID_PCI3			83

#define DV_N_IID			84
#define DV_N_SOFTVECTOR		100		/* Allows some interrupts to be demultiplexed. */

#include <cpufamily/arm/h/dv-arm-kconfig.h>

#endif
