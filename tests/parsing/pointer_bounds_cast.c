// Feature tests of parsing new Checked C dynamic and assume bounds
// cast. The following lines are for the LLVM test harness:
//
// RUN: %clang_cc1 -verify -fcheckedc-extension %s

#include "../../include/stdchecked.h"

extern void f0(){
  int a checked[10];
  void *e;
  array_ptr<void> g: count(10) = 0; //expected-error{{expected 'g' to have}}
  array_ptr<int> b: count(10) = (array_ptr<int>) a;
  array_ptr<int> c: count(10) = _Dynamic_bounds_cast<array_ptr<int>>(a,5);
  array_ptr<int> f: count(10) = _Dynamic_bounds_cast<array_ptr<int>>(e,4); //expected-error{{count bounds expression}} expected-error{{expression has no}}
  array_ptr<int> h: count(10) = _Dynamic_bounds_cast<array_ptr<int>>(g,4); //expected-error{{count bounds expression}}
}

extern void f1(){
  array_ptr<int> a : count(1) =0;
  int i;
  ptr<int> c = 0;
  int b[10];
  array_ptr<int> ch : bounds(a, i, i+1) =0 ; //expected-error {{expected expression with}}
  
  int *p ;
  array_ptr<int> checkedc_p : bounds(checkedc_p, checkedc_p+1)= 0;
  
  a = _Dynamic_bounds_cast<array_ptr<int>>(b,10); 
  c = _Assume_bounds_cast<ptr<int>>(p); //expected-error {{expression has no bounds}}

  c = _Dynamic_bounds_cast<int>(p); // expected-error{{assigning to '_Ptr<int>' from incompatible type 'int'}} expected-error{{invalid bounds cast}}
  c = _Dynamic_bounds_cast<ptr<int>>(p); //expected-error {{expression has no bounds}}

  a = _Assume_bounds_cast<array_ptr<int>>(p,4);//expected-error {{expression has no bounds}}

  c = _Dynamic_bounds_cast<ptr<int>>(p,4); // expected-error{{invalid bounds cast}} expected-error {{expression has no bounds}}


  checkedc_p = _Assume_bounds_cast<array_ptr<int>>(p,p,p+1); //expected-error {{expression has no bounds}}
  checkedc_p = _Dynamic_bounds_cast<array_ptr<int>>(p,p,p+1);//expected-error {{expression has no bounds}}
}

extern void f2(){
  char *p;
  array_ptr<int>a : count(1) = 0;
  int b checked[10];
  array_ptr<int> c: count(10) = (array_ptr<int>) a;
  array_ptr<int> d: count(10) = _Dynamic_bounds_cast<array_ptr<int>>(a,5);
  a = _Assume_bounds_cast<array_ptr<int>> (p); //expected-error{{invalid bounds cast}} expected-error{{expression has no bounds}}
  a = _Assume_bounds_cast<array_ptr<int>> (p,1); //expected-error {{expression has no bounds}}
  a = _Assume_bounds_cast<array_ptr<int>> (p, p, p+1);//expected-error {{expression has no bounds}}
  a = _Assume_bounds_cast<array_ptr<int>) (p, p, p+1); // expected-error{{expected '>'}} expected-note {{to match this '<'}}
}

extern void f3(){
  char *p;
  ptr<int> c = 0;
  array_ptr<int> a : count(2) = 0;
  array_ptr<int> d = _Assume_bounds_cast<array_ptr<int>>(p,4); //expected-error{{expression has no bounds}}
  
  a = _Assume_bounds_cast<array_ptr<int>>(p, p+2); // expected-error{{invalid argument type 'char *' to count expression}} expected-error{{expression has no bounds}}
  c = _Assume_bounds_cast<ptr<int>>(p); //expected-error {{expression has no bounds}}
  c = _Assume_bounds_cast<ptr<int>>(p,1);//expected-error{{invalid bounds cast}} expected-error{{expression has no bounds}}
  p = _Assume_bounds_cast<char*>(c);
}

extern void f4(){
  array_ptr<int> a : count(2) = 0;
  array_ptr<ptr<char>> b : count(2) = 0;
  
  b = _Assume_bounds_cast<array_ptr<ptr<char>>>(a,2);
  a = _Assume_bounds_cast<array_ptr<int>>(b); //expected-error{{invalid bounds cast}}
}

