// Feature tests of parsing new Checked C member bounds declarations.
//
// The following lines are for the LLVM test harness:
//
// RUN: %clang_cc1 -verify %s

#include <stdchecked.h>

_Static_assert(sizeof(void*) > sizeof(int),
  "Pointers must be larger than ints");

// Unions of pointers and integers, where the least significant
// bit is used as a tag.

// We tag integers with 1 instead of trying to tag pointers with 1.
// Null pointers tagged with 1 are not allowed by the current 
// Checked C definition.

#define is_tagged_int(p) ((int)(p) & 1)
#define untag_int(p) ((p) & ~1)

union U4 {
  array_ptr<int> ip : bounds(ip, is_tagged_int(ip) ? ip : ip + 5); // expected-warning {{cast to smaller integer type 'int' from '_Array_ptr<int>'}}
  int i;
};
