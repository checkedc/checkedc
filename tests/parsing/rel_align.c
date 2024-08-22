// Feature tests of parsing new Checked C Relative Bounds Clause
//
// The following lines are for the LLVM test harness:
//
// RUN: %clang_cc1 -Wno-check-bounds-decls -verify %s

#include <stdchecked.h>

extern int cLen;
extern array_ptr<int> f : byte_count(cLen * sizeof(int));
extern array_ptr<int> g : bounds(f - 2, f + 3) rel_align(char);
extern array_ptr<int> h : bounds(f - 2, f + 3) rel_align_value(sizeof(char));
extern int* _Array i  _Count(5), * _Array j  _Bounds(i - 2, i + 3) rel_align(char),
    * _Array l  _Bounds(i - 2, i + 3) rel_align_value(sizeof(char)),
    * _Array k  _Bounds(j + 2, j + 5) rel_align(char),
    * _Array  m  _Bounds(j + 2, j + 5) rel_align_value(sizeof(char));

char buf checked[128] : count(128);
array_ptr<char> cursor : bounds(buf, buf + 128) rel_align(char) = buf + 64;
array_ptr<char> cursor1 : bounds(buf, buf + 128)
                              rel_align_value(sizeof(int)) = buf + 64;

extern void f1(array_ptr<int> arr : count(5)) {
  int len = 5;
  array_ptr<int> v : bounds(v, v + 5) rel_align(char) = arr;
  array_ptr<int> w : bounds(w, w + 5) rel_align_value(1) = arr;
  array_ptr<int> midarr : bounds(midarr - 1, midarr - 1 + 2) rel_align(char) = arr + 2;
  array_ptr<int> midarr1 : bounds(midarr1 - 1, midarr1 - 1 + 2) rel_align_value(sizeof(char)) = arr + 2;

  static array_ptr<int> cache1 : count(5);
  static int* _Array cache1_ptr  _Bounds(cache1 - 2, cache1 + 3) rel_align(char);
  static int* _Array cache1_ptr1  _Bounds(cache1 - 2, cache1 + 3) rel_align_value(sizeof(char));
}

extern void f2(array_ptr<int> arr : count(5)) {
  int len = 5;
  array_ptr<int> t : byte_count(5 * sizeof(int)) = arr,
                                    u : bounds(u, u + 5) rel_align(char) = arr,
                                    v : bounds(unknown) = arr;

  int* _Array t1  _Byte_count(5 * sizeof(int)) = arr,
                                 * _Array u1  _Bounds(u1, u1 + 5) rel_align_value(sizeof(int)) = arr,
                                 * _Array v1  _Bounds(unknown) = arr;
}

extern void f3(array_ptr<int> arr : count(5)) {
  int len = 5;
  array_ptr<int> v : bounds(v, (v + 5) + len - len) rel_align(char) = arr;
  array_ptr<int> w : bounds((w + len - (len)), (w + len)) rel_align(char) = arr;
  array_ptr<int> midarr : bounds(midarr - 1, (midarr - 1) + 2) rel_align(char) = arr + 2;

  array_ptr<int> v1 : bounds(v1, (v1 + 5) + len - len) rel_align_value(sizeof(char)) = arr;
  array_ptr<int> w1 : bounds((w1 + len - (len)), (w1 + len)) rel_align_value(sizeof(char)) = arr;
  int* _Array midarr1  _Bounds(midarr1 - 1, (midarr1 - 1) + 2) rel_align_value(1) = arr + 2;
}

extern void f4(array_ptr<int> arr : count(len), int len) {
  int count = len;
  int unknown = 0;
  array_ptr<int> s : bounds(arr + unknown, arr + len) rel_align(char) = arr;
  array_ptr<int> t : bounds(t, t + count) rel_align(char) = arr;
  array_ptr<int> s1 : bounds(arr + unknown, arr + len) rel_align_value(len) = arr; // expected-error {{expression is not an integer constant expression}} 
  array_ptr<int> t1 : bounds(t, t + count) rel_align_value(sizeof(char)) = arr;
  int bounds = len;
  array_ptr<int> u : bounds(u, u + bounds) rel_align(char) = arr;
  int* _Array u1  _Bounds(u1, u1 + bounds) rel_align_value(sizeof(char)) = arr;
}

