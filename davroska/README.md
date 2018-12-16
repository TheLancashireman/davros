# Davroska : a tiny little operating system.

Any similarities with OSEK are purely intentional :-)

The contents of this sub-tree are unrelated to the main davros-3 tree.

## Status:

* tested on a linux host - no timing, no interrupts
* activation and termination of basic tasks appears to work.
* chaining a task appears to work
* taking and dropping locks seems to work.

## Quick user's guide

### Basics

You really need access to the OSEK spec, but here's a summary.

You've got a collection of tasks and interrupt service routines (ISRs) collectively
known as executables. Executable 0 is the idle loop.

Executables can call API functions to do stuff.

Executables can activate other tasks (dv_activatetask, dv_chaintask)

Executables run strictly according to priority. So if you activate a task with a higher priority
than the calling task it will run straight away, but if you activate a task with the same or lower
priority it gets queued for later.
Executables must voluntarily yield the CPU (dv_terminatetask, dv_chaintask) to allow other executables
with the same or lower priority to run.

Tasks can have "multiple activations", though the use case for this is obscure. It means that if
the task can't run because of a higher-priority executable, its activations get queued.
Queued executables of the same priority run in FIFO order

An executable can raise its priority (dv_takelock) or lower it again (dv_droplock), but never
lower than the executable's base priority.

Interrupts: not implemented yet.
Counters and alarms: not implemented yet.
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
* DV_CFG_NSLOT_EXTRA - the number of extra queue elements you need (to cover multiple activations)

### Booting davroska:

From main(): call dv_startos(0). Getting to main() involves setting up the CPU, caches, page tables
and everyhting else and is your problem, not mine. Though you might find some help in the various
demos....

dv_startos() calls various callout functions that you supply:

* callout_addtasks() - create the tasks that you need.
  * this function calls dv_addtask() for every task in your application
* callout_addlocks() - create the locks that you need.
  * this function calls dv_addlock() for each lock in your application
  * in addition, it calls dv_addlockuser for each executable that uses each lock
* callout_autostart
  * this function calls dv_activatetask() for every task that you want to run automatically after startup
  * (not implemented yet) automatic alarm activation etc.

For advanced users: the parameter that you give to dv_startos() is passed to each of the above callout
functions. This way, you can select various operating modes of your application.

When dv_startos() is done, davroska will schedule the tasks and ISRs of your application.

### API reference

#### Creating objects
* dv_id_t dv_addtask(const char *name, void (*fn)(void), dv_prio_t prio, dv_qty_t maxact)
  * adds a task to the list of executables
    * name is the name of the task
    * fn is the address of the "main" function of the task (the one that's "called" when the task runs
    * prio is the (base) priority of the task
    * maxact is the maximum number of concurrent activations
  * dv_addtask() returns the identifier for the task. (-1) indicates an error
* dv_id_t dv_addlock(const char *name, dv_qty_t maxtake)
  * adds a lock to the list of locks
    * name is the name of the lock
    * maxtake is the highest number of times the lock can be taken (without dropping) by the same executable
  * dv_addlock() returns the identifer for the lock. (-1) indicates an error
* void dv_addlockuser(dv_id_t l, dv_id_t e)
  * adds an executable (e) as a "user" of the lock (l).
    * all executables that are added to a lock will be prevented from running when an executable occupies the lock
    * the blocking is done by means of the "immediate priority ceiling protocol", so other executables might be blocked

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

#### Callouts provided by the application
* void callout_addtasks(dv_id_t mode)
  * called during dv_startos()
  * may call dv_addtask() to create tasks
    * one call per task
  * must not call any other API
* void callout_addlocks(dv_id_t mode)
  * called during dv_startos()
  * may call dv_addlock() to create locks
    * one call per lock
  * should call dv_addlockuser() at least once for each lock created
    * one call per user per lock
    * can be "optimised" by adding just the highest-priority user
  * must not call any other API
* void callout_autostart(dv_id_t mode)
  * called during dv_startos()
  * may call dv_activatetask() to activate a task at startup
    * one call per task that should run after startup
    * subject to normal maxact limits
 * may call dv_setrelalarm()/dv_setabsalarm()/... (not implemented yet)
