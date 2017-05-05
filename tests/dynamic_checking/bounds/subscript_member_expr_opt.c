// Tests that the Array Subscript Operator works with dynamic checks
//
// The following lines are for the clang automated test suite
//
// This builds the file `subscript_member_expr.c` at optimisation level O3, 
// outputting executables at %tn. We then run these executables with the same 
// arguments as we run the unoptimised binaroes with, and use FileCheck to 
// ensure the output is what we expected, as specified in 
// `subscript_member_expr.c`. This is run as a separate test so we know if 
// optimisation is breaking the dynamic checks.

//
// RUN: %clang -fcheckedc-extension %S/subscript_member_expr.c -o %t1 -DTEST_READ -DARROW_OPERATOR -Werror
// RUN: %t1 pass1 | FileCheck %S/subscript_member_expr.c --check-prefixes=CHECK,CHECK-PASS,ARROW,PASS-1-READ
// RUN: %t1 pass2 | FileCheck %S/subscript_member_expr.c --check-prefixes=CHECK,CHECK-PASS,ARROW,PASS-2-READ
// RUN: %t1 pass3 | FileCheck %S/subscript_member_expr.c --check-prefixes=CHECK,CHECK-PASS,ARROW,PASS-3-READ
// RUN: %t1 pass4 | FileCheck %S/subscript_member_expr.c --check-prefixes=CHECK,CHECK-PASS,ARROW,PASS-4-READ
// RUN: %t1 fail1 | FileCheck %S/subscript_member_expr.c --check-prefixes=CHECK,CHECK-FAIL,ARROW,FAIL-1
// RUN: %t1 fail2 | FileCheck %S/subscript_member_expr.c --check-prefixes=CHECK,CHECK-FAIL,ARROW,FAIL-2
// RUN: %t1 fail3 | FileCheck %S/subscript_member_expr.c --check-prefixes=CHECK,CHECK-FAIL,ARROW,FAIL-3
// RUN: %t1 fail4 | FileCheck %S/subscript_member_expr.c --check-prefixes=CHECK,CHECK-FAIL,ARROW,FAIL-4
// RUN: %t1 fail5 | FileCheck %S/subscript_member_expr.c --check-prefixes=CHECK,CHECK-FAIL,ARROW,FAIL-5
//
// RUN: %clang -fcheckedc-extension %S/subscript_member_expr.c -o %t2 -DTEST_WRITE -DARROW_OPERATOR -Werror
// RUN: %t2 pass1 | FileCheck %S/subscript_member_expr.c --check-prefixes=CHECK,CHECK-PASS,ARROW,PASS-1-WRITE
// RUN: %t2 pass2 | FileCheck %S/subscript_member_expr.c --check-prefixes=CHECK,CHECK-PASS,ARROW,PASS-2-WRITE
// RUN: %t2 pass3 | FileCheck %S/subscript_member_expr.c --check-prefixes=CHECK,CHECK-PASS,ARROW,PASS-3-WRITE
// RUN: %t2 pass4 | FileCheck %S/subscript_member_expr.c --check-prefixes=CHECK,CHECK-PASS,ARROW,PASS-4-WRITE
// RUN: %t2 fail1 | FileCheck %S/subscript_member_expr.c --check-prefixes=CHECK,CHECK-FAIL,ARROW,FAIL-1
// RUN: %t2 fail2 | FileCheck %S/subscript_member_expr.c --check-prefixes=CHECK,CHECK-FAIL,ARROW,FAIL-2
// RUN: %t2 fail3 | FileCheck %S/subscript_member_expr.c --check-prefixes=CHECK,CHECK-FAIL,ARROW,FAIL-3
// RUN: %t2 fail4 | FileCheck %S/subscript_member_expr.c --check-prefixes=CHECK,CHECK-FAIL,ARROW,FAIL-4
// RUN: %t2 fail5 | FileCheck %S/subscript_member_expr.c --check-prefixes=CHECK,CHECK-FAIL,ARROW,FAIL-5

