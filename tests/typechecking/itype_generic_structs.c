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
