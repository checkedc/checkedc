// Tests that the Dereference Operator works with dynamic checks
//
// The following lines are for the clang automated test suite
//
// This builds the file `write.c` at optimisation level O3, outputting an
// executable at %t. We then run this with the same arguments as we run the unoptimised
// binary with, and use FileCheck to ensure the output is what we expected, as specified
// in `write.c`. This is run as a seperate test so we know if optimisation is
// breaking our dynamic checks.
//
// RUN: %clang -fcheckedc-extension %S\write.c -o %t1 -DTEST_READ -Werror
// RUN: %t1 pass1 | FileCheck %S\write.c --check-prefixes=CHECK,CHECK-PASS,PASS-1-READ
// RUN: %t1 pass2 | FileCheck %S\write.c --check-prefixes=CHECK,CHECK-PASS,PASS-2-READ
// RUN: %t1 pass3 | FileCheck %S\write.c --check-prefixes=CHECK,CHECK-PASS,PASS-3-READ
// RUN: %t1 fail1 | FileCheck %S\write.c --check-prefixes=CHECK,CHECK-FAIL,FAIL-1
// RUN: %t1 fail2 | FileCheck %S\write.c --check-prefixes=CHECK,CHECK-FAIL,FAIL-2
// RUN: %t1 fail3 | FileCheck %S\write.c --check-prefixes=CHECK,CHECK-FAIL,FAIL-3
// RUN: %t1 fail4 | FileCheck %S\write.c --check-prefixes=CHECK,CHECK-FAIL,FAIL-4
//
// RUN: %clang -fcheckedc-extension %S\write.c -o %t2 -DTEST_WRITE -Werror
// RUN: %t2 pass1 | FileCheck %S\write.c --check-prefixes=CHECK,CHECK-PASS,PASS-1-WRITE
// RUN: %t2 pass2 | FileCheck %S\write.c --check-prefixes=CHECK,CHECK-PASS,PASS-2-WRITE
// RUN: %t2 pass3 | FileCheck %S\write.c --check-prefixes=CHECK,CHECK-PASS,PASS-3-WRITE
// RUN: %t2 fail1 | FileCheck %S\write.c --check-prefixes=CHECK,CHECK-FAIL,FAIL-1
// RUN: %t2 fail2 | FileCheck %S\write.c --check-prefixes=CHECK,CHECK-FAIL,FAIL-2
// RUN: %t2 fail3 | FileCheck %S\write.c --check-prefixes=CHECK,CHECK-FAIL,FAIL-3
// RUN: %t2 fail4 | FileCheck %S\write.c --check-prefixes=CHECK,CHECK-FAIL,FAIL-4

// RUN: %clang -fcheckedc-extension %S\write.c -o %t3 -DTEST_INCREMENT -Werror
// RUN: %t3 pass1 | FileCheck %S\write.c --check-prefixes=CHECK,CHECK-PASS,PASS-1-INCREMENT
// RUN: %t3 pass2 | FileCheck %S\write.c --check-prefixes=CHECK,CHECK-PASS,PASS-2-INCREMENT
// RUN: %t3 pass3 | FileCheck %S\write.c --check-prefixes=CHECK,CHECK-PASS,PASS-3-INCREMENT
// RUN: %t3 fail1 | FileCheck %S\write.c --check-prefixes=CHECK,CHECK-FAIL,FAIL-1
// RUN: %t3 fail2 | FileCheck %S\write.c --check-prefixes=CHECK,CHECK-FAIL,FAIL-2
// RUN: %t3 fail3 | FileCheck %S\write.c --check-prefixes=CHECK,CHECK-FAIL,FAIL-3
// RUN: %t3 fail4 | FileCheck %S\write.c --check-prefixes=CHECK,CHECK-FAIL,FAIL-4

// RUN: %clang -fcheckedc-extension %S\write.c -o %t4 -DTEST_COMPOUND_ASSIGN -Werror
// RUN: %t4 pass1 | FileCheck %S\write.c --check-prefixes=CHECK,CHECK-PASS,PASS-1-COMPOUND-ASSIGN
// RUN: %t4 pass2 | FileCheck %S\write.c --check-prefixes=CHECK,CHECK-PASS,PASS-2-COMPOUND-ASSIGN
// RUN: %t4 pass3 | FileCheck %S\write.c --check-prefixes=CHECK,CHECK-PASS,PASS-3-COMPOUND-ASSIGN
// RUN: %t4 fail1 | FileCheck %S\write.c --check-prefixes=CHECK,CHECK-FAIL,FAIL-1
// RUN: %t4 fail2 | FileCheck %S\write.c --check-prefixes=CHECK,CHECK-FAIL,FAIL-2
// RUN: %t4 fail3 | FileCheck %S\write.c --check-prefixes=CHECK,CHECK-FAIL,FAIL-3
// RUN: %t4 fail4 | FileCheck %S\write.c --check-prefixes=CHECK,CHECK-FAIL,FAIL-4

#import <stdlib.h>

int main(void) {
  return EXIT_FAILURE;
}
