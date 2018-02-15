// Test bounds checking in checked scopes of uses of pointers
// and arrays with bounds-safe interfaces.
//
// RUN: %clang %s -o %t1 -Wno-unused-value
// RUN:  %t1 0 0 | FileCheck %s --check-prefixes=CHECK,NO-BOUNDS-FAILURES
// RUN:  %t1 1 0 | FileCheck %s --check-prefixes=CHECK
// RUN:  %t1 2 0 | FileCheck %s --check-prefixes=CHECK
// RUN:  %t1 3 0 | FileCheck %s --check-prefixes=CHECK
// RUN:  %t1 4 0 | FileCheck %s --check-prefixes=CHECK
// RUN:  %t1 5 0 | FileCheck %s --check-prefixes=CHECK
// RUN:  %t1 6 0 | FileCheck %s --check-prefixes=CHECK
// RUN:  %t1 7 0 | FileCheck %s --check-prefixes=CHECK
// RUN:  %t1 8 0 | FileCheck %s --check-prefixes=CHECK
// RUN:  %t1 9 0 | FileCheck %s --check-prefixes=CHECK
// RUN:  %t1 10 0 | FileCheck %s --check-prefixes=CHECK
// RUN:  %t1 11 0 | FileCheck %s --check-prefixes=CHECK
// RUN:  %t1 12 0 | FileCheck %s --check-prefixes=CHECK
// RUN:  %t1 0 1 | FileCheck %s --check-prefixes=CHECK
// RUN:  %t1 0 2 | FileCheck %s --check-prefixes=CHECK
// RUN:  %t1 0 3 | FileCheck %s --check-prefixes=CHECK
// RUN:  %t1 0 4 | FileCheck %s --check-prefixes=CHECK
// RUN:  %t1 0 5 | FileCheck %s --check-prefixes=CHECK
// RUN:  %t1 0 6 | FileCheck %s --check-prefixes=CHECK
// RUN:  %t1 0 7 | FileCheck %s --check-prefixes=CHECK
// RUN:  %t1 0 8 | FileCheck %s --check-prefixes=CHECK
// RUN:  %t1 0 9 | FileCheck %s --check-prefixes=CHECK
// RUN:  %t1 0 10 | FileCheck %s --check-prefixes=CHECK
// RUN:  %t1 0 11 | FileCheck %s --check-prefixes=CHECK
// RUN:  %t1 0 12 | FileCheck %s --check-prefixes=CHECK


#include <assert.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdchecked.h>

// Handle an out-of-bounds reference by immediately exiting. This causes
// some output to be missing.
void handle_error(int err) {
  _Exit(0);
}

int global_arr_len = 5;
int global_arr[5] : count(global_arr_len);
struct S {
  int *f : count(len);
  int len;
};

void write_driver(int failure_point, int *a : count(10),
                  char *b : itype(nt_array_ptr<char>),
                  struct S *s1 : itype(ptr<struct S>));
void write_test(int failure_point, int *p : count(len), int len,
                char *r : itype(nt_array_ptr<char>), int pos,
                struct S *s : itype(ptr<struct S>));
void read_driver(int failure_point, int *a : count(10),
                 char *b : itype(nt_array_ptr<char>),
                 struct S *s1 : itype(ptr<struct S>));
void read_test(int failure_point, int *p : count(len), int len,
               char *r : itype(nt_array_ptr<char>), int pos,
               struct S *s : itype(ptr<struct S>));

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

  int write_target = atoi(argv[1]);
  int read_target = atoi(argv[2]);

  int a[10] = { 0, 1, 2, 4, 5, 6, 7, 8, 9 };
  char b nt_checked[5] = "abcd";
  int tmp[10] = { -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 };
  struct S s1 = { tmp, 10 };

  // CHECK: Starting Test
  puts("Starting Test");
  write_driver(write_target, a, b, &s1);
  if (write_target == 0) {
    // NO-BOUNDS-FAILURES: No bounds failure on write
    puts("No bounds failure on write");
  }
  else {
    // CHECK-NOT: Expected bounds failure
    puts("Expected bounds failure");
  }

  read_driver(read_target, a, b, &s1);
  if (read_target == 0) {
    // NO-BOUNDS-FAILURES: No bounds failure on read
    puts("No bounds failure on read");
  }
  else {
    // CHECK-NOT: Expected bounds failure
    puts("Expected bounds failure");
  }

  return EXIT_SUCCESS;
}

