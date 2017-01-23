// Feature tests of dynamic checking in Checked C
//
// The following lines are for the LLVM test harness:
//
// RUN: %clang -fcheckedc-extension -o %t.exe %s
// RUN: %t.exe

#include "../../include/stdchecked.h"

#include <stdio.h>

void f1(int i) {
  dynamic_check(i < 30);

  puts("PASS: Successful Arithmetic Dynamic Check ()");
}

int main(void) {
  f1(10);

  return 0;
}
