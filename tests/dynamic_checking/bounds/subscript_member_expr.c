// Tests that operations involving member arrays accessed using the arrow operators
// work with dynamic checks.
//
// The following lines are for the clang automated test suite
//
// -----------------------------------------------------------------------------------
// Arrow operator versions of tests
//
// These have the preprocessor symbol ARROW_OPERATOR defined..
// -----------------------------------------------------------------------------------
//
// RUN: %clang -fcheckedc-extension %s -o %t1 -DTEST_READ -DARROW_OPERATOR -Werror
// RUN: %t1 pass1 | FileCheck %s --check-prefixes=CHECK,CHECK-PASS,ARROW,PASS-1-READ
// RUN: %t1 pass2 | FileCheck %s --check-prefixes=CHECK,CHECK-PASS,ARROW,PASS-2-READ
// RUN: %t1 pass3 | FileCheck %s --check-prefixes=CHECK,CHECK-PASS,ARROW,PASS-3-READ
// RUN: %t1 pass4 | FileCheck %s --check-prefixes=CHECK,CHECK-PASS,ARROW,PASS-4-READ
// RUN: %t1 fail1 | FileCheck %s --check-prefixes=CHECK,CHECK-FAIL,ARROW,FAIL-1
// RUN: %t1 fail2 | FileCheck %s --check-prefixes=CHECK,CHECK-FAIL,ARROW,FAIL-2
// RUN: %t1 fail3 | FileCheck %s --check-prefixes=CHECK,CHECK-FAIL,ARROW,FAIL-3
// RUN: %t1 fail4 | FileCheck %s --check-prefixes=CHECK,CHECK-FAIL,ARROW,FAIL-4
// RUN: %t1 fail5 | FileCheck %s --check-prefixes=CHECK,CHECK-FAIL,ARROW,FAIL-5
//
// RUN: %clang -fcheckedc-extension %s -o %t2 -DTEST_WRITE -DARROW_OPERATOR -Werror
// RUN: %t2 pass1 | FileCheck %s --check-prefixes=CHECK,CHECK-PASS,ARROW,PASS-1-WRITE
// RUN: %t2 pass2 | FileCheck %s --check-prefixes=CHECK,CHECK-PASS,ARROW,PASS-2-WRITE
// RUN: %t2 pass3 | FileCheck %s --check-prefixes=CHECK,CHECK-PASS,ARROW,PASS-3-WRITE
// RUN: %t2 pass4 | FileCheck %s --check-prefixes=CHECK,CHECK-PASS,ARROW,PASS-4-WRITE
// RUN: %t2 fail1 | FileCheck %s --check-prefixes=CHECK,CHECK-FAIL,ARROW,FAIL-1
// RUN: %t2 fail2 | FileCheck %s --check-prefixes=CHECK,CHECK-FAIL,ARROW,FAIL-2
// RUN: %t2 fail3 | FileCheck %s --check-prefixes=CHECK,CHECK-FAIL,ARROW,FAIL-3
// RUN: %t2 fail4 | FileCheck %s --check-prefixes=CHECK,CHECK-FAIL,ARROW,FAIL-4
// RUN: %t2 fail5 | FileCheck %s --check-prefixes=CHECK,CHECK-FAIL,ARROW,FAIL-5

// RUN: %clang -fcheckedc-extension %s -o %t3 -DTEST_INCREMENT -DARROW_OPERATOR -Werror
// RUN: %t3 pass1 | FileCheck %s --check-prefixes=CHECK,CHECK-PASS,ARROW,PASS-1-INCREMENT
// RUN: %t3 pass2 | FileCheck %s --check-prefixes=CHECK,CHECK-PASS,ARROW,PASS-2-INCREMENT
// RUN: %t3 pass3 | FileCheck %s --check-prefixes=CHECK,CHECK-PASS,ARROW,PASS-3-INCREMENT
// RUN: %t3 pass4 | FileCheck %s --check-prefixes=CHECK,CHECK-PASS,ARROW,PASS-4-INCREMENT
// RUN: %t3 fail1 | FileCheck %s --check-prefixes=CHECK,CHECK-FAIL,ARROW,FAIL-1
// RUN: %t3 fail2 | FileCheck %s --check-prefixes=CHECK,CHECK-FAIL,ARROW,FAIL-2
// RUN: %t3 fail3 | FileCheck %s --check-prefixes=CHECK,CHECK-FAIL,ARROW,FAIL-3
// RUN: %t3 fail4 | FileCheck %s --check-prefixes=CHECK,CHECK-FAIL,ARROW,FAIL-4
// RUN: %t3 fail5 | FileCheck %s --check-prefixes=CHECK,CHECK-FAIL,ARROW,FAIL-5

