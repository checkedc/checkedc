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
  static T t; // expected-error {{illegal use of free type variable 'T' in declaration of static variable 't'}} \
              // expected-note@15 {{free type variable 'T' declared here}}

  static _Ptr<U> p; // expected-error {{illegal use of free type variable 'U' in declaration of static variable 'p'}} \
                    // expected-note@15 {{free type variable 'U' declared here}}

  static _Array_ptr<V> a1; // expected-error {{illegal use of free type variable 'V' in declaration of static variable 'a1'}} \
                           // expected-note@15 {{free type variable 'V' declared here}}

  static _Array_ptr<_Array_ptr<T>> a2; // expected-error {{illegal use of free type variable 'T' in declaration of static variable 'a2'}} \
                                       // expected-note@15 {{free type variable 'T' declared here}}

  static struct S<T, U, T> s1; // expected-error {{illegal use of free type variable 'T' in declaration of static variable 's1'}} \
                               // expected-note@15 {{free type variable 'T' declared here}} \
                               // expected-error {{illegal use of free type variable 'U' in declaration of static variable 's1'}} \
                               // expected-note@15 {{free type variable 'U' declared here}}

  static _Exists(T, _Ptr<U>) e1; // expected-error {{illegal use of free type variable 'U' in declaration of static variable 'e1'}} \
                                 // expected-note@15 {{free type variable 'U' declared here}}

  static _Exists(T, struct S<T, U, V>) e2; // expected-error {{illegal use of free type variable 'U' in declaration of static variable 'e2'}} \
                                           // expected-note@15 {{free type variable 'U' declared here}} \
                                           // expected-error {{illegal use of free type variable 'V' in declaration of static variable 'e2'}} \
                                           // expected-note@15 {{free type variable 'V' declared here}}

  static _Exists(A, struct S<T, U, V>) e3; // expected-error {{illegal use of free type variable 'T' in declaration of static variable 'e3'}} \
                                           // expected-note@15 {{free type variable 'T' declared here}} \
                                           // expected-error {{illegal use of free type variable 'U' in declaration of static variable 'e3'}} \
                                           // expected-note@15 {{free type variable 'U' declared here}} \
                                           // expected-error {{illegal use of free type variable 'V' in declaration of static variable 'e3'}} \
                                           // expected-note@15 {{free type variable 'V' declared here}}

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