// Invoke write_test, setting up conditions to cause
// a failure at the test specified by failure_point.
// When failure_point is 0, all tests in write_test should pass.
void write_driver(int failure_point, int *a : count(10),
                  char *b : itype(nt_array_ptr<char>),
                  struct S *s1 : itype(ptr<struct S>)) {
  dynamic_check(s1->len >= 5);
  switch (failure_point) {
    case 0: 
      write_test(13, a, 10, b, 0, s1);
      break;
    case 1:
      global_arr_len = 0;
      write_test(failure_point, a, 10, b, 0, s1);
      break;
    case 2: 
      global_arr_len = 1;
      write_test(failure_point, a, 10, b, 0, s1);
      break;
    case 3:
      global_arr_len = 2;
      write_test(failure_point, a, 10, b, 0, s1);
      break;
    case 4:
      global_arr_len = 3;
      write_test(failure_point, a, 10, b, 0, s1);
      break;
    case 5:
      write_test(failure_point, a, 0, b, 0, s1);
      break;
    case 6:
      write_test(failure_point, a, 1, b, 0, s1);
      break;
    case 7:
      write_test(failure_point, a, 2, b, 0, s1);
      break;
    case 8:
      write_test(failure_point, a, 3, b, 0, s1);
      break;
    case 9:
      s1->len = 0;
      write_test(failure_point, a, 10, b, 0, s1);
      break;
    case 10:
      s1->len = 1;
      write_test(failure_point, a, 10, b,  0, s1);
      break;
    case 11:
      s1->len = 2;
      write_test(failure_point, a, 10, b, 0, s1);
      break;
    case 12:
      write_test(failure_point, a, 10, b, 1, s1);
      break;
    default:
      // CHECK-NOT Unexpected test case
      puts("Unexpected test case");
      break;

  }
}

// Executes a sequences of tests, expects to fail on the nth test
// specified by failure_point. If failure_point is 0, no tests should
// fail.
//
// For tests of array_ptrs, the appropriate parameter, global variable, or
// structure member should be to set to provoke failure:
// - len is the bounds for p.
// - global_arr_len is the bounds for global_arr
// - S is pointer to a struct with a pointer whose bounds is
// is given by a member.
//
// For nt_array_ptr, we can't specify the length yet. The length
// is inferred to be 0, which means we can read the 0th element and
// write a null value to the the 0th element.  We intead specify
// a position about 0 to write as a means of forcing a failure.
void write_test(int failure_point, int *p : count(len), int len,
                char *r : itype(nt_array_ptr<char>), int pos,
                struct S *s : itype(ptr<struct S>)) checked {
  *global_arr = 100;
  if (failure_point == 1) goto unexpected_success;

  *(global_arr + 1) = 101;
  if (failure_point == 2) goto unexpected_success;

  global_arr[2] = 102;
  if (failure_point == 3) goto unexpected_success;

  (global_arr + 1)[2] = 103;
  if (failure_point == 4) goto unexpected_success;

  *p = 200;
  if (failure_point == 5) goto unexpected_success;

  *(p + 1) = 201;
  if (failure_point == 6) goto unexpected_success;

  p[2] = 202;
  if (failure_point == 7) goto unexpected_success;

  (p + 1)[2] = 203;
  if (failure_point == 8) goto unexpected_success;

  *(s->f) = 300;
  if (failure_point == 9) goto unexpected_success;

  *(s->f + 1) = 301;
  if (failure_point == 10) goto unexpected_success;

  s->f[2] = 302;
  if (failure_point == 11) goto unexpected_success;

  r[pos] = '\0';
  if (failure_point == 12) goto unexpected_success;

  return;

unexpected_success:
  unchecked{
    // CHECK-NOT: Unexpected success
    printf("Unexpected write_test success: expect to stop at %d\n", failure_point);
  }
  exit(EXIT_FAILURE);
  return;
}

