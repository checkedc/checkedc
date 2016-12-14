// Feature tests of typechecking new Checked C bounds declarations.
//
// The following lines are for the LLVM test harness:
//
// RUN: %clang_cc1 -verify -fcheckedc-extension %s

// Test expressions with standard signed and unsigned integers types as
// arguments to count and byte_count.

#include "../../include/stdchecked.h"

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
  array_ptr<int> t10 : count(c10) = 0;
  array_ptr<int> t11 : count(c11) = 0;
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
  array_ptr<int> t25 : byte_count(c10) = 0;
  array_ptr<int> t26 : byte_count(c11) = 0;
  // Spot-check type checking of byte_count expressions involving file-scoped
  // variables.
  array_ptr<int> t27 : byte_count(A) = 0;
  array_ptr<int> t28 : byte_count(B) = 0;
  array_ptr<int> t29 : byte_count(12) = 0;
  // This will eventually fail static checking of bounds declarations, at which
  // point we'll need to add an expected error message.
  array_ptr<int> t30 : byte_count(-8) = 0;
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
  array_ptr<int> t12 : count(5.0f) = 0; // expected-error {{invalid argument type}}
  array_ptr<int> t13 : count(globalFloat) = 0; // expected-error {{invalid argument type}}

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
  array_ptr<int> t25 : byte_count(5.0f) = 0; // expected-error {{invalid argument type}}
  array_ptr<int> t26 : byte_count(globalFloat) = 0; // expected-error {{invalid argument type}}

#ifndef __STDC_NO_COMPLEX__
  array_ptr<int> t27 : byte_count(c8) = 0;   // expected-error {{invalid argument type}}
#endif
}

