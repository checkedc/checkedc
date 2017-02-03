// Feature tests of dynamic checking in Checked C
//
// The following lines are for the LLVM test harness:
//
// RUN: %clang -fcheckedc-extension -Xclang -verify -o %t.exe %s
// LLVM thinks that exiting via llvm.trap() is a crash.
// RUN: not --crash %t.exe

// The dynamic_check in f1 cannot be statically checked by clang yet
// expected-no-diagnostics

#include "../../include/stdchecked.h"

void f1(int i) {
  // This is expected fail at runtime
  dynamic_check(i < 30);
}

int main(void) {
  f1(50);

  return 0;
}