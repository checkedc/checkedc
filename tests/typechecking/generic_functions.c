// Test type checking of generic function calls.
//
// RUN: %clang_cc1 -verify -verify-ignore-unexpected=note %s

//
// Test mismatches between the number of type variables and type arguments.
//
_For_any(T) int oneTypeVariable(_Ptr<T> a, _Ptr<T> b) {
  return 0;
}

_For_any(T, S, R, V) int manyTypeVariables(_Ptr<T> t, _Ptr<S> s, _Ptr<R> r, _Ptr<V> v) {
  return 0;
}

void CallGenericFunction(void) {
  int a = 0, b = 0, c = 0, d = 0;
  _Ptr<int> ap = &a;
  _Ptr<int> bp = &b;
  _Ptr<int> cp = &c;
  _Ptr<int> dp = &d;

  oneTypeVariable<int, int>(ap, bp);  //expected-error {{mismatch between the number of types listed and number of type variables}}
  oneTypeVariable<>(ap, bp);          //expected-error {{mismatch between the number of types listed and number of type variables}}
  manyTypeVariables<int, int>(ap, bp, cp, dp); //expected-error {{mismatch between the number of types listed and number of type variables}}
  manyTypeVariables<>(ap, bp, cp, dp); //expected-error {{mismatch between the number of types listed and number of type variables}}
}

//
// Test that generic functions without prototype are not allowed.
//

// Declaration of a generic no-prototype function.
_For_any(T) T *func1();             // expected-error {{expected prototype for a generic function}}

// Declaration of old-style K&R function definition.
_For_any(T) void func2(x, y)        // expected-error {{expected prototype for a generic function}}
  T *x;
  T *y;
{
}

