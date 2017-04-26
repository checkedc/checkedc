// Tests that writing through member arrays access via the dot operator
// works with dynamic checks.
//
// The following lines are for the clang automated test suite
//
// RUN: %clang -fcheckedc-extension %s -o %t -Werror
// RUN: %t pass1 | FileCheck %s --check-prefixes=CHECK,CHECK-PASS,CHECK-PASS-1
// RUN: %t pass2 | FileCheck %s --check-prefixes=CHECK,CHECK-PASS,CHECK-PASS-2
// RUN: %t pass3 | FileCheck %s --check-prefixes=CHECK,CHECK-PASS,CHECK-PASS-3
// RUN: %t fail1 | FileCheck %s --check-prefixes=CHECK,CHECK-FAIL,CHECK-FAIL-1
// RUN: %t fail2 | FileCheck %s --check-prefixes=CHECK,CHECK-FAIL,CHECK-FAIL-2
// RUN: %t fail3 | FileCheck %s --check-prefixes=CHECK,CHECK-FAIL,CHECK-FAIL-3

#include <assert.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../../../include/stdchecked.h"

struct S1 {
  array_ptr<int> p : count(len);
  int len;
};

struct S2 {
  int arr checked[10];
  int data;
};

void passing_test_1(int i);
void passing_test_2(void);
void passing_test_3(struct S2 s, int i);

void failing_test_1(int i);
void failing_test_3(struct S2 s, int i);

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

  struct S2 s1 = { { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 }, -1 };
  
  // CHECK: Starting Test
  puts("Starting Test");

  if (strcmp(argv[1], "pass1") == 0) {
    // CHECK-PASS-1: Assignable: 1
    // CHECK-PASS-1: Expected Success
    passing_test_1(5);
  }
  else if (strcmp(argv[1], "pass2") == 0) {
    // CHECK-PASS-2: Assignable: 2
    // CHECK-PASS-2: Expected Success
    passing_test_2();
  }
  else if (strcmp(argv[1], "pass3") == 0) {
    // CHECK-PASS-3: Assignable: 3
    // CHECK-PASS-3: Expected Success
    passing_test_3(s1, 5);
  }
  else if (strcmp(argv[1], "fail1") == 0) {
    // CHECK-FAIL-1-NOT: Unassignable
    // CHECK-FAIL-1-NOT: Unexpected Success
    failing_test_1(11);
  }
  else if (strcmp(argv[1], "fail2") == 0) {
    // CHECK-FAIL-2-NOT: Unassignable
    // CHECK-FAIL-2-NOT: Unexpected Success
    failing_test_1(-1);
  }
  else if (strcmp(argv[1], "fail3") == 0) {
    // CHECK-FAIL-3-NOT: Unassignable
    // CHECK-FAIL-3-NOT: Unexpected Success
    failing_test_3(s1, 11);
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

// Subscript struct member array
void passing_test_1(int i) {
  assert(0 <= i && i < 10);
  struct S2 s = { { 9, 8, 7, 6, 5, 4, 3, 2, 1, 0 }, 10};
  s.arr[i] = 1;
  printf("Assignable: %d\n", s.arr[i]);

  puts("Expected Success");
}

// Dereference struct member array
void passing_test_2(void) {
  struct S2 s = { { 9, 8, 7, 6, 5, 4, 3, 2, 1, 0 }, 10 };
  *(s.arr) = 2;
  printf("Assignable: %d\n", *(s.arr));

  puts("Expected Success");
}

// Subscript struct member array passed
// as an argument.
void passing_test_3(struct S2 s, int i) {
  assert(0 <= i && i < 10);
  s.arr[i] = 3;
  printf("Assignable: %d\n", s.arr[i]);

  puts("Expected Success");
}

// Out-of-bounds dereference of struct member
// array
void failing_test_1(int i) { 
  assert(i <= 0 || i >= 10);
  struct S2 s = { { 9, 8, 7, 6, 5, 4, 3, 2, 1, 0 }, 10 };
  s.arr[i] = 1;
  printf("Unassignable: %d\n", s.arr[i]);
  
  puts("Unexpected Success");
}

void failing_test_3(struct S2 s, int i) {
  assert(i <= 0 || i >= 10);
  s.arr[i] = 1;
  printf("Unassignable: %d\n", s.arr[i]);

  puts("Unexpected Success");
}
