// Feature tests of typechecking no prototype functions.
//
// The following lines are for the LLVM test harness:
//
// RUN: %clang_cc1 -verify -verify-ignore-unexpected=note %s

#include <stdchecked.h>

struct S1 {
  ptr<int> m1;
};

struct S2 {
  array_ptr<int> m1;
};

struct S3 {
  ptr<int> (*m1)(void);
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
  ptr<int>(*m1)(void);
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

extern int* _Single f1();  // expected-error {{function with no prototype cannot have a return type that is a checked type}}
extern array_ptr<int> f2();// expected-error {{function with no prototype cannot have a return type that is a checked type}}
extern struct S1 f3();     // expected-error {{function with no prototype cannot have a return type that is a structure with a member with a checked type}}
extern struct S2 f4();     // expected-error {{function with no prototype cannot have a return type that is a structure with a member with a checked type}}
extern struct S3 f5();     // expected-error {{function with no prototype cannot have a return type that is a structure with a member with a checked type}}
extern struct S4 f6();     // expected-error {{function with no prototype cannot have a return type that is a structure with a member with a checked type}}
extern struct S5 f7();     // expected-error {{function with no prototype cannot have a return type that is a structure with a member with a checked type}}
extern struct S6 f8();     // expected-error {{function with no prototype cannot have a return type that is a structure with a member with a checked type}}
extern struct S7 f9();     // expected-error {{function with no prototype cannot have a return type that is a structure with a member with a checked type}}
extern struct S8 f10();    // expected-error {{function with no prototype cannot have a return type that is a structure with a member with a checked type}}
extern union U1 f12();     // expected-error {{function with no prototype cannot have a return type that is a union with a member with a checked type}}
extern union U2 f13();     // expected-error {{function with no prototype cannot have a return type that is a union with a member with a checked type}}
extern union U3 f14();     // expected-error {{function with no prototype cannot have a return type that is a union with a member with a checked type}}
extern union U4 f15();     // expected-error {{function with no prototype cannot have a return type that is a union with a member with a checked type}}
extern union U5 f16();     // expected-error {{function with no prototype cannot have a return type that is a union with a member with a checked type}}
extern union U6 f17();     // expected-error {{function with no prototype cannot have a return type that is a union with a member with a checked type}}
extern union U7 f18();     // expected-error {{function with no prototype cannot have a return type that is a union with a member with a checked type}}

// No prototype functions that return function pointers

// Returns an unchecked pointer to a function with a checked argument (ptr<int>)
extern int (*f19())(ptr<int>);  // expected-error {{function with no prototype cannot have a return type that is a checked type}}

// Returns an unchecked pointer to a no-prototype function returning a checked
// value (ptr<int>)
extern int* _Single (*f20(void))(); // expected-error {{function with no prototype cannot have a return type that is a checked type}}
// Returns a checked pointer to a function with a checked argument (ptr<int>)
extern ptr<int (ptr<int>)> f21(); // expected-error {{function with no prototype cannot have a return type that is a checked type}}

// Returns a checked pointer to a no-prototype function returning a checked
// value (ptr<int>)
// This unexpectedly produces a syntax error
// extern ptr<ptr<int> ()> f21();

// No prototype functions can return unchecked pointers to checked values
// or checked values embedded in objects.  They cannot return unchecked
// pointers to function types that take or return checked values.

extern ptr<int> *f30();
extern int* _Array *f31();
extern struct S1 *f32();
extern struct S2 *f33();
extern struct S3 *f34();
extern struct S4 *f36();
extern struct S5 *f37();
extern struct S6 *f38();
extern struct S7 *f39();
extern struct S8 *f40();
extern struct S9 *f41();

//
// No prototype functions cannot return checked types.  They can
// declare bounds-safe interfaces.
//

extern array_ptr<int> f50() : count(5); // expected-error {{function with no prototype cannot have a return type that is a checked type}}
extern int f51() : byte_count(10);
extern int *f52() : byte_count(10);
extern int *f53() _Itype(ptr<int>);

// No prototype functions cannot be redeclared to take arguments that are
// checked values or objects with checked values embedded within them.

extern void f60(); 
extern void f60(ptr<int>); // expected-error {{cannot redeclare a function with no prototype to have an argument type that is a checked type}}

extern void f61();
extern void f61(int* _Array); // expected-error {{cannot redeclare a function with no prototype to have an argument type that is a checked type}}

extern void f62();
extern void f62(struct S1); // expected-error {{cannot redeclare a function with no prototype to have an argument type that is a structure with a member with a checked type}}

extern void f63();
extern void f63(struct S2); // expected-error {{cannot redeclare a function with no prototype to have an argument type that is a structure with a member with a checked type}}

extern void f64();
extern void f64(struct S3); // expected-error {{cannot redeclare a function with no prototype to have an argument type that is a structure with a member with a checked type}}

extern void f65();
extern void f65(struct S4); // expected-error {{cannot redeclare a function with no prototype to have an argument type that is a structure with a member with a checked type}}

extern void f66();
extern void f66(struct S5); // expected-error {{cannot redeclare a function with no prototype to have an argument type that is a structure with a member with a checked type}}

extern void f67();
extern void f67(struct S6); // expected-error {{cannot redeclare a function with no prototype to have an argument type that is a structure with a member with a checked type}}

extern void f68();
extern void f68(struct S7); // expected-error {{cannot redeclare a function with no prototype to have an argument type that is a structure with a member with a checked type}}

extern void f69();
extern void f69(struct S8); // expected-error {{cannot redeclare a function with no prototype to have an argument type that is a structure with a member with a checked type}}

extern void f71();
extern void f71(union U1); // expected-error {{cannot redeclare a function with no prototype to have an argument type that is a union with a member with a checked type}}

extern void f72();
extern void f72(union U2); // expected-error {{cannot redeclare a function with no prototype to have an argument type that is a union with a member with a checked type}}

extern void f73();
extern void f73(union U3); // expected-error {{cannot redeclare a function with no prototype to have an argument type that is a union with a member with a checked type}}

extern void f74();
extern void f74(union U4); // expected-error {{cannot redeclare a function with no prototype to have an argument type that is a union with a member with a checked type}}

extern void f75();
extern void f75(union U5); // expected-error {{cannot redeclare a function with no prototype to have an argument type that is a union with a member with a checked type}}

extern void f76();
extern void f76(union U6); // expected-error {{cannot redeclare a function with no prototype to have an argument type that is a union with a member with a checked type}}

extern void f77();
extern void f77(union U7); // expected-error {{cannot redeclare a function with no prototype to have an argument type that is a union with a member with a checked type}}


extern void f80();
extern void f80(int checked[]); // expected-error {{cannot redeclare a function with no prototype to have an argument type that is a checked type}}

// Function type arguments.

extern void f81();
extern void f81(int* _Single f(void)); // expected-error {{cannot redeclare a function with no prototype to have an argument type that is a checked type}}

extern void f82();
extern void f82(int f(ptr<int>)); // expected-error {{cannot redeclare a function with no prototype to have an argument type that is a checked type}}

extern void f83(int f());
extern void f83(int f(ptr<int>));  // expected-error {{conflicting types for 'f83'}}

extern void f84(int f(ptr<int>));  
extern void f84(int f());   // expected-error {{conflicting types for 'f84'}}

// No prototype functions can be redeclared with bounds-safe interfaces on integer and
// unchecked pointer arguments.
extern void f85();
extern void f85(char *p _Bounds(p, p + len), int len);

//
// Redeclaring a function with a checked argument as a no prototype function is not allowed.
//

extern void f90(ptr<int>);
extern void f90(); // expected-error {{cannot redeclare a function that has a checked argument or argument bounds to have no prototype}}

// Redeclaring a function with a bounds-safe interface on an argument as a no protoype
// functino is allowed.  The function types are considered compatible.
extern void f91(char *p : bounds(p, p + len), int len);
extern void f91();

//
// Spot-check other attempts at creating no prototype functions that return
// a checked value.
//

struct S20 {
  int* _Single (*f)(); // expected-error {{function with no prototype cannot have a return type that is a checked type}}
};

typedef int* _Single functype1(); // expected-error {{function with no prototype cannot have a return type that is a checked type}}

// Check the obscure case of a function being declared as a no-prototype, then
// being declared to have a prototype with an incomplete type, and then
// the incomplete type being completed.

struct S21;
extern void f100();
extern void f100(struct S21);
struct S21 {
  int* _Single m;
};

extern void f100(struct S21 x) { // expected-error {{cannot redeclare a function with no prototype to have an argument type that is a structure with a member with a checked type}}
}

typedef int functype2(ptr<int>);
struct S22 {
  functype2 *m;
};

extern void f101();
extern void f101(struct S22); //expected-error {{cannot redeclare a function with no prototype to have an argument type that is a structure with a member with a checked type}}

extern void f102();
extern void f102(functype2 arg); //expected-error {{cannot redeclare a function with no prototype to have an argument type that is a checked type}}
