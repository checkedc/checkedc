// Feature tests of dynamic checking in Checked C
//
// The following lines are for the LLVM test harness:
//
// RUN: %clang -Xclang -verify -o %t.exe %s %checkedc_target_flags
// LLVM thinks that exiting via llvm.trap() is a crash.
// RUN: %checkedc_rununder %t.exe

// The dynamic_check in f1 cannot be statically checked by clang yet
// expected-no-diagnostics

#include <signal.h>
#include <stdlib.h>
#include <stdchecked.h>

void f1(int i) {
  // This is expected fail at runtime
  dynamic_check(i < 30);
}

void handle_error(int err) {
  _Exit(0);
}

int main(void) {
  // Currently the Checked C clang implementation raises a SIGILL when a
  // dynamic check fails.  This may change in the future.
  signal(SIGILL, handle_error);

  f1(50);

  return 1;
}