extern void bounds_exprs(void) {
   int i[2];
   // check combinations of different kinds of pointers to the same
   // object type.

   array_ptr<int> array_ptr_lb = i, array_ptr_ub = i + 1;
   ptr<int> ptr_lb = i, ptr_ub = i + 1;
   int *unchecked_ptr_lb = i, *unchecked_ptr_ub = i + 1;
   array_ptr<int> t1 : bounds(array_ptr_lb, array_ptr_ub) = i;
   array_ptr<int> t2 : bounds(ptr_lb, array_ptr_ub) = i;
   array_ptr<int> t3 : bounds(array_ptr_lb, ptr_ub) = i;
   array_ptr<int> t4 : bounds(unchecked_ptr_lb, array_ptr_ub);
   array_ptr<int> t5 : bounds(array_ptr_lb, unchecked_ptr_ub);
   array_ptr<int> t6 : bounds(ptr_lb, ptr_ub) = i;
   array_ptr<int> t7 : bounds(unchecked_ptr_lb, ptr_ub) = i;
   array_ptr<int> t8 : bounds(ptr_lb, unchecked_ptr_ub) = i;
   array_ptr<int> t9 : bounds(unchecked_ptr_lb, unchecked_ptr_ub);

   // use an array-typed value as the lower bound.  This value
   // should be converted implicitly to be a pointer type.

   array_ptr<int> t10 : bounds(i, i + 1) = i;
   array_ptr<int> t11 : bounds(i, array_ptr_ub) = i;
   array_ptr<int> t13 : bounds(i, ptr_ub) = i;

   array_ptr<void> void_array_ptr_lb = i, void_array_ptr_ub = i + 1;
   ptr<void> void_ptr_lb = i, void_ptr_ub = i + 1;
   void *void_unchecked_ptr_lb = i, *void_unchecked_ptr_ub = i + 1;

   // check combinations of differents kinds of pointers to void
   array_ptr<int> t21 : bounds(void_array_ptr_lb, void_array_ptr_ub) = i;
   array_ptr<int> t22 : bounds(void_ptr_lb, void_array_ptr_ub) = i;
   array_ptr<int> t23 : bounds(void_array_ptr_lb, void_ptr_ub) = i;
   array_ptr<int> t24 : bounds(void_unchecked_ptr_lb, void_array_ptr_ub);
   array_ptr<int> t25 : bounds(void_array_ptr_lb, void_unchecked_ptr_ub);
   array_ptr<int> t26 : bounds(void_ptr_lb, void_ptr_ub) = i;
   array_ptr<int> t27 : bounds(void_unchecked_ptr_lb, void_ptr_ub) = i;
   array_ptr<int> t28 : bounds(void_ptr_lb, void_unchecked_ptr_ub) = i;
   array_ptr<int> t29 : bounds(void_unchecked_ptr_lb, void_unchecked_ptr_ub);

   // check combinations of pointers to void and pointers to non-void types

   array_ptr<int> t42 : bounds(array_ptr_lb, void_array_ptr_ub) = i;
   array_ptr<int> t43 : bounds(ptr_lb, void_array_ptr_ub) = i;
   array_ptr<int> t44 : bounds(void_ptr_lb, array_ptr_ub) = i;
   array_ptr<int> t45 : bounds(void_array_ptr_lb, ptr_ub) = i;
   array_ptr<int> t46 : bounds(array_ptr_lb, void_ptr_ub) = i;
   array_ptr<int> t47 : bounds(unchecked_ptr_lb, void_array_ptr_ub);
   array_ptr<int> t48 : bounds(void_unchecked_ptr_lb, array_ptr_ub);
   array_ptr<int> t49 : bounds(void_array_ptr_lb, unchecked_ptr_ub);
   array_ptr<int> t50 : bounds(array_ptr_lb, void_unchecked_ptr_ub);

   array_ptr<int> t51 : bounds(void_ptr_lb, ptr_ub) = i;
   array_ptr<int> t52 : bounds(ptr_lb, void_ptr_ub) = i;
   array_ptr<int> t53 : bounds(unchecked_ptr_lb, void_ptr_ub) = i;
   array_ptr<int> t54 : bounds(void_unchecked_ptr_lb, ptr_ub) = i;
   array_ptr<int> t55 : bounds(void_ptr_lb, unchecked_ptr_ub) = i;
   array_ptr<int> t56 : bounds(ptr_lb, void_unchecked_ptr_ub) = i;

   // spot check cases where the value being declared has a different pointer type
   // than the bounds.
   array_ptr<char> t71 : bounds(array_ptr_lb, array_ptr_ub) = (array_ptr<char>) i;
   array_ptr<char> t72 : bounds(ptr_lb, array_ptr_ub) = (array_ptr<char>) i;
   array_ptr<char> t73 : bounds(unchecked_ptr_lb, ptr_ub) = (array_ptr<char>) i;
   array_ptr<char> t75 : bounds(void_array_ptr_lb, array_ptr_ub) = (array_ptr<char>) i;
   array_ptr<char> t76 : bounds(void_unchecked_ptr_lb, array_ptr_ub) = (array_ptr<char>) i;
   array_ptr<char> t77 : bounds(array_ptr_lb, void_unchecked_ptr_ub) = (array_ptr<char>) i;

   // use an array-typed value as the lower bound.  This should be converted
   // implicitly to be a pointer type.
   array_ptr<char> t78 : bounds(i, array_ptr_ub) = (array_ptr<char>) i;
   array_ptr<char> t79 : bounds(i, ptr_ub) = (array_ptr<char>) i;

   // spot check that typechecking looks through typedefs
   typedef array_ptr<int> int_array_ptr;
   typedef ptr<int> int_ptr;
   typedef int *int_unchecked_ptr;

   int_array_ptr typedef_array_ptr_lb = i, typedef_array_ptr_ub = i + 1;
   int_ptr typedef_ptr_lb = i, typedef_ptr_ub = i + 1;
   int_unchecked_ptr typedef_unchecked_ptr_lb = i, typedef_unchecked_ptr_ub = i + 1;

   array_ptr<int> t91 : bounds(typedef_array_ptr_lb, array_ptr_ub) = i;
   array_ptr<int> t92 : bounds(ptr_lb, typedef_array_ptr_ub) = i;
   array_ptr<int> t93 : bounds(array_ptr_lb, typedef_ptr_ub) = i;
   array_ptr<int> t94 : bounds(typedef_unchecked_ptr_lb, array_ptr_ub);
   array_ptr<int> t95 : bounds(typedef_array_ptr_lb, unchecked_ptr_ub);
   array_ptr<int> t96 : bounds(typedef_ptr_lb, ptr_ub) = i;
   array_ptr<int> t97 : bounds(unchecked_ptr_lb, typedef_ptr_ub) = i;
   array_ptr<int> t98 : bounds(ptr_lb, typedef_unchecked_ptr_ub) = i;
   array_ptr<int> t99 : bounds(typedef_unchecked_ptr_lb, unchecked_ptr_ub);

   // check that type qualifiers are discarded when comparing pointer types
   // in bounds expressions

   // permutations of array_ptr and const
   array_ptr<const int> array_ptr_const_lb = i;
   const array_ptr<int> const_array_ptr_lb = i;
   const array_ptr<const int> const_array_ptr_const_lb = i;
   array_ptr<const int> array_ptr_const_ub = i + 1;
   const array_ptr<int> const_array_ptr_ub = i + 1;
   const array_ptr<const int> const_array_ptr_const_ub = i + 1;

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

   array_ptr<int> t121 : bounds(array_ptr_const_lb, array_ptr_ub) = i;
   array_ptr<int> t122 : bounds(const_array_ptr_lb, array_ptr_ub) = i;
   array_ptr<int> t123 : bounds(const_array_ptr_const_lb, array_ptr_ub) = i;

   array_ptr<int> t124 : bounds(array_ptr_lb, array_ptr_const_ub) = i;
   array_ptr<int> t125 : bounds(array_ptr_lb, const_array_ptr_ub) = i;
   array_ptr<int> t126 : bounds(array_ptr_lb, const_array_ptr_const_ub) = i;

   array_ptr<int> t127 : bounds(const_array_ptr_lb, array_ptr_const_ub) = i;
   array_ptr<int> t128 : bounds(array_ptr_const_lb, const_array_ptr_ub) = i;
   array_ptr<int> t129 : bounds(const_array_ptr_const_lb, const_array_ptr_const_ub) = i;

   array_ptr<int> t130 : bounds(ptr_lb, array_ptr_ub) = i;
   array_ptr<int> t131 : bounds(array_ptr_lb, ptr_ub) = i;
   array_ptr<int> t132 : bounds(unchecked_ptr_lb, array_ptr_ub);
   array_ptr<int> t133 : bounds(array_ptr_lb, unchecked_ptr_ub);
   array_ptr<int> t134 : bounds(ptr_lb, ptr_ub) = i;
   array_ptr<int> t135 : bounds(unchecked_ptr_lb, ptr_ub) = i;
   array_ptr<int> t136 : bounds(ptr_lb, unchecked_ptr_ub) = i;
   array_ptr<int> t137 : bounds(unchecked_ptr_lb, unchecked_ptr_ub);
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
   array_ptr<int> t16 : bounds(test_func, test_func) = 0; // expected-error 2 {{invalid argument type 'void (*)(void)' to bounds expression}}

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
   array_ptr<char> t35 : bounds(char_unchecked_ptr_lb, int_ptr_ub) = (array_ptr<char>) i; // expected-error {{pointer type mismatch}}
}

