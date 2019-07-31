// Test bounds checking of uses of array subscript expressions where the
// pointer values are member expressions formed using the arrow operator.
// Here is anexample:
//
// struct S *pv;
// (p->v)[e]
//
//
// This builds the file `subscript_dot_member_expr.c` for different operations
// with -DARROW_OPERATOR defined.
//
// The following lines are for the clang automated test suite.
//
// RUN: %clang %S/subscript_dot_member_expr.c -o %t1 -DTEST_READ -DARROW_OPERATOR -Werror %checkedc_target_flags
// RUN: %checkedc_rununder %t1 pass1 | FileCheck %S/subscript_dot_member_expr.c --check-prefixes=CHECK,CHECK-PASS,ARROW,PASS-1-READ
// RUN: %checkedc_rununder %t1 pass2 | FileCheck %S/subscript_dot_member_expr.c --check-prefixes=CHECK,CHECK-PASS,ARROW,PASS-2-READ
// RUN: %checkedc_rununder %t1 pass3 | FileCheck %S/subscript_dot_member_expr.c --check-prefixes=CHECK,CHECK-PASS,ARROW,PASS-3-READ
// RUN: %checkedc_rununder %t1 pass4 | FileCheck %S/subscript_dot_member_expr.c --check-prefixes=CHECK,CHECK-PASS,ARROW,PASS-4-READ
// RUN: %checkedc_rununder %t1 fail1 | FileCheck %S/subscript_dot_member_expr.c --check-prefixes=CHECK,CHECK-FAIL,ARROW,FAIL-1
// RUN: %checkedc_rununder %t1 fail2 | FileCheck %S/subscript_dot_member_expr.c --check-prefixes=CHECK,CHECK-FAIL,ARROW,FAIL-2
// RUN: %checkedc_rununder %t1 fail3 | FileCheck %S/subscript_dot_member_expr.c --check-prefixes=CHECK,CHECK-FAIL,ARROW,FAIL-3
// RUN: %checkedc_rununder %t1 fail4 | FileCheck %S/subscript_dot_member_expr.c --check-prefixes=CHECK,CHECK-FAIL,ARROW,FAIL-4
// RUN: %checkedc_rununder %t1 fail5 | FileCheck %S/subscript_dot_member_expr.c --check-prefixes=CHECK,CHECK-FAIL,ARROW,FAIL-5
//
// RUN: %clang %S/subscript_dot_member_expr.c -o %t2 -DTEST_WRITE -DARROW_OPERATOR -Werror %checkedc_target_flags
// RUN: %checkedc_rununder %t2 pass1 | FileCheck %S/subscript_dot_member_expr.c --check-prefixes=CHECK,CHECK-PASS,ARROW,PASS-1-WRITE
// RUN: %checkedc_rununder %t2 pass2 | FileCheck %S/subscript_dot_member_expr.c --check-prefixes=CHECK,CHECK-PASS,ARROW,PASS-2-WRITE
// RUN: %checkedc_rununder %t2 pass3 | FileCheck %S/subscript_dot_member_expr.c --check-prefixes=CHECK,CHECK-PASS,ARROW,PASS-3-WRITE
// RUN: %checkedc_rununder %t2 pass4 | FileCheck %S/subscript_dot_member_expr.c --check-prefixes=CHECK,CHECK-PASS,ARROW,PASS-4-WRITE
// RUN: %checkedc_rununder %t2 fail1 | FileCheck %S/subscript_dot_member_expr.c --check-prefixes=CHECK,CHECK-FAIL,ARROW,FAIL-1
// RUN: %checkedc_rununder %t2 fail2 | FileCheck %S/subscript_dot_member_expr.c --check-prefixes=CHECK,CHECK-FAIL,ARROW,FAIL-2
// RUN: %checkedc_rununder %t2 fail3 | FileCheck %S/subscript_dot_member_expr.c --check-prefixes=CHECK,CHECK-FAIL,ARROW,FAIL-3
// RUN: %checkedc_rununder %t2 fail4 | FileCheck %S/subscript_dot_member_expr.c --check-prefixes=CHECK,CHECK-FAIL,ARROW,FAIL-4
// RUN: %checkedc_rununder %t2 fail5 | FileCheck %S/subscript_dot_member_expr.c --check-prefixes=CHECK,CHECK-FAIL,ARROW,FAIL-5

