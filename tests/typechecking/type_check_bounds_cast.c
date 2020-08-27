// Feature tests of parsing new Checked C dynamic and assume bounds
// cast. The following lines are for the LLVM test harness:
//
// RUN: %clang_cc1 -verify -verify-ignore-unexpected=note %s

#include <stdchecked.h>

extern void f1() {
  array_ptr<int> a : count(1) = 0;
  int i;
  ptr<int> c = 0;
  int b[10];
  int p[10];

  array_ptr<int> checkedc_p : bounds(checkedc_p, checkedc_p + 1) = 0;
  a = _Dynamic_bounds_cast<array_ptr<int>>(b, count(10));
  c = _Dynamic_bounds_cast<int>(p); // expected-error {{expected _Ptr or * type}}
}

extern void f2() {
  char p[10];
  array_ptr<int> a : count(1) = 0;
  int b checked[10];
  array_ptr<int> c : count(10) = (array_ptr<int>)a; // expected-error {{declared bounds for 'c' are invalid after initialization}}
  array_ptr<int> d : count(10) = _Dynamic_bounds_cast<array_ptr<int>>(a, count(5)); // expected-error {{declared bounds for 'd' are invalid after initialization}}
  a = _Assume_bounds_cast<array_ptr<int>>(p); // expected-error {{expected _Ptr or * type}}
}

extern void f3() {
  char p[10];
  ptr<int> c = 0;
  array_ptr<int> a : count(2) = 0;
  a = _Assume_bounds_cast<array_ptr<int>>(p, count(p + 2)); // expected-error {{invalid argument type 'char *' to count expression}}
  c = _Assume_bounds_cast<ptr<int>>(p, count(1)); // expected-error {{expected _Array_ptr type}}
}

extern void f4() {
  array_ptr<int> a : count(2) = 0;
  array_ptr<char> b : count(2) = 0;

  b = _Assume_bounds_cast<array_ptr<char>>(a, count(2));
  a = _Assume_bounds_cast<array_ptr<int>>(b); // expected-error {{expected _Ptr or * type}}
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
  int *p;
  ptr<int> q = 0;
  array_ptr<int> r;
  array_ptr<int> s : bounds(r, r + 5) = 0;
  p = _Assume_bounds_cast<int *>(q);
  p = _Assume_bounds_cast<int *>(s);
}

extern array_ptr<int> h4(void) : count(3) {
  array_ptr<int> p : bounds(p, p + 3) = 0;
  return p;
}

extern void f6() {
  int i[2];
  array_ptr<int> int_array_ptr_lb = i, int_array_ptr_ub = i + 1;
  ptr<int> int_ptr_lb = i, int_ptr_ub = i + 1;
  int *int_unchecked_ptr_lb = i, *int_unchecked_ptr_ub = i + 1;

  array_ptr<char> char_array_ptr_lb = (array_ptr<char>)i,
                  char_array_ptr_ub = (array_ptr<char>)i + 1;
  ptr<char> char_ptr_lb = (ptr<char>)i, char_ptr_ub = (ptr<char>)(i + 1);
  char *char_unchecked_ptr_lb = (char *)i,
       *char_unchecked_ptr_ub = (char *)i + 1;

  array_ptr<int> t20 : bounds(int_array_ptr_lb, char_array_ptr_ub) = // expected-error {{pointer type mismatch}}
        _Assume_bounds_cast<array_ptr<int>>(i, bounds(int_array_ptr_lb, char_array_ptr_ub)); // expected-error {{pointer type mismatch}}

  array_ptr<int> t21 : bounds(int_ptr_lb, char_array_ptr_ub) = // expected-error {{pointer type mismatch}}
        _Assume_bounds_cast<array_ptr<int>>(i, bounds(int_ptr_lb, int_array_ptr_ub));
}

extern void f7(void *p) {
  array_ptr<int> r : count(3) = 0;
  ptr<int> q = 0;
  r = _Assume_bounds_cast<array_ptr<int>>(p, count(3));
  q = _Assume_bounds_cast<ptr<int>>(h4());
}

extern void f8() {
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
  t = _Dynamic_bounds_cast<ptr<ptr<int>>>(q);
  r = _Assume_bounds_cast<ptr<int>>(q);
  p = _Assume_bounds_cast<int *>(q);

}

extern void f9() {
  array_ptr<int> r : count(3) = 0;
  ptr<int> q = 0;

  // The declared bounds of h4() use the value of r, but r is overwritten
  // in the assignment. The value of r (used in the declared bounds (r, r + 4))
  // is lost, so the inferred bounds for the cast expression are unknown.
  r = _Assume_bounds_cast<array_ptr<int>>(h4(), bounds(r, r + 4) rel_align(int)); // expected-error {{inferred bounds for 'r' are unknown after assignment}}
  
  // The declared bounds of h4() do not use the value of r, so the bounds of the
  // cast expression are not invalidated.
  r = _Assume_bounds_cast<array_ptr<int>>(h4(), count(4));

  q = _Assume_bounds_cast<ptr<int>>(h4(), bounds(r, r + 4) rel_align_value(sizeof(int))); // expected-error {{expected _Array_ptr type}}
}

