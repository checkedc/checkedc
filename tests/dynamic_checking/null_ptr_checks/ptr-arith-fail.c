// Test arithmetic on null pointers.
//
// RUN: %clang %s -o %t1 %checkedc_target_flags

// RUN: %checkedc_rununder %t1 1 -DARR | FileCheck %s --check-prefix FAIL1
// RUN: %checkedc_rununder %t1 2 -DARR | FileCheck %s --check-prefix FAIL2
// RUN: %checkedc_rununder %t1 3 -DARR | FileCheck %s --check-prefix FAIL3

// RUN: %checkedc_rununder %t1 1 | FileCheck %s --check-prefix FAIL1
// RUN: %checkedc_rununder %t1 2 | FileCheck %s --check-prefix FAIL2
// RUN: %checkedc_rununder %t1 3 | FileCheck %s --check-prefix FAIL3

// RUN: %checkedc_rununder %t1 100 -DARR | FileCheck %s --check-prefix PASS1
// RUN: %checkedc_rununder %t1 100 | FileCheck %s --check-prefix PASS1

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

void pass1() {
  // PASS1: PASS
#ifdef ARR
  arr_s++;
#else
  t++;
#endif
  puts("PASS");
}

int main(int argc, array_ptr<char*> argv : count(argc)) {
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

    case 100:
      pass1();
      break;

    default:
      // CHECK-NOT: Unexpected test case
      puts("Unexpected test case");
      return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}
