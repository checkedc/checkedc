// Feature tests of parsing new Checked C bounds declarations.
//
// The following lines are for the LLVM test harness:
//
// RUN: %clang_cc1 -Wno-check-bounds-decls -verify %s

#include <stdchecked.h>

// Top level declarations with different storage classes and
// storage classes omitted.
extern array_ptr<int> a : count(5);
extern array_ptr<int> b : count(3 + 2);
extern int cLen;
extern array_ptr<int> c : count(cLen);
extern array_ptr<int> d : byte_count(20);
extern array_ptr<int> e : byte_count(5 * sizeof(int));
extern array_ptr<int> f : byte_count(cLen * sizeof(int));
extern array_ptr<int> g : bounds(unknown);
extern array_ptr<int> h : bounds(f - 2, f + 3);
extern array_ptr<int> i : count(5), j : bounds(i - 2, i + 3),
                      k : bounds(j + 2, j + 5);

static float vals[5] = { 0.0, 1.0, 2.0, 3.0, 4.0 };
static array_ptr<float> l : count(5) = vals;
static _Thread_local array_ptr<int> m : count(5);

array_ptr<int> b : count(3 + 2) = 0;
array_ptr<int> d : byte_count(20) = 0;
array_ptr<int> g : bounds(unknown) = 0;

// Declare a bounds for a checked array.  This is legal, but
// unnecessary.
char buf checked[128] : count(128);
array_ptr<char> cursor : bounds(buf, buf + 128) = buf + 64;

// Local variable declarations.
extern void f1(array_ptr<int> arr : count(5)) {
  array_ptr<int> p : count(5) = arr;
  array_ptr<int> q : count(3 + 2) = arr;
  int len = 5;
  array_ptr<int> r : count(len) = arr;
  array_ptr<int> s : byte_count(20) = arr;
  array_ptr<int> t : byte_count(5 * sizeof(int)) = arr;
  array_ptr<int> u : byte_count(len * sizeof(int)) = arr;
  array_ptr<int> v : bounds(v, v + 5) = arr;
  array_ptr<int> w : bounds(unknown) = arr;
  array_ptr<int> x : bounds(unknown);
  array_ptr<int> midarr : bounds(midarr - 1, midarr - 1 + 2) = arr + 2;

  static array_ptr<int> cache1 : count(5);
  static array_ptr<int> cache1_ptr : bounds(cache1 - 2, cache1 + 3);
}

// Multiple declarators in one declaration
extern void f2(array_ptr<int> arr : count(5)) {
  array_ptr<int> p : count(5) = 0, q : count(3 + 2) = arr;
  int len = 5;
  array_ptr<int> r : count(len) = arr, s : byte_count(20) = arr;
  array_ptr<int> t : byte_count(5 * sizeof(int)) = arr,
                 u : bounds(u, u + 5) = arr,
                 v : bounds(unknown) = arr;
}

// Parenthesized subexpressions
extern void f3(array_ptr<int> arr : count(5)) {
  array_ptr<int> p : count((5)) = arr;
  array_ptr<int> q : count(3 + (2)) = arr;
  int len = 5;
  array_ptr<int> r : count((len)) = arr;
  array_ptr<int> s : byte_count((20)) = arr;
  array_ptr<int> t : byte_count(5 * (sizeof(int))) = arr;
  array_ptr<int> u : byte_count((len) * sizeof(int)) = arr;
  array_ptr<int> v : bounds(v, (v + 5) + len - len) = arr;
  array_ptr<int> w: bounds((w + len - (len)), (w + len)) = arr;
  array_ptr<int> midarr : bounds(midarr - 1, (midarr - 1) + 2) = arr + 2;
}


