// The following lines are for the clang automated test suite
//
// RUN: %clang %s -o %t -Werror
// RUN: %t pass1 | FileCheck %s --check-prefixes=CHECK,CHECK-PASS,CHECK-PASS-1
// RUN: %t pass2 | FileCheck %s --check-prefixes=CHECK,CHECK-PASS,CHECK-PASS-2
// RUN: %t pass3 | FileCheck %s --check-prefixes=CHECK,CHECK-PASS,CHECK-PASS-3
// RUN: %t fail1 | FileCheck %s --check-prefixes=CHECK,CHECK-FAIL,CHECK-FAIL-1
// RUN: %t fail2 | FileCheck %s --check-prefixes=CHECK,CHECK-FAIL,CHECK-FAIL-2
// RUN: %t fail3 | FileCheck %s --check-prefixes=CHECK,CHECK-FAIL,CHECK-FAIL-3
// RUN: %t fail4 | FileCheck %s --check-prefixes=CHECK,CHECK-FAIL,CHECK-FAIL-4
// RUN: %t fail5 | FileCheck %s --check-prefixes=CHECK,CHECK-FAIL,CHECK-FAIL-5
// RUN: %t fail6 | FileCheck %s --check-prefixes=CHECK,CHECK-FAIL,CHECK-FAIL-6
// RUN: %t fail7 | FileCheck %s --check-prefixes=CHECK,CHECK-FAIL,CHECK-FAIL-7
// RUN: %t fail8 | FileCheck %s --check-prefixes=CHECK,CHECK-FAIL,CHECK-FAIL-8

#include <assert.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../../include/stdchecked.h"

void passing_test_1(void);
void passing_test_2(void);
void passing_test_3(void);

void failing_test_1(void);
void failing_test_2(void);
void failing_test_3(void);
void failing_test_4(int k);
void failing_test_5(array_ptr<char> pc : count(len), unsigned len);
void failing_test_6(char* _Array pc  _Count(len), unsigned len);
void failing_test_7(array_ptr<char> pc : count(len), unsigned len);
void failing_test_8(unsigned len);

// Handle an out-of-bounds reference by immediately exiting. This causes
// some output to be missing.
void handle_error(int err) {
  _Exit(0);
}

// This signature for main is exactly what we want here,
// it also means any uses of argv[i] are checked too!
int main(int argc, char**_Array argv  _Count(argc)) {

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

  int a checked[10] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };

  // CHECK: Starting Test
  puts("Starting Test");

  if (strcmp(argv[1], "pass1") == 0) {
    // CHECK-PASS-1: Printable0
    // CHECK-PASS-1: Printable1
    // CHECK-PASS-1: Printable2
    // CHECK-PASS-1: Printable3
    // CHECK-PASS-1: Printable4
    // CHECK-PASS-1: Expected Success
    passing_test_1();
  }
  else if (strcmp(argv[1], "pass2") == 0) {
    // CHECK-PASS-2: Printable0
    // CHECK-PASS-2: Printable1
    // CHECK-PASS-2: Printable2
    // CHECK-PASS-2: Printable3
    // CHECK-PASS-2: Expected Success
    passing_test_2();
  }
  else if (strcmp(argv[1], "pass3") == 0) {
    // CHECK-PASS-3: Passed p1
    // CHECK-PASS-3: Passed p2
    // CHECK-PASS-3: Passed p3
    // CHECK-PASS-3: Passed p4
    // CHECK-PASS-3: Expected Success
    passing_test_3();
  } else if (strcmp(argv[1], "fail1") == 0) {
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
    // CHECK-FAIL-3 : Printable0
    // CHECK-FAIL-3 : Printable1
    // CHECK-FAIL-3 : Printable2
    // CHECK-FAIL-3-NOT: Unprintable
    // CHECK-FAIL-3-NOT: Unexpected Success
    failing_test_3();
  } else if (strcmp(argv[1], "fail4") == 0) {
    // CHECK-FAIL-4 : Printable1
    // CHECK-FAIL-4-NOT: Unprintable2
    // CHECK-FAIL-4-NOT: Unexpected Success
    failing_test_4(5);
  } else if (strcmp(argv[1], "fail5") == 0) {
    char* _Array p  _Count(12) = "\0\0\0\0\0\0\0\0abcd";
    // CHECK-FAIL-5: Successful pointer conversion
    // CHECK-FAIL-5-NOT: Unexpected Success
    _Static_assert(sizeof(int) <= 8, "update test for integers larger than 64 bits.");
    failing_test_5(p, sizeof(int) + 1);
    failing_test_5(p, sizeof(int) - 1);
  } else if (strcmp(argv[1], "fail6") == 0) {
    array_ptr<char> p : count(4) = "abcd";
    // CHECK-FAIL-6: Successful conversion to ptr<void>
    // CHECK-FAIL-6-NOT: Unexpected Success
    failing_test_6(p, 1);
    failing_test_6(p, 0);
  } else if (strcmp(argv[1], "fail7") == 0) {
    char* _Array p  _Count(4) = "abcd";
    // CHECK-FAIL-7: Successful conversion to void *
    // CHECK-FAIL-7-NOT: Unexpected Success
    failing_test_7(p, 1);
    failing_test_7(p, 0);
  } else if (strcmp(argv[1], "fail8") == 0) {
    // CHECK-FAIL-8: Successful conversion to nt_array_ptr<const char>
    // CHECK-FAIL-8-NOT: Unexpected Success
    failing_test_8(5);
    failing_test_8(7);
  } else {
    // CHECK-NOT: Unexpected Test Name
    puts("Unexpected Test Name");
    return EXIT_FAILURE;
  }

  // CHECK-PASS: All Dynamic Checks Passed
  // CHECK-FAIL-NOT: All Dynamic Checks Passed
  puts("All Dynamic Checks Passed");

  return EXIT_SUCCESS;
}

