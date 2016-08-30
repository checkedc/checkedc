// Feature tests of typechecking new Checked C bounds declarations.
//
// The following lines are for the LLVM test harness:
//
// RUN: %clang_cc1 -verify -fcheckedc-extension %s

// Test expressions with standard signed and unsigned integers types as
// arguments to count and byte_count.

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

extern void f1(void) {}

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

  void (*func_ptr)(void) = f1;

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
  array_ptr<int> t9 : count(f1) = 0; // expected-error {{invalid argument type}}
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

//
// Check bounds declarations
//
// We need to check typechecking for the cross-product of (variable or member
// declaration, scope (local or global), the kind of the bounds expression,
// type for the variable).

void local_var_bounds_decl(void)
{
   // count
   array_ptr<int> t1 : count(5) = 0;
   int t2 checked[5] : count(5);

   // byte_count
   array_ptr<int> t3 : byte_count(5 * sizeof(int)) = 0;
   int t4 checked[5] : byte_count(5 * sizeof(int));
   unsigned int t5 checked[5] : byte_count(5 * sizeof(int));
   array_ptr<void> t6 : byte_count(8) = 0;

   // bounds
   array_ptr<int> t7 : bounds(t7, t7 + 5) = 0;
   // The use of 't2' is intentional - pointer arithmetic is not
   // allowed on pointers to void types.
   array_ptr<void> t8 : bounds(t2, t2 + 5) = t2;
   int t9 checked[5] : bounds(t9, t9 + 5);
   unsigned int t10 checked[5] : bounds(t10, t10 + 5);
}

void int_local_var_bounds_decl(void) {
  // bounds declarations are allowed for integer variables to support
  // casting of pointers to integers and back.  We usually expect this
  // to happen within expressions, but to allow uniform use of language
  // features, we allow bounds on integer-typed variables.
  int t1 checked[5];

  // byte_count
  short int t2 : byte_count(5 * sizeof(int)) = (short int)t1;
  int t3 : byte_count(5 * sizeof(int)) = (int)t1;
  long int t4 : byte_count(5 * sizeof(int)) = (long int)t1;
  unsigned long int t5 : byte_count(5 * sizeof(int)) = (unsigned long int) t1;
  enum E1 t6 : byte_count(8) = EnumVal1;

  // bounds
  int t7 : bounds(t1, t1 + 5) = (int) t1;
  long int t8 : bounds(t1, t1 + 5) = (int) t1;
  unsigned long int t9 : bounds(t1, t1 + 5) = (int) t1;
  enum E1 t10 : bounds(t1, t1 + 5) = (int) t1;
}

