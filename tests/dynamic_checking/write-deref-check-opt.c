// Tests that the Dereference Operator works with dynamic checks
//
// The following lines are for the clang automated test suite
//
// This builds the file `write-deref-check.c` at optimisation level O3, outputting an
// executable at %t. We then run this with the same arguments as we run the unoptimised
// binary with, and use FileCheck to ensure the output is what we expected, as specified
// in `write-deref-check.c`. This is run as a seperate test so we know if optimisation is
// breaking our dynamic checks.
//
// RUN: %clang -fcheckedc-extension %S/write-deref-check.c -o %t -Werror -O3
// RUN: %t pass1 | FileCheck %S/write-deref-check.c --check-prefixes=CHECK,CHECK-PASS,CHECK-PASS-1
// RUN: %t pass2 | FileCheck %S/write-deref-check.c --check-prefixes=CHECK,CHECK-PASS,CHECK-PASS-2
// RUN: %t fail1 | FileCheck %S/write-deref-check.c --check-prefixes=CHECK,CHECK-FAIL,CHECK-FAIL-1
// RUN: %t fail2 | FileCheck %S/write-deref-check.c --check-prefixes=CHECK,CHECK-FAIL,CHECK-FAIL-2
// RUN: %t fail3 | FileCheck %S/write-deref-check.c --check-prefixes=CHECK,CHECK-FAIL,CHECK-FAIL-3
// RUN: %t fail4 | FileCheck %S/write-deref-check.c --check-prefixes=CHECK,CHECK-FAIL,CHECK-FAIL-4

#import <stdlib.h>

int main(void) {
  return EXIT_FAILURE;
}