// dynamic_check(r != NULL) && dynamic_check(r <= r && r+1 <= r+10) - > OK
// dynamic_check(r != NULL) && dynamic_check(r <= r && r+3 <= r+10) - > OK
// dynamic_check((r+3) != NULL) && dynamic_check(r <= r+3 && r+6 <= r+10) - > OK
// dynamic_check((r+3) != NULL) && dynamic_check(r <= r && r+15 <= r+10) - > OK
// dynamic_check("abcdef", count(2)) -> OK.
void passing_test_1(void) {
  int* _Single q = 0;
  int r checked[10] = {0,1,2,3,4,5,6,7,8,9};
  int* _Array s  _Count(5) = r;

  q = _Dynamic_bounds_cast<ptr<int>>(r);
  printf("Printable0\n");

  q = _Dynamic_bounds_cast_M(int* _Array, r, _Count(3));
  printf("Printable1\n");

  q = _Dynamic_bounds_cast<array_ptr<int>>(r+3, count(3));
  printf("Printable2\n");

  q = _Dynamic_bounds_cast_M(int* _Array, r,  _Bounds(s, s+3));
  printf("Printable3\n");

  const char* _Nt_array p  _Count(2) =
      _Dynamic_bounds_cast<nt_array_ptr<const char>>("abcdef", _Count(2));
  printf("Printable4\n");

  puts("Expected Success");
}

// dynamic_check(base == NULL || ...)
// dynamic_check(base == NULL || ...)
void passing_test_2(void) {
  ptr<int> q = 0;
  int r checked[10] = {0,1,2,3,4,5,6,7,8,9};
  array_ptr<int> s : count(5) = r;

  // constant folded
  q = _Dynamic_bounds_cast<ptr<int>>(NULL);
  printf("Printable0\n");

  q = _Dynamic_bounds_cast_M(int*_Array, NULL,  _Bounds(r, r+5));
  printf("Printable1\n");

  s = NULL;
  q = _Dynamic_bounds_cast<ptr<int>>(s);
  printf("Printable2\n");

  q = _Dynamic_bounds_cast_M(int* _Array, s, _Bounds(r, r+5));
  printf("Printable3\n");

  puts("Expected Success");
}

