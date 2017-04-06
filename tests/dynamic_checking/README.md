# High-Level Dynamic Check Tests

These tests all compile and run programs, making sure that their 
dynamic behaviour includes exiting/stopping when dynamic checks fail.

## Similar Tests

There are 3 (6 including their optimised verisons) different duplicated tests, each
for a different kind of l-value.
- `deref-arith-check.c` and `deref-arith-check-opt.c`
- `deref-check.c` and `deref-check-opt.c`
- `subscript-check.c` and `subscript-check-opt.c`

Each of these is duplicated for the following operators applied to these kinds of
l-values:
- `increment` applies `++` (post-increment) to the l-value
- `read` reads the lvalue (aka does an "l-value conversion")
- `compound-assign` applies compound assignment (such as `+=`, `-=`) to the l-value
- `write` assigns into the l-value with `=`

This results in 24 total tests at the current count. As we add more l-value kinds
this value will increase. We chose not to use the C preprocessor for this at this
time.

*Note:* do not use the words "install", "setup", "update" in test names where the files
will be compiled and executed. Windows x86 has heuristics that decide executables with those
words in the name need elevated privileges to run because they might be an installer,
even if they don't actually install anything. Documentation is here:
https://msdn.microsoft.com/en-us/enus/library/aa905330.aspx

### Optimised versions

Each of these 3 tests has an optimized counterpart, (the files with `-opt` as
a filename suffix). This runs the original test, but with `-O3`, and tests
against the same source file using FileCheck. This is so that we know high levels
of optimization don't delete our dynamic checks.

## Other Tests

Tests with the `dynamic_check-` filename prefix test the `dynamic_check` builtin,
these are not duplicated in the same way as the tests above.
