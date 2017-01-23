// Feature tests of dynamic checking in Checked C
//
// The following lines are for the LLVM test harness:
//
// RUN: %clang -fcheckedc-extension -o %t.exe %s
// RUN: %t.exe

#include <stdbool.h>
#include "../../include/stdchecked.h"

int main(void) {
  dynamic_check(true);

  return 0;
}
