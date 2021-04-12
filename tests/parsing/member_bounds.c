// Feature tests of parsing new Checked C member bounds declarations.
//
// The following lines are for the LLVM test harness:
//
// RUN: %clang_cc1 -verify %s

#include <stdchecked.h>

struct S1 {
  array_ptr<int> arr : count(5);
};

struct S2 {
  array_ptr<int> arr : count(3 + 2);
};

struct S3 {
  int len;
  array_ptr<int> arr : count(len);
};

struct S4 {
  array_ptr<int> arr : count(len);
  int len;
};

struct S5 {
  array_ptr<int> arr : byte_count(20);
};

struct S6 {
  array_ptr<int> arr : byte_count(5 * sizeof(int));
};

struct S7 {
  array_ptr<int> arr : bounds(arr, arr + 5);
};

// Structure with a variable length array at the end
struct S8 {
  int len;
  int arr checked[] : count(len);
};

// Pointer into the middle of an array
struct S9 {
  int start;
  array_ptr<int> arr : bounds(arr - start, arr - start + 5);
};

struct S10 {
  array_ptr<int> arr : bounds(unknown);
};

// Count is a contextual keyword.  It is only a keyword when it immediately
// folows the ':' in a bounds declaration.  Otherwise it can be used as an
// identifer.
struct S11 {
  int count;
  array_ptr<int> arr : bounds(unknown);
};

struct S12 {
  int count;
  array_ptr<int> arr : count(count);
};

struct S13 {
  // 'unknown' is a contextual keyword.  It is only a keyword when it
  // is the sole argument to a 'bounds' expression.
  // not a keyword
  int unknown;
  // a keyword
  array_ptr<int> arr1 : bounds(unknown);
  // not a keyword
  array_ptr<int> arr2 : count(unknown);
  // a keyword
  array_ptr<int> arr3 : bounds(unknown + arr2, unknown + arr2 + 5); // expected-error {{expected ')'}} \
                                                              // expected-note {{to match this '('}}
  // not a keyword
  array_ptr<int> arr4 : bounds(arr3, arr3 + unknown); 
};

struct S14 {
  // 'bounds' is a contextual keyword.  It is only a keyword when it follows
  // the ':' in a bounds declaration.

  // not a keyword
  int bounds;   
  // not a keyword as an argument
  array_ptr<int> arr1 : bounds(bounds + arr1, bounds + arr1 + 2);
  // a keyword
  array_ptr<int> arr2 : bounds(unknown);
  // not a keyword as an argument.
  array_ptr<int> arr3 : bounds(bounds + arr2, bounds + arr2 + 5);
  // not a keyword
  array_ptr<int> arr4 : bounds(arr3, arr3 + bounds);
};

//
// Parsing of more complicated structure member declarations with bounds
// declarations
//

struct S15 {
  // Members that are array_ptrs to ptrs
  array_ptr<ptr<int>> arr1 : count(5);
  array_ptr<ptr<int>> arr2 : byte_count(5 * sizeof(ptr<int>));
  array_ptr<ptr<int>> arr3 : bounds(arr3, arr3 + 5);
  // pointer into middle of arr3
  array_ptr<ptr<int>> mid1 : bounds(arr3, arr3 + 5);
  // Members that are array_ptrs to unchecked arrays
  array_ptr<int[10]> arr4 : count(5);
  array_ptr<int[10]> arr5 : byte_count(5 * sizeof(ptr<int>));
  array_ptr<int[10]> arr6 : bounds(arr6, arr6 + 5);
  // pointer into middle of arr6
  array_ptr<int[10]> mid2 : bounds(arr6, arr6 + 5);
  // Members that are array_ptrs to checked arrays
  array_ptr<int checked[10]> arr7 : count(5);
  array_ptr<int checked[10]> arr8 : byte_count(5 * sizeof(ptr<int>));
  array_ptr<int checked[10]> arr9 : bounds(arr9, arr9 + 9);
  // Members that are unchecked pointers with bounds.  These will
  // be used for interoperation.
  int *arr10 : count(5);
  int *arr11 : byte_count(5 * sizeof(int));
  int *arr12 : bounds(arr12, arr12 + 9);
  // Members that are arrays and that have bounds specified for them.
  // Legal but not too interestting.
  int arr13[10] : count(9);
  int arr14[10] : byte_count(9 * sizeof(int));
  int arr15[10] : bounds(arr15, arr15 + 9);
};

// Members that are pointers to functions that have bounds declarations on
// return values
extern void S16(void) {
  // Checked pointer to a function that returns an array_ptr to 5 integers.
  ptr<array_ptr<int>(void) : count(5)> p1 = 0;
  // Checked pointer to a function that returns an array_ptr to n integers,
  // where n is n argument.
  ptr<array_ptr<int>(int n) : count(n)> p2 = 0;
  // Use 'byte_count; instead of 'count'
  ptr<array_ptr<int>(void) : byte_count(5 * sizeof(int))> q1 = 0;
  ptr<int(int arg) : byte_count(5 * sizeof(int))> q2 = 0;
  ptr<int(int n, int arg) : byte_count(n * sizeof(int))> q3 = 0;
  // Use 'bounds' instead of 'count'.
  ptr<array_ptr<int>(array_ptr<int> arg : count(5)) : bounds(arg, arg + 5)>
    r1 = 0; 
  ptr<int(array_ptr<int> arg : count(n), int n) : bounds(arg, arg + n)> r2 = 0;
  // Unchecked pointers to functions.
  int(*s1)(array_ptr<int> : count(5));
  int(*s2)(array_ptr<int> arg : count(5));
  int(*s3)(int n, array_ptr<int> arg : count(n));
  int(*s4)(array_ptr<int> arg : count(n), int n);
  // Use 'byte_count' instead of 'count'.
  int(*t1)(array_ptr<int> : byte_count(5 * sizeof(int)));
  int(*t2)(array_ptr<int> arg : count(5 * sizeof(int)));
  int(*t3)(int n, array_ptr<int> arg : count(n * sizeof(int)));
  int(*t4)(array_ptr<int> arg : count(n * sizeof(int)), int n);
}

