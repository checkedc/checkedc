// Unit tests for typechecking new Checked C array types
//
// The following line is for the LLVM test harness:
// RUN: %clang_cc1 -Wno-unused-value -Wno-pointer-bool-conversion -verify -verify-ignore-unexpected=note %s
//

#include <stdchecked.h>

extern void check_indirection_unchecked(int p[10], const int const_p[10], int y) {
  *p = y;
  y = *p;
  *const_p = y; // expected-error {{read-only variable is not assignable}}
  y = *const_p;
}

extern void check_indirection_checked(int p checked[10], const int const_p checked[10], int y) {
  *p = y;
  y = *p;
  *const_p = y; // expected-error {{read-only variable is not assignable}}
  y = *const_p;
}

extern void check_indirection_checked_incomplete(int p checked[] : count(len),
                                                 const int const_p checked[] : count(len),
                                                 int len, int y) {
  *p = y;
  y = *p;
  *const_p = y; // expected-error {{read-only variable is not assignable}}
  y = *const_p;
}

extern void check_indirection_nt_checked(int p nt_checked[10], const int const_p nt_checked[10], int y) {
  *p = y;
  y = *p;
  *const_p = y; // expected-error {{read-only variable is not assignable}}
  y = *const_p;
}

extern void check_indirection_checked_nt_incomplete(int p checked[] : count(len),
                                                    const int const_p nt_checked[] : count(len),
                                                    int len, int y) {
  *p = y;
  y = *p;
  *const_p = y; // expected-error {{read-only variable is not assignable}}
  y = *const_p;
}

extern void check_subscript_unchecked(int p[10], int y) {
  p[0] = y;
  y = p[0]; 
  0[p] = y;
  y = 0[p];
}

extern void check_subscript_checked(int p checked[10], const int p_const checked[10], int y) {
  p[0] = y;  // OK
  y = p[0];  // OK
  0[p] = y;  // OK
  y = 0[p];  // OK
  p_const[0] = y;  // expected-error {{read-only variable is not assignable}}
  y = p_const[0];  // OK
  0[p_const] = y;  // expected-error {{read-only variable is not assignable}}
  y = 0[p_const];  // OK
}

extern void check_subscript_checked_incomplete(int p checked[] : count(len),
                                               const int p_const checked[] : count(len),
                                               int len, int y) {
  p[0] = y;  // OK
  y = p[0];  // OK
  0[p] = y;  // OK
  y = 0[p];  // OK
  p_const[0] = y;  // expected-error {{read-only variable is not assignable}}
  y = p_const[0];  // OK
  0[p_const] = y;  // expected-error {{read-only variable is not assignable}}
  y = 0[p_const];  // OK
}

extern void check_subscript_nt_checked(int p nt_checked[10], const int p_const checked[10], int y) {
  p[0] = y;  // OK
  y = p[0];  // OK
  0[p] = y;  // OK
  y = 0[p];  // OK
  p_const[0] = y;  // expected-error {{read-only variable is not assignable}}
  y = p_const[0];  // OK
  0[p_const] = y;  // expected-error {{read-only variable is not assignable}}
  y = 0[p_const];  // OK
}

extern void check_subscript_nt_checked_incomplete(int p checked[] : count(len),
                                               const int p_const checked[] : count(len),
                                               int len, int y) {
  p[0] = y;  // OK
  y = p[0];  // OK
  0[p] = y;  // OK
  y = 0[p];  // OK
  p_const[0] = y;  // expected-error {{read-only variable is not assignable}}
  y = p_const[0];  // OK
  0[p_const] = y;  // expected-error {{read-only variable is not assignable}}
  y = 0[p_const];  // OK
}

static int global_arr[10];
static int global_checked_arr checked[10];
static int global_nt_checked_arr nt_checked[10];

typedef int unchecked_arr_type[10];
typedef int checked_arr_type[10];
typedef int nt_checked_arr_type[10];

// Test assignments between pointers and arrays, excluding const/volatile attributes.
extern void check_assign(int val, int p[10], int q[], int r checked[10], int s checked[],
                         int s2d checked[10][10], int v nt_checked[10], int w nt_checked[],
                         int w2d checked[10]nt_checked[10]) {
  int t[10];
  int t2d[10][10];
  int u checked[10];
  int u2d checked[10][10]; // This is a checked array of checked arrays. checked propagates
                            // to immediately nested array types in array declarators.  It does
                            // not propagate through typedefs
  int x nt_checked[10];
  int x2d checked[10]nt_checked[10];

  // Single-dimensional array type conversions to pointer types.
  int *t1 = p;          // T *  = T[constant] OK
  int *t2 = q;          // T *  = T[] OK
  int *t3 = t;          // T *  = T[constant] OK;
  int *t4 = r;          // expected-error {{expression of incompatible type}} 
                        // Assignment of checked pointer to unchecked pointer not allowed
  int *t4a = v;         // expected-error {{expression of incompatible type}}
  int *t5 = s;          // expected-error {{expression of incompatible type}} 
                        // ditto
  int *t5a = w;         // expected-error {{expression of incompatible type}}
  int *t6 = u;          // expected-error {{expression of incompatible type 'int _Checked[10]'}}
                        // ditto
  int *t6a = x;         // expected-error {{expression of incompatible type 'int _Nt_checked[10]'}}
    
  // Various forms of array_ptr<T> = T[]. Note that the rhs does not need to have known bounds
  // because the lhs pointers have unknown bounds (and cannot be dereferenced).  
  //
  // Note if there need to be known bounds, the bounds of p and q are unknown
  // because C does not guarantee that array sizes match for parameter passing
  array_ptr<int> t7 = p;  
  array_ptr<int> t8 = q;
  array_ptr<int> t9 = r;
  array_ptr<int> t9a = v;
  array_ptr<int> t10  = s;
  array_ptr<int> t9b = w;
  array_ptr<int> t11 = t;
  array_ptr<int> t11a = u;
  array_ptr<int> t12 = u;
  nt_array_ptr<int> t12a = x;
  nt_array_ptr<int> t12b = u;      // expected-error {{expression of incompatible type 'int _Checked[10]'}}
  array_ptr<int> t13 = s2d[0];
  array_ptr<int> t13a = w2d[0];
  nt_array_ptr<int> t13b : bounds(unknown) = w2d[0];
  nt_array_ptr<int> t13c = s2d[0]; // expected-error {{expression of incompatible type 'int _Checked[10]'}}
  array_ptr<int> t14 = t2d[0];
  array_ptr<int> t15 = u2d[0];
  array_ptr<int> t15a = x2d[0];
  nt_array_ptr<int> t15b : bounds(unknown) = x2d[0];
  nt_array_ptr<int> t15c = u2d[0]; // expected-error {{expression of incompatible type 'int _Checked[10]'}}

  // Multi-dimensional array type conversions to pointer types.
  int *t16 = s2d[0];     // expected-error {{expression of incompatible type 'int _Checked[10]'}}
  int *t16a = w2d[0];    // expected-error {{expression of incompatible type 'int _Nt_checked[10]'}}
  int *t17 = t2d[0];
  int *t18 = u2d[0];     // expected-error {{expression of incompatible type 'int _Checked[10]'}}
  int *t18a = x2d[0];     // expected-error {{expression of incompatible type 'int _Nt_checked[10]'}}
  int(*t19)[10] = s2d;   // expected-error {{expression of incompatible type '_Array_ptr<int _Checked[10]>'}}
                         // assignment of checked array to unchecked array not allowed
  int(*t19a)[10] = w2d;  // expected-error {{expression of incompatible type '_Array_ptr<int _Nt_checked[10]>'}}
                         // assignment of checked array to unchecked array not allowed
  int (*t20)[10] = t2d;
  int (*t21)[10] = u2d;  // expected-error {{expression of incompatible type 'int _Checked[10][10]'}}
                         // assignment of checked array to unchecked array not allowed
  int(*t21a)[10] = x2d;  // expected-error {{expression of incompatible type 'int _Checked[10]_Nt_checked[10]'}}
                         // assignment of checked array to unchecked array not allowed
  array_ptr<int[10]> t22 = s2d; // expected-error {{expression of incompatible type '_Array_ptr<int _Checked[10]>'}}
                                // assignment of checked to unchecked not allowed
  array_ptr<int[10]> t22a = w2d; // expected-error {{expression of incompatible type '_Array_ptr<int _Nt_checked[10]>'}}
                                 // assignment of checked to unchecked not allowed
  array_ptr<int checked[10]> t22b = w2d; // expected-error {{expression of incompatible type '_Array_ptr<int _Nt_checked[10]>'}}
                                         // assignment of checked to unchecked not allowed
  array_ptr<int nt_checked[10]> t22c = s2d; // expected-error {{expression of incompatible type '_Array_ptr<int _Checked[10]>'}}
                                            // assignment of checked to unchecked not allowed
  array_ptr<int[10]> t23 = t2d;
  array_ptr<int[10]> t24 = u2d; // expected-error {{expression of incompatible type 'int _Checked[10][10]'}}
                                // assignment of checked to unchecked not allowed
  array_ptr<int[10]> t24a = x2d; // expected-error {{expression of incompatible type 'int _Checked[10]_Nt_checked[10]'}}
                                 // assignment of checked to unchecked not allowed
  array_ptr<int checked[10]> t24b = x2d; // expected-error {{expression of incompatible type 'int _Checked[10]_Nt_checked[10]'}}
                                 // assignment of checked to unchecked not allowed
  array_ptr<int nt_checked[10]> t24c = u2d; // expected-error {{expression of incompatible type 'int _Checked[10][10]'}}
                                         // assignment of checked to unchecked not allowed
  array_ptr<int checked[10]> t25 = s2d;
  array_ptr<int nt_checked[10]> t25a = w2d;
  array_ptr<int checked[10]> t26 = t2d;
  array_ptr<int nt_checked[10]> t26a = t2d; // expected-error {{expression of incompatible type 'int [10][10]'}}
  array_ptr<int checked[10]> t27 = u2d;

  // Assignments to array-typed parameters are allowed.  The outermost array modifier
  // is converted to a pointer type.
  p = q;
  q = p;
  s = r;
  r = t;
  w = x;
  p = r;  // expected-error {{assigning to 'int *' from incompatible type '_Array_ptr<int>'}}
          // assignment of checked pointer to unchecked pointer not allowed
  p = w;  // expected-error {{assigning to 'int *' from incompatible type '_Nt_array_ptr<int>'}}
          // assignment of checked pointer to unchecked pointer not allowed

  // Assignments to array-typed local and global variables are not allowed
  t = p;  // expected-error {{array type 'int [10]' is not assignable}}
  t = r;  // expected-error {{array type 'int [10]' is not assignable}}
  u = r;  // expected-error {{array type 'int _Checked[10]' is not assignable}}
  x = w;  // expected-error {{array type 'int _Nt_checked[10]' is not assignable}}
  global_arr = p; // expected-error {{array type 'int [10]' is not assignable}}
  global_arr = r; // expected-error {{array type 'int [10]' is not assignable}}
  global_checked_arr = r; // expected-error {{array type 'int _Checked[10]' is not assignable}}
}

