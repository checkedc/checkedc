// Tests to make sure generic function call errors are parsed correctly.
//
// More specifically, we are testing for below cases of error :
// 1) Make sure that syntatic error in calling generic functions are handled
// 2) Using generic function call syntax on normal function will not be
//    allowed. 
// 3) a special case that I found. Description below.
//
// RUN: %clang_cc1 -fcheckedc-extension -verify %s

_For_any(T) T Foo(T a, T b) {
  T a;
  return a;
}

void Bar() {
  // There will be parsing error here, since you cannot create a variable of
  // type void. This results in the Expr representation of Foo to be NULL for
  // some reason. Since I use isa<DeclRefExpr>() in the code, assert kicks in
  // saying you cannot do isa<> on NULL pointer, crashing compiler. I fixed the
  // code to check whether the Expr* is NULL or not.
  void* x, y; //expected-error{{variable has incomplete type 'void'}}
  Foo<void*>(x, y);
  return;
}

void CallGenericFunction() {
  void *x, *y;
  // I was wondering if I should create a separate check and error message to
  // say "Bar is not generic function, so we cannot have this syntax." However
  // I think it is sufficient enough for the clang to generate error message
  // as if generic functions were never implemented, in the case that the code
  // tried to call generic function call on a non-generic function.
  Bar<unsigned int>(); //expected-error{{expected expression}}
  Foo<void* unsigned int>(x, y); //expected-error{{expected , or >}}
  Foo<void*, >(x, y); //expected-error{{expected a type}}
  Foo<, , >(x, y); //expected-error{{expected a type}}
  Foo(x, y); //expected-error{{expected a list of types expression for generic function application}}
}
