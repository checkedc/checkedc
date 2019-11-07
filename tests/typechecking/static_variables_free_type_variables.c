// Test type checking of static variables.
// Static variable declarations should not use free type variables.
//
// RUN: %clang_cc1 -verify %s

struct S _For_any(X, Y, Z) { };

// Global static variable declarations can use bound type variables.
static _Exists(T, struct S<T, T, T>) gs;

//
// Test that declarations of static variables cannot use free type variables.
//

_For_any(T, U, V) void f1(_Ptr<T> pt, struct S<T, U, V> s) {
  static T t; // expected-error {{type 'T' for static variable 't' cannot use type variable 'T' that is bound by an enclosing scope}} \
              // expected-note@15 {{type variable 'T' declared here}}

  static _Ptr<U> p; // expected-error {{type '_Ptr<U>' for static variable 'p' cannot use type variable 'U' that is bound by an enclosing scope}} \
                    // expected-note@15 {{type variable 'U' declared here}}

  static _Array_ptr<V> a1; // expected-error {{type '_Array_ptr<V>' for static variable 'a1' cannot use type variable 'V' that is bound by an enclosing scope}} \
                           // expected-note@15 {{type variable 'V' declared here}}

  static _Array_ptr<_Array_ptr<T>> a2; // expected-error {{type '_Array_ptr<_Array_ptr<T>>' for static variable 'a2' cannot use type variable 'T' that is bound by an enclosing scope}} \
                                       // expected-note@15 {{type variable 'T' declared here}}

  static struct S<T, U, T> s1; // expected-error {{type 'struct S<T, U, T>' for static variable 's1' cannot use type variable 'T' that is bound by an enclosing scope}} \
                               // expected-note@15 {{type variable 'T' declared here}} \
                               // expected-error {{type 'struct S<T, U, T>' for static variable 's1' cannot use type variable 'U' that is bound by an enclosing scope}} \
                               // expected-note@15 {{type variable 'U' declared here}}

  static _Exists(T, _Ptr<U>) e1; // expected-error {{type 'Exists(T, _Ptr<U>)' for static variable 'e1' cannot use type variable 'U' that is bound by an enclosing scope}} \
                                 // expected-note@15 {{type variable 'U' declared here}}

  static _Exists(T, struct S<T, U, V>) e2; // expected-error {{type 'Exists(T, struct S<T, U, V>)' for static variable 'e2' cannot use type variable 'U' that is bound by an enclosing scope}} \
                                           // expected-note@15 {{type variable 'U' declared here}} \
                                           // expected-error {{type 'Exists(T, struct S<T, U, V>)' for static variable 'e2' cannot use type variable 'V' that is bound by an enclosing scope}} \
                                           // expected-note@15 {{type variable 'V' declared here}}

  static _Exists(A, struct S<T, U, V>) e3; // expected-error {{type 'Exists(A, struct S<T, U, V>)' for static variable 'e3' cannot use type variable 'T' that is bound by an enclosing scope}} \
                                           // expected-note@15 {{type variable 'T' declared here}} \
                                           // expected-error {{type 'Exists(A, struct S<T, U, V>)' for static variable 'e3' cannot use type variable 'U' that is bound by an enclosing scope}} \
                                           // expected-note@15 {{type variable 'U' declared here}} \
                                           // expected-error {{type 'Exists(A, struct S<T, U, V>)' for static variable 'e3' cannot use type variable 'V' that is bound by an enclosing scope}} \
                                           // expected-note@15 {{type variable 'V' declared here}}

  // Static variable declarations can use integral and pointer types and bound type variables.
  static int i;
  static struct S<char, _Ptr<int>, double> s2;
  static _Exists(T, _Ptr<T>) e4;
  static _Exists(A, struct S<A, _Ptr<A>, _Array_ptr<A>>) e5;

  // Non-static variable declarations can use free type variables.
  _Ptr<T> q = 0;
  _Array_ptr<_Array_ptr<U>> a3;
  struct S<T, U, V> s3;
  _Exists(U, _Ptr<T>) e6;
}
