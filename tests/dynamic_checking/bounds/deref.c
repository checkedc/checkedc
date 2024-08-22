// Test runtime bounds checking of uses of pointer dereference expressions.
//
// Uses are tested in read, assignment,increment, and compound assignment
// expressions.  The type of use is controlled by the macro names TEST_READ,
// TEST_WRITE, TEST_INCREMENT, and TEST_COMPOUND_ASSIGNMENT.  The file must
// be compiled with exactly one of those macro names defined.
//
// The following lines are for the clang automated test suite.
//
// RUN: %clang %s -o %t1 -DTEST_READ -Werror -Wno-unused-value -Wno-check-bounds-decls-unchecked-scope %checkedc_target_flags
// RUN: %checkedc_rununder %t1 pass1 | FileCheck %s --check-prefixes=CHECK,CHECK-PASS,PASS-1-READ
// RUN: %checkedc_rununder %t1 pass2 | FileCheck %s --check-prefixes=CHECK,CHECK-PASS,PASS-2-READ
// RUN: %checkedc_rununder %t1 pass3 | FileCheck %s --check-prefixes=CHECK,CHECK-PASS,PASS-3-READ
// RUN: %checkedc_rununder %t1 fail1 | FileCheck %s --check-prefixes=CHECK,CHECK-FAIL,FAIL-1
// RUN: %checkedc_rununder %t1 fail2 | FileCheck %s --check-prefixes=CHECK,CHECK-FAIL,FAIL-2
// RUN: %checkedc_rununder %t1 fail3 | FileCheck %s --check-prefixes=CHECK,CHECK-FAIL,FAIL-3
// RUN: %checkedc_rununder %t1 fail4 | FileCheck %s --check-prefixes=CHECK,CHECK-FAIL,FAIL-4
//
// RUN: %clang %s -o %t2 -DTEST_WRITE -Werror -Wno-check-bounds-decls-unchecked-scope %checkedc_target_flags
// RUN: %checkedc_rununder %t2 pass1 | FileCheck %s --check-prefixes=CHECK,CHECK-PASS,PASS-1-WRITE
// RUN: %checkedc_rununder %t2 pass2 | FileCheck %s --check-prefixes=CHECK,CHECK-PASS,PASS-2-WRITE
// RUN: %checkedc_rununder %t2 pass3 | FileCheck %s --check-prefixes=CHECK,CHECK-PASS,PASS-3-WRITE
// RUN: %checkedc_rununder %t2 fail1 | FileCheck %s --check-prefixes=CHECK,CHECK-FAIL,FAIL-1
// RUN: %checkedc_rununder %t2 fail2 | FileCheck %s --check-prefixes=CHECK,CHECK-FAIL,FAIL-2
// RUN: %checkedc_rununder %t2 fail3 | FileCheck %s --check-prefixes=CHECK,CHECK-FAIL,FAIL-3
// RUN: %checkedc_rununder %t2 fail4 | FileCheck %s --check-prefixes=CHECK,CHECK-FAIL,FAIL-4

// RUN: %clang %s -o %t3 -DTEST_INCREMENT -Werror -Wno-check-bounds-decls-unchecked-scope %checkedc_target_flags
// RUN: %checkedc_rununder %t3 pass1 | FileCheck %s --check-prefixes=CHECK,CHECK-PASS,PASS-1-INCREMENT
// RUN: %checkedc_rununder %t3 pass2 | FileCheck %s --check-prefixes=CHECK,CHECK-PASS,PASS-2-INCREMENT
// RUN: %checkedc_rununder %t3 pass3 | FileCheck %s --check-prefixes=CHECK,CHECK-PASS,PASS-3-INCREMENT
// RUN: %checkedc_rununder %t3 fail1 | FileCheck %s --check-prefixes=CHECK,CHECK-FAIL,FAIL-1
// RUN: %checkedc_rununder %t3 fail2 | FileCheck %s --check-prefixes=CHECK,CHECK-FAIL,FAIL-2
// RUN: %checkedc_rununder %t3 fail3 | FileCheck %s --check-prefixes=CHECK,CHECK-FAIL,FAIL-3
// RUN: %checkedc_rununder %t3 fail4 | FileCheck %s --check-prefixes=CHECK,CHECK-FAIL,FAIL-4

