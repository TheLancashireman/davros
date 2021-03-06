/*	dv-coverage.h - instrumentation for test coverage tracing
 *
 *	Copyright David Haworth
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
#ifndef dv_coverage_h
#define dv_coverage_h	1

#if !DV_ASM

#include <kernel/h/dv-kernel-types.h>

/* Macros to help tracing test coverage.
 *
 * How to use this file:
 *	1.	Include this header file into your C source file
 *	2.	After all includes, add a line
 *			DV_COVDEF(name);
 *		The name can be anything, but usually a base name of the file.
 *	3.	At the start of the function, after the variable declarations, add a line
 *			dv_fcov(n)
 *		where n is the index of the coverage point.
 *	4.	Replace evrey boolean condition C with dv_ccov(n1, n2, C)
 *		where n1 and n2 are the indexes of the false and true coverage points, respectively,
 *		and C is the original condition.
 *	5.	Create a file containing all the coverage structures. They are all called dv_coverage_<name>
 *		ToDo: The last step can be done automatically.
 *
 *	Example:
 *
 *	DV_COVDEF(foo);
 *	int foo(int p)
 *	{
 *		int result;
 *		dv_fcov(0);
 *		if ( dv_ccov(1, 2, (p >= 0)) && dv_ccov(3, 4, (p < N)) )
 *			result = 1;
 *		else
 *			result = -1;
 *
 *		return result;
 *	}
*/

#if DV_COVERAGE

#define DV_COVDEF(x) \
	extern dv_coverage_t dv_coverage_##x;	\
	static dv_coverage_t * const dv_covp = &dv_coverage_##x

#define dv_fcov(p)			dv_functioncovered(dv_covp, (p))
#define dv_ccov(pf,pt,c)	dv_conditioncovered(dv_covp, (pf), (pt), (c))

#else

/* Empty versions of the coverage macros if coverage tracing is turned off
*/
#define DV_COVDEF(x)	extern void dv_nothing(void)

#define dv_fcov(p)			do{}while(0)
#define dv_ccov(pt,pf,c)	(c)

#endif

struct dv_coverage_s
{
	char *functionname;
	int npoints;
	dv_u8_t *counters;
};

extern void dv_functioncovered(dv_coverage_t *, int);
extern dv_boolean_t dv_conditioncovered(dv_coverage_t *, int, int, dv_boolean_t);

#endif

#endif
