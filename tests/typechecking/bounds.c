// Feature tests of typechecking new Checked C bounds declarations.
//
// The following lines are for the LLVM test harness:
//
// RUN: %clang_cc1 -Wno-check-bounds-decls -verify -verify-ignore-unexpected=note %s

#include <stdchecked.h>

static int A = 8;
static long long int B = 8;

struct S1 {
  int f;
};

struct S2 {
  float f;
};

union U1 {
  int f;
};

enum E1 {
  EnumVal1,
  EnumVal2
};

// Test expressions with standard signed and unsigned integers types as
// arguments to count and byte_count.

extern void count_exprs(void) {
  char c1 = 8;
  short c2 = 8;
  int c3 = 8;
  long int c4 = 8;
  long long int c5 = 8;

  _Bool c6 = 1;
  unsigned char c7 = 8;
  unsigned short c8 = 8;
  unsigned int c9 = 8;
  unsigned long int c10 = 8;
  unsigned long long int c11 = 8;

  array_ptr<int> t1 : count(c1) = 0;
  array_ptr<int> t2 : count(c2) = 0;
  array_ptr<int> t3 : count(c3) = 0;
  array_ptr<int> t4 : count(c4) = 0;
  array_ptr<int> t5 : count(c5) = 0;
  array_ptr<int> t6 : count(c6) = 0;
  array_ptr<int> t7 : count(c7) = 0;
  array_ptr<int> t8 : count(c8) = 0;
  array_ptr<int> t9 : count(c9) = 0;
  int* _Array t10  _Count(c10) = 0;
  int* _Array t11  _Count(c11) = 0;
  // Spot-check type checking of count expressions involving file-scoped
  // variables.
  array_ptr<int> t12 : count(A) = 0;
  array_ptr<int> t13 : count(B) = 0;
  array_ptr<int> t14 : count(12) = 0;
  // This will eventually fail static checking of bounds declarations, at which
  // point we'll need to add an expected error message.
  array_ptr<int> t15 : count(-5) =  0;

  array_ptr<int> t16 : byte_count(c1) = 0;
  array_ptr<int> t17 : byte_count(c2) = 0;
  array_ptr<int> t18 : byte_count(c3) = 0;
  array_ptr<int> t19 : byte_count(c4) = 0;
  array_ptr<int> t20 : byte_count(c5) = 0;
  array_ptr<int> t21 : byte_count(c6) = 0;
  array_ptr<int> t22 : byte_count(c7) = 0;
  array_ptr<int> t23 : byte_count(c8) = 0;
  array_ptr<int> t24 : byte_count(c9) = 0;
  int* _Array t25  _Byte_count(c10) = 0;
  int* _Array t26  _Byte_count(c11) = 0;
  // Spot-check type checking of byte_count expressions involving file-scoped
  // variables.
  array_ptr<int> t27 : byte_count(A) = 0;
  array_ptr<int> t28 : byte_count(B) = 0;
  array_ptr<int> t29 : byte_count(12) = 0;
  // This will eventually fail static checking of bounds declarations, at which
  // point we'll need to add an expected error message.
  array_ptr<int> t30 : byte_count(-8) = 0;

  // Spot check nt_array_ptr.  This should be treated the same as
  // array_ptr.

  nt_array_ptr<char> t50 : count(c1) = 0;
  nt_array_ptr<char> t51 : count(c2) = 0;
  nt_array_ptr<char> t52 : count(c3) = 0;
  nt_array_ptr<char> t53 : count(10) = 0;

  nt_array_ptr<char> t54 : byte_count(c7) = 0;
  nt_array_ptr<char> t55 : byte_count(c8) = 0;
  char* _Nt_array t56  _Byte_count(c9) = 0;
  char* _Nt_array t57  _Byte_count(A) = 0;

}

// Test expressions involving enum names and member bit fields that can be
// converted to integer types as arguments to count and byte_count.
extern void count_exprs_with_integral_operands(void) {
  enum E2 {
    c1 = 8,
    c2
  };

  struct S3 {
    int f : 5;
  } s;
  s.f = 8;

  array_ptr<int> t1 : count(c1) = 0;
  array_ptr<int> t2 : count(c2) = 0;
  array_ptr<int> t3 : count(s.f) = 0;
  int* _Nt_array t4  _Count(c2) = 0;
}

float globalFloat = 8;

extern void test_func(void) {}

// Test types that should not work as arguments for count and byte_count.
extern void invalid_count_exprs(void) {

  float c1 = 8.0;
  double c2 = 8.0;
  char *c3 = 0;
  double *c4 = 0;

  struct S1 c5 = {0};
  union U1 c6 = {0};

  double c7[10];

  struct S2 s = { 8.0 };

  void (*func_ptr)(void) = test_func;

#ifndef __STDC_NO_COMPLEX__
 float _Complex c8 = 8.0;
#endif

  array_ptr<int> t1 : count(c1) = 0;   // expected-error {{invalid argument type 'float' to count expression}}
  array_ptr<int> t2 : count(c2) = 0;   // expected-error {{invalid argument type}}
  array_ptr<int> t3 : count(c3) = 0;   // expected-error {{invalid argument type}}
  array_ptr<int> t4 : count(c4) = 0;   // expected-error {{invalid argument type}}
  array_ptr<int> t5 : count(c5) = 0;   // expected-error {{invalid argument type}}
  array_ptr<int> t6 : count(c6) = 0;   // expected-error {{invalid argument type}}
  array_ptr<int> t7 : count(c7) = 0;   // expected-error {{invalid argument type}}
  array_ptr<int> t8 : count(s.f) = 0;  // expected-error {{invalid argument type}}
  array_ptr<int> t9 : count(test_func) = 0; // expected-error {{invalid argument type}}
  array_ptr<int> t10 : count(func_ptr) = 0; // expected-error {{invalid argument type}}
  array_ptr<int> t11 : count("test") = 0; // expected-error {{invalid argument type}}
  int* _Array t12  _Count(5.0f) = 0; // expected-error {{invalid argument type}}
  int* _Array t13  _Count(globalFloat) = 0; // expected-error {{invalid argument type}}

#ifndef __STDC_NO_COMPLEX__
  array_ptr<int> t14 : count(c8) = 0;   // expected-error {{invalid argument type}}
#endif

  array_ptr<int> t15 : byte_count(c1) = 0;  // expected-error {{invalid argument type 'float' to byte_count expression}}
  array_ptr<int> t16 : byte_count(c2) = 0;  // expected-error {{invalid argument type}}
  array_ptr<int> t17 : byte_count(c3) = 0;  // expected-error {{invalid argument type}}
  array_ptr<int> t18 : byte_count(c4) = 0;  // expected-error {{invalid argument type}}
  array_ptr<int> t19 : byte_count(c5) = 0;  // expected-error {{invalid argument type}}
  array_ptr<int> t20 : byte_count(c6) = 0;  // expected-error {{invalid argument type}}
  array_ptr<int> t21 : byte_count(c7) = 0;  // expected-error {{invalid argument type}}
  array_ptr<int> t22 : byte_count(s.f) = 0; // expected-error {{invalid argument type}}
  array_ptr<int> t23 : byte_count(func_ptr) = 0;  // expected-error {{invalid argument type}}
  array_ptr<int> t24 : byte_count("test") = 0; // expected-error {{invalid argument type}}
  int* _Array t25  _Byte_count(5.0f) = 0; // expected-error {{invalid argument type}}
  int* _Array t26  _Byte_count(globalFloat) = 0; // expected-error {{invalid argument type}}

#ifndef __STDC_NO_COMPLEX__
  array_ptr<int> t27 : byte_count(c8) = 0;   // expected-error {{invalid argument type}}
#endif

  // spot check nt_array_ptr
  nt_array_ptr<int> t40 : count(c7) = 0;   // expected-error {{invalid argument type}}
}

