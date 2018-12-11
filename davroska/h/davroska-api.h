/* davroska-api.h - public header file for OSEK-like member of the davros family
 *
 * (c) David Haworth
*/
#ifndef dv_davroska_api_h
#define dv_davroska_api_h	1

#include <h/davroska.h>


/* StatusType has to be defined in conformance with the OSEK Binding Specification.
*/
#ifndef STATUSTYPEDEFINED
#define STATUSTYPEDEFINED   1
typedef unsigned char StatusType;
#endif

/* OSEK-like data types
*/
typedef dv_taskid_t TaskType;

/* OSEK-like "services"
*/
StatusType ActivateTask(TaskType t);
StatusType TerminateTask(void);
