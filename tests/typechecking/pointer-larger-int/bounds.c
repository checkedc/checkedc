// Feature tests of typechecking new Checked C bounds declarations.
//
// The following lines are for the LLVM test harness:
//
// RUN: %clang_cc1 -Wno-check-bounds-decls -verify -verify-ignore-unexpected=note %s

#include <stdchecked.h>

_Static_assert(sizeof(void*) > sizeof(int),
  "Pointers must be larger than ints");

enum E1 {
  EnumVal1,
  EnumVal2
};

void int_local_var_bounds_decl(void) {
  // bounds declarations are allowed for integer variables to support
  // casting of pointers to integers and back.  We usually expect this
  // to happen within expressions, but to allow uniform use of language
  // features, we allow bounds on integer-typed variables.
  int a1 checked[5];

  // byte_count
  int t21  _Byte_count(5 * sizeof(int)) = (int)a1; // expected-warning {{cast to smaller integer type 'int' from '_Array_ptr<int>'}}

  // bounds
  int t25 : bounds(a1, a1 + 5) = (int)a1; // expected-warning {{cast to smaller integer type 'int' from '_Array_ptr<int>'}}
  long int t26 : bounds(a1, a1 + 5) = (int)a1; // expected-warning {{cast to smaller integer type 'int' from '_Array_ptr<int>'}}
  unsigned long int t27 : bounds(a1, a1 + 5) = (int)a1; // expected-warning {{cast to smaller integer type 'int' from '_Array_ptr<int>'}}
  enum E1 t28   _Bounds(a1, a1 + 5) = (int)a1; // expected-warning {{cast to smaller integer type 'int' from '_Array_ptr<int>'}}
}
