# Davros-3 test set : basics

This set of tests is designed to exercise the basic thread/executable functionality.

## basics-1

Tests the error return values from the dv_spawn() system call.

* Index out of range
** boundary conditions: -1 and no-of-executables
* Executable not created
** boundary conditions: two ranges (before and after caller)
* Executable already spawned
** specific executables (idle and test control task); no boundary conditions

## basics-2

Tests that an executable can be pre-empted by a higher priority executable

* Two executables: Foo (priority 1) and Bar (priority 2)
* Control task spawns Foo
* Foo spawns Bar
* Expectations: Bar runs immediately on spawning, Foo continues when Bar exits

## basics-3

Tests that an executable cannot be pre-empted by a same priority executable

* Two executables: Foo (priority 1) and Bar (priority 1)
* Control task spawns Foo
* Foo spawns Bar
* Expectations: Bar runs after Foo exits