extern void f5(){
  array_ptr<int> a : count(2) = 0;
  array_ptr<char> b : count(2) = 0;
  
  a = _Asume_bounds_cast<array_ptr<int>>(b,2); // expected-error{{use of undeclared identifier}} expected-error {{expected expression}}
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
  struct S1 *p;
  int *b;
  a = _Assume_bounds_cast<array_ptr<int>>(p,2);  //expected-error {{expression has no bounds}}
  p = _Assume_bounds_cast<struct S1*>(a); 
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

  p = _Assume_bounds_cast<int*>(q);
  p = (int *)r;
  p = _Assume_bounds_cast<int *>(r); // expected-error {{expression has no bounds}} /* I'm not sure if this is error situation. r has no bounds and CastExprBounds is (r,count(1)) */
  p = (int *)s;
  p = _Assume_bounds_cast<int *>(s);
  q = p;  // expected-error 2 {{expression has no bounds}}
  q = r;  // expected-error 2 {{expression has no bounds}}
  q = s;

  q = _Assume_bounds_cast<ptr<int>>(p); //expected-error {{expression has no bounds}}
  q = _Assume_bounds_cast<ptr<int>>(r);//expected-error {{expression has no bounds}}
  
  q = _Assume_bounds_cast<ptr<int>>(r) + 3; //expected-error{{arithmetic on _Ptr type}}
  q = _Assume_bounds_cast<ptr<int>>(r,1) + 3;  //expected-error{{arithmetic on _Ptr type}} //expected-error{{invalid bounds cast}}
  *(_Assume_bounds_cast<ptr<int>>(r) + 2) = 4; //expected-error{{arithmetic on _Ptr type}}
  *(_Assume_bounds_cast<array_ptr<int>>(r,1) + 2) = 4;  //expected-error {{expression has no bounds}}
  
  s = p;  // expected-error {{expression has no bounds}}
  s = _Assume_bounds_cast<array_ptr<int>>(p,5); //expected-error {{expression has no bounds}}
  s = r; //expected-error{{expression has no bounds, right-hand side of assignment expected to have bounds because the left-hand side has bounds}}
  s = _Assume_bounds_cast<array_ptr<int>>(r, 5); //expected-error {{expression has no bounds}}
  s = q;
}

extern ptr<int> f8(int arr checked[]) {
  return _Assume_bounds_cast<ptr<int>>(arr); //expected-error{{expression has no bounds}}
}

checked int * f9(int *p, ptr<int> q, array_ptr<int> r, array_ptr<int> s : count(2)) unchecked { // expected-error {{return cannot have an unchecked pointer type}} \
                                                                                                    // expected-error {{parameter cannot have an unchecked pointer type}}

  ptr<int> t8 = _Assume_bounds_cast<ptr<int>>(r,1);  //expected-error{{invalid bounds cast}} expected-error{{expression has no bounds}}
  int a = 5;
  checked { 
    *q = 2;
    *(_Assume_bounds_cast<ptr<int>>(r,1)) = 3;  //expected-error{{invalid bounds cast}} expected-error{{expression has no bounds}}
  *s = 4;
  unchecked {
    int b checked[5][5];
    for (int i = 0; i < 5; i++) {
      for (int j = 0; j < 5; j++) {
        b[i][j] += *q + *(_Assume_bounds_cast<array_ptr<int>>(r,1)); //expected-error {{expression has no bounds}}
      }
    }
  }
  }
  return 0;
}


extern void f10(ptr<int> p, int y) {
    *p = y;
}

extern array_ptr<int> h3(void) : count(3) {
   return 0;
}

extern array_ptr<int> h4(void) : count(3) {
  array_ptr<int> p : bounds(p, p + 3) = 0;
  return p;
}

extern void f11(){
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

    _Assume_bounds_cast<array_ptr<int>>(i, int_array_ptr_lb, char_array_ptr_ub); // expected-error {{pointer type mismatch}}
  
  array_ptr<int> t21 : bounds(int_ptr_lb, char_array_ptr_ub) =  // expected-error {{pointer type mismatch}}
    _Assume_bounds_cast<array_ptr<int>>(i,int_ptr_lb, int_array_ptr_ub);
}