void invalid_local_var_bounds_decl(void)
{
  int arr[16];
  char t1 : count(5) = 0;           // expected-error {{expected 't1' to have pointer or array type}}
  _Bool t2 : count(5) = 0;          // expected-error {{expected 't2' to have pointer or array type}}
  short int t3 : count(5) = 0;      // expected-error {{expected 't3' to have pointer or array type}}
  int t4 : count(5) = 0;            // expected-error {{expected 't4' to have pointer or array type}}
  long int t5 : count(5) = 0;       // expected-error {{expected 't5' to have pointer or array type}}
  unsigned short int t6 : count(5) = 0; // expected-error {{expected 't6' to have pointer or array type}}
  unsigned int t7 : count(5) = 0;       // expected-error {{expected 't7' to have pointer or array type}}
  unsigned long int t8 : count(5) = 0;  // expected-error {{expected 't8' to have pointer or array type}}

  float t9 : count(5) = 0;          // expected-error {{expected 't9' to have pointer or array type}}
  double t10 : count(5) = 0;        // expected-error {{expected 't10' to have pointer or array type}}
  struct S1 t11 : count(5) = { 0 }; // expected-error {{expected 't11' to have pointer or array type}}
  union U1 t12 : count(5) = { 0 };  // expected-error {{expected 't12' to have pointer or array type}}
  enum E1 t13 : count(5) = EnumVal1; // expected-error {{expected 't13' to have pointer or array type}}

  int *t14 : count(1) = 0;          // expected-error {{expected local variable 't14' to have array_ptr type}}
  ptr<int> t15 : count(1) = 0;      // expected-error {{bounds declaration not allowed because 't15' has ptr type}}
  int t16[5] : count(5);            // expected-error {{expected local variable 't16' to have checked array type}}
  array_ptr<void> t17 : count(1) = 0; // expected-error {{expected 't17' to have non-void pointer type}}

  // byte_count
  float t30 : byte_count(8);              // expected-error {{expected 't30' to have pointer, array, or integer type}}
  double t31 : byte_count(8);             // expected-error {{expected 't31' to have pointer, array, or integer type}}
  struct S1 t32 : byte_count(8) = { 0 };  // expected-error {{expected 't32' to have pointer, array, or integer type}}
  union U1 t33 : byte_count(8) = { 0 } ;  // expected-error {{expected 't33' to have pointer, array, or integer type}}

  int *t34 : byte_count(sizeof(int)) = 0;       // expected-error {{expected local variable 't34' to have array_ptr type}}
  ptr<int> t35 : byte_count(sizeof(int)) = 0;   // expected-error {{bounds declaration not allowed because 't35' has ptr type}}
  int t36[5] : byte_count(5 * sizeof(int));     // expected-error {{expected local variable 't36' to have checked array type}}

  // bounds
  float t40 : bounds(arr, arr + 1);              // expected-error {{expected 't40' to have pointer, array, or integer type}}
  double t41 : bounds(arr, arr + 1);             // expected-error {{expected 't41' to have pointer, array, or integer type}}
  struct S1 t42 : bounds(arr, arr + 1) = { 0 };  // expected-error {{expected 't42' to have pointer, array, or integer type}}
  union U1 t43 : bounds(arr, arr + 1) = { 0 };  // expected-error {{expected 't43' to have pointer, array, or integer type}}

  int *t44 : bounds(arr, arr + 1) = 0;          // expected-error {{expected local variable 't44' to have array_ptr type}}
  ptr<int> t45 : bounds(arr, arr + 1) = 0;      // expected-error {{bounds declaration not allowed because 't45' has ptr type}}
  int t46[5] : bounds(arr, arr + 1);            // expected-error {{expected local variable 't46' to have checked array type}}
}

//
// Test global variable declarations
//

// Valid bounds declarations for global variables

// count
array_ptr<int> g1 : count(5) = 0;
int g2 checked[5] : count(5);

// byte_count
array_ptr<int> g3 : byte_count(5 * sizeof(int)) = 0;
int g4 checked[5] : byte_count(5 * sizeof(int));
unsigned int g5 checked[5] : byte_count(5 * sizeof(int));
unsigned int g6[5] : byte_count(5 * sizeof(int));
array_ptr<void> g7 : byte_count(8) = 0;

// bounds
array_ptr<int> g8 : bounds(g8, g8 + 5) = 0;
// The use of 'g2' is intentional - pointer arithmetic is not
// allowed on pointers to void types
array_ptr<void> g9 : bounds(g2, g2 + 5) = g2;
int g10 checked[5] : bounds(g10, g10 + 5);
unsigned int g11 checked[5] : bounds(g11, g11 + 5);
int g12[5] : bounds(g12, g12 + 5);
unsigned int g13[5] : bounds(g13, g13 + 5);
int *g14 : count(1) = 0;

// Valid bounds declarations for integer-typed global variables.

int s1 checked[16];

// byte_count
int g20 : byte_count(5 * sizeof(int)) = (int)s1;
long int g21 : byte_count(5 * sizeof(int)) = (long int)s1;
unsigned long int g22 : byte_count(5 * sizeof(int)) = (unsigned long int) s1;
enum E1 g23 : byte_count(8) = EnumVal1;