extern void bounds_exprs(void) {
  int i[3] = { 0, 1, 0 };
   // check combinations of different kinds of pointers to the same
   // object type.

   array_ptr<int> array_ptr_lb = i, array_ptr_ub = i + 1;
   ptr<int> ptr_lb = i, ptr_ub = i + 1;
   int *unchecked_ptr_lb = i, *unchecked_ptr_ub = i + 1;
   // unsafe cast
   nt_array_ptr<int> nt_array_ptr_lb = (nt_array_ptr<int>) i,
                     nt_array_ptr_ub = (nt_array_ptr<int>) i + 1;

   array_ptr<int> t1 : bounds(array_ptr_lb, array_ptr_ub) = i;
   array_ptr<int> t2 : bounds(ptr_lb, array_ptr_ub) = i;
   array_ptr<int> t3 : bounds(array_ptr_lb, ptr_ub) = i;
   array_ptr<int> t4 : bounds(unchecked_ptr_lb, array_ptr_ub) = i;
   array_ptr<int> t5 : bounds(array_ptr_lb, unchecked_ptr_ub) = i;
   array_ptr<int> t6 : bounds(ptr_lb, ptr_ub) = i;
   array_ptr<int> t7 : bounds(unchecked_ptr_lb, ptr_ub) = i;
   int* _Array t8  _Bounds(ptr_lb, unchecked_ptr_ub) = i;
   int* _Array t9  _Bounds(unchecked_ptr_lb, unchecked_ptr_ub) = i;

   // spot check mixed uses of nt_array_ptr and other pointer types.
   array_ptr<int> t9a : bounds(nt_array_ptr_lb, ptr_ub) = i;
   array_ptr<int> t9b : bounds(array_ptr_lb, nt_array_ptr_ub) = i;
   array_ptr<int> t9c : bounds(ptr_lb, array_ptr_ub) = i;

   // use an array-typed value as the lower bound.  This value
   // should be converted implicitly to be a pointer type.

   array_ptr<int> t10 : bounds(i, i + 1) = i;
   array_ptr<int> t11 : bounds(i, array_ptr_ub) = i;
   int* _Array t11a  _Bounds(i, nt_array_ptr_ub) = i;
   int* _Array t13  _Bounds(i, ptr_ub) = i;

   array_ptr<void> void_array_ptr_lb = i, void_array_ptr_ub = i + 1;
   ptr<void> void_ptr_lb = i, void_ptr_ub = i + 1;
   void *void_unchecked_ptr_lb = i, *void_unchecked_ptr_ub = i + 1;

   // check combinations of differents kinds of pointers to void
   array_ptr<int> t21 : bounds(void_array_ptr_lb, void_array_ptr_ub) = i;
   array_ptr<int> t22 : bounds(void_ptr_lb, void_array_ptr_ub) = i;
   array_ptr<int> t23 : bounds(void_array_ptr_lb, void_ptr_ub) = i;
   array_ptr<int> t24 : bounds(void_unchecked_ptr_lb, void_array_ptr_ub) = i;
   array_ptr<int> t25 : bounds(void_array_ptr_lb, void_unchecked_ptr_ub) = i;
   array_ptr<int> t26 : bounds(void_ptr_lb, void_ptr_ub) = i;
   array_ptr<int> t27 : bounds(void_unchecked_ptr_lb, void_ptr_ub) = i;
   array_ptr<int> t28 : bounds(void_ptr_lb, void_unchecked_ptr_ub) = i;
   int* _Array t29  _Bounds(void_unchecked_ptr_lb, void_unchecked_ptr_ub) = i;

   // check combinations of pointers to void and pointers to non-void types

   array_ptr<int> t42 : bounds(array_ptr_lb, void_array_ptr_ub) = i;
   array_ptr<int> t43 : bounds(ptr_lb, void_array_ptr_ub) = i;
   array_ptr<int> t44 : bounds(void_ptr_lb, array_ptr_ub) = i;
   array_ptr<int> t45 : bounds(void_array_ptr_lb, ptr_ub) = i;
   array_ptr<int> t46 : bounds(array_ptr_lb, void_ptr_ub) = i;
   array_ptr<int> t47 : bounds(unchecked_ptr_lb, void_array_ptr_ub) = i;
   array_ptr<int> t48 : bounds(void_unchecked_ptr_lb, array_ptr_ub) = i;
   int* _Array t49  _Bounds(void_array_ptr_lb, unchecked_ptr_ub) = i;
   int* _Array t50  _Bounds(array_ptr_lb, void_unchecked_ptr_ub) = i;

   array_ptr<int> t51 : bounds(void_ptr_lb, ptr_ub) = i;
   array_ptr<int> t52 : bounds(ptr_lb, void_ptr_ub) = i;
   array_ptr<int> t53 : bounds(unchecked_ptr_lb, void_ptr_ub) = i;
   array_ptr<int> t54 : bounds(void_unchecked_ptr_lb, ptr_ub) = i;
   array_ptr<int> t55 : bounds(void_ptr_lb, unchecked_ptr_ub) = i;
   array_ptr<int> t56 : bounds(ptr_lb, void_unchecked_ptr_ub) = i;

   // spot check mixed uses of pointers to void and null-terminated pointers
   array_ptr<int> t60 : bounds(void_array_ptr_lb, nt_array_ptr_ub) = i;
   array_ptr<int> t61 : bounds(nt_array_ptr_lb, void_ptr_ub) = i;

   // spot check cases where the value being declared has a different pointer type
   // than the bounds.
   array_ptr<char> t71 : bounds(array_ptr_lb, array_ptr_ub) = (array_ptr<char>) i;
   array_ptr<char> t72 : bounds(ptr_lb, array_ptr_ub) = (array_ptr<char>) i;
   array_ptr<char> t73 : bounds(unchecked_ptr_lb, ptr_ub) = (array_ptr<char>) i;
   array_ptr<char> t75 : bounds(void_array_ptr_lb, array_ptr_ub) = (array_ptr<char>) i;
   array_ptr<char> t76 : bounds(void_unchecked_ptr_lb, array_ptr_ub) = (array_ptr<char>) i;
   char* _Array t77  _Bounds(array_ptr_lb, void_unchecked_ptr_ub) = (char* _Array) i;
   char* _Array t77a  _Bounds(nt_array_ptr_lb, void_unchecked_ptr_ub) = (char* _Array) i;

   // use an array-typed value as the lower bound.  This should be converted
   // implicitly to be a pointer type.
   array_ptr<char> t78 : bounds(i, array_ptr_ub) = (array_ptr<char>) i;
   array_ptr<char> t79 : bounds(i, ptr_ub) = (array_ptr<char>) i;
   array_ptr<char> t80 : bounds(i, nt_array_ptr_ub) = (array_ptr<char>) i;

   // spot check that typechecking looks through typedefs
   typedef int* _Array int_array_ptr;
   typedef ptr<int> int_ptr;
   typedef int *int_unchecked_ptr;
   typedef nt_array_ptr<int> int_nt_array_ptr;

   int_array_ptr typedef_array_ptr_lb = i, typedef_array_ptr_ub = i + 1;
   int_ptr typedef_ptr_lb = i, typedef_ptr_ub = i + 1;
   int_unchecked_ptr typedef_unchecked_ptr_lb = i, typedef_unchecked_ptr_ub = i + 1;
   int_nt_array_ptr typedef_nt_array_ptr_lb = (int_nt_array_ptr)i,
                    typedef_nt_array_ptr_ub = (int_nt_array_ptr)(i + 1);

   array_ptr<int> t91 : bounds(typedef_array_ptr_lb, array_ptr_ub) = i;
   array_ptr<int> t92 : bounds(ptr_lb, typedef_array_ptr_ub) = i;
   array_ptr<int> t93 : bounds(array_ptr_lb, typedef_ptr_ub) = i;
   array_ptr<int> t94 : bounds(typedef_unchecked_ptr_lb, array_ptr_ub) = i;
   array_ptr<int> t95 : bounds(typedef_array_ptr_lb, unchecked_ptr_ub) = i;
   array_ptr<int> t96 : bounds(typedef_ptr_lb, ptr_ub) = i;
   array_ptr<int> t97 : bounds(unchecked_ptr_lb, typedef_ptr_ub) = i;
   array_ptr<int> t98 : bounds(ptr_lb, typedef_unchecked_ptr_ub) = i;
   array_ptr<int> t99 : bounds(typedef_unchecked_ptr_lb, unchecked_ptr_ub) = i;
   array_ptr<int> t100 : bounds(typedef_nt_array_ptr_lb, unchecked_ptr_ub) = i;
   int* _Array t101  _Bounds(typedef_array_ptr_lb, typedef_nt_array_ptr_ub) = i;

   // check that type qualifiers are discarded when comparing pointer types
   // in bounds expressions

   // permutations of array_ptr and const
   array_ptr<const int> array_ptr_const_lb = i;
   const array_ptr<int> const_array_ptr_lb = i;
   const array_ptr<const int> const_array_ptr_const_lb = i;
   array_ptr<const int> array_ptr_const_ub = i + 1;
   const array_ptr<int> const_array_ptr_ub = i + 1;
   const array_ptr<const int> const_array_ptr_const_ub = i + 1;
   const nt_array_ptr<int> const_nt_array_ptr_lb = (const nt_array_ptr<int>) i;
   const nt_array_ptr<int> const_nt_array_ptr_ub = (const nt_array_ptr<int>) i + 1;

   // permutations of ptr and const
   ptr<int const> ptr_const_lb = i;
   const ptr<int> const_ptr_lb = i;
   const ptr<const int> const_ptr_const_lb = i;
   ptr<int const> ptr_const_ub = i + 1;
   const ptr<int> const_ptr_ub = i + 1;
   const ptr<const int> const_ptr_const_ub = i + 1;

   // permutations of unchecked pointers and const
   int *const const_unchecked_ptr_lb = i;
   const int *unchecked_ptr_const_lb = i;
   const int *const const_unchecked_ptr_const_lb = i;
   int *const const_unchecked_ptr_ub = i + 1;
   const int *unchecked_ptr_const_ub = i + 1;
   const int *const const_unchecked_ptr_const_ub = i + 1;

   int* _Array t121  _Bounds(array_ptr_const_lb, array_ptr_ub) = i;
   array_ptr<int> t122 : bounds(const_array_ptr_lb, array_ptr_ub) = i;
   array_ptr<int> t123 : bounds(const_array_ptr_const_lb, array_ptr_ub) = i;

   array_ptr<int> t124 : bounds(array_ptr_lb, array_ptr_const_ub) = i;
   array_ptr<int> t125 : bounds(array_ptr_lb, const_array_ptr_ub) = i;
   array_ptr<int> t126 : bounds(array_ptr_lb, const_array_ptr_const_ub) = i;

   array_ptr<int> t127 : bounds(const_array_ptr_lb, array_ptr_const_ub) = i;
   array_ptr<int> t128 : bounds(array_ptr_const_lb, const_array_ptr_ub) = i;
   int* _Array t129  _Bounds(const_array_ptr_const_lb, const_array_ptr_const_ub) = i;

   array_ptr<int> t130 : bounds(ptr_lb, array_ptr_ub) = i;
   array_ptr<int> t131 : bounds(array_ptr_lb, ptr_ub) = i;
   array_ptr<int> t132 : bounds(unchecked_ptr_lb, array_ptr_ub) = i;
   array_ptr<int> t133 : bounds(array_ptr_lb, unchecked_ptr_ub) = i;
   array_ptr<int> t134 : bounds(ptr_lb, ptr_ub) = i;
   array_ptr<int> t135 : bounds(unchecked_ptr_lb, ptr_ub) = i;
   array_ptr<int> t136 : bounds(ptr_lb, unchecked_ptr_ub) = i;
   array_ptr<int> t137 : bounds(unchecked_ptr_lb, unchecked_ptr_ub) = i;

   // spot check const nt_array_ptr
   array_ptr<int> t140 : bounds(nt_array_ptr_lb, const_nt_array_ptr_ub) = i;
   int* _Array t141  _Bounds(const_nt_array_ptr_lb, nt_array_ptr_ub) = i;
 }

 extern void invalid_bounds_exprs(void) {
   // test types that should not work as arguments to bounds expressions
   char c1 = 8;
   short c2 = 8;
   int c3 = 8;
   long int c4 = 8;
   long long int c5 = 8;

   _Bool c6 = 1;
   unsigned char c7 = 8;
   unsigned short c8 = 8;
   unsigned int c9 = 8;
   unsigned long int c10 = 8;
   unsigned long long int c11 = 8;

   float c12 = 8.0;
   double c13 = 8.0;
   struct S1 c14 = { 0 };
   union U1 c15 = { 0 };

   void(*func_ptr)(void) = test_func;
   int *single_indir = 0;
   int **double_indir = 0;

#ifndef __STDC_NO_COMPLEX__
   float _Complex c17 = 8.0;
#endif

   array_ptr<int> t1 : bounds(c1, c1) = 0; // expected-error 2 {{expected expression with pointer type}}
   array_ptr<int> t2 : bounds(c2, c2) = 0; // expected-error 2 {{expected expression with pointer type}}
   array_ptr<int> t3 : bounds(c3, c3) = 0; // expected-error 2 {{expected expression with pointer type}}
   array_ptr<int> t4 : bounds(c4, c4) = 0; // expected-error 2 {{expected expression with pointer type}}
   array_ptr<int> t5 : bounds(c5, c5) = 0; // expected-error 2 {{expected expression with pointer type}}
   array_ptr<int> t6 : bounds(c6, c6) = 0; // expected-error 2 {{expected expression with pointer type}}
   array_ptr<int> t7 : bounds(c7, c7) = 0; // expected-error 2 {{expected expression with pointer type}}
   array_ptr<int> t8 : bounds(c8, c8) = 0; // expected-error 2 {{expected expression with pointer type}}
   array_ptr<int> t9 : bounds(c9, c9) = 0; // expected-error 2 {{expected expression with pointer type}}
   array_ptr<int> t10 : bounds(c10, c10) = 0; // expected-error 2 {{expected expression with pointer type}}
   array_ptr<int> t11 : bounds(c11, c11) = 0; // expected-error 2 {{expected expression with pointer type}}
   array_ptr<int> t12 : bounds(c12, c12) = 0; // expected-error 2 {{expected expression with pointer type}}
   array_ptr<int> t13 : bounds(c13, c13) = 0; // expected-error 2 {{expected expression with pointer type}}
   array_ptr<int> t14 : bounds(c14, c14) = 0; // expected-error 2 {{expected expression with pointer type}}
   array_ptr<int> t15 : bounds(c15, c15) = 0; // expected-error 2 {{expected expression with pointer type}}
   int* _Array t16  _Bounds(test_func, test_func) = 0; // expected-error 2 {{invalid argument type 'void (*)(void)' to bounds expression}}

   // have values with different levels of indirection
   array_ptr<int> t17 : bounds(double_indir, c3) = 0; // expected-error {{expected expression with pointer type}}
   array_ptr<int> t18 : bounds(double_indir, single_indir) = 0; // expected-error {{pointer type mismatch}}

   // test mismatched bounds expression types
   int i[2];
   array_ptr<int> int_array_ptr_lb = i, int_array_ptr_ub = i + 1;
   ptr<int> int_ptr_lb = i, int_ptr_ub = i + 1;
   int *int_unchecked_ptr_lb = i, *int_unchecked_ptr_ub = i + 1;

   array_ptr<char> char_array_ptr_lb = (array_ptr<char>) i, char_array_ptr_ub = (array_ptr<char>) i + 1;
   ptr<char> char_ptr_lb = (ptr<char>) i, char_ptr_ub = (ptr<char>)  (i + 1);
   char *char_unchecked_ptr_lb = (char *) i, *char_unchecked_ptr_ub = (char *) i + 1;

   array_ptr<int> t20 : bounds(int_array_ptr_lb, char_array_ptr_ub) = i;     // expected-error {{pointer type mismatch}}
   array_ptr<int> t21 : bounds(int_ptr_lb, char_array_ptr_ub) = i;           // expected-error {{pointer type mismatch}}
   array_ptr<int> t22 : bounds(int_unchecked_ptr_lb, char_array_ptr_ub) = i; // expected-error {{pointer type mismatch}}
   array_ptr<char> t23 : bounds(char_array_ptr_lb, int_array_ptr_ub) = (array_ptr<char>) i;     // expected-error {{pointer type mismatch}}
   array_ptr<char> t24 : bounds(char_ptr_lb, int_array_ptr_ub) = (array_ptr<char>) i;           // expected-error {{pointer type mismatch}}
   array_ptr<char> t25 : bounds(char_unchecked_ptr_lb, int_array_ptr_ub) = (array_ptr<char>) i; // expected-error {{pointer type mismatch}}

   array_ptr<int> t30 : bounds(int_array_ptr_lb, char_ptr_ub) = i;     // expected-error {{pointer type mismatch}}
   array_ptr<int> t31 : bounds(int_ptr_lb, char_ptr_ub) = i;           // expected-error {{pointer type mismatch}}
   array_ptr<int> t32 : bounds(int_unchecked_ptr_lb, char_ptr_ub) = i; // expected-error {{pointer type mismatch}}
   array_ptr<char> t33 : bounds(char_array_ptr_lb, int_ptr_ub) = (array_ptr<char>) i;     // expected-error {{pointer type mismatch}}
   array_ptr<char> t34 : bounds(char_ptr_lb, int_ptr_ub) = (array_ptr<char>) i;           // expected-error {{pointer type mismatch}}
   char* _Array t35  _Bounds(char_unchecked_ptr_lb, int_ptr_ub) = (char* _Array) i; // expected-error {{pointer type mismatch}}

   // spot check nt_array_ptr with invalid bounds expression
   nt_array_ptr<char> t40 : bounds(int_array_ptr_lb, char_array_ptr_ub) = 0;     // expected-error {{pointer type mismatch}}
}