//
// Test type requirements for bounds declarations.   There are various 
// requirements for the types of variables with bounds declaration.
//
// We need to test the cross-product of (variable or member 
//  declaration, scope for variables (global or local), the kind of the bounds
// expression, and the type for the variable).
//
// The test sfollow a specific pattern: there is a set of tests for global
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

// byte_count
array_ptr<int> g4 : byte_count(5 * sizeof(int)) = 0;
array_ptr<void> g5 : byte_count(5 * sizeof(int)) = 0;
int *g6 : byte_count(5 * sizeof(int)) = 0;
int g7 checked[5] : byte_count(5 * sizeof(int));
unsigned int g8 checked[5] : byte_count(5 * sizeof(int));
unsigned int g9[5] : byte_count(5 * sizeof(int));

// bounds
array_ptr<int> g10 : bounds(g10, g10 + 5) = 0;
// The use of 'g3' is intentional - pointer arithmetic is not
// allowed on pointers to void types
array_ptr<void> g11 : bounds(g3, g3 + 5);
int *g12 : bounds(g12, g12 + 5) = 0;
int g13 checked[5] : bounds(g13, g13 + 5);
unsigned int g14 checked[5] : bounds(g14, g14 + 5);
int g15[5] : bounds(g15, g15 + 5);
unsigned int g16[5] : bounds(g16, g16 + 5);