// RUN: %clang -fcheckedc-extension %S/subscript_member_expr.c -o %t3 -DTEST_INCREMENT -DARROW_OPERATOR -Werror
// RUN: %t3 pass1 | FileCheck %S/subscript_member_expr.c --check-prefixes=CHECK,CHECK-PASS,ARROW,PASS-1-INCREMENT
// RUN: %t3 pass2 | FileCheck %S/subscript_member_expr.c --check-prefixes=CHECK,CHECK-PASS,ARROW,PASS-2-INCREMENT
// RUN: %t3 pass3 | FileCheck %S/subscript_member_expr.c --check-prefixes=CHECK,CHECK-PASS,ARROW,PASS-3-INCREMENT
// RUN: %t3 pass4 | FileCheck %S/subscript_member_expr.c --check-prefixes=CHECK,CHECK-PASS,ARROW,PASS-4-INCREMENT
// RUN: %t3 fail1 | FileCheck %S/subscript_member_expr.c --check-prefixes=CHECK,CHECK-FAIL,ARROW,FAIL-1
// RUN: %t3 fail2 | FileCheck %S/subscript_member_expr.c --check-prefixes=CHECK,CHECK-FAIL,ARROW,FAIL-2
// RUN: %t3 fail3 | FileCheck %S/subscript_member_expr.c --check-prefixes=CHECK,CHECK-FAIL,ARROW,FAIL-3
// RUN: %t3 fail4 | FileCheck %S/subscript_member_expr.c --check-prefixes=CHECK,CHECK-FAIL,ARROW,FAIL-4
// RUN: %t3 fail5 | FileCheck %S/subscript_member_expr.c --check-prefixes=CHECK,CHECK-FAIL,ARROW,FAIL-5

// RUN: %clang -fcheckedc-extension %S/subscript_member_expr.c -o %t4 -DTEST_COMPOUND_ASSIGN -DARROW_OPERATOR -Werror
// RUN: %t4 pass1 | FileCheck %S/subscript_member_expr.c --check-prefixes=CHECK,CHECK-PASS,ARROW,PASS-1-COMPOUND-ASSIGN
// RUN: %t4 pass2 | FileCheck %S/subscript_member_expr.c --check-prefixes=CHECK,CHECK-PASS,ARROW,PASS-2-COMPOUND-ASSIGN
// RUN: %t4 pass3 | FileCheck %S/subscript_member_expr.c --check-prefixes=CHECK,CHECK-PASS,ARROW,PASS-3-COMPOUND-ASSIGN
// RUN: %t4 pass4 | FileCheck %S/subscript_member_expr.c --check-prefixes=CHECK,CHECK-PASS,ARROW,PASS-4-COMPOUND-ASSIGN
// RUN: %t4 fail1 | FileCheck %S/subscript_member_expr.c --check-prefixes=CHECK,CHECK-FAIL,ARROW,FAIL-1
// RUN: %t4 fail2 | FileCheck %S/subscript_member_expr.c --check-prefixes=CHECK,CHECK-FAIL,ARROW,FAIL-2
// RUN: %t4 fail3 | FileCheck %S/subscript_member_expr.c --check-prefixes=CHECK,CHECK-FAIL,ARROW,FAIL-3
// RUN: %t4 fail4 | FileCheck %S/subscript_member_expr.c --check-prefixes=CHECK,CHECK-FAIL,ARROW,FAIL-4
// RUN: %t4 fail5 | FileCheck %S/subscript_member_expr.c --check-prefixes=CHECK,CHECK-FAIL,ARROW,FAIL-5

// -----------------------------------------------------------------------------------
// Dot operator versions of tests
//
// These are just cut-and-pasted versions of the prior section, with -DARROW_OPERATOR
// removed and new temporary names used.
// -----------------------------------------------------------------------------------

// RUN: %clang -fcheckedc-extension %S/subscript_member_expr.c -o %t5 -DTEST_READ -Werror
// RUN: %t5 pass1 | FileCheck %S/subscript_member_expr.c --check-prefixes=CHECK,CHECK-PASS,PASS-1-READ
// RUN: %t5 pass2 | FileCheck %S/subscript_member_expr.c --check-prefixes=CHECK,CHECK-PASS,PASS-2-READ
// RUN: %t5 pass3 | FileCheck %S/subscript_member_expr.c --check-prefixes=CHECK,CHECK-PASS,PASS-3-READ
// RUN: %t5 pass4 | FileCheck %S/subscript_member_expr.c --check-prefixes=CHECK,CHECK-PASS,PASS-4-READ
// RUN: %t5 fail1 | FileCheck %S/subscript_member_expr.c --check-prefixes=CHECK,CHECK-FAIL,FAIL-1
// RUN: %t5 fail2 | FileCheck %S/subscript_member_expr.c --check-prefixes=CHECK,CHECK-FAIL,FAIL-2
// RUN: %t5 fail3 | FileCheck %S/subscript_member_expr.c --check-prefixes=CHECK,CHECK-FAIL,FAIL-3
// RUN: %t5 fail4 | FileCheck %S/subscript_member_expr.c --check-prefixes=CHECK,CHECK-FAIL,FAIL-4
// RUN: %t5 fail5 | FileCheck %S/subscript_member_expr.c --check-prefixes=CHECK,CHECK-FAIL,FAIL-5
//
// RUN: %clang -fcheckedc-extension %S/subscript_member_expr.c -o %t6 -DTEST_WRITE -Werror
// RUN: %t6 pass1 | FileCheck %S/subscript_member_expr.c --check-prefixes=CHECK,CHECK-PASS,PASS-1-WRITE
// RUN: %t6 pass2 | FileCheck %S/subscript_member_expr.c --check-prefixes=CHECK,CHECK-PASS,PASS-2-WRITE
// RUN: %t6 pass3 | FileCheck %S/subscript_member_expr.c --check-prefixes=CHECK,CHECK-PASS,PASS-3-WRITE
// RUN: %t6 pass4 | FileCheck %S/subscript_member_expr.c --check-prefixes=CHECK,CHECK-PASS,PASS-4-WRITE
// RUN: %t6 fail1 | FileCheck %S/subscript_member_expr.c --check-prefixes=CHECK,CHECK-FAIL,FAIL-1
// RUN: %t6 fail2 | FileCheck %S/subscript_member_expr.c --check-prefixes=CHECK,CHECK-FAIL,FAIL-2
// RUN: %t6 fail3 | FileCheck %S/subscript_member_expr.c --check-prefixes=CHECK,CHECK-FAIL,FAIL-3
// RUN: %t6 fail4 | FileCheck %S/subscript_member_expr.c --check-prefixes=CHECK,CHECK-FAIL,FAIL-4
// RUN: %t6 fail5 | FileCheck %S/subscript_member_expr.c --check-prefixes=CHECK,CHECK-FAIL,FAIL-5

