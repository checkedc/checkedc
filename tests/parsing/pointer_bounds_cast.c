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
  c = _Dynamic_bounds_cast<int>(p);  
  a = _Assume_bounds_cast<int>(p,4);
  c = _Dynamic_bounds_cast<int>(p,4);    
  checkedc_p = _Assume_bounds_cast<int>(p,p,p+1);
  checkedc_p = _Dynamic_bounds_cast<int>(p,p,p+1);  
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

extern void f5(){
  array_ptr<int> a : count(2) = 0;
  array_ptr<char> b : count(2) = 0;
  a = _Asume_bounds_cast<int>(b,2); // expected-error{{use of undeclared identifier}} expected-error {{expected expression}}
  a = _Dssume_bounds_cast<int>(b); // expected-error{{use of undeclared identifier}} expected-error {{expected expression}}
}

struct S1 {
  struct {
    array_ptr<int> lower;
    array_ptr<int> upper;
  } pair;
  array_ptr<int> arr1 : bounds(pair.lower, pair.upper);
  struct {
    array_ptr<int> arr2 : bounds(pair.lower, pair.upper);
  } nested;
};

extern void f6(){
  array_ptr<int> a : count(2) = 0;
  char *p;
  int *b;
  a = _Assume_bounds_cast<struct S1>(p,2); //expected-error {{assigning to '_Array_ptr<int>' from incompatible type '_Array_ptr<struct S1>'}}
  p = _Assume_bounds_cast<char>(a); //expected-error {{assigning to 'char *' from incompatible type '_Ptr<char>'}}
}

extern void f7(){
  int *p;
  ptr<int> q = 0;
  array_ptr<int> r;
  array_ptr<int> s : bounds(r, r+5) = 0;

  p = q;  // expected-error {{incompatible type}}
  p = r;  // expected-error {{incompatible type}}
  p = s;  // expected-error {{incompatible type}}

  // explicit casting vs dynamic_bounds_cast<T>
  p = (int *)q;
  p = _Assume_bounds_cast<int>(q); // expected-error{{incompatible type}}
  p = (int *)r;
  p = _Assume_bounds_cast<int>(r); // expected-error {{incompatible type}}
  p = (int *)s;
  p = _Assume_bounds_cast<int>(s);// expected-error {{incompatible type}}

  q = p;  // expected-error 2 {{expression has no bounds}}
  q = r;  // expected-error 2 {{expression has no bounds}}
  q = s;

  // dynamic_bounds_cast<T>(e) = {ptr<T>, (T *)} -> checked type with bounds
  // unchecked -> checked : ok
  // checked -> checked : ok
  q = (ptr<int>)(p);  // expected-error 2 {{expression has no bounds}}
  q = _Assume_bounds_cast<int>(p);
  q = (ptr<int>)(r);  // expected-error 2 {{expression has no bounds}}
  q = _Assume_bounds_cast<int>(r);
  
  q = _Assume_bounds_cast<int>(r) + 3; //expected-error{{arithmetic on _Ptr type}}
  q = _Assume_bounds_cast<int>(r,1) + 3;  
  *(_Assume_bounds_cast<int>(r) + 2) = 4; //expected-error{{arithmetic on _Ptr type}}
  *(_Assume_bounds_cast<int>(r,1) + 2) = 4; 
  
  q = s;

  r = p;
  r = q;
  r = s;

  s = p;  // expected-error {{expression has no bounds}}
  s = _Assume_bounds_cast<int>(p,5);
  s = r; //expected-error{{expression has no bounds, right-hand side of assignment expected to have bounds because the left-hand side has bounds}}
  s = _Assume_bounds_cast<int>(r, 5);
  s = q;
}

extern ptr<int> f8(int arr checked[]) {
  return _Assume_bounds_cast<int>(arr);
}

checked int * f9(int *p, ptr<int> q, array_ptr<int> r, array_ptr<int> s : count(2)) unchecked { // expected-error {{return cannot have an unchecked pointer type}} \
                                                                                                    // expected-error {{parameter cannot have an unchecked pointer type}}
  int a = 5;
  checked {
    *q = 2;
    *(_Assume_bounds_cast<int>(r,1)) = 3;
  *s = 4;
  unchecked {
    int b checked[5][5];
    for (int i = 0; i < 5; i++) {
      for (int j = 0; j < 5; j++) {
        b[i][j] += *q + *(_Assume_bounds_cast<int>(r,1));
      }
    }
  }
  }
  return 0;
}

extern void f10(){

  int i[2];
  array_ptr<int> int_array_ptr_lb = i, int_array_ptr_ub = i + 1;
  ptr<int> int_ptr_lb = i, int_ptr_ub = i + 1;
  int *int_unchecked_ptr_lb = i, *int_unchecked_ptr_ub = i + 1;

  array_ptr<char> char_array_ptr_lb = (array_ptr<char>)i,
                  char_array_ptr_ub = (array_ptr<char>)i + 1;
  ptr<char> char_ptr_lb = (ptr<char>)i, char_ptr_ub = (ptr<char>)(i + 1);
  char *char_unchecked_ptr_lb = (char *)i,
       *char_unchecked_ptr_ub = (char *)i + 1;

  array_ptr<int> t20 : bounds(int_array_ptr_lb, char_array_ptr_ub) =  // expected-error {{pointer type mismatch}}
    _Assume_bounds_cast<int>(i, int_array_ptr_lb, char_array_ptr_ub); // expected-error {{pointer type mismatch}}
  
  array_ptr<int> t21 : bounds(int_ptr_lb, char_array_ptr_ub) =  // expected-error {{pointer type mismatch}}
    _Assume_bounds_cast<int>(i,int_ptr_lb, int_array_ptr_ub); 
}
