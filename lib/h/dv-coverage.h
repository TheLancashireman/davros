/*	dv-coverage.h - code coverage measurement
 *
 *	Copyright David Haworth
 *
 *	This file is free software; you can redistribute it and/or modify
 *	it under the terms of the GNU General Public License as published by
 *	the Free Software Foundation; either version 2, or (at your option)
 *	any later version.
 *
 *	It is distributed in the hope that it will be useful,
 *	but WITHOUT ANY WARRANTY; without even the implied warranty of
 *	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *	GNU General Public License for more details.
 *
 *	You should have received a copy of the GNU General Public License
 *	along with this program; see the file COPYING.  If not, write to
 *	the Free Software Foundation, 59 Temple Place - Suite 330,
 *	Boston, MA 02111-1307, USA.
 *
 *
 *	This file contains the "stdio" functions
 *
*/
#ifndef dv_coverage_h
#define dv_coverage_h

#ifndef DV_COVERAGE
#define DV_COVERAGE	0
#endif

#if !DV_ASM

#if DV_COVERAGE

#include <dv-coverage-gen.h>

/* Coverage enabled:
 *	dv_sc() expands to dv_scov_f()
 *	dv_cc() expands to dv_ccov_f()
*/
#define dv_sc(n)		dv_scov_f(DV_COV_BASE+n)
#define dv_cc(c, n)		dv_ccov_f((c), (DV_COV_BASE+n))

#else

/* Coverage disabled:
 *	dv_sc() expands to an empty statement
 *	dv_cc() expands to just the condition
*/
#define dv_sc(n)		do {} while (0)
#define dv_cc(c, t, f)	(c)

#endif

#if DV_COVERAGE

/* dv_cov_f() - abstract coverage function; sets a bit in the coverage array
*/
static inline void dv_cov_f(unsigned n)
{
	dv_coverage[n>>5] |= (1 << (n & 31));
}

/* dv_scov_f() - statement/branch coverage
*/
static inline void dv_scov_f(unsigned n)
{
	dv_cov_f(n);
}

/* dv_ccov_f() - condition coverage
*/
static inline dv_boolean_t dv_ccov_f(dv_boolean_t c, unsigned t)
{
	if ( c )
		dv_cov_f(t);
	else
		dv_cov_f(t+1);
	return c;
}

#endif

#endif

#endif
