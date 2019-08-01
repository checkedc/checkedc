// Feature tests of dynamic checking in Checked C
//
// The following lines are for the LLVM test harness:
//
// RUN: %clang -Xclang -verify -o %t.exe %s %checkedc_target_flags
// RUN: %checkedc_rununder %t.exe

// expected-no-diagnostics

#include <stdchecked.h>

void f1(int i) {
  dynamic_check(i < 30);
}

int main(void) {
  f1(10);

  return 0;
}