//
// Test type requirements for bounds declarations.   There are various
// requirements for the types of variables with bounds declaration.
//
// We need to test the cross-product of (variable or member
//  declaration, scope for variables (global or local), the kind of the bounds
// expression, and the type for the variable).
//
// The tests follow a specific pattern: there is a set of tests for global
// variables that covers most of the different requirements.   The tests
// for the other cases (local variables, parameter variables, member
// declarations, and return bounds declarations) are specialized clones
//  of the cases for global variables.  The syntax is altered where necessary,
// prefixes for variables and members are renamed to avoid overlapping names
// and a small number of tests are added/removed.  The numbering for variables
//  and members should be kept the same across the different cases.
//
// The tests themselves are ordered by the kind of bounds expression and then
// grouped by kind of type within that.

//
// Test global variable declarations
//

//
// Valid bounds declarations for global variables
//

// count
array_ptr<int> g1 : count(5) = 0;
int *g2 : count(5) = 0;
int g3 checked[5] : count(5);
int g3a[5] : count(5);
int g3b nt_checked[5]: count(4);

// byte_count
array_ptr<int> g4 : byte_count(5 * sizeof(int)) = 0;
array_ptr<void> g5 : byte_count(5 * sizeof(int)) = 0;
int *g6 : byte_count(5 * sizeof(int)) = 0;
int g7 checked[5] : byte_count(5 * sizeof(int));
unsigned int g8 checked[5] : byte_count(5 * sizeof(int));
unsigned int g8a nt_checked[5] : byte_count(4 * sizeof(int));
unsigned int g9[5] : byte_count(5 * sizeof(int));

