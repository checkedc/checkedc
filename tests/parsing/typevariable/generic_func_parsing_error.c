// Tests to make sure generic function call errors are parsed correctly.
//
// Make sure that syntatic error in calling generic functions are handled
//
// RUN: %clang_cc1 -fcheckedc-extension -verify %s

_For_any(T) _Ptr<T> Foo(_Ptr<T> a, _Ptr<T> b) {
  return a;
}

void CallGenericFunction() {
  void *x, *y;
  Foo<int int>(x, y); //expected-error{{expected , or >}}
  Foo<void, >(x, y); //expected-error{{expected a type}}
  Foo<, , >(x, y); //expected-error{{expected a type}}
  Foo(x, y); //expected-error{{expected a list of type arguments for a generic function}}
}
