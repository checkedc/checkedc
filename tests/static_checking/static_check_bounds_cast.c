// Feature tests of static checking of Pointer Bounds Cast
// The following lines are for the LLVM test harness:
//
// RUN: %clang_cc1 -verify -verify-ignore-unexpected=note  %s

#include <stdchecked.h>

extern void f1() {
  array_ptr<int> a : count(1) = 0;
  int i;
  ptr<int> c = 0;
  int b[10];

  int *p = 0;
  array_ptr<int> checkedc_p : bounds(checkedc_p, checkedc_p + 1) = 0;
  c = _Dynamic_bounds_cast<ptr<int>>(p); // expected-error {{expression has unknown bounds}}
  c = _Dynamic_bounds_cast<ptr<int>>(p); // expected-error {{expression has unknown bounds}}
  a = _Assume_bounds_cast<array_ptr<int>>(p, count(4));
  checkedc_p = _Assume_bounds_cast<array_ptr<int>>(p, bounds(p, p + 1));
  checkedc_p = _Dynamic_bounds_cast<array_ptr<int>>(p, bounds(p, p + 1)); // expected-error {{expression has unknown bounds}}
  a = _Assume_bounds_cast<array_ptr<int>>(p, count(1));
  a = _Assume_bounds_cast<array_ptr<int>>(p, bounds(p, p + 1));
  array_ptr<int> d = _Assume_bounds_cast<array_ptr<int>>(p, count(4));
  c = _Dynamic_bounds_cast<ptr<int>>(p); // expected-error {{expression has unknown bounds}}
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

extern void f2() {
  array_ptr<int> a : count(2) = 0;
  struct S1 *p = 0;
  a = _Dynamic_bounds_cast<array_ptr<int>>(p, count(2)); // expected-error {{expression has unknown bounds}}
}

extern void f3() {
  int *p = 0;
  ptr<int> q = 0;
  array_ptr<int> r = 0;
  array_ptr<int> s1 = 0;
  array_ptr<int> s2 : bounds(r, r + 5) = 0;
  array_ptr<int> t : count(5) = 0;
  p = _Assume_bounds_cast<int *>(r);
  p = _Dynamic_bounds_cast<int *>(r); // expected-error {{expression has unknown bounds}}
  q = _Assume_bounds_cast<ptr<int>>(p);
  q = _Dynamic_bounds_cast<ptr<int>>(p); // expected-error {{expression has unknown bounds}}
  q = _Dynamic_bounds_cast<ptr<int>>(r); // expected-error {{expression has unknown bounds}}
  q = _Dynamic_bounds_cast<ptr<int>>(r) + 3; // expected-error{{arithmetic on _Ptr type}}

  *(_Assume_bounds_cast<ptr<int>>(r) + 2) = 4; // expected-error{{arithmetic on _Ptr type}}
  // For the statement below, the compiler figures out that r + 2 is out of bounds r : count(1).
  // r : count(1) normals to bounds(r, r + 1), and r + 2 is out of that range.
  *(_Dynamic_bounds_cast<array_ptr<int>>(r, count(1)) + 2) = 4; // expected-error {{expression has unknown bounds}} expected-error {{out-of-bounds memory access}}
  // For the statement below, the compiler figures out that t[3] is out of bounds t : count(3).
  // t : count(3) normals to bounds(t, t + 3), and t[3] is out of that range.
  int n = (_Dynamic_bounds_cast<array_ptr<int>>(t, count(3)))[3]; // expected-error {{out-of-bounds memory access}}
  s1 = _Dynamic_bounds_cast<array_ptr<int>>(p, count(5)); // expected-error {{expression has unknown bounds}}
  s2 = _Assume_bounds_cast<array_ptr<int>>(r, count(5));
}

extern ptr<int> f4(int arr checked[]) {
  return _Dynamic_bounds_cast<ptr<int>>(arr); // expected-error{{expression has unknown bounds}}
}

checked int *f5(int *p, ptr<int> q, array_ptr<int> r, array_ptr<int> s: count(2)) unchecked { // expected-error {{return in a checked scope must have a checked type or a bounds-safe interface}} \
     expected-error {{parameter in a checked scope must have a checked type or a bounds-safe interface}}
  ptr<int> t8 = 0;
  t8 = _Assume_bounds_cast<ptr<int>>(r, count(1)); // expected-error{{expected _Array_ptr type}}
  int a = 5;
  checked {
    *q = 2;
    *(_Dynamic_bounds_cast<ptr<int>>(r, count(1))) = 3; // expected-error{{expected _Array_ptr type}}
    *s = 4;
    unchecked {
      int b checked[5][5];
      for (int i = 0; i < 5; i++) {
        for (int j = 0; j < 5; j++) {
          b[i][j] += *q + *(_Dynamic_bounds_cast<array_ptr<int>>(r, count(1))); // expected-error {{expression has unknown bounds}}
        }
      }
    }
  }
  return 0;
}

extern array_ptr<int> h5(void) {
  array_ptr<int> k = 0;
  return k;
}

extern void f6() {
  int *p;
  p = _Dynamic_bounds_cast<int *>(h5); // expected-error{{expected h5 to have a pointer, array, or integer type}}
}

extern int *f7(int arr checked[]) {
  int k;
  return _Dynamic_bounds_cast<int *>(k); // expected-error{{expression has unknown bounds}}
}

extern void f18(int i) {
  int c;
  int *p = 0;
  char *cp = 0;
  ptr<int> q = 0;
  ptr<char> cq = 0;
  array_ptr<int> r : count(5) = 0;
  array_ptr<char> cr = 0;

  p = _Dynamic_bounds_cast<int *>(p); // expected-error{{expression has unknown bounds}}
  p = _Dynamic_bounds_cast<char *>(p);   // expected-warning {{incompatible pointer type}} expected-error {{expression has unknown bounds}}

  p = _Dynamic_bounds_cast<int *>(i); // expected-error {{expression has unknown bounds}}
  p = _Dynamic_bounds_cast<char *>(i); // expected-error {{expression has unknown bounds}} expected-warning {{incompatible pointer}}

  p = _Dynamic_bounds_cast<int *>(q);

  p = _Dynamic_bounds_cast<int *>(q, bounds(q, q + 1)); // expected-error {{arithmetic on _Ptr}}

  p = _Dynamic_bounds_cast<int *>(r);
  p = _Dynamic_bounds_cast<int *>(r, count(1)); // expected-error {{expected _Array_ptr type}}
  p = _Dynamic_bounds_cast<int *>(r, bounds(r, r + 1)); // expected-error {{expected _Array_ptr type}}

  q = _Dynamic_bounds_cast<ptr<int>>(p); // expected-error {{expression has unknown bounds}}
  q = _Dynamic_bounds_cast<ptr<char>>(p); // expected-error {{assigning to '_Ptr<int>'}}

  q = _Dynamic_bounds_cast<ptr<int>>(i); // expected-error {{expression has unknown bounds}}
  q = _Dynamic_bounds_cast<ptr<char>>(i); // expected-error{{assigning to '_Ptr<int>'}}

  q = _Dynamic_bounds_cast<ptr<int>>(q);
  q = _Dynamic_bounds_cast<ptr<int>>(q, bounds(q, q + 1)); // expected-error {{arithmetic on _Ptr}}

  q = _Dynamic_bounds_cast<ptr<int>>(r);

  r = _Dynamic_bounds_cast<array_ptr<int>>(p, count(1)); // expected-error {{expression has unknown bounds}} expected-error {{declared bounds for 'r' are invalid after assignment}}
  r = _Dynamic_bounds_cast<array_ptr<int>>(p, bounds(p, p + 1)); // expected-error {{expression has unknown bounds}} expected-error {{declared bounds for 'r' are invalid after assignment}}

  r = _Dynamic_bounds_cast<array_ptr<int>>(i, count(5)); // expected-error {{expression has unknown bounds}}
  r = _Dynamic_bounds_cast<array_ptr<int>>(i, bounds(i, i + 1)); // expected-error 2 {{expected expression with pointer}}

  int len;

  r = _Dynamic_bounds_cast<array_ptr<int>>(q, count(len));       // expected-error {{it is not possible to prove that the inferred bounds of 'r' imply the declared bounds of 'r' after assignment}}
  r = _Dynamic_bounds_cast<array_ptr<int>>(q, bounds(q, q + 1)); // expected-error {{arithmetic on _Ptr type}}

  r = _Dynamic_bounds_cast<array_ptr<int>>(r, count(1));        // expected-error {{declared bounds for 'r' are invalid after assignment}}
  r = _Dynamic_bounds_cast<array_ptr<int>>(r, bounds(r, r + 1)); // expected-error {{declared bounds for 'r' are invalid after assignment}}

  p = _Dynamic_bounds_cast<char *>(p); // expected-warning{{incompatible pointer types assigning}} expected-error{{expression has unknown bounds}}

  p = _Assume_bounds_cast<int *>(q);
  p = _Assume_bounds_cast<int *>(cq);
  p = _Assume_bounds_cast<int *>(cr);
  p = _Dynamic_bounds_cast<int *>(cr); // expected-error{{expression has unknown bounds}}
  cp = _Dynamic_bounds_cast<char *>(p); // expected-error{{expression has unknown bounds}}
  q = _Dynamic_bounds_cast<ptr<int>>(p); // expected-error {{expression has unknown bounds}}
  p = _Assume_bounds_cast<int *>(r);
}

extern float h6(void) {
  float k = 0;
  return k;
}

extern void f19(){
  int p[10];
  int a : bounds(p, p+1) = 0;
  float b;
  array_ptr<int> x : count(10) = 0;

  p = _Dynamic_bounds_cast<int *>(h6); // expected-error {{to have a pointer}}
  p = _Dynamic_bounds_cast<int *>(h6, count(2)); // expected-error {{to have a pointer}}
  p = _Dynamic_bounds_cast<int *>(h6, bounds(p, p + 1)); // expected-error {{to have a pointer}}
  x = _Dynamic_bounds_cast<array_ptr<int>>(a, count(10));
  x = _Dynamic_bounds_cast<int *>(b); // expected-error {{to have a pointer}}
  x = _Dynamic_bounds_cast<array_ptr<int>>(b, count(1)); // expected-error {{to have a pointer}}
  x = _Dynamic_bounds_cast<array_ptr<int>>(b, bounds(p, p + 1)); // expected-error {{to have a pointer}}

  x = _Dynamic_bounds_cast<array_ptr<int>>(p, count(b)); // expected-error {{invalid argument type}}
  x = _Dynamic_bounds_cast<array_ptr<int>>(p, bounds(p, 1)); // expected-error {{expected expression with}}
  x = _Dynamic_bounds_cast<array_ptr<int>>(p, bounds(p, p + 1)); // expected-error {{declared bounds for 'x' are invalid after assignment}}
}

extern array_ptr<int> h7(void) : count(3) {
  array_ptr<int> p : bounds(p, p + 3) = 0;
  return p;
}

extern array_ptr<char> h8(void) : count(8) {
  array_ptr<char> buf : count(8) = 0;
  return buf;
}

extern void f20(void *p) {
  array_ptr<int> intbuf : count(3) = 0;
  intbuf = _Assume_bounds_cast<array_ptr<int>>(h7(), count(3));
  int i = intbuf[2];
}

extern void f21(array_ptr<char> buf : count(len), int len) {
  array_ptr<int> intbuf : count(12) = _Dynamic_bounds_cast<array_ptr<int>>(buf, bounds(intbuf, intbuf + 12));
  int i = intbuf[12]; // expected-error {{out-of-bounds memory access}} \
                      // expected-note {{accesses memory at or above the upper bound}} \
                      // expected-note {{(expanded) inferred bounds are 'bounds(intbuf, intbuf + 12)'}}
}

extern void f22() {
  array_ptr<int> intbuf : count(2) = _Dynamic_bounds_cast<array_ptr<int>>(h8(), count(2));
  int i = intbuf[2]; // expected-error {{out-of-bounds memory access}} \
                     // expected-note {{accesses memory at or above the upper bound}} \
                     // expected-note {{(expanded) inferred bounds are 'bounds(intbuf, intbuf + 2)'}}
}

extern void f23() {
  array_ptr<char> buf : count(10) = _Assume_bounds_cast<array_ptr<char>>(h7(), count(10));
  char c = buf[10]; // expected-error {{out-of-bounds memory access}} \
                    // expected-note {{accesses memory at or above the upper bound}} \
                    // expected-note {{(expanded) inferred bounds are 'bounds(buf, buf + 10)'}}
}

extern void f24() {
  array_ptr<char> buf : count(3) = "abc";

  // The declared bounds of h7() use the value of buf, but buf is overwritten
  // in the assignment. The value of buf (used in the declared bounds (buf, buf + 3))
  // is lost, so the inferred bounds for the cast expression are unknown.
  buf = _Dynamic_bounds_cast<array_ptr<char>>(h7(), bounds(buf, buf + 3)); // expected-error {{inferred bounds for 'buf' are unknown after assignment}}

  // The declared bounds of h7() do not use the value of buf, so the bounds of the
  // cast expression are not invalidated.
  buf = _Dynamic_bounds_cast<array_ptr<char>>(h7(), count(3));

  char c = buf[3]; // expected-error {{out-of-bounds memory access}} \
                   // expected-note {{accesses memory at or above the upper bound}} \
                   // expected-note {{(expanded) inferred bounds are 'bounds(buf, buf + 3)'}}
}

extern void f25(array_ptr<char> buf : count(len), int len) {
  array_ptr<int> intbuf : count(6) = _Dynamic_bounds_cast<array_ptr<int>>(buf + 5, count(6));
  int i = intbuf[6]; // expected-error {{out-of-bounds memory access}} \
                     // expected-note {{accesses memory at or above the upper bound}} \
                     // expected-note {{(expanded) inferred bounds are 'bounds(intbuf, intbuf + 6)'}}
}

