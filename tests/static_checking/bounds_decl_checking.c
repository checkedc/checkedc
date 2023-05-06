// Feature tests of static checking of bounds declarations.
// The following lines are for the LLVM test harness:
//
// RUN: %clang_cc1 -verify -verify-ignore-unexpected=note %s

#include <stdchecked.h>

struct S1 {
  int f;
};

int gtmp;
int *g1 = &gtmp;
ptr<int> g2 = &gtmp;
array_ptr<int> g3 = &gtmp;
int* _Array g4  _Count(1) = &gtmp;


extern void check_exprs(int *arg1, ptr<int> arg2, array_ptr<int> arg3,
                        array_ptr<int> arg4 : count(1)) {
  struct S1 s;
  int arr[5] = { 0, 1, 2, 3, 4 };

  // constants
  array_ptr<int> orig_arg4 : count(1) = arg4;
  arg4 = 0;
  arg4 = (int *)0xabcd;  // expected-error {{inferred bounds for 'arg4' are unknown after assignment}}
  arg4 = orig_arg4;

  // compound literals
  array_ptr<int> arr1 : count(3) = (int checked[3]){0, 1, 2};
  array_ptr<int> arr2 : count(4) = (int checked[3]){0, 1, 2}; // expected-error {{declared bounds for 'arr2' are invalid after initialization}}
  array_ptr<struct S1> arr_struct1 : count(1) = &(struct S1){0};
  struct S1* _Array arr_struct2  _Count(2) = &(struct S1){0}; // expected-error {{declared bounds for 'arr_struct2' are invalid after initialization}}

  // TODO: assignments of variables with array types
  // to pointer variables, and reads/writes of struct members.

  // address-of
  int tmp1 = 0;
  arg4 = &tmp1;
  arg4 = &*arg4;
  arg4 = &*arg1;          // expected-error {{inferred bounds for 'arg4' are unknown after assignment}}
  arg4 = &s.f;
  ptr<struct S1> ps = &s;
  arg4 = &(ps->f);
  // TODO:
  // - Simplification of &e1[e2] to e1 + e2 during checking of bounds declaration.
  // - Re-expressing bounds for arg4 in terms of the rhs and checking those bounds.
  // - Possibly reassociation.
  arg4 = &arr[4];         // expected-warning {{cannot prove declared bounds for 'arg4' are valid after assignment}}

  // variables

  // globals assigned from a global
  g1 = g1;
  g2 = g1;            // expected-error {{expression has unknown bounds}}
  g3 = g1;
  g4 = g1;            // expected-error {{inferred bounds for 'g4' are unknown after assignment}}

  g1 = g2;            // expected-error {{incompatible type}}
  g2 = g2;
  g3 = g2;
  g4 = g2;

  g1 = g3;            // expected-error {{incompatible type}}
  g2 = g3;            // expected-error {{expression has unknown bounds}}
  g3 = g3;
  g4 = g3;            // expected-error {{inferred bounds for 'g4' are unknown after assignment}}

  g4 = g1;            // expected-error {{inferred bounds for 'g4' are unknown after assignment}}
  g4 = g2;
  g4 = g3;            // expected-error {{inferred bounds for 'g4' are unknown after assignment}}
  g4 = g4;

  // parameters assigned from a global
  arg1 = g1;
  arg2 = g1;            // expected-error {{expression has unknown bounds}}
  arg3 = g1;
  arg4 = g1;            // expected-error {{inferred bounds for 'arg4' are unknown after assignment}}

  arg1 = g2;            // expected-error {{incompatible type}}
  arg2 = g2;
  arg3 = g3;
  arg4 = g4;

  arg1 = g3;            // expected-error {{incompatible type}}
  arg2 = g3;            // expected-error {{expression has unknown bounds}}
  arg3 = g3;
  arg4 = g3;            // expected-error {{inferred bounds for 'arg4' are unknown after assignment}}

  arg1 = g4;            // expected-error {{incompatible type}}
  arg2 = g4;
  arg3 = g4;
  arg4 = g4;

  // globals assigned from parameters
  g1 = arg1;
  g2 = arg1;            // expected-error {{expression has unknown bounds}}
  g3 = arg1;
  g4 = arg1;            // expected-error {{inferred bounds for 'g4' are unknown after assignment}}

  g1 = arg2;            // expected-error {{incompatible type}}
  g2 = arg2;
  g3 = arg2;
  g4 = arg2;

  g1 = arg3;            // expected-error {{incompatible type}}
  g2 = arg3;            // expected-error {{expression has unknown bounds}}
  g3 = arg3;
  g4 = arg3;            // expected-error {{inferred bounds for 'g4' are unknown after assignment}}

  g1 = arg4;            // expected-error {{incompatible type}}
  g2 = arg4;
  g3 = arg4;
  g4 = arg4;

    // locals assigned from parameters
  int *t1 = &tmp1;
  ptr<int> t2 = &tmp1;
  array_ptr<int> t3 = &tmp1;
  int* _Array t4  : count(1) = &tmp1;

  t1 = arg1;
  t2 = arg1;            // expected-error {{expression has unknown bounds}}
  t3 = arg1;
  t4 = arg1;            // expected-error {{inferred bounds for 't4' are unknown after assignment}}

  t1 = arg2;            // expected-error {{incompatible type}}
  t2 = arg2;
  t3 = arg3;
  t4 = arg4;

  t1 = arg3;            // expected-error {{incompatible type}}
  t2 = arg3;            // expected-error {{expression has unknown bounds}}
  t3 = arg3;
  t4 = arg3;            // expected-error {{inferred bounds for 't4' are unknown after assignment}}

  t1 = arg4;            // expected-error {{incompatible type}}
  t2 = arg4;
  t3 = arg4;
  t4 = arg4;

  // spot-check locals assigned from globals
  t1 = g4;            // expected-error {{incompatible type}}
  t2 = g3;            // expected-error {{expression has unknown bounds}}
  t3 = g2;
  t4 = g1;            // expected-error {{inferred bounds for 't4' are unknown after assignment}}

  // spot-check globals assigned from locals
  g1 = t2;            // expected-error {{incompatible type}}
  g2 = t3;            // expected-error {{expression has unknown bounds}}
  g3 = t4;
  g4 = t1;            // expected-error {{inferred bounds for 'g4' are unknown after assignment}}

  // expressions

  // nested assignments
  array_ptr<int> t5 : count(1) = 0;
  t5 = (arg4 = t4);
  t5 = (t4 = arg4);
  t5 = (t4 = t3);     // expected-error {{inferred bounds for 't5' are unknown after assignment}} \
                      // expected-error {{inferred bounds for 't4' are unknown after assignment}}

  // assignment through pointer
  *t1 = 1;
  *t2 = 2;
  *t3 = 3;            // expected-error {{expression has unknown bounds}}
  *t4 = 4;

  // read through a pointer
  int t6 = *t1;
  t6 = *t2;
  t6 = *t3;           // expected-error {{expression has unknown bounds}}
  t6 = *t4;

  // assignment via subcript
  t1[0] = 1;
  t3[0] = 3;          // expected-error {{expression has unknown bounds}}
  t4[0] = 4;

  // read via subscript

  int t7 = t1[0];
  t7 = t3[0];         // expected-error {{expression has unknown bounds}}
  t7 = t4[0];

  // pre-increment/post-increment
  ++(*t1);
  ++(*t2);
  ++(*t3);            // expected-error {{expression has unknown bounds}}
  ++(*t4);

  --(*t1);
  --(*t2);
  --(*t3);            // expected-error {{expression has unknown bounds}}
  --(*t4);

  (*t1)++;
  (*t2)++;
  (*t3)++;            // expected-error {{expression has unknown bounds}}
  (*t4)++;

  --(*t1);
  --(*t2);
  --(*t3);            // expected-error {{expression has unknown bounds}}
  --(*t4);

  // Initialization with unknown src bounds.
  array_ptr<int> t8 : count(1) = t3;  // expected-error {{inferred bounds for 't8' are unknown after initialization}}
}