// bounds
int g24 : bounds(s1, s1 + 5) = (int)s1;
long int g25 : bounds(s1, s1 + 5) = (int)s1;
unsigned long int g26 : bounds(s1, s1 + 5) = (int)s1;
enum E1 g28 : bounds(s1, s1 + 5) = (int)s1;

// Invalid bounds declarations for global variables

// count
char g41 : count(5) = 0;           // expected-error {{expected 'g41' to have pointer or array type}}
_Bool g42 : count(5) = 0;          // expected-error {{expected 'g42' to have pointer or array type}}
short int g43 : count(5) = 0;      // expected-error {{expected 'g43' to have pointer or array type}}
int g44 : count(5) = 0;            // expected-error {{expected 'g44' to have pointer or array type}}
long int g45 : count(5) = 0;       // expected-error {{expected 'g45' to have pointer or array type}}
unsigned short int g46 : count(5) = 0; // expected-error {{expected 'g46' to have pointer or array type}}
unsigned int g47 : count(5) = 0;       // expected-error {{expected 'g47' to have pointer or array type}}
unsigned long int g48 : count(5) = 0;  // expected-error {{expected 'g48' to have pointer or array type}}

float g49 : count(5) = 0;          // expected-error {{expected 'g49' to have pointer or array type}}
double g50 : count(5) = 0;         // expected-error {{expected 'g50' to have pointer or array type}}
struct S1 g51 : count(5) = { 0 };  // expected-error {{expected 'g51' to have pointer or array type}}
union U1 g52 : count(5) = { 0 };   // expected-error {{expected 'g52' to have pointer or array type}}
enum E1 g53 : count(5) = EnumVal1; // expected-error {{expected 'g53' to have pointer or array type}}
ptr<int> g54: count(1) = 0;        // expected-error {{bounds declaration not allowed because 'g54' has ptr type}}
array_ptr<void> g55 : count(1) = 0; // expected-error {{expected 'g55' to have non-void pointer type}}

// byte_count
float g60 : byte_count(8);             // expected-error {{expected 'g60' to have pointer, array, or integer type}}
double g61 : byte_count(8);            // expected-error {{expected 'g61' to have pointer, array, or integer type}}
struct S1 g62 : byte_count(8) = { 0 }; // expected-error {{expected 'g62' to have pointer, array, or integer type}}
union U1 g63 : byte_count(8) = { 0 };  // expected-error {{expected 'g63' to have pointer, array, or integer type}}
ptr<int> g64 : byte_count(8) = 0;      // expected-error {{bounds declaration not allowed because 'g64' has ptr type}}

// bounds
float g70 : bounds(s1, s1 + 1);              // expected-error {{expected 'g70' to have pointer, array, or integer type}}
double g71 : bounds(s1, s1 + 1);             // expected-error {{expected 'g71' to have pointer, array, or integer type}}
struct S1 g72 : bounds(s1, s1 + 1) = { 0 };  // expected-error {{expected 'g72' to have pointer, array, or integer type}}
union U1 g73 : bounds(s1, s1 + 1) = { 0 };  // expected-error {{expected 'g73' to have pointer, array, or integer type}}
ptr<int> g74 : bounds(s1, s1 + 1) = 0;      // expected-error {{bounds declaration not allowed because 'g74' has ptr type}}

//
// Test member bounds declarations
//

// Test valid member bounds declarations

// count
struct S3 {
  array_ptr<int> f1 : count(5);
  int f2 checked[5] : count(5);
};

// byte_count
struct S4 {
  array_ptr<int> f3 : byte_count(5 * sizeof(int));
  int f4 checked[5] : byte_count(5 * sizeof(int));
  unsigned int f5 checked[5] : byte_count(5 * sizeof(int));
  unsigned int f6[5] : byte_count(5 * sizeof(int));
  array_ptr<void> f7 : byte_count(8);
};

