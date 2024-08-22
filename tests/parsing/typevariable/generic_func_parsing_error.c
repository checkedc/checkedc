// Tests to make sure parsing errors for generic function calls are detected
// properly.
//
// RUN: %clang_cc1 -verify %s

_For_any(T) T* _Single Foo(T* _Single a, T* _Single b) {
  return a;
}

void CallGenericFunction() {
  int num = 0;
  _Ptr<int> x = &num;
  Foo<int int>(x, x); //expected-error{{cannot combine with previous 'int' declaration specifier}}
  Foo<int, >(x, x);   //expected-error{{expected a type}}
  Foo(x, x);          //expected-error{{expected a type argument list for a generic function call}}
}
