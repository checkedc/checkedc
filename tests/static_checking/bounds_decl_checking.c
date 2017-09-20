// Feature tests of static checking of bounds declarations.
// The following lines are for the LLVM test harness:
//
// RUN: %clang_cc1 -verify -verify-ignore-unexpected=note -fcheckedc-extension %s

#include <stdchecked.h>

struct S1 {
  int f;
};

int gtmp;
int *g1 = &gtmp;
ptr<int> g2 = &gtmp;
array_ptr<int> g3 = &gtmp;
array_ptr<int> g4 : count(1) = &gtmp;

extern void check_exprs(int *arg1, ptr<int> arg2, array_ptr<int> arg3,
                        array_ptr<int> arg4 : count(1)) {
  struct S1 s;
  int arr[5] = { 0, 1, 2, 3, 4 };

  // constants
  array_ptr<int> orig_arg4 : count(1) = arg4;
  arg4 = 0;
  arg4 = (int *)0xabcd;  // expected-error {{expression has no bounds}}
  arg4 = orig_arg4;

  // address-of
  int tmp1 = 0;
  arg4 = &tmp1;
  arg4 = &*arg4;
  arg4 = &*arg1;          // expected-error {{expression has no bounds}}
  arg4 = &s.f;
  ptr<struct S1> ps = &s;
  arg4 = &(ps->f);
  arg4 = &arr[5];

  // variables

  // globals assigned from a global
  g1 = g1;
  g2 = g1;            // expected-error {{expression has no bounds}}
  g3 = g1;
  g4 = g1;            // expected-error {{expression has no bounds}}

  g1 = g2;            // expected-error {{incompatible type}}
  g2 = g2;
  g3 = g2;
  g4 = g2;

  g1 = g3;            // expected-error {{incompatible type}}
  g2 = g3;            // expected-error {{expression has no bounds}}
  g3 = g3;
  g4 = g3;            // expected-error {{expression has no bounds}}

  g4 = g1;            // expected-error {{expression has no bounds}}
  g4 = g2;
  g4 = g3;            // expected-error {{expression has no bounds}}
  g4 = g4;

  // parameters assigned from a global
  arg1 = g1;
  arg2 = g1;            // expected-error {{expression has no bounds}}
  arg3 = g1;
  arg4 = g1;            // expected-error {{expression has no bounds}}

  arg1 = g2;            // expected-error {{incompatible type}}
  arg2 = g2;
  arg3 = g3;
  arg4 = g4;

  arg1 = g3;            // expected-error {{incompatible type}}
  arg2 = g3;            // expected-error {{expression has no bounds}}
  arg3 = g3;
  arg4 = g3;            // expected-error {{expression has no bounds}}

  arg1 = g4;            // expected-error {{incompatible type}}
  arg2 = g4;
  arg3 = g4;
  arg4 = g4;

  // globals assigned from parameters
  g1 = arg1;
  g2 = arg1;            // expected-error {{expression has no bounds}}
  g3 = arg1;
  g4 = arg1;            // expected-error {{expression has no bounds}}

  g1 = arg2;            // expected-error {{incompatible type}}
  g2 = arg2;
  g3 = arg3;
  g4 = arg4;

  g1 = arg3;            // expected-error {{incompatible type}}
  g2 = arg3;            // expected-error {{expression has no bounds}}
  g3 = arg3;
  g4 = arg3;            // expected-error {{expression has no bounds}}

  g1 = arg4;            // expected-error {{incompatible type}}
  g2 = arg4;
  g3 = arg4;
  g4 = arg4;

    // locals assigned from parameters
  int *t1 = &tmp1;
  ptr<int> t2 = &tmp1;
  array_ptr<int> t3 = &tmp1;
  array_ptr<int> t4 : count(1) = &tmp1;

  t1 = arg1;
  t2 = arg1;            // expected-error {{expression has no bounds}}
  t3 = arg1;
  t4 = arg1;            // expected-error {{expression has no bounds}}

  t1 = arg2;            // expected-error {{incompatible type}}
  t2 = arg2;
  t3 = arg3;
  t4 = arg4;

  t1 = arg3;            // expected-error {{incompatible type}}
  t2 = arg3;            // expected-error {{expression has no bounds}}
  t3 = arg3;
  t4 = arg3;            // expected-error {{expression has no bounds}}

  t1 = arg4;            // expected-error {{incompatible type}}
  t2 = arg4;
  t3 = arg4;
  t4 = arg4;

  // spot-check locals assigned from globals
  t1 = g4;            // expected-error {{incompatible type}}
  t2 = g3;            // expected-error {{expression has no bounds}}
  t3 = g2;
  t4 = g1;            // expected-error {{expression has no bounds}}

  // spot-check globals assigned from locals
  g1 = t2;            // expected-error {{incompatible type}}
  g2 = t3;            // expected-error {{expression has no bounds}}
  g3 = t4;
  g4 = t1;            // expected-error {{expression has no bounds}}

  // expressions

  // nested assignments
  array_ptr<int> t5 : count(1) = 0;
  t5 = (arg4 = t4);
  t5 = (t4 = arg4);
  t5 = (t4 = t3);     // expected-error 2 {{expression has no bounds}}

  // assignment through pointer
  *t1 = 1;
  *t2 = 2;
  *t3 = 3;            // expected-error {{expression has no bounds}}
  *t4 = 4;

  // read through a pointer
  int t6 = *t1;
  t6 = *t2;
  t6 = *t3;           // expected-error {{expression has no bounds}}
  t6 = *t4;

  // assignment via subcript
  t1[0] = 1;
  t3[0] = 3;          // expected-error {{expression has no bounds}}
  t4[0] = 4;

  // read via subscript

  int t7 = t1[0];
  t7 = t3[0];         // expected-error {{expression has no bounds}}
  t7 = t4[0];

  // pre-increment/post-increment
  ++(*t1);
  ++(*t2);
  ++(*t3);            // expected-error {{expression has no bounds}}
  ++(*t4);

  --(*t1);
  --(*t2);
  --(*t3);            // expected-error {{expression has no bounds}}
  --(*t4);

  (*t1)++;
  (*t2)++;
  (*t3)++;            // expected-error {{expression has no bounds}}
  (*t4)++;

  --(*t1);
  --(*t2);
  --(*t3);            // expected-error {{expression has no bounds}}
  --(*t4);
}