// bounds
array_ptr<int> g10 : bounds(g10, g10 + 5) = 0;
// The use of 'g3' is intentional - pointer arithmetic is not
// allowed on pointers to void types
array_ptr<void> g11 : bounds(g3, g3 + 5);
int *g12 : bounds(g12, g12 + 5) = 0;
int g13 checked[5] : bounds(g13, g13 + 5);
unsigned int g14 checked[5] : bounds(g14, g14 + 5);
unsigned int g14a nt_checked[5] : bounds(g14a, g14a + 5);
int g15[5] : bounds(g15, g15 + 5);
unsigned int g16[5] : bounds(g16, g16 + 5);

//
// Valid bounds declarations for integer-typed global variables.
//

int s1 checked[16];



//
// Test invalid bounds declarations for global variables
//

// count
char g41 : count(5) = 0;           // expected-error {{expected 'g41' to have a pointer or array type}}
_Bool g42 : count(5) = 0;          // expected-error {{expected 'g42' to have a pointer or array type}}
short int g43 : count(5) = 0;      // expected-error {{expected 'g43' to have a pointer or array type}}
int g44 : count(5) = 0;            // expected-error {{expected 'g44' to have a pointer or array type}}
long int g45 : count(5) = 0;       // expected-error {{expected 'g45' to have a pointer or array type}}
unsigned short int g46 : count(5) = 0; // expected-error {{expected 'g46' to have a pointer or array type}}
unsigned int g47 : count(5) = 0;       // expected-error {{expected 'g47' to have a pointer or array type}}
unsigned long int g48 : count(5) = 0;  // expected-error {{expected 'g48' to have a pointer or array type}}

float g49 : count(5) = 0;           // expected-error {{expected 'g49' to have a pointer or array type}}
double g50 : count(5) = 0;          // expected-error {{expected 'g50' to have a pointer or array type}}
struct S1 g51 : count(5) = { 0 };   // expected-error {{expected 'g51' to have a pointer or array type}}
union U1 g52 : count(5) = { 0 };    // expected-error {{expected 'g52' to have a pointer or array type}}
enum E1 g53 : count(5) = EnumVal1;  // expected-error {{expected 'g53' to have a pointer or array type}}
ptr<int> g54: count(1) = 0;         // expected-error {{bounds declaration not allowed because 'g54' has a _Ptr type}}
void* _Array g55  _Count(1) = 0; // expected-error {{expected 'g55' to have a non-void pointer type}}
void((*g56)(void))  _Count(1);      // expected-error {{bounds declaration not allowed because 'g56' has a function pointer type}}

// byte_count
float g60 : byte_count(8);             // expected-error {{expected 'g60' to have a pointer, array, or integer type}}
double g61 : byte_count(8);            // expected-error {{expected 'g61' to have a pointer, array, or integer type}}
struct S1 g62 : byte_count(8) = { 0 }; // expected-error {{expected 'g62' to have a pointer, array, or integer type}}
union U1 g63 : byte_count(8) = { 0 };  // expected-error {{expected 'g63' to have a pointer, array, or integer type}}
ptr<int> g64 : byte_count(8) = 0;      // expected-error {{bounds declaration not allowed because 'g64' has a _Ptr type}}
void((*g65)(void)) : byte_count(1);    // expected-error {{bounds declaration not allowed because 'g65' has a function pointer type}}

// bounds
float g70 : bounds(s1, s1 + 1);             // expected-error {{expected 'g70' to have a pointer, array, or integer type}}
double g71 : bounds(s1, s1 + 1);            // expected-error {{expected 'g71' to have a pointer, array, or integer type}}
struct S1 g72 : bounds(s1, s1 + 1) = { 0 }; // expected-error {{expected 'g72' to have a pointer, array, or integer type}}
union U1 g73 : bounds(s1, s1 + 1) = { 0 };  // expected-error {{expected 'g73' to have a pointer, array, or integer type}}
int* _Single g74  _Bounds(s1, s1 + 1) = 0;      // expected-error {{bounds declaration not allowed because 'g74' has a _Ptr type}}
void((*g75)(void))  _Bounds(s1, s1 + 1);    // expected-error {{bounds declaration not allowed because 'g75' has a function pointer type}}

//
// Test bounds declarations for local variables.
//

void local_var_bounds_decl(void)
{
  // count
  array_ptr<int> t1 : count(5) = 0;
  int t3 checked[5] : count(5);
  int t3a nt_checked[5] : count(4);

  // byte_count
  array_ptr<int> t4 : byte_count(5 * sizeof(int)) = 0;
  array_ptr<void> t5 : byte_count(5 * sizeof(int)) = 0;
  int t7 checked[5] : byte_count(5 * sizeof(int));
  unsigned int t8 checked[5] : byte_count(5 * sizeof(int));
  unsigned int t8a nt_checked[5] : byte_count(4 * sizeof(int));

  // bounds
  array_ptr<int> t10 : bounds(t10, t10 + 5) = 0;
  // The use of 't3' is intentional - pointer arithmetic is not
  // allowed on pointers to void types.
  array_ptr<void> t11 : bounds(t3, t3 + 5) = t3;
  int t13 checked[5] : bounds(t13, t13 + 5);
  unsigned int t14 checked[5] : bounds(t14, t14 + 5);
  unsigned int t14a nt_checked[5]  _Bounds(t14, t14 + 4);
}

void int_local_var_bounds_decl(void) {
  // bounds declarations are allowed for integer variables to support
  // casting of pointers to integers and back.  We usually expect this
  // to happen within expressions, but to allow uniform use of language
  // features, we allow bounds on integer-typed variables.
  int a1 checked[5];

  // byte_count
  short int t20 : byte_count(5 * sizeof(int)) = (short int)a1; // expected-warning {{cast to smaller integer type 'short' from '_Array_ptr<int>'}}
  enum E1 t24 : byte_count(8) = EnumVal1;
}