nt_array_ptr<int> g11 : bounds(unknown) = 0;
nt_array_ptr<int> g12 = (int nt_checked[]) { 1, 0 }; // default bounds of count(0);
int* _Nt_array g13  _Count(1) = (int nt_checked[]) { 1, 0 };

struct S2 {
  nt_array_ptr<int> f1 : bounds(unknown);
  nt_array_ptr<int> f2;
  int* _Nt_array f3  _Count(1);
};

extern void check_exprs_nullterm(nt_array_ptr<int> arg1 : bounds(unknown),
                                 nt_array_ptr<int> arg2,
                                 nt_array_ptr<int> arg3 : count(1)) {
  int arr nt_checked[5] = { 0, 1, 2, 3, 0 };

  // constants
  arg1 = 0;
  arg2 = 0;
  arg3 = 0;
  arg1 = (nt_array_ptr<int>)0xabcd;
  arg2 = (nt_array_ptr<int>)0xabcd;  // expected-error {{inferred bounds for 'arg2' are unknown after assignment}}
  arg3 = (int* _Nt_array)0xabcd;  // expected-error {{inferred bounds for 'arg3' are unknown after assignment}}

  // address-of
  arg1 = &*arg1;
  arg1 = &*arg2;
  arg1 = &*arg3;
  arg1 = &*arr;           // arg1 and &*arr both have type _Nt_array_ptr<int>.
  arg1 = &arr[1];         // arg1 and &arr[1] both have type _Nt_array_ptr<int>.
  arg2 = &*arg1;          // expected-error {{inferred bounds for 'arg2' are unknown after assignment}}
  arg2 = &*arg2;
  arg2 = &*arg3;
  arg3 = &*arg1;          // expected-error {{inferred bounds for 'arg3' are unknown after assignment}}
  arg3 = &*arg2;          // expected-error {{declared bounds for 'arg3' are invalid after assignment}}
  arg3 = &*arg3;

  // variables

  // globals assigned from a global
  g11 = g11;
  g12 = g11;           // expected-error {{inferred bounds for 'g12' are unknown after assignment}}
  g13 = g11;           // expected-error {{inferred bounds for 'g13' are unknown after assignment}}

  g11 = g12;
  g12 = g12;
  g13 = g12;           // expected-error {{declared bounds for 'g13' are invalid after assignment}}

  g11 = g13;
  g12 = g13;
  g13 = g13;

  // parameters assigned from a global
  arg1 = g11;
  arg2 = g11;           // expected-error {{inferred bounds for 'arg2' are unknown after assignment}}
  arg3 = g11;           // expected-error {{inferred bounds for 'arg3' are unknown after assignment}}

  arg1 = g12;
  arg2 = g12;
  arg3 = g12;           // expected-error {{declared bounds for 'arg3' are invalid after assignment}}

  arg1 = g13;
  arg2 = g13;
  arg3 = g13;


  // globals assigned from parameters
  g11 = arg1;
  g12 = arg1;           // expected-error {{inferred bounds for 'g12' are unknown after assignment}}
  g13 = arg1;           // expected-error {{inferred bounds for 'g13' are unknown after assignment}}

  g11 = arg2;
  g12 = arg2;
  g13 = arg3;

  g11 = arg3;
  g12 = arg3;
  g13 = arg3;

  // locals assigned from parameters
  nt_array_ptr<int> t1 : bounds(unknown);
  nt_array_ptr<int> t2 = 0;
  int* _Nt_array t3  _Count(1) = 0;

  t1 = arg1;
  t2 = arg1;            // expected-error {{inferred bounds for 't2' are unknown after assignment}}
  t3 = arg1;            // expected-error {{inferred bounds for 't3' are unknown after assignment}}

  t1 = arg2;
  t2 = arg2;
  t3 = arg2;            // expected-error {{declared bounds for 't3' are invalid after assignment}}

  t1 = arg3;
  t2 = arg3;
  t3 = arg3;

  // locals assigned from locals
  nt_array_ptr<int> c1 = 0;
  unsigned int u1, u2;
  int i1;
  nt_array_ptr<int> c2 : count(u1) = 0;
  nt_array_ptr<int> c3 : count(u1 * u2 + 2 * i1) = 0;
  int* _Nt_array c4  _Count(i1) = 0;

  c1 = c2;
  c2 = c1;            // expected-error {{it is not possible to prove that the inferred bounds of 'c2' imply the declared bounds of 'c2' after assignment}}
  c1 = c3;
  c1 = c4;            // expected-error {{it is not possible to prove that the inferred bounds of 'c1' imply the declared bounds of 'c1' after assignment}}

  // spot-check locals assigned from globals
  t1 = g13;
  t2 = g12;
  t3 = g11;            // expected-error {{inferred bounds for 't3' are unknown after assignment}}

  // spot-check globals assigned from locals
  g11 = t2;
  g12 = t3;
  g13 = t1;            // expected-error {{inferred bounds for 'g13' are unknown after assignment}}

  // expressions

  // nested assignments
  nt_array_ptr<int> t4 : count(1) = 0;
  t4 = (arg3 = t3);
  t4 = (t3 = arg3);
  t4 = (t2 = arg3);
  t4 = (t2 = t1);     // expected-error {{inferred bounds for 't2' are unknown after assignment}} \
                      // expected-error {{inferred bounds for 't4' are unknown after assignment}}

  // assignment through pointer
  *t1 = 1;            // expected-error {{expression has unknown bounds}}
  *t2 = 2;
  *t3 = 3;

  // read through a pointer
  int t6 = *t1;       // expected-error {{expression has unknown bounds}}
  t6 = *t2;
  t6 = *t3;

  // assignment via subcript
  t1[0] = 1;          // expected-error {{expression has unknown bounds}}
  t2[0] = 3;
  t3[0] = 4;

  // read via subscript

  int t7 = t1[0];     // expected-error {{expression has unknown bounds}}
  t7 = t2[0];
  t7 = t3[0];

  // pre-increment/post-increment
  ++(*t1);            // expected-error {{expression has unknown bounds}}
  ++(*t2);            // expected-error {{out-of-bounds memory access}}
  ++(*t3);

  --(*t1);            // expected-error {{expression has unknown bounds}}
  --(*t2);            // expected-error {{out-of-bounds memory access}}
  --(*t3);

  (*t1)++;            // expected-error {{expression has unknown bounds}}
  (*t2)++;            // expected-error {{out-of-bounds memory access}}
  (*t3)++;

  --(*t1);            // expected-error {{expression has unknown bounds}}
  --(*t2);            // expected-error {{out-of-bounds memory access}}
  --(*t3);

  // operations involving struct members
  struct S2 s = {0};
  s.f1 = t1;
  s.f1 = t2;
  s.f1 = t3;

  s.f2 = t1;          // expected-error {{inferred bounds for 's.f2' are unknown after assignment}}
  s.f2 = t2;
  s.f2 = t3;

  s.f3 = t1;          // expected-error {{inferred bounds for 's.f3' are unknown after assignment}}
  s.f3 = t2;          // expected-error {{declared bounds for 's.f3' are invalid after assignment}}
  s.f3 = t3;

  t1 = s.f1;
  t1 = s.f2;
  t1 = s.f3;

  t2 = s.f1;          // expected-error {{inferred bounds for 't2' are unknown after assignment}}
  t2 = s.f2;
  t2 = s.f3;

  t3 = s.f1;          // expected-error {{inferred bounds for 't3' are unknown after assignment}}
  t3 = s.f2;          // expected-error {{declared bounds for 't3' are invalid after assignment}}
  t3 = s.f3;

  nt_array_ptr<int> ntp = (int nt_checked[]) { 0, 1, 2, 3, 0 };
/* HACK:
  Taking the address of pointers with bounds is not allowed.  Disable this code and
  stop the verification checking by converting "expected-error" to "expected error".
  ptr<nt_array_ptr<int>> pntp = &ntp;
  *pntp = arg1;       // expected error {{expression has unknown bounds}}
  *pntp = arg2;
  *pntp = arg3;
  arg1 = *pntp;
  arg2 = *pntp;
  arg3 = *pntp;        // expected error {{declared bounds for arg3 are invalid after assignment}}
*/
}

