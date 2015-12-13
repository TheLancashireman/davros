/*	dv-activateexecutable.c - activate an instance of an executable object
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
#include <kernel/include/dv-kconfig.h>
#include <user/include/dv-basic-types.h>
#include <kernel/include/dv-kernel-types.h>
#include <kernel/include/dv-executable.h>
#include <kernel/include/dv-coverage.h>
#include <user/include/dv-error.h>

DV_COVDEF(activateexecutable);

/* dv_activateexecutable() - activate an instance of an executable
*/
dv_errorid_t dv_activateexecutable(dv_kernel_t *kvars, dv_executable_t *executable)
{
}

/* man-page-generation - to be defined
*/
