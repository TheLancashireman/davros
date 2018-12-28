# Davroska : a tiny little operating system.

Any similarities with OSEK are purely intentional :-)

The contents of this sub-tree are unrelated to the main davros-3 tree.

## Status:

* tested on a raspberry pi zero - uart interrupts working
* activation and termination of basic tasks appears to work.
* chaining a task appears to work
* taking and dropping locks appears to work.
* setting an alarm appears to work
* advancing a counter appears to work

## Quick user's guide

### Basics

You really need access to the OSEK spec, but here's a summary.

You have a collection of tasks and interrupt service routines (ISRs) collectively
known as executables. Executable 0 is the idle loop.

Executables can call API functions to do stuff.

Executables can activate other tasks (dv_activatetask, dv_chaintask)

Executables run strictly according to priority. So if you activate a task with a higher priority
than the calling task it will run straight away, but if you activate a task with the same or lower
priority it gets queued for later.
Executables must voluntarily yield the CPU (dv_terminatetask, dv_chaintask) to allow other executables
with the same or lower priority to run.

Tasks can have "multiple activations", though the use case for this is obscure. It means that if
the task cannot run because of a higher-priority executable, its activations get queued.
Queued executables of the same priority run in FIFO order

An executable can raise its priority (dv_takelock) or lower it again (dv_droplock), but never
lower than the base priority of the executable.

Interrupts are hardware mechanisms that are handled by the OS. You can create a special kind
of executable called an ISR that is activated by an interrupt.

Counters are objects that consist of a 64-bit value and a list of alarms. Counters are assumed to be monotonically
increasing - a wrap-around is a serious error, but will never happen in reality. You can advance
a counter by calling dv_advancecounter. Advancing a counter might cause alarms in its list to expire.
A counter can measure the passage of time or the occurrence of other things like network packets etc.

Alarms are objects that can trigger an action when a counter reaches a predetermined value. Unlike
OSEK, alarms are not permanently associated with a counter. Instead, the association happens when you
activate the alarm (dv_setalarm_rel, dv_setalarm_abs). An activated alarm sits in the alarm list of the
counter on which you activated it. The list is ordered by ascending expiry time. When the counter reaches
the expiry time of an alarm in its list, it calls the function that you configured for the alarm.
This function can do several things: activate tasks, advance other counters, set events for extended
tasks (when available: see below). If the expiry function returns zero, the alarm becomes inactive.
If the expiry function returns non-zero, the value is used to increase the expiry time of the
alarm so that it occurs again at some time in the future.

You can use hardware timers to drive counters. The simplest case is a periodic interval timer. This kind of
timer triggers an interrupt at pre-programmed intervals, and typically needs little or no
software intervention. To use this kind of timer you create an ISR to handle the interrupt.
In the ISR you call dv_advancecounter() with an increment of 1. A counter driven in this way has
a fairly low resolution - typically a millisecond per tick.

A more complicated type of hardware timer has a compare register that triggers an interrupt when the
timer matches the compare register. Of course, you can use this mechanism to generate a regular interrupt
that advances a counter by 1. However, you can also calculate the number of hardware ticks that have
elapsed since the last time you advanced the counter and advance the counter by that many ticks.
When the counter update is finished you can read the expiry time of the first alarm in the counter's
list and use that to calculate a new value for the compare register. A counter driven in this way has
a high resolution - typically a few nanoseconds. It only generates interrupts when there's something to
do, but beware of spreading your alarms out too much because the interrupt overhead increases.
Also beware that the current value of the counter might be stale. Before setting alarms it may be
wise to advance the counter by the number of timer ticks that have elapsed since the last advance.

Extended tasks and events: not implemented yet.
Hooks: not implemented yet.
Anything else in the OSEK spec that isn't mentioned: not implemented yet.

### Configuration:

The actual configuration of tasks, ISRs, locks etc. is done at startup. The compile time configuration
simply sets limits on what you can configure.

You provide a header file (dv-config.h) that defines the following macros:

* DV_CFG_MAXEXE - the maximum number of executables (excluding the idle loop) that you can have
* DV_CFG_MAXPRIO - the maximum number of priorites that your executables will use
* DV_CFG_MAXLOCK - the maximum number of locks that you can have
* DV_CFG_MAXCOUNTER - the maximum number of counters that you can have
* DV_CFG_MAXALARM - the maximum number of alarms that you can have
* DV_CFG_NSLOT_EXTRA - the number of extra queue elements you need (to cover multiple activations)

### Booting davroska:

From main(): call dv_startos(0). Getting to main() involves setting up the CPU, caches, page tables
and everything else and that is your problem, not mine. Though you might find some help in the various
demos....

dv_startos() calls various callout functions that you supply:

* callout_addtasks() - create the tasks that you need.
  * this function calls dv_addtask() for every task in your application
* callout_addisrs() - create the isrs that you need.
  * this function calls dv_addisr() for every isr in your application
* callout_addlocks() - create the locks that you need.
  * this function calls dv_addlock() for each lock in your application
  * in addition, it calls dv_addlockuser for each executable that uses each lock
* callout_addcounters() - create the counters that you need.
  * this function calls dv_addcounter() for every counter in your application
* callout_addalarms() - create the alarms that you need.
  * this function calls dv_addalarm() for every alarm in your application
