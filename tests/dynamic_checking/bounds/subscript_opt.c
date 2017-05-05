// Tests that the Array Subscript Operator works with dynamic checks
//
// The following lines are for the clang automated test suite
//
// This builds the file `subscript.c` at optimisation level O3, outputting an
// executable at %t. We then run this with the same arguments as we run the unoptimised
// binary with, and use FileCheck to ensure the output is what we expected, as specified
// in `subscript.c`. This is run as a seperate test so we know if optimisation is
// breaking our dynamic checks.
//

// The following lines are cut and passed
// RUN: %clang -fcheckedc-extension %S/subscript.c -DTEST_READ -o %t1 -Werror -O3
// RUN: %t1 0 0 0 0  0 0   0 0 0  | FileCheck %S/subscript.c
// RUN: %t1 1 2 4 4  1 2   1 1 1  | FileCheck %S/subscript.c
// RUN: %t1 2 4 8 8  2 1   2 2 2  | FileCheck %S/subscript.c
// RUN: %t1 2 4 8 8  0 4   0 3 2  | FileCheck %S/subscript.c
// RUN: %t1 2 4 8 8  1 3   0 1 5  | FileCheck %S/subscript.c
// RUN: %t1 2 4 8 8  2 -1  2 -1 2 | FileCheck %S/subscript.c
// RUN: %t1 3        | FileCheck %S/subscript.c --check-prefix=CHECK-FAIL-1
// RUN: %t1 -1       | FileCheck %S/subscript.c --check-prefix=CHECK-FAIL-1
// RUN: %t1 0 5      | FileCheck %S/subscript.c --check-prefix=CHECK-FAIL-1
// RUN: %t1 0 -1     | FileCheck %S/subscript.c --check-prefix=CHECK-FAIL-1
// RUN: %t1 0 0 9    | FileCheck %S/subscript.c --check-prefix=CHECK-FAIL-1
// RUN: %t1 0 0 -1   | FileCheck %S/subscript.c --check-prefix=CHECK-FAIL-1
// RUN: %t1 0 0 0 9  | FileCheck %S/subscript.c --check-prefix=CHECK-FAIL-1
// RUN: %t1 0 0 0 -1 | FileCheck %S/subscript.c --check-prefix=CHECK-FAIL-1
// RUN: %t1 0 0 0 0  3 0   | FileCheck %S/subscript.c --check-prefix=CHECK-FAIL-2
// RUN: %t1 0 0 0 0  2 3   | FileCheck %S/subscript.c --check-prefix=CHECK-FAIL-2
// RUN: %t1 0 0 0 0  0 9   | FileCheck %S/subscript.c --check-prefix=CHECK-FAIL-2
// RUN: %t1 0 0 0 0  -1 -1 | FileCheck %S/subscript.c --check-prefix=CHECK-FAIL-2
// RUN: %t1 0 0 0 0  0 0  3 0 0    | FileCheck %S/subscript.c --check-prefix=CHECK-FAIL-3
// RUN: %t1 0 0 0 0  0 0  2 9 0    | FileCheck %S/subscript.c --check-prefix=CHECK-FAIL-3
// RUN: %t1 0 0 0 0  0 0  2 2 3    | FileCheck %S/subscript.c --check-prefix=CHECK-FAIL-3
// RUN: %t1 0 0 0 0  0 0  0 0 27   | FileCheck %S/subscript.c --check-prefix=CHECK-FAIL-3
// RUN: %t1 0 0 0 0  0 0  -1 -1 -1 | FileCheck %S/subscript.c --check-prefix=CHECK-FAIL-3
//
// RUN: %clang -fcheckedc-extension %S/subscript.c -DTEST_WRITE -o %t2 -Werror -O3
// RUN: %t2 0 0 0 0  0 0   0 0 0  | FileCheck %S/subscript.c
// RUN: %t2 1 2 4 4  1 2   1 1 1  | FileCheck %S/subscript.c
// RUN: %t2 2 4 8 8  2 1   2 2 2  | FileCheck %S/subscript.c
// RUN: %t2 2 4 8 8  0 4   0 3 2  | FileCheck %S/subscript.c
// RUN: %t2 2 4 8 8  1 3   0 1 5  | FileCheck %S/subscript.c
// RUN: %t2 2 4 8 8  2 -1  2 -1 2 | FileCheck %S/subscript.c
// RUN: %t2 3        | FileCheck %S/subscript.c --check-prefix=CHECK-FAIL-1
// RUN: %t2 -1       | FileCheck %S/subscript.c --check-prefix=CHECK-FAIL-1
// RUN: %t2 0 5      | FileCheck %S/subscript.c --check-prefix=CHECK-FAIL-1
// RUN: %t2 0 -1     | FileCheck %S/subscript.c --check-prefix=CHECK-FAIL-1
// RUN: %t2 0 0 9    | FileCheck %S/subscript.c --check-prefix=CHECK-FAIL-1
// RUN: %t2 0 0 -1   | FileCheck %S/subscript.c --check-prefix=CHECK-FAIL-1
// RUN: %t2 0 0 0 9  | FileCheck %S/subscript.c --check-prefix=CHECK-FAIL-1
// RUN: %t2 0 0 0 -1 | FileCheck %S/subscript.c --check-prefix=CHECK-FAIL-1
// RUN: %t2 0 0 0 0  3 0   | FileCheck %S/subscript.c --check-prefix=CHECK-FAIL-2
// RUN: %t2 0 0 0 0  2 3   | FileCheck %S/subscript.c --check-prefix=CHECK-FAIL-2
// RUN: %t2 0 0 0 0  0 9   | FileCheck %S/subscript.c --check-prefix=CHECK-FAIL-2
// RUN: %t2 0 0 0 0  -1 -1 | FileCheck %S/subscript.c --check-prefix=CHECK-FAIL-2
// RUN: %t2 0 0 0 0  0 0  3 0 0    | FileCheck %S/subscript.c --check-prefix=CHECK-FAIL-3
// RUN: %t2 0 0 0 0  0 0  2 9 0    | FileCheck %S/subscript.c --check-prefix=CHECK-FAIL-3
// RUN: %t2 0 0 0 0  0 0  2 2 3    | FileCheck %S/subscript.c --check-prefix=CHECK-FAIL-3
// RUN: %t2 0 0 0 0  0 0  0 0 27   | FileCheck %S/subscript.c --check-prefix=CHECK-FAIL-3
// RUN: %t2 0 0 0 0  0 0  -1 -1 -1 | FileCheck %S/subscript.c --check-prefix=CHECK-FAIL-3
//
// RUN: %clang -fcheckedc-extension %S/subscript.c -DTEST_INCREMENT -o %t3 -Werror -O3
// RUN: %t3 0 0 0 0  0 0   0 0 0  | FileCheck %S/subscript.c
// RUN: %t3 1 2 4 4  1 2   1 1 1  | FileCheck %S/subscript.c
// RUN: %t3 2 4 8 8  2 1   2 2 2  | FileCheck %S/subscript.c
// RUN: %t3 2 4 8 8  0 4   0 3 2  | FileCheck %S/subscript.c
// RUN: %t3 2 4 8 8  1 3   0 1 5  | FileCheck %S/subscript.c
// RUN: %t3 2 4 8 8  2 -1  2 -1 2 | FileCheck %S/subscript.c
// RUN: %t3 3        | FileCheck %S/subscript.c --check-prefix=CHECK-FAIL-1
// RUN: %t3 -1       | FileCheck %S/subscript.c --check-prefix=CHECK-FAIL-1
// RUN: %t3 0 5      | FileCheck %S/subscript.c --check-prefix=CHECK-FAIL-1
// RUN: %t3 0 -1     | FileCheck %S/subscript.c --check-prefix=CHECK-FAIL-1
// RUN: %t3 0 0 9    | FileCheck %S/subscript.c --check-prefix=CHECK-FAIL-1
// RUN: %t3 0 0 -1   | FileCheck %S/subscript.c --check-prefix=CHECK-FAIL-1
// RUN: %t3 0 0 0 9  | FileCheck %S/subscript.c --check-prefix=CHECK-FAIL-1
// RUN: %t3 0 0 0 -1 | FileCheck %S/subscript.c --check-prefix=CHECK-FAIL-1
// RUN: %t3 0 0 0 0  3 0   | FileCheck %S/subscript.c --check-prefix=CHECK-FAIL-2
// RUN: %t3 0 0 0 0  2 3   | FileCheck %S/subscript.c --check-prefix=CHECK-FAIL-2
// RUN: %t3 0 0 0 0  0 9   | FileCheck %S/subscript.c --check-prefix=CHECK-FAIL-2
// RUN: %t3 0 0 0 0  -1 -1 | FileCheck %S/subscript.c --check-prefix=CHECK-FAIL-2
// RUN: %t3 0 0 0 0  0 0  3 0 0    | FileCheck %S/subscript.c --check-prefix=CHECK-FAIL-3
// RUN: %t3 0 0 0 0  0 0  2 9 0    | FileCheck %S/subscript.c --check-prefix=CHECK-FAIL-3
// RUN: %t3 0 0 0 0  0 0  2 2 3    | FileCheck %S/subscript.c --check-prefix=CHECK-FAIL-3
// RUN: %t3 0 0 0 0  0 0  0 0 27   | FileCheck %S/subscript.c --check-prefix=CHECK-FAIL-3
// RUN: %t3 0 0 0 0  0 0  -1 -1 -1 | FileCheck %S/subscript.c --check-prefix=CHECK-FAIL-3
//
// RUN: %clang -fcheckedc-extension %S/subscript.c -DTEST_COMPOUND_ASSIGN -o %t4 -Werror -O3
// RUN: %t4 0 0 0 0  0 0   0 0 0  | FileCheck %S/subscript.c
// RUN: %t4 1 2 4 4  1 2   1 1 1  | FileCheck %S/subscript.c
// RUN: %t4 2 4 8 8  2 1   2 2 2  | FileCheck %S/subscript.c
// RUN: %t4 2 4 8 8  0 4   0 3 2  | FileCheck %S/subscript.c
// RUN: %t4 2 4 8 8  1 3   0 1 5  | FileCheck %S/subscript.c
// RUN: %t4 2 4 8 8  2 -1  2 -1 2 | FileCheck %S/subscript.c
// RUN: %t4 3        | FileCheck %S/subscript.c --check-prefix=CHECK-FAIL-1
// RUN: %t4 -1       | FileCheck %S/subscript.c --check-prefix=CHECK-FAIL-1
// RUN: %t4 0 5      | FileCheck %S/subscript.c --check-prefix=CHECK-FAIL-1
// RUN: %t4 0 -1     | FileCheck %S/subscript.c --check-prefix=CHECK-FAIL-1
// RUN: %t4 0 0 9    | FileCheck %S/subscript.c --check-prefix=CHECK-FAIL-1
// RUN: %t4 0 0 -1   | FileCheck %S/subscript.c --check-prefix=CHECK-FAIL-1
// RUN: %t4 0 0 0 9  | FileCheck %S/subscript.c --check-prefix=CHECK-FAIL-1
// RUN: %t4 0 0 0 -1 | FileCheck %S/subscript.c --check-prefix=CHECK-FAIL-1
// RUN: %t4 0 0 0 0  3 0   | FileCheck %S/subscript.c --check-prefix=CHECK-FAIL-2
// RUN: %t4 0 0 0 0  2 3   | FileCheck %S/subscript.c --check-prefix=CHECK-FAIL-2
// RUN: %t4 0 0 0 0  0 9   | FileCheck %S/subscript.c --check-prefix=CHECK-FAIL-2
// RUN: %t4 0 0 0 0  -1 -1 | FileCheck %S/subscript.c --check-prefix=CHECK-FAIL-2
// RUN: %t4 0 0 0 0  0 0  3 0 0    | FileCheck %S/subscript.c --check-prefix=CHECK-FAIL-3
// RUN: %t4 0 0 0 0  0 0  2 9 0    | FileCheck %S/subscript.c --check-prefix=CHECK-FAIL-3
// RUN: %t4 0 0 0 0  0 0  2 2 3    | FileCheck %S/subscript.c --check-prefix=CHECK-FAIL-3
// RUN: %t4 0 0 0 0  0 0  0 0 27   | FileCheck %S/subscript.c --check-prefix=CHECK-FAIL-3
// RUN: %t4 0 0 0 0  0 0  -1 -1 -1 | FileCheck %S/subscript.c --check-prefix=CHECK-FAIL-3

#include <stdlib.h>

int main(void) {
  // Don't run this file
  return EXIT_FAILURE;
}