// Test that dimensions in multi-dimensional arrays are either all checked or unchecked arrays.
extern void check_dimensions1(void) {
  int t1 checked[10][5]checked[5];     // multiple checked modifiers are allowed
  int t1a checked[10][5]nt_checked[5];
  int t2 checked[10][5][5]checked[5];

  // checked mixing of checked/unchecked array dimensions
  int t3[10]checked[10];               // expected-error {{unchecked array of checked array not allowed}}
  int t3a[10]nt_checked[10];           // expected-error {{unchecked array of checked array not allowed}}
  int t3b nt_checked[10]checked[10];   // expected-error {{only integer and pointer types allowed}}
  int t3c nt_checked[10][10];          // expected-error {{only integer and pointer types allowed}}
  typedef int dim_unchecked[10];
  dim_unchecked t4 checked[10];        // expected-error {{checked array of unchecked array not allowed \
('dim_unchecked' is an unchecked array)}}

  typedef int dim_checked checked[10];
  dim_checked t5[10];                  // expected-error {{unchecked array of checked array not allowed \
('dim_checked' is a checked array)}}

  // checked parenthesized declarators
  int (t6 checked[10])[10];            // checked propagates to enclosing array declarators
  int(t7 checked[10])[5][5]checked[5]; // multiple checked modifiers are allowed
  int (t8[10])checked[10];             // expected-error {{unchecked array of checked array not allowed}}
  int ((t9[10]))checked[10];           // expected-error {{unchecked array of checked array not allowed}}
  dim_unchecked(t10 checked[10])[10]; // expected-error {{checked array of unchecked array not allowed \
('dim_unchecked' is an unchecked array)}}
  dim_checked (t11[10])[10];           // expected-error {{unchecked array of checked array not allowed \
('dim_checked' is a checked array)}}

  // make sure checked-ness propagated
  int *t12 = t6[0];                    // expected-error {{expression of incompatible type 'int _Checked[10]'}}
  array_ptr<int> t13 = t6[0];
}

// Test that dimensions for incomplete array types are either all checked or unchecked arrays
extern void check_dimensions2(int r2d checked[][10] : count(len), int len) {
}

extern void check_dimensions2a(int r2d checked[]nt_checked[10] : count(len), int len) {
}

extern void check_dimensions3(int (r2d checked[])[10] : count(len), int len) {
}

extern void check_dimensions4(int r2d []checked[10] : count(len), int len) { // expected-error {{unchecked array of checked array not allowed}}
}

extern void check_dimensions4a(int r2d[]nt_checked[10] : count(len), int len) { // expected-error {{unchecked array of checked array not allowed}}
}

extern void check_dimensions5(int (r2d[])checked[10] : count(len), int len) { // expected-error {{unchecked array of checked array not allowed}}
}

// Test that qualifiers on the outermost dimension of a checked array-typed parameter are preserved.
extern void check_dimensions6(int r2d checked[const][10] : count(len), int len) {
  r2d = 0;          // expected-error {{cannot assign to variable 'r2d' with const-qualified type}}
  int *t1 = r2d[0]; // expected-error {{expression of incompatible type 'int _Checked[10]'}}
}

extern void check_dimensions7(int (r2d checked[const])[10] : count(len), int len) {
  r2d = 0;          // expected-error {{cannot assign to variable 'r2d' with const-qualified type}}
  int *t1 = r2d[0]; // expected-error {{expression of incompatible type 'int _Checked[10]'}}
}

extern void check_dimensions8(int (r2d) checked[const][10] : count(len), int len) {
  r2d = 0;          // expected-error {{cannot assign to variable 'r2d' with const-qualified type}}
  int *t1 = r2d[0]; // expected-error {{expression of incompatible type 'int _Checked[10]'}}
}


// Test assignments between pointers of different kinds with const/volatile
// attributes on referent types
extern void check_assign_cv(int param[10],
                            int checked_param checked[10],
                            int nt_checked_param nt_checked[10],
                            int param_const_ptr[const 10], 
                            int checked_param_const_ptr checked[const 10],
                            int nt_checked_param_const_ptr nt_checked[const 10],
                            const int const_param[10],
                            const int const_checked_param checked[10],
                            const int const_param_const_ptr[const 10],
                            const int const_checked_param_const_ptr checked[const 10],
                            volatile int volatile_param[10],
                            volatile int checked_volatile_param checked[10]) {
  int a[10];
  int b checked[10];
  // We only spot check a few cases for null-terminated array pointers.
  int c nt_checked[10];
  const int const_a[10];
  const int const_b checked[10];
  const int const_c nt_checked[10];
  volatile int volatile_a[10];

  int a_const_ptr[const 10]; // expected-error {{type qualifier used in array declarator}}

  //
  // check assignments to parameters
  //
  // assign an unchecked array,where the element type does not have modifiers
  param = a;
  checked_param = a;
  nt_checked_param = c;
  param_const_ptr = a;                // expected-error {{cannot assign to variable}}
  checked_param_const_ptr = a;        // expected-error {{cannot assign to variable}}
  nt_checked_param_const_ptr = c;     // expected-error {{cannot assign to variable}}
  const_param = a;
  const_checked_param = a;
  const_param_const_ptr = a;          // expected-error {{cannot assign to variable}}
  const_checked_param_const_ptr = a;  // expected-error {{cannot assign to variable}}
  volatile_param = a;
  checked_volatile_param = a;

  // assign a checked array, where the element type does not have modifiers
  param = b;                          // expected-error {{incompatible type}}
  checked_param = b;
  nt_checked_param = c;
  nt_checked_param = b;               // expected-error {{incompatible type}}
  param_const_ptr = b;                // expected-error {{cannot assign to variable}}
  checked_param_const_ptr = b;        // expected-error {{cannot assign to variable}}
  const_param = b;                    // expected-error {{incompatible type}}
  const_checked_param = b;
  const_param_const_ptr = b;          // expected-error {{cannot assign to variable}}
  const_checked_param_const_ptr = b;  // expected-error {{cannot assign to variable}}
  volatile_param = b;                 // expected-error {{incompatible type}}

  // check assigning an unchecked array where the element type has modifiers
  param = const_a;                    // expected-warning {{discards qualifiers}}
  checked_param = const_a;            // expected-warning {{discards qualifiers}}
  nt_checked_param = const_c;         // expected-warning {{discards qualifiers}}
  const_param = const_a;
  const_checked_param = const_a;
  volatile_param = const_a;           // expected-warning {{discards qualifiers}}
  checked_volatile_param = const_a;   // expected-warning {{discards qualifiers}}

  // spot check assigning a checked array where the element type has modifiers
  checked_param = const_b;            // expected-warning {{discards qualifiers}}
  const_checked_param = const_b;
  volatile_param = const_b;           // expected-error {{incompatible type}}
  checked_volatile_param = const_b;   // expected-warning {{discards qualifiers}}    

  //
  // check assignments to local variable pointers
  // 

  // the rhs is a parameter with array type
  int *p = 0;
  const int *p_const = 0;
  volatile int *p_volatile = 0;
  ptr<int> q = 0;
  ptr<const int> q_const = 0;
  ptr<volatile int> q_volatile = 0;
  array_ptr<int> r = 0;
  array_ptr<const int> r_const = 0;
  array_ptr<volatile int> r_volatile = 0;

  // the rhs is an unchecked array where the element type does not have modifiers
  p = param;
  p_const = param;
  p_volatile = param;
  // q = param;          not allowed: param has unknown bounds
  // q_const = param;    not allowed: param has unknown bounds
  // q_volatile = param; not allowed: param has unknown bounds
  r = param;
  r_const = param;
  r_volatile = param;

  // the rhs is a checked array where the element type does not have modifiers
  p = checked_param;              // expected-error {{incompatible type}}
  p_const = checked_param;        // expected-error {{incompatible type}}
  p_volatile = checked_param;     // expected-error {{incompatible type}} 
  q = checked_param;              // ptr<T> = array_ptr<T> OK
  q_const = checked_param;        // ptr<T> = array_ptr<T> OK
  q_volatile = checked_param;     // ptr<T> = array_ptr<T> OK
  r = checked_param;
  r_const = checked_param;
  r_volatile = checked_param;

  // the rhs is an unchecked array where the element type has modifiers
  p = const_param;              // expected-warning {{discards qualifiers}}
  p_const = const_param;
  p_volatile = const_param;     // expected-warning {{discards qualifiers}}  
  // q = const_param;           not allowed: param has unknown bounds
  // q_const = const_param;     not allowed: param has unknown bounds
  // q_volatile = const_param;  not allowed: param has unknown bounds
  r = const_param;              // expected-warning {{discards qualifiers}}
  r_const = const_param;
  r_volatile = const_param;     // expected-warning {{discards qualifiers}}

  // the rhs is an checked array where the element type has modifiers  
  q = const_checked_param;      // expected-warning {{discards qualifiers}}
                                // ptr<T> = array_ptr<T> OK 
  r = const_checked_param;      // expected-warning {{discards qualifiers}}
  r_const = const_checked_param;
  r_volatile = const_checked_param; // expected-warning {{discards qualifiers}}

  //
  // the rhs is a local array
  // 

  // the rhs is an unchecked array where the element type does not have modifiers
  p = a;
  p_const = a;
  p_volatile = a;
  q = a;
  q_const = a;
  q_volatile = a;
  r = a;
  r_const = a;

  // the rhs is an checked array where the element type does not have modifiers
  p = b;              // expected-error {{incompatible type}}
  p_const = b;        // expected-error {{incompatible type}}
  p_volatile = b;     // expected-error {{incompatible type}} 
  q = b;              // ptr<T> = array_ptr<T> OK
  q_const = b;        // ptr<T> = array_ptr<T> OK
  q_volatile = b;     // ptr<T> = array_ptr<T> OK
  r = b;
  r_const = b;
  r_volatile = b;

  // the rhs is an unchecked array where the element type has modifiers
  p = const_a;              // expected-warning {{discards qualifiers}}
  p_const = const_a;
  p_volatile = const_a;     // expected-warning {{discards qualifiers}}  
  q = const_a;              // expected-warning {{discards qualifiers}}
  q_const = const_a;
  q_volatile = const_a;     // expected-warning {{discards qualifiers}}
  r = const_a;              // expected-warning {{discards qualifiers}}
  r_const = const_a;
  r_volatile = const_a;     // expected-warning {{discards qualifiers}}

  // the rhs is an checked array where the element type has modifiers  
  q = const_b;      // expected-warning {{discards qualifiers}}
                    // ptr<T> = array_ptr<T> OK
  r = const_b;      // expected-warning {{discards qualifiers}}
  r_const = const_b;
  r_volatile = const_b; // expected-warning {{discards qualifiers}}
}

