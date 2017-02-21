// Feature tests of parsing new Checked C member bounds declarations.
//
// The following lines are for the LLVM test harness:
//
// RUN: %clang_cc1 -verify -fcheckedc-extension %s

#include "../../include/stdchecked.h"

struct S7 {
  array_ptr<int> arr : bounds(arr, arr + 5) rel_align(char);
};

// Pointer into the middle of an array
struct S9 {
  int start;
  array_ptr<int> arr : bounds(arr - start, arr - start + 5) rel_align(char);
};

struct S13 {
  // 'none' is a contextual keyword.  It is only a keyword when it
  // is the sole argument to a 'bounds' expression.
  // not a keyword
  int none;
  // a keyword
  array_ptr<int> arr1 : bounds(none);
  // not a keyword
  array_ptr<int> arr2 : count(none);
  // a keyword
  array_ptr<int> arr3 : bounds(none + arr2, none + arr2 + 5) rel_align(char); // expected-error {{expected ')'}} \
                                                              // expected-note {{to match this '('}}  expected-error {{expected ')'}} expected-note {{to match this '('}}
  // not a keyword
  array_ptr<int> arr4 : bounds(arr3, arr3 + none) rel_align(char); 
};

struct S14 {
  // 'bounds' is a contextual keyword.  It is only a keyword when it follows
  // the ':' in a bounds declaration.

  // not a keyword
  int bounds;   
  // not a keyword as an argument
  array_ptr<int> arr1 : bounds(bounds + arr1, bounds + arr1 + 2) rel_align(char);
  // a keyword
  array_ptr<int> arr2 : bounds(none);
  // not a keyword as an argument.
  array_ptr<int> arr3 : bounds(bounds + arr2, bounds + arr2 + 5) rel_align(char);
  // not a keyword
  array_ptr<int> arr4 : bounds(arr3, arr3 + bounds) rel_align(char);
};

//
// Parsing of more complicated structure member declarations with bounds
// declarations
//

struct S15 {
  // Members that are array_ptrs to ptrs
  array_ptr<ptr<int>> arr3 : bounds(arr3, arr3 + 5) rel_align(char);
  // pointer into middle of arr3
  array_ptr<ptr<int>> mid1 : bounds(arr3, arr3 + 5) rel_align(char);
  // Members that are array_ptrs to unchecked arrays
  array_ptr<int[10]> arr6 : bounds(arr6, arr6 + 5) rel_align(char);
  // pointer into middle of arr6
  array_ptr<int[10]> mid2 : bounds(arr6, arr6 + 5) rel_align(char);
  // Members that are array_ptrs to checked arrays
  array_ptr<int checked[10]> arr9 : bounds(arr9, arr9 + 9) rel_align(char);
  // Members that are unchecked pointers with bounds.  These will
  // be used for interoperation.
  int *arr12 : bounds(arr12, arr12 + 9) rel_align(char);
  // Members that are arrays and that have bounds specified for them.
  // Legal but not too interestting.
  int arr15[10] : bounds(arr15, arr15 + 9) rel_align(char);
};

// Members that are pointers to functions that have bounds declarations on
// return values
extern void S16(void) {
  // Use 'bounds' instead of 'count'.
  ptr<array_ptr<int>(array_ptr<int> arg : count(5)) : bounds(arg, arg + 5) rel_align(char)>
    r1 = 0; 
  ptr<int(array_ptr<int> arg : count(n), int n) : bounds(arg, arg + n) rel_align(char)> r2 = 0;
  // Unchecked pointers to functions.
}
// Bounds distributed across multiple nested members
struct S18 {
  struct S19 {
     array_ptr<int> lower;
     array_ptr<int> upper;
  } pair;
  array_ptr<int> arr1 : bounds(pair.lower, pair.upper) rel_align(char);
  struct S20 {
    array_ptr<int> arr2 : bounds(pair.lower, pair.upper) rel_align(char);
  } nested;
};

// Anonymous struct version
struct S21 {
  struct {
    array_ptr<int> lower;
    array_ptr<int> upper;
  } pair;
  array_ptr<int> arr1 : bounds(pair.lower, pair.upper) rel_align(char);
  struct {
    array_ptr<int> arr2 : bounds(pair.lower, pair.upper) rel_align(char);
  } nested;
};

//
// Errors in declaring structure members with bounds
//

struct S22 {
  array_ptr<int> arr : bounds(arr, unknown_id) rel_align(char); // expected-error {{use of undeclared member}} expected-error {{expected ')'}}   expected-note {{to match this '('}}
};

// Misspell bounds to cause an semantic checking error.
// clang will parse this as a constant-expression that specifies a bit field
// and generate several errors.
struct S24 {
  int len;
  array_ptr<int> arr : boounds(arr, arr + 5) rel_align(char);  // expected-error 2 {{use of undeclared identifier 'arr'}} \
                                               // expected-warning {{implicit declaration of function 'boounds'}}
};

// Omit both arguments to bounds to cause a parsing error
struct S27 {
  int len;
  array_ptr<int> arr : bounds() rel_align(char); //expected-error {{expected expression}} expected-error {{expected ')'}}  expected-note {{to match this '('}}
};

//
// Member bounds cannot refer to variables.
//
array_ptr<int> global_bound;

struct S29 {
  int len;
  array_ptr<int> arr : bounds(global_bound, global_bound + len) rel_align(char); // expected-error 2 {{use of undeclared member 'global_bound'}} expected-error {{expected ')'}}  expected-note {{to match this '('}}
};


int f1(void) {
  int buffer checked[100];
  struct S30 {
     int len;
    array_ptr<int> arr : bounds(buffer, buffer + len) rel_align(char); // expected-error 2 {{use of undeclared member 'buffer'}} expected-error {{expected ')'}} expected-note {{to match this '('}}
  };
}

