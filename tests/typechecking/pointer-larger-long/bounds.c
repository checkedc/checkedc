// Feature tests of typechecking new Checked C bounds declarations.
//
// The following lines are for the LLVM test harness:
//
// RUN: %clang_cc1 -Wno-check-bounds-decls -verify -verify-ignore-unexpected=note %s

#include <stdchecked.h>

_Static_assert(sizeof(void*) > sizeof(long),
  "Pointers must be larger than longs");

void int_local_var_bounds_decl(void) {
  // bounds declarations are allowed for integer variables to support
  // casting of pointers to integers and back.  We usually expect this
  // to happen within expressions, but to allow uniform use of language
  // features, we allow bounds on integer-typed variables.
  int a1 checked[5];

  long int t22 : byte_count(5 * sizeof(int)) = (long int)a1; // expected-warning {{cast to smaller integer type 'long' from '_Array_ptr<int>'}}
  unsigned long int t23 : byte_count(5 * sizeof(int)) = (unsigned long int) a1; // expected-warning {{cast to smaller integer type 'unsigned long' from '_Array_ptr<int>'}}
}
