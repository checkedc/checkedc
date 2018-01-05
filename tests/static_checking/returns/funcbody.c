// Tests that incorrect returns in function bodies are not allowed for checked
// code.
//
// The following lines are for the LLVM test harness:
//
// RUN: %clang_cc1 -verify %s

#include <stdchecked.h>

// Returning a value when none is expected.
void f1(void) checked {
  return 5; // expected-error {{void function 'f1' cannot return a value in a checked scope}}
}

checked void f2(void) {
  ptr<int> p = 0;
  return p; // expected-error {{void function 'f2' cannot return a value in a checked scope}}
}

void f3(void) checked {
  array_ptr<int> p = 0;
  return p; // expected-error {{void function 'f3' cannot return a value in a checked scope}}
}

// Failing to return a value when one is expected.
ptr<int> f10(void) {
  return;  // expected-error {{function 'f10' with checked pointer return type must return a value}}
}

int f11(void) checked {
  return; // expected-error {{non-void function 'f11' must return a value in a checked scope}}
}

int f12(array_ptr<int> p) : bounds(p, p + 1) checked {
  return; // expected-error {{non-void function 'f12' must return a value when there are return bounds}}
}


array_ptr<int> f13(void) : count(5) checked {
  return; // expected-error {{function 'f13' with checked pointer return type must return a value}}
}

array_ptr<int> f14(void) : count(5) {
  return;  // expected-error {{function 'f14' with checked pointer return type must return a value}}
}