// RUN: %clang -fcheckedc-extension %s -o %t4 -DTEST_COMPOUND_ASSIGN -DARROW_OPERATOR -Werror
// RUN: %t4 pass1 | FileCheck %s --check-prefixes=CHECK,CHECK-PASS,ARROW,PASS-1-COMPOUND-ASSIGN
// RUN: %t4 pass2 | FileCheck %s --check-prefixes=CHECK,CHECK-PASS,ARROW,PASS-2-COMPOUND-ASSIGN
// RUN: %t4 pass3 | FileCheck %s --check-prefixes=CHECK,CHECK-PASS,ARROW,PASS-3-COMPOUND-ASSIGN
// RUN: %t4 pass4 | FileCheck %s --check-prefixes=CHECK,CHECK-PASS,ARROW,PASS-4-COMPOUND-ASSIGN
// RUN: %t4 fail1 | FileCheck %s --check-prefixes=CHECK,CHECK-FAIL,ARROW,FAIL-1
// RUN: %t4 fail2 | FileCheck %s --check-prefixes=CHECK,CHECK-FAIL,ARROW,FAIL-2
// RUN: %t4 fail3 | FileCheck %s --check-prefixes=CHECK,CHECK-FAIL,ARROW,FAIL-3
// RUN: %t4 fail4 | FileCheck %s --check-prefixes=CHECK,CHECK-FAIL,ARROW,FAIL-4
// RUN: %t4 fail5 | FileCheck %s --check-prefixes=CHECK,CHECK-FAIL,ARROW,FAIL-5

// -----------------------------------------------------------------------------------
// Dot operator versions of tests
//
// These are just cut-and-pasted versions of the prior section, with -DARROW_OPERATOR
// removed and new temporary names used.
// -----------------------------------------------------------------------------------

// RUN: %clang -fcheckedc-extension %s -o %t5 -DTEST_READ -Werror
// RUN: %t5 pass1 | FileCheck %s --check-prefixes=CHECK,CHECK-PASS,PASS-1-READ
// RUN: %t5 pass2 | FileCheck %s --check-prefixes=CHECK,CHECK-PASS,PASS-2-READ
// RUN: %t5 pass3 | FileCheck %s --check-prefixes=CHECK,CHECK-PASS,PASS-3-READ
// RUN: %t5 pass4 | FileCheck %s --check-prefixes=CHECK,CHECK-PASS,PASS-4-READ
// RUN: %t5 fail1 | FileCheck %s --check-prefixes=CHECK,CHECK-FAIL,FAIL-1
// RUN: %t5 fail2 | FileCheck %s --check-prefixes=CHECK,CHECK-FAIL,FAIL-2
// RUN: %t5 fail3 | FileCheck %s --check-prefixes=CHECK,CHECK-FAIL,FAIL-3
// RUN: %t5 fail4 | FileCheck %s --check-prefixes=CHECK,CHECK-FAIL,FAIL-4
// RUN: %t5 fail5 | FileCheck %s --check-prefixes=CHECK,CHECK-FAIL,FAIL-5
//
// RUN: %clang -fcheckedc-extension %s -o %t6 -DTEST_WRITE -Werror
// RUN: %t6 pass1 | FileCheck %s --check-prefixes=CHECK,CHECK-PASS,PASS-1-WRITE
// RUN: %t6 pass2 | FileCheck %s --check-prefixes=CHECK,CHECK-PASS,PASS-2-WRITE
// RUN: %t6 pass3 | FileCheck %s --check-prefixes=CHECK,CHECK-PASS,PASS-3-WRITE
// RUN: %t6 pass4 | FileCheck %s --check-prefixes=CHECK,CHECK-PASS,PASS-4-WRITE
// RUN: %t6 fail1 | FileCheck %s --check-prefixes=CHECK,CHECK-FAIL,FAIL-1
// RUN: %t6 fail2 | FileCheck %s --check-prefixes=CHECK,CHECK-FAIL,FAIL-2
// RUN: %t6 fail3 | FileCheck %s --check-prefixes=CHECK,CHECK-FAIL,FAIL-3
// RUN: %t6 fail4 | FileCheck %s --check-prefixes=CHECK,CHECK-FAIL,FAIL-4
// RUN: %t6 fail5 | FileCheck %s --check-prefixes=CHECK,CHECK-FAIL,FAIL-5