struct S17 {
  // Members that are pointers to functions that have bounds on
  // arguments or return values.
  int (*f1)(int len, array_ptr<int> arr : count(len));
  array_ptr<int> (*f2)(int len, array_ptr<int> arr : count(len)) : count(len);
  // same as f1, but checked ptr
  ptr<int (int len, array_ptr<int> arr : count(len))> f3; 
  // same as f2, but checked ptr
  ptr<array_ptr<int> (int len, array_ptr<int> arr : count(len)) : count(len)> f4;
  // Use count as a parameter variable name.
  int(*f5)(int count, array_ptr<int> arr : count(count));
  // Unchecked pointer to a function that returns a ptr to a function that
  // takes in a length and returns an array_ptr of that length.
  ptr<array_ptr<int>(int len) : count(len)> (*f6)(int len);
  // Checked pointer to a function instead.
  ptr<ptr<array_ptr<int>(int len) : count(len)> (int len)> f7;
};

// Bounds distributed across multiple nested members
struct S18 {
  struct S19 {
     array_ptr<int> lower;
     array_ptr<int> upper;
  } pair;
  array_ptr<int> arr1 : bounds(pair.lower, pair.upper);
  struct S20 {
    array_ptr<int> arr2 : bounds(pair.lower, pair.upper);
  } nested;
};

// Anonymous struct version
struct S21 {
  struct {
    array_ptr<int> lower;
    array_ptr<int> upper;
  } pair;
  array_ptr<int> arr1 : bounds(pair.lower, pair.upper);
  struct {
    array_ptr<int> arr2 : bounds(pair.lower, pair.upper);
  } nested;
};

//
// Errors in declaring structure members with bounds
//

struct S22 {
  array_ptr<int> arr : bounds(arr, unknown_id); // expected-error {{use of undeclared member}}
};

struct S23 {
  array_ptr<int> arr : 6 + 6; // expected-error {{expected bounds expression}}
};

// Misspell bounds to cause an semantic checking error.
// clang will parse this as a constant-expression that specifies a bit field
// and generate several errors.
struct S24 {
  int len;
  array_ptr<int> arr : boounds(arr, arr + 5);  // expected-error 2 {{use of undeclared identifier 'arr'}} \
                                               // expected-warning {{implicit declaration of function 'boounds'}}
};

// Misspell count to cause an semantic checking error.
// clang will parse this as a constant-expression that specifies a bit field
// and generate several errors.
struct S25 {
  int len;
  array_ptr<int> arr : coount(5); // expected-error {{expected bounds expression}} \
                                  // expected-warning {{implicit declaration of function 'coount'}}
};

// Omit an argument to bounds to cause a parsing error
struct S26 {
  int len;
  array_ptr<int> arr : bounds(arr); // expected-error {{expected ','}}
};

// Omit both arguments to bounds to cause a parsing error
struct S27 {
  int len;
  array_ptr<int> arr : bounds(); //expected-error {{expected expression}}
};

// Omit the argument to count to cause a parsing error.
struct S28 {
  int len;
  array_ptr<int> arr : count(); //expected-error {{expected expression}}
};

//
// Member bounds cannot refer to variables.
//
array_ptr<int> global_bound;

struct S29 {
  int len;
  array_ptr<int> arr : bounds(global_bound, global_bound + len); // expected-error 2 {{use of undeclared member 'global_bound'}}
};


int f1(void) {
  int buffer checked[100];
  struct S30 {
     int len;
     array_ptr<int> arr : bounds(buffer, buffer + len); // expected-error 2 {{use of undeclared member 'buffer'}}
  };
}

int f2(void) {
  const int bounds = 4;
  struct S31 {
    // This should be parsed as an incorrect bounds expression+-
    int x : bounds; // expected-error {{expected '(' after 'bounds'}}
  };
}

int f3(void) {
  enum E {
    bounds = 4
  };
  struct S30 {
    // This should be parsed as an incorrect bounds expression+-
    int x : bounds; // expected-error {{expected '(' after 'bounds'}}
  };
}

//
// Union members
//

// Union that is bounds-safe because its only members are scalar types
// and arrays of scalar types.
union U1 {
  int i;
  float f;
  char arr checked[sizeof(int)];
};

// Union that is bounds safe because all members point to objects
// with the same size that are arrays of scalars.
union U2 {
  array_ptr<int> ip : count(4);
  array_ptr<char> cp : count(4 * sizeof(int));
};

// Unions where dynamic tags are used to ensure correct use of members.
// There still needs to be a design that connects a particular tag value
// with the validity of a specific member.
union U3 {
  enum E {
    Array,
    Ptr,
    Integer
  } tag;

  // TODO: enum identifiers should be accessible in
  // member bounds expressions
  array_ptr<int> f : count((tag == 0) ? 5 : 0);
  ptr<int> p;
  int i;
};

// Union of pointers where lengths depend on a tag in an enclosing structure
struct S32 {
  int len : 31;
  int tag : 1;
  union {
    array_ptr<char> cp : count(tag ? len : 0);
    array_ptr<int> ip : count(!tag ? 0 : len);
  };
};

// Unions without tags that would depend on program invariants for bounds safety.
union U5 {
  _Bool isInteger;
  ptr<int> ip;
  ptr<float> fp;
};

union U6 {
  array_ptr<int> f : count(5);
  ptr<int> p;
  int i;
};