// Invoke read_test, setting up conditions to cause
// a failure at the test specified by failure_point.
void read_driver(int failure_point, int *a : count(10),
                 char *b : itype(nt_array_ptr<char>),
                 struct S *s1 : itype(ptr<struct S>)) {
  dynamic_check(s1->len >= 5);
  switch (failure_point) {
    case 0:
      read_test(13, a, 13, b, 0, s1);
      break;
    case 1:
      global_arr_len = 0;
      read_test(failure_point, a, 10, b, 0, s1);
      break;
    case 2:
      global_arr_len = 1;
      read_test(failure_point, a, 10, b, 0, s1);
      break;
    case 3:
      global_arr_len = 2;
      read_test(failure_point, a, 10, b, 0, s1);
      break;
    case 4:
      global_arr_len = 3;
      read_test(failure_point, a, 10, b, 0, s1);
      break;
    case 5:
      read_test(failure_point, a, 0, b, 0, s1);
      break;
    case 6:
      read_test(failure_point, a, 1, b, 0, s1);
      break;
    case 7:
      read_test(failure_point, a, 2, b, 0, s1);
      break;
    case 8:
      read_test(failure_point, a, 3, b, 0, s1);
      break;
    case 9:
      s1->len = 0;
      read_test(failure_point, a, 10, b, 0, s1);
      break;
    case 10:
      s1->len = 1;
      read_test(failure_point, a, 10, b, 0, s1);
      break;
    case 11:
      s1->len = 2;
      read_test(failure_point, a, 10, b, 0, s1);
      break;
    case 12:
      read_test(failure_point, a, 10, b, 1, s1);
      break;
    default:
      // CHECK-NOT Unexpected test case
      puts("Unexpected test case");
      break;

  }
}

// Like write_test, but does read operations instead.  It also verify
// that the data read is what is expected to be written by write_test,
// if it succeeds.
void read_test(int failure_point, int *p : count(len), int len, 
               char *r : itype(nt_array_ptr<char>), int pos,
               struct S *s : itype(ptr<struct S>)) checked {
  if (*global_arr != 100) goto fail;
  if (failure_point == 1) goto unexpected_success;

  if (*(global_arr + 1) != 101) goto fail;
  if (failure_point == 2) goto unexpected_success;

  if (global_arr[2] != 102) goto fail;
  if (failure_point == 3) goto unexpected_success;

  if ((global_arr + 1)[2] != 103) goto fail;
  if (failure_point == 4) goto unexpected_success;

  if (*p != 200) goto fail;
  if (failure_point == 5) goto unexpected_success;

  if (*(p + 1) != 201) goto fail;
  if (failure_point == 6) goto unexpected_success;

  if (p[2] != 202) goto fail;
  if (failure_point == 7) goto unexpected_success;

  if ((p + 1)[2] != 203) goto fail;
  if (failure_point == 8) goto unexpected_success;

  if (*(s->f) != 300) goto fail;
  if (failure_point == 9) goto unexpected_success;

  if (*(s->f + 1) != 301) goto fail;
  if (failure_point == 10) goto unexpected_success;

  if (s->f[2] != 302) goto fail;
  if (failure_point == 11) goto unexpected_success;

  if (r[pos] != 0) goto fail;
  if (failure_point == 12) goto unexpected_success;

  return;

fail:
  unchecked {
    // CHECK-NOT read_test found incorrect data
    puts("read_test found incorrect data");
  }
  exit(EXIT_FAILURE);
  return;

unexpected_success:
  unchecked {
    // CHECK-NOT Unexpected read_test success
    printf("Unexpected read_test success: expected to stop at %d\n", failure_point);
  }
  exit(EXIT_FAILURE);
  return;

}
