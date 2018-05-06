/*	dv-boardconfig.h - bcm2835 kernel configuration header file for davros
 *
 *	This file defines the configuration of the kernel libraries for original raspberry pi boards
 *	- processor selection
 *	- compiler selection
 *	- instrumentation for test coverage
 *	- debug and trace macros
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
#ifndef dv_boardconfig_h
#define dv_boardconfig_h	1

/* Select the family, CPU and compiler
*/
#define DV_FAMILY		DV_ARM
#define DV_CPU			DV_ARM1176
#define DV_COMPILER		DV_COMPILER_GNU

/* DV_COVERAGE
 *	0		- don't trace code coverage
 *	other	- trace code coverage
*/
#define DV_COVERAGE		0

/* See dv-arm-bcm2835-interruptcontroller.c for the list of supported interrupt sources.
 * ToDo: make the table configurable.
 *
 * On a single-core CPU there are no banked interrupts, so the number of vectors
 * is the same as the number of interrupts.
*/
#define DV_N_IID			3
#define DV_N_SOFTVECTOR		DV_N_IID

#include <cpufamily/arm/h/dv-arm-kconfig.h>

/* Header files for BCM2835
*/
#define DV_H_SYSTEMTIMER			<devices/h/dv-arm-bcm2835-timer.h>
#define DV_H_INTERRUPTCONTROLLER	<devices/h/dv-arm-bcm2835-interruptcontroller.h>
#define DV_H_CONSOLEUART			<devices/h/dv-arm-bcm2835-uart.h>

#endif