// RUN: %clang %s -o %t4 -DTEST_COMPOUND_ASSIGN -Werror -Wno-check-bounds-decls-unchecked-scope %checkedc_target_flags
// RUN: %checkedc_rununder %t4 pass1 | FileCheck %s --check-prefixes=CHECK,CHECK-PASS,PASS-1-COMPOUND-ASSIGN
// RUN: %checkedc_rununder %t4 pass2 | FileCheck %s --check-prefixes=CHECK,CHECK-PASS,PASS-2-COMPOUND-ASSIGN
// RUN: %checkedc_rununder %t4 pass3 | FileCheck %s --check-prefixes=CHECK,CHECK-PASS,PASS-3-COMPOUND-ASSIGN
// RUN: %checkedc_rununder %t4 fail1 | FileCheck %s --check-prefixes=CHECK,CHECK-FAIL,FAIL-1
// RUN: %checkedc_rununder %t4 fail2 | FileCheck %s --check-prefixes=CHECK,CHECK-FAIL,FAIL-2
// RUN: %checkedc_rununder %t4 fail3 | FileCheck %s --check-prefixes=CHECK,CHECK-FAIL,FAIL-3
// RUN: %checkedc_rununder %t4 fail4 | FileCheck %s --check-prefixes=CHECK,CHECK-FAIL,FAIL-4

#include <assert.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdchecked.h>

#ifdef TEST_READ
#define TEST_OP(e1, e2) e1
#endif

#ifdef TEST_WRITE
#define TEST_OP(e1, e2) e1 = e2
#endif

#ifdef TEST_INCREMENT
#define TEST_OP(e1, e2) (e1)++
#endif

#ifdef TEST_COMPOUND_ASSIGN
#define TEST_OP(e1, e2) e1 -= e2;
#endif

int g_zero = 0;

void passing_test_1(void);
void passing_test_2(array_ptr<int> a : count(1));
void passing_test_3(array_ptr<int> a : count(len), int len);

void failing_test_1(int k);
void failing_test_2(int k);
void failing_test_3(array_ptr<int> a : count(g_zero));
void failing_test_4(array_ptr<int> a : count(len), int len);

// Handle an out-of-bounds reference by immediately exiting. This causes
// some output to be missing.
void handle_error(int err) {
  _Exit(0);
}

