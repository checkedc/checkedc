#include <stdchecked.h>
// Test type checking of itype generic function calls.
//
// RUN: %clang_cc1 -verify -verify-ignore-unexpected=note %s

//
// Test mismatches between the number of type variables and type arguments.
//
_Itype_for_any(T) void* oneTypeVariable(void* a : itype(_Ptr<T>), void* b : itype(_Ptr<T>)) : itype(_Ptr<T>) {
  return a;
}

_Itype_for_any(T, Q) void* manyTypeVariables(void* a : itype(_Ptr<T>), void* b : itype(_Ptr<Q>)) : itype(_Ptr<Q>) {
  return b;
}


_Itype_for_any(T)
void* validItypeGenericFunction(int a, void* b : itype(_Ptr<T>), void* c : itype(_Ptr<T>)) : itype(_Ptr<T>);

_Itype_for_any(T)
void* validItypeGenericFunction(int a, void* b : itype(_Ptr<T>) , void* c : itype(_Ptr<T>) ) : itype(_Ptr<T>) {
  _Ptr<T> m = b;
  return m;
}



void CallItypeGenericFunctions(void) {
  int a = 0, b = 0, c = 0, d = 0;
  _Ptr<int> ap = &a;
  _Ptr<int> bp = &b;
  _Ptr<int> cp = &c;
  _Ptr<int> dp = &d;

  oneTypeVariable<int, int>(ap, bp);  //expected-error {{mismatch between the number of types listed and number of type variables}}
  oneTypeVariable<>(ap, bp);          //expected-error {{mismatch between the number of types listed and number of type variables}}
  manyTypeVariables<int, int>(ap, bp);
  manyTypeVariables<>(ap, bp); //expected-error {{mismatch between the number of types listed and number of type variables}}

  float y = 5.5, z = 0.0;
  _Ptr<float> p2 = &y;
  _Ptr<float> p = &z;
  checked {
    p = validItypeGenericFunction<float>(5, p2, p2);
    //Checked scope expects type arguments
    validItypeGenericFunction(5, 0, 0); //expected-error {{expected a type argument list for a bounds-safe interface call in a checked scope}}
  }
  unchecked{
    void * p3 = (void *) p2;
    //Unchecked scope. Type arguments are not mandated.
    float *p4 = validItypeGenericFunction(5, p3, p3);
    _Ptr<float> p5 = validItypeGenericFunction<float>(5,p2,p2);
  }
}

