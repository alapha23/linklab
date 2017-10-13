# Linklab: Memtrace

This lab consists of three parts and a bonus question. Change into one
of the subdirectories (part1, part2, part3, and bonus) and run `make`.

See the lab's handout [here](linklab.pdf) for a detail explanation.

**Deadline: Mon. Oct. 30 2017 14:00**

## Repository Structure

* `part1-3`:Source files for part 1 to 3.
* `utils`: utility function and definitions for book keeping.
* `tests`: set of test files
* `bonus`: bonus part.

## Utilities

### [`memlist`](utils)

Defines and implements a link list of `item`s. This link list is used to keep
track information regarding allocated memory blocks.

### [`memlog`](utils)

Set of macro definitions to easily use log different events. This allows us
to use the same format accross different implementations. Since, this lab will
be graded using a script you should make use of this macros in order to produce
any printable output instead of using `printf` directly.

> The function `memlog` is used on all the macros. However, you should
**not** use this function directly.