extern void f5(void) {
  int len = 5;
  auto int arr checked[5] : bounds(arr, arr + 5) rel_align(char);
  auto int arr1 checked[5] : bounds(arr1, arr1 + 5)
                                 rel_align_value(sizeof(char));
  auto int arr2 checked[5] : bounds(arr2, arr2 + len) rel_align(char);
  auto int arr3 checked[5] : bounds(arr3, arr3 + len) rel_align_value(sizeof(len));

  static int cache checked[5] : count(5);
  static array_ptr<int> cache_ptr : bounds(cache - 2, cache + 3) rel_align(char);
  static int* _Array cache_ptr1  _Bounds(cache - 2, cache + 3) rel_align_value(sizeof(char));
}

extern void f6(int *arr checked[] : count(5)) {
  int len = 5;
  int((*arr0 checked[5])(int, int)) : count(len);
  int((*arr1 checked[5])(int, int)) : bounds(arr, arr + len) rel_align(char);
  int((*arr2 checked[5])(int, int)) : bounds(arr2, arr2 + len) rel_align_value(len); // expected-error {{expression is not an integer constant expression}} 

  ptr<int(int, int)> arr3 checked[5] : bounds(arr3, arr3 + 5) rel_align(char) = {0};
  ptr<int(int, int)> arr4 checked[5] : bounds(arr4, arr4 + 5) rel_align_value(sizeof(char)) = {0};

  array_ptr<ptr<int(int, int)>> v : bounds(v, v + 5) rel_align(char) = 0;
  array_ptr<ptr<int(int, int)>> w : bounds(v, v + 5) rel_align_value(sizeof(char)) = 0;
  array_ptr<ptr<int(int, int)>> x : bounds(v, v + 5) rel_align(char) = 0;
  array_ptr<ptr<int(int, int)>> y : bounds(v, v + 5) rel_align_value(1) = 0;
  array_ptr<ptr<int(int len, array_ptr<int> arr : count(len))>> z
                                                : bounds(x, x + 5) rel_align(char) = 0;
  array_ptr<ptr<int(int len, array_ptr<int> arr : count(len))>> a
                                                : bounds(x, x + 5) rel_align_value(sizeof(char)) = 0;
}

extern void f7(array_ptr<int> arr : count(5)) {
  array_ptr<int> p : bounds(start, end + 5) rel_align(char) = 0; // expected-error {{undeclared identifier 'start'}} \
                                                                 // expected-error {{undeclared identifier 'end'}} 
  array_ptr<ptr<int(int len, array_ptr<int> arr : count(badvar))>> x // expected-error {{undeclared identifier 'badvar'}}
      : bounds(x, x + 5) rel_align(char) = 0;
}

extern void f7s(array_ptr<int> arr : count(5)) {
  array_ptr<int> p : bounds(start, end + 5) rel_align_value(sizeof(char)) = 0; // expected-error {{undeclared identifier 'start'}}\
                                                                               // expected-error {{undeclared identifier 'end'}} 
  array_ptr<ptr<int(int len, array_ptr<int> arr : count(badvar))>> x // expected-error {{undeclared identifier 'badvar'}}
      : bounds(x, x + 5) rel_align_value(sizeof(p)) = 0;
}

extern array_ptr<int> f8(array_ptr<int> arr : bounds(arr, arr + 5) rel_align(char))
                      : bounds(arr, arr + 5) rel_align(char);

extern array_ptr<int> f8s(array_ptr<int> arr : bounds(arr, arr + 5) rel_align_value(1))
                      : bounds(arr, arr + 5) rel_align_value(sizeof(char));

extern array_ptr<int> f9(int start, array_ptr<int> arr : bounds(arr - start, arr - start + 5))
                      : bounds(arr - start, arr - start + 5) rel_align(char);

extern array_ptr<int> f9s(int start, array_ptr<int> arr : bounds(arr - start, arr - start + 5))
                      : bounds(arr - start, arr - start + 5) rel_align_value(1);

extern array_ptr<int> f10(int bounds, array_ptr<int> arr : count(bounds))
                      : bounds(arr, arr + bounds) rel_align(char);

extern int* _Array f10s(int bounds, int* _Array arr  _Count(bounds))
    _Bounds(arr, arr + bounds) rel_align_value(sizeof(char));

