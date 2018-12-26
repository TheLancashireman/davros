/* os.h - OSEK compatibility header for davroska
 *
 * (c) David Haworth
*/
#ifndef os_h
#define os_h	1

/* DON'T USE THIS! */

/* StatusType has to be defined in conformance with the OSEK Binding Specification.
*/
#ifndef STATUSTYPEDEFINED
#define STATUSTYPEDEFINED   1
typedef unsigned char StatusType;
#endif

/* OSEK-like data types
*/
typedef dv_id_t TaskType;

/* OSEK-like "services"
*/
static inline StatusType ActivateTask(TaskType t)
{
	return (StatusType)dv_activatetask(t);
}

static inline StatusType TerminateTask(void)
{
	return (StatusType)dv_terminatetask();
}

#endif