* callout_autostart
  * this function calls dv_activatetask() for every task that you want to run automatically after startup
  * it also calls dv_setalarm_rel() or dv_setalarm_abs() for each alarm that you wish run after startup
  * You can also configure hardware timers and enable their interrupt requests.

For advanced users: the parameter that you give to dv_startos() is passed to each of the above callout
functions. This way, you can select various operating modes of your application.

When dv_startos() is done, davroska will schedule the tasks and ISRs of your application.

### API reference

#### Creating objects
* dv_id_t dv_addtask(const char *name, void (*fn)(void), dv_prio_t prio, dv_qty_t maxact)
  * adds a task to the list of executables
    * name is the name of the task
    * fn is the address of the "main" function of the task (the one that's "called" when the task runs)
    * prio is the (base) priority of the task
    * maxact is the maximum number of concurrent activations
  * dv_addtask() returns the identifier for the task. (-1) indicates an error
* dv_id_t dv_addisr(const char *name, void (*fn)(void), dv_prio_t irqid, dv_prio_t prio)
  * adds an ISR to the list of executables
    * name is the name of the ISR
    * fn is the address of the "main" function of the ISR (the one that's "called" when the ISR runs)
    * irqid is the identity of an interrupt request source. See hardware specific docs for details.
    * prio is the priority of the ISR and must be higher than your tasks
  * dv_addisr() returns the identifier for the ISR. (-1) indicates an error
  * davroska calculates the interrupt levels automatically based on the ISR priorities.
* dv_id_t dv_addlock(const char *name, dv_qty_t maxtake)
  * adds a lock to the list of locks
    * name is the name of the lock
    * maxtake is the highest number of times the lock can be taken (without dropping) by the same executable
  * dv_addlock() returns the identifer for the lock. (-1) indicates an error
* void dv_addlockuser(dv_id_t l, dv_id_t e)
  * adds an executable (e) as a "user" of the lock (l).
    * all executables that are added to a lock will be prevented from running when an executable occupies the lock
    * the blocking is done by means of the "immediate priority ceiling protocol", so other executables might be blocked
* dv_id_t dv_addcounter(const char *name)
  * adds a counter to the list of counters
    * name is the name of the counter
  * dv_addcounter() returns the identifer for the counter. (-1) indicates an error
dv_id_t dv_addalarm(const char *name, dv_u32_t (*fn)(dv_id_t a))
  * adds an alarm to the list of alarms
    * name is the name of the alarm
    * fn is the expiry function that davroska calls whenever the alarm expires
      * fn returns the cycle counter (for cyclic alarms) or 0 (for single-shot alarms)

#### Runtime stuff
* dv_statustype_t dv_terminatetask(void)
  * terminates the calling task
  * allows other tasks of same or lower priority to run, in descending priority order
    * including further instances of caller
* dv_statustype_t dv_activatetask(dv_id_t task)
  * activates the indicated task
    * if the task has a higher priority that the caller's cuerrent priority, it runs immediately (preemption)
    * otherwise it waits in a queue until its turn comes around
* dv_statustype_t dv_chaintask(dv_id_t task)
  * terminates the caller and activates the indicated task
* dv_statustype_t dv_takelock(dv_id_t lock)
  * takes the indicated lock
  * raises the priority of the caller to the ceiling priority of the lock
    * never lowers the priority of the caller
* dv_statustype_t dv_droplock(dv_id_t lock)
  * drops the indicated lock
  * reduces the priority of the caller to the priority before taking the lock
  * allows other tasks of intermediate priority to run
* dv_statustype_t dv_setalarm_rel(dv_id_t c, dv_id_t a, dv_u32_t v)
  * activates an alarm to expire when the counter reaches a value of v ticks beyond its current value
* dv_statustype_t dv_setalarm_abs(dv_id_t c, dv_id_t a, dv_u64_t v)
  * activates an alarm to expire when the counter reaches the value v
  * if v is less than the current counter value the alarm will expire on the next advance.
* dv_u64_t dv_readtime(void)
  * returns the value of a 64-bit free-running timer provided by the hardware


#### Callouts provided by the application
* void callout_addtasks(dv_id_t mode)
  * called during dv_startos()
  * may call dv_addtask() to create tasks
    * one call per task
  * must not call any other API
* void callout_addisrs(dv_id_t mode)
  * called during dv_startos()
  * may call dv_addisr() to create isrs
    * one call per isr
  * must not call any other API
* void callout_addlocks(dv_id_t mode)
  * called during dv_startos()
  * may call dv_addlock() to create locks
    * one call per lock
  * should call dv_addlockuser() at least once for each lock created
    * one call per user per lock
    * can be "optimised" by adding just the highest-priority user
  * must not call any other API
* void callout_addcounters(dv_id_t mode)
  * called during dv_startos()
  * may call dv_addcounter() to create counters
    * one call per counter
  * must not call any other API
* void callout_addalarms(dv_id_t mode)
  * called during dv_startos()
  * may call dv_addalarm() to create alarms
    * one call per alarm
  * must not call any other API
* void callout_autostart(dv_id_t mode)
  * called during dv_startos()
  * may call dv_activatetask() to activate a task at startup
    * one call per task that should run after startup
    * subject to normal maxact limits
  * may call dv_setrelalarm()/dv_setabsalarm()
  * may intialise hardware timers and enable the interrupt sources for them
