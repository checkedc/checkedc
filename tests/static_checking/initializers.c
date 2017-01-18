// Feature tests of static checking of bounds declarations for variables with
//  initializers. 
//
// The following lines are for the LLVM test harness:
//
// RUN: %clang_cc1 -verify -fcheckedc-extension %s

#include "../../include/stdchecked.h"

// Check that declarations of automatic variables with Checked C bounds 
// declarations or _Ptr types always have initializers.
extern void f() {
  array_ptr<int> v1;
  array_ptr<int> v2 = 0;
  array_ptr<int> v3 : bounds(none);
  array_ptr<int> v4 : bounds(none) = 0;
  array_ptr<int> v5 : count(5) = 0;
  array_ptr<int> v6 : count(5);                       // expected-error {{automatic variable 'v6' with bounds must have initializer}}
  array_ptr<int> v7 : byte_count(5 * sizeof(int)) = 0;
  array_ptr<int> v8 : byte_count(5 * sizeof(int));    // expected-error {{automatic variable 'v8' with bounds must have initializer}}
  array_ptr<int> v9 : bounds(v9, v9 + 5) = 0;
  array_ptr<int> v10 : bounds(v10, v10 + 5);          // expected-error {{automatic variable 'v10' with bounds must have initializer}}

  ptr<int> v20 = 0;
  ptr<int> v21;         // expected-error {{automatic variable 'v21' with _Ptr type must have initializer}}
}