void invalid_local_var_bounds_decl(void)
{
  int arr[16];
  char t41 : count(5) = 0;           // expected-error {{expected 't41' to have a pointer or array type}}
  _Bool t42 : count(5) = 0;          // expected-error {{expected 't42' to have a pointer or array type}}
  short int t43 : count(5) = 0;      // expected-error {{expected 't43' to have a pointer or array type}}
  int t44 : count(5) = 0;            // expected-error {{expected 't44' to have a pointer or array type}}
  long int t45 : count(5) = 0;       // expected-error {{expected 't45' to have a pointer or array type}}
  unsigned short int t46 : count(5) = 0; // expected-error {{expected 't46' to have a pointer or array type}}
  unsigned int t47 : count(5) = 0;       // expected-error {{expected 't47' to have a pointer or array type}}
  unsigned long int t48 : count(5) = 0;  // expected-error {{expected 't48' to have a pointer or array type}}

  float t49 : count(5) = 0;          // expected-error {{expected 't49' to have a pointer or array type}}
  double t50 : count(5) = 0;         // expected-error {{expected 't50' to have a pointer or array type}}
  struct S1 t51 : count(5) = { 0 };  // expected-error {{expected 't51' to have a pointer or array type}}
  union U1 t52 : count(5) = { 0 };   // expected-error {{expected 't52' to have a pointer or array type}}
  enum E1 t53 : count(5) = EnumVal1; // expected-error {{expected 't53' to have a pointer or array type}}
  ptr<int> t54 : count(1) = 0;       // expected-error {{bounds declaration not allowed because 't54' has a _Ptr type}}
  void* _Array t55  _Count(1) = 0; // expected-error {{expected 't55' to have a non-void pointer type}}
  array_ptr<void (void)> t56  _Count(1);  // expected-error {{declared as _Array_ptr to function of type 'void (void)'; use _Ptr to function instead}}

  int *t57 : count(1) = 0;          // expected-error {{bounds declaration not allowed for local variable with unchecked pointer type}}
  int t58[5] : count(5);            // expected-error {{bounds declaration not allowed for local variable with unchecked array type}}

  // byte_count
  float t60 : byte_count(8);                  // expected-error {{expected 't60' to have a pointer, array, or integer type}}
  double t61 : byte_count(8);                 // expected-error {{expected 't61' to have a pointer, array, or integer type}}
  struct S1 t62 : byte_count(8) = { 0 };      // expected-error {{expected 't62' to have a pointer, array, or integer type}}
  union U1 t63 : byte_count(8) = { 0 };       // expected-error {{expected 't63' to have a pointer, array, or integer type}}
  ptr<int> t64 : byte_count(sizeof(int)) = 0; // expected-error {{bounds declaration not allowed because 't64' has a _Ptr type}}
  array_ptr<void (void)> t65 : byte_count(1); // expected-error {{declared as _Array_ptr to function of type 'void (void)'; use _Ptr to function instead}}

  int *t67  _Byte_count(sizeof(int)) = 0;     // expected-error {{bounds declaration not allowed for local variable with unchecked pointer type}}
  int t68[5]  _Byte_count(5 * sizeof(int));   // expected-error {{bounds declaration not allowed for local variable with unchecked array type}}

  // bounds
  float t70 : bounds(arr, arr + 1);             // expected-error {{expected 't70' to have a pointer, array, or integer type}}
  double t71 : bounds(arr, arr + 1);            // expected-error {{expected 't71' to have a pointer, array, or integer type}}
  struct S1 t72 : bounds(arr, arr + 1) = { 0 }; // expected-error {{expected 't72' to have a pointer, array, or integer type}}
  union U1 t73 : bounds(arr, arr + 1) = { 0 };  // expected-error {{expected 't73' to have a pointer, array, or integer type}}
  ptr<int> t74 : bounds(arr, arr + 1) = 0;      // expected-error {{bounds declaration not allowed because 't74' has a _Ptr type}}
  array_ptr<void (void)> t75 : bounds(arr, arr + 1);  // expected-error {{declared as _Array_ptr to function of type 'void (void)'; use _Ptr to function instead}}

  int *t78 : bounds(arr, arr + 1) = 0;          // expected-error {{bounds declaration not allowed for local variable with unchecked pointer type}}
  int t79[5]  _Bounds(arr, arr + 1);            // expected-error {{bounds declaration not allowed for local variable with unchecked array type}}
}

//
// Test parameter variable bounds declarations.
//

void param_var_bounds_decl(
  // count
  array_ptr<int> t1 : count(5),
  nt_array_ptr<int> t1a : count(5),
  int *t2 : count(5),
  int t3 checked[5] : count(5),
  int t3a[5] : count(5),
  int t3b nt_checked[5] : count(4),
  int t3c nt_checked[] : count(0),

  // byte_count
  array_ptr<int> t4 : byte_count(5 * sizeof(int)),
  array_ptr<void> t5 : byte_count(5 * sizeof(int)),
  int *t6 : byte_count(5 * sizeof(int)),
  int t7 checked[5]  _Byte_count(5 * sizeof(int)),
  unsigned int t8 checked[5]  _Byte_count(5 * sizeof(int)),
  unsigned int t9[5] : byte_count(5 * sizeof(int)),

  // bounds
  array_ptr<int> t10 : bounds(t10, t10 + 5),
  // The use of 't3' is intentional - pointer arithmetic is not
  // allowed on pointers to void types.
  array_ptr<void> t11 : bounds(t3, t3 + 5),
  int *t12 : bounds(t12, t12 + 5),
  int t13 checked[5] : bounds(t13, t13 + 5),
  unsigned int t14 checked[5] : bounds(t14, t14 + 5),
  int t15[5] : bounds(t15, t15 + 5),
  unsigned int t16[5] : bounds(t16, t16 + 5))
{
}

extern int garr[10];

extern void anonymous_param_var_bounds_decl(
  // count
  array_ptr<int> : count(5),
  nt_array_ptr<int> : count(5),
  int * : count(5),
  int checked[5] : count(5),
  int[5] : count(5),
  int nt_checked[6] : count(5),
  int nt_checked[] : count(0),

  // byte_count
  array_ptr<int> : byte_count(5 * sizeof(int)),
  array_ptr<void> : byte_count(5 * sizeof(int)),
  int * : byte_count(5 * sizeof(int)),
  int checked[5] : byte_count(5 * sizeof(int)),
  unsigned int checked[5] : byte_count(5 * sizeof(int)),
  unsigned int [5] : byte_count(5 * sizeof(int)),

  // bounds
  array_ptr<int> : bounds(garr, garr + 5),
  array_ptr<void> : bounds(garr, garr + 5),
  int * : bounds(garr, garr + 5),
  int t13 checked[5] : bounds(garr, garr + 5),
  int t15[5] : bounds(garr, garr + 5));

void int_param_var_bounds_decl(
  int s1 checked[16],
  int t21 : byte_count(5 * sizeof(int)),
  long int t22 : byte_count(5 * sizeof(int)),
  unsigned long int t23 : byte_count(5 * sizeof(int)),
  enum E1 t24 : byte_count(8),

  // bounds
  int t25 : bounds(s1, s1 + 5),
  long int t26 : bounds(s1, s1 + 5),
  unsigned long int t27 : bounds(s1, s1 + 5),
  enum E1 t28 : bounds(s1, s1 + 5))
{
}

void invalid_param_var_bounds_decl(
  // count
  int s1 checked[16],
  char t41 : count(5),           // expected-error {{expected 't41' to have a pointer or array type}}
  _Bool t42 : count(5),          // expected-error {{expected 't42' to have a pointer or array type}}
  short int t43 : count(5),      // expected-error {{expected 't43' to have a pointer or array type}}
  int t44 : count(5),            // expected-error {{expected 't44' to have a pointer or array type}}
  long int t45 : count(5),       // expected-error {{expected 't45' to have a pointer or array type}}
  unsigned short int t46 : count(5), // expected-error {{expected 't46' to have a pointer or array type}}
  unsigned int t47 : count(5),       // expected-error {{expected 't47' to have a pointer or array type}}
  unsigned long int t48 : count(5),  // expected-error {{expected 't48' to have a pointer or array type}}

  float t49 : count(5),          // expected-error {{expected 't49' to have a pointer or array type}}
  double t50 : count(5),         // expected-error {{expected 't50' to have a pointer or array type}}
  struct S1 t51 : count(5),      // expected-error {{expected 't51' to have a pointer or array type}}
  union U1 t52 : count(5),       // expected-error {{expected 't52' to have a pointer or array type}}
  enum E1 t53 : count(5),        // expected-error {{expected 't53' to have a pointer or array type}}
  ptr<int> t54: count(1),        // expected-error {{bounds declaration not allowed because 't54' has a _Ptr type}}
  array_ptr<void> t55 : count(1), // expected-error {{expected 't55' to have a non-void pointer type}}
  void((*t56)(void)) : count(1),  // expected-error {{bounds declaration not allowed because 't56' has a function pointer type}}

  // byte_count
  float t60 : byte_count(8),         // expected-error {{expected 't60' to have a pointer, array, or integer type}}
  double t61 : byte_count(8),        // expected-error {{expected 't61' to have a pointer, array, or integer type}}
  struct S1 t62 : byte_count(8),     // expected-error {{expected 't62' to have a pointer, array, or integer type}}
  union U1 t63 : byte_count(8),      // expected-error {{expected 't63' to have a pointer, array, or integer type}}
  int* _Single t64  _Byte_count(8),      // expected-error {{bounds declaration not allowed because 't64' has a _Ptr type}}
  void((*t65)(void))  _Byte_count(1),// expected-error {{bounds declaration not allowed because 't65' has a function pointer type}}

  // bounds
  float t70 : bounds(s1, s1 + 1),          // expected-error {{expected 't70' to have a pointer, array, or integer type}}
  double t71 : bounds(s1, s1 + 1),         // expected-error {{expected 't71' to have a pointer, array, or integer type}}
  struct S1 t72 : bounds(s1, s1 + 1),      // expected-error {{expected 't72' to have a pointer, array, or integer type}}
  union U1 t73 : bounds(s1, s1 + 1),       // expected-error {{expected 't73' to have a pointer, array, or integer type}}
  ptr<int> t74 : bounds(s1, s1 + 1),       // expected-error {{bounds declaration not allowed because 't74' has a _Ptr type}}
  void((*t75)(void)) : bounds(s1, s1 + 1) // expected-error {{bounds declaration not allowed because 't75' has a function pointer type}}
  )
{
}

