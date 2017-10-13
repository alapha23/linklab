# Simple Library Interpositioning Example

Intercept calls to the `rand()` function to always return a fixed number.

* `dice.c`: simple program to request user for an integer and compare it with a randomly generated number.
* `Makefile`: compiles both the dice program and the shared library.
* `librand.c`: intercept calls to `rand()`
