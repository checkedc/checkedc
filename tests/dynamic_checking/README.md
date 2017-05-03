# High-Level Dynamic Check Tests

These tests all compile and run programs, making sure that their 
dynamic behaviour includes exiting/stopping when dynamic checks fail.

## Similar Tests

There are 7 different groups of tests of bounds checking, with each group covering a
different kind of l-value-producing expression.  Each group has its own subdirectory:
- `deref-arith-check`.  This is for `*` applied to pointer arithmetic expressions.
- `deref-arrow-check`.  This is for `*` applied to member accesses using the `->` operator.
- `deref-check`.  This is for `*`  applied to variables.
- `deref-dot-check`.  This is for `*` applied to member accesses using the `.` operator.
- `subscript-arrow-check`.  This is for `[]` applied member accesses using the `->` operator.
- `subscript-check`.   This is for `[]` applied array variables.
- `subscript-dot-check`.  This is for `[]` applied member accesses using the `.` operator.

We still need to write tests for `.` where the base expression requires a bounds
check and `->`, where the `->` incorporates a bound check.

Within a subdirectory, each test is duplicated for the following operators applied to the
l-value expression:
- `increment.c` applies `++` (post-increment) to the l-value
- `read.c` reads the lvalue (aka does an "l-value conversion")
- `compound-assign.c` applies compound assignment (such as `+=`, `-=`) to the l-value
- `write.c` assigns into the l-value with `=`

Each of these tests has an unoptimized and optimized version.  The optimized version ends in `-opt.c'

This results in 56 total tests at the current count.  We chose not to use the C
preprocessor for this at this time.

*Note:* do not use the words "install", "setup", "update" in test names where the files
will be compiled and executed. Windows x86 has heuristics that decide executables with those
words in the name need elevated privileges to run because they might be an installer,
even if they don't actually install anything. Documentation is here:
https://msdn.microsoft.com/en-us/enus/library/aa905330.aspx

### Optimised versions

Each of these tests has an optimized counterpart, (the files with `-opt` as
a filename suffix). This runs the original test, but with `-O3`, and tests
against the same source file using FileCheck. This is so that we know high levels
of optimization don't delete our dynamic checks.

## Other Tests

Tests in the`dynamic_check` subdirectory test the `dynamic_check` builtin.
These are not duplicated in the same way as the tests above.