extern void test_f1(int *p);
extern void test_f2(ptr<int> p);
extern void test_f3(array_ptr<int> p);
extern void test_f4(array_ptr<int> p : count(1));
extern void test_f5(array_ptr<int> p : count(len), int len);
extern void test_f6(array_ptr<int> p : count(0));

extern void check_call_args(int *arg1, ptr<int> arg2, array_ptr<int> arg3,
                            array_ptr<int> arg4 : count(1),
                            int* _Array arg5  _Count(arglen), int arglen,
                            array_ptr<int> arg6 : count(arglen_u), unsigned int arglen_u) {
  test_f1(arg1);
  test_f2(arg1);     // expected-error {{expression has unknown bounds}}
  test_f3(arg1);
  test_f4(arg1);     // expected-error {{argument has unknown bounds}}
  test_f5(arg1, 1);  // expected-error {{argument has unknown bounds}}

  test_f1(arg2);     // expected-error {{incompatible type}}
  test_f2(arg2);
  test_f3(arg2);
  test_f4(arg2);
  test_f5(arg2, 1);

  test_f1(arg3);     // expected-error {{incompatible type}}
  test_f2(arg3);     // expected-error {{expression has unknown bounds}}
  test_f3(arg3);
  test_f4(arg3);     // expected-error {{argument has unknown bounds}}
  test_f5(arg3, 1);  // expected-error {{argument has unknown bounds}}

  test_f1(arg4);     // expected-error {{incompatible type}}
  test_f2(arg4);
  test_f3(arg4);
  test_f4(arg4);
  test_f5(arg4, 1);

  test_f1(arg5);     // expected-error {{incompatible type}}
  test_f2(arg5);
  test_f3(arg5);
  test_f4(arg5);    // expected-error {{it is not possible to prove argument meets declared bounds for 1st parameter}}
  test_f5(arg5, 1); // expected-error {{it is not possible to prove argument meets declared bounds for 1st parameter}}
  test_f5(arg5, arglen);

  int count = arglen - 1;
  test_f5(arg5, ++count);  // expected-error {{increment expression not allowed}}
  test_f5(arg5, count++);  // expected-error {{increment expression not allowed}}

  test_f1(arg6);               // expected-error {{incompatible type}}
  test_f2(arg6);
  test_f3(arg6);
  test_f4(arg6);               // expected-error {{it is not possible to prove argument meets declared bounds for 1st parameter}}
  test_f5(arg6, arglen_u);     // expected-warning {{cannot prove argument meets declared bounds for 1st parameter}}
  test_f6(arg6);
}

