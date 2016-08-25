// Feature tests of typechecking new Checked C bounds declarations.
//
// The following lines are for the LLVM test harness:
//
// RUN: %clang_cc1 -verify -fcheckedc-extension %s

// Test expressions with standard signed and unsigned integers types as
// arguments to count and byte_count.

static int A = 8;
static long long int B = 8;

extern void f1(void) {

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
extern void f2(void) {
  enum E {
    c1 = 8,
    c2
  };

  struct S {
    int f : 5;
  } s;
  s.f = 8;

  array_ptr<int> t1 : count(c1) = 0;
  array_ptr<int> t2 : count(c2) = 0;
  array_ptr<int> t3 : count(s.f) = 0;
}

float globalFloat = 8;

// Test types that should not work as arguments for count and byte_count.
extern void f3(void) {

  float c1 = 8.0;
  double c2 = 8.0;
  char *c3 = 0;
  double *c4 = 0;

  struct S1 {
    int f;
  } c5 = {0};

  union U1 {
    int f;
  } c6 = {0};

  double c7[10];

  struct S2 {
    float f;
  } s = { 8.0 };

  void (*func_ptr)(void) = f2;

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
  array_ptr<int> t9 : count(f2) = 0; // expected-error {{invalid argument type}}
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

