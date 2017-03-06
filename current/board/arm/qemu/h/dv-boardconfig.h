/*	dv-kernelconfig.h - kernel configuration header file for davros
 *
 *	This file defines the configuration of the kernel libraries
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
#ifndef dv_kernelconfig_h
#define dv_kernelconfig_h	1

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

#include <cpufamily/arm/h/dv-arm-kconfig.h>

#endif