extern void f10() {
  int *p;
  array_ptr<int> q : count(3) = 0;
  ptr<int> r = 0;
  ptr<int *> s = 0;
  ptr<ptr<int>> t = 0;
  array_ptr<int> qq : count(4) = 0;
  r = _Assume_bounds_cast<ptr<int>>(q, bounds(q, q + 4) rel_align(int)); // expected-error {{expected _Array_ptr type}}
  p = _Assume_bounds_cast<int *>(q, bounds(q, q + 4) rel_align_value(sizeof(int))); // expected-error {{expected _Array_ptr type}}

  s = _Assume_bounds_cast<ptr<int *>>(q, bounds(q, q + 4) rel_align(int)); // expected-error {{expected _Array_ptr type}}
  t = _Assume_bounds_cast<ptr<ptr<int>>>(q, bounds(q, q + 4) rel_align_value(sizeof(int))); // expected-error {{expected _Array_ptr type}}

  qq = _Assume_bounds_cast<array_ptr<int>>(q, bounds(q, q + 4)  rel_align(int)) ;
  p = _Assume_bounds_cast<int *>(q, bounds(q, q + 4) rel_align_value(sizeof(int))); // expected-error {{expected _Array_ptr type}}
}

extern void f11() {
  int c, i;
  int *p;
  int len = 5;
  array_ptr<int> q : count(2) = 0;
  ptr<int> r = 0;
  ptr<char> cq = 0;
  array_ptr<char> rr;
  array_ptr<int> rrr;
  
  r = _Dynamic_bounds_cast<ptr<int>>(q, bounds(q, q + 4) rel_align(len)); // expected-error {{unknown type name 'len'}}
  p = _Dynamic_bounds_cast<int *>(q, bounds(q, q + 4) rel_align_value(len)); // expected-error {{expression is not an integer constant expression}} expected-error {{expected _Array_ptr type}}
  p = _Dynamic_bounds_cast<int *>(q, bounds(q, q + 4) rel_align_value(1)); // expected-error {{expected _Array_ptr type}}

  c = _Assume_bounds_cast<int>(cq); // expected-error {{expected _Ptr or * type}}
  p = _Dynamic_bounds_cast<char>(p, bounds(p, p + 2)); // expected-error {{expected _Array_ptr type}}
  p = _Dynamic_bounds_cast<char>(p); // expected-error {{expected _Ptr or * type}}
  r = _Dynamic_bounds_cast<array_ptr<int>>(r); // expected-error{{expected _Ptr or * type}}
  r = _Dynamic_bounds_cast<array_ptr<int>>(q); // expected-error {{expected _Ptr or * type}}
  r = _Dynamic_bounds_cast<array_ptr<int>>(i); // expected-error {{expected _Ptr or * type}}
  r = _Dynamic_bounds_cast<array_ptr<char>>(i); // expected-error {{expected _Ptr or * type}}
  r = _Dynamic_bounds_cast<array_ptr<int>>(p); // expected-error {{expected _Ptr or * type}}
  rr = _Dynamic_bounds_cast<array_ptr<int>>(p); // expected-error {{expected _Ptr or * type}}
  rrr = _Dynamic_bounds_cast<array_ptr<int>>(p); // expected-error {{expected _Ptr or * type}}
  r = _Dynamic_bounds_cast<array_ptr<char>>(p); // expected-error {{expected _Ptr or * type}}
  q = _Dynamic_bounds_cast<ptr<int>>(rr, count(1)); // expected-error{{expected _Array_ptr type}}
  q = _Dynamic_bounds_cast<ptr<int>>(rr, bounds(rr, rr + 1)); // expected-error{{expected _Array_ptr type}}

  p = _Dynamic_bounds_cast<int *>(q, count(1)); // expected-error {{expected _Array_ptr type}}
  q = _Dynamic_bounds_cast<ptr<int>>(q, count(1)); // expected-error {{expected _Array_ptr type}}
  q = _Dynamic_bounds_cast<ptr<int>>(i, count(1)); // expected-error {{expected _Array_ptr type}}
  p = _Dynamic_bounds_cast<int *>(i, count(1)); // expected-error {{expected _Array_ptr type}}
  q = _Dynamic_bounds_cast<ptr<int>>(i, bounds(i, i + 1)); // expected-error 2 {{expected expression with pointer type}}
  p = _Dynamic_bounds_cast<int *>(i, bounds(i, i + 1)); // expected-error 2 {{expected expression with pointer type}}

  q = _Dynamic_bounds_cast<ptr<int>>(p, count(1)); // expected-error {{expected _Array_ptr type}}
  p = _Dynamic_bounds_cast<int *>(p, count(1)); // expected-error {{expected _Array_ptr type}}
  q = _Dynamic_bounds_cast<ptr<int>>(p, bounds(p, p + 1)); // expected-error {{expected _Array_ptr type}}
  p = _Dynamic_bounds_cast<int *>(p, bounds(p, p + 1)); // expected-error {{expected _Array_ptr type}}
  
  q = _Assume_bounds_cast<ptr<int>>(r, count(1)) + 3; // expected-error{{expected _Array_ptr type}}
  c = _Dynamic_bounds_cast<ptr<int>>(p, count(4)); // expected-error{{expected _Array_ptr type}}
}