extern void test_f1(int *p);
extern void test_f2(ptr<int> p);
extern void test_f3(array_ptr<int> p);
extern void test_f4(array_ptr<int> p : count(1));
extern void test_f5(array_ptr<int> p : count(len), int len);

extern void check_call_args(int *arg1, ptr<int> arg2, array_ptr<int> arg3,
                            array_ptr<int> arg4 : count(1), 
                            array_ptr<int> arg5 : count(arglen), int arglen) {
  test_f1(arg1);
  test_f2(arg1);     // expected-error {{expression has no bounds}}
  test_f3(arg1);
  test_f4(arg1);     // expected-error {{argument has no bounds}}
  test_f5(arg1, 1);  // expected-error {{argument has no bounds}}

  test_f1(arg2);     // expected-error {{incompatible type}}
  test_f2(arg2);
  test_f3(arg2);
  test_f4(arg2);
  test_f5(arg2, 1);

  test_f1(arg3);     // expected-error {{incompatible type}}
  test_f2(arg3);     // expected-error {{expression has no bounds}}
  test_f3(arg3);
  test_f4(arg3);     // expected-error {{argument has no bounds}}
  test_f5(arg3, 1);  // expected-error {{argument has no bounds}}
  
  test_f1(arg4);     // expected-error {{incompatible type}}
  test_f2(arg4);
  test_f3(arg4);
  test_f4(arg4);
  test_f5(arg4, 1);

  test_f1(arg5);     // expected-error {{incompatible type}}
  test_f2(arg5);
  test_f3(arg5);
  test_f4(arg5);
  test_f5(arg5, 1);

  int count = arglen - 1;
  test_f5(arg5, ++count);  // expected-error {{increment expression not allowed}}
  test_f5(arg5, count++);  // expected-error {{increment expression not allowed}}
}

//
// Test checking of bounds declarations for arguments to functions with
// bounds-safe interfaces (BSI = bounds-safe interface)
//

extern void test_bsi_f1(int *p);
extern void test_bsi_f2(int *p : itype(ptr<int>));
extern void test_bsi_f3(int *p : itype(array_ptr<int>));
extern void test_bsi_f4(int *p : count(1));
extern void test_bsi_f5(int *p : count(len), int len);

extern void check_call_bsi(int *arg1, ptr<int> arg2, array_ptr<int> arg3,
                           array_ptr<int> arg4 : count(1),
                           array_ptr<int> arg5 : count(arglen), int arglen) {
  test_bsi_f1(arg1);    // no checking expected when passing unchecked pointers.
  test_bsi_f2(arg1);
  test_bsi_f3(arg1);
  test_bsi_f4(arg1);
  test_bsi_f5(arg1, 1);

  test_bsi_f1(arg2);     // expected-error {{incompatible type}}
  test_bsi_f2(arg2);
  test_bsi_f3(arg2);
  test_bsi_f4(arg2);
  test_bsi_f5(arg2, 1);

  test_bsi_f1(arg3);     // expected-error {{incompatible type}}
  test_bsi_f2(arg3);     // expected-error {{argument has no bounds}}                    
  test_bsi_f3(arg3);     
  test_bsi_f4(arg3);     // expected-error {{argument has no bounds}}
  test_bsi_f5(arg3, 1);  // expected-error {{argument has no bounds}}

  test_bsi_f1(arg4);     // expected-error {{incompatible type}}
  test_bsi_f2(arg4);
  test_bsi_f3(arg4);
  test_bsi_f4(arg4);
  test_bsi_f5(arg4, 1);

  test_bsi_f1(arg5);     // expected-error {{incompatible type}}
  test_bsi_f2(arg5);
  test_bsi_f3(arg5);
  test_bsi_f4(arg5);
  test_bsi_f5(arg5, 1);

  int count = arglen - 1;
  test_bsi_f5(arg5, ++count);  // expected-error {{increment expression not allowed}}
  test_bsi_f5(arg5, count++);  // expected-error {{increment expression not allowed}}
}

