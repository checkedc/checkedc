// Test bounds checking in checked scopes of uses of pointers
// and arrays with bounds-safe interfaces.
//
// RUN: %clang %s -o %t1 -Wno-unused-value %checkedc_target_flags
// RUN: %checkedc_rununder %t1 0 0 | FileCheck %s --check-prefixes=CHECK,NO-BOUNDS-FAILURES
// RUN: %checkedc_rununder %t1 1 0 | FileCheck %s --check-prefixes=CHECK
// RUN: %checkedc_rununder %t1 2 0 | FileCheck %s --check-prefixes=CHECK
// RUN: %checkedc_rununder %t1 3 0 | FileCheck %s --check-prefixes=CHECK
// RUN: %checkedc_rununder %t1 4 0 | FileCheck %s --check-prefixes=CHECK
// RUN: %checkedc_rununder %t1 5 0 | FileCheck %s --check-prefixes=CHECK
// RUN: %checkedc_rununder %t1 6 0 | FileCheck %s --check-prefixes=CHECK
// RUN: %checkedc_rununder %t1 7 0 | FileCheck %s --check-prefixes=CHECK
// RUN: %checkedc_rununder %t1 8 0 | FileCheck %s --check-prefixes=CHECK
// RUN: %checkedc_rununder %t1 9 0 | FileCheck %s --check-prefixes=CHECK
// RUN: %checkedc_rununder %t1 10 0 | FileCheck %s --check-prefixes=CHECK
// RUN: %checkedc_rununder %t1 11 0 | FileCheck %s --check-prefixes=CHECK
// RUN: %checkedc_rununder %t1 12 0 | FileCheck %s --check-prefixes=CHECK
// RUN: %checkedc_rununder %t1 13 0 | FileCheck %s --check-prefixes=CHECK
// RUN: %checkedc_rununder %t1 14 0 | FileCheck %s --check-prefixes=CHECK
// RUN: %checkedc_rununder %t1 15 0 | FileCheck %s --check-prefixes=CHECK
// RUN: %checkedc_rununder %t1 16 0 | FileCheck %s --check-prefixes=CHECK
// RUN: %checkedc_rununder %t1 17 0 | FileCheck %s --check-prefixes=CHECK
// RUN: %checkedc_rununder %t1 18 0 | FileCheck %s --check-prefixes=CHECK
// RUN: %checkedc_rununder %t1 19 0 | FileCheck %s --check-prefixes=CHECK
// RUN: %checkedc_rununder %t1 20 0 | FileCheck %s --check-prefixes=CHECK
// RUN: %checkedc_rununder %t1 21 0 | FileCheck %s --check-prefixes=CHECK
// RUN: %checkedc_rununder %t1 0 1 | FileCheck %s --check-prefixes=CHECK
// RUN: %checkedc_rununder %t1 0 2 | FileCheck %s --check-prefixes=CHECK
// RUN: %checkedc_rununder %t1 0 3 | FileCheck %s --check-prefixes=CHECK
// RUN: %checkedc_rununder %t1 0 4 | FileCheck %s --check-prefixes=CHECK
// RUN: %checkedc_rununder %t1 0 5 | FileCheck %s --check-prefixes=CHECK
// RUN: %checkedc_rununder %t1 0 6 | FileCheck %s --check-prefixes=CHECK
// RUN: %checkedc_rununder %t1 0 7 | FileCheck %s --check-prefixes=CHECK
// RUN: %checkedc_rununder %t1 0 8 | FileCheck %s --check-prefixes=CHECK
// RUN: %checkedc_rununder %t1 0 9 | FileCheck %s --check-prefixes=CHECK
// RUN: %checkedc_rununder %t1 0 10 | FileCheck %s --check-prefixes=CHECK
// RUN: %checkedc_rununder %t1 0 11 | FileCheck %s --check-prefixes=CHECK
// RUN: %checkedc_rununder %t1 0 12 | FileCheck %s --check-prefixes=CHECK
// RUN: %checkedc_rununder %t1 0 13 | FileCheck %s --check-prefixes=CHECK
// RUN: %checkedc_rununder %t1 0 14 | FileCheck %s --check-prefixes=CHECK
// RUN: %checkedc_rununder %t1 0 15 | FileCheck %s --check-prefixes=CHECK
// RUN: %checkedc_rununder %t1 0 16 | FileCheck %s --check-prefixes=CHECK
// RUN: %checkedc_rununder %t1 0 17 | FileCheck %s --check-prefixes=CHECK
// RUN: %checkedc_rununder %t1 0 18 | FileCheck %s --check-prefixes=CHECK
// RUN: %checkedc_rununder %t1 0 19 | FileCheck %s --check-prefixes=CHECK
// RUN: %checkedc_rununder %t1 0 20 | FileCheck %s --check-prefixes=CHECK
// RUN: %checkedc_rununder %t1 0 21 | FileCheck %s --check-prefixes=CHECK

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