// Test conditional expressions where arms have different
// kinds of checked and unchecked arrays.
extern void check_condexpr(int val) {
  int p[5];
  int r checked[5];
  float s[5];
  float u checked[5];
  int v nt_checked[6];

  // N and M are arbitrary positive integer constants below.
  int *t1 = val ? p : p;            // T[N] and T[M] OK;
  array_ptr<int> t2 = val ? p : r;  // T[N] and T checked[M] OK
  array_ptr<int> t2a = val ? p : v; // T[N] and T nt_checked[M] OK.
  nt_array_ptr<int> t2b = val ? p : v; // expected-error {{incompatible type}}
                                       // T[N] and T nt_checked[M] produce
                                       // only array_ptr.
  array_ptr<int> t3 = val ? r : p;  // T checked[N] and T[M] OK
  array_ptr<int> t3a = val ? v : p; // T nt_checked[N] and T[M] OK.
  nt_array_ptr<int> t3b = val ? v : p; // expected-error {{incompatible type}}
                                       // But they produce only array_ptr.
  array_ptr<int> t4 = val ? r : r;  // T checked[N] and T checked[M] OK.
  array_ptr<int> t4a = val ? r : v;  // T checked[N] and T nt_checked[M] OK.
  array_ptr<int> t4b = val ? v : r;  // T nt_checked[N] and T checked[M] OK.
  nt_array_ptr<int> t4c = val ? r : v;  // expected-error {{incompatible type}}
                                        // But they produce only array_ptr.
  nt_array_ptr<int> t4d = val ? v : r;  // expected-error {{incompatible type}}
                                        // But they produce only array_ptr
  nt_array_ptr<int> t4e = val ? v : v;  // T nt_checked[M] and T nt_checked[N] OK.

  // omit assignment because type of expression is not valid when there is an error.
  val ? s : r;     // expected-error {{pointer type mismatch}}
                   // S[5] and T checked[5] not OK;
  val ? r : s;     // expected-error {{pointer type mismatch}}
                   // T checked[5] and S[5] not OK.
  val ? r : u;     // expected-error {{pointer type mismatch}}
                    // T checked[5] and S checked[5] not OK
  val ? u : r;     // expected-error {{pointer type mismatch}}
                   // S checked[5] and T checked[5] not OK

  // Some C compilers have allowed implicit integer to pointer conversions.
  // These are errors for the new safe pointer types.
  val ? r : val;   // expected-error {{incompatible operand types}}
                   // T checked[5] and int not OK
  val ? val : r;   // expected-error {{incompatible operand types}}
                   // int and T checked[5] not OK
  val ? u : val;   // expected-error {{incompatible operand types}}
                   // T checked[5] and int not OK
  val ? val : u;   // expected-error {{incompatible operand types}}
                    // int and T checked[5] not OK

  // Implicit conversion of 0 to a safe pointer type is OK.
  array_ptr<int> t5 = val ? r : 0;
  array_ptr<int> t5a = val ? v : 0;
  array_ptr<int> t6 = val ? 0 : r;
  nt_array_ptr<int> t6a = val ? 0 : v; // expected-warning {{cannot prove declared bounds for 't6a' are valid after initialization}}
  array_ptr<float> t7 = val ? u : 0;
  array_ptr<float> t8 = val ? 0 : u;
}

extern void check_condexpr_2d(int val) {
  int p[5][6];
  int r checked[5][6];
  float s[5][6];
  float u checked[5][6];
  int y checked[5][20];
  int z checked[5]nt_checked[6];

  int (*t1)[6] = val ? p : p;                  // T[5][6] and T[5][6] OK;
  array_ptr<int checked[6]> t2 = val ? p : r;  // T[5][6] and T checked[5][6] OK
  array_ptr<int checked[6]> t3 = val ? r : p;  // T checked[5][6] and T[5][6] OK
  array_ptr<int checked[6]> t4 = val ? r : r;  // T checked[5][6] and T checked[5][6] OK.
  array_ptr<int nt_checked[6]> t4a = val ? z : z;  // T checked[5][6] and T checked[5][6] OK.

  array_ptr<int [6]> t5 = val ? p : r;  // expected-error {{incompatible type}}
                                        // T[5][6] and T checked[5][6] produce a checked array
  array_ptr<int [6]> t6 = val ? r : p;  // expected-error {{incompatible type}}
                                        // T checked[5][6] and T[5][6] produce a checked array
  array_ptr<int [6]> t7 = val ? r : r;  // expected-error {{incompatible type}}
                                        // T checked[5][6] and T checked[5][6] produce a checked array
  array_ptr<int[6]> t7a = val ? z : z;  // expected-error {{incompatible type}}
                                        // T _Nt_checked[5][6] and T _Nt_checked[5][6] produce an nt_checked array
  array_ptr<int checked[6]> t7b = val ? z : z; // expected-error {{incompatible type}}
                                       // T checked[5]_Nt_checked[6] and T checked[5]_Nt_checked[6] produce an nt_checked array

  // omit assignment because type of expression is not valid when there is an error.
  val ? s : r;     // expected-error {{pointer type mismatch}}
                    // S[5][6] and T checked[5][6] not OK;
  val ? r : s;     // expected-error {{pointer type mismatch}}
                    // T checked[5][6] and S[5][6] not OK.
  val ? r : u;     // expected-error {{pointer type mismatch}}
                    // T checked[5][6] and S checked[5][6] not OK
  val ? u : r;     // expected-error {{pointer type mismatch}}
                    // S checked[5][6] and T checked[5][6] not OK

                    // Some C compilers have allowed implicit integer to pointer conversions.
                    // These are errors for the new safe pointer types.
  val ? r : val;   // expected-error {{incompatible operand types}}
                    // T checked[5][6] and int not OK
  val ? val : r;   // expected-error {{incompatible operand types}}
                    // int and T checked[5][6] not OK
  val ? u : val;   // expected-error {{incompatible operand types}}
                    // T checked[5][6] and int not OK
  val ? val : u;   // expected-error {{incompatible operand types}}
                    // int and T checked[5][6] not OK

  // check that mismatching dimension sizes for the 2nd dimension cause
  // a typechecking error.
  val ? r : y;      // expected-error {{pointer type mismatch}}
                    // different dimension sizes are not OK

  // check that mismatching checkedness of inner arrays causes an error.
  val ? p : z;   // expected-error {{pointer type mismatch}}
  val ? z : p;   // expected-error {{pointer type mismatch}}
  val ? r : z;   // expected-error {{pointer type mismatch}}
  val ? z : r;   // expected-error {{pointer type mismatch}}


  // Implicit conversion of 0 to a checked pointer type is OK.
  array_ptr<int checked[6]> t11 = val ? r : 0;
  array_ptr<int checked[6]> t12 = val ? 0 : r;
  array_ptr<float checked[6]> t15 = val ? u : 0;
  array_ptr<float checked[6]> t16 = val ? 0 : u;
}

