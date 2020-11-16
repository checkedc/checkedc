// This file tests the compiler diagnostic messages for free variables
// in the inferred or declared bounds that cause the bounds proof to fail.
// See checkedc-clang#903 for the definition of free variables.

// RUN: %clang_cc1 -verify %s

#include <stdchecked.h>

void f0(void) {
  int a[] = { 1, 2, 3, 4, 5 };
  int len2 = 6, i1, i2;
  unsigned len1;

  int x1, x2, x3;
  i1 = x1;
  i2 = x2;
  x3 = x2;

  // i1 free
  array_ptr<int> p : count(i1) = a; // expected-error {{it is not possible to prove that the inferred bounds of 'p' imply the declared bounds of 'p' after initialization}} \
                                                                    // expected-note {{the declared upper bounds use the variable 'i1' and there is no relational information involving 'i1' and any of the expressions used by the inferred upper bounds}} \
                                                                    // expected-note {{declared bounds are 'bounds(p, p + i1)'}} \
                                                                    // expected-note {{inferred bounds are 'bounds(a, a + 5)'}}
  // i1, i2 free
  array_ptr<int> q : count(i2) = p; // expected-error {{it is not possible to prove that the inferred bounds of 'q' imply the declared bounds of 'q' after initialization}} \
                                    // expected-note {{the inferred upper bounds use the variable 'i1' and there is no relational information involving 'i1' and any of the expressions used by the declared upper bounds}} \
                                    // expected-note {{the declared upper bounds use the variable 'i2' and there is no relational information involving 'i2' and any of the expressions used by the inferred upper bounds}} \
                                    // expected-note {{(expanded) declared bounds are 'bounds(q, q + i2)'}} \
                                    // expected-note {{(expanded) inferred bounds are 'bounds(p, p + i1)'}}

  array_ptr<int> p0 : bounds(p, p + (i1 + i2)) = 0;

  // p1 and p are free.
  // 'i1' and 'i2' are free but not reported!
  // The compiler checks free variables only if the bases are equal.
  array_ptr<int> p1 : count(len1) = p0; // expected-error {{it is not possible to prove that the inferred bounds of 'p1' imply the declared bounds of 'p1' after initialization}} \
                                        // expected-note {{the declared bounds use the variable 'p1' and there is no relational information involving 'p1' and any of the expressions used by the inferred bounds}} \
                                        // expected-note {{the inferred bounds use the variable 'p' and there is no relational information involving 'p' and any of the expressions used by the declared bounds}} \
                                        // expected-note 2 {{(expanded) declared bounds are 'bounds(p1, p1 + len1)'}} \
                                        // expected-note {{(expanded) inferred bounds are 'bounds(p, p + (i1 + i2))'}} \

  // There is an indirect relationship between p1 and a.
  // 'len1' in the declared upper bound of p1 is free but not reported.
  // The compiler checks free variables in the lower and upper bounds
  // only if the bases are equal.
  p1 = &a[0]; // expected-warning {{cannot prove declared bounds for 'p1' are valid after assignment}} \
              // expected-note {{(expanded) inferred bounds are 'bounds(a, a + 5)'}}

  // 'len1' and 'len1' from 'p2' are free.
  array_ptr<int> p2 : bounds(a + len1, a + len2) = 0;
  array_ptr<int> p3 : bounds(a + len2, a + len1) = p2; // expected-error {{it is not possible to prove that the inferred bounds of 'p3' imply the declared bounds of 'p3' after initialization}} \
                                                       // expected-note {{the inferred lower bounds use the variable 'len1' and there is no relational information involving 'len1' and any of the expressions used by the declared lower bounds}} \
                                                       // expected-note {{the declared upper bounds use the variable 'len1' and there is no relational information involving 'len1' and any of the expressions used by the inferred upper bounds}} \
                                                       // expected-note {{(expanded) declared bounds are 'bounds(a + len2, a + len1)'}} \
                                                       // expected-note {{(expanded) inferred bounds are 'bounds(a + len1, a + len2)'}}

  // Compound bases. 'len1' is free.
  array_ptr<int> p4 : bounds((p4 + len1) + 0, (p4 + len1) + 1) = 0;
  array_ptr<int> p5 : bounds((p5 + len2) + 0, (p5 + len2) + 1) = p4; // expected-error {{it is not possible to prove that the inferred bounds of 'p5' imply the declared bounds of 'p5' after initialization}} \
                                                                     // expected-note {{the inferred bounds use the variable 'len1' and there is no relational information involving 'len1' and any of the expressions used by the declared bounds}} \
                                                                     // expected-note {{(expanded) declared bounds are 'bounds((p5 + len2) + 0, (p5 + len2) + 1)'}} \
                                                                     // expected-note {{(expanded) inferred bounds are 'bounds((p4 + len1) + 0, (p4 + len1) + 1)'}}

  // Multiple equivalent variables. Emit note once for 'len1' and 'len2'.
  array_ptr<int> p6 : count(len1 + len1 + len1 + len2) = 0;
  array_ptr<int> p7 : count(i1) = p6; // expected-error {{it is not possible to prove that the inferred bounds of 'p7' imply the declared bounds of 'p7' after initialization}} \
                                      // expected-note {{the declared upper bounds use the variable 'i1' and there is no relational information involving 'i1' and any of the expressions used by the inferred upper bounds}} \
                                      // expected-note {{the inferred upper bounds use the variable 'len1' and there is no relational information involving 'len1' and any of the expressions used by the declared upper bounds}} \
                                      // expected-note {{(expanded) declared bounds are 'bounds(p7, p7 + i1)'}} \
                                      // expected-note {{(expanded) inferred bounds are 'bounds(p6, p6 + len1 + len1 + len1 + len2)'}}

  // a and b equal to some constant. v is free.
  unsigned v;
  array_ptr<int> p8 : count(i1) = 0;
  array_ptr<int> p9 : count(i2 + i1) = 0;
  array_ptr<int> p10 : count(v) = 0; // expected-note 2 {{(expanded) declared bounds are 'bounds(p10, p10 + v)'}}
  p10 = p8; // expected-error {{it is not possible to prove that the inferred bounds of 'p10' imply the declared bounds of 'p10' after assignment}} \
            // expected-note {{the declared upper bounds use the variable 'v' and there is no relational information involving 'v' and any of the expressions used by the inferred upper bounds}} \
            // expected-note {{the inferred upper bounds use the variable 'i1' and there is no relational information involving 'i1' and any of the expressions used by the declared upper bounds}} \
            // expected-note {{(expanded) inferred bounds are 'bounds(p8, p8 + i1)'}}
  p10 = p9; // expected-error {{it is not possible to prove that the inferred bounds of 'p10' imply the declared bounds of 'p10' after assignment}} \
            // expected-note {{the declared upper bounds use the variable 'v' and there is no relational information involving 'v' and any of the expressions used by the inferred upper bounds}} \
            // expected-note {{the inferred upper bounds use the variable 'i2' and there is no relational information involving 'i2' and any of the expressions used by the declared upper bounds}} \
            // expected-note {{the inferred upper bounds use the variable 'i1' and there is no relational information involving 'i1' and any of the expressions used by the declared upper bounds}} \
            // expected-note {{(expanded) inferred bounds are 'bounds(p9, p9 + i2 + i1)'}}
}

