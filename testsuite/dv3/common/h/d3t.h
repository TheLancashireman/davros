/*	dt3.h - header file for davros-3 test suite
 *
 *	Copyright 2020 David Haworth
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
#ifndef dt3_h
#define dt3_h	1

/* Stringification macros:
 *
 * dt3_str()	- convert token to string
 * dt3_xstr()	- convert expanded value of a macro to a string
*/
#define dt3_xstr(s)		dt3_str(s)
#define dt3_str(s)		#s

/* dt3_testcase_init() - test case initialization
 *
 *	- Called from the dt3 init task just before termination.
 *	- Can create additional executables that the test case needs.
*/
extern void dt3_testcase_init(void);

/* dt3_controltask() - the test case control task
 *
 *	- Must be provided by the test case
 *	- Runs in an executable of the same name at priority zero.
 *	- Controls the activities of the test case
*/
extern void dt3_controltask(void);

/* dt3_abort() - prints a message and stops in an endless loop
 *
 *	- Can be called to abort a test sequence
*/
extern void dt3_abort(char *, int);

/* dt3_sequencepoint() - records a sequence point
 *
 *	- Can be called from a test case to record a sequence of execution
*/
extern void dt3_sequencepoint(char);

/* dt3_coreindex() - returns the core that it is called on
 *
 * ToDo: implement properly for multi-core
*/
static inline int dt3_coreindex(void)
{
	return 0;
}

#endif