void write_driver(int failure_point, int *a1 : count(10),
                  int *a2 : count(10),
                  int *a3 : count(5),
                  char *b1 : itype(nt_array_ptr<char>) count(10),
                  char *b2 : itype(nt_array_ptr<char>),
                  struct S *s1 : itype(ptr<struct S>));
void write_test(int failure_point, int *p : count(p_len), int p_len,
                int *q : itype(array_ptr<int>) count(q_len), int q_len,
                int *r : itype(int checked[5]), int r_pos,
                char *t : itype(nt_array_ptr<char>) count(t_len), int t_len,
                char *u : itype(nt_array_ptr<char>), int u_pos,
                struct S *s : itype(ptr<struct S>));
void read_driver(int failure_point, int *a1 : count(10),
                 int *a2 : count(10),
                 int *a3 : count(5),
                 char *b1 : itype(nt_array_ptr<char>) count(10),
                 char *b2 : itype(nt_array_ptr<char>),
                 struct S *s1 : itype(ptr<struct S>));
void read_test(int failure_point, int *p : count(p_len), int p_len,
               int *q : itype(array_ptr<int>) count(q_len), int q_len,
               int *r : itype(int checked[5]), int r_pos,
               char *t : itype(nt_array_ptr<char>) count(t_len), int t_len,
               char *u : itype(nt_array_ptr<char>), int u_pos,
               struct S *s : itype(ptr<struct S>));

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

  int write_target = atoi(argv[1]);
  int read_target = atoi(argv[2]);

  int a[10] = { 0, 1, 2, 4, 5, 6, 7, 8, 9 };
  int a2[10] = {0 , 1, 2, 4, 5, 6, 7, 8, 9 };
  int a3[5] = { 4, 3, 2, 1, 0 };
  char b nt_checked[5] = "abcd";
  char b1 nt_checked[11] = "0123456789";
  int tmp[10] = { -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 };
  struct S s1 = { tmp, 10 };

  // CHECK: Starting Test
  puts("Starting Test");
  write_driver(write_target, a, a2, a3, b1, b, &s1);
  if (write_target == 0) {
    // NO-BOUNDS-FAILURES: No bounds failure on write
    puts("No bounds failure on write");
  }
  else {
    // CHECK-NOT: Expected bounds failure
    puts("Expected bounds failure");
  }

  read_driver(read_target, a, a2, a3, b1, b, &s1);
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
void write_driver(int failure_point, int *a1 : count(10),
                  int *a2 : count(10),
                  int *a3 : count(5),
                  char *b1 : itype(nt_array_ptr<char>) count(10),
                  char *b2 : itype(nt_array_ptr<char>),
                  struct S *s1 : itype(ptr<struct S>)) {
  dynamic_check(s1->len >= 5);
  switch (failure_point) {
    // Vary global variable.
    case 0:
      write_test(failure_point, a1, 10, a2, 10, a3, 2, b1, 10, b2, 0, s1);
      break;
    case 1:
      global_arr_len = 0;
      write_test(failure_point, a1, 10, a2, 10, a3, 2, b1, 10, b2, 0, s1);
      break;
    case 2:
      global_arr_len = 1;
      write_test(failure_point, a1, 10, a2, 10, a3, 2, b1, 10, b2, 0, s1);
      break;
    case 3:
      global_arr_len = 2;
      write_test(failure_point, a1, 10, a2, 10, a3, 2, b1, 10, b2, 0, s1);
      break;
    case 4:
      global_arr_len = 3;
      write_test(failure_point, a1, 10, a2, 10, a3, 2, b1, 10, b2, 0, s1);
      break;
    // Vary bounds of a1 (3rd parameter).
    case 5:
      write_test(failure_point, a1, 0, a2, 10, a3, 2, b1, 10, b2, 0, s1);
      break;
    case 6:
      write_test(failure_point, a1, 1, a2, 10, a3, 2, b1, 10, b2, 0, s1);
      break;
    case 7:
      write_test(failure_point, a1, 2, a2, 10, a3, 2, b1, 10, b2, 0, s1);
      break;
    case 8:
      write_test(failure_point, a1, 3, a2, 10, a3, 2, b1, 10, b2, 0, s1);
      break;
    // Vary bounds of a2 (5th parameter).
    case 9:
      write_test(failure_point, a1, 10, a2, 0, a3, 2, b1, 10, b2, 0, s1);
      break;
    case 10:
      write_test(failure_point, a1, 10, a2, 1, a3, 2, b1, 10, b2, 0, s1);
      break;
    case 11:
      write_test(failure_point, a1, 10, a2, 2, a3, 2, b1, 10, b2, 0, s1);
      break;
    case 12:
      write_test(failure_point, a1, 10, a2, 3, a3, 2, b1, 10, b2, 0, s1);
      break;
    // Vary structure lengths.
    case 13:
      s1->f = _Dynamic_bounds_cast<_Array_ptr<int>>(s1->f, count(0)), s1->len = 0;
      write_test(failure_point, a1, 10, a2, 10,  a3, 2, b1, 10, b2, 0, s1);
      break;
    case 14:
      s1->f = _Dynamic_bounds_cast<_Array_ptr<int>>(s1->f, count(1)), s1->len = 1;
      write_test(failure_point, a1, 10, a2, 10, a3, 2, b1, 10, b2, 0, s1);
      break;
    case 15:
      s1->f = _Dynamic_bounds_cast<_Array_ptr<int>>(s1->f, count(2)), s1->len = 2;
      write_test(failure_point, a1, 10, a2, 10,  a3, 2, b1, 10, b2, 0, s1);
      break;
    // Vary write position for a3 (7th parameter)
    case 16:
      write_test(failure_point, a1, 10, a2, 10, a3, -5, b1, 10, b2, 0, s1);
      break;
    // Vary bounds of b1 (9th parameter)
    case 17:
      write_test(failure_point, a1, 10, a2, 10, a3, 2, b1, 1, b2, 0, s1);
      break;
    case 18:
      write_test(failure_point, a1, 10, a2, 10, a3, 2, b1, 2, b2, 0, s1);
      break;
    case 19:
      write_test(failure_point, a1, 10, a2, 10, a3, 2, b1, 3, b2, 0, s1);
      break;
    case 20:
      write_test(failure_point, a1, 10, a2, 10, a3, 2, b1, 4, b2, 0, s1);
      break;
    // Vary write position for b2 (11th parameter)
    case 21:
      write_test(failure_point, a1, 10, a2, 10, a3, 2, b1, 10, b2, 1, s1);
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
// For tests of array_ptrs with variable size, the appropriate parameter,
// global variable, or structure member should be to set to provoke failure:
// - p_len is the bounds for p.
// - global_arr_len is the bounds for global_arr
// - S is pointer to a struct with a pointer whose bounds is
// is given by a member.
//
// We also test array_ptrs with fixed sizes. For those cases,
// we vary the position instead of the size to force a failure.
void write_test(int failure_point, int *p : count(p_len), int p_len,
                int *q : itype(array_ptr<int>) count(q_len), int q_len,
                int *r : itype(int checked[5]), int r_pos,
                char *t : itype(nt_array_ptr<char>) count(t_len), int t_len,
                char *u : itype(nt_array_ptr<char>), int u_pos,
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

  *q = 300;
  if (failure_point == 9) goto unexpected_success;

  *(q + 1) = 301;
  if (failure_point == 10) goto unexpected_success;

  q[2] = 302;
  if (failure_point == 11) goto unexpected_success;

  (q + 1)[2] = 303;
  if (failure_point == 12) goto unexpected_success;

  *(s->f) = 400;
  if (failure_point == 13) goto unexpected_success;

  *(s->f + 1) = 401;
  if (failure_point == 14) goto unexpected_success;

  s->f[2] = 402;
  if (failure_point == 15) goto unexpected_success;

  r[0] = 500;
  r[4] = 501;
  r[r_pos] = -1;
  if (failure_point == 16) goto unexpected_success;

  *t = 'z';
  *(t + 1) = 'a';
  if (failure_point == 17) goto unexpected_success;

  *(t + 2) = 'b';
  if (failure_point == 18) goto unexpected_success;

  t[3] = 'c';
  if (failure_point == 19) goto unexpected_success;

  (t + 1)[3] = 'd';
  if (failure_point == 20) goto unexpected_success;

  u[u_pos] = '\0';
  if (failure_point == 21) goto unexpected_success;

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
void read_driver(int failure_point, int *a1 : count(10),
                 int *a2 : count(10),
                 int *a3 : count(5),
                 char *b1 : itype(nt_array_ptr<char>) count(10),
                 char *b2 : itype(nt_array_ptr<char>),
                 struct S *s1 : itype(ptr<struct S>)) {
  dynamic_check(s1->len >= 5);
  switch (failure_point) {
    // Vary global variable.
    case 0:
      read_test(22, a1, 10, a2, 10, a3, 2, b1, 10, b2, 0, s1);
      break;
    case 1:
      global_arr_len = 0;
      read_test(failure_point, a1, 10, a2, 10, a3, 2, b1, 10, b2, 0, s1);
      break;
    case 2:
      global_arr_len = 1;
      read_test(failure_point, a1, 10, a2, 10, a3, 2, b1, 10, b2, 0, s1);
      break;
    case 3:
      global_arr_len = 2;
      read_test(failure_point, a1, 10, a2, 10, a3, 2, b1, 10, b2, 0, s1);
      break;
    case 4:
      global_arr_len = 3;
      read_test(failure_point, a1, 10, a2, 10, a3, 2, b1, 10, b2, 0, s1);
      break;
    // Vary bounds of a1 (3rd parameter).
    case 5:
      read_test(failure_point, a1, 0, a2, 10, a3, 2, b1, 10, b2, 0, s1);
      break;
    case 6:
      read_test(failure_point, a1, 1, a2, 10, a3, 2, b1, 10, b2, 0, s1);
      break;
    case 7:
      read_test(failure_point, a1, 2, a2, 10, a3, 2, b1, 10, b2, 0, s1);
      break;
    case 8:
      read_test(failure_point, a1, 3, a2, 10, a3, 2, b1, 10, b2, 0, s1);
      break;
    // Vary bounds of a2 (5th parameter).
    case 9:
      read_test(failure_point, a1, 10, a2, 0, a3, 2, b1, 10, b2, 0, s1);
      break;
    case 10:
      read_test(failure_point, a1, 10, a2, 1, a3, 2, b1, 10, b2, 0, s1);
      break;
    case 11:
      read_test(failure_point, a1, 10, a2, 2, a3, 2, b1, 10, b2, 0, s1);
      break;
    case 12:
      read_test(failure_point, a1, 10, a2, 3, a3, 2, b1, 10, b2, 0, s1);
      break;
    // Vary structure lengths.
    case 13:
      s1->f = _Dynamic_bounds_cast<_Array_ptr<int>>(s1->f, count(0)), s1->len = 0,
      read_test(failure_point, a1, 10, a2, 10, a3, 2, b1, 10, b2, 0, s1);
      break;
    case 14:
      s1->f = _Dynamic_bounds_cast<_Array_ptr<int>>(s1->f, count(1)), s1->len = 1;
      read_test(failure_point, a1, 10, a2, 10, a3, 2, b1, 10, b2, 0, s1);
      break;
    case 15:
      s1->f = _Dynamic_bounds_cast<_Array_ptr<int>>(s1->f, count(2)), s1->len = 2;
      read_test(failure_point, a1, 10, a2, 10, a3, 2, b1, 10, b2, 0, s1);
      break;
    // Vary read position for a3 (7th parameter)
    case 16:
      read_test(failure_point, a1, 10, a2, 10, a3, -5, b1, 10, b2, 0, s1);
      break;
    // Vary bounds of b1 (9th parameter). Note that for reads, the length
    // is always one lower than the correponding length for writes.  This
    // is because we can always read the element just at the upper bound
    // for an nt_array_ptr.
    case 17:
      read_test(failure_point, a1, 10, a2, 10, a3, 2, b1, 0, b2, 0, s1);
      break;
    case 18:
      read_test(failure_point, a1, 10, a2, 10, a3, 2, b1, 1, b2, 0, s1);
      break;
    case 19:
      read_test(failure_point, a1, 10, a2, 10, a3, 2, b1, 2, b2, 0, s1);
      break;
    case 20:
      read_test(failure_point, a1, 10, a2, 10, a3, 2, b1, 3, b2, 0, s1);
      break;
    // Vary read position for b (11th parameter)
    case 21:
      read_test(failure_point, a1, 10, a2, 10, a3, 2, b1, 10, b2, 1, s1);
      break;
    default:
      // CHECK-NOT Unexpected test case
      puts("Unexpected test case");
      break;
  }
}

// Like write_test, but does read operations instead.  It also verifies
// that the data read is what is expected to be written by write_test,
// if it succeeds.
void read_test(int failure_point, int *p : count(p_len), int p_len,
               int *q : itype(array_ptr<int>) count(q_len), int q_len,
               int *r : itype(int checked[5]), int r_pos,
               char *t : itype(nt_array_ptr<char>) count(t_len), int t_len,
               char *u : itype(nt_array_ptr<char>), int u_pos,
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

  if (*q != 300) goto fail;
  if (failure_point == 9) goto unexpected_success;

  if (*(q + 1) != 301) goto fail;
  if (failure_point == 10) goto unexpected_success;

  if (q[2] != 302) goto fail;
  if (failure_point == 11) goto unexpected_success;

  if ((q + 1)[2] != 303) goto fail;
  if (failure_point == 12) goto unexpected_success;

  if (*(s->f) != 400) goto fail;
  if (failure_point == 13) goto unexpected_success;

  if (*(s->f + 1) != 401) goto fail;
  if (failure_point == 14) goto unexpected_success;

  if (s->f[2] != 402) goto fail;
  if (failure_point == 15) goto unexpected_success;

  if (r[0] != 500 || r[4] != 501 || r[r_pos] != -1) goto fail;
  if (failure_point == 16) goto unexpected_success;

  if (*t != 'z') goto fail;
  if (*(t + 1) != 'a') goto fail;
  if (failure_point == 17) goto unexpected_success;

  if (*(t + 2) != 'b') goto fail;
  if (failure_point == 18) goto unexpected_success;

  if (t[3] != 'c') goto fail;
  if (failure_point == 19) goto unexpected_success;

  if ((t + 1)[3] != 'd') goto fail;
  if (failure_point == 20) goto unexpected_success;

  if (u[u_pos] != 0) goto fail;
  if (failure_point == 21) goto unexpected_success;

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
