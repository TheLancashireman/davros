# Davros-3

An embedded OS - eventually with memory protection.

Forever under construction ;-)

## Concept

The application consists of a set of *executables*. An executable is a main function and a set of
configuration parameters that control its priority and what it can do when it runs. You can create
and destroy executables dynamically, but the main functions are always linked in to the binary
- though I guess it would be possible to do some kind of dynamic loading of code.

The scheduling algorithm is
[fixed-priority pre-emptive scheduling](https://en.wikipedia.org/wiki/Fixed-priority_pre-emptive_scheduling)
without time slicing. Tasks of equal priority run until they terminate or call a blocking API.

Davros uses a set of *threads* to manage the executables. All executables of the same priority
share a single thread. The active threads are held in a linked list ordered by priority. The
active executables in a thread are queued using a ringbuffer.

Once created, an executable remains present in the system until it is explicitly destroyed, unless it is
created with the DV_EXEFLAG_AUTODESTROY attribute. The persistence means that an executable can be
spawned as often as necessary, although there can only be one spawned instance at any time.

There is a *system timer*; a 64-bit counter driven by a hardware timer that starts nominally at zero
and never wraps around. Certainly not during the expected lifetime of any hardware that is supported.
The resolution of the timer depends on the hardware but is typically one microsecond or less.

The davros kernel is non-reentrant.

Executables normally run in a non-privileged mode. API calls that change the state of the kernel
must use a system call, although this is wrapped in a small function (usually assembly language) that
translates the C calling convention into the davros kernel binary interface. The small function might
in turn be wrapped in a larger C function.

API calls that require privileges also use a system call or some other mechanism of entering
the kernel.

There are some API calls that do not change the state of the kernel; these are implemented
as plain C functions.


## API

### System calls

* void dv_nullsc(dv_machineword_t, dv_machineword_t, dv_machineword_t, dv_machineword_t);
	Does nothing. Used for testing the interface when adding support for new hardware.
* void dv_exit(dv_machineword_t, dv_machineword_t);
	Terminates the caller. The two parameters can be returned to the parent executable under some circumstances.
* dv_errorid_t dv_spawn(dv_index_t);
	Activates an executable and places it in its thread, or in the job queue if the thread isn't idle.
* dv_dual_t dv_create_exe(const dv_execonfig_t *);
	Creates an executable using the configuration parameters provided; returns a a status code and
	the executable ID.
* dv_errorid_t dv_sleep(dv_u32_t);
	Blocks the executable for the given number of system timer ticks.
* dv_errorid_t dv_sleep_until(dv_u64_t);
	Blocks the executable until the system timer reaches the given value.
* dv_errorid_t dv_suspend(void);
	Suspends the executable. Another executable can call dv_resume() to allow execution to continue.
* dv_errorid_t dv_resume(dv_index_t);
	Takes an exectuable out of the suspended state.

### Plain function calls

* dv_index_t dv_get_exeid(void);
	Returns the ID of the calling executable.
* dv_u64_t dv_readtime()
	Returns the value of the system timer.

## Comparison with davroska and OSEK

* davros-3 does not support multiple activations of tasks
* davros-3 supports *basic* tasks; they are tasks that have neither DV_EXEFLAG_BLOCKING nor DV_EXEFLAG_EVENTS

## Known bugs and limitations

1. When a thread is freed after all the executables have been destroyed, the job queue associated with the
thread does not get freed. Instead, it remains associated with the thread. This has two effects:
	* You might need more ring buffers and buffer space than you might think.
	* A thread that gets recycled and has more executables associated with it than in its first incarnation
might cause occasional errors (job queue overflow) or a kernel panic if the overflow happens when awakening an
executable from the sleep state.