// Contextual keywords
extern void f4(array_ptr<int> arr : count(len), int len) {
  int count = len;
  array_ptr<int> p : count(count) = arr;
  int unknown = 0;
  // unknown is a contextual a keyword if it follows 'bounds' '('
  array_ptr<int> q : bounds(unknown) = arr;
  array_ptr<int> r : bounds(unknown + arr, arr + len) = arr; // expected-error {{expected ')'}} expected-note {{to match this '('}}
  array_ptr<int> s : bounds(arr + unknown, arr + len) = arr;
  array_ptr<int> t : bounds(t, t + count) = arr;
  int bounds = len;
  array_ptr<int> u : bounds(u, u + bounds) = arr;
}

// Checked array declarations
extern void f5(void) {
  int arr1 checked[5] : count(5);
  int arr2 checked[5]: count(2 + 3);
  int arr3 checked[6] : count(5);
  int len = 5;
  int arr4 checked[5] : count(len);
  int arr7 checked[5] : byte_count(5 * sizeof(int));
  int arr8 checked[5] : byte_count(len * sizeof(int));
  auto int arr5 checked[5] : bounds(arr5, arr5 + 5);
  auto int arr6 checked[5] : bounds(arr5, arr5 + len);

  static int cache checked[5] : count(5);
  static array_ptr<int> cache_ptr : bounds(cache - 2, cache + 3);
}

// Parsing of more complicated declarations with bounds declarations
// 
extern void f6(int *arr checked[] : count(5)) {
  array_ptr<int *> p : count(5) = arr;
  array_ptr<int *> q : count(3 + 2) = arr;
  int len = 5;
  array_ptr<int *> r : count(len) = arr;
  array_ptr<int *> s : byte_count(5 * sizeof(int)) = arr;
  array_ptr<int *> t : byte_count(len * sizeof(int)) = arr;
  // redundant but legal bounds expressions on arrays.
  int *arr1 checked[5] : count(5);
  int *arr2 checked[5] : count(3 + 2);
  int *arr3 checked[5] : count(len);
  // Checked array of unchecked pointers to functions.  We have to parenthesize
  // the inner declarator to avoid : count(len) being parsed as part of the 
  // type of arr, not a bounds expression.
  int ((*arr4 checked[5])(int, int)) : count(5);
  int ((*arr5 checked[5])(int, int)) : count(len);
  int ((*arr6 checked[5])(int, int)) : bounds(arr5, arr5 + len);
  // Checked array of checked pointers to functions
  ptr<int(int, int)> arr7 checked[5] : count(5) = {0};
  ptr<int(int, int)> arr8 checked[5] : bounds(arr8, arr8 + 5) = {0};
  // Array_ptrs to checked pointers to functions.
  array_ptr<ptr<int (int, int)>> u : count(5) = 0;
  array_ptr<ptr<int (int, int)>> v : bounds(v, v + 5) = 0;
  array_ptr<ptr<int (int, int)>> w : bounds(v, v + 5) = 0;
  array_ptr<ptr<int (int len, array_ptr<int> arr : count(len))>> x : bounds(x, x + 5) = 0;
}

// Bounds expressions with incorrect syntax or semantics
extern void f7(array_ptr<int> arr : count(5)) {
  array_ptr<int> p : bounds(start, end + 5) = 0; // expected-error {{undeclared identifier 'start'}} expected-error {{undeclared identifier 'end'}}
  array_ptr<int> q : count(len) = 0;       // expected-error {{undeclared identifier 'len'}}
  array_ptr<int> r : byte_count(len) = 0;  // expected-error {{undeclared identifier 'len'}}
  array_ptr<int> s : 6 + 6 = 0;            // expected-error {{expected bounds expression}}
  array_ptr<int> t : 6 + 6, u : count(5) = arr; // expected-error {{expected bounds expression}}
  array_ptr<int> v : boounds(a, b + 5) = 0;  // expected-error {{expected bounds expression}}
  array_ptr<int> w : coount(5) = 0;         // expected-error {{expected bounds expression}}
  array_ptr<ptr<int (int len, array_ptr<int> arr : count(badvar))>> x // expected-error {{undeclared identifier 'badvar'}}
    : bounds(x, x + 5) = 0;
}

