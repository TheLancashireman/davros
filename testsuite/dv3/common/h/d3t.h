/*	d3t.h - header file for davros-3 test suite
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
#ifndef d3t_h
#define d3t_h	1

/* Stringification macros:
 *
 * d3t_str()	- convert token to string
 * d3t_xstr()	- convert expanded value of a macro to a string
*/
#define d3t_xstr(s)		d3t_str(s)
#define d3t_str(s)		#s

/* d3t_testcase_init() - test case initialization
 *
 *	- Called from the d3t init task just before termination.
 *	- Can create additional executables that the test case needs.
*/
extern void d3t_testcase_init(void);

/* d3t_controltask() - the test case control task
 *
 *	- Must be provided by the test case
 *	- Runs in an executable of the same name at priority zero.
 *	- Controls the activities of the test case
*/
extern void d3t_controltask(void);

/* d3t_abort() - prints a message and stops in an endless loop
 *
 *	- Can be called to abort a test sequence
*/
extern void d3t_abort(char *, int);

/* d3t_starttest() - start a new test
 *
 *	- Can be called to start an individual test of a test case.
 *	- Clears the test point sequence, sets the test name etc.
*/
extern void d3t_starttest(char *);

/* d3t_finishtest() - finish a test
 *
 *	- Can be called to finish an individual test of a test case.
*/
extern void d3t_finishtest(char *);

/* d3t_testpoint() - records that the test reached a given place
 *
 *	- Can be called from a test case to record a sequence of execution
*/
extern void d3t_testpoint(char);

/* d3t_alldone() - records the end of a set of tests.
 *
 *	- Called to notifiy the world that the set of tests has finised.
 *	- Stops in an endless loop.
*/
extern void d3t_alldone(char *);

/* d3t_coreindex() - returns the core that it is called on
 *
 * ToDo: implement properly for multi-core
*/
static inline int d3t_coreindex(void)
{
	return 0;
}

#endif
