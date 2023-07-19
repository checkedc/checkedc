// Feature tests of dynamic checking in Checked C
//
// The following lines are for the LLVM test harness:
//
// RUN %clang -###
// RUN: %clang -Xclang -verify -o %t.exe %s %checkedc_target_flags
// RUN: %checkedc_rununder %t.exe

#include <stdbool.h>
#include <signal.h>
#include <stdlib.h>
#include <stdchecked.h>

void handle_error(int err) {
  _Exit(0);
}

int main(void) {
  // Set up the handler for a failing dynamic check.  Currently the Checked C
  // clang implementation raises a SIGILL or SIGTRAP when a dynamic check fails,
  // depending on the target platform.  This may change in the future.
  signal(SIGILL, handle_error);
#if defined(__APPLE__) && defined(__aarch64__)
  signal(SIGTRAP, handle_error);
#endif

  // This is expected fail at runtime. It is simple enough for clang to issue a warning
  dynamic_check(false); // expected-warning {{dynamic check will always fail}}

  return 1;
}
