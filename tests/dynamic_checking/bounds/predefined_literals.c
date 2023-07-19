// Test bounds on predefined literals.
//
// RUN: %clang %s -o %t1 %checkedc_target_flags

// RUN: %checkedc_rununder %t1 1 | FileCheck %s --check-prefix PASS1
// RUN: %checkedc_rununder %t1 100 | FileCheck %s --check-prefix FAIL1

#include <signal.h>
#include <stdchecked.h>
#include <stdio.h>
#include <stdlib.h>

void handle_error(int err) {
  puts("Error: out-of-bounds access of predefined literal");
  _Exit(0);
}

void pass1(_Ptr<const char> s) {
  // PASS1: main
  // PASS1-NEXT: pass1
  // PASS1-NEXT: p
  // PASS1-NEXT: a
  // PASS1-NEXT: s
  // PASS1-NEXT: s
  // PASS1-NEXT: 1
  printf("%s\n", s);
  printf("%s\n", __func__);
  printf("%c\n", __func__[0]);
  printf("%c\n", __func__[1]);
  printf("%c\n", __func__[2]);
  printf("%c\n", __func__[3]);
  printf("%c\n", __func__[4]);
}

void fail1(int x) {
// FAIL1: Error: out-of-bounds access of predefined literal
#pragma CHECKED_SCOPE ON
  char c = __func__[x];
#pragma CHECKED_SCOPE OFF
}

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

  int testcase = atoi(argv[1]);
  switch (testcase) {
    case 1:
      pass1(__func__);
      break;
    case 100:
      fail1(100);
      break;

    default:
      // CHECK-NOT: Unexpected test case
      puts("Unexpected test case");
      return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}