extern int* _Array f11(int* _Array arr  _Bounds(arr, arr + 5) rel_align(char))
    _Bounds(arr, arr + 5) rel_align(char) {
  return arr;
}

extern array_ptr<int> f11s(array_ptr<int> arr : bounds(arr, arr + 5) rel_align_value(5))
    : bounds(arr, arr + 5) rel_align_value(sizeof(int)) {
  return arr;
}

extern array_ptr<int> f12(int start, array_ptr<int> arr : bounds(arr - start, arr - start + 5))
    : bounds(arr - start, arr - start + 5) rel_align(char) {
  return arr;
}

extern array_ptr<int> f12s(int start, array_ptr<int> arr : bounds(arr - start, arr - start + 5))
    : bounds(arr - start, arr - start + 5) rel_align_value(sizeof(char)) {
  return arr;
}

extern array_ptr<int> f13(int bounds, array_ptr<int> arr : count(bounds))
    : bounds(arr, arr + bounds) rel_align(char) {
  return arr;
}

extern array_ptr<int> f13s(int bounds, array_ptr<int> arr : count(bounds))
    : bounds(arr, arr + bounds) rel_align_value(sizeof(char)) {
  return arr;
}

extern array_ptr<ptr<int>> f14(array_ptr<ptr<int>> arr : count(5))
    : bounds(arr, arr + 5) rel_align(char) {
  return arr;
}

extern array_ptr<ptr<int>> f14s(array_ptr<ptr<int>> arr  _Count(5))
    _Bounds(arr, arr + 5) rel_align_value(5) {
  return arr;
}

extern array_ptr<int[10]> f15(array_ptr<int[10]> arr : count(5))
    : bounds(arr, arr + 3) rel_align(char) {
  return arr;
}

extern array_ptr<int[10]> f15s(array_ptr<int[10]> arr  _Count(5))
    _Bounds(arr, arr + 3) rel_align_value(sizeof(char)) {
  return arr;
}

extern void f16(void) {
  ptr<array_ptr<int>(array_ptr<int> arg : count(5))
    : bounds(arg, arg + 5) rel_align(char)> r1 = 0;
  ptr<array_ptr<int>(array_ptr<int> arg1 : count(5))
    : bounds(arg1, arg1 + 5) rel_align_value(5)> r2 = 0;
  ptr<int(array_ptr<int> arg2 : count(n), int n)
    : bounds(arg2, arg2 + n) rel_align(char)> r3 = 0;
  ptr<int(array_ptr<int> arg3 : count(n), int n)
    : bounds(arg3, arg3 + n) rel_align_value(sizeof(char))> r4 = 0;
  typedef int func2(int* _Array arr  _Count(len), int len)
      _Bounds(arr, arr + len) rel_align(char);
}

extern array_ptr<int> f17(int len, array_ptr<int> arr : count(len)) : boounds(arr, arr + len) rel_align(1) { // expected-error {{expected bounds expression}}
													    }
extern array_ptr<int> f18(int len, array_ptr<int> arr : count(len)) : boounds(arr, arr + len) rel_align(char) { // expected-error {{expected bounds expression}}
}
extern int* _Array f19(int len, int* _Array arr  _Count(len)) : boounds(arr, arr + len) rel_align_value(len) { // expected-error {{expected bounds expression}}
}

int f20(void) {
  int buffer checked[100];
  struct S30 {
    int len;
    array_ptr<int> arr : bounds(buffer, buffer + len) rel_align(char); // expected-error 2 {{use of undeclared member 'buffer'}}
    int* _Array arr1 _Bounds(buffer, buffer + len) rel_align_value(sizeof(len));// expected-error 2 {{use of undeclared member 'buffer'}}
    array_ptr<int> arr2: bounds(buffer, buffer + len) rel_align_value(sizeof(char));// expected-error 2 {{use of undeclared member 'buffer'}}
  };
}

extern void f21(int *p : itype(ptr<int>) rel_alive(1), int y) {  // expected-error {{expected ')'}} expected-note {{to match this '('}}
   *p = y; // expected-error {{use of undeclared}}
}

extern void f22(array_ptr<int> p : bounds(p, p+1) rel_alive(1), int y) { // expected-error {{expected ')'}} expected-note {{to match this '('}}
  *p = y; //expected-error {{use of undeclared identifier 'y'}}
}