// Test conditional expressions where arms have different kinds of
// array types and const/volatile modifiers.
extern void check_condexpr_cv(void)
{
  int val = 0;
  int p[5];
  const int p_const[5] = { 0, 1, 2, 3, 4};
  volatile int p_volatile[5];
  int r checked[5];
  const int r_const checked[5] = { 0, 1, 2, 3, 4};
  volatile int r_volatile checked[5];
  int s nt_checked[5];
  const int s_const nt_checked[5] = { 0, 1, 2, 3, 0 };
  volatile int s_volatile nt_checked[5];

  // test different kinds of pointers with const modifiers
  const int *t1 = val ? p : p_const;       // int * and const int * OK
  const int *t2 = val ? p_const : p;       // const int * and int * OK
  const int *t3 = val ? p_const : p_const; // const int * and const int * OK

  int *t4 = val ? p : p_const;             // expected-warning {{discards qualifiers}}
                                           // int * and const int * produce const int *
  int *t5 = val ? p_const : p;             // expected-warning {{discards qualifiers}}
                                           // const int * and int * produce const int *
  int *t6 = val ? p_const : p_const;       // expected-warning {{discards qualifiers}}
                                           // const int * and const int * produce const int *

  array_ptr<const int> t25 = val ? p : r_const;       // int * and array_ptr<const int> OK
  array_ptr<const int> t26 = val ? r_const : p;       // array_ptr<const int> and int * OK
  array_ptr<const int> t27 = val ? p_const : r;       // const int * and array_ptr<int> OK
  array_ptr<const int> t28 = val ? r : p_const;       // array_ptr<int> and const int * OK
  array_ptr<const int> t29 = val ? p_const : r_const; // const int * and array_ptr<const int> OK
  array_ptr<const int> t30 = val ? r_const : p_const; // array_ptr<const int> and const int * OK
  array_ptr<const int> t31 = val ? r : r_const;       // array_ptr<int> and array_ptr<const int> OK
  array_ptr<const int> t32 = val ? r_const : r;       // array_ptr<const int> and array_ptr<int> OK
  array_ptr<const int> t33 = val ? r_const : r_const; // array_ptr<const int> and array_ptr<const int> OK

  array_ptr<const int> t25a = val ? p : s_const;       // int * and nt_array_ptr<const int> OK
  array_ptr<const int> t26a = val ? s_const : p;       // nt_array_ptr<const int> and int * OK
  array_ptr<const int> t27a = val ? p_const : s;       // const int * and nt_array_ptr<int> OK
  array_ptr<const int> t28a = val ? s : p_const;       // nt_array_ptr<int> and const int * OK
  array_ptr<const int> t29a = val ? p_const : s_const; // const int * and nt_array_ptr<const int> OK
  array_ptr<const int> t30a = val ? s_const : p_const; // nt_array_ptr<const int> and const int * OK
  array_ptr<const int> t31a = val ? s : r_const;       // nt_array_ptr<int> and array_ptr<const int> OK
  array_ptr<const int> t32a = val ? r_const : s;       // array_ptr<const int> and nt_array_ptr<int> OK
  array_ptr<const int> t33a = val ? s_const : r_const; // nt_array_ptr<const int> and array_ptr<const int> OK
  array_ptr<const int> t33b = val ? r_const : s_const; // array_ptr<const int> and nt_array_ptr<const int> OK
  array_ptr<const int> t33c = val ? s : s_const;       // nt_array_ptr<int> and nt_array_ptr<const int> OK
  array_ptr<const int> t33d = val ? s_const : s;       // nt_array_ptr<const int> and nt_array_ptr<int> OK
  array_ptr<const int> t33e = val ? s_const : s_const; // nt_array_ptr<const int> and nt_array_ptr<const int> OK

  array_ptr<int> t34 = val ? p : r_const;   // expected-warning {{discards qualifiers}}
                                            // int * and array_ptr<const int> produce array_ptr<const int>
  array_ptr<int> t35 = val ? r_const : p;   // expected-warning {{discards qualifiers}}
                                            // array_ptr<const int> and int * produce array_ptr<const int>
  array_ptr<int> t36 = val ? p_const : r;   // expected-warning {{discards qualifiers}}
                                            // const int * and array_ptr<int> produce array_ptr<const int>
  array_ptr<int> t37 = val ? r : p_const;   // expected-warning {{discards qualifiers}}
                                            // array_ptr<int> and const int * produce array_ptr<const int>
  array_ptr<int> t38 = val ? p_const : r_const;   // expected-warning {{discards qualifiers}}
                                            // const int * and array_ptr<const int> produce array_ptr<const int>
  array_ptr<int> t39 = val ? r_const : p_const;   // expected-warning {{discards qualifiers}}
                                            // array_ptr<const int> and const int * produce array_ptr<const int>
  array_ptr<int> t40 = val ? r : r_const;   // expected-warning {{discards qualifiers}}
                                            // array_ptr<int> and array_ptr<const int> produce array_ptr<const int>
  array_ptr<int> t41 = val ? r_const : r;   // expected-warning {{discards qualifiers}}
                                            // array_ptr<const int> and array_ptr<int> produce array_ptr<const int>
  array_ptr<int> t42 = val ? r_const : r_const;   // expected-warning {{discards qualifiers}}
                                            // array_ptr<const int> and array_ptr<const int> produce array_ptr<const int>

  array_ptr<int> t34a = val ? p : s_const;  // expected-warning {{discards qualifiers}}
                                            // int * and nt_array_ptr<const int> produce array_ptr<const int>
  array_ptr<int> t35a = val ? s_const : p;  // expected-warning {{discards qualifiers}}
                                            // nt_array_ptr<const int> and int * produce array_ptr<const int>
  array_ptr<int> t36a = val ? p_const : s;  // expected-warning {{discards qualifiers}}
                                            // const int * and nt_array_ptr<int> produce array_ptr<const int>
  array_ptr<int> t37a= val ? s : p_const;   // expected-warning {{discards qualifiers}}
                                            // nt_array_ptr<int> and const int * produce array_ptr<const int>
  array_ptr<int> t38a = val ? p_const : s_const;  // expected-warning {{discards qualifiers}}
                                                  // const int * and nt_array_ptr<const int> produce array_ptr<const int>
  array_ptr<int> t39a = val ? s_const : p_const;  // expected-warning {{discards qualifiers}}
                                                  // nt_array_ptr<const int> and const int * produce array_ptr<const int>
  array_ptr<int> t40a = val ? s : r_const;  // expected-warning {{discards qualifiers}}
                                            // nt_array_ptr<int> and array_ptr<const int> produce array_ptr<const int>
  array_ptr<int> t41a = val ? r_const : s;  // expected-warning {{discards qualifiers}}
                                            // array_ptr<const int> and nt_array_ptr<int> produce array_ptr<const int>
  array_ptr<int> t42a = val ? r_const : s_const;  // expected-warning {{discards qualifiers}}
                                                  // array_ptr<const int> and nt_array_ptr<const int> produce array_ptr<const int>
  array_ptr<int> t43a = val ? s_const : r_const;  // expected-warning {{discards qualifiers}}
                                                  // nt_array_ptr<const int> and array_ptr<const int> produce array_ptr<const int>
  array_ptr<int> t43b = val ? r_const : s_const; // expected-warning {{discards qualifiers}}
                                                 // array_ptr<const int> and nt_array_ptr<const int> produce array_ptr<const int>
  array_ptr<int> t43c = val ? s : s_const;       // expected-warning {{discards qualifiers}}
                                                 // nt_array_ptr<int> and nt_array_ptr<const int> produce array_ptr<const int>
  array_ptr<int> t43d = val ? s_const : s;       // expected-warning {{discards qualifiers}}
                                                 // nt_array_ptr<const int> and nt_array_ptr<int> produce array_ptr<const int>
  array_ptr<int> t42e = val ? s_const : s_const; // expected-warning {{discards qualifiers}}
                                                 // nt_array_ptr<const int> and nt_array_ptr<const int> produce array_ptr<const int>

  // test different kinds of pointers with volatile modifers
  volatile int *t50 = val ? p : p_volatile;          // int * and volatile int * OK
  volatile int *t51 = val ? p_volatile : p;          // volatile int * and int * OK
  volatile int *t52 = val ? p_volatile : p_volatile; // volatile int * and volatile int * OK

  int *t53 = val ? p : p_volatile;                   // expected-warning {{discards qualifiers}}
                                                     // int * and volatile int * produce volatile int *
  int *t54 = val ? p_volatile : p;                   // expected-warning {{discards qualifiers}}
                                                     // volatile int * and int * produce volatile int *
  int *t55 = val ? p_volatile : p_volatile;          // expected-warning {{discards qualifiers}}
                                                     // volatile int * and volatile int * produce volatile int *

  array_ptr<volatile int> t74 = val ? p : r_volatile;          // int * and array_ptr<volatile int> OK
  array_ptr<volatile int> t75 = val ? r_volatile : p;          // array_ptr<volatile int> and int * OK
  array_ptr<volatile int> t76 = val ? p_volatile : r;          // volatile int * and array_ptr<int> OK
  array_ptr<volatile int> t77 = val ? r : p_volatile;          // array_ptr<int> and volatile int * OK
  array_ptr<volatile int> t78 = val ? p_volatile : r_volatile; // volatile int * and array_ptr<volatile int> OK
  array_ptr<volatile int> t79 = val ? r_volatile : p_volatile; // array_ptr<volatile int> and volatile int * OK
  array_ptr<volatile int> t80 = val ? r : r_volatile;          // array_ptr<int> and array_ptr<volatile int> OK
  array_ptr<volatile int> t81 = val ? r_volatile : r;          // array_ptr<volatile int> and array_ptr<int> OK
  array_ptr<volatile int> t82 = val ? r_volatile : r_volatile; // array_ptr<volatile int> and array_ptr<volatile int> OK

  array_ptr<volatile int> t74a = val ? p : s_volatile;          // int * and nt_array_ptr<volatile int> OK
  array_ptr<volatile int> t75a = val ? s_volatile : p;          // nt_array_ptr<volatile int> and int * OK
  array_ptr<volatile int> t76a = val ? p_volatile : s;          // volatile int * and nt_array_ptr<int> OK
  array_ptr<volatile int> t77a = val ? s : p_volatile;          // nt_array_ptr<int> and volatile int * OK
  array_ptr<volatile int> t78a = val ? p_volatile : s_volatile; // volatile int * and nt_array_ptr<volatile int> OK
  array_ptr<volatile int> t79a = val ? s_volatile : p_volatile; // nt_array_ptr<volatile int> and volatile int * OK
  array_ptr<volatile int> t80a = val ? s : r_volatile;          // nt_array_ptr<int> and nt_array_ptr<volatile int> OK
  array_ptr<volatile int> t81a = val ? r_volatile : s;          // nt_array_ptr<volatile int> and nt_array_ptr<int> OK
  array_ptr<volatile int> t82a = val ? r_volatile : s_volatile;  // array_ptr<volatile int> and nt_array_ptr<volatile int> OK
  array_ptr<volatile int> t82b = val ? s_volatile : r_volatile; // nt_array_ptr<volatile int> and array_ptr<volatile int> OK
  array_ptr<volatile int> t82c = val ? s : s_volatile;          // nt_array_ptr<int> and nt_array_ptr<volatile int> OK
  array_ptr<volatile int> t82d = val ? s_volatile : s;          // nt_array_ptr<volatile int> and nt_array_ptr<int> OK
  array_ptr<volatile int> t82e = val ? s_volatile : s_volatile; // nt_array_ptr<volatile int> and nt_array_ptr<volatile int> OK

  array_ptr<int> t83 = val ? p : r_volatile;          // expected-warning {{discards qualifiers}}
                                                      // int * and array_ptr<volatile int> produce array_ptr<volatile int>
  array_ptr<int> t84 = val ? r_volatile : p;          // expected-warning {{discards qualifiers}}
                                                      // array_ptr<volatile int> and int * produce array_ptr<volatile int>
  array_ptr<int> t85 = val ? p_volatile : r;          // expected-warning {{discards qualifiers}}
                                                      // volatile int * and array_ptr<int> produce array_ptr<volatile int>
  array_ptr<int> t86 = val ? r : p_volatile;          // expected-warning {{discards qualifiers}}
                                                      // array_ptr<int> and volatile int * produce array_ptr<volatile int>
  array_ptr<int> t87 = val ? p_volatile : r_volatile; // expected-warning {{discards qualifiers}}
                                                      // volatile int * and array_ptr<volatile int> produce array_ptr<volatile int>
  array_ptr<int> t88 = val ? r_volatile : p_volatile; // expected-warning {{discards qualifiers}}
                                                      // array_ptr<volatile int> and volatile int * produce array_ptr<volatile int>
  array_ptr<int> t89 = val ? r : r_volatile;          // expected-warning {{discards qualifiers}}
                                                      // array_ptr<int> and array_ptr<volatile int> produce array_ptr<volatile int>
  array_ptr<int> t90 = val ? r_volatile : r;          // expected-warning {{discards qualifiers}}
                                                      // array_ptr<volatile int> and array_ptr<int> produce array_ptr<volatile int>
  array_ptr<int> t92 = val ? r_volatile : r_volatile;  // expected-warning {{discards qualifiers}}
                                                       // array_ptr<volatile int> and array_ptr<volatile int> produce array_ptr<volatile int>

  array_ptr<int> t83a = val ? p : s_volatile;          // expected-warning {{discards qualifiers}}
                                                       // int * and nt_array_ptr<volatile int> produce array_ptr<volatile int>
  array_ptr<int> t84a = val ? s_volatile : p;          // expected-warning {{discards qualifiers}}
                                                       // mt+array_ptr<volatile int> and int * produce array_ptr<volatile int>
  array_ptr<int> t85a = val ? p_volatile : s;          // expected-warning {{discards qualifiers}}
                                                       // volatile int * and nt_array_ptr<int> produce array_ptr<volatile int>
  array_ptr<int> t86a = val ? s : p_volatile;          // expected-warning {{discards qualifiers}}
                                                       // nt_array_ptr<int> and volatile int * produce array_ptr<volatile int>
  array_ptr<int> t87a = val ? p_volatile : s_volatile; // expected-warning {{discards qualifiers}}
                                                       // volatile int * and nt_array_ptr<volatile int> produce array_ptr<volatile int>
  array_ptr<int> t88a = val ? s_volatile : p_volatile; // expected-warning {{discards qualifiers}}
                                                       // nt_array_ptr<volatile int> and volatile int * produce array_ptr<volatile int>
  array_ptr<int> t89a = val ? s : r_volatile;          // expected-warning {{discards qualifiers}}
                                                       // nt_array_ptr<int> and array_ptr<volatile int> produce array_ptr<volatile int>
  array_ptr<int> t90a = val ? r_volatile : s;          // expected-warning {{discards qualifiers}}
                                                       // array_ptr<volatile int> and nt_array_ptr<int> produce array_ptr<volatile int>
  array_ptr<int> t92a = val ? s : r_volatile;          // expected-warning {{discards qualifiers}}
                                                       // nt_array_ptr<volatile int> and array_ptr<volatile int> produce array_ptr<volatile int>
  array_ptr<int> t92b = val ? r_volatile : s_volatile; // expected-warning {{discards qualifiers}}
                                                        // array_ptr<volatile int> and nt_array_ptr<volatile int> produce array_ptr<volatile int>
  array_ptr<int> t92c = val ? s : s_volatile;          // expected-warning {{discards qualifiers}}
                                                       // nt_array_ptr<int> and nt_array_ptr<volatile int> produce array_ptr<volatile int>
  array_ptr<int> t92d = val ? s_volatile : s;          // expected-warning {{discards qualifiers}}
                                                       // nt_array_ptr<volatile int> and nt_array_ptr<int> produce array_ptr<volatile int>
  array_ptr<int> t92e = val ? s_volatile : s_volatile; // expected-warning {{discards qualifiers}}
                                                       // nt_array_ptr<volatile int> and nt_array_ptr<volatile int> produce array_ptr<volatile int>
}

