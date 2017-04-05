// Feature tests of parsing new Checked C dynamic and assume bounds
// cast. The following lines are for the LLVM test harness:
//
// RUN: %clang_cc1 -verify -fcheckedc-extension %s

#include "../../include/stdchecked.h"

extern void f1(){
  array_ptr<int> a : count(1) =0;
  ptr<int> c = 0;
  int *p ;
  array_ptr<int> checkedc_p : bounds(checkedc_p, checkedc_p+1)= 0;

  c = _Assume_bounds_cast<int>(p);
  a = _Assume_bounds_cast<int>(p,4);
  checkedc_p = _Assume_bounds_cast<int>(p,p,p+1);
}

extern void f2(){
  char *p;
  array_ptr<int>a : count(1) = 0;
  a = _Assume_bounds_cast<int> (p);
  a = _Assume_bounds_cast<int> (p,1);
  a = _Assume_bounds_cast<int> (p, p, p+1);
  a = _Assume_bounds_cast<int) (p, p, p+1); // expected-error{{expected '>'}} expected-note {{to match this '<'}}
}

extern void f3(){
  char *p;
  ptr<int> c = 0;
  array_ptr<int> a : count(2) = 0;
  array_ptr<int> d = _Assume_bounds_cast<int>(p,4);
  
  a = _Assume_bounds_cast<int>(p, p+2); // expected-error{{invalid argument type 'char *' to count expression}} expected-error{{expression has no bounds,}}
  c = _Assume_bounds_cast<int>(p);
  c = _Assume_bounds_cast<int>(p,1);
}

extern void f4(){
  array_ptr<int> a : count(2) = 0;
  array_ptr<char> b : count(2) = 0;
  a = _Assume_bounds_cast<int>(b,2);
  a = _Assume_bounds_cast<int>(b); 
}
