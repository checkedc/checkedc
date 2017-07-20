// Tests to make sure generic function call errors are parsed correctly.
//
// Make sure that syntatic error in calling generic functions are handled
//
// RUN: %clang_cc1 -fcheckedc-extension -verify %s

_For_any(T) int oneTypeVariable(_Ptr<T> a, _Ptr<T> b) { //expected-note{{Type variable(s) declared here}}
  return 0;
}

_For_any(T, S, R, V) int manyTypeVariables(_Ptr<T> t, _Ptr<S> s, _Ptr<R> r, _Ptr<V> v) { //expected-note{{Type variable(s) declared here}}
  return 0;
}

void CallGenericFunction() {
  int a = 0, b = 0, c = 0, d = 0;
  _Ptr<int> ap = &a;
  _Ptr<int> bp = &b;
  _Ptr<int> cp = &c;
  _Ptr<int> dp = &d;

  oneTypeVariable<int, int>(ap, bp); //expected-error{{Mismatch between the number of types listed and number of type variables}}
  manyTypeVariables<int, int>(ap, bp, cp, dp); //expected-error{{Mismatch between the number of types listed and number of type variables}}

}