extern void f23(int *p : iitype(ptr<int>) rel_alive(1), int y) {// expected-error {{expected bounds expression or bounds-safe interface type}} 
   *p = y;
}

extern array_ptr<int> f24(array_ptr<int> arr : bounds(arr, arr + 5) rel_align(1))// expected-error {{expected a type}}
  : bounds(arr, arr + 5) rel_align(1);// expected-error {{expected a type}}

extern array_ptr<int> f25(int len, array_ptr<int> arr : count(len)) : boounds(arr, arr + len) rel_align(1){} // expected-error {{expected bounds expression or bounds-safe interface type}}

extern array_ptr<int> f26(int len, array_ptr<int> arr : count(len)) : boounds() rel_align(1) {} // expected-error {{expected bounds expression or bounds-safe interface type}}

extern int* _Array f27(int len, int* _Array arr  _Count(len)) : boounds() rel_alive(1) { // expected-error {{expected bounds expression or bounds-safe interface type}}
}

extern void f28(void) {
   int* _Array arg _Bo0unds(arg, arg + 5) rel_alive(1) = 0;  // expected-error {{expected ';' at end of declaration}}
   int* _Array arg1 _Bo0unds rel_alive(1) = 0;  // expected-error {{expected ';' at end of declaration}}
   int* _Array arg2 _Bo0unds rel_align(1) = 0;  // expected-error {{expected ';' at end of declaration}}
}

struct S1 {
  array_ptr<int> arr : bounds(arr, arr + 5) rel_align(char);
  array_ptr<int> arr1 : bounds(arr1, arr1 + 5) rel_align_value(1);
};

struct S2 {
  int start;
  array_ptr<int> arr : bounds(arr - start, arr - start + 5) rel_align(char);
  int* _Array arr1  _Bounds(arr1 - start, arr1 - start + 5)
      rel_align_value(sizeof(char) + sizeof(char));
};

struct S3 {
  int unknown;
  array_ptr<int> arr2 : count(unknown);
  array_ptr<int> arr3 : bounds(unknown + arr2, unknown + arr2 + 5) rel_align(1);  // expected-error {{expected ')'}} \
                                                              // expected-note {{to match this '('}} \
                                                              // expected-error {{expected a type}} \
                                                              // expected-error {{expected range bounds expression}} 
  array_ptr<int> arr4 : bounds(arr2, arr2 + unknown) rel_align(char);
  
  array_ptr<int> arr5 : bounds(unknown + arr2, unknown + arr2 + 5) rel_align_value(sizeof(char));// expected-error {{expected ')'}} \
                                                                                           // expected-note {{to match this '('}} \
                                                                                           // expected-error {{expected range bounds expression}} 
  int* _Array arr6  _Bounds(arr2, arr2 + unknown) rel_align_value(1);
};

struct S4 {
  int bounds;
  array_ptr<int> arr1 : bounds(bounds + arr1, bounds + arr1 + 2) rel_align(char);
  array_ptr<int> arr2 : bounds(unknown);
  array_ptr<int> arr3 : bounds(bounds + arr2, bounds + arr2 + 5) rel_align(char);
  array_ptr<int> arr4 : bounds(arr3, arr3 + bounds) rel_align(char);

  array_ptr<int> arr5 : bounds(bounds + arr5, bounds + arr5 + 2) rel_align_value(sizeof(int));
  array_ptr<int> arr6 : bounds(bounds + arr2, bounds + arr2 + 5) rel_align_value(sizeof(char));
  int* _Array arr7  _Bounds(arr6, arr6 + bounds) rel_align_value(sizeof(char));
};