extern void test_nullterm_f1(nt_array_ptr<char> p : bounds(unknown));
extern void test_nullterm_f2(nt_array_ptr<char> p);
extern void test_nullterm_f3(nt_array_ptr<char> p : count(1));
extern void test_nullterm_f4(nt_array_ptr<char> p : count(len), int len);

extern void check_nullterm_call_args(
  nt_array_ptr<char> arg1 : bounds(unknown),
  nt_array_ptr<char> arg2,
  nt_array_ptr<char> arg3 : count(1),
  nt_array_ptr<char> arg4 : count(arglen), int arglen,
  char* _Nt_array arg5  _Count(arglen_u), unsigned int arglen_u) {
  test_nullterm_f1(arg1);
  test_nullterm_f2(arg1);     // expected-error {{argument has unknown bounds}}
  test_nullterm_f3(arg1);     // expected-error {{argument has unknown bounds}}
  test_nullterm_f4(arg1, 1);  // expected-error {{argument has unknown bounds}}

  test_nullterm_f1(arg2);
  test_nullterm_f2(arg2);
  test_nullterm_f3(arg2);     // expected-error {{argument does not meet declared bounds for 1st parameter}}
  test_nullterm_f4(arg2, 1);  // expected-error {{argument does not meet declared bounds for 1st parameter}}

  test_nullterm_f1(arg3);
  test_nullterm_f2(arg3);
  test_nullterm_f3(arg3);
  test_nullterm_f4(arg3, 1);

  test_nullterm_f1(arg4);
  test_nullterm_f2(arg4);     // expected-error {{it is not possible to prove argument meets declared bounds for 1st parameter}}
  test_nullterm_f3(arg4);     // expected-error {{it is not possible to prove argument meets declared bounds for 1st parameter}}
  test_nullterm_f4(arg4, 1);  // expected-error {{it is not possible to prove argument meets declared bounds for 1st parameter}}
  test_nullterm_f4(arg4,  arglen);

  int count = arglen - 1;
  test_nullterm_f4(arg4, ++count);  // expected-error {{increment expression not allowed}}
  test_nullterm_f4(arg4, count++);  // expected-error {{increment expression not allowed}}

  test_nullterm_f1(arg5);
  test_nullterm_f2(arg5);
  test_nullterm_f3(arg5);     // expected-error {{it is not possible to prove argument meets declared bounds for 1st parameter}}
}