// RUN: %clang -fcheckedc-extension %S/subscript_member_expr.c -o %t7 -DTEST_INCREMENT -Werror
// RUN: %t7 pass1 | FileCheck %S/subscript_member_expr.c --check-prefixes=CHECK,CHECK-PASS,PASS-1-INCREMENT
// RUN: %t7 pass2 | FileCheck %S/subscript_member_expr.c --check-prefixes=CHECK,CHECK-PASS,PASS-2-INCREMENT
// RUN: %t7 pass3 | FileCheck %S/subscript_member_expr.c --check-prefixes=CHECK,CHECK-PASS,PASS-3-INCREMENT
// RUN: %t7 pass4 | FileCheck %S/subscript_member_expr.c --check-prefixes=CHECK,CHECK-PASS,PASS-4-INCREMENT
// RUN: %t7 fail1 | FileCheck %S/subscript_member_expr.c --check-prefixes=CHECK,CHECK-FAIL,FAIL-1
// RUN: %t7 fail2 | FileCheck %S/subscript_member_expr.c --check-prefixes=CHECK,CHECK-FAIL,FAIL-2
// RUN: %t7 fail3 | FileCheck %S/subscript_member_expr.c --check-prefixes=CHECK,CHECK-FAIL,FAIL-3
// RUN: %t7 fail4 | FileCheck %S/subscript_member_expr.c --check-prefixes=CHECK,CHECK-FAIL,FAIL-4
// RUN: %t7 fail5 | FileCheck %S/subscript_member_expr.c --check-prefixes=CHECK,CHECK-FAIL,FAIL-5

// RUN: %clang -fcheckedc-extension %S/subscript_member_expr.c -o %t8 -DTEST_COMPOUND_ASSIGN -Werror
// RUN: %t8 pass1 | FileCheck %S/subscript_member_expr.c --check-prefixes=CHECK,CHECK-PASS,PASS-1-COMPOUND-ASSIGN
// RUN: %t8 pass2 | FileCheck %S/subscript_member_expr.c --check-prefixes=CHECK,CHECK-PASS,PASS-2-COMPOUND-ASSIGN
// RUN: %t8 pass3 | FileCheck %S/subscript_member_expr.c --check-prefixes=CHECK,CHECK-PASS,PASS-3-COMPOUND-ASSIGN
// RUN: %t8 pass4 | FileCheck %S/subscript_member_expr.c --check-prefixes=CHECK,CHECK-PASS,PASS-4-COMPOUND-ASSIGN
// RUN: %t8 fail1 | FileCheck %S/subscript_member_expr.c --check-prefixes=CHECK,CHECK-FAIL,FAIL-1
// RUN: %t8 fail2 | FileCheck %S/subscript_member_expr.c --check-prefixes=CHECK,CHECK-FAIL,FAIL-2
// RUN: %t8 fail3 | FileCheck %S/subscript_member_expr.c --check-prefixes=CHECK,CHECK-FAIL,FAIL-3
// RUN: %t8 fail4 | FileCheck %S/subscript_member_expr.c --check-prefixes=CHECK,CHECK-FAIL,FAIL-4
// RUN: %t8 fail5 | FileCheck %S/subscript_member_expr.c --check-prefixes=CHECK,CHECK-FAIL,FAIL-5

#include <stdlib.h>

int main(void) {
  // Don't run this file
  return EXIT_FAILURE;
}
