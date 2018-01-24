// Test that falling off ends of functions is not allowed for checked code.
// This is tested by a set of files because clang suppresses these analysis-based
// errors after the first one occurs.
//
// The following lines are for the LLVM test harness:
//
// RUN: %clang_cc1 -verify %s

#include <stdchecked.h>

int f24(array_ptr<int> p : count(1)) : bounds(p, p + 1) {
  if (p)
    return 0;
} // expected-error {{control may reach end of function with return bounds}}
