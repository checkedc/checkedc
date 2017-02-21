// Feature tests of parsing new Checked C bounds declarations for
// function return values
//
// The following lines are for the LLVM test harness:
//
// RUN: %clang_cc1 -verify -verify-ignore-unexpected=note -fcheckedc-extension %s

#include "../../include/stdchecked.h"

// Parsing of function declarations
extern array_ptr<int> f6(array_ptr<int> arr : bounds(arr, arr + 5) rel_align(char))
  : bounds(arr, arr + 5) rel_align(char);
extern array_ptr<int> f7(int start,
                         array_ptr<int> arr : bounds(arr - start, arr - start + 5))
  : bounds(arr - start, arr - start + 5) rel_align(char);
extern array_ptr<int> f11(int bounds, array_ptr<int> arr : count(bounds))
  : bounds(arr, arr + bounds) rel_align(char);

extern array_ptr<int> f6(array_ptr<int> arr : bounds(arr, arr + 5) rel_align(char))
  : bounds(arr, arr + 5) rel_align(char) {
  return arr;
}

extern array_ptr<int> f7(int start,
                         array_ptr<int> arr : bounds(arr - start, arr - start + 5))
  : bounds(arr - start, arr - start + 5) rel_align(char) {
   return arr;
}

// 'bounds' is a contextual keyword.  It is only a keyword when it
// immediately follows the ':' in a bounds declaration.
extern array_ptr<int> f11(int bounds, array_ptr<int> arr : count(bounds))
  : bounds(arr, arr + bounds) rel_align(char) {
  return arr;
}

//
// Parsing of more complicated function declarations with bounds declarations
//

// Functions that return array_ptrs to ptrs

extern array_ptr<ptr<int>> f14(array_ptr<ptr<int>> arr : count(5))
  : bounds(arr, arr + 5) rel_align(char) {
  return arr;
}

// Functions that return array_ptrs to arrays

extern array_ptr<int[10]> f17(array_ptr<int[10]> arr : count(5))
  : bounds(arr, arr + 3) rel_align(char) {
  return arr;
}

// Pointers to functions that have bounds declarations on return values
extern void f24(void) {
  // Use 'bounds' instead of 'count'.
  ptr<array_ptr<int>(array_ptr<int> arg : count(5)) : bounds(arg, arg + 5) rel_align(char)>
    r1 = 0;
  ptr<int(array_ptr<int> arg : count(n), int n) : bounds(arg, arg + n) rel_align(char)> r2 = 0;
  // Unchecked pointers to functions.
typedef int func2(array_ptr<int> arr : count(len), int len)
  : bounds(arr, arr + len) rel_align(char);
}
// Misspell bounds to cause a parsing error.
extern array_ptr<int> f27(int len, array_ptr<int> arr : count(len)) : boounds(arr, arr + len) rel_align(char) ) { // expected-error {{expected bounds expression}} 
                                                                                                                }