// Define functions used to test typechecking of call expressions.

extern void f1(int *p, int y) {
}

extern void f2(int p[10], int y) {
}

extern void f3(int p checked[10], int y) {
  *p = y;
}

extern void f3a(int p nt_checked[10], int y) {
  *p = y;
}

extern void f4(int **p, int y) {
}

extern void f5(int(*p)[10], int y) {
}

extern void f6(ptr<int[10]> p, int y) {
}

extern void f7(array_ptr<int[10]> p, int y) {
}

extern void f8(int(*p) checked[10], int y) {
}

extern void f8a(int(*p) nt_checked[10], int y) {
}

extern void f9(ptr<int checked[10]> p, int y) {
}

extern void f9a(ptr<int nt_checked[10]> p, int y) {
}

extern void f10(array_ptr<int checked[10]> p, int y) {
}

extern void f10a(array_ptr<int nt_checked[10]> p, int y) {
}

extern void f11(int p[10][10], int y) {
}

extern void f12(int p checked[10][10],int y) {
}

extern void f12a(int p checked[10]nt_checked[10], int y) {
}

extern void f13(_Bool p, int y) {
}

extern void f1_void(void *p, int y) {
}

extern void f2_void(ptr<void> p, int y) {
}

extern void f3_void(array_ptr<void> p, int y) {
}

extern void f1_const(const int p[10], int y) {
}

extern void f2_const(const int p checked[10], int y) {
}

extern void f2a_const(const int p checked[10], int y) {
}

// Spot check second parameter whose type invovles an array
//

extern void g1(int y, int *p) {
}

extern void g2(int y, int p[10]) {
  *p = y;
}

extern void g3(int y, int p checked[10]) {
  *p = y;
}

extern void g3a(int y, int p nt_checked[10]) {
  *p = y;
}

extern void check_call(void) {
  int x[10];
  int y checked[10];
  int x2d[10][10];
  int y2d checked[10][10];
  int z nt_checked[10];
  int z2d checked[10]nt_checked[10];


  // f1(int *p, int y)
  f1(x, 0);
  f1(y, 0);              // expected-error {{parameter of incompatible type 'int *'}}
  f1(z, 0);              // expected-error {{parameter of incompatible type 'int *'}}
  f1(x2d, 0);            // expected-warning {{incompatible pointer types passing}}
  f1(y2d, 0);            // expected-error {{passing 'int _Checked[10][10]' to parameter of incompatible type 'int *'}}
  f1(z2d, 0);            // expected-error {{passing 'int _Checked[10]_Nt_checked[10]' to parameter of incompatible type 'int *'}}

  // f2(int p[10], int y)
  f2(x, 0);
  f2(y, 0);              // expected-error {{parameter of incompatible type 'int *'}}
  f2(z, 0);              // expected-error {{parameter of incompatible type 'int *'}}
  f2(x2d, 0);            // expected-warning {{incompatible pointer types passing}}
  f2(y2d, 0);            // expected-error {{passing 'int _Checked[10][10]' to parameter of incompatible type 'int *'}}
  f2(z2d, 0);            // expected-error {{passing 'int _Checked[10]_Nt_checked[10]' to parameter of incompatible type 'int *'}}

  // f3(int p checked[10], int y)
  f3(x, 0);
  f3(y, 0);
  f3(z, 0);              // expected-error {{argument does not meet declared bounds for 1st parameter}}
  f3(x2d, 0);            // expected-error {{parameter of incompatible type}}
  f3(y2d, 0);            // expected-error {{passing 'int _Checked[10][10]' to parameter of incompatible type '_Array_ptr<int>'}}
  f3(z2d, 0);            // expected-error {{passing 'int _Checked[10]_Nt_checked[10]' to parameter of incompatible type '_Array_ptr<int>'}}

  // f3a(int p nt_checked[10], int y)
  f3a(x, 0);             // expected-error {{passing 'int [10]' to parameter of incompatible type '_Nt_array_ptr<int>'}}
  f3a(y, 0);             // expected-error {{passing 'int _Checked[10]' to parameter of incompatible type '_Nt_array_ptr<int>'}}
  f3a(z, 0);
  f3a(x2d, 0);            // expected-error {{parameter of incompatible type}}
  f3a(y2d, 0);            // expected-error {{passing 'int _Checked[10][10]' to parameter of incompatible type '_Nt_array_ptr<int>'}}
  f3a(z2d, 0);            // expected-error {{passing 'int _Checked[10]_Nt_checked[10]' to parameter of incompatible type '_Nt_array_ptr<int>'}}


  // f4(int **p, int y);
  f4(x, 0);              // expected-warning {{incompatible pointer types passing}}
  f4(y, 0);              // expected-error {{passing 'int _Checked[10]' to parameter of incompatible type 'int **'}}
  f4(z, 0);              // expected-error {{passing 'int _Nt_checked[10]' to parameter of incompatible type 'int **'}}
  f4(x2d, 0);            // expected-warning {{incompatible pointer types passing}}
  f4(y2d, 0);            // expected-error {{passing 'int _Checked[10][10]' to parameter of incompatible type 'int **'}}
  f4(z2d, 0);            // expected-error {{passing 'int _Checked[10]_Nt_checked[10]' to parameter of incompatible type 'int **'}}

  // f5(int (*p)[10], int y);
  f5(x, 0);              // expected-warning {{incompatible pointer types passing}}
  f5(y, 0);              // expected-error {{passing 'int _Checked[10]' to parameter of incompatible type 'int (*)[10]'}}
  f5(z, 0);              // expected-error {{passing 'int _Nt_checked[10]' to parameter of incompatible type 'int (*)[10]'}}
  f5(x2d, 0);            // OK
  f5(y2d, 0);            // expected-error {{passing 'int _Checked[10][10]' to parameter of incompatible type 'int (*)[10]'}}
  f5(z2d, 0);            // expected-error {{passing 'int _Checked[10]_Nt_checked[10]' to parameter of incompatible type 'int (*)[10]'}}

   // f6(ptr<int[10]>, int y);
  f6(x, 0);              // expected-error {{parameter of incompatible type}}
  f6(y, 0);              // expected-error {{passing 'int _Checked[10]' to parameter of incompatible type '_Ptr<int [10]>'}}
  f6(z, 0);              // expected-error {{passing 'int _Nt_checked[10]' to parameter of incompatible type '_Ptr<int [10]>'}}
  f6(x2d, 0);            // OK
  f6(y2d, 0);            // expected-error {{passing 'int _Checked[10][10]' to parameter of incompatible type '_Ptr<int [10]>'}}
  f6(z2d, 0);            // expected-error {{passing 'int _Checked[10]_Nt_checked[10]' to parameter of incompatible type '_Ptr<int [10]>'}}

   // f7(array_ptr<int[10]>, int y);
  f7(x, 0);              // expected-error {{parameter of incompatible type}}
  f7(y, 0);              // expected-error {{parameter of incompatible type}}
  f7(z, 0);              // expected-error {{parameter of incompatible type}}
  f7(x2d, 0);            // OK
  f7(y2d, 0);            // expected-error {{parameter of incompatible type}}
  f7(z2d, 0);            // expected-error {{parameter of incompatible type}}

  // f8(int (*p) checked[10], int y);
  f8(x, 0);              // expected-error {{parameter of incompatible type}}
  f8(y, 0);              // expected-error {{parameter of incompatible type}}
  f8(z, 0);              // expected-error {{parameter of incompatible type}}
  f8(x2d, 0);            // OK
  f8(y2d, 0);            // expected-error {{parameter of incompatible type}}
  f8(z2d, 0);            // expected-error {{parameter of incompatible type}}

  // f8a(int (*p) nt_checked[10], int y);
  f8a(x, 0);              // expected-error {{parameter of incompatible type}}
  f8a(y, 0);              // expected-error {{parameter of incompatible type}}
  f8a(z, 0);              // expected-error {{parameter of incompatible type}}
  f8a(x2d, 0);            // expected-error {{parameter of incompatible type}}
  f8a(y2d, 0);            // expected-error {{parameter of incompatible type}}
  f8a(z2d, 0);            // expected-error {{parameter of incompatible type}}

  // f9(ptr<int checked[10]> p, int y);
  f9(x, 0);              // expected-error {{parameter of incompatible type}}
  f9(y, 0);              // expected-error {{parameter of incompatible type}}
  f9(z, 0);              // expected-error {{parameter of incompatible type}}
  f9(x2d, 0);            // OK
  f9(y2d, 0);            // OK
  f9(z2d, 0);            // expected-error {{parameter of incompatible type}}

  // f9a(ptr<int nt_checked[10]> p, int y);
  f9a(x, 0);              // expected-error {{parameter of incompatible type}}
  f9a(y, 0);              // expected-error {{parameter of incompatible type}}
  f9a(z, 0);              // expected-error {{parameter of incompatible type}}
  f9a(x2d, 0);            // expected-error {{parameter of incompatible type}}
  f9a(y2d, 0);            // expected-error {{parameter of incompatible type}}
  f9a(z2d, 0);            // OK

  // f10(array_ptr<int checked[10]> p, int y);
  f10(x, 0);              // expected-error {{parameter of incompatible type}}
  f10(y, 0);              // expected-error {{parameter of incompatible type}}
  f10(z, 0);              // expected-error {{parameter of incompatible type}}
  f10(x2d, 0);            // OK
  f10(y2d, 0);            // OK
  f10(z2d, 0);            // expected-error {{parameter of incompatible type}}

  // f10a(array_ptr<int nt_checked[10]> p, int y);
  f10a(x, 0);              // expected-error {{parameter of incompatible type}}
  f10a(y, 0);              // expected-error {{parameter of incompatible type}}
  f10a(z, 0);              // expected-error {{parameter of incompatible type}}
  f10a(x2d, 0);            // expected-error {{parameter of incompatible type}}
  f10a(y2d, 0);            // expected-error {{parameter of incompatible type}}
  f10a(z2d, 0);            // OK.

  // f11(int p[10][10], int y);
  f11(x, 0);              // expected-warning {{incompatible pointer types}}
  f11(y, 0);              // expected-error {{parameter of incompatible type}}
  f11(z, 0);              // expected-error {{parameter of incompatible type}}
  f11(x2d, 0);            // OK
  f11(y2d, 0);            // expected-error {{parameter of incompatible type}}
  f11(z2d, 0);            // expected-error {{parameter of incompatible type}}

  // f12(int p checked[10][10], int y);
  f12(x, 0);              // expected-error {{parameter of incompatible type}}
  f12(y, 0);              // expected-error {{parameter of incompatible type}}
  f12(z, 0);              // expected-error {{parameter of incompatible type}}
  f12(x2d, 0);            // OK
  f12(y2d, 0);            // OK
  f12(z2d, 0);            // expected-error {{parameter of incompatible type}}

  // f12a(int p checked[10]nt_checked[10], int y);
  f12a(x, 0);              // expected-error {{parameter of incompatible type}}
  f12a(y, 0);              // expected-error {{parameter of incompatible type}}
  f12a(z, 0);              // expected-error {{parameter of incompatible type}}
  f12a(x2d, 0);            // expected-error {{parameter of incompatible type}}
  f12a(y2d, 0);            // expected-error {{parameter of incompatible type}}
  f12a(z2d, 0);            // OK.

  // f13(_Bool b, int y);
  f13(x, 0);              // OK
  f13(y, 0);              // OK
  f13(z, 0);              // OK
  f13(x2d, 0);            // OK
  f13(y2d, 0);            // OK
  f13(z2d, 0);            // OK

  // spot check calls where an array is the second argument
  g1(0, x);
  g1(0, y);  // expected-error {{parameter of incompatible type}}
  g1(0, z);  // expected-error {{parameter of incompatible type}}
  g2(0, x);
  g2(0, y);  // expected-error {{parameter of incompatible type}}
  g2(0, z);  // expected-error {{parameter of incompatible type}}
  g3(0, x);
  g3(0, y);
  g3(0, z);  // expected-error {{argument does not meet declared bounds for 2nd parameter}}
  g3a(0, x); // expected-error {{parameter of incompatible type}}
  g3a(0, y); // expected-error {{parameter of incompatible type}}
  g3a(0, z);
}