// bounds
struct S6 {
  array_ptr<int> f8 : bounds(f8, f8 + 5);
  // The use of 'g2' is intentional - pointer arithmetic is not
  // allowed on pointers to void types
  array_ptr<void> f9 : bounds(f8, f8 + 5);
  int f10 checked[5] : bounds(f10, f10 + 5);
  unsigned int f11 checked[5] : bounds(f11, f11 + 5);
  int f12[5] : bounds(f12, f12 + 5);
  unsigned int f13[5] : bounds(f13, f13 + 5);
  int *f14 : count(1);
};

// Test valid member bounds declarations on integer-typed members

struct S7 {
  int f1[16];
  // byte_count
  int f20 : byte_count(5 * sizeof(int));
  long int f21 : byte_count(5 * sizeof(int));
  unsigned long int f22 : byte_count(5 * sizeof(int));
  enum E1 f23 : byte_count(8);

  // bounds
  int f24 : bounds(f1, f1 + 5);
  long int f25 : bounds(f1, f1 + 5);
  unsigned long int f26 : bounds(f1, f1 + 5);
  enum E1 f28 : bounds(f1, f1 + 5);
};

// Test invalid member bounds declarations for integer-typed members.

struct s8 {
  // count
  char g41 : count(5);           // expected-error {{expected 'g41' to have pointer or array type}}
  _Bool g42 : count(5);          // expected-error {{expected 'g42' to have pointer or array type}}
  short int g43 : count(5);      // expected-error {{expected 'g43' to have pointer or array type}}
  int g44 : count(5);            // expected-error {{expected 'g44' to have pointer or array type}}
  long int g45 : count(5);       // expected-error {{expected 'g45' to have pointer or array type}}
  unsigned short int g46 : count(5); // expected-error {{expected 'g46' to have pointer or array type}}
  unsigned int g47 : count(5);       // expected-error {{expected 'g47' to have pointer or array type}}
  unsigned long int g48 : count(5);  // expected-error {{expected 'g48' to have pointer or array type}}

  float g49 : count(5);      // expected-error {{expected 'g49' to have pointer or array type}}
  double g50 : count(5);     // expected-error {{expected 'g50' to have pointer or array type}}
  struct S1 g51 : count(5);  // expected-error {{expected 'g51' to have pointer or array type}}
  union U1 g52 : count(5);   // expected-error {{expected 'g52' to have pointer or array type}}
  enum E1 g53 : count(5);    // expected-error {{expected 'g53' to have pointer or array type}}
  ptr<int> g54: count(1);    // expected-error {{bounds declaration not allowed because 'g54' has ptr type}}
  array_ptr<void> g55 : count(1); // expected-error {{expected 'g55' to have non-void pointer type}}

  // byte_count
  float g60 : byte_count(8);       // expected-error {{expected 'g60' to have pointer, array, or integer type}}
  double g61 : byte_count(8);      // expected-error {{expected 'g61' to have pointer, array, or integer type}}
  struct S1 g62 : byte_count(8);   // expected-error {{expected 'g62' to have pointer, array, or integer type}}
  union U1 g63 : byte_count(8);    // expected-error {{expected 'g63' to have pointer, array, or integer type}}
  ptr<int> g64 : byte_count(8);    // expected-error {{bounds declaration not allowed because 'g64' has ptr type}}

  int s1[16];
  // bounds
  float g70 : bounds(s1, s1 + 1);     // expected-error {{expected 'g70' to have pointer, array, or integer type}}
  double g71 : bounds(s1, s1 + 1);    // expected-error {{expected 'g71' to have pointer, array, or integer type}}
  struct S1 g72 : bounds(s1, s1 + 1); // expected-error {{expected 'g72' to have pointer, array, or integer type}}
  union U1 g73 : bounds(s1, s1 + 1);  // expected-error {{expected 'g73' to have pointer, array, or integer type}}
  ptr<int> g74 : bounds(s1, s1 + 1);  // expected-error {{bounds declaration not allowed because 'g74' has ptr type}}
};