//
// Valid bounds declarations for integer-typed global variables.
//

int s1 checked[16];

// byte_count
// Cannot initialize this at compile time.
// short int g20 : byte_count(5 * sizeof(int)) = (short int) s1;
int g21 : byte_count(5 * sizeof(int)) = (int)s1;
long int g22 : byte_count(5 * sizeof(int)) = (long int)s1;
unsigned long int g23 : byte_count(5 * sizeof(int)) = (unsigned long int) s1;
enum E1 g24 : byte_count(8) = EnumVal1;

// bounds
int g25 : bounds(s1, s1 + 5) = (int)s1;
long int g26 : bounds(s1, s1 + 5) = (int)s1;
unsigned long int g27 : bounds(s1, s1 + 5) = (int)s1;
enum E1 g28 : bounds(s1, s1 + 5) = (int)s1;

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
array_ptr<void> g55 : count(1) = 0; // expected-error {{expected 'g55' to have a non-void pointer type}}
void((*g56)(void)) : count(1);      // expected-error {{bounds declaration not allowed because 'g56' has a function pointer type}}

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
ptr<int> g74 : bounds(s1, s1 + 1) = 0;      // expected-error {{bounds declaration not allowed because 'g74' has a _Ptr type}}
void((*g75)(void)) : bounds(s1, s1 + 1);    // expected-error {{bounds declaration not allowed because 'g75' has a function pointer type}}

//
// Test bounds declarations for local variables.
//

void local_var_bounds_decl(void)
{
  // count
  array_ptr<int> t1 : count(5) = 0;
  int t3 checked[5] : count(5);

  // byte_count
  array_ptr<int> t4 : byte_count(5 * sizeof(int)) = 0;
  array_ptr<void> t5 : byte_count(5 * sizeof(int)) = 0;
  int t7 checked[5] : byte_count(5 * sizeof(int));
  unsigned int t8 checked[5] : byte_count(5 * sizeof(int));

  // bounds
  array_ptr<int> t10 : bounds(t10, t10 + 5) = 0;
  // The use of 't3' is intentional - pointer arithmetic is not
  // allowed on pointers to void types.
  array_ptr<void> t11 : bounds(t3, t3 + 5) = t3;
  int t13 checked[5] : bounds(t13, t13 + 5);
  unsigned int t14 checked[5] : bounds(t14, t14 + 5);
}

