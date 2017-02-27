// Tests that the Dereference Operator works with dynamic checks
//
// The following lines are for the clang automated test suite
//
// RUN: %clang -fcheckedc-extension %S/deref-check.c -o %t -Werror -O3
// RUN: %t pass1 | FileCheck %S/deref-check.c --check-prefixes=CHECK,CHECK-PASS,CHECK-PASS-1
// RUN: %t pass2 | FileCheck %S/deref-check.c --check-prefixes=CHECK,CHECK-PASS,CHECK-PASS-2
// RUN: not --crash %t fail1 | FileCheck %S/deref-check.c --check-prefixes=CHECK,CHECK-FAIL,CHECK-FAIL-1
// RUN: not --crash %t fail2 | FileCheck %S/deref-check.c --check-prefixes=CHECK,CHECK-FAIL,CHECK-FAIL-2
// RUN: not --crash %t fail3 | FileCheck %S/deref-check.c --check-prefixes=CHECK,CHECK-FAIL,CHECK-FAIL-3
// RUN: not --crash %t fail4 | FileCheck %S/deref-check.c --check-prefixes=CHECK,CHECK-FAIL,CHECK-FAIL-4

#import <stdlib.h>

int main(void) {
  return EXIT_FAILURE;
}