// Test bounds checking of uses of pointer dereference expressions where the
// pointer values are member expressions (formed using the dot or arrow
// operators) witn optimization enabled.
//
// Uses are tested in read, assignment,increment, and compound assignment
// expressions.
//
// This builds the file `dereft_member_expr.c` at optimisation level O3,
// outputting executables at %tn. We then run these with the same arguments as
// we run the unoptimised binaries with, and use FileCheck to ensure the
// output is what we expected, as specified in `deref_dot_member_expr.c`. This
// is run as a separate test so we know if optimisation is breaking some
// dynamic checks.
//
// The following lines are for the clang automated test suite.
//
// RUN: %clang %S/deref_dot_member_expr.c -o %t1 -DTEST_READ -Werror -Wno-unused-value -O3 %checkedc_target_flags
// RUN: %checkedc_rununder %t1 pass1 | FileCheck %S/deref_dot_member_expr.c --check-prefixes=CHECK,CHECK-PASS,PASS-1-READ
// RUN: %checkedc_rununder %t1 pass2 | FileCheck %S/deref_dot_member_expr.c --check-prefixes=CHECK,CHECK-PASS,PASS-2-READ
// RUN: %checkedc_rununder %t1 pass3 | FileCheck %S/deref_dot_member_expr.c --check-prefixes=CHECK,CHECK-PASS,PASS-3-READ
// RUN: %checkedc_rununder %t1 pass4 | FileCheck %S/deref_dot_member_expr.c --check-prefixes=CHECK,CHECK-PASS,PASS-4-READ
// RUN: %checkedc_rununder %t1 pass5 | FileCheck %S/deref_dot_member_expr.c --check-prefixes=CHECK,CHECK-PASS,PASS-5-READ
// RUN: %checkedc_rununder %t1 fail1 | FileCheck %S/deref_dot_member_expr.c --check-prefixes=CHECK,CHECK-FAIL,FAIL-1
// RUN: %checkedc_rununder %t1 fail2 | FileCheck %S/deref_dot_member_expr.c --check-prefixes=CHECK,CHECK-FAIL,FAIL-2
// RUN: %checkedc_rununder %t1 fail3 | FileCheck %S/deref_dot_member_expr.c --check-prefixes=CHECK,CHECK-FAIL,FAIL-3
// RUN: %checkedc_rununder %t1 fail4 | FileCheck %S/deref_dot_member_expr.c --check-prefixes=CHECK,CHECK-FAIL,FAIL-4
// RUN: %checkedc_rununder %t1 fail5 | FileCheck %S/deref_dot_member_expr.c --check-prefixes=CHECK,CHECK-FAIL,FAIL-5
//
// RUN: %clang %S/deref_dot_member_expr.c -o %t2 -DTEST_WRITE -Werror -O3 %checkedc_target_flags
// RUN: %checkedc_rununder %t2 pass1 | FileCheck %S/deref_dot_member_expr.c --check-prefixes=CHECK,CHECK-PASS,PASS-1-WRITE
// RUN: %checkedc_rununder %t2 pass2 | FileCheck %S/deref_dot_member_expr.c --check-prefixes=CHECK,CHECK-PASS,PASS-2-WRITE
// RUN: %checkedc_rununder %t2 pass3 | FileCheck %S/deref_dot_member_expr.c --check-prefixes=CHECK,CHECK-PASS,PASS-3-WRITE
// RUN: %checkedc_rununder %t2 pass4 | FileCheck %S/deref_dot_member_expr.c --check-prefixes=CHECK,CHECK-PASS,PASS-4-WRITE
// RUN: %checkedc_rununder %t2 pass5 | FileCheck %S/deref_dot_member_expr.c --check-prefixes=CHECK,CHECK-PASS,PASS-5-WRITE
// RUN: %checkedc_rununder %t2 fail1 | FileCheck %S/deref_dot_member_expr.c --check-prefixes=CHECK,CHECK-FAIL,FAIL-1
// RUN: %checkedc_rununder %t2 fail2 | FileCheck %S/deref_dot_member_expr.c --check-prefixes=CHECK,CHECK-FAIL,FAIL-2
// RUN: %checkedc_rununder %t2 fail3 | FileCheck %S/deref_dot_member_expr.c --check-prefixes=CHECK,CHECK-FAIL,FAIL-3
// RUN: %checkedc_rununder %t2 fail4 | FileCheck %S/deref_dot_member_expr.c --check-prefixes=CHECK,CHECK-FAIL,FAIL-4
// RUN: %checkedc_rununder %t2 fail5 | FileCheck %S/deref_dot_member_expr.c --check-prefixes=CHECK,CHECK-FAIL,FAIL-5