void int_local_var_bounds_decl(void) {
  // bounds declarations are allowed for integer variables to support
  // casting of pointers to integers and back.  We usually expect this
  // to happen within expressions, but to allow uniform use of language
  // features, we allow bounds on integer-typed variables.
  int a1 checked[5];

  // byte_count
  short int t20 : byte_count(5 * sizeof(int)) = (short int)a1;
  int t21 : byte_count(5 * sizeof(int)) = (int)a1;
  long int t22 : byte_count(5 * sizeof(int)) = (long int)a1;
  unsigned long int t23 : byte_count(5 * sizeof(int)) = (unsigned long int) a1;
  enum E1 t24 : byte_count(8) = EnumVal1;

  // bounds
  int t25 : bounds(a1, a1 + 5) = (int)a1;
  long int t26 : bounds(a1, a1 + 5) = (int)a1;
  unsigned long int t27 : bounds(a1, a1 + 5) = (int)a1;
  enum E1 t28 : bounds(a1, a1 + 5) = (int)a1;
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
  array_ptr<void> t55 : count(1) = 0; // expected-error {{expected 't55' to have a non-void pointer type}}
  array_ptr<void (void)> t56 : count(1);  // expected-error {{bounds declaration not allowed because 't56' has a function pointer type}}

  int *t57 : count(1) = 0;          // expected-error {{bounds declaration not allowed for local variable with unchecked pointer type}}
  int t58[5] : count(5);            // expected-error {{bounds declaration not allowed for local variable with unchecked array type}}

  // byte_count
  float t60 : byte_count(8);                  // expected-error {{expected 't60' to have a pointer, array, or integer type}}
  double t61 : byte_count(8);                 // expected-error {{expected 't61' to have a pointer, array, or integer type}}
  struct S1 t62 : byte_count(8) = { 0 };      // expected-error {{expected 't62' to have a pointer, array, or integer type}}
  union U1 t63 : byte_count(8) = { 0 };       // expected-error {{expected 't63' to have a pointer, array, or integer type}}
  ptr<int> t64 : byte_count(sizeof(int)) = 0; // expected-error {{bounds declaration not allowed because 't64' has a _Ptr type}}
  array_ptr<void (void)> t65 : byte_count(1); // expected-error {{bounds declaration not allowed because 't65' has a function pointer type}}

  int *t67 : byte_count(sizeof(int)) = 0;     // expected-error {{bounds declaration not allowed for local variable with unchecked pointer type}}
  int t68[5] : byte_count(5 * sizeof(int));   // expected-error {{bounds declaration not allowed for local variable with unchecked array type}}

  // bounds
  float t70 : bounds(arr, arr + 1);             // expected-error {{expected 't70' to have a pointer, array, or integer type}}
  double t71 : bounds(arr, arr + 1);            // expected-error {{expected 't71' to have a pointer, array, or integer type}}
  struct S1 t72 : bounds(arr, arr + 1) = { 0 }; // expected-error {{expected 't72' to have a pointer, array, or integer type}}
  union U1 t73 : bounds(arr, arr + 1) = { 0 };  // expected-error {{expected 't73' to have a pointer, array, or integer type}}
  ptr<int> t74 : bounds(arr, arr + 1) = 0;      // expected-error {{bounds declaration not allowed because 't74' has a _Ptr type}}
  array_ptr<void (void)> t75 : bounds(arr, arr + 1);  // expected-error {{bounds declaration not allowed because 't75' has a function pointer type}}

  int *t78 : bounds(arr, arr + 1) = 0;          // expected-error {{bounds declaration not allowed for local variable with unchecked pointer type}}
  int t79[5] : bounds(arr, arr + 1);            // expected-error {{bounds declaration not allowed for local variable with unchecked array type}}
}

//
// Test parameter variable bounds declarations.
// 

void param_var_bounds_decl(
  // count
  array_ptr<int> t1 : count(5),
  int *t2 : count(5),
  int t3 checked[5] : count(5),
  int t3a[5] : count(5),

  // byte_count
  array_ptr<int> t4 : byte_count(5 * sizeof(int)),
  array_ptr<void> t5 : byte_count(5 * sizeof(int)),
  int *t6 : byte_count(5 * sizeof(int)),
  int t7 checked[5] : byte_count(5 * sizeof(int)),
  unsigned int t8 checked[5] : byte_count(5 * sizeof(int)),
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
  int * : count(5),
  int checked[5] : count(5),
  int [5] : count(5),

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
  ptr<int> t64 : byte_count(8),      // expected-error {{bounds declaration not allowed because 't64' has a _Ptr type}}
  void((*t65)(void)) : byte_count(1),// expected-error {{bounds declaration not allowed because 't65' has a function pointer type}}

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
  unsigned int : count(5),       // expected-error {{expected '' to have a pointer or array type}}
  unsigned long : count(5),  // expected-error {{expected '' to have a pointer or array type}}

  float : count(5),          // expected-error {{expected '' to have a pointer or array type}}
  double : count(5),         // expected-error {{expected '' to have a pointer or array type}}
  struct S1 : count(5),      // expected-error {{expected '' to have a pointer or array type}}
  union U1 : count(5),       // expected-error {{expected '' to have a pointer or array type}}
  enum E1 : count(5),        // expected-error {{expected '' to have a pointer or array type}}
  ptr<int> : count(1),        // expected-error {{bounds declaration not allowed because '' has a _Ptr type}}
  array_ptr<void> : count(1), // expected-error {{expected '' to have a non-void pointer type}}
  void((*)(void)) : count(1),  // expected-error {{bounds declaration not allowed because '' has a function pointer type}}

  // byte_count
  float : byte_count(8),         // expected-error {{expected '' to have a pointer, array, or integer type}}
  double : byte_count(8),        // expected-error {{expected '' to have a pointer, array, or integer type}}
  struct S1 : byte_count(8),     // expected-error {{expected '' to have a pointer, array, or integer type}}
  union U1 : byte_count(8),      // expected-error {{expected '' to have a pointer, array, or integer type}}
  ptr<int> : byte_count(8),      // expected-error {{bounds declaration not allowed because '' has a _Ptr type}}
  void((*)(void)) : byte_count(1),// expected-error {{bounds declaration not allowed because '' has a function pointer type}}

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
  int *f2 : count(5);
  int f3 checked[5] : count(5);
  int f3a[5] : count(5);
};

