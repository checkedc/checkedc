// Feature tests of dynamic checking in Checked C
//
// The following lines are for the LLVM test harness:
//
// RUN: %clang -fcheckedc-extension -Xclang -verify -o %t.exe %s
// RUN: %t.exe

// expected-no-diagnostics

#include "../../include/stdchecked.h"

void f1(int i) {
  dynamic_check(i < 30);
}

int main(void) {
  f1(10);

  return 0;
}
