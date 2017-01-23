// Feature tests of dynamic checking in Checked C
//
// The following lines are for the LLVM test harness:
//
// RUN: %clang -fcheckedc-extension -o %t.exe %s
// LLVM thinks that exiting via llvm.trap() is a crash.
// RUN: not --crash %t.exe

#include "../../include/stdchecked.h"

#include <stdbool.h>
#include <stdio.h>

int main(void) {
  dynamic_check(false);

  // If we get to here, the test has failed
  puts("FAIL: Failing Simple Dynamic Check ()");

  return 0;
}
