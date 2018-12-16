Davroska : a tiny little operating system.

Any similarities with OSEK are purely intentional :-)

The contents of this sub-tree are unrelated to the main davros-3 tree.

Status:

* tested on a linux host - no timing, no interrupts
* activation and termination of basic tasks appears to work.
* chaining a task appears to work
* taking and dropping locks seems to work.

Quick user's guide

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

Configuration:

The actual configuration of tasks, ISRs, locks etc. is done at startup. The compile time configuration
simple sets limits on what you can configure.

You provide a header file (dv-config.h) that defines the following macros:

* DV_CFG_MAXEXE - the maximum number of executables (excluding the idle loop) that you can have
* DV_CFG_MAXPRIO - the maximum number of priorites that your executables will use
* DV_CFG_MAXLOCK - the maximum number of locks that you can have
* DV_CFG_NSLOT_EXTRA - the number of extra queue elements you need (to cover multiple activations)


Booting davroska:

From main(): call dv_startos(0). Getting to main() involves setting up the CPU, caches, page tables
and everyhting else and is your problem, not mine. Though you might find some help in the various
demos....

dv_startos() calls various callout functions that you supply:

* callout_addtasks() - create the tasks that you need.
  ** this function calls dv_addtask() for every task in your application
* callout_addlocks() - create the locks that you need.
  ** this function calls dv_addlock() for each lock in your application
  ** in addition, it calls dv_addlockuser for each executable that uses each lock
* callout_autostart
  ** this function calls dv_activatetask() for every task that you want to run automatically after startup
  ** (not implemented yet) automatic alarm activation etc.

For advanced users: the parameter that you give to dv_startos() is passed to each of the above callout
functions. This way, you can select various operating modes of your application.

When dv_startos() is done, davroska will schedule the tasks and ISRs of your application.
  