// Test dynamic checks involving conversions to void pointers.
void passing_test_3(void) {
  int i = 10;
  ptr<int> pi = &i;
  ptr<void> pv = pi;
  void *unchecked_pv = 0;
  void* _Single p1 = _Dynamic_bounds_cast_M_N(void* _Single, pi);
  printf("Passed p1");
  ptr<void> p2 = _Dynamic_bounds_cast<ptr<void>>(pv);
  printf("Passed p2");
  unchecked_pv = _Dynamic_bounds_cast<void *>(pi);
  printf("Passed unchecked_pv");
  ptr<void> p3 = _Assume_bounds_cast<ptr<void>>(unchecked_pv);
  printf("Passed p3");
  void *p4 = _Assume_bounds_cast_M_N(void *, unchecked_pv);
  printf("Passed p4");
  puts("Expected Success");
}

// dynamic_check(r != NULL) && dynamic_check(r <= r && r+15 <= r+10) -> FAIL
void failing_test_1(void) {
  ptr<int> q = 0;
  int r checked[10] = {0,1,2,3,4,5,6,7,8,9};
  q = _Dynamic_bounds_cast_M(int* _Array, r, _Count(15));

  printf("Unprintable\n");

  puts("Unexpected Success");
}

// dynamic_check((r+8) != NULL) && dynamic_check(r <= r+8 && (r+8+3) <= r+10) -> FAIL
void failing_test_2(void) {
  ptr<int> q = 0;
  int r checked[10] = {0,1,2,3,4,5,6,7,8,9};
  q = _Dynamic_bounds_cast_M(int* _Array, r+8, _Count(3));

  printf("Unprintable\n");

  puts("Unexpected Success");
}

// dynamic_check(r != NULL) && dynamic_check(r <= s && s+3 <= r+10) -> FAIL
void failing_test_3(void) {
  ptr<int> q = 0;
  int r checked[10] = {0,1,2,3,4,5,6,7,8,9};
  array_ptr<int> s : count(5) = r;

  q = _Dynamic_bounds_cast_M_N(int* _Single, r);
  printf("Printable0\n");

  q = _Dynamic_bounds_cast<array_ptr<int>>(r, count(5));
  printf("Printable1\n");

  q = _Dynamic_bounds_cast_M(int* _Array, r,  _Bounds(s, s+3));
  printf("Printable2\n");

  s = 0;
  q = _Dynamic_bounds_cast<array_ptr<int>>(r, bounds(s, s+3));

  printf("Unprintable\n");

  puts("Unexpected Success");
}

// bounds_cast insert dynamic_check(r <= r && (r+5) <= r+10) -> OK;
// dereference insert dynamic_check(s <= s+5 && (s+5) < s+3) -> FAIL
// k = 5
void failing_test_4(int k) {
  int r checked[10] = {0,1,2,3,4,5,6,7,8,9};
  int* _Array s  _Count(3) = _Dynamic_bounds_cast_M(array_ptr<int>, r, _Count(5));

  printf("Printable1\n");
  printf("Unprintable2: %d\n", *(s+k));

  puts("Unexpected Success");
}

// TEst dynamic checks involving possibly failig conversions to ptr<int>.
void failing_test_5(array_ptr<char> pc : count(len), unsigned len) {
  int* _Single pi = _Dynamic_bounds_cast_M_N(int* _Single, pc);
  if (len < sizeof(int))
    puts("Unexpected Success");
  else if (*pi == 0)
    puts("Successful pointer conversion");
}

// Test dynamic checks involving possibly failing conversions to ptr<void>.
void failing_test_6(array_ptr<char> pc : count(len), unsigned len) {
  ptr<void> pv = _Dynamic_bounds_cast<ptr<void>>(pc);
  if (len == 0)
    puts("Unexpected Success");
  else if (pv != 0)
    puts("Successful conversion to ptr<void>");
}

// Test dynamic checks involving possibly failing conversions to void *.
void failing_test_7(array_ptr<char> pc : count(len), unsigned len) {
 void *pv = _Dynamic_bounds_cast<void *>(pc);
  if (len == 0)
    puts("Unexpected Success");
  else if (pv != 0)
    puts("Successful conversion to void *");
}

// Test dynamic checks involving possibly failing conversions from
// string literals.
void failing_test_8(unsigned len) {
  const char* _Nt_array p  _Count(len) =
      _Dynamic_bounds_cast_M(const char *_Nt_array, "123456", _Count(len));
  if (len > 6)
    puts("Unexpected Success");
  else if ((len == 0 && p != 0) || *p == '1')
    puts("Successful conversion to nt_array_ptr<const char>");
}