extern void f12(){
  array_ptr<int> r : count(3) = 0;
  ptr<int> q = 0;
  r = _Assume_bounds_cast<array_ptr<int>>(h4(),3);
  q = _Assume_bounds_cast<ptr<int>>(h4());
}

extern int h5(void){
  int k=0;
  return k;
}

extern void f13(){
  int *p;
  ptr<int> q = 0;
  ptr<int> r = 0;
  ptr<int*> s = 0;
  ptr<ptr<int>> t = 0;
  r = _Assume_bounds_cast<ptr<int>>(q);
  p = _Assume_bounds_cast<int *>(q); 

  r = _Dynamic_bounds_cast<ptr<int>>(q);
  p = _Dynamic_bounds_cast<int *>(q);
  
  s = _Assume_bounds_cast<ptr<int*>>(q);
  t = _Assume_bounds_cast<ptr<ptr<int>>>(q);

  s = ynamic_bounds_cast<ptr<int*>>(q); //expected-error {{use of undeclared identifier}} expected-error {{expected expression}}
  t = _Dynamic_bounds_cast<ptr<ptr<int>>>(q);

  r = _Assume_bounds_cast<ptr<int>>(q);
  p = _Assume_bounds_cast<int *>(q); 
  p = _Dssume_bounds_cast<int *>(h5);  //expected-error{{expected expression}} expected-error {{use of undeclared identifier}}
  p = _Dynamic_bounds_cast<int *>(h5); //expected-error{{expression has no bounds}} 
}

extern void f14(){
  array_ptr<int> r : count(3) = 0;
  ptr<int> q = 0;
  r = _Assume_bounds_cast<array_ptr<int>,rel_align(int)>(h4(),r, r+4); 
  q = _Assume_bounds_cast<ptr<int>, rel_align_value(sizeof(int))>(h4(), r, r+4); //expected-error{{invalid bounds cast}}
}

extern void f15(){
  int *p;
  array_ptr<int> q = 0;
  ptr<int> r = 0;
  ptr<int*> s = 0;
  ptr<ptr<int>> t = 0;
  array_ptr<int> qq : count(4)= 0;  
  r = _Assume_bounds_cast<ptr<int>, rel_align(int)>(q, q, q+4);  //expected-error{{invalid bounds cast}} expected-error{{expression has no bounds}}
  p = _Assume_bounds_cast<int *, rel_align_value(sizeof(int))>(q, q, q+4);  //expected-error{{invalid bounds cast}} expected-error{{expression has no bounds}}
  
  s = _Assume_bounds_cast<ptr<int*>, rel_align(int)>(q, q, q+4);  //expected-error{{invalid bounds cast}}  expected-error{{expression has no bounds}}
  t = _Assume_bounds_cast<ptr<ptr<int>>, rel_align_value(sizeof(int))>(q, q, q+4); //expected-error{{invalid bounds cast}}  expected-error{{expression has no bounds}}

  qq = _Assume_bounds_cast<array_ptr<int>, rel_align(int)>(q, q, q+4); //expected-error {{expression has no bounds}}
  p = _Assume_bounds_cast<int *, rel_align_value(sizeof(int))>(q, q, q+4);  //expected-error{{invalid bounds cast}} expected-error{{expression has no bounds}}
}

extern void f16(){
  int *p;
  int len=5;
  array_ptr<int> q = 0;
  ptr<int> r = 0;
  r = _Dynamic_bounds_cast<ptr<int>, rel_align(len)>(q, q, q+4); //expected-error {{unknown type name 'len'}} expected-error {{invalid bounds cast}} expected-error{{expression has no bounds}}
  p = _Dynamic_bounds_cast<int *, rel_align_value(len)>(q, q, q+4); //expected-error {{expression is not an integer constant expression}} expected-error{{invalid bounds cast}} expected-error{{expression has no bounds}}
  p = _Dynamic_bounds_cast<int *, rel_align_value(1)>(q, q, q+4); //expected-error{{invalid bounds cast}} expected-error{{expression has no bounds}}
  
  p = _Dynamic_bounds_cast<int *, rel_align_value(1)>(q); //expected-error{{expression has no bounds}}
  p = _Dynamic_bounds_cast<int *, rel_align(int)>(q); //expected-error{{expression has no bounds}}
}