// RUN: %clang %S/deref_dot_member_expr.c -o %t3 -DTEST_INCREMENT -Werror -O3 %checkedc_target_flags
// RUN: %checkedc_rununder %t3 pass1 | FileCheck %S/deref_dot_member_expr.c --check-prefixes=CHECK,CHECK-PASS,PASS-1-INCREMENT
// RUN: %checkedc_rununder %t3 pass2 | FileCheck %S/deref_dot_member_expr.c --check-prefixes=CHECK,CHECK-PASS,PASS-2-INCREMENT
// RUN: %checkedc_rununder %t3 pass3 | FileCheck %S/deref_dot_member_expr.c --check-prefixes=CHECK,CHECK-PASS,PASS-3-INCREMENT
// RUN: %checkedc_rununder %t3 pass4 | FileCheck %S/deref_dot_member_expr.c --check-prefixes=CHECK,CHECK-PASS,PASS-4-INCREMENT
// RUN: %checkedc_rununder %t3 pass5 | FileCheck %S/deref_dot_member_expr.c --check-prefixes=CHECK,CHECK-PASS,PASS-5-INCREMENT
// RUN: %checkedc_rununder %t3 fail1 | FileCheck %S/deref_dot_member_expr.c --check-prefixes=CHECK,CHECK-FAIL,FAIL-1
// RUN: %checkedc_rununder %t3 fail2 | FileCheck %S/deref_dot_member_expr.c --check-prefixes=CHECK,CHECK-FAIL,FAIL-2
// RUN: %checkedc_rununder %t3 fail3 | FileCheck %S/deref_dot_member_expr.c --check-prefixes=CHECK,CHECK-FAIL,FAIL-3
// RUN: %checkedc_rununder %t3 fail4 | FileCheck %S/deref_dot_member_expr.c --check-prefixes=CHECK,CHECK-FAIL,FAIL-4
// RUN: %checkedc_rununder %t3 fail5 | FileCheck %S/deref_dot_member_expr.c --check-prefixes=CHECK,CHECK-FAIL,FAIL-5

// RUN: %clang %S/deref_dot_member_expr.c -o %t4 -DTEST_COMPOUND_ASSIGN -Werror -O3 %checkedc_target_flags
// RUN: %checkedc_rununder %t4 pass1 | FileCheck %S/deref_dot_member_expr.c --check-prefixes=CHECK,CHECK-PASS,PASS-1-COMPOUND-ASSIGN
// RUN: %checkedc_rununder %t4 pass2 | FileCheck %S/deref_dot_member_expr.c --check-prefixes=CHECK,CHECK-PASS,PASS-2-COMPOUND-ASSIGN
// RUN: %checkedc_rununder %t4 pass3 | FileCheck %S/deref_dot_member_expr.c --check-prefixes=CHECK,CHECK-PASS,PASS-3-COMPOUND-ASSIGN
// RUN: %checkedc_rununder %t4 pass4 | FileCheck %S/deref_dot_member_expr.c --check-prefixes=CHECK,CHECK-PASS,PASS-4-COMPOUND-ASSIGN
// RUN: %checkedc_rununder %t4 pass5 | FileCheck %S/deref_dot_member_expr.c --check-prefixes=CHECK,CHECK-PASS,PASS-5-COMPOUND-ASSIGN
// RUN: %checkedc_rununder %t4 fail1 | FileCheck %S/deref_dot_member_expr.c --check-prefixes=CHECK,CHECK-FAIL,FAIL-1
// RUN: %checkedc_rununder %t4 fail2 | FileCheck %S/deref_dot_member_expr.c --check-prefixes=CHECK,CHECK-FAIL,FAIL-2
// RUN: %checkedc_rununder %t4 fail3 | FileCheck %S/deref_dot_member_expr.c --check-prefixes=CHECK,CHECK-FAIL,FAIL-3
// RUN: %checkedc_rununder %t4 fail4 | FileCheck %S/deref_dot_member_expr.c --check-prefixes=CHECK,CHECK-FAIL,FAIL-4
// RUN: %checkedc_rununder %t4 fail5 | FileCheck %S/deref_dot_member_expr.c --check-prefixes=CHECK,CHECK-FAIL,FAIL-5

#include <stdlib.h>

int main(void) {
  // Don't run this file
  return EXIT_FAILURE;
}