extern void check_call_void(void) {
  int val = 0;
  int p[10];
  int r checked[10];
  int v nt_checked[10];

  // TODO: s will need bounds information
  void *s = 0;
  ptr<void> t = 0;
  array_ptr<void> u = 0;

  // Test different kinds of pointers where the parameter type is a pointer to void and
  // the referent type is not a void pointer.

  // Type of first parameter is a pointer type.
  // Expected to typecheck
  f1_void(p, val);    // param ptr<void>, arg int[10] OK.
  f3_void(r, val);    // param array_ptr<void>, arg int checked[10] OK.
  f3_void(p, val);    // param array_ptr<void>, arg int[10] OK, provided that param has unknown bounds.
  f3_void(r, val);    // param array_ptr<void>, arg int checked[10] OK, provided that param has unknown bounds.
  f3_void(v, val);    // param array_ptr<void>, arg int nt+checked[10] OK, provided that param has unknown bounds.

  // Expected to not typecheck
  f1_void(r, val);    // expected-error {{incompatible type}}
                      // param void *, arg int checked[10] not OK
  f1_void(v, val);    // expected-error {{incompatible type}}
                      // param void *, arg int nt_checked[10] not OK
  f2_void(r, val);    // param ptr<void>, arg int checked[10] OK
  f2_void(v, val);    // param ptr<void>, arg int nt_checked[10] OK

  // Try passing void pointers to functions expected array types
  // f1(int *, int)
  f1(s, 0);
  f1(t, 0);           // expected-error {{incompatible type}}
  f1(u, 0);           // expected-error {{incompatible type}}

  // f2(int p[10], int)
  f2(s, 0);
  f2(t, 0);           // expected-error {{incompatible type}}
  f2(u, 0);           // expected-error {{incompatible type}}

  // f3(int p checked[10], int)
  f3(s, 0);           // expected-error {{argument has unknown bounds}}
  f3(t, 0);           // expected-error {{incompatible type}}
  f3(u, 0);           // expected-error {{argument has unknown bounds}}

  // f3a(int p nt_checked[10], int)
  f3a(s, 0);           // expected-error {{incompatible type}}
  f3a(t, 0);           // expected-error {{incompatible type}}
  f3a(u, 0);           // expected-error {{incompatible type}}
}

void check_call_cv(void) {
  int val = 0;
  int p[10];
  const int p_const[10] = { 0, 1, 2, 3, 4,5, 6, 7, 8, 9};
  int r checked[10];
  const int r_const checked[10] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
  int s nt_checked[11];
  const int s_const nt_checked[11];

  // Parameters that are pointers to constants being passed pointers to non-const & const values.
  f1_const(p, val);           // param const int[10], arg int[10] OK
  f1_const(p_const, val);     // param const int[10], arg const int[10] OK
  f2_const(p, val);           // param const int checked[10], arg int[10] OK, provided int * has bounds.
  f2_const(p_const, val);     // param const int checked[10], arg const int[10] OK, provided int * has bounds
  f2_const(r, val);           // param const int checked[10], arg int checked[10] OK
  f2_const(r_const, val);     // param const int checked[10], arg const int checked[10]  OK
  f2a_const(s, val);           // param const int nt_checked[11], arg int nt_checked[10] OK
  f2a_const(s_const, val);     // param const int nt_checked[11], arg const int nt_checked[10]  OK

  // Parameters that are not pointers to constants being passed arrays of const
  f1(p_const, val);     // expected-warning {{discards qualifiers}}
                        // param int *, arg const int[10] not OK
  f2(p_const, val);     // expected-warning {{discards qualifiers}}
                        // param int[10], arg const int[10] not OK
  f3(r_const, val);     // expected-warning {{discards qualifiers}}
                        // param int checked[10], arg const int checked[10] not OK
  f3(s_const, val);     // expected-warning {{discards qualifiers}}
                        // param int nt_checked[11], arg const int checked[10] not OK
}

//
// Test typechecking of function returns
// 
//
// try to return an array type. This is not allowed by the C standard.
//

extern unchecked_arr_type h1(void) {  // expected-error {{function cannot return array type}}
  return 0;
}

extern checked_arr_type h2(void) {    // expected-error {{function cannot return array type}}
  return 0;
}

extern nt_checked_arr_type h2a(void) {    // expected-error {{function cannot return array type}}
  return 0;
}

int global[10];
int checked_global checked[10];
int nt_checked_global nt_checked[10];

int *h3(void) {
  return global;
}

ptr<int> h4(void) {
  return global;
}

array_ptr<int> h5(void) {
  return global;
}

nt_array_ptr<int> h5a(void) {
  return global;         // expected-error {{incompatible result type}}
}

int *h6(void) {
  return checked_global; // expected-error {{incompatible result type}}
}

ptr<int> h7(void) {
  return checked_global; // ptr<T> = array_ptr<T> OK
}

ptr<int> h7a(void) {
  return nt_checked_global; // ptr<T> = nt_array_ptr<T> OK
}

array_ptr<int> h8(void) {
  return checked_global;
}

array_ptr<int> h8a(void) {
  return nt_checked_global;
}

nt_array_ptr<int> h8b(void) {
  return checked_global;    // expected-error {{incompatible result type}}
}

nt_array_ptr<int> h8c(void) {
  return nt_checked_global;
}


int *h9(int arr[10]) {
  return arr;
}

array_ptr<int> h11(int arr checked[10]) {
  return arr;
}

array_ptr<int> h11a(int arr nt_checked[10]) {
  return arr;
}

int *h12(int arr checked[10]) {
  return arr;  // expected-error {{incompatible result type}}
}

int *h12a(int arr nt_checked[10]) {
  return arr;  // expected-error {{incompatible result type}}
}

ptr<int> h13(int arr checked[10]) {
  return arr;  // ptr<T> = array_ptr<T> OK
}

ptr<int> h13a(int arr nt_checked[10]) {
  return arr;  // ptr<T> = nt_array_ptr<T> OK
}

nt_array_ptr<int> h14(int arr checked[10]) {
  return arr;  // expected-error {{incompatible result type}}
}

nt_array_ptr<int> h14a(int arr nt_checked[10]) {
  return arr;
}


int *h15(int arr checked[]) {
  return arr;  // expected-error {{incompatible result type}}
}

ptr<int> h17(int arr checked[]) {
  return arr;  // expected-error {{expression has unknown bounds}}, ptr<T> = array_ptr<T> OK
}

ptr<int> h17a(int arr nt_checked[]) {
  return arr;  // expected-error {{cast source bounds are too narrow for '_Ptr<int>'}}
               // ptr<T> = nt_array_ptr<T> OK for typechecking, but bounds
               // declaration checking fails.
}

array_ptr<int> h18(int arr checked[]) {
  return arr;
}

array_ptr<int> h18a(int arr nt_checked[]) {
  return arr;
}

// h19 is a function that returns a pointer to a 10-element array of integers.
int(*h19(int arr[10][10]))[10]{
  return arr;
}

int global_arr2[10][10];
ptr<int[10]> h20(void) {
  return global_arr2;
}

array_ptr<int[10]> h21(int arr[10][10]) {
  return arr;
}

// h22 is a function that returns a pointer to a 10-element array of integers.
int (*h22(int arr checked[10][10]))[10] {
  return arr;  // expected-error {{incompatible result type}}
}

// h22a is a function that returns a pointer to a 10-element array of integers.
int(*h22a(int arr checked[10]nt_checked[10]))[10]{
  return arr;  // expected-error {{incompatible result type}}
}

ptr<int[10]> h23(int arr checked[10][10]) {
  return arr;  // expected-error {{incompatible result type}}
}

ptr<int[10]> h23a(int arr checked[10]nt_checked[10]) {
  return arr;  // expected-error {{incompatible result type}}
}


array_ptr<int[10]> h24(int arr checked[10][10]) {
  return arr;  // expected-error {{incompatible result type}}
}

array_ptr<int[10]> h24a(int arr checked[10]nt_checked[10]) {
  return arr;  // expected-error {{incompatible result type}}
}

// h25 is a function that returns a pointer to 10-element array of integers.
int (*h25(int arr checked[10][10])) checked[10]{
  return arr;  // expected-error {{incompatible result type}}
}

int(*h25a(int arr checked[10]nt_checked[10])) checked[10]{
  return arr;  // expected-error {{incompatible result type}}
}


ptr<int checked[10]> h26(int arr checked[10][10]) {
  return arr;  // ptr<T> = array_ptr<T> OK
}

ptr<int checked[10]> h26a(int arr checked[10]nt_checked[10]) {
  return arr;  // expected-error {{incompatible result type}}
}

array_ptr<int checked[10]> h27(int arr checked[10][10]) {
  return arr;
}

array_ptr<int nt_checked[10]> h27a(int arr checked[10]checked[10]) {
  return arr;  // expected-error {{incompatible result type}}
}

array_ptr<int nt_checked[10]> h27b(int arr checked[10]nt_checked[10]) {
  return arr;
}