extern void anonymous_invalid_param_var_bounds_decl(
  // count
  int s1 checked[16],
  char : count(5),           // expected-error {{expected '' to have a pointer or array type}}
  _Bool : count(5),          // expected-error {{expected '' to have a pointer or array type}}
  short int  : count(5),     // expected-error {{expected '' to have a pointer or array type}}
  int : count(5),            // expected-error {{expected '' to have a pointer or array type}}
  long int : count(5),       // expected-error {{expected '' to have a pointer or array type}}
  unsigned short int : count(5), // expected-error {{expected '' to have a pointer or array type}}
  unsigned int  _Count(5),       // expected-error {{expected '' to have a pointer or array type}}
  unsigned long  _Count(5),  // expected-error {{expected '' to have a pointer or array type}}

  float : count(5),          // expected-error {{expected '' to have a pointer or array type}}
  double : count(5),         // expected-error {{expected '' to have a pointer or array type}}
  struct S1 : count(5),      // expected-error {{expected '' to have a pointer or array type}}
  union U1 : count(5),       // expected-error {{expected '' to have a pointer or array type}}
  enum E1 : count(5),        // expected-error {{expected '' to have a pointer or array type}}
  ptr<int> : count(1),        // expected-error {{bounds declaration not allowed because '' has a _Ptr type}}
  array_ptr<void> : count(1), // expected-error {{expected '' to have a non-void pointer type}}
  void((*)(void)) : count(1),  // expected-error {{bounds declaration not allowed because '' has a function pointer type}}

  // byte_count
  float  _Byte_count(8),         // expected-error {{expected '' to have a pointer, array, or integer type}}
  double  _Byte_count(8),        // expected-error {{expected '' to have a pointer, array, or integer type}}
  struct S1  _Byte_count(8),     // expected-error {{expected '' to have a pointer, array, or integer type}}
  union U1  _Byte_count(8),      // expected-error {{expected '' to have a pointer, array, or integer type}}
  int* _Single  _Byte_count(8),      // expected-error {{bounds declaration not allowed because '' has a _Ptr type}}
  void((*)(void))  _Byte_count(1),// expected-error {{bounds declaration not allowed because '' has a function pointer type}}

  // bounds
  float : bounds(s1, s1 + 1),          // expected-error {{expected '' to have a pointer, array, or integer type}}
  double : bounds(s1, s1 + 1),         // expected-error {{expected '' to have a pointer, array, or integer type}}
  struct S1 : bounds(s1, s1 + 1),      // expected-error {{expected '' to have a pointer, array, or integer type}}
  union U1 : bounds(s1, s1 + 1),       // expected-error {{expected '' to have a pointer, array, or integer type}}
  ptr<int> : bounds(s1, s1 + 1),       // expected-error {{bounds declaration not allowed because '' has a _Ptr type}}
  void((*)(void)) : bounds(s1, s1 + 1) // expected-error {{bounds declaration not allowed because '' has a function pointer type}}
  );


//
// Test member bounds declarations
//

//
// Test valid member bounds declarations
//

// count
struct S3 {
  array_ptr<int> f1 : count(5);
  nt_array_ptr<int> f1a : count(5);
  int *f2 : count(5);
  int f3 checked[5] : count(5);
  int f3a[5] : count(5);
  int f3b nt_checked[5] : count(4);
};

// byte_count
struct S4 {
  array_ptr<int> f4 : byte_count(5 * sizeof(int));
  array_ptr<void> f5 : byte_count(5 * sizeof(int));
  int *f6 : byte_count(5 * sizeof(int));
  int f7 checked[5] : byte_count(5 * sizeof(int));
  int f7a nt_checked[5] : byte_count(4 * sizeof(int));
  unsigned int f8 checked[5] : byte_count(5 * sizeof(int));
  unsigned int f9[5]  _Byte_count(5 * sizeof(int));
};

// bounds
struct S6 {
  array_ptr<int> f10 : bounds(f10, f10 + 5);
  // The use of 'f10' is intentional - pointer arithmetic is not
  // allowed on pointers to void types
  array_ptr<void> f11 : bounds(f10, f10 + 5);
  int *f12 : bounds(f12, f12 + 5);
  int f13 checked[5] : bounds(f13, f13 + 5);
  unsigned int f14 checked[5] : bounds(f14, f14 + 5);
  unsigned int f14a nt_checked[5]  _Bounds(f14a, f14a + 5);
  int f15[5] : bounds(f15, f15 + 5);
  unsigned int f16[5]  _Bounds(f16, f16 + 5);
};

//
// Test valid member bounds declarations on integer-typed members
//

struct S7 {
  int f1 checked[16];
  short int f20 : byte_count(5 * sizeof(int));
  int f21 : byte_count(5 * sizeof(int));
  long int f22 : byte_count(5 * sizeof(int));
  unsigned long int f23 : byte_count(5 * sizeof(int));
  enum E1 f24 : byte_count(8);

  // bounds
  int f25 : bounds(f1, f1 + 5);
  long int f26 : bounds(f1, f1 + 5);
  unsigned long int f27 : bounds(f1, f1 + 5);
  enum E1 f28 : bounds(f1, f1 + 5);

};

//
// Test invalid member bounds declarations.
//

struct s8 {
  // count
  int s1 checked[16];
  char g41 : count(5);           // expected-error {{expected 'g41' to have a pointer or array type}}
  _Bool g42 : count(5);          // expected-error {{expected 'g42' to have a pointer or array type}}
  short int g43 : count(5);      // expected-error {{expected 'g43' to have a pointer or array type}}
  int g44 : count(5);            // expected-error {{expected 'g44' to have a pointer or array type}}
  long int g45 : count(5);       // expected-error {{expected 'g45' to have a pointer or array type}}
  unsigned short int g46 : count(5); // expected-error {{expected 'g46' to have a pointer or array type}}
  unsigned int g47 : count(5);       // expected-error {{expected 'g47' to have a pointer or array type}}
  unsigned long int g48 : count(5);  // expected-error {{expected 'g48' to have a pointer or array type}}

  float g49 : count(5);           // expected-error {{expected 'g49' to have a pointer or array type}}
  double g50 : count(5);          // expected-error {{expected 'g50' to have a pointer or array type}}
  struct S1 g51 : count(5);       // expected-error {{expected 'g51' to have a pointer or array type}}
  union U1 g52 : count(5);        // expected-error {{expected 'g52' to have a pointer or array type}}
  enum E1 g53 : count(5);         // expected-error {{expected 'g53' to have a pointer or array type}}
  ptr<int> g54: count(1);         // expected-error {{bounds declaration not allowed because 'g54' has a _Ptr type}}
  array_ptr<void> g55 : count(1); // expected-error {{expected 'g55' to have a non-void pointer type}}
  void((*g56)(void)) : count(1);  // expected-error {{bounds declaration not allowed because 'g56' has a function pointer type}}

  // byte_count
  float g60 : byte_count(8);      // expected-error {{expected 'g60' to have a pointer, array, or integer type}}
  double g61 : byte_count(8);     // expected-error {{expected 'g61' to have a pointer, array, or integer type}}
  struct S1 g62 : byte_count(8);  // expected-error {{expected 'g62' to have a pointer, array, or integer type}}
  union U1 g63 : byte_count(8);   // expected-error {{expected 'g63' to have a pointer, array, or integer type}}
  ptr<int> g64 : byte_count(8);   // expected-error {{bounds declaration not allowed because 'g64' has a _Ptr type}}
  void((*g65)(void)) : byte_count(1);    // expected-error {{bounds declaration not allowed because 'g65' has a function pointer type}}

  // bounds
  float g70 : bounds(s1, s1 + 1);          // expected-error {{expected 'g70' to have a pointer, array, or integer type}}
  double g71 : bounds(s1, s1 + 1);         // expected-error {{expected 'g71' to have a pointer, array, or integer type}}
  struct S1 g72 : bounds(s1, s1 + 1);      // expected-error {{expected 'g72' to have a pointer, array, or integer type}}
  union U1 g73 : bounds(s1, s1 + 1);       // expected-error {{expected 'g73' to have a pointer, array, or integer type}}
  int* _Single g74  _Bounds(s1, s1 + 1);       // expected-error {{bounds declaration not allowed because 'g74' has a _Ptr type}}
  void((*g75)(void))  _Bounds(s1, s1 + 1); // expected-error {{bounds declaration not allowed because 'g75' has a function pointer type}}
};


//
// Test function return bounds declarations.
//

//
// Test valid function return bounds declarations
//

// count
int* _Array fn1(void)  _Count(5) { return 0; }
nt_array_ptr<int> fn1a(void) : count(0) { return 0; }
int *fn2(void) : count(5) { return 0; }
int *fn3(int len) : count(len) { return 0; }

