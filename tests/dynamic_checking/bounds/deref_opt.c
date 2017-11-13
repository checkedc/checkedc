// Test runtime bounds checking of uses of pointer dereference expressions with
// optimization enabled.
//
// Uses are tested in read, assignment,increment, and compound assignment 
// expressions.
//
// This builds the file `deref.c` at optimisation level O3, 
// outputting executables at %tn. We then run these with the same arguments as
// we run the unoptimised binaries with, and use FileCheck to ensure the
// output is what we expected, as specified in `deref.c`. This
// is run as a separate test so we know if optimisation is breaking some
// dynamic checks.
//
// The following lines are for the clang automated test suite.
//
// RUN: %clang -fcheckedc-extension %S/deref.c -o %t1 -DTEST_READ -Werror -Wno-unused-value  -Wno-check-memory-accesses -O3
// RUN: %t1 pass1 | FileCheck %S/deref.c --check-prefixes=CHECK,CHECK-PASS,PASS-1-READ
// RUN: %t1 pass2 | FileCheck %S/deref.c --check-prefixes=CHECK,CHECK-PASS,PASS-2-READ
// RUN: %t1 pass3 | FileCheck %S/deref.c --check-prefixes=CHECK,CHECK-PASS,PASS-3-READ
// RUN: %t1 fail1 | FileCheck %S/deref.c --check-prefixes=CHECK,CHECK-FAIL,FAIL-1
// RUN: %t1 fail2 | FileCheck %S/deref.c --check-prefixes=CHECK,CHECK-FAIL,FAIL-2
// RUN: %t1 fail3 | FileCheck %S/deref.c --check-prefixes=CHECK,CHECK-FAIL,FAIL-3
// RUN: %t1 fail4 | FileCheck %S/deref.c --check-prefixes=CHECK,CHECK-FAIL,FAIL-4
//
// RUN: %clang -fcheckedc-extension %S/deref.c -o %t2 -DTEST_WRITE -Werror  -Wno-check-memory-accesses -O3
// RUN: %t2 pass1 | FileCheck %S/deref.c --check-prefixes=CHECK,CHECK-PASS,PASS-1-WRITE
// RUN: %t2 pass2 | FileCheck %S/deref.c --check-prefixes=CHECK,CHECK-PASS,PASS-2-WRITE
// RUN: %t2 pass3 | FileCheck %S/deref.c --check-prefixes=CHECK,CHECK-PASS,PASS-3-WRITE
// RUN: %t2 fail1 | FileCheck %S/deref.c --check-prefixes=CHECK,CHECK-FAIL,FAIL-1
// RUN: %t2 fail2 | FileCheck %S/deref.c --check-prefixes=CHECK,CHECK-FAIL,FAIL-2
// RUN: %t2 fail3 | FileCheck %S/deref.c --check-prefixes=CHECK,CHECK-FAIL,FAIL-3
// RUN: %t2 fail4 | FileCheck %S/deref.c --check-prefixes=CHECK,CHECK-FAIL,FAIL-4

// RUN: %clang -fcheckedc-extension %S/deref.c -o %t3 -DTEST_INCREMENT -Werror  -Wno-check-memory-accesses -O3
// RUN: %t3 pass1 | FileCheck %S/deref.c --check-prefixes=CHECK,CHECK-PASS,PASS-1-INCREMENT
// RUN: %t3 pass2 | FileCheck %S/deref.c --check-prefixes=CHECK,CHECK-PASS,PASS-2-INCREMENT
// RUN: %t3 pass3 | FileCheck %S/deref.c --check-prefixes=CHECK,CHECK-PASS,PASS-3-INCREMENT
// RUN: %t3 fail1 | FileCheck %S/deref.c --check-prefixes=CHECK,CHECK-FAIL,FAIL-1
// RUN: %t3 fail2 | FileCheck %S/deref.c --check-prefixes=CHECK,CHECK-FAIL,FAIL-2
// RUN: %t3 fail3 | FileCheck %S/deref.c --check-prefixes=CHECK,CHECK-FAIL,FAIL-3
// RUN: %t3 fail4 | FileCheck %S/deref.c --check-prefixes=CHECK,CHECK-FAIL,FAIL-4

// RUN: %clang -fcheckedc-extension %S/deref.c -o %t4 -DTEST_COMPOUND_ASSIGN -Werror -Wno-check-memory-accesses -O3
// RUN: %t4 pass1 | FileCheck %S/deref.c --check-prefixes=CHECK,CHECK-PASS,PASS-1-COMPOUND-ASSIGN
// RUN: %t4 pass2 | FileCheck %S/deref.c --check-prefixes=CHECK,CHECK-PASS,PASS-2-COMPOUND-ASSIGN
// RUN: %t4 pass3 | FileCheck %S/deref.c --check-prefixes=CHECK,CHECK-PASS,PASS-3-COMPOUND-ASSIGN
// RUN: %t4 fail1 | FileCheck %S/deref.c --check-prefixes=CHECK,CHECK-FAIL,FAIL-1
// RUN: %t4 fail2 | FileCheck %S/deref.c --check-prefixes=CHECK,CHECK-FAIL,FAIL-2
// RUN: %t4 fail3 | FileCheck %S/deref.c --check-prefixes=CHECK,CHECK-FAIL,FAIL-3
// RUN: %t4 fail4 | FileCheck %S/deref.c --check-prefixes=CHECK,CHECK-FAIL,FAIL-4

#import <stdlib.h>

int main(void) {
  return EXIT_FAILURE;
}
