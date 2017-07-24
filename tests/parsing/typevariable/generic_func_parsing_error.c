// Tests to make sure generic function call errors are parsed correctly.
//
// Make sure that syntatic error in calling generic functions are handled
//
// RUN: %clang_cc1 -fcheckedc-extension -verify %s

_For_any(T) _Ptr<T> Foo(_Ptr<T> a, _Ptr<T> b) {
  return a;
}

void CallGenericFunction() {
  int num = 0;
  _Ptr<int> x = &num;
  Foo<int int>(x, x); //expected-error{{cannot combine with previous 'int' declaration specifier}}
  Foo<int, >(x, x); //expected-error{{expected a type}}
  Foo<, , >(x, x); //expected-error{{expected a type}}
  Foo(x, x); //expected-error{{expected a list of type arguments for a generic function}}
}
