# Sample Checked C code

This directory contains some sample Checked C code:
- [hello-world.c](hello-world.c): The canonical "hello, world" program, using Checked C.  Show how to declare bounds for the array of arguments.
- [echo-args.c](echo-args.c): Echo arguments to the output console.  Use the array of arguments with bounds.
- [echo-args-buggy.c](echo-args-buggy.c): A buggy version of [echo-args.c](echo-args.c) with an off-by-one error that tries to read past the end of the array of arguments.
- [find-pattern.c](find-pattern.c): The "find pattern" program from Kernighan and Richie, rewritten to use Checked C.  This program illustrates some subtle points about using strings in Checked C.
- [string-helper.c](string-helpers.c): A short tutorial on using strings in Checked C, with Checked C versions of `atoi`, `squeeze`, `strcmp`, `strlen`, and `reverse`.
+ [avoid-warning.c](avoid-warning.c): This example shows how to use `dynamic_bounds_cast` to avoid a compile-time warning that the compiler cannot prove declared bounds are valid.
- [spec](spec): Examples drawn from the Checked C specification.