void f13() {
  int p[10];
  array_ptr<int> x : count(10) = 0; // expected-note {{(expanded) declared bounds are 'bounds(x, x + 10)'}}
  array_ptr<int> cache1 : count(5) = 0;

  x = _Dynamic_bounds_cast<array_ptr<int>>(p, bounds(cache1 - 2, cache1 + 3)); // expected-error {{declared bounds for 'x' are invalid after assignment}} \
                                                                               // expected-note {{destination bounds are wider than the source bounds}} \
                                                                               // expected-note {{(expanded) inferred bounds are 'bounds(cache1 - 2, cache1 + 3)'}}
}

struct S1 {
	_Array_ptr<int> p : count(len2);
	int len2;
};

void f1(struct S1 *s) {
  // We currently do not detect free variables for indirect accesses.
  array_ptr<int> p : count(5) = 0;
  s->p = p; // expected-warning {{cannot prove declared bounds for s->p are valid after assignment}} \
            // expected-note {{declared bounds are}} \
            // expected-note {{inferred bounds are}}

  int a[] = { 1, 2 };
  array_ptr<int> q : bounds(q, &a[0]) = p; // expected-warning {{cannot prove declared bounds for 'q' are valid after initialization}} \
                                           // expected-note {{declared bounds are}} \
                                           // expected-note {{inferred bounds are}}
  s->p = &a[1]; // expected-warning {{cannot prove declared bounds for s->p are valid after assignment}} \
                // expected-note {{declared bounds are}} \
                // expected-note {{inferred bounds are}}
}

