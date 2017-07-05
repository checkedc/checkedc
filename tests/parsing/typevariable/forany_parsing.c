// Tests to make sure _For_any specifier is parsed correctly.
//
// More specifically, we are testing for correctness of _For_any specifier.
// 1) _For_any specifier is correctly parsed along with new type polymorphism.
// 2) _For_any specifier correctly enters a new scope and function declaration
//    or definition is registered to a correct scope.
// For this test file, we expect that there are no errors.
//
// RUN: %clang_cc1 -fcheckedc-extension -S -emit-llvm -verify %s
// expected-no-diagnostics

// Testing for function declaration with function body, without parameters.
_For_any(T) T TestDefinitionWithNoParameter(void) {
  // Testing the scope created by forany specifier contains function body scope
  T returnVal;
  return returnVal;
}

// Testing for function declaration with function body, with parameters
_For_any(T, S) T TestDefinitionWithParameter(T at, T bt, S cs) {
  S newT = at;
  return newT;
}

// Testing for function declaration without function body, without parameters.
_For_any(R) R TestDeclarationWithNoParameter(void);
// Testing for function declaration without function body, with parameters
_For_any(Q) Q TestDeclarationWithParameter(Q aq, Q bq, Q cq);

int callPolymorphicTypes() {
  void *x, *y, *z;
  // Testing to make sure function declaration is registered in decl scope
  // outside of forany scope.
  TestDefinitionWithNoParameter<void *>();
  TestDefinitionWithParameter<void *, void *>(x, y, z);
  TestDeclarationWithNoParameter<void *>();
  TestDeclarationWithParameter<void *>(x, y, z);
  return 0;
}