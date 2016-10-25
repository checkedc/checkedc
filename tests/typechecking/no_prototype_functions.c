// Feature tests of typechecking no prototype functions.
//
// The following lines are for the LLVM test harness:
//
// RUN: %clang_cc1 -verify -fcheckedc-extension %s

#include "../../include/stdchecked.h"

struct S1 {
  ptr<int> m1;
};

struct S2 {
  array_ptr<int> m1;
};

struct S3 {
  ptr<int> (*m1)();
};

struct S4 {
  int (*m1)(ptr<int>);
};

struct S5 {
  struct S1 m1;
  int m2;
};

struct S6 {
  int m1;
  ptr<int> m2;
};

struct S7 {
  int m1 checked[10];
};

struct S8 {
  int base;
  int m1 checked[];
};

union U1 {
  ptr<int> m1;
  int *m2;
};

union U2 {
  array_ptr<int> m1;
  array_ptr<char> m2;
};

union U3 {
  ptr<int>(*m1)();
  int *(*m2)();
};

union U4 {
  int(*m1)(ptr<int>);
  int(*m2)(int *);
};

union U5 {
  struct S1 m1;
  struct S1 m2;
};

union U6 {
  int *m1;
  ptr<int> m2;
};

union U7 {
  int m1 checked[10];
  int m2[10];
};

//
// No prototype functions cannot return checked values or checked values
// embedded in objects.
//

extern ptr<int> f1();      // expected-error {{function without prototype cannot return a checked value}}
extern array_ptr<int> f2();// expected-error {{function without prototype cannot return a checked value}}
extern struct S1 f3();     // expected-error {{function without prototype cannot return a checked value}}
extern struct S2 f4();     // expected-error {{function without prototype cannot return a checked value}}
extern struct S3 f5();     // expected-error {{function without prototype cannot return a checked value}}
extern struct S4 f6();     // expected-error {{function without prototype cannot return a checked value}}
extern struct S5 f7();     // expected-error {{function without prototype cannot return a checked value}}
extern struct S6 f8();     // expected-error {{function without prototype cannot return a checked value}}
extern struct S7 f9();     // expected-error {{function without prototype cannot return a checked value}}
extern struct S8 f10();     // expected-error {{function without prototype cannot return a checked value}}
extern union U1 f11();      // expected-error {{function without prototype cannot return a checked value}}
extern union U2 f12();     // expected-error {{function without prototype cannot return a checked value}}
extern union U3 f13();     // expected-error {{function without prototype cannot return a checked value}}
extern union U4 f14();     // expected-error {{function without prototype cannot return a checked value}}
extern union U5 f15();     // expected-error {{function without prototype cannot return a checked value}}
extern union U6 f16();     // expected-error {{function without prototype cannot return a checked value}}
extern union U7 f17();     // expected-error {{function without prototype cannot return a checked value}}

// No prototype functions that return function pointers

// Returns an unchecked pointer to a function with a checked argument (ptr<int>)
extern int (*f18())(ptr<int>);  // expected-error {{function without prototype cannot return a checked value}}

// Returns an unchecked pointer to a no-prototype function returning a checked
// value (ptr<int>)
extern ptr<int> (*f19())(); // expected-error {{function without prototype cannot return a checked value}}
// Returns a checked pointer to a function with a checked argument (ptr<int>)
extern ptr<int (ptr<int>)> f20(); // expected-error {{function without prototype cannot return a checked value}}

// Returns a checked pointer to a no-prototype function returning a checked
// value (ptr<int>)
// This unexpectedly produces a syntax error
// extern ptr<ptr<int> ()> f21();

// No prototype functions can return unchecked pointers to checked values
// or checked values embedded in objects.  They cannot return unchecked
// pointers to function types that take or return checked values.

extern ptr<int> *f30();
extern array_ptr<int> *f31();
extern struct S1 *f32();
extern struct S2 *f33();
extern struct S3 *f34();
extern struct S4 *f36();
extern struct S5 *f37();
extern struct S6 *f38();

//
// No prototype functions cannot have return bounds declarations.  They can
// declare bounds-safe interfaces.
//

extern array_ptr<int> f50() : count(5); // expected-error {{cannot return a checked value}} expected-error {{cannot have a return bounds}}
extern int f51() : byte_count(10);      // expected-error {{cannot have a return bounds}}
extern int *f52() : byte_count(10);
extern int *f53() : itype(ptr<int>);

// No prototype functions cannot be redeclared to take arguments that are
// checked values or objects with checked values embedded within them.

extern void f60(); 
extern void f60(ptr<int>); // expected-error {{redeclaring a no-prototype function with checked argument not allowed}}

extern void f61();
extern void f61(array_ptr<int>); // expected-error {{redeclaring a no-prototype function with checked argument not allowed}}

extern void f62();
extern void f62(struct S1); // expected-error {{redeclaring a no-prototype function with checked argument not allowed}} 

extern void f63();
extern void f63(struct S2); // expected-error {{redeclaring a no-prototype function with checked argument not allowed}}

extern void f64();
extern void f64(struct S3); // expected-error {{redeclaring a no-prototype function with checked argument not allowed}}

extern void f65();
extern void f65(struct S4); // expected-error {{redeclaring a no-prototype function with checked argument not allowed}}

extern void f66();
extern void f66(struct S5); // expected-error {{redeclaring a no-prototype function with checked argument not allowed}}

extern void f67();
extern void f67(struct S6); // expected-error {{redeclaring a no-prototype function with checked argument not allowed}}

extern void f68();
extern void f68(struct S7); // expected-error {{redeclaring a no-prototype function with checked argument not allowed}}

extern void f69();
extern void f69(struct S8); // expected-error {{redeclaring a no-prototype function with checked argument not allowed}}

extern void f70();
extern void f70(union U1); // expected-error {{redeclaring a no-prototype function with checked argument not allowed}}

extern void f71();
extern void f71(union U2); // expected-error {{redeclaring a no-prototype function with checked argument not allowed}}

extern void f72();
extern void f72(union U3); // expected-error {{redeclaring a no-prototype function with checked argument not allowed}}

extern void f73();
extern void f73(union U4); // expected-error {{redeclaring a no-prototype function with checked argument not allowed}}

extern void f74();
extern void f74(union U5); // expected-error {{redeclaring a no-prototype function with checked argument not allowed}}

extern void f75();
extern void f75(union U6); // expected-error {{redeclaring a no-prototype function with checked argument not allowed}}

extern void f76();
extern void f76(union U7); // expected-error {{redeclaring a no-prototype function with checked argument not allowed}}

extern void f80();
extern void f80(int checked[]); // expected-error {{redeclaring a no-prototype function with checked argument not allowed}}

// Function type arguments.
extern void f81();
extern void f81(ptr<int> f()); // expected-error {{redeclaring a no-prototype function with checked argument not allowed}}

extern void f82();
extern void f82(int f(ptr<int>)); // expected-error {{redeclaring a no-prototype function with checked argument not allowed}}

extern void f83(int f());
extern void f84(int f(ptr<int>));