void f2(struct S1 a3) {
  //int l = a3->len2;
  struct S1 a4 = {};
  array_ptr<int> p : count(5) = 0;
  //array_ptr<int> p : count(l) = local_arr1;
  a3 = a4;
}

void f3(struct S1 a3) {
  //int l = a3->len2;
  struct S1 a4 = {};
  array_ptr<int> p : count(5) = 0;
  a3.p = p; // expected-error {{it is not possible to prove that the inferred bounds of a3.p imply the declared bounds of a3.p after assignment}} \
            // expected-note {{the declared upper bounds use the variable 'a3' and there is no relational information involving 'a3' and any of the expressions used by the inferred upper bounds}} \
            // expected-note {{(expanded) declared bounds are 'bounds(a3.p, a3.p + a3.len2)'}} \
            // expected-note {{(expanded) inferred bounds are 'bounds(p, p + 5)'}}
}

void f4(void) {
  int a checked[5];
  // Check that parentheses are correctly ignored.
  short int t1 : byte_count(5 * sizeof(int)) = ((((short int)(a)))); // expected-warning {{cannot prove declared bounds for 't1' are valid after initialization}} \
                                                                     // expected-note {{(expanded) declared bounds are 'bounds((_Array_ptr<char>)t1, (_Array_ptr<char>)t1 + 5 * sizeof(int))'}} \
                                                                     // expected-note {{(expanded) inferred bounds are 'bounds(a, a + 5)'}}
  array_ptr<int> t2 : byte_count(5 * sizeof(int)) = (((a)));
  array_ptr<int> t3 : byte_count(5 * sizeof(int)) = (((array_ptr<int>)(((a)))));

  array_ptr<int> p : count(2) = 0;
  // There is an indirect relationship between q and p.
  array_ptr<int> q : count(1) = p + 1; // expected-warning {{cannot prove declared bounds for 'q' are valid after initialization}} \
                                       // expected-note {{(expanded) declared bounds are 'bounds(q, q + 1)'}} \
                                       // expected-note {{(expanded) inferred bounds are 'bounds(p, p + 2)'}}
}

void g(void) {
  int a, b, c, d, e;
  a = b = 0;
  c = d = 1;

  // Warning but no free variables because 'a == b'.
  array_ptr<int> p1 : count(a) = 0;
  array_ptr<int> p2 : count(b + 1) = p1; // expected-warning {{cannot prove declared bounds for 'p2' are valid after initialization}} \
                                         // expected-note {{declared bounds are}} \
                                         // expected-note {{inferred bounds are}}

  // No free variables. a and c equal to some constants.
  array_ptr<int> p3 : count(3) = 0;
  array_ptr<int> p4 : bounds(p4 + c, p4 + a) = p3; // expected-warning {{cannot prove declared bounds for 'p4' are valid after initialization}} \
                                                   // expected-note {{declared bounds are}} \
                                                   // expected-note {{inferred bounds are}}

  // Unable to create base ranges.
  int* q;
  array_ptr<int> p5 : bounds(p2, q + 1) = p4; // expected-warning {{cannot prove declared bounds for 'p5' are valid after initialization}} \
                                              // expected-note {{declared bounds are}} \
                                              // expected-note {{inferred bounds are}}
}