void check_pointer_arithmetic(void) {
  int p[5];
  int r checked[5];
  int s nt_checked[5];

  int *p_tmp;
  array_ptr<int> r_tmp;
  // s_tmp has default bounds of count(0).  We don't to test bounds declaration
  // checking at the assignments to s_tmp below, so set the bounds to unknown.
  nt_array_ptr<int> s_tmp : bounds(unknown) = 0;

  p_tmp = p + 5;
  p_tmp = 5 + p;
  p_tmp = p_tmp - 2;
  p_tmp = 2 - p;      // expected-error {{invalid operands}}
  p_tmp = p++;        // expected-error {{cannot increment value}}
  p_tmp = p--;        // expected-error {{cannot decrement value}}
  p_tmp = ++p;        // expected-error {{cannot increment value}}
  p_tmp = --p;        // expected-error {{cannot decrement value}}
  p_tmp = (p += 1);   // expected-error {{invalid operands}}
  p_tmp = (p -= 1);   // expected-error {{invalid operands}}

  // 0 interpreted as an integer, not null
  p_tmp = p + 0;
  p_tmp = 0 + p;  
  p_tmp = p - 0;
  p_tmp = 0 - p;  // expected-error {{invalid operands to binary expression}}

  r_tmp = r + 5;
  r_tmp = 5 + r;
  r_tmp = r_tmp - 2; 
  r_tmp = 2 - r;         // expected-error {{invalid operands}}
  r_tmp = r++;        // expected-error {{cannot increment value}}
  r_tmp = r--;        // expected-error {{cannot decrement value}}
  r_tmp = ++r;        // expected-error {{cannot increment value}}
  r_tmp = --r;        // expected-error {{cannot decrement value}}
  r_tmp = (r += 1);   // expected-error {{invalid operands}}
  r_tmp = (r -= 1);   // expected-error {{invalid operands}}
  // 0 interpreted as an integer, not null
  r_tmp = r + 0;
  r_tmp = 0 + r;
  r_tmp = r - 0;
  r_tmp = 0 - r; // expected-error {{invalid operands to binary expression}}

  // adding two pointers is not allowed
  r + r; // expected-error {{invalid operands}}

  s_tmp = s + 5;
  s_tmp = 5 + s;
  s_tmp = s_tmp - 2;
  s_tmp = 2 - s;         // expected-error {{invalid operands}}
  s_tmp = s++;        // expected-error {{cannot increment value}}
  s_tmp = s--;        // expected-error {{cannot decrement value}}
  s_tmp = ++s;        // expected-error {{cannot increment value}}
  s_tmp = --s;        // expected-error {{cannot decrement value}}
  s_tmp = (s += 1);   // expected-error {{invalid operands}}
  s_tmp = (s -= 1);   // expected-error {{invalid operands}}
                      // 0 interpreted as an integer, not null
  s_tmp = s + 0;
  s_tmp = 0 + s;
  s_tmp = s - 0;
  s_tmp = 0 - s; // expected-error {{invalid operands to binary expression}}

                 // adding two pointers is not allowed
  s + s; // expected-error {{invalid operands}}
}

void check_pointer_difference(int flag) {
  int count;
  int val_int[5];
  float val_float [5];
  int *p_int = val_int;
  float *p_float = val_float;
  array_ptr<int> r_int = val_int;

  int a_int[5];
  int checked_a_int checked[5];
  int nt_checked_a_int nt_checked[5];

  float a_float[5];
  float checked_a_float checked[5];

  if (flag) {
      p_int = a_int;
      r_int = checked_a_int;
  }

  // pointer - array
  count = p_int - a_int;
  count = p_int - checked_a_int;
  count = p_int - nt_checked_a_int;
  count = r_int - a_int;
  count = r_int - checked_a_int;
  count = r_int - nt_checked_a_int;

  // array - pointer
  count = a_int - p_int;
  count = checked_a_int - p_int;
  count = nt_checked_a_int - p_int;
  count = a_int - r_int;
  count = checked_a_int - r_int;
  count = nt_checked_a_int - r_int;

  // array - array
  count = a_int - checked_a_int;
  count = checked_a_int - a_int;
  count = nt_checked_a_int - checked_a_int;
  count = checked_a_int - nt_checked_a_int;
  count = nt_checked_a_int - nt_checked_a_int;


  // spot check mismatched types
  count = a_float - p_int;          // expected-error {{not pointers to compatible types}}
  count = p_int - checked_a_float;  // expected-error {{not pointers to compatible types}}
  count = checked_a_float - r_int;  // expected-error {{not pointers to compatible types}}
}

void check_pointer_relational_compare(void) {
  int result;

  int val_int[5];
  float val_float[5];
  int checked_val_int checked[5];
  int nt_checked_val_int nt_checked[5];
  float checked_val_float checked[5];

  float *p_float = val_float;
  int *p_int = val_int;

  ptr<float> q_float = val_float;
  ptr<int> q_int = val_int;

  array_ptr<float> r_float = val_float;
  array_ptr<int> r_int = val_int;
  nt_array_ptr<int> s_int = 0;

  // relational comparisons between pointers and unchecked arrays;
  result = val_int < p_int;
  result = val_int <= q_int;
  result = val_int >= r_int;
  result = val_int >= s_int;

  result = p_int > val_int;
  result = q_int < val_int;
  result = r_int <= val_int;
  result = s_int <= val_int;

  // relational comparisons between pointers and checked arrays;
  result = checked_val_int < p_int;
  result = checked_val_int <= q_int;
  result = checked_val_int >= r_int;
  result = checked_val_int >= s_int;

  result = p_int > checked_val_int;
  result = q_int < checked_val_int;
  result = r_int <= checked_val_int;
  result = s_int <= checked_val_int;

  // invalid relational comparisons

  // spot check comparisons between pointers and unchecked arrays;
  result = val_int < p_float;  // expected-warning {{comparison of distinct pointer types}}
  result = val_float <= q_int; // expected-warning {{comparison of distinct pointer types}}
  result = val_int >= r_float; // expected-warning {{comparison of distinct pointer types}}

  result = p_int > val_float;  // expected-warning {{comparison of distinct pointer types}}
  result = q_float < val_int;  // expected-warning {{comparison of distinct pointer types}}
  result = r_int <= val_float; // expected-warning {{comparison of distinct pointer types}}
  result = s_int <= val_float; // expected-warning {{comparison of distinct pointer types}}

  // spot check comparisons between pointers and checked arrays;
  result = checked_val_int < p_float;  // expected-warning {{comparison of distinct pointer types}} 
  result = checked_val_float <= q_int; // expected-warning {{comparison of distinct pointer types}}
  result = checked_val_int >= r_float; // expected-warning {{comparison of distinct pointer types}}

  result = p_int > checked_val_float;  // expected-warning {{comparison of distinct pointer types}}
  result = q_float < checked_val_int;  // expected-warning {{comparison of distinct pointer types}}
  result = r_int <= checked_val_float; // expected-warning {{comparison of distinct pointer types}}
  result = s_int <= checked_val_float; // expected-warning {{comparison of distinct pointer types}}
}

void check_pointer_equality_compare(void) {
  int result;

  int val_int[5];
  float val_float[5];
  int checked_val_int checked[5];
  int nt_checked_val_int nt_checked[5];
  float checked_val_float checked[5];

  float *p_float = val_float;
  int *p_int = val_int;

  ptr<float> q_float = val_float;
  ptr<int> q_int = val_int;

  array_ptr<float> r_float = val_float;
  array_ptr<int> r_int = val_int;
  nt_array_ptr<int> s_int = 0;

  // equality/inequality comparisons between pointers and unchecked arrays;
  result = val_int == p_int;
  result = val_int != q_int;
  result = val_int == r_int;
  result = val_int == s_int;

  result = p_int != val_int;
  result = q_int == val_int;
  result = r_int != val_int;
  result = s_int != val_int;

  // equality/inequality comparisons between pointers and checked arrays;
  result = checked_val_int == p_int;
  result = checked_val_int != q_int;
  result = checked_val_int == r_int;

  result = nt_checked_val_int == p_int;
  result = nt_checked_val_int != q_int;
  result = nt_checked_val_int == r_int;
  result = nt_checked_val_int == s_int;

  result = p_int != checked_val_int;
  result = q_int == checked_val_int;
  result = r_int != checked_val_int;

  result = p_int != nt_checked_val_int;
  result = q_int == nt_checked_val_int;
  result = r_int != nt_checked_val_int;
  result = s_int != nt_checked_val_int;

  // invalid equality/inequality comparisons

  // spot check equality comparisons between pointers and unchecked arrays;
  result = val_int == p_float;  // expected-warning {{comparison of distinct pointer types}}
  result = val_float != q_int; // expected-warning {{comparison of distinct pointer types}}
  result = val_int == r_float; // expected-warning {{comparison of distinct pointer types}}
  result = val_float != s_int; // expected-warning {{comparison of distinct pointer types}}

  result = p_int != val_float;  // expected-warning {{comparison of distinct pointer types}}
  result = q_float == val_int;  // expected-warning {{comparison of distinct pointer types}}
  result = r_int != val_float; // expected-warning {{comparison of distinct pointer types}}
  result = s_int != val_float; // expected-warning {{comparison of distinct pointer types}}

  // spot check equality comparisons between pointers and checked arrays;
  result = checked_val_int == p_float;  // expected-warning {{comparison of distinct pointer types}} 
  result = checked_val_float != q_int; // expected-warning {{comparison of distinct pointer types}}
  result = checked_val_int != r_float; // expected-warning {{comparison of distinct pointer types}}

  result = nt_checked_val_int == p_float;  // expected-warning {{comparison of distinct pointer types}} 
  result = nt_checked_val_int!= q_float; // expected-warning {{comparison of distinct pointer types}}
  result = nt_checked_val_int != r_float; // expected-warning {{comparison of distinct pointer types}}

  result = p_int == checked_val_float;  // expected-warning {{comparison of distinct pointer types}}
  result = q_float != checked_val_int;  // expected-warning {{comparison of distinct pointer types}}
  result = r_int == checked_val_float; // expected-warning {{comparison of distinct pointer types}}
}

void check_logical_operators(void) {
  int p[5];
  int r checked[5];
  int s nt_checked[5];

  _Bool b;

  b = !p;
  b = !r;
  b = !s;

  b = p || b;
  b = r || b;
  b = s || b;
  b = b || p;
  b = b || r;
  b = b || s;

  b = r || p;
  b = s || p;
  b = p || r;
  b = p || s;

  b = p && b;
  b = r && b;
  b = s && b;
  b = b && p;
  b = b && r;
  b = b && s;

  b = r && p;
  b = s && p;
  b = p && r;
  b = p && s;
}

void check_cast_operator(void) {
  int x = 0;
  int arr checked[5];
  int nt_arr nt_checked[5];

  // casts involving array types
  array_ptr<int> pax = (array_ptr<int>) &x;
  pax = (int checked[]) &x;   // expected-error {{cast to incomplete type}}
  pax = (int checked[1]) &x;  // expected-error {{arithmetic or pointer type is required}}

  array_ptr<int> nt_pax = (array_ptr<int>) &x;
  pax = (int nt_checked[]) &x;   // expected-error {{cast to incomplete type}}
  pax = (int nt_checked[1]) &x;  // expected-error {{arithmetic or pointer type is required}}

  // casts involving pointers to array types

  // unchecked pointer to array
  ptr<int checked[5]> parr = 0;
  parr = (int(*)checked[5]) &arr;
  parr = (int(*)checked[5]) ((int(*)checked[]) &arr);
  parr = (int(*)checked[3]) &arr; // expected-error {{incompatible type}}

  ptr<int nt_checked[5]> nt_parr = 0;
  nt_parr = (int(*)nt_checked[5]) &nt_arr;
  nt_parr = (int(*)nt_checked[5]) ((int(*)nt_checked[]) &nt_arr);
  nt_parr = (int(*)nt_checked[3]) &nt_arr; // expected-error {{incompatible type}}

  parr = (int(*)nt_checked[5]) &nt_arr; // expected-error {{incompatible type}}
  parr = (int(*)nt_checked[5]) ((int(*)nt_checked[]) &nt_arr); // expected-error {{incompatible type}}
  parr = (int(*)nt_checked[3]) &nt_arr; // expected-error {{incompatible type}}

  // ptr to array
  parr = (ptr<int checked[5]>) &arr;
  parr = (ptr<int checked[5]>) ((ptr<int checked[]>) &arr);

  parr = (ptr<int checked[3]>) &arr; // expected-error {{incompatible type}}

  nt_parr = (ptr<int nt_checked[5]>) &arr;
  nt_parr = (ptr<int nt_checked[5]>) ((ptr<int checked[]>) &arr);
  nt_parr = (ptr<int nt_checked[3]>) &arr; // expected-error {{incompatible type}}

  // array_ptr to array
  array_ptr<int checked[5]> aparr = 0;
  aparr = (array_ptr<int checked[5]>) &arr;
  aparr = (array_ptr<int checked[5]>) ((array_ptr<int checked[]>) &arr);
  aparr = (array_ptr<int checked[3]>) &arr; // expected-error {{incompatible type}}
}

