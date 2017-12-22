// Tests to make sure _For_any specifier is parsed correctly.
//
// More specifically, we are testing for correctness of _For_any specifier.
// 1) _For_any specifier is correctly parsed along with new type polymorphism.
// 2) _For_any specifier correctly enters a new scope and function declaration
//    or definition is registered to a correct scope.
// For this test file, we expect that there are no errors.
//
// RUN: %clang_cc1 -verify %s
// expected-no-diagnostics

// Testing for function declaration with function body, with parameters
_For_any(T, S) _Ptr<T> TestDefinitionWithParameter(_Ptr<T> at, _Ptr<T> bt, _Ptr<S> cs) {
  _Ptr<T> newT = at;
  return newT;
}

// Testing for function declaration without function body, without parameters.
_For_any(R) _Ptr<R> TestDeclarationWithNoParameter(void);
// Testing for function declaration without function body, with parameters
_For_any(Q) _Ptr<Q> TestDeclarationWithParameter(_Ptr<Q> aq, _Ptr<Q> bq, _Ptr<Q> cq);

int callPolymorphicTypes() {
  int num = 0;
  _Ptr<int> x = &num;
  // Testing to make sure function declaration is registered in decl scope
  // outside of forany scope.
  TestDefinitionWithParameter<int, int>(x, x, x);
  TestDeclarationWithNoParameter<int>();
  TestDeclarationWithParameter<int>(x, x, x);
  return 0;
}