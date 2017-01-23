// Feature tests of dynamic checking in Checked C
//
// The following lines are for the LLVM test harness:
//
// RUN: %clang -fcheckedc-extension -o %t.exe %s
// RUN: %t.exe

#include "../../include/stdchecked.h"

#include <stdbool.h>
#include <stdio.h>

int main(void) {
  dynamic_check(true);

  puts("PASS: Successful Simple Dynamic Check ()");

  return 0;
}
