// Tests to make sure generic function call is parsed correctly.
//
// More specifically, we are testing for below cases to parse and not error :
// 1) When calling a generic function, type specification in <> is parsed. We
//    do not yet care whether the number of type variables in function
//    declaration is consistent with number of types specified in function call
// 2) An expression that may be ambiguous to generic function call is a
//    comparison expression. ex) foo < bar; Make sure this isn't going to break
//
// RUN: %clang_cc1 -verify %s
// expected-no-diagnostics

_For_any(T) _Ptr<T> foo(_Ptr<T> a, _Ptr<T> b) {
  return a;
}

void bar() {
  return;
}

void callPolymorphicTypes() {
  int num = 0;
  _Ptr<int> x = &num;
  // This line tests that the compiler is parsing generic function call.
  foo<int>(x, x);
  // This line tests that the compiler is parsing non generic function call.
  bar();
  int a, b;
  // This line tests that the compiler parses comparison not generic function
  if (a < b) {};
}