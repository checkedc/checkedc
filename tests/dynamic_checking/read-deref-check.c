// Tests that the Dereference Operator works with dynamic checks
//
// The following lines are for the clang automated test suite
//
// RUN: %clang -fcheckedc-extension %s -o %t -Werror
// RUN: %t pass1 | FileCheck %s --check-prefixes=CHECK,CHECK-PASS,CHECK-PASS-1
// RUN: %t pass2 | FileCheck %s --check-prefixes=CHECK,CHECK-PASS,CHECK-PASS-2
// RUN: %t fail1 | FileCheck %s --check-prefixes=CHECK,CHECK-FAIL,CHECK-FAIL-1
// RUN: %t fail2 | FileCheck %s --check-prefixes=CHECK,CHECK-FAIL,CHECK-FAIL-2
// RUN: %t fail3 | FileCheck %s --check-prefixes=CHECK,CHECK-FAIL,CHECK-FAIL-3
// RUN: %t fail4 | FileCheck %s --check-prefixes=CHECK,CHECK-FAIL,CHECK-FAIL-4

#include <assert.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../../include/stdchecked.h"

void passing_test_1(void);
void passing_test_2(array_ptr<int> a : count(1));
void passing_test_3(array_ptr<int> a : count(len), int len);

void failing_test_1(void);
void failing_test_2(void);
void failing_test_3(array_ptr<int> a : count(0));
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

  int a checked[10] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };

  // CHECK: Starting Test
  puts("Starting Test");

  if (strcmp(argv[1], "pass1") == 0) {
    // CHECK-PASS-1: Printable
    // CHECK-PASS-1: Expected Success
    passing_test_1();
  }
  else if (strcmp(argv[1], "pass2") == 0) {
    // CHECK-PASS-2: Printable
    // CHECK-PASS-2: Expected Success
    passing_test_2(a);
  }
  else if (strcmp(argv[1], "pass3") == 0) {
    // CHECK-PASS-3: Printable
    // CHECK-PASS-3: Expected Success
    passing_test_3(a, 3);
  }
  else if (strcmp(argv[1], "fail1") == 0) {
    // CHECK-FAIL-1-NOT: Unprintable
    // CHECK-FAIL-1-NOT: Unexpected Success
    failing_test_1();
  }
  else if (strcmp(argv[1], "fail2") == 0) {
    // CHECK-FAIL-2-NOT: Unprintable
    // CHECK-FAIL-2-NOT: Unexpected Success
    failing_test_2();
  }
  else if (strcmp(argv[1], "fail3") == 0) {
    // CHECK-FAIL-3-NOT: Unprintable
    // CHECK-FAIL-3-NOT: Unexpected Success
    failing_test_3(a);
  }
  else if (strcmp(argv[1], "fail4") == 0) {
    // CHECK-FAIL-4-NOT: Unprintable
    // CHECK-FAIL-4-NOT: Unexpected Success
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
  int a checked[10] = { 0,0,0,0,0,0,0,0,0,0 };
  array_ptr<int> b : count(5) = &a[2];

  printf("Printable: %d\n", *b);

  puts("Expected Success");
}

// Bounds Describe valid pointer, within array, deref is fine
void passing_test_2(array_ptr<int> a : count(1)) {
  printf("Printable: %d\n", *a);

  puts("Expected Success");
}

// Bounds describe valid pointer (assuming len > 0), deref is fine
void passing_test_3(array_ptr<int> a : count(len), int len) {
  assert(len > 0);

  printf("Printable: %d\n", *a);

  puts("Expected Success");
}

// Bounds describe empty range, no deref
void failing_test_1(void) {
  int a checked[2] = { 0, 0 };
  array_ptr<int> b : bounds(a, a) = a;
  
  printf("Unprintable: %d\n", *b);
  
  puts("Unexpected Success");
}

// Bounds describe empty range (a + 2 > a), no deref
void failing_test_2(void) {
  int a checked[3] = { 0, 0, 0 };
  array_ptr<int> b : bounds(a + 2, a) = a;

  printf("Unprintable: %d\n", *b);

  puts("Unexpected Success");
}

// Bounds describe empty range, no deref
void failing_test_3(array_ptr<int> a : count(0)) {
  printf("Unprintable: %d\n", *a);

  puts("Unexpected Success");
}

// Bounds describe empty range (assuming len == 0), no deref
void failing_test_4(array_ptr<int> a : count(len), int len) {
  assert(len == 0);

  printf("Unprintable: %d\n", *a);

  puts("Unexpected Success");
}