// RUN: %clang %S/subscript_dot_member_expr.c -o %t3 -DTEST_INCREMENT -DARROW_OPERATOR -Werror %checkedc_target_flags
// RUN: %checkedc_rununder %t3 pass1 | FileCheck %S/subscript_dot_member_expr.c --check-prefixes=CHECK,CHECK-PASS,ARROW,PASS-1-INCREMENT
// RUN: %checkedc_rununder %t3 pass2 | FileCheck %S/subscript_dot_member_expr.c --check-prefixes=CHECK,CHECK-PASS,ARROW,PASS-2-INCREMENT
// RUN: %checkedc_rununder %t3 pass3 | FileCheck %S/subscript_dot_member_expr.c --check-prefixes=CHECK,CHECK-PASS,ARROW,PASS-3-INCREMENT
// RUN: %checkedc_rununder %t3 pass4 | FileCheck %S/subscript_dot_member_expr.c --check-prefixes=CHECK,CHECK-PASS,ARROW,PASS-4-INCREMENT
// RUN: %checkedc_rununder %t3 fail1 | FileCheck %S/subscript_dot_member_expr.c --check-prefixes=CHECK,CHECK-FAIL,ARROW,FAIL-1
// RUN: %checkedc_rununder %t3 fail2 | FileCheck %S/subscript_dot_member_expr.c --check-prefixes=CHECK,CHECK-FAIL,ARROW,FAIL-2
// RUN: %checkedc_rununder %t3 fail3 | FileCheck %S/subscript_dot_member_expr.c --check-prefixes=CHECK,CHECK-FAIL,ARROW,FAIL-3
// RUN: %checkedc_rununder %t3 fail4 | FileCheck %S/subscript_dot_member_expr.c --check-prefixes=CHECK,CHECK-FAIL,ARROW,FAIL-4
// RUN: %checkedc_rununder %t3 fail5 | FileCheck %S/subscript_dot_member_expr.c --check-prefixes=CHECK,CHECK-FAIL,ARROW,FAIL-5

// RUN: %clang %S/subscript_dot_member_expr.c -o %t4 -DTEST_COMPOUND_ASSIGN -DARROW_OPERATOR -Werror %checkedc_target_flags
// RUN: %checkedc_rununder %t4 pass1 | FileCheck %S/subscript_dot_member_expr.c --check-prefixes=CHECK,CHECK-PASS,ARROW,PASS-1-COMPOUND-ASSIGN
// RUN: %checkedc_rununder %t4 pass2 | FileCheck %S/subscript_dot_member_expr.c --check-prefixes=CHECK,CHECK-PASS,ARROW,PASS-2-COMPOUND-ASSIGN
// RUN: %checkedc_rununder %t4 pass3 | FileCheck %S/subscript_dot_member_expr.c --check-prefixes=CHECK,CHECK-PASS,ARROW,PASS-3-COMPOUND-ASSIGN
// RUN: %checkedc_rununder %t4 pass4 | FileCheck %S/subscript_dot_member_expr.c --check-prefixes=CHECK,CHECK-PASS,ARROW,PASS-4-COMPOUND-ASSIGN
// RUN: %checkedc_rununder %t4 fail1 | FileCheck %S/subscript_dot_member_expr.c --check-prefixes=CHECK,CHECK-FAIL,ARROW,FAIL-1
// RUN: %checkedc_rununder %t4 fail2 | FileCheck %S/subscript_dot_member_expr.c --check-prefixes=CHECK,CHECK-FAIL,ARROW,FAIL-2
// RUN: %checkedc_rununder %t4 fail3 | FileCheck %S/subscript_dot_member_expr.c --check-prefixes=CHECK,CHECK-FAIL,ARROW,FAIL-3
// RUN: %checkedc_rununder %t4 fail4 | FileCheck %S/subscript_dot_member_expr.c --check-prefixes=CHECK,CHECK-FAIL,ARROW,FAIL-4
// RUN: %checkedc_rununder %t4 fail5 | FileCheck %S/subscript_dot_member_expr.c --check-prefixes=CHECK,CHECK-FAIL,ARROW,FAIL-5
#import <stdlib.h>

int main(void) {
  return EXIT_FAILURE;
}
