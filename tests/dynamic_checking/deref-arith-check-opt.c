// Tests that the Dereference Operator works with dynamic checks and pointer arithmetic
//
// The following lines are for the clang automated test suite
//
// RUN: %clang -fcheckedc-extension %S/deref-arith-check.c -o %t -Werror -O3
// RUN: %t 0 0 0 0  0 0   0 0 0  | FileCheck %S/deref-arith-check.c
// RUN: %t 1 2 4 4  1 2   1 1 1  | FileCheck %S/deref-arith-check.c
// RUN: %t 2 4 8 8  2 1   2 2 2  | FileCheck %S/deref-arith-check.c
// RUN: %t 2 4 8 8  0 4   0 3 2  | FileCheck %S/deref-arith-check.c
// RUN: %t 2 4 8 8  1 3   0 1 5  | FileCheck %S/deref-arith-check.c
// RUN: %t 2 4 8 8  2 -1  2 -1 2 | FileCheck %S/deref-arith-check.c
// RUN: not --crash %t 3        | FileCheck %S/deref-arith-check.c --check-prefix=CHECK-FAIL-1
// RUN: not --crash %t -1       | FileCheck %S/deref-arith-check.c --check-prefix=CHECK-FAIL-1
// RUN: not --crash %t 0 5      | FileCheck %S/deref-arith-check.c --check-prefix=CHECK-FAIL-1
// RUN: not --crash %t 0 -1     | FileCheck %S/deref-arith-check.c --check-prefix=CHECK-FAIL-1
// RUN: not --crash %t 0 0 9    | FileCheck %S/deref-arith-check.c --check-prefix=CHECK-FAIL-1
// RUN: not --crash %t 0 0 -1   | FileCheck %S/deref-arith-check.c --check-prefix=CHECK-FAIL-1
// RUN: not --crash %t 0 0 0 9  | FileCheck %S/deref-arith-check.c --check-prefix=CHECK-FAIL-1
// RUN: not --crash %t 0 0 0 -1 | FileCheck %S/deref-arith-check.c --check-prefix=CHECK-FAIL-1
// RUN: not --crash %t 0 0 0 0  3 0   | FileCheck %S/deref-arith-check.c --check-prefix=CHECK-FAIL-2
// RUN: not --crash %t 0 0 0 0  2 3   | FileCheck %S/deref-arith-check.c --check-prefix=CHECK-FAIL-2
// RUN: not --crash %t 0 0 0 0  0 9   | FileCheck %S/deref-arith-check.c --check-prefix=CHECK-FAIL-2
// RUN: not --crash %t 0 0 0 0  -1 -1 | FileCheck %S/deref-arith-check.c --check-prefix=CHECK-FAIL-2
// RUN: not --crash %t 0 0 0 0  0 0  3 0 0    | FileCheck %S/deref-arith-check.c --check-prefix=CHECK-FAIL-3
// RUN: not --crash %t 0 0 0 0  0 0  2 9 0    | FileCheck %S/deref-arith-check.c --check-prefix=CHECK-FAIL-3
// RUN: not --crash %t 0 0 0 0  0 0  2 2 3    | FileCheck %S/deref-arith-check.c --check-prefix=CHECK-FAIL-3
// RUN: not --crash %t 0 0 0 0  0 0  0 0 27   | FileCheck %S/deref-arith-check.c --check-prefix=CHECK-FAIL-3
// RUN: not --crash %t 0 0 0 0  0 0  -1 -1 -1 | FileCheck %S/deref-arith-check.c --check-prefix=CHECK-FAIL-3

#include <stdlib.h>

int main(void) {
  return EXIT_FAILURE;
}