/*	dv-error.h - error codes for davros
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
#ifndef dv_error_h
#define dv_error_h	1

#if !DV_ASM
/* API error handling.
*/
enum dv_errorid_e
{
	dv_eid_UnknownError = 0x7fffffff,	/* Used as default; should never occur in practice */
	dv_eid_None = 0,					/* No error - all ok */
	dv_eid_TryAgain,					/* Try the operation again - not an error */

	dv_eid_IndexOutOfRange,
	dv_eid_UnconfiguredExecutable,
	dv_eid_ExecutableQuarantined,
	dv_eid_MaxInstancesExceeded,
	dv_eid_InvalidPointerParameter,
	dv_eid_ExecutableCreationFailed,
	dv_eid_ExecutableIsNonBlocking,

	dv_nerrors							/* Not used as an error code */
};

typedef enum dv_errorid_e dv_errorid_t;

/* Panic handling.
*/
enum dv_panic_e
{
	dv_panic_none = 0,
	dv_panic_unimplemented,
	dv_panic_hardwareerror,
	dv_panic_unknownqueuetype,
	dv_panic_threadqueueempty,
	dv_panic_currentthreadnotqueuehead,
	dv_panic_objectsearchfailed,
	dv_panic_last
};

typedef enum dv_panic_e dv_panic_t;

void dv_panic(dv_panic_t, char *, char *) __attribute__((noreturn));

#endif

#endif
