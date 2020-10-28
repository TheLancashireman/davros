# Davros-3 test set : blocking

This set of tests is designed to exercise the simple blocking system calls:
* dv_sleep()
* dv_sleep_until()
* dv_suspend() and dv_resume()

## blocking-1

Tests the error return values from the system calls. 

* dv_sleep() from a non-blocking executable
* dv_sleep_until() from a non-blocking executable
* dv_suspend() from a non-blocking executable
* dv_resume() index out of range
	* boundary conditions: -1 and no-of-executables
* dv_resume() for an executable that doesn't exist (not created)
	* boundary conditions: two ranges (before and after caller)
* dv_sleep() for too short a time
	* boundary conditions: DV_MIN_SLEEP-1
* dv_sleep_until() for too short a time
	* the exact boundary is difficult to hit because it depends on the execution time
	* boundary used: DV_MIN_SLEEP-1

## blocking-2

Tests that an executable can sleep for a specified time (+/- tolerance)

* An executable Foo (priority 1, blocking)
* Control task spawns Foo
* Foo sleeps twice:
	* Once using dv_sleep()
	* Once using dv_sleep_until()
	* Each time it measures the time slept, as accurately as possible
* The control task monitors the progress by means of a state variable
* After the end of the test, the control task prints the measured sleep times
