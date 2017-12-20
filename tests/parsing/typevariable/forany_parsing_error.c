// Tests to make sure _For_any errors are produced correctly.
//
// More specifically, we are testing for following errors.
// 1) _For_any() must have a list of type variables within the parentheses.
// 2) Make sure type declaration syntax error is caught.
// 3) _For_any scope should be confined within function declaration.
//
// RUN: %clang_cc1 -verify %s

_For_any(R) _Ptr<R> foo(void);
// Testing scope created by for any specifier is exited successfully.
R *thisShouldProduceError; //expected-error{{unknown type name 'R'}}
_For_any() void foo2(void); // expected-error{{expected type variable identifier}}
_For_any(R, ) _Ptr<R> foo3(void); // expected-error{{expected type variable identifier}}
_For_any(R T) _Ptr<R> foo4(void); // expected-error{{expected , or )}}
