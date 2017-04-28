// Feature tests of static checking of Pointer Bounds Cast
// The following lines are for the LLVM test harness:
//
// RUN: %clang_cc1 -verify -fcheckedc-extension %s

#include "../../include/stdchecked.h"

extern void f1() {
  array_ptr<int> a : count(1) = 0;
  int i;
  ptr<int> c = 0;
  int b[10];

  int *p;
  array_ptr<int> checkedc_p : bounds(checkedc_p, checkedc_p + 1) = 0;
  c = _Dynamic_bounds_cast<ptr<int>>(p); // expected-error {{expression has no bounds}}
  c = _Dynamic_bounds_cast<ptr<int>>(p); // expected-error {{expression has no bounds}}
  a = _Assume_bounds_cast<array_ptr<int>>(p, 4);
  checkedc_p = _Assume_bounds_cast<array_ptr<int>>(p, p, p + 1);
  checkedc_p = _Dynamic_bounds_cast<array_ptr<int>>(p, p, p + 1); // expected-error {{expression has no bounds}}
  a = _Assume_bounds_cast<array_ptr<int>>(p, 1);
  a = _Assume_bounds_cast<array_ptr<int>>(p, p, p + 1);
  array_ptr<int> d = _Assume_bounds_cast<array_ptr<int>>(p, 4); 
  c = _Dynamic_bounds_cast<ptr<int>>(p); // expected-error {{expression has no bounds}}
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
  struct S1 *p;
  a = _Dynamic_bounds_cast<array_ptr<int>>(p, 2); // expected-error {{expression has no bounds}}
}

extern void f3() {
  int *p;
  ptr<int> q = 0;
  array_ptr<int> r;
  array_ptr<int> s : bounds(r, r + 5) = 0;
  p = _Assume_bounds_cast<int *>(r);
  p = _Dynamic_bounds_cast<int *>(r); // expected-error {{expression has no bounds}}  
  q = _Assume_bounds_cast<ptr<int>>(p);
  q = _Dynamic_bounds_cast<ptr<int>>(p); // expected-error {{expression has no bounds}}
  q = _Dynamic_bounds_cast<ptr<int>>(r); // expected-error {{expression has no bounds}}
  q = _Dynamic_bounds_cast<ptr<int>>(r) + 3; // expected-error{{arithmetic on _Ptr type}}

  *(_Assume_bounds_cast<ptr<int>>(r) + 2) = 4; // expected-error{{arithmetic on _Ptr type}}
  *(_Dynamic_bounds_cast<array_ptr<int>>(r, 1) + 2) = 4; // expected-error {{expression has no bounds}}
  s = _Dynamic_bounds_cast<array_ptr<int>>(p, 5); // expected-error {{expression has no bounds}}
  s = _Assume_bounds_cast<array_ptr<int>>(r, 5); 
}

extern ptr<int> f4(int arr checked[]) {
  return _Dynamic_bounds_cast<ptr<int>>(arr); // expected-error{{expression has no bounds}}
}

