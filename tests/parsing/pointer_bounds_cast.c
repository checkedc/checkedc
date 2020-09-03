// Feature tests of parsing new Checked C dynamic and assume bounds
// cast. The following lines are for the LLVM test harness:
//
// RUN: %clang_cc1 -verify -verify-ignore-unexpected=note %s

#include <stdchecked.h>

extern void f1() {
  array_ptr<int> a : count(1) = 0;
  int b[10];
  a = _Dynamic_bounds_cast<array_ptr<int>>(b, count(10));
}

extern void f2() {
  char p[10];
  array_ptr<int> a : count(1) = 0;
  array_ptr<int> d : count(10) = _Dynamic_bounds_cast<array_ptr<int>>(a, count(5)); // expected-error {{declared bounds for 'd' are invalid after initialization}}
  a = _Assume_bounds_cast<array_ptr<int>) (p, bounds(p, p+1)); // expected-error {{expected '>'}}
}

extern void f3() {
  array_ptr<int> a : count(2) = 0;
  array_ptr<char> b : count(2) = 0;

  b = _Assume_bounds_cast<array_ptr<char>>(a, count(2));
}

extern void f4() {
  array_ptr<int> a : count(2) = 0;
  array_ptr<char> b : count(2) = 0;

  a = _Asume_bounds_cast<array_ptr<int>>(b, count(2)); // expected-error{{use of undeclared identifier}} expected-warning {{implicit declaration of function 'count' is invalid in C99}}
  a = _Dssume_bounds_cast<int>(b); // expected-error{{use of undeclared identifier}}
}

struct S1 {
  struct {
    array_ptr<int> lower;
    array_ptr<int> upper;
  } pair;
  array_ptr<int> arr1 : bounds(pair.lower, pair.upper);
  struct {
    array_ptr<int> arr2 : bounds(pair.lower, pair.upper);
  } nested;
};

extern void f5() {
  array_ptr<int> a : count(2) = 0;
  struct S1 *p;
  int *b;
  p = _Assume_bounds_cast<struct S1 *>(a);
}

extern void f6() {
  int *p;
  ptr<int> q = 0;
  array_ptr<int> s : bounds(p, p + 5) = 0;
  p = _Assume_bounds_cast<int *>(q); // expected-error {{inferred bounds for 's' are unknown after assignment}}
  p = _Assume_bounds_cast<int *>(s); // expected-error {{inferred bounds for 's' are unknown after assignment}}
}

extern array_ptr<int> h4(void) : count(3) {
  array_ptr<int> p : bounds(p, p + 3) = 0;
  return p;
}

extern void f7(void *p) {
  array_ptr<int> r : count(3) = 0;
  ptr<int> q = 0;
  r = _Assume_bounds_cast<array_ptr<int>>(p, count(3));
  q = _Assume_bounds_cast<ptr<int>>(h4());
}

extern void f10() {
  int *p;
  ptr<int> q = 0;
  ptr<int> r = 0;
  ptr<int *> s = 0;
  ptr<ptr<int>> t = 0;
  r = _Assume_bounds_cast<ptr<int>>(q);
  p = _Assume_bounds_cast<int *>(q);

  r = _Dynamic_bounds_cast<ptr<int>>(q);
  p = _Dynamic_bounds_cast<int *>(q);

  s = _Assume_bounds_cast<ptr<int *>>(q);
  t = _Assume_bounds_cast<ptr<ptr<int>>>(q);

  s = ynamic_bounds_cast<ptr<int *>>(q); // expected-error {{use of undeclared identifier}}
  t = _Dynamic_bounds_cast<ptr<ptr<int>>>(q);

  r = _Assume_bounds_cast<ptr<int>>(q);
  p = _Assume_bounds_cast<int *>(q);
  p = _Dssume_bounds_cast<int *>(h5); // expected-error 2 {{use of undeclared identifier}}
}

extern void f11() {
  array_ptr<int> r : count(3) = 0;
  ptr<int> q = 0;

  // The declared bounds of h4() use the value of r, but r is overwritten
  // in the assignment. The value of r (used in the declared bounds (r, r + 4))
  // is lost, so the inferred bounds for the cast expression are unknown.
  r = _Assume_bounds_cast<array_ptr<int>>(h4(), bounds(r, r + 4)  rel_align(int)); // expected-error {{inferred bounds for 'r' are unknown after assignment}}

  // The declared bounds of h4() do not use the value of r, so the bounds of the
  // cast expression are not invalidated.
  r = _Assume_bounds_cast<array_ptr<int>>(h4(), count(4));

  q = _Assume_bounds_cast<ptr<int>>(h4());
}

extern void f12() {
  int *p;
  array_ptr<int> q : count(3) = 0;
  ptr<int> r = 0;
  ptr<int *> s = 0;
  ptr<ptr<int>> t = 0;
  array_ptr<int> qq : count(4) = 0;
  r = _Assume_bounds_cast<ptr<int>>(q);
  p = _Assume_bounds_cast<int *>(q);

  s = _Assume_bounds_cast<ptr<int *>>(q);
  t = _Assume_bounds_cast<ptr<ptr<int>>>(q);

  qq = _Assume_bounds_cast<array_ptr<int>>(q, bounds(q, q + 4) rel_align(int));
  p = _Assume_bounds_cast<int *>(q);
}

extern void f13(array_ptr<int> arr : count(5)) {
  int p[10];
  array_ptr<int> x : count(10) = 0;
  array_ptr<int> q : count(10) = 0;
  int len = 5;
  static array_ptr<int> cache1 : count(5);

  x = _Dynamic_bounds_cast<array_ptr<int>>(p, count(10));
  x = _Dynamic_bounds_cast<array_ptr<int>>(p, bounds(p, p + 10));
  x = _Dynamic_bounds_cast<array_ptr<int>>(p, bounds(cache1 - 2, cache1 + 3)); // expected-error {{it is not possible to prove that the inferred bounds of 'x' imply the declared bounds of 'x' after assignment}}
  x = _Dynamic_bounds_cast<array_ptr<int>>(x, bounds(arr, arr + len));  // expected-error {{it is not possible to prove that the inferred bounds of 'x' imply the declared bounds of 'x' after assignment}}
  x = _Dynamic_bounds_cast<array_ptr<int>>(x, bounds(arr)); // expected-error {{expected ','}}
  x = _Dynamic_bounds_cast<array_ptr<int>>(x, count(3 + 2));// expected-error {{declared bounds for 'x' are invalid after assignment}}
  x = _Dynamic_bounds_cast<array_ptr<int>>(x, count(len));  // expected-warning {{cannot prove declared bounds for 'x' are valid after assignment}}
}