//
// Test checking of bounds declarations for arguments to functions with
// bounds-safe interfaces (BSI = bounds-safe interface)
//

extern void test_bsi_f1(int *p);
extern void test_bsi_f2(int *p : itype(ptr<int>));
extern void test_bsi_f3(int *p : itype(array_ptr<int>));
extern void test_bsi_f4(int *p : count(1));
extern void test_bsi_f5(int *p  _Count(len), int len);
extern void test_bsi_f6(int((*compar)(const int *, const int *)) :
  itype(_Ptr<int(_Ptr<const int>, _Ptr<const int>)>));
extern int test_cmp(_Ptr<const int> a, _Ptr<const int> b);

extern void check_call_bsi(int *arg1, ptr<int> arg2, array_ptr<int> arg3,
                           array_ptr<int> arg4 : count(1),
                             int* _Array arg5 : count(arglen), int arglen) {
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
  test_bsi_f2(arg3);     // expected-error {{argument has unknown bounds}}
  test_bsi_f3(arg3);
  test_bsi_f4(arg3);     // expected-error {{argument has unknown bounds}}
  test_bsi_f5(arg3, 1);  // expected-error {{argument has unknown bounds}}

  test_bsi_f1(arg4);     // expected-error {{incompatible type}}
  test_bsi_f2(arg4);
  test_bsi_f3(arg4);
  test_bsi_f4(arg4);
  test_bsi_f5(arg4, 1);

  test_bsi_f1(arg5);     // expected-error {{incompatible type}}
  test_bsi_f2(arg5);     // expected-error {{it is not possible to prove argument meets declared bounds for 1st parameter}}
  test_bsi_f3(arg5);
  test_bsi_f4(arg5);     // expected-error {{it is not possible to prove argument meets declared bounds for 1st parameter}}
  test_bsi_f5(arg5, 1);  // expected-error {{it is not possible to prove argument meets declared bounds for 1st parameter}}
  _Checked {
    test_bsi_f6(test_cmp);
  }

  int count = arglen - 1;
  test_bsi_f5(arg5, ++count);  // expected-error {{increment expression not allowed}}
  test_bsi_f5(arg5, count++);  // expected-error {{increment expression not allowed}}
}

