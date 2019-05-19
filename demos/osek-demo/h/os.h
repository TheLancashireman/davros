/* os.h -  "generated" OS header file
 *
 * (c) David Haworth
*/
#ifndef os_h
#define os_h
#include <dv-config.h>
#include <davroska-osek.h>

/* Object identifiers
*/
extern dv_id_t Uart, Timer;		/* ISRs */
extern dv_id_t Ticker;			/* Davroska counters */
extern dv_id_t OsekCounter;		/* OSEK counters */
extern const EventMaskType ev_Flicker;
extern const EventMaskType ev_Update;

extern TASK(Init);
extern TASK(Led);
extern TASK(Bit0);
extern TASK(Bit1);
extern TASK(Bit2);
extern TASK(Bit3);
extern ISR(Uart);
extern ISR(Timer);

#endif
