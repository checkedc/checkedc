// Tests that the Array Subscript Operator works with dynamic checks
//
// In particular, this re-runs the tests in subscript-check.c with Optimizer at O3
//
// The following lines are for the clang automated test suite
//
// RUN: %clang -fcheckedc-extension %S/subscript-check.c -o %t -Werror -O3
// RUN: %t 0 0 0 0  0 0   0 0 0  | FileCheck %S/subscript-check.c
// RUN: %t 1 2 4 4  1 2   1 1 1  | FileCheck %S/subscript-check.c
// RUN: %t 2 4 8 8  2 1   2 2 2  | FileCheck %S/subscript-check.c
// RUN: %t 2 4 8 8  0 4   0 3 2  | FileCheck %S/subscript-check.c
// RUN: %t 2 4 8 8  1 3   0 1 5  | FileCheck %S/subscript-check.c
// RUN: %t 2 4 8 8  2 -1  2 -1 2 | FileCheck %S/subscript-check.c
// RUN: not --crash %t 3        | FileCheck %S/subscript-check.c --check-prefix=CHECK-FAIL-1
// RUN: not --crash %t -1       | FileCheck %S/subscript-check.c --check-prefix=CHECK-FAIL-1
// RUN: not --crash %t 0 5      | FileCheck %S/subscript-check.c --check-prefix=CHECK-FAIL-1
// RUN: not --crash %t 0 -1     | FileCheck %S/subscript-check.c --check-prefix=CHECK-FAIL-1
// RUN: not --crash %t 0 0 9    | FileCheck %S/subscript-check.c --check-prefix=CHECK-FAIL-1
// RUN: not --crash %t 0 0 -1   | FileCheck %S/subscript-check.c --check-prefix=CHECK-FAIL-1
// RUN: not --crash %t 0 0 0 9  | FileCheck %S/subscript-check.c --check-prefix=CHECK-FAIL-1
// RUN: not --crash %t 0 0 0 -1 | FileCheck %S/subscript-check.c --check-prefix=CHECK-FAIL-1
// RUN: not --crash %t 0 0 0 0  3 0   | FileCheck %S/subscript-check.c --check-prefix=CHECK-FAIL-2
// RUN: not --crash %t 0 0 0 0  2 3   | FileCheck %S/subscript-check.c --check-prefix=CHECK-FAIL-2
// RUN: not --crash %t 0 0 0 0  0 9   | FileCheck %S/subscript-check.c --check-prefix=CHECK-FAIL-2
// RUN: not --crash %t 0 0 0 0  -1 -1 | FileCheck %S/subscript-check.c --check-prefix=CHECK-FAIL-2
// RUN: not --crash %t 0 0 0 0  0 0  3 0 0    | FileCheck %S/subscript-check.c --check-prefix=CHECK-FAIL-3
// RUN: not --crash %t 0 0 0 0  0 0  2 9 0    | FileCheck %S/subscript-check.c --check-prefix=CHECK-FAIL-3
// RUN: not --crash %t 0 0 0 0  0 0  2 2 3    | FileCheck %S/subscript-check.c --check-prefix=CHECK-FAIL-3
// RUN: not --crash %t 0 0 0 0  0 0  0 0 27   | FileCheck %S/subscript-check.c --check-prefix=CHECK-FAIL-3
// RUN: not --crash %t 0 0 0 0  0 0  -1 -1 -1 | FileCheck %S/subscript-check.c --check-prefix=CHECK-FAIL-3

#include <stdlib.h>

int main(void) {
  // Don't run this file
  return EXIT_FAILURE;
}