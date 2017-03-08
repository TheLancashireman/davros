/*	dv-kconfig.h - kernel configuration header file for davros
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
#ifndef dv_kconfig_h
#define dv_kconfig_h	1

#ifndef DV_ASM
#define DV_ASM	0
#endif

#include <kernel/h/dv-defs.h>

/* Include the configuration selections from the board.
 *
 * The included file MUST define
 *	- DV_FAMILY
 *	- DV_CPU
 *	- DV_COMPILER
*/
#include <dv-boardconfig.h>

#ifndef DV_FAMILY
#error "dv_boardconfig.h does not define DV_FAMILY. Please fix it."
#endif
#ifndef DV_CPU
#error "dv_boardconfig.h does not define DV_CPU. Please fix it."
#endif
#ifndef DV_COMPILER
#error "dv_boardconfig.h does not define DV_COMPILER. Please fix it."
#endif

#include <project/h/dv-projectconfig.h>

/* Default values for other kernel configuration options.
*/
#ifndef DV_COVERAGE
#define DV_COVERAGE		0
#endif

/* Assume single core unless told otherwise.
*/
#ifndef DV_NCORES
#define DV_NCORES		1
#endif

/* Assume no extra stack unless told otherwise.
*/
#ifndef DV_STACKEXTRA
#define DV_STACKEXTRA	0
#endif

#endif
