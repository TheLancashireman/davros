# Davros-3 test suite

A test suite that is designed to test the davros-3 kernel.

The intention is to achieve C/DC coverage of the code. In practical terms it means that
whenever a condition is evaluated, then there is a test case that causes the condition
to evaluate to TRUE and a test case thast causes the condition to evaluate to FALSE.

## Subdirectories

### common

Contains a common test framework for all the tests, including a build environment.

### d3t-template

Contains a template test set with no tests.

Serves as an example of how to construct test sets using the framework.

### basics

Contains tests for the basic functionality of creating and spawning executables.

