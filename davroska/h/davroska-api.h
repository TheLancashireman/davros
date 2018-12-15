/* davroska-api.h - public header file for OSEK-like member of the davros family
 *
 * (c) David Haworth
*/
#ifndef dv_davroska_api_h
#define dv_davroska_api_h	1

typedef short dv_id_t;
typedef unsigned short dv_prio_t;
typedef short dv_qty_t;

typedef enum
{	dv_suspended,
	dv_ready,
	dv_running,
	dv_waiting
} dv_tstate_t;

typedef enum
{	dv_e_ok,
	dv_e_access,
	dv_e_calllevel,
	dv_e_id,
	dv_e_limit,
	dv_e_nofunc,
	dv_e_resource,
	dv_e_state,
	dv_e_value,
	dv_e_longjmp_ok		/* Must be last */
} dv_statustype_t;

typedef enum
{
	dv_sid_terminatetask,
	dv_sid_activatetask,
	dv_sid_chaintask
} dv_sid_t;

dv_statustype_t dv_startos(dv_id_t mode);

dv_id_t dv_addtask(const char *name, void (*fn)(void), dv_prio_t prio, dv_qty_t maxact);

dv_statustype_t dv_terminatetask(void);
dv_statustype_t dv_activatetask(dv_id_t task);
dv_statustype_t dv_chaintask(dv_id_t task);

/* Callout functions: provided by the application
*/
void callout_addtasks(dv_id_t mode);
void callout_autostart(dv_id_t mode);


#endif
