// This file tests the compiler diagnostic messages for free variables
// in the inferred or declared bounds that cause the bounds proof to fail.
// See checkedc-clang#903 for the definition of free variables.

// RUN: %clang_cc1 -verify %s

// RUN: %clang -cc1 -verify -Wextra -fcheckedc-extension %s

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
	array_ptr<int> p : count(i1) = a; // expected-error {{it is impossible to prove that the inferred bounds of 'p' imply the declared bounds of 'p' after initialization}}
									  // expected-note {{the declared upper bounds use the value of the variable 'i1', and there is no relational information between 'i1' and any of the variables used by the inferred upper bounds}}
									  // expected-note {{declared bounds are 'bounds(p, p + i1)'}}
									  // expected-note {{inferred bounds are 'bounds(a, a + 5)'}}
	// i1, i2 free
	array_ptr<int> q : count(i2) = p; // expected-error {{it is impossible to prove that the inferred bounds of 'q' imply the declared bounds of 'q' after initialization}}


	array_ptr<int> p0 : bounds(p, p + (i1 + i2)) = 0;

	// p1 and p are free.
	// 'i1' and 'i2' are free but not reported!
	// The compiler checks free variables only if the bases are equal.
	array_ptr<int> p1 : count(len1) = p0; // expected-error {{it is impossible to prove that the inferred bounds of 'p1' imply the declared bounds of 'p1' after initialization}}


	// 'a' p1 free.
	p1 = &a[0];

	// 'len1' and 'len1' from 'p2' are free.
	array_ptr<int> p2 : bounds(a + len1, a + len2) = 0;
	array_ptr<int> p3 : bounds(a + len2, a + len1) = p2;

	// Compound bases. 'len1' is free.
	array_ptr<int> p4 : bounds((p4 + len1) + 0, (p4 + len1) + 1) = 0;
	array_ptr<int> p5 : bounds((p5 + len2) + 0, (p5 + len2) + 1) = p4;

	// Multiple equivalent variables. Emit note once for 'len1' and 'len2'.
	array_ptr<int> p6 : count(len1 + len1 + len1 + len2) = 0;
	array_ptr<int> p7 : count(i1) = p6;

	// a and b equal to some constant. v is free.
	unsigned v;
	array_ptr<int> p8 : count(i1) = 0;
	array_ptr<int> p9 : count(i2 + i1) = 0;
	array_ptr<int> p10 : count(v) = 0;
	p10 = p8;
	p10 = p9;
}

void f13() {
  int p[10];
  array_ptr<int> x : count(10) = 0;
  array_ptr<int> cache1 : count(5) = 0;

  x = _Dynamic_bounds_cast<array_ptr<int>>(p, bounds(cache1 - 2, cache1 + 3));
}

struct S1 {
	_Array_ptr<int> p : count(len2);
	int len2;
};

void f1(struct S1 *s) {
	array_ptr<int> p : count(5) = 0;
	s->p = p;

	int a[] = { 1, 2 };
	array_ptr<int> q : bounds(q, &a[0]) = p;
	s->p = &a[1];
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
	a3.p = p;
}

void f4(void) {
	array_ptr<int> p : count(2) = 0;
	array_ptr<int> q : count(1) = p + 1; // expected-error {{it is not possible to prove that the inferred bounds of 'q' imply the declared bounds of 'q' after initialization}}
}

void g(void) {
	int a, b, c, d, e;
	a = b = 0;
	c = d = 1;

	// Warning but no free variables because 'a == b'.
	array_ptr<int> p1 : count(a) = 0;
	array_ptr<int> p2 : count(b + 1) = p1;

	// No free variables. a and c equal to some constants.
	array_ptr<int> p3 : count(3) = 0;
	array_ptr<int> p4 : bounds(p4 + c, p4 + a) = p3;

	// Unable to create base ranges.
	int* q;
	array_ptr<int> p5 : bounds(p2, q + 1) = p4;
}