// byte_count
extern array_ptr<int> fn4(void) : byte_count(5 * sizeof(int));
extern nt_array_ptr<int> fn4a(void) : byte_count(5 * sizeof(int));
extern array_ptr<void> fn5(void) : byte_count(5 * sizeof(int));
extern int *fn6(void) : byte_count(5 * sizeof(int));

// bounds
array_ptr<int> fn10(void) : bounds(s1, s1 + 5) { return 0; }
int* _Nt_array fn10a(void)  _Bounds(s1, s1 + 5) { return 0; }
array_ptr<void> fn11(void) : bounds(s1, s1 + 5) { return 0; }
int *fn12(void) : bounds(s1, s1 + 5) { return 0; }

// Test valid return bounds declarations for integer-typed values
short int fn20(void) : byte_count(5 * sizeof(int)) { return (short int) s1; } // expected-warning {{cast to smaller integer type 'short' from '_Array_ptr<int>'}} \
                                                                              // expected-error {{it is not possible to prove that return value bounds imply declared return bounds for 'fn20'}}
int fn21(void) : byte_count(5 * sizeof(int)) { return (short int)s1; } // expected-warning {{cast to smaller integer type 'short' from '_Array_ptr<int>'}} \
                                                                       // expected-error {{it is not possible to prove that return value bounds imply declared return bounds for 'fn21'}}
long int fn22(void) : byte_count(5 * sizeof(int)) { return (short int)s1; } // expected-warning {{cast to smaller integer type 'short' from '_Array_ptr<int>'}} \
                                                                            // expected-error {{it is not possible to prove that return value bounds imply declared return bounds for 'fn22'}}
unsigned long int fn23(void) : byte_count(5 * sizeof(int)) { return (short int)s1; } // expected-warning {{cast to smaller integer type 'short' from '_Array_ptr<int>'}} \
                                                                                     // expected-error {{it is not possible to prove that return value bounds imply declared return bounds for 'fn23'}}
enum E1 fn24(void) : byte_count(8) { return (short int)s1; } // expected-warning {{cast to smaller integer type 'short' from '_Array_ptr<int>'}} \
                                                             // expected-error {{it is not possible to prove that return value bounds imply declared return bounds for 'fn24'}}

// bounds
extern int fn25(void) : bounds(s1, s1 + 5);
extern long int fn26(void) : bounds(s1, s1 + 5);
extern unsigned long int fn27(void) : bounds(s1, s1 + 5);
extern enum E1 fn28(void) : bounds(s1, s1 + 5);

//
// Test invalid return bounds declarations
//

// count
char fn41(void) : count(5);         // expected-error {{count bounds expression only allowed for pointer return type}}
_Bool fn42(void) : count(5);        // expected-error {{count bounds expression only allowed for pointer return type}}
short int fn43(void) : count(5);    // expected-error {{count bounds expression only allowed for pointer return type}}
int fn44(void) : count(5);          // expected-error {{count bounds expression only allowed for pointer return type}}
long int fn45(void) : count(5);     // expected-error {{count bounds expression only allowed for pointer return type}}
unsigned short int fn46(void) : count(5); // expected-error {{count bounds expression only allowed for pointer return type}}
unsigned int fn47(void) : count(5);       // expected-error {{count bounds expression only allowed for pointer return type}}
unsigned long int fn48(void) : count(5);  // expected-error {{count bounds expression only allowed for pointer return type}}

float fn49(void) : count(5);        // expected-error {{count bounds expression only allowed for pointer return type}}
double fn50(void) : count(5);       // expected-error {{count bounds expression only allowed for pointer return type}}
struct S1 fn51(void) : count(5);    // expected-error {{count bounds expression only allowed for pointer return type}}
union U1 fn52(void) : count(5);     // expected-error {{count bounds expression only allowed for pointer return type}}
enum E1 fn53(void) : count(5);      // expected-error {{count bounds expression only allowed for pointer return type}}
ptr<int> fn54(void) : count(1);     // expected-error {{bounds declaration not allowed for a _Ptr return type}}
array_ptr<void> fn55(void) : count(1); // expected-error {{count bounds expression not allowed for a void pointer return type}}
void (*fn56(void) : count(1))(int);    // expected-error {{bounds declaration not allowed for a function pointer return type}}
ptr<void(int)> fn57(void) : count(1); // expected-error {{bounds declaration not allowed for a _Ptr return type}}

// byte_count
float fn60(void) : byte_count(8);     // expected-error {{bounds declaration only allowed for a pointer or integer return type}}
double fn61(void) : byte_count(8);    // expected-error {{bounds declaration only allowed for a pointer or integer return type}}
struct S1 fn62(void) : byte_count(8); // expected-error {{bounds declaration only allowed for a pointer or integer return type}}
union U1 fn63(void) : byte_count(8);  // expected-error {{bounds declaration only allowed for a pointer or integer return type}}
ptr<int> fn64(void) : byte_count(sizeof(int)); // expected-error {{bounds declaration not allowed for a _Ptr return type}}
void (*fn65(void) : byte_count(1))(int);   // expected-error {{bounds declaration not allowed for a function pointer return type}}
ptr<void(int)> fn66(void) : byte_count(1); // expected-error {{bounds declaration not allowed for a _Ptr return type}}

// bounds
float fn70(void) : bounds(s1, s1 + 1);      // expected-error {{bounds declaration only allowed for a pointer or integer return type}}
double fn71(void) : bounds(s1, s1 + 1);     // expected-error {{bounds declaration only allowed for a pointer or integer return type}}
struct S1 fn72(void) : bounds(s1, s1 + 1);  // expected-error {{bounds declaration only allowed for a pointer or integer return type}}
union U1 fn73(void) : bounds(s1, s1 + 1);   // expected-error {{bounds declaration only allowed for a pointer or integer return type}}
ptr<int> fn74(void) : bounds(s1, s1 + 1);   // expected-error {{bounds declaration not allowed for a _Ptr return type}}
void (*fn75(void) : bounds(s1, s1 + 1))(int);  // expected-error {{bounds declaration not allowed for a function pointer return type}}
ptr<void(int)> fn76(void) : bounds(s1, s1 + 1);  // expected-error {{bounds declaration not allowed for a _Ptr return type}}

//
// Test bounds declarations on function parameters
//

// These are numbered so that they correspond to the numbering of
// functions with function pointer parameters in the next section of
// tests.
void fn100(array_ptr<int> p1 : count(5));
void fn100a(array_ptr<int> p1 : count(6));
void fn101(int p1 : count(5));             // expected-error {{expected 'p1' to have a pointer or array type}}
void fn102(array_ptr<void> p1 : count(5)); // expected-error {{expected 'p1' to have a non-void pointer type}}

int fn103(array_ptr<int> p1 : byte_count(5 * sizeof(int)));
int fn103a(array_ptr<int> p1 : byte_count(7 * sizeof(int)));
int fn104(int *p1 : byte_count(5 * sizeof(int)));
int fn104a(int *p1 : byte_count(6 * sizeof(int)));
int fn104b(float *p1 : byte_count(6 * sizeof(float)));
int fn105(int p1 : byte_count(5 * sizeof(int)));
int fn106(float p1 : byte_count(5 * sizeof(int))); // expected-error {{expected 'p1' to have a pointer, array, or integer type}}
int fn107(array_ptr<void> p1 : byte_count(5 * sizeof(int)));

void fn108(array_ptr<int> p1 : bounds(p1, p1 + 5));
void fn108a(nt_array_ptr<int> p1 : bounds(p1, p1 + 5));
void fn109(array_ptr<int> p1, int p2 : bounds(p1, p1 + 5));
void fn110(array_ptr<int> p1, float p2 : bounds(p1, p1 + 5)); // expected-error {{expected 'p2' to have a pointer, array, or integer type}}
void fn111(array_ptr<void> p1 : bounds((char *)p1, (char *)p1 + (5 * sizeof(int))));

// A few functions with multiple arguments.
void fn120(array_ptr<int> mid : bounds(p1, p1 + 5), array_ptr<int> p1);
void fn122(array_ptr<void> mid : bounds((char *)p1, (char *)p1 + (5 * sizeof(int))),
           array_ptr<int> p1);

//
// Test bounds declaration on parameters of function pointer types
//

// These are numbered so that they correspond to the numbering of
// functions in the prior section of tests: fn200 - fn211 take fn100 - fn111 as
//  arguments.  fn220-231 are versions of fn200 - fn211 that take checked
// function pointers.

// Unchecked function pointers
void fn200(void (*fnptr)(int* _Array p1  _Count(5)));
void fn201(void (*fnptr)(int p1  _Count(5)));             // expected-error {{expected 'p1' to have a pointer or array type}}
void fn202(void (*fnptr)(void* _Array p1  _Count(5))); // expected-error {{expected 'p1' to have a non-void pointer type}}