// RUN: %clang -fcheckedc-extension %s -o %t7 -DTEST_INCREMENT -Werror
// RUN: %t7 pass1 | FileCheck %s --check-prefixes=CHECK,CHECK-PASS,PASS-1-INCREMENT
// RUN: %t7 pass2 | FileCheck %s --check-prefixes=CHECK,CHECK-PASS,PASS-2-INCREMENT
// RUN: %t7 pass3 | FileCheck %s --check-prefixes=CHECK,CHECK-PASS,PASS-3-INCREMENT
// RUN: %t7 pass4 | FileCheck %s --check-prefixes=CHECK,CHECK-PASS,PASS-4-INCREMENT
// RUN: %t7 fail1 | FileCheck %s --check-prefixes=CHECK,CHECK-FAIL,FAIL-1
// RUN: %t7 fail2 | FileCheck %s --check-prefixes=CHECK,CHECK-FAIL,FAIL-2
// RUN: %t7 fail3 | FileCheck %s --check-prefixes=CHECK,CHECK-FAIL,FAIL-3
// RUN: %t7 fail4 | FileCheck %s --check-prefixes=CHECK,CHECK-FAIL,FAIL-4
// RUN: %t7 fail5 | FileCheck %s --check-prefixes=CHECK,CHECK-FAIL,FAIL-5

// RUN: %clang -fcheckedc-extension %s -o %t8 -DTEST_COMPOUND_ASSIGN -Werror
// RUN: %t8 pass1 | FileCheck %s --check-prefixes=CHECK,CHECK-PASS,PASS-1-COMPOUND-ASSIGN
// RUN: %t8 pass2 | FileCheck %s --check-prefixes=CHECK,CHECK-PASS,PASS-2-COMPOUND-ASSIGN
// RUN: %t8 pass3 | FileCheck %s --check-prefixes=CHECK,CHECK-PASS,PASS-3-COMPOUND-ASSIGN
// RUN: %t8 pass4 | FileCheck %s --check-prefixes=CHECK,CHECK-PASS,PASS-4-COMPOUND-ASSIGN
// RUN: %t8 fail1 | FileCheck %s --check-prefixes=CHECK,CHECK-FAIL,FAIL-1
// RUN: %t8 fail2 | FileCheck %s --check-prefixes=CHECK,CHECK-FAIL,FAIL-2
// RUN: %t8 fail3 | FileCheck %s --check-prefixes=CHECK,CHECK-FAIL,FAIL-3
// RUN: %t8 fail4 | FileCheck %s --check-prefixes=CHECK,CHECK-FAIL,FAIL-4
// RUN: %t8 fail5 | FileCheck %s --check-prefixes=CHECK,CHECK-FAIL,FAIL-5

#include <assert.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../../../include/stdchecked.h"

#ifdef TEST_READ
#define TEST_OP(e1, e2)
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

struct S1 {
  array_ptr<int> p : count(len);
  int len;
};

struct S2 {
  int arr checked[10];
  int data;
};

void passing_test_1(int i);
void passing_test_2(int i);
void passing_test_3(void);
#ifdef ARROW_OPERATOR
void passing_test_4(struct S2 *s, int i);
#else
void passing_test_4(struct S2 s, int i);
#endif

