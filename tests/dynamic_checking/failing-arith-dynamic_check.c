// Feature tests of dynamic checking in Checked C
//
// The following lines are for the LLVM test harness:
//
// RUN: %clang -fcheckedc-extension -o %t.exe %s
// LLVM thinks that exiting via llvm.trap() is a crash.
// RUN: not --crash %t.exe

#include "../../include/stdchecked.h"

#include <stdio.h>

void f1(int i) {
  dynamic_check(i < 30);

  // If we get to here, the test has failed
  puts("FAIL: Failing Arithmetic Dynamic Check ()");
}

int main(void) {
  f1(50);

  return 0;
}