checked int *f5(int *p, ptr<int> q, array_ptr<int> r, array_ptr<int> s: count(2)) unchecked { // expected-error {{return cannot have an unchecked pointer type}} expected-error {{parameter cannot have an unchecked pointer type}}
  ptr<int> t8 = 0;
  t8 = _Assume_bounds_cast<ptr<int>>(r, 1); // expected-error{{invalid bounds cast}}
  int a = 5;
  checked {
    *q = 2;
    *(_Dynamic_bounds_cast<ptr<int>>(r, 1)) = 3; // expected-error{{invalid bounds cast}}
    *s = 4;
    unchecked {
      int b checked[5][5];
      for (int i = 0; i < 5; i++) {
        for (int j = 0; j < 5; j++) {
          b[i][j] += *q + *(_Dynamic_bounds_cast<array_ptr<int>>(r, 1)); // expected-error {{expression has no bounds}}
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
  p = _Dynamic_bounds_cast<int *>(h5); // expected-error{{expression has no bounds}}
}

extern int *f7(int arr checked[]) {
  int k;
  return _Dynamic_bounds_cast<int *>(k); // expected-error{{expression has no bounds}}
}

extern void f18(int i) {
  int c;
  int *p;
  char *cp;
  ptr<int> q = 0;
  ptr<char> cq = 0;
  array_ptr<int> r : count(5) = 0;
  array_ptr<char> cr = 0;

  p = _Dynamic_bounds_cast<int *>(p); // expected-error{{expression has no bounds}}
  p = _Dynamic_bounds_cast<char *>(p);   // expected-warning {{incompatible pointer type}} expected-error {{expression has no bounds}}

  p = _Dynamic_bounds_cast<int *>(i); // expected-error {{expression has no bounds}}
  p = _Dynamic_bounds_cast<char *>(i); // expected-error {{expression has no bounds}} expected-warning {{incompatible pointer}}

  p = _Dynamic_bounds_cast<int *>(q);

  p = _Dynamic_bounds_cast<int *>(q, q, q + 1); // expected-error {{arithmetic on _Ptr}}

  p = _Dynamic_bounds_cast<int *>(r);
  p = _Dynamic_bounds_cast<int *>(r, 1); // expected-error {{invalid bounds cast}}
  p = _Dynamic_bounds_cast<int *>(r, r, r + 1); // expected-error {{invalid bounds cast}}

  q = _Dynamic_bounds_cast<ptr<int>>(p); // expected-error {{expression has no bounds}}
  q = _Dynamic_bounds_cast<ptr<char>>(p); // expected-error {{assigning to '_Ptr<int>'}}

  q = _Dynamic_bounds_cast<ptr<int>>(i); // expected-error {{expression has no bounds}}
  q = _Dynamic_bounds_cast<ptr<char>>(i); // expected-error{{assigning to '_Ptr<int>'}}

  q = _Dynamic_bounds_cast<ptr<int>>(q);
  q = _Dynamic_bounds_cast<ptr<int>>(q, q, q + 1); // expected-error {{arithmetic on _Ptr}}

  q = _Dynamic_bounds_cast<ptr<int>>(r);

  r = _Dynamic_bounds_cast<array_ptr<int>>(p, 1); // expected-error {{expression has no bounds}}
  r = _Dynamic_bounds_cast<array_ptr<int>>(p, p, p + 1); // expected-error {{expression has no bounds}}

  r = _Dynamic_bounds_cast<array_ptr<int>>(i, 1); // expected-error {{expression has no bounds}}
  r = _Dynamic_bounds_cast<array_ptr<int>>(i, i, i + 1); // expected-error 2 {{expected expression with pointer}}

  int len;

  r = _Dynamic_bounds_cast<array_ptr<int>>(q, len);
  r = _Dynamic_bounds_cast<array_ptr<int>>(q, q, q + 1); // expected-error {{arithmetic on _Ptr type}}

  r = _Dynamic_bounds_cast<array_ptr<int>>(r, 1);
  r = _Dynamic_bounds_cast<array_ptr<int>>(r, r, r + 1);

  p = _Dynamic_bounds_cast<char *>(p); // expected-warning{{incompatible pointer types assigning}} expected-error{{expression has no bounds}}

  p = _Assume_bounds_cast<int *>(q);
  p = _Assume_bounds_cast<int *>(cq);
  p = _Assume_bounds_cast<int *>(cr);
  p = _Dynamic_bounds_cast<int *>(cr); // expected-error{{expression has no bounds}}  
  cp = _Dynamic_bounds_cast<char *>(p); // expected-error{{expression has no bounds}}
  q = _Dynamic_bounds_cast<ptr<int>>(p); // expected-error {{expression has no bounds}}
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
  p = _Dynamic_bounds_cast<int *>(h6, 2); // expected-error {{to have a pointer}}
  p = _Dynamic_bounds_cast<int *>(h6, p, p + 1); // expected-error {{to have a pointer}}
  x = _Dynamic_bounds_cast<array_ptr<int>>(a, 10);
  x = _Dynamic_bounds_cast<int *>(b); // expected-error {{to have a pointer}}
  x = _Dynamic_bounds_cast<array_ptr<int>>(b, 1); // expected-error {{to have a pointer}}
  x = _Dynamic_bounds_cast<array_ptr<int>>(b, p, p + 1); // expected-error {{to have a pointer}}

  x = _Dynamic_bounds_cast<array_ptr<int>>(p, b); // expected-error {{invalid argument type}}
  x = _Dynamic_bounds_cast<array_ptr<int>>(p, p, 1); // expected-error {{expected expression with}}
  x = _Dynamic_bounds_cast<array_ptr<int>>(p, p, p + 1);
}