void failing_test_1(int i);
void failing_test_2(int i);
#ifdef ARROW_OPERATOR
void failing_test_3(struct S2 *s, int i);
#else
void failing_test_3(struct S2 s, int i);
#endif

// Handle an out-of-bounds reference by immediately exiting. This causes
// some output to be missing.
void handle_error(int err) {
  _Exit(0);
}

// This signature for main is exactly what we want here,
// it also means any uses of argv[i] are checked too!
int main(int argc, array_ptr<char*> argv : count(argc)) {

  // Set up the handler for a failing bounds check.  Currently the Checked C
  // clang implementation raises a SIGILL when a bounds check fails.  This
  // may change in the future.
  signal(SIGILL, handle_error);

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

  int v checked[5] = { 0, 1, 2, 3, 4};
  struct S1 s1 = { v, 5 };
  struct S2 s2 = { { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 }, -1 };
  
  // CHECK: Starting Test
  puts("Starting Test");
#ifdef ARROW_OPERATOR
  puts("Using arrow operator"); // ARROW: Using arrow operator
#else
  puts("Using dot operator");
#endif 


  if (strcmp(argv[1], "pass1") == 0) {
    // PASS-1-READ: Result: 3
    // PASS-1-READ: Expected Success
    // PASS-1-WRITE: Result: 1
    // PASS-1-WRITE: Expected Success
    // PASS-1-INCREMENT: Result: 4
    // PASS-1-INCREMENT: Expected Success
    // PASS-1-COMPOUND-ASSIGN: Result: 2
    // PASS-1-COMPOUND-ASSIGN: Expected Success
    passing_test_1(6);
  }
  else if (strcmp(argv[1], "pass2") == 0) {
    // PASS-2-READ: Result: 4
    // PASs-2-READ: Expected Success
    // PASS-2-WRITE: Result: 1
    // PASs-2-WRITE: Expected Success
    // PASS-2-INCREMENT: Result: 5
    // PASS-2-INCREMENT: Expected Success
    // PASS-2-COMPOUND-ASSIGN: Result: 3
    // PASS-2-COMPOUND-ASSIGN: Expected Success
    passing_test_2(5);
  }
  else if (strcmp(argv[1], "pass3") == 0) {
    // PASS-3-READ: Result: 9
    // PASS-3-READ: Expected Success
    // PASS-3-WRITE: Result: 2
    // PASS-3-WRITE: Expected Success
    // PASS-3-INCREMENT: Result: 10
    // PASS-3-INCREMENT: Expected Success
    // PASS-3-COMPOUND-ASSIGN: Result: 7
    // PASS-3-COMPOUND-ASSIGN: Expected Success
    passing_test_3();
  }
  else if (strcmp(argv[1], "pass4") == 0) {
    // PASS-4-READ: Result: 5
    // PASS-4-READ: Expected Success
    // PASS-4-WRITE: Result: 3
    // PASS-4-WRITE: Expected Success
    // PASS-4-INCREMENT: Result: 6
    // PASS-4-INCREMENT: Expected Success
    // PASS-4-COMPOUND-ASSIGN: Result: 2
    // PASS-4-COMPOUND-ASSIGN: Expected Success
#ifdef ARROW_OPERATOR
    passing_test_4(&s2, 5);
#else
    passing_test_4(s2, 5);
#endif

  }
  else if (strcmp(argv[1], "fail1") == 0) {
    // FAIL-1-NOT: Unreachable
    // FAIL-1-NOT: Unexpected Success
    failing_test_1(11);
  }
  else if (strcmp(argv[1], "fail2") == 0) {
    // FAIL-2-NOT: Unreachable
    // FAIL-2-NOT: Unexpected Success
    failing_test_1(-1);
  }
  else if (strcmp(argv[1], "fail3") == 0) {
    // FAIL-3-NOT: Unreachable
    // FAIL-3-NOT: Unexpected Success
    failing_test_2(11);
  }
  else if (strcmp(argv[1], "fail4") == 0) {
    // FAIL-4-NOT: Unreachable
    // FAIL-4-NOT: Unexpected Success
    failing_test_2(-1);
  }
  else if (strcmp(argv[1], "fail5") == 0) {
    // FAIL-5-NOT: Unreachable
    // FAIL-5-NOT: Unexpected Success
#ifdef ARROW_OPERATOR
    failing_test_3(&s2, 11);
#else
    failing_test_3(s2, 11);
#endif
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

// Subscript struct member pointer
void passing_test_1(int i) {
  assert(0 <= i && i < 10);
  int v checked[10] = { 9, 8, 7, 6, 5, 4, 3, 2, 1, 0 };
  struct S1 s = { v, 10 };
#ifdef ARROW_OPERATOR
  struct S1 *sp = &s;
  TEST_OP(sp->p[i], 1);
  printf("Result: %d\n", sp->p[i]);
#else
  TEST_OP(s.p[i], 1);
  printf("Result: %d\n", s.p[i]);
#endif
  puts("Expected Success");
}

// Subscript struct member array
void passing_test_2(int i) {
  assert(0 <= i && i < 10);
  struct S2 s = { { 9, 8, 7, 6, 5, 4, 3, 2, 1, 0 }, 10};
#ifdef ARROW_OPERATOR
  struct S2 *sp = &s;
  TEST_OP(sp->arr[i], 1);
  printf("Result: %d\n", sp->arr[i]);
#else
  TEST_OP(s.arr[i], 1);
  printf("Result: %d\n", s.arr[i]);
#endif
  puts("Expected Success");
}

// Dereference struct member array
void passing_test_3(void) {
  struct S2 s = { { 9, 8, 7, 6, 5, 4, 3, 2, 1, 0 }, 10 };
#ifdef ARROW_OPERATOR
  struct S2 *sp = &s;
  TEST_OP(*(sp->arr), 2);
  printf("Result: %d\n", *(sp->arr));
#else
  TEST_OP(*(s.arr), 2);
  printf("Result: %d\n", *(s.arr));
#endif
  puts("Expected Success");
}

// Subscript struct member array passed
// as an argument.

#ifdef ARROW_OPERATOR
void passing_test_4(struct S2 *s, int i) {
#else
void passing_test_4(struct S2 s, int i) {
#endif
  assert(0 <= i && i < 10);
#if ARROW_OPERATOR
  TEST_OP(s->arr[i], 3);
  printf("Result: %d\n", s->arr[i]);
#else
  TEST_OP(s.arr[i], 3);
  printf("Result: %d\n", s.arr[i]);
#endif

  puts("Expected Success");
}

// Out-of-bounds dereference of struct member
// pointer
void failing_test_1(int i) {
  assert(i < 0 || i >= 10);
  int v checked[10] = { 9, 8, 7, 6, 5, 4, 3, 2, 1, 0 };
  struct S1 s = { v, 10 };
#ifdef ARROW_OPERATOR
  struct S1 *sp = &s;
  TEST_OP(sp->p[i], 5);
  printf("Unreachable: %d\n", sp->p[i]);
#else
  TEST_OP(s.p[i], 5);
  printf("Unreachable: %d\n", s.p[i]);
#endif
  puts("Unexpected Success");
}

// Out-of-bounds dereference of struct member
// array
void failing_test_2(int i) {
  assert(i < 0 || i >= 10);
  struct S2 s = { { 9, 8, 7, 6, 5, 4, 3, 2, 1, 0 }, 10 };
#ifdef ARROW_OPERATOR
  struct S2 *sp = &s;
  TEST_OP(sp->arr[i], 1);
  printf("Unreachable: %d\n", sp->arr[i]);
#else
  TEST_OP(s.arr[i], 1);
  printf("Unreachable: %d\n", s.arr[i]);
#endif
  
  puts("Unexpected Success");
}

#ifdef ARROW_OPERATOR
void failing_test_3(struct S2 *s, int i) {
#else
void failing_test_3(struct S2 s, int i) {
#endif
  assert(i < 0 || i >= 10);
#ifdef ARROW_OPERATOR
  TEST_OP(s->arr[i], 1);
  printf("Unreachable: %d\n", s->arr[i]);
#else
  TEST_OP(s.arr[i], 1);
  printf("Unreachable: %d\n", s.arr[i]);
#endif

  puts("Unexpected Success");
}
