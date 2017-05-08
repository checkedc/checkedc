# High-Level Dynamic Check Tests

These tests all compile and run programs, making sure that their 
dynamic behaviour includes exiting/stopping when dynamic checks fail.

## Bounds-checking Tests

There are 5 different tests of bounds checking, with each test covering a
different kind of l-value-producing expression.  The tests are in the
bounds subdirectory:

- `deref`.  This is for `*`  applied to variables.
- `deref-arith`.  This is for `*` applied to pointer arithmetic expressions.
- `deref-member-expr`.  This is for `*` applied to member accesses using
 the `.` or `->` operators.
- `subscript`   This is for `[]` applied to array variables.
- `subscript-member-expr`.  This is for `[]` applied to member accesses using
`.` or the `->` operator.

We still need to write tests for `.` where the base expression requires a bounds
check and `->`, where the `->` incorporates a bound check.

Each test is compiled 4 different ways with the following operators applied to
different versions of the l-value-producing expression.
Macro names are used to control the operator used in one
instance of the test.  The macro names are:

- `TEST_READ` reads the lvalue (aka does an "l-value conversion")
- `TEST_WRITE` assigns into the l-value with `=`
- `TEST_INCREMENT` applies `++` (post-increment) to the l-value
- `TEST_COMPOUND_ASSIGN` applies compound assignment (such as `+=`, `-=`) to the l-value
Each of these tests has an unoptimized and optimized version.  The optimized version ends in `-opt.c'

The test file for `subscript` is re-used for `deref-arith`. The file
`deref-arith` only contains test suite driver commands.

This results in 56 total test configurations at the current count. There are 8 test 
configurations each for `deref`, `deref-artih`, and `subscript`: 4 operators
using the lvalue expression x 2 optimization configurations.  There are 16 test 
configurations  for `deref-member-expr` and `subscript-member-expr`: 4 operators
x 2 optimization configurations x 2 kinds of member expression operators.

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

## `Dynamic_check` Tests

Tests in the `dynamic_check` subdirectory test the `dynamic_check` builtin.
These are not duplicated in the same way as the tests above.