extern int* f17(int arr checked[]) {
  int k;
  return _Assume_bounds_cast<int*>(k); //expected-error{{expression has no bounds}}
}

extern void f18(int i){
  int c;
  int *p;
  char *cp;
  ptr<int> q = 0;
  ptr<char> cq = 0;
  array_ptr<int> r : count(5) = 0;
  array_ptr<char> cr = 0;

  /* DestType : unchecked, Src->Type : unchecked */
  p = _Dynamic_bounds_cast<int *>(p); //expected-error{{expression has no bounds}}
  p = _Dynamic_bounds_cast<char *>(p);  // expected-warning {{incompatible pointer type}} expected-error{{expression has no bounds}}
  p = _Dynamic_bounds_cast<int *>(p, 1); // expected-error {{invalid bounds cast}} expected-error{{expression has no bounds}}
  p = _Dynamic_bounds_cast<int *>(p, p, p+1); // expected-error {{invalid bounds cast}} expected-error{{expression has no bounds}}
  /* DestType : unchecked, Src->Type : int */

  p = _Dynamic_bounds_cast<int *>(i); //expected-error {{expression has no bounds}}
  p = _Dynamic_bounds_cast<char *>(i);   //expected-error {{expression has no bounds}} expected-warning {{incompatible pointer}}
  p = _Dynamic_bounds_cast<int *>(i, 1);  //expected-error {{invalid bounds cast}} expected-error{{expression has no bounds}}

  p = _Dynamic_bounds_cast<int *>(i, i, i+1); //expected-error {{invalid bounds cast}} expected-error{{expression has no bounds}}
  p = _Dynamic_bounds_cast<int *>(q); 
  p = _Dynamic_bounds_cast<int *>(q, 1); //expected-error {{invalid bounds cast}}
  p = _Dynamic_bounds_cast<int *>(q, q, q+1); //expected-error {{arithmetic on _Ptr}} expected-error {{invalid bounds cast}}

  /* DestType : unchecked, Src->Type : array_ptr */
  p = _Dynamic_bounds_cast<int *>(r); 
  p = _Dynamic_bounds_cast<int *>(r, 1); // expected-error {{invalid bounds cast}}
  p = _Dynamic_bounds_cast<int *>(r, r, r+1); //expected-error {{invalid bounds cast}}

  /* DestType : ptr, Src->Type : unchecked */
  q = (ptr<int>)p;  // expected-error 2 {{expression has no bounds}}
  q = (ptr<char>)p; // expected-error {{incompatible type}}
  q = _Dynamic_bounds_cast<ptr<int>>(p); //expected-error {{expression has no bounds}}
  q = _Dynamic_bounds_cast<ptr<char>>(p); // expected-error {{assigning to '_Ptr<int>'}}
  q = _Dynamic_bounds_cast<ptr<int>>(p, 1); //expected-error {{expression has no bounds}} expected-error{{invalid bounds cast}}
  q = _Dynamic_bounds_cast<ptr<int>>(p, p, p+1); //expected-error {{invalid bounds cast}} expected-error {{expression has no bounds}}

  /* DestType : ptr, Src->Type : int */
  q = _Dynamic_bounds_cast<ptr<int>>(i); //expected-error {{expression has no bounds}}
  q = _Dynamic_bounds_cast<ptr<char>>(i); //expected-error{{assigning to '_Ptr<int>'}}
  q = _Dynamic_bounds_cast<ptr<int>>(i, 1); //expected-error {{invalid bounds cast}} expected-error {{expression has no bounds}}
  q = _Dynamic_bounds_cast<ptr<int>>(i, i, i+1); //expected-error {{invalid bounds cast}} expected-error {{expression has no bounds}}
  /* DestType : ptr, Src->Type : ptr */
  q = _Dynamic_bounds_cast<ptr<int>>(q);
  q = _Dynamic_bounds_cast<ptr<int>>(q, 1); //expected-error {{invalid bounds cast}}
  q = _Dynamic_bounds_cast<ptr<int>>(q, q, q+1); //expected-error {{arithmetic on _Ptr}} expected-error {{invalid bounds cast}} 

  /* DestType : ptr, Src->Type : array_ptr */
  q = _Dynamic_bounds_cast<ptr<int>>(r);
  q = _Dynamic_bounds_cast<ptr<int>>(r, 1); //expected-error{{invalid bounds cast}}
  q = _Dynamic_bounds_cast<ptr<int>>(r, r, r+1); //expected-error{{invalid bounds cast}}

  /* DestType : array_ptr, Src->Type : unchecked */
  array_ptr<char> rr;
  array_ptr<int> rrr;
  rr = (array_ptr<char>)p;
  r = (array_ptr<int>)p; //expected-error {{expression has no bounds}}
  r = (array_ptr<int>)p; // expected-error {{expression has no bounds}}
  r = _Dynamic_bounds_cast<array_ptr<int>>(p); //expected-error {{invalid bounds cast}} expected-error {{expression has no bounds}}
  rr = _Dynamic_bounds_cast<array_ptr<int>>(p); //expected-error{{incompatible type}} //expected-error {{invalid bounds cast}}
  rrr = _Dynamic_bounds_cast<array_ptr<int>>(p);//expected-error {{invalid bounds cast}} expected-error{{expression has no bounds}}
  r = _Dynamic_bounds_cast<array_ptr<char>>(p); // expected-error {{incompatible type}} //expected-error {{invalid bounds cast}}
  r = _Dynamic_bounds_cast<array_ptr<int>>(p, 1);  //expected-error {{expression has no bounds}}
  r = _Dynamic_bounds_cast<array_ptr<int>>(p, p, p+1);  //expected-error {{expression has no bounds}}

  /* DestType : array_ptr, Src->Type : int */
  r = _Dynamic_bounds_cast<array_ptr<int>>(i); //expected-error {{invalid bounds cast}} expected-error {{expression has no bounds}}
  r = _Dynamic_bounds_cast<array_ptr<char>>(i); //expected-error {{invalid bounds cast}} expected-error{{assigning to}}
  r = _Dynamic_bounds_cast<array_ptr<int>>(i, 1); //expected-error {{expression has no bounds}}
  r = _Dynamic_bounds_cast<array_ptr<int>>(i, i, i+1); //expected-error {{expression has no bounds}} //expected-error 2 {{expected expression with pointer}}

  /* DestType : ptr, Src->Type : ptr */
  int len;
  r = _Dynamic_bounds_cast<array_ptr<int>>(q); //expected-error {{invalid bounds cast}} 
  r = _Dynamic_bounds_cast<array_ptr<int>>(q, len);
  r = _Dynamic_bounds_cast<array_ptr<int>>(q, q, q+1);  // expected-error {{arithmetic on _Ptr type}} 

  /* DestType : ptr, Src->Type : array_ptr */
  r = _Dynamic_bounds_cast<array_ptr<int>>(r); //expected-error{{invalid bounds cast}} 
  r = _Dynamic_bounds_cast<array_ptr<int>>(r, 1);
  r = _Dynamic_bounds_cast<array_ptr<int>>(r, r, r+1);

  p = (char*)(p); // expected-warning{{incompatible pointer types assigning}}
  p = _Assume_bounds_cast<char*>(p); // expected-warning{{incompatible pointer types assigning}} expected-error{{expression has no bounds}}
  p = (char)(p);// expected-warning{{incompatible integer to}}

  p = _Assume_bounds_cast<char>(p); // expected-warning{{incompatible integer to}} expected-error {{invalid bounds cast}} expected-error{{expression has no bounds}}

  p = _Dynamic_bounds_cast<char>(p, p, p+2); //expected-warning{{incompatible integer to pointer}} expected-error {{invalid bounds cast}} expected-error{{expression has no bounds}}
  p = _Assume_bounds_cast<int*>(q); 

  p = _Assume_bounds_cast<int*>(cq); 

  p = _Assume_bounds_cast<int*>(cr); //expected-error{{expression has no bounds}}

  cp = _Assume_bounds_cast<char*>(p);  //expected-error{{expression has no bounds}}
  q = _Assume_bounds_cast<ptr<int>>(p); //expected-error {{expression has no bounds}}

  cq = _Assume_bounds_cast<ptr<char>>(p);//expected-error {{expression has no bounds}}
  c = _Assume_bounds_cast<int>(cq); //expected-error {{invalid bounds cast}}
  p = _Assume_bounds_cast<int*>(r);
}