// byte_count
struct S4 {
  array_ptr<int> f4 : byte_count(5 * sizeof(int));
  array_ptr<void> f5 : byte_count(5 * sizeof(int));
  int *f6 : byte_count(5 * sizeof(int));
  int f7 checked[5] : byte_count(5 * sizeof(int));
  unsigned int f8 checked[5] : byte_count(5 * sizeof(int));
  unsigned int f9[5] : byte_count(5 * sizeof(int));
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
  int f15[5] : bounds(f15, f15 + 5);
  unsigned int f16[5] : bounds(f16, f16 + 5);
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
  ptr<int> g74 : bounds(s1, s1 + 1);       // expected-error {{bounds declaration not allowed because 'g74' has a _Ptr type}}
  void((*g75)(void)) : bounds(s1, s1 + 1); // expected-error {{bounds declaration not allowed because 'g75' has a function pointer type}}
};

//
// Test function return bounds declarations.
// 

//
// Test valid function return bounds declarations
//

// count
array_ptr<int> fn1(void) : count(5) { return 0; }
int *fn2(void) : count(5) { return 0; }

// byte_count
extern array_ptr<int> fn4(void) : byte_count(5 * sizeof(int));
extern array_ptr<void> fn5(void) : byte_count(5 * sizeof(int));
extern int *fn6(void) : byte_count(5 * sizeof(int));

// bounds
array_ptr<int> fn10(void) : bounds(s1, s1 + 5) { return 0; }
array_ptr<void> fn11(void) : bounds(s1, s1 + 5) { return 0; }
int *fn12(void) : bounds(s1, s1 + 5) { return 0; }

// Test valid rEturn bounds declarations for integer-typed values
short int fn20(void) : byte_count(5 * sizeof(int)) { return (short int) s1; }
int fn21(void) : byte_count(5 * sizeof(int)) { return (short int)s1; }
long int fn22(void) : byte_count(5 * sizeof(int)) { return (short int)s1; }
unsigned long int fn23(void) : byte_count(5 * sizeof(int)) { return (short int)s1; }
enum E1 fn24(void) : byte_count(8) { return (short int)s1; }

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
// Spot check bounds declaration on parameters of function pointer types
//

void fn100(int (*fnptr)(array_ptr<int> p1 : count(5)));
void fn101(int (*fnptr)(int p1 : count(5)));             // expected-error {{expected 'p1' to have a pointer or array type}}
void fn102(int (*fnptr)(array_ptr<void> p1 : count(5))); // expected-error {{expected 'p1' to have a non-void pointer type}}

void fn103(int (*fnptr)(array_ptr<int> p1 : byte_count(5 * sizeof(int))));
void fn104(int (*fnptr)(float p1 : byte_count(5 * sizeof(int)))); // expected-error {{expected 'p1' to have a pointer, array, or integer type}}
void fn105(int (*fnptr)(array_ptr<void> p1 : byte_count(5 * sizeof(int))));

void fn106(int (*fnptr)(array_ptr<int> p1 : bounds(p1, p1 + 5)));
void fn107(int (*fnptr)(array_ptr<int> p1, float p2 : bounds(p1, p1 + 5))); // expected-error {{expected 'p2' to have a pointer, array, or integer type}}
void fn108(int (*fnptr)(array_ptr<void> p1 : bounds((char *) p1, (char *) p1 + (5 * sizeof(int)))));

//
// Spot check bounds declaration for return values of function pointer types
//

void fn120(array_ptr<int> (*fnptr)(void) : count(5));
void fn121(int (*fnptr)(void) : count(5)); // expected-error {{count bounds expression only allowed for pointer return type}}
void fn122(array_ptr<void> (*fnptr)(void) : count(5)); // expected-error {{count bounds expression not allowed for a void pointer return type}}
