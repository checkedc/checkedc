// Feature tests of dynamic checking in Checked C
//
// The following lines are for the LLVM test harness:
//
// RUN: %clang -Xclang -verify -o %t.exe %s %checkedc_target_flags
// RUN: %checkedc_rununder %t.exe

// expected-no-diagnostics

#include <stdbool.h>
#include <stdchecked.h>

int main(void) {
  dynamic_check(true);

  return 0;
}