struct S5 {
  array_ptr<ptr<int>> arr : bounds(arr, arr + 5) rel_align(char);
  array_ptr<ptr<int>> arr1 : bounds(arr1, arr1 + 5) rel_align_value(sizeof(char));
  array_ptr<ptr<int>> mid1 : bounds(arr1, arr1 + 5) rel_align(char);
  array_ptr<ptr<int>> mid2 : bounds(arr1, arr1 + 5) rel_align_value(sizeof(char));
  array_ptr<int[10]> arr2 : bounds(arr2, arr2 + 5) rel_align(char);
  array_ptr<int[10]> arr3 : bounds(arr3, arr3 + 5) rel_align_value(sizeof(char));
  array_ptr<int[10]> mid3 : bounds(arr2, arr2 + 5) rel_align(char);
  array_ptr<int[10]> mid4 : bounds(arr2, arr2 + 5) rel_align_value(1);
  array_ptr<int checked[10]> arr4 : bounds(arr4, arr4 + 9) rel_align(int);
  int *arr12 : bounds(arr12, arr12 + 9) rel_align(char);
  int *arr13 : bounds(arr13, arr13 + 9) rel_align_value(sizeof(char));
  int arr14[10] : bounds(arr14, arr14 + 9) rel_align(char);
  int arr15[10]  _Bounds(arr15, arr15 + 9) rel_align_value(sizeof(char));
};

extern void S6(void) {
  ptr<array_ptr<int>(array_ptr<int> arg : count(5))
    : bounds(arg, arg + 5) rel_align(char)> r1 = 0;
  ptr<array_ptr<int>(array_ptr<int> arg1 : count(5))
    : bounds(arg1, arg1 + 5) rel_align_value(sizeof(char))> r2 = 0;
  ptr<int(array_ptr<int> arg2 : count(n), int n)
    : bounds(arg2, arg2 + n) rel_align(char)> r3 = 0;
  ptr<int(int* _Array arg3  _Count(n), int n)
          _Bounds(arg3, arg3 + n) rel_align_value(n)> r4 = 0; // expected-error {{expression is not an integer constant expression}}
}

struct S7 {
  struct S8 {
    array_ptr<int> lower;
    array_ptr<int> upper;
  } pair;
  array_ptr<int> arr : bounds(pair.lower, pair.upper) rel_align(char);
  array_ptr<int> arr1 : bounds(pair.lower, pair.upper) rel_align_value(sizeof(pair));
  struct S9 {
    array_ptr<int> arr2 : bounds(pair.lower, pair.upper) rel_align(char);
    int* _Array arr3  _Bounds(pair.lower, pair.upper) rel_align_value(sizeof(pair));
  } nested;
};

struct S10 {
  struct {
    array_ptr<int> lower;
    array_ptr<int> upper;
  } pair;
  array_ptr<int> arr : bounds(pair.lower, pair.upper) rel_align(char);
  array_ptr<int> arr1 : bounds(pair.lower, pair.upper) rel_align_value(sizeof(char));
  struct {
    array_ptr<int> arr2 : bounds(pair.lower, pair.upper) rel_align(char);
    int* _Array arr3  _Bounds(pair.lower, pair.upper) rel_align_value(sizeof(pair));
  } nested;
};

struct S11 {
  array_ptr<int> arr : bounds(arr, unknown_id) rel_align(char); // expected-error {{use of undeclared member}} 

  int* _Array arr1 _Bounds(arr1, unknown_id) rel_align_value(sizeof(char)); // expected-error {{use of undeclared member}}

};

struct S12 {
  int len;
  array_ptr<int> arr : boounds(arr, arr + 5) rel_align(char);  // expected-error 2 {{use of undeclared identifier 'arr'}} \
                                               // expected-warning {{implicit declaration of function 'boounds'}} 
  array_ptr<int> arr1: booundss(arr1, arr1 + 5) rel_align_value(sizeof(int)); // expected-error 2 {{use of undeclared identifier 'arr1'}} \
                                               // expected-warning {{implicit declaration of function 'booundss'}}
};

struct S13 {
  int len;
  array_ptr<int> arr : bounds() rel_align(char); // expected-error {{expected expression}} 

  array_ptr<int> arr1: bounds() rel_align_value(len); // expected-error {{expected expression}} \
						      // expected-error {{expression is not an integer constant expression}} 
  int* _Array arr3  _Bounds() rel_align(); // expected-error {{expected expression}}  expected-error {{expected a type}}

  int* _Array arr4  _Bounds() rel_align_value(); // expected-error 2 {{expected expression}}
};

array_ptr<int> global_bound;

struct S14 {
  int len;
  int* _Array arr  _Bounds(global_bound, global_bound + len) rel_align(int); // expected-error 2 {{use of undeclared member 'global_bound'}}

  array_ptr<int> arr1
      : bounds(global_bound, global_bound + len) rel_align_value(sizeof(int)); // expected-error 2 {{use of undeclared member 'global_bound'}}
};
