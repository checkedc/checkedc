// Feature tests of dynamic checking in Checked C
//
// The following lines are for the LLVM test harness:
//
// RUN: %clang -fcheckedc-extension -o %t.exe %s
// LLVM thinks that exiting via llvm.trap() is a crash.
// RUN: not --crash %t.exe

#include <stdbool.h>
#include "../../include/stdchecked.h"

int main(void) {
  dynamic_check(false);

  return 0;
}