// This signature for main is exactly what we want here,
// it also means any uses of argv[i] are checked too!
int main(int argc, array_ptr<char*> argv : count(argc)) {

  // Set up the handler for a failing bounds check.  Currently the Checked C
  // clang implementation raises a SIGILL or SIGTRAP when a bounds check fails,
  // depending on the target platform.  This may change in the future.
  signal(SIGILL, handle_error);
#if defined(__APPLE__) && defined(__aarch64__)
  signal(SIGTRAP, handle_error);
#endif

  // This makes sure output is not buffered for when
  // we hit errors.
  int err = setvbuf(stdout, NULL, _IONBF, 0);
  if (err) {
    // CHECK-NOT: Error Setting Up Buffering
    puts("Error Setting Up Buffering");
    return EXIT_FAILURE;
  }

  if (argc < 2) {
    // CHECK-NOT: Requires Argument
    puts("Requires Argument");
    return EXIT_FAILURE;
  }

  int a checked[10] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };

  // CHECK: Starting Test
  puts("Starting Test");

  if (strcmp(argv[1], "pass1") == 0) {
    // PASS-1-READ: Result: 7
    // PASS-1-READ: Expected Success
    // PASS-1-WRITE: Result: 1
    // PASS-1-WRITE: Expected Success
    // PASS-1-INCREMENT: Result: 8
    // PASS-1-INCREMENT: Expected Success
    // PASS-1-COMPOUND-ASSIGN: Result: 6
    // PASS-1-COMPOUND-ASSIGN: Expected Success
    passing_test_1();
  }
  else if (strcmp(argv[1], "pass2") == 0) {
    // PASS-2-READ: Result: 0
    // PASs-2-READ: Expected Success
    // PASS-2-WRITE: Result: 2
    // PASs-2-WRITE: Expected Success
    // PASS-2-INCREMENT: Result: 1
    // PASS-2-INCREMENT: Expected Success
    // PASS-2-COMPOUND-ASSIGN: Result: -2
    // PASS-2-COMPOUND-ASSIGN: Expected Success
    passing_test_2(a);
  }
  else if (strcmp(argv[1], "pass3") == 0) {
    // PASS-3-READ: Result: 0
    // PASS-3-READ: Expected Success
    // PASS-3-WRITE: Result: 3
    // PASS-3-WRITE: Expected Success
    // PASS-3-INCREMENT: Result: 1
    // PASS-3-INCREMENT: Expected Success
    // PASS-3-COMPOUND-ASSIGN: Result: -3
    // PASS-3-COMPOUND-ASSIGN: Expected Success
    passing_test_3(a, 3);
  }
  else if (strcmp(argv[1], "fail1") == 0) {
    // FAIL-1-NOT: Unreachable
    // FAIL-1-NOT: Unexpected Success
    failing_test_1(0);
  }
  else if (strcmp(argv[1], "fail2") == 0) {
    // FAIL-2-NOT: Unreachable
    // FAIL-2-NOT: Unexpected Success
    failing_test_2(2);
  }
  else if (strcmp(argv[1], "fail3") == 0) {
    // FAIL-3-NOT: Unreachable
    // FAIL-3-NOT: Unexpected Success
    array_ptr<int> p : count(g_zero) = dynamic_bounds_cast<array_ptr<int>>(a, count(g_zero));
    failing_test_3(p);
  }
  else if (strcmp(argv[1], "fail4") == 0) {
    // FAIL-4-NOT: Unreachable
    // FAIL-4-NOT: Unexpected Success
    failing_test_4(a, 0);
  }
  else {
    // CHECK-NOT: Unexpected Test Name
    puts("Unexpected Test Name");
    return EXIT_FAILURE;
  }

  // CHECK-PASS: All Dynamic Checks Passed
  // CHECK-FAIL-NOT: All Dynamic Checks Passed
  puts("All Dynamic Checks Passed");

  return EXIT_SUCCESS;
}

// Bounds Describe Valid Pointer, within array, deref is fine
void passing_test_1(void) {
  int a checked[10] = { 9, 8, 7, 6, 5, 4, 3, 2, 1};
  array_ptr<int> b : bounds(a, a + 10) = &a[2];

  TEST_OP(*b, 1);
  printf("Result: %d\n", *b);

  puts("Expected Success");
}

// Bounds Describe valid pointer, within array, deref is fine
void passing_test_2(array_ptr<int> a : count(1)) {
  TEST_OP(*a, 2);
  printf("Result: %d\n", *a);

  puts("Expected Success");
}

// Bounds describe valid pointer (assuming len > 0), deref is fine
void passing_test_3(array_ptr<int> a : count(len), int len) {
  assert(len > 0);

  TEST_OP(*a, 3);
  printf("Result: %d\n", *a);

  puts("Expected Success");
}

// Bounds describe empty range (function is called with k=0), no deref
void failing_test_1(int k) {
  int a checked[2] = { 0, 0 };
  array_ptr<int> b : bounds(a, a + k) = dynamic_bounds_cast<array_ptr<int>>(a, count(k));

  TEST_OP(*b, 1);
  printf("Unreachable: %d\n", *b);

  puts("Unexpected Success");
}

// Bounds describe invalid range (function is called with k=2, a + 2 > a), no deref
void failing_test_2(int k) {
  int a checked[3] = { 0, 0, 0 };
  array_ptr<int> c : bounds(a + k, a) = dynamic_bounds_cast<array_ptr<int>>(a, bounds(a + k, a));
  array_ptr<int> b : bounds(a + k, a) = c;
  TEST_OP(*b, 2);
  printf("Unreachable: %d\n", *b);

  puts("Unexpected Success");
}

// Bounds describe empty range, no deref
void failing_test_3(array_ptr<int> a : count(g_zero)) {
  TEST_OP(*a, 3);
  printf("Unreachable: %d\n", *a);

  puts("Unexpected Success");
}

// Bounds describe empty range (assuming len == 0), no deref
void failing_test_4(array_ptr<int> a : count(len), int len) {
  assert(len == 0);

  TEST_OP(*a, 4);
  printf("Unreachable: %d\n", *a);

  puts("Unexpected Success");
}
