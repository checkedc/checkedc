// Tests that writing through pointers obtained using members accessed via
// the dot operator works with dynamic checks.
//
// The following lines are for the clang automated test suite
//
// RUN: %clang -fcheckedc-extension %s -o %t -Werror
// RUN: %t pass1 | FileCheck %s --check-prefixes=CHECK,CHECK-PASS,CHECK-PASS-1
// RUN: %t pass2 | FileCheck %s --check-prefixes=CHECK,CHECK-PASS,CHECK-PASS-2
// RUN: %t pass3 | FileCheck %s --check-prefixes=CHECK,CHECK-PASS,CHECK-PASS-3
// RUN: %t pass4 | FileCheck %s --check-prefixes=CHECK,CHECK-PASS,CHECK-PASS-4
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

struct S1 {
  array_ptr<int> p : count(len);
  int len;
};

struct S2 {
  array_ptr<int> p : bounds(low, high);
  array_ptr<int> low, high;
};

void passing_test_1(void);
void passing_test_2(void);
void passing_test_3(struct S1 s);  // struct member bounds expected to be non-empty
void passing_test_4(struct S2 s);  // struct member bounds expected to be non-empty

void failing_test_1(void);
void failing_test_2(void);
void failing_test_3(struct S1 s);  // struct member bounds expected to be non-empty
void failing_test_4(struct S2 s);  // struct member bounds expected to be non-empty

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
  struct S1 s1_nonempty = { a, 10};
  struct S2 s2_nonempty  = { a + 5, a, a + 10 };

  struct S1 s1_empty = { a, 0 };
  struct S2 s2_empty = { a, a, a };

  // CHECK: Starting Test
  puts("Starting Test");

  if (strcmp(argv[1], "pass1") == 0) {
    // CHECK-PASS-1: Assignable
    // CHECK-PASS-1: Expected Success
    passing_test_1();
  }
  else if (strcmp(argv[1], "pass2") == 0) {
    // CHECK-PASS-2: Assignable
    // CHECK-PASS-2: Expected Success
    passing_test_2();
  }
  else if (strcmp(argv[1], "pass3") == 0) {
    // CHECK-PASS-3: Assignable
    // CHECK-PASS-3: Expected Success
    passing_test_3(s1_nonempty);
  }
  else if (strcmp(argv[1], "pass4") == 0) {
    // CHECK-PASS-4: Assignable
    // CHECK-PASS-4: Expected Success
    passing_test_4(s2_nonempty);
  }
  else if (strcmp(argv[1], "fail1") == 0) {
    // CHECK-FAIL-1-NOT: Unassignable
    // CHECK-FAIL-1-NOT: Unexpected Success
    failing_test_1();
  }
  else if (strcmp(argv[1], "fail2") == 0) {
    // CHECK-FAIL-2-NOT: Unassignable
    // CHECK-FAIL-2-NOT: Unexpected Success
    failing_test_2();
  }
  else if (strcmp(argv[1], "fail3") == 0) {
    // CHECK-FAIL-3-NOT: Unassignable
    // CHECK-FAIL-3-NOT: Unexpected Success
    failing_test_3(s1_empty);
  }
  else if (strcmp(argv[1], "fail4") == 0) {
    // CHECK-FAIL-4-NOT: Unassignable
    // CHECK-FAIL-4-NOT: Unexpected Success
    failing_test_4(s2_empty);
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

// Dereference pointer in struct member with valid bounds given by a count.
void passing_test_1(void) {
  int a checked[10] = { 0,0,0,0,0,0,0,0,0,0 };
  struct S1 s = {a, 10};

  *(s.p + 5) = 1;
  printf("Assignable: %d\n", *(s.p + 5));

  puts("Expected Success");
}

// Dereference ponter in struct member with valid bounds given by a range.
void passing_test_2(void) {
  int a checked[10] = { 0,0,0,0,0,0,0,0,0,0 };
  struct S2 s = { a + 2, a, a + 10 };

  *(s.p) = 2;
  printf("Assignable: %d\n", *(s.p));

  puts("Expected Success");
}

// Dereference pointer in struct member with valid bounds given by a count,
// where the struct is a parameter.
void passing_test_3(struct S1 s) {
  assert(s.len > 0);
  *(s.p) = 3;
  printf("Assignable: %d\n", *(s.p));

  puts("Expected Success");
}

// Dereference pointer in struct member with valid bounds given by a range
// where the struct is a parameter.
void passing_test_4(struct S2 s) {
  assert(s.p >= s.low && s.p < s.high);

  *(s.p) = 4;
  printf("Assignable: %d\n", *(s.p));

  puts("Expected Success");
}

// Struct member bounds describe empty range, no deref
void failing_test_1(void) {
  int a checked[2] = { 0, 0 };
  struct S1 s = {a , 0};

  *(s.p) = 1;
  printf("Unassignable: %d\n", *(s.p));
  
  puts("Unexpected Success");
}

// Struct member bounds describe empty range (a + 2 > a), 
// no deref
void failing_test_2(void) {
  int a checked[3] = { 0, 0, 0 };
  struct S2 s = { a, a + 2, a };

  *(s.p) = 2;
  printf("Unassignable: %d\n", *(s.p));

  puts("Unexpected Success");
}

// Struct member bounds describe empty range, no deref
void failing_test_3(struct S1 s) {
  assert(s.len == 0);

  *(s.p) = 3;
  printf("Unassignable: %d\n", *(s.p));


  puts("Unexpected Success");
}

// Struct member bounds describe empty range, no deref
void failing_test_4(struct S2 s) {
  assert(s.low == s.high);

  *(s.p) = 4;
  printf("Unassignable: %d\n", *(s.p));

  puts("Unexpected Success");
}