// spot check operators that aren't supposed to be used with array types:
//   *, /, %, <<, >>, |, &, ^, ~, unary -, and unary +
void check_illegal_operators(void) {
  int p[5];
  int r checked[5];
  int s nt_checked[5];

  p * 5;  // expected-error {{invalid operands to binary expression}}
  5 * p;  // expected-error {{invalid operands to binary expression}}
  p *= 5; // expected-error {{invalid operands to binary expression}}

  r * 5;  // expected-error {{invalid operands to binary expression}}
  5 * r;  // expected-error {{invalid operands to binary expression}}
  r *= 5; // expected-error {{invalid operands to binary expression}}

  s * 5;  // expected-error {{invalid operands to binary expression}}
  5 * s;  // expected-error {{invalid operands to binary expression}}
  s *= 5; // expected-error {{invalid operands to binary expression}}

  p * p;  // expected-error {{invalid operands to binary expression}}
  p *= p; // expected-error {{invalid operands to binary expression}}

  r * r;  // expected-error {{invalid operands to binary expression}}
  r *= r; // expected-error {{invalid operands to binary expression}}

  s * s;  // expected-error {{invalid operands to binary expression}}
  s *= s; // expected-error {{invalid operands to binary expression}}

  //
  // Test /
  //

  p / 5;  // expected-error {{invalid operands to binary expression}}
  5 / p;  // expected-error {{invalid operands to binary expression}}
  p /= 5; // expected-error {{invalid operands to binary expression}}

  r / 5;  // expected-error {{invalid operands to binary expression}}
  5 / r;  // expected-error {{invalid operands to binary expression}}
  r /= 5; // expected-error {{invalid operands to binary expression}}

  s / 5;  // expected-error {{invalid operands to binary expression}}
  5 / s;  // expected-error {{invalid operands to binary expression}}
  s /= 5; // expected-error {{invalid operands to binary expression}}

  p / p;  // expected-error {{invalid operands to binary expression}}
  p /= p; // expected-error {{invalid operands to binary expression}}

  r / r;  // expected-error {{invalid operands to binary expression}}
  r /= r; // expected-error {{invalid operands to binary expression}}

  s / s;  // expected-error {{invalid operands to binary expression}}
  s /= s; // expected-error {{invalid operands to binary expression}}


  //
  // Test %
  //

  p % 5;  // expected-error {{invalid operands to binary expression}}
  5 % p;  // expected-error {{invalid operands to binary expression}}
  p %= 5; // expected-error {{invalid operands to binary expression}}

  r % 5;  // expected-error {{invalid operands to binary expression}}
  5 % r;  // expected-error {{invalid operands to binary expression}}
  r %= 5; // expected-error {{invalid operands to binary expression}}

  s % 5;  // expected-error {{invalid operands to binary expression}}
  5 % s;  // expected-error {{invalid operands to binary expression}}
  s %= 5; // expected-error {{invalid operands to binary expression}}


  p % p;  // expected-error {{invalid operands to binary expression}}
  p %= p; // expected-error {{invalid operands to binary expression}}

  r % r;  // expected-error {{invalid operands to binary expression}}
  r %= r; // expected-error {{invalid operands to binary expression}}

  s % s;  // expected-error {{invalid operands to binary expression}}
  s %= s; // expected-error {{invalid operands to binary expression}}

  //
  // Test <<
  //

  p << 5;  // expected-error {{invalid operands to binary expression}}
  5 << p;  // expected-error {{invalid operands to binary expression}}
  p <<= 5; // expected-error {{invalid operands to binary expression}}

  r << 5;  // expected-error {{invalid operands to binary expression}}
  5 << r;  // expected-error {{invalid operands to binary expression}}
  r <<= 5; // expected-error {{invalid operands to binary expression}}

  s << 5;  // expected-error {{invalid operands to binary expression}}
  5 << s;  // expected-error {{invalid operands to binary expression}}
  s <<= 5; // expected-error {{invalid operands to binary expression}}

  p << p;  // expected-error {{invalid operands to binary expression}}
  p <<= p; // expected-error {{invalid operands to binary expression}}

  r << r;  // expected-error {{invalid operands to binary expression}}
  r <<= r; // expected-error {{invalid operands to binary expression}}

  s << s;  // expected-error {{invalid operands to binary expression}}
  s <<= s; // expected-error {{invalid operands to binary expression}}

  //
  // Test >>
  //
  p >> 5;  // expected-error {{invalid operands to binary expression}}
  5 >> p;  // expected-error {{invalid operands to binary expression}}
  p >>= 5; // expected-error {{invalid operands to binary expression}}

  r >> 5;  // expected-error {{invalid operands to binary expression}}
  5 >> r;  // expected-error {{invalid operands to binary expression}}
  r >>= 5; // expected-error {{invalid operands to binary expression}}

  s >> 5;  // expected-error {{invalid operands to binary expression}}
  5 >> s;  // expected-error {{invalid operands to binary expression}}
  s >>= 5; // expected-error {{invalid operands to binary expression}}

  p >> p;  // expected-error {{invalid operands to binary expression}}
  p >>= p; // expected-error {{invalid operands to binary expression}}

  r >> r;  // expected-error {{invalid operands to binary expression}}
  r >>= r; // expected-error {{invalid operands to binary expression}}

  s >> s;  // expected-error {{invalid operands to binary expression}}
  s >>= s; // expected-error {{invalid operands to binary expression}}

  //
  // Test |
  //

  p | 5;  // expected-error {{invalid operands to binary expression}}
  5 | p;  // expected-error {{invalid operands to binary expression}}
  p |= 5; // expected-error {{invalid operands to binary expression}}

  r | 5;  // expected-error {{invalid operands to binary expression}}
  5 | r;  // expected-error {{invalid operands to binary expression}}
  r |= 5; // expected-error {{invalid operands to binary expression}}

  s | 5;  // expected-error {{invalid operands to binary expression}}
  5 | s;  // expected-error {{invalid operands to binary expression}}
  s |= 5; // expected-error {{invalid operands to binary expression}}

  p | p;  // expected-error {{invalid operands to binary expression}}
  p |= p; // expected-error {{invalid operands to binary expression}}

  r | r;  // expected-error {{invalid operands to binary expression}}
  r |= r; // expected-error {{invalid operands to binary expression}}

  s | s;  // expected-error {{invalid operands to binary expression}}
  s |= s; // expected-error {{invalid operands to binary expression}}

  //
  // Test &
  //

  p & 5;  // expected-error {{invalid operands to binary expression}}
  5 & p;  // expected-error {{invalid operands to binary expression}}
  p &= 5; // expected-error {{invalid operands to binary expression}}

  r & 5;  // expected-error {{invalid operands to binary expression}}
  5 & r;  // expected-error {{invalid operands to binary expression}}
  r &= 5; // expected-error {{invalid operands to binary expression}}

  s & 5;  // expected-error {{invalid operands to binary expression}}
  5 & s;  // expected-error {{invalid operands to binary expression}}
  s &= 5; // expected-error {{invalid operands to binary expression}}

  p & p;  // expected-error {{invalid operands to binary expression}}
  p &= p; // expected-error {{invalid operands to binary expression}}

  r & r;  // expected-error {{invalid operands to binary expression}}
  r &= r; // expected-error {{invalid operands to binary expression}}

  s & s;  // expected-error {{invalid operands to binary expression}}
  s &= s; // expected-error {{invalid operands to binary expression}}

  //
  // Test ^
  //

  p ^ 5;  // expected-error {{invalid operands to binary expression}}
  5 ^ p;  // expected-error {{invalid operands to binary expression}}
  p ^= 5; // expected-error {{invalid operands to binary expression}}

  r ^ 5;  // expected-error {{invalid operands to binary expression}}
  5 ^ r;  // expected-error {{invalid operands to binary expression}}
  r ^= 5; // expected-error {{invalid operands to binary expression}}

  s ^ 5;  // expected-error {{invalid operands to binary expression}}
  5 ^ s;  // expected-error {{invalid operands to binary expression}}
  s ^= 5; // expected-error {{invalid operands to binary expression}}

  p ^ p;  // expected-error {{invalid operands to binary expression}}
  p ^= p; // expected-error {{invalid operands to binary expression}}

  r ^ r;  // expected-error {{invalid operands to binary expression}}
  r ^= r; // expected-error {{invalid operands to binary expression}}

  s ^ s;  // expected-error {{invalid operands to binary expression}}
  s ^= s; // expected-error {{invalid operands to binary expression}}

  //
  // Test ~
  //
  ~p;  // expected-error {{invalid argument type}}
  ~r;  // expected-error {{invalid argument type}}
  ~s;  // expected-error {{invalid argument type}}

  //
  // Test unary -
  //
  -p;  // expected-error {{invalid argument type}}
  -r;  // expected-error {{invalid argument type}}
  -s;  // expected-error {{invalid argument type}}

  //
  // Test unary +
  //
  +p;  // expected-error {{invalid argument type}}
  +r;  // expected-error {{invalid argument type}}
  +s;  // expected-error {{invalid argument type}}
}

extern void check_vla(int i) {
  int x[i];
  int y checked[i];     // expected-error {{checked variable-length array not allowed}}
  int z checked[10][i]; // expected-error {{checked variable-length array not allowed}}
  int a nt_checked[i];  // expected-error {{checked variable-length array not allowed}}
}

// Test assigning an unchecked pointer with an nt_array_ptr
// bounds-safe interface to a checked nt_array_ptr.
extern void check_nt_bounds_safe_interface(const char *interop_string : itype(nt_array_ptr<const char>),
                                           char **interop_ptr : itype(nt_array_ptr<ptr<char>>),
                                           char checked_arr nt_checked[3],
                                           char unchecked_arr[3]) {
  // LHS of assignment has nt_array_ptr<T> type and RHS of assignment
  // is an unchecked pointer with nt_array_ptr<T> bounds-safe interface.
  nt_array_ptr<const char> checked_str = interop_string;
  nt_array_ptr<ptr<char>> checked_ptr = interop_ptr;

  // LHS of assignment has nt_array_ptr<T> type and RHS of assigment
  // is an unchecked pointer with nt_array_ptr<U> bounds-safe interface,
  // where T and U are not assignment-compatible.
  checked_str = interop_ptr; // expected-error {{assigning to '_Nt_array_ptr<const char>' from incompatible type 'char **'}}

  // RHS of assignment has no bounds-safe interface type.
  checked_arr = unchecked_arr; // expected-error {{assigning to '_Nt_array_ptr<char>' from incompatible type 'char *'}}
}