void fn203(int (*fnptr)(array_ptr<int> p1 : byte_count(5 * sizeof(int))));
void fn204(int (*fnptr)(int * : byte_count(5 * sizeof(int))));
void fn205(int (*fnptr)(int p1 : byte_count(5 * sizeof(int))));
void fn206(int (*fnptr)(float p1 : byte_count(5 * sizeof(int)))); // expected-error {{expected 'p1' to have a pointer, array, or integer type}}
void fn207(int (*fnptr)(array_ptr<void> p1 : byte_count(5 * sizeof(int))));

void fn208(void (*fnptr)(array_ptr<int> p1 : bounds(p1, p1 + 5)));
void fn208a(void(*fnptr)(nt_array_ptr<int> p1 : bounds(p1, p1 + 5)));
void fn209(void (*fnptr)(array_ptr<int> p1, int p2 : bounds(p1, p1 + 5)));
void fn210(void (*fnptr)(array_ptr<int> p1, float p2 : bounds(p1, p1 + 5))); // expected-error {{expected 'p2' to have a pointer, array, or integer type}}
void fn211(void (*fnptr)(array_ptr<void> p1 : bounds((char *) p1, (char *) p1 + (5 * sizeof(int)))));

// Checked function pointers
void fn220(ptr<void (array_ptr<int> p1 : count(5))> fnptr);
void fn221(ptr<void (int p1 : count(5))> fnptr);             // expected-error {{expected 'p1' to have a pointer or array type}}
void fn222(ptr<void (array_ptr<void> p1 : count(5))> fnptr); // expected-error {{expected 'p1' to have a non-void pointer type}}

void fn223(ptr<int (array_ptr<int> p1 : byte_count(5 * sizeof(int)))> fnptr);
void fn224(ptr<int (int * : byte_count(5 * sizeof(int)))> fnptr);
void fn225(ptr<int (int p1 : byte_count(5 * sizeof(int)))> fnptr);
void fn226(ptr<int (float p1 : byte_count(5 * sizeof(int)))> fnptr); // expected-error {{expected 'p1' to have a pointer, array, or integer type}}
void fn227(ptr<int (array_ptr<void> p1 : byte_count(5 * sizeof(int)))> fnptr);

void fn228(ptr<void (array_ptr<int> p1 : bounds(p1, p1 + 5))> fnptr);
void fn229(ptr<void (array_ptr<int> p1, int p2 : bounds(p1, p1 + 5))> fnptr);
void fn230(ptr<void (array_ptr<int> p1, float p2 : bounds(p1, p1 + 5))> fnptr); // expected-error {{expected 'p2' to have a pointer, array, or integer type}}
void fn231(ptr<void (array_ptr<void> p1 : bounds((char *)p1, (char *)p1 + (5 * sizeof(int))))> fnptr);

// Function pointers with multiple arguments.
void fn240(ptr<int (array_ptr<int> mid : bounds(p1, p1 + 5), array_ptr<int> p1)> fnptr);
void fn241(ptr<int (void* _Array mid  _Bounds((char *)p1, (char *)p1 + (5 * sizeof(int))),
                   int* _Array p1)> fnptr);

//
// Spot check bounds-safe interfaces on parameters of function pointer types
//

void fn250(int(*fnptr)(int *p1 : count(5)));
void fn251(int(*fnptr)(int *p1 : byte_count(5 * sizeof(int))));
void fn252(int(*fnptr)(int *p1 : bounds(p1, p1 + 5)));
//
// Spot check bounds declaration for return values of function pointer types
//

void fn260(array_ptr<int> (*fnptr)(void) : count(5));
void fn261(array_ptr<int>(*fnptr)(int i) : count(i));
void fn262(int (*fnptr)(void) : count(5)); // expected-error {{count bounds expression only allowed for pointer return type}}
void fn263(void* _Array (*fnptr)(void)  _Count(5)); // expected-error {{count bounds expression not allowed for a void pointer return type}}

//
// Test bounds declarations for function pointers
//

void function_pointers(void) {
  // Assignments to function pointers with return bounds on array_ptr types
  array_ptr<int>(*t1)(void) : count(5) = fn1;
  int* _Nt_array(*t1a)(void)  _Count(0) = fn1a;
  // Assignment to function pointers with bounds-safe interfaces on
  // unchecked pointer return types.  Unchecked pointers are compatible with
  // unchecked pointers with bounds-safe interfaces.  This extends recursively
  // to parameters and returns of function types.
  int *(*t2)(void) = fn2;
  int *(*t3)(void) : count(5) = fn2;
  ptr<int *(void) : count(5)> t4 = fn2;

  int *(*t5)(int i) = fn3;
  int *(*t6)(int i) : count(i) = fn3;
  ptr<int *(int j) : count(j)> t7 = fn3;

  array_ptr<int>(*t8)(void) : byte_count(5 * sizeof(int)) = fn4;
  void* _Array(*t9)(void)  _Byte_count(5 * sizeof(int)) = fn5;
  int *(*t10)(void) = fn6;
  int *(*t11)(void) : byte_count(5*sizeof(int)) = fn6;
  ptr<int *(void) : byte_count(5*sizeof(int))> t12 = fn6;

  array_ptr<int>(*t13)(void) : bounds(s1, s1 + 5) = fn10;
  int* _Nt_array(*t13a)(void)  _Bounds(s1, s1 + 5) = fn10a;
  int *(*t14)(void) = fn12;
  int *(*t15)(void) : bounds(s1, s1 + 5) = fn12;
  int *(*t16)(void) : bounds(s1, s1 + 6) = fn12;    // expected-warning {{incompatible function pointer types}}
  ptr<int *(void) : bounds(s1, s1 + 6)> t17 = fn12; // expected-error {{incompatible type}}

  // Unchecked pointer to function assigned to checked pointer to
  // function.
  ptr<array_ptr<int>(void) : count(5)> t100 = fn1;
  // The reverse is not allowed
  int* _Array(*t101)(void)  _Count(5) = t100; // expected-error {{incompatible type}}

  // Calls that pass function pointers with bounds
  fn200(fn100);
  fn200(fn100a); // expected-error {{parameter of incompatible type}}
  fn201(fn101);
  fn202(fn102);
  fn203(fn103);
  fn203(fn103a); // expected-error {{parameter of incompatible type}}
  fn204(fn104);
  // These are mismatched unchecked function pointers with bounds-safe interfaces
  // on parameters.
  fn204(fn104a); // expected-warning {{incompatible function pointer types}}
  fn204(fn104b); // expected-warning {{incompatible function pointer types}}
  fn205(fn105);
  fn206(fn106);
  fn207(fn107);
  fn208(fn108);
  fn208(fn108a); // expected-error {{parameter of incompatible type}}
  fn208a(fn108a);
  fn209(fn109);
  fn210(fn110);
  fn211(fn111);

  fn220(fn100);
  fn220(fn100a); // expected-error {{parameter of incompatible type}}
  fn221(fn101);
  fn222(fn102);
  fn223(fn103);
  fn223(fn103a); // expected-error {{parameter of incompatible type}}
  fn224(fn104);
  // These are mismatched checked function pointers with bounds-safe interfaces
  // on parameters.
  fn224(fn104a); // expected-error {{parameter of incompatible type}}
  fn224(fn104b); // expected-error {{parameter of incompatible type}}
  fn225(fn105);
  fn226(fn106);
  fn227(fn107);
  fn228(fn108);
  fn229(fn109);
  fn230(fn110);
  fn231(fn111);
}

void invalid_function_pointers(void) {
  array_ptr<int>(*t1)(void) : count(4) = fn1;  // expected-error {{incompatible type}}
  ptr<array_ptr<int>(void) : count(4)> t1a = fn1;  // expected-error {{incompatible type}}
  array_ptr<int>(*t4)(void) : byte_count(6 * sizeof(int)) = fn4; // expected-error {{incompatible type}}
  int* _Array(*t10)(void)  _Bounds(s1, s1 + 4) = fn10; // expected-error {{incompatible type}}
}

// Test type checking of return_value

extern int arr[10];
int f300(void) : bounds(arr, arr + return_value);
array_ptr<int> f301(void) : bounds(return_value, return_value + 10);
array_ptr<int> f302(void) : bounds(return_value - 5, return_value + 5);
array_ptr<int> f303(void) : count(return_value); // expected-error {{invalid argument type}}
array_ptr<int> f304(void) : bounds(arr, arr + (return_value << 5)); // expected-error {{invalid operands to binary expression}}
void* _Array f305(void)  _Bounds(return_value, return_value + 5); // expected-error {{arithmetic on a pointer to void}}
