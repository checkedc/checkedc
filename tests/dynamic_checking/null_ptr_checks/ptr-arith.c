// Test arithmetic on null pointers.
//
// RUN: %clang %s -o %t1 %checkedc_target_flags

// RUN: %checkedc_rununder %t1 1 -DARR | FileCheck %s --check-prefix FAIL1
// RUN: %checkedc_rununder %t1 2 -DARR | FileCheck %s --check-prefix FAIL2
// RUN: %checkedc_rununder %t1 3 -DARR | FileCheck %s --check-prefix FAIL3
// RUN: %checkedc_rununder %t1 4 -DARR | FileCheck %s --check-prefix FAIL4
// RUN: %checkedc_rununder %t1 5 -DARR | FileCheck %s --check-prefix FAIL5

// RUN: %checkedc_rununder %t1 1 | FileCheck %s --check-prefix FAIL1
// RUN: %checkedc_rununder %t1 2 | FileCheck %s --check-prefix FAIL2
// RUN: %checkedc_rununder %t1 3 | FileCheck %s --check-prefix FAIL3
// RUN: %checkedc_rununder %t1 4 | FileCheck %s --check-prefix FAIL4
// RUN: %checkedc_rununder %t1 5 | FileCheck %s --check-prefix FAIL5

// RUN: %checkedc_rununder %t1 100 -DARR | FileCheck %s --check-prefix PASS1
// RUN: %checkedc_rununder %t1 200 -DARR | FileCheck %s --check-prefix PASS2
// RUN: %checkedc_rununder %t1 300 -DARR | FileCheck %s --check-prefix PASS3

// RUN: %checkedc_rununder %t1 100 | FileCheck %s --check-prefix PASS1
// RUN: %checkedc_rununder %t1 200 | FileCheck %s --check-prefix PASS2
// RUN: %checkedc_rununder %t1 300 | FileCheck %s --check-prefix PASS3

#include <assert.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdchecked.h>

array_ptr<char> arr_p : count(3) = NULL;
nt_array_ptr<char> nt_arr_p : count(3) = NULL;

array_ptr<char> arr_s : count(3) = "abc";
int *t = NULL;

void handle_error(int err) {
  puts("Error: null pointer arithmetic");
  _Exit(0);
}

void fail1() {
  // FAIL1: Error: null pointer arithmetic
#ifdef ARR
  arr_p++;
#else
  nt_arr_p++;
#endif
}

void fail2() {
  // FAIL2: Error: null pointer arithmetic
#ifdef ARR
  arr_p[1]--;
#else
  nt_arr_p[1]--;
#endif
}

void f1(array_ptr<char> a) {}
void f2(nt_array_ptr<char> a) {}

void fail3() {
  // FAIL3: Error: null pointer arithmetic
#ifdef ARR
  f1(--arr_p);
#else
  f2(--nt_arr_p);
#endif
}

void fail4() {
  // FAIL4: Error: null pointer arithmetic
#ifdef ARR
  array_ptr<char> arr_loc : count(3) = NULL;
  arr_loc++;
#else
  nt_array_ptr<char> nt_arr_loc : count(3) = NULL;
  nt_arr_loc++;
#endif
}

void f3(array_ptr<char> a) { a--; }
void f4(nt_array_ptr<char> a) { a--; }
void f5(int *i) { i--; }

void fail5() {
  // FAIL5: Error: null pointer arithmetic
#ifdef ARR
  f3(NULL);
#else
  f4(NULL);
#endif
}

void pass1() {
  // PASS1: PASS
#ifdef ARR
  arr_s++;
#else
  t++;
#endif
  puts("PASS");
}

void pass2() {
  // PASS2: PASS
#ifdef ARR
  array_ptr<char> arr_loc : count(3) = "xyz";
  arr_loc++;
#else
  int *p = NULL;
  p++;
#endif
  puts("PASS");
}

void pass3() {
  // PASS3: PASS
#ifdef ARR
  f3("abc");
#else
  f5(NULL);
#endif
  puts("PASS");
}

int main(int argc, array_ptr<char *> argv : count(argc)) {
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

  int testcase = atoi(argv[1]);
  switch (testcase) {
  case 1:
    fail1();
    break;
  case 2:
    fail2();
    break;
  case 3:
    fail3();
    break;
  case 4:
    fail4();
    break;
  case 5:
    fail5();
    break;

  case 100:
    pass1();
    break;
  case 200:
    pass2();
    break;
  case 300:
    pass3();
    break;

  default:
    // CHECK-NOT: Unexpected test case
    puts("Unexpected test case");
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}
