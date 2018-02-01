// Test that falling off ends of functions is not allowed for checked code.
// This is tested by a set of files because clang suppresses these analysis-based
// errors after the first one occurs.
//
// The following lines are for the LLVM test harness:
//
// RUN: %clang_cc1 -verify %s

#include <stdchecked.h>

int f25(int i) checked {
  if (i)
    return 0;
}  // expected-error {{control may reach end of checked function body}}
