// Test bounds on predefined literals.
//
// RUN: %clang %s -o %t1 %checkedc_target_flags

// RUN: %checkedc_rununder %t1 1 | FileCheck %s --check-prefix PASS1

#include <assert.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdchecked.h>

void handle_error(int err) {
  puts("Error: null pointer arithmetic");
  _Exit(0);
}

void funcPass1(_Ptr<const char> s) {
  // PASS1: main
  // PASS1-NEXT: funcPass1
  printf("%s\n", s);
  printf("%s\n", __func__);
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
      funcPass1(__func__);
      break;

    default:
      // CHECK-NOT: Unexpected test case
      puts("Unexpected test case");
      return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}
