// Test interaction of generic structs with interface types (itypes)
//
// RUN: %clang_cc1 -verify -verify-ignore-unexpected=note %s

#include <stdchecked.h>

//
// Test that substitution applies to interface types of struct fields.
//
void TestItypeInGeneric() {
  struct Foo _For_any(T) {
    T *x : itype(_Ptr<T>);
  };
  
  struct Foo<int> intFoo;
  int *x = intFoo.x;

  struct Foo<char> charFoo;
  char *c = charFoo.x;

  checked {
    int *x = intFoo.x; // expected-error {{local variable in a checked scope must have a checked type}}
    _Ptr<int> x2 = intFoo.x;

    char *c = charFoo.x; // expected-error {{local variable in a checked scope must have a checked type}} 
    _Ptr<char> c2 = charFoo.x;
  }
}

//
// Test that we can parse an _Itype_for_any clause in a definition.
//
void TestItypeForAnyInDef() {
  struct Foo _Itype_for_any(T, U) {
  };
}

//
// Test generic bounds safe interface in checked and unchecked scopes.
//
void TestItypeForAnyScopes() {
  struct Foo _Itype_for_any(T) {
    void *x : itype(_Ptr<T>);	
  };

  // In an unchecked scope, if no type args are specified, we use void args.
  struct Foo foo1;
  void *x1 = foo1.x; // ok: uses declared type (void*) for field 
  _Ptr<void> y1 = foo1.x; // ok: uses bounds-safe interface (_Ptr<void>)

  struct Foo<char> foo2;
  char *x2 = foo2.x; // ok: conversion from _Ptr<char> to char*
  _Ptr<void> y2 = foo2.x; // ok: conversion from void* to _Ptr<void>

  _Checked {
    struct Foo foo3; // expected-error {{expected a type argument list for a struct with a bounds-safe interface in a checked scope}}

    struct Foo<char> foo4;
    _Ptr<void> x4 = foo4.x; // ok: widening conversion
    _Ptr<int> y4 = foo4.x; // expected-error {{initializing '_Ptr<int>' with an expression of incompatible type '_Ptr<char>'}}
    _Ptr<char> z4 = foo4.x;
  }
}

//
// Test new compatibilty rules in bounds interfaces.
//
void TestListIType() {
  struct List _Itype_for_any(T) {
    void *head : itype(_Ptr<T>);
    struct List *next : itype(_Ptr<struct List<T> >);
  };
}

//
// Test that we can detect expanding cycles that go through
// itypes.
//
void TestExpandingCycles() {
  struct FooGood _Itype_for_any(T) {
    struct FooGood *next : itype(_Ptr<struct FooGood<T> >); // ok: cycle is non-expanding
  };

  // Expanding cycle with one node.
  struct FooBad _Itype_for_any(T) { // expected-error {{expanding cycle in struct definition}}
    struct FooBad *next : itype(_Ptr<struct FooBad<struct FooBad<T> > >); // itype causes expanding cycle
  };

  // Expanding cycle with three nodes.
  // The cycle passes through both for_any and itype_forany nodes.
  struct A _For_any(T);
  struct B _Itype_for_any(T);
  struct C _Itype_for_any(T);

  struct A _For_any(T) {
    struct B<T> *b;
  };

  struct B _Itype_for_any(T) {
    struct C *c: itype(_Ptr<struct C<struct B<T> > >);
  };

  struct C _Itype_for_any(T) { // expected-error {{expanding cycle in struct definition}}
    struct A<T> *a;
  };
}