//
// Test checking of bounds-safe interfaces involving null-terminated pointers.
//

// TODO: Checked C no way of declaring a bounds-safe interface type and a count for
// a null-terminated pointer at this time.  When there is a way, we need to add tests.

extern void test_nullterm_bsi_f1(int *p);
extern void test_nullterm_bsi_f2(int *p : itype(nt_array_ptr<int>));
extern void test_nullterm_bsi_f3(int **p : itype(ptr<nt_array_ptr<int>>));
extern void test_nullterm_bsi_f4(int((*compar)(const int *, const int *)) :
  itype(ptr<int(nt_array_ptr<const int>, nt_array_ptr<const int>)>));
extern int test_nullterm_cmp(nt_array_ptr<const int> a, nt_array_ptr<const int> b);

extern void check_nullterm_call_bsi(int *arg1 : itype(nt_array_ptr<int>),
                                    nt_array_ptr<int> arg2 : bounds(unknown),
                                    nt_array_ptr<int> arg3,
                                    int* _Nt_array arg4  _Count(1),
                                    nt_array_ptr<int> arg5 : count(arglen),
                                    int arglen,
                                    int **arg6,
                                    int **arg7 : itype(ptr<nt_array_ptr<int>>),
                                    ptr<nt_array_ptr<int>> arg8) {
  test_nullterm_bsi_f1(arg1);    // no checking expected when passing unchecked pointers.
  test_nullterm_bsi_f2(arg1);

  test_nullterm_bsi_f1(arg2);    // expected-error {{incompatible type}}
  test_nullterm_bsi_f2(arg2);    // expected-error {{argument has unknown bounds}}

  test_nullterm_bsi_f1(arg3);    // expected-error {{incompatible type}}
  test_nullterm_bsi_f2(arg3);

  test_nullterm_bsi_f1(arg4);    // expected-error {{incompatible type}}
  test_nullterm_bsi_f2(arg4);

  test_nullterm_bsi_f1(arg5);    // expected-error {{incompatible type}}
  test_nullterm_bsi_f2(arg5);    // expected-error {{it is not possible to prove argument meets declared bounds for 1st parameter}}

  test_nullterm_bsi_f3(arg6);    // no checking expected when passing unchecked pointers
  test_nullterm_bsi_f3(arg7);    // no checking expected when passing unchecked pointers
  test_nullterm_bsi_f3(arg8);

  _Checked{
    test_nullterm_bsi_f3(arg6);  // expected-error {{parameter used in a checked scope must have a checked type or a bounds-safe interface}}
    test_nullterm_bsi_f4(test_nullterm_cmp);
    arg1 = arg2;                 // expected-error {{inferred bounds for 'arg1' are unknown after assignment}}
    *arg7 = arg2;                // expected-error {{inferred bounds for '*arg7' are unknown after assignment}}
    *arg8 = arg2;                // expected-error {{inferred bounds for '*arg8' are unknown after assignment}}

    arg2 = arg1;
    arg2 = *arg7;
    arg2 = *arg8;

    arg1 = arg3;
    *arg7 = arg3;
    *arg8 = arg3;

    arg3 = arg1;
    arg3 = *arg7;
    arg3 = *arg8;

    arg4 = *arg7;               // expected-error {{declared bounds for 'arg4' are invalid after assignment}}
  }
}

char* _Nt_array nullterm_return1(void);
nt_array_ptr<char> nullterm_return2(void) : bounds(unknown);

void check_nullterm_return_use(void) {
  nt_array_ptr<char> p = nullterm_return1();
  p = nullterm_return2(); // expected-error {{inferred bounds for 'p' are unknown after assignment}}
}

char* _Nt_array check_nullterm_return1(void) {
  nt_array_ptr<char> p : bounds(unknown) = 0;
  return p; // expected-error {{return value has unknown bounds, bounds expected because the function 'check_nullterm_return1' has bounds}}
}
