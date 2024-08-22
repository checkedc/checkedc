// Unit tests for typechecking new Checked C pointer types.
//
// The following line is for the LLVM test harness:
// RUN: %clang_cc1 -Wno-unused-value -Wno-pointer-bool-conversion -verify -verify-ignore-unexpected=note %s
//

#include <stdchecked.h>

extern void check_indirection_unsafe_ptr(int *p, const int *const_p, int y) {
	*p = y;
	y = *p;
	*const_p = y; // expected-error {{read-only variable is not assignable}}
	y = *const_p;
}

extern void check_indirection_ptr(int* _Single p, const int *_Single const_p, int y) {
        *p = y;
        y = *p;
        *const_p = y; // expected-error {{read-only variable is not assignable}}
        y = *const_p;
}

extern void check_indirection_array_ptr(array_ptr<int> p : count(1),
                                        array_ptr<const int> const_p : count(1),
                                        int y) {
	*p = y;
	y = *p;
	*const_p = y; // expected-error {{read-only variable is not assignable}}
	y = *const_p;
}

extern void check_indirection_nt_array_ptr(nt_array_ptr<int> p : count(1),
                                           nt_array_ptr<const int> const_p : count(1),
                                           int y) {
  *p = y;
  y = *p;
  *const_p = y; // expected-error {{read-only variable is not assignable}}
  y = *const_p;
}

extern void check_subscript_unsafe_ptr(int *p, int y) {
    p[0] = y;
    y = p[0]; 
    0[p] = y;
    y = 0[p];
}

extern void check_subscript_ptr(ptr<int> p, ptr<const int> p_const, int y) {
   p[0] = y;  // expected-error {{subscript of '_Ptr<int>'}}
   y = p[0];  // expected-error {{subscript of '_Ptr<int>'}}
   0[p] = y;  // expected-error {{subscript of '_Ptr<int>'}}
   y = 0[p];  // expected-error {{subscript of '_Ptr<int>'}}
   p_const[0] = y;  // expected-error {{subscript of '_Ptr<const int>'}}
   y = p_const[0];  // expected-error {{subscript of '_Ptr<const int>'}}
   0[p_const] = y;  // expected-error {{subscript of '_Ptr<const int>'}}
   y = 0[p_const];  // expected-error {{subscript of '_Ptr<const int>'}}
}

extern void check_subscript_array_ptr(array_ptr<int> p : count(1),
                                      array_ptr<const int> p_const : count(1),
                                      int y) {
   p[0] = y;  // OK
   y = p[0];  // OK
   0[p] = y;  // OK
   y = 0[p];  // OK
   p_const[0] = y;  // expected-error {{read-only variable is not assignable}}
   y = p_const[0];  // OK
   0[p_const] = y;  // expected-error {{read-only variable is not assignable}}
   y = 0[p_const];  // OK
}

extern void check_subscript_nt_array_ptr(nt_array_ptr<int> p : count(1),
                                         nt_array_ptr<const int> p_const : count(1),
                                         int y) {
  p[0] = y;  // OK
  y = p[0];  // OK
  0[p] = y;  // OK
  y = 0[p];  // OK
  p_const[0] = y;  // expected-error {{read-only variable is not assignable}}
  y = p_const[0];  // OK
  0[p_const] = y;  // expected-error {{read-only variable is not assignable}}
  y = 0[p_const];  // OK
}

// Test restrictions on null-terminated pointer types.
void check_nullterm_restrictions(void) {
  int* _Nt_array t1 = 0;              // integer types are OK.
  int* _Single *_Nt_array t2 = 0;         // pointer types are OK.
  enum E { Null, Blue, White };
  enum E* _Nt_array t3 = 0;           // enum types are OK

  void* _Nt_array t10 = 0;            // expected-error {{only integer and pointer types are allowed}}
  float* _Nt_array t11 = 0;           // expected-error {{only integer and pointer types are allowed}}
  double* _Nt_array t12 = 0;          // expected-error {{only integer and pointer types are allowed}}
  int* _Nt_array t13 checked[5] = 0;  // expected-error {{array initializer must be an initializer list}}
  struct S { int i; };
  struct S* _Nt_array  t14 = 0;        // expected-error {{only integer and pointer types are allowed}}
}


// Test assignments between different kinds of pointers, excluding
// void pointers and pointers with constant/volatile attributes.
extern void check_assign(int val, int *p, ptr<int> q, array_ptr<int> r,
                         float *s, ptr<float> t, array_ptr<float> u,
                         nt_array_ptr<int> v : count(1), nt_array_ptr<short> x) {
    int *t1 = p;              // T *  = T * OK
    ptr<int> t2 = &val;       // ptr<T> = T * OK when T * has known bounds
    ptr<int> t3 = q;          // ptr<T> = ptr<T> OK
    array_ptr<int> t4 = &val; // array_ptr<T> = T * OK when T * has known bounds
    array_ptr<int> t5 = r;    // array_ptr<T> = array_ptr<T> OK
    nt_array_ptr<int> t5a = v;// nt_array_ptr<T> = nt_array_ptr<T> OK.
    nt_array_ptr<int> t5b = &val; // expected-error {{incompatible type}}
                                  // nt_array_ptr<T> = T * not OK, even when T
                                  // has known bounds.
    int *t6 = q;              // expected-error {{incompatible type}}
                              // T * = ptr<T> not OK;
    int *t7 = r;              // expected-error {{incompatible type}}
                              // T * = array_ptr<T> not OK
    int *t7a = v;             // // expected-error {{incompatible type}}
                              // T * = nt_array_ptr<T> not OK
    ptr<int> t8 = r;          // expected-error {{expression has unknown bounds}}
                              // ptr<T> = array_ptr<T> OK
    int* _Single t8a = v;         // ptr<T> = nt_array_ptr<T> OK.
    int* _Array t9 = q;    // array_ptr<T> = ptr<T> OK
    int* _Array t10a = v;  // array_ptr<T> = nt_array_ptr<T> OK.
    int* _Nt_array t10b = q; // expected-error {{incompatible type}}
                              // nt_array_ptr<T> = ptr<T> not OK.
    int* _Nt_array t10ca = r; // expected-error {{incompatible type}}
                              // nt_array_ptr<T> = array_ptr<T> not OK.

    // check assigning different kinds of pointers with different referent
    // types

    // right hand referent type is int and left hand referent type is float
    int *t11 = t;             // expected-error {{incompatible type}}
                              // T * = ptr<S> not OK
    int *t12 = u;             // expected-error {{incompatible type}}
                              // T * = array_ptr<S> not OK;
    ptr<int> t13 = s;         // expected-error {{incompatible type}}
                              // ptr<T> = S * not OK
    ptr<int> t14 = t;         // expected-error {{incompatible type}}
                              // ptr<T> = ptr<S> not OK
    ptr<int> t15 = u;         // expected-error {{incompatible type}}
                              // ptr<T> = array_ptr<S> not OK
    array_ptr<int> t16 = s;   // expected-error {{incompatible type}}
                              // array_ptr<T> = S * not OK;
    array_ptr<int> t17 = t;   // expected-error {{incompatible type}}
                              // array_ptr<T> = ptr<S> not OK;
    array_ptr<int> t18 = u;   // expected-error {{incompatible type}}
                              // array_ptr<T> = array_ptr<S> not OK
    nt_array_ptr<int> t18a = s; // expected-error {{incompatible type}}
                                // nt_array_ptr<T> = S * not OK;
    nt_array_ptr<int> t18b = t; // expected-error {{incompatible type}}
                                // nt_array_ptr<T> = ptr<S> not OK;
    nt_array_ptr<int> t18c = u; // expected-error {{incompatible type}}
                                // nt_array_ptr<T> = array_ptr<S> not OK
    nt_array_ptr<int> t18d = x; // expected-error {{incompatible type}}
                                // nt_array_ptr<T> = nt_array_ptr<S> not OK

    // left hand referent type is float and right hand  referent type is int
    float *t19 = q;           // expected-error {{incompatible type}}
                              // T * = ptr<S> not OK
    float *t20 = r;           // expected-error {{incompatible type}}
                              // T * = array_ptr<S> not OK;
    ptr<float> t21 = p;       // expected-error {{incompatible type}}
                              // ptr<T> = S * not OK
    ptr<float> t22 = q;       // expected-error {{incompatible type}}
                              // ptr<T> = ptr<S> not OK
    ptr<float> t23 = r;       // expected-error {{incompatible type}}
                              // ptr<T> = array_ptr<S> not OK
    array_ptr<float> t24 = p; // expected-error {{incompatible type}}
                              // array_ptr<T> = S * not OK;
    array_ptr<float> t25 = q; // expected-error {{incompatible type}}
                              // array_ptr<T> = ptr<S> not OK;
    float* _Array t26 = r; // expected-error {{incompatible type}}
                              // array_ptr<T> = array_ptr<S> not OK

    // C compilers enforcing C99 conversion rules allow implicit 
    // integer to pointer conversions. These are errors for the new safe
    // pointer types.
    ptr<int> t27 = val;       // expected-error {{incompatible type}}
                              // ptr<T> = int not OK
    ptr<float> t28 = val;     // expected-error {{incompatible type}}
                              // ptr<T> = int not OK;
    int t29 = q;              // expected-error {{incompatible type}}
                              // int = ptr<T> not OK;
    int t30 = t;              // expected-error {{incompatible type}}
                              // int = ptr<T> not OK
    array_ptr<int> t32 = val; // expected-error {{incompatible type}}
                              // array_ptr<T> = int not OK
    nt_array_ptr<int> t32a = val; // expected-error {{incompatible type}}
                                  // nt_array_ptr<T> = int not OK
    int t33 = q;              // expected-error {{incompatible type}}
                              // int = array_ptr<int> not OK.
    int t33a = v;              // expected-error {{incompatible type}}
                               // int = array_ptr<int> not OK.

    // spot check converting a pointer to a floating point type
    float t31 = q;            // expected-error {{incompatible type}}
                              // float = ptr<T> not OK

    // Implicit conversion of a safe pointer type to _Bool is OK.
    _Bool t34 = q;
    _Bool t35 = r;
    _Bool t36 = t;
    _Bool t37 = u;
    _Bool t37a = v;

    // _Bool to safe pointer is not OK.
    ptr<int> t38 = (_Bool)(1);   // expected-error {{incompatible type}}
    ptr<float> t39 = (_Bool)(1); // expected-error {{incompatible type}}
    array_ptr<int> t40 = (_Bool)(1);   // expected-error {{incompatible type}}
    float* _Array t41 = (_Bool)(1); // expected-error {{incompatible type}}
    int* _Nt_array t41a = (_Bool)(1); // expected-error {{incompatible type}}

    // Implicit conversion of 0 to a safe pointer type is OK.
    ptr<int> t42 = 0;
    array_ptr<int> t43 = 0;
    ptr<float> t44 = 0;
    array_ptr<float> t45 = 0;
    nt_array_ptr<int> t46 = 0;

    // Check assignments of pointers to pointers.
    //
    // There are language extensions where only warnings are issued when pointer
    // referent types are mismatched or have different levels of indirection. Except
    // for conversions of unchecked pointer to T to checked pointers to T, these
    // should be errors for checked pointers.  Otherwise the integrity of bounds checking 
    // can be compromised accidentally.

    int **unchecked_ptr_to_unchecked_ptr = &p;
    ptr<int> *unchecked_ptr_to_checked_ptr= &q;
    ptr<int *> checked_ptr_to_unchecked_ptr = &p;
    ptr<ptr<int>> checked_ptr_to_checked_ptr = &q;  
    array_ptr<ptr<int>> array_ptr_to_checked_ptr = &q;
    array_ptr<int *> array_ptr_to_unchecked_ptr = &p;

    // First check pointers with different levels of indirection.
    ptr<int> t50 = unchecked_ptr_to_checked_ptr;  // expected-error {{incompatible type}}
    ptr<int> t51 = checked_ptr_to_checked_ptr;    // expected-error {{incompatible type}}
    ptr<int> t52 = array_ptr_to_checked_ptr;      // expected-error {{incompatible type}}

    array_ptr<int> t53 = unchecked_ptr_to_checked_ptr;  // expected-error {{incompatible type}}
    array_ptr<int> t54 = checked_ptr_to_checked_ptr;    // expected-error {{incompatible type}}
    int* _Array t56 = array_ptr_to_checked_ptr;      // expected-error {{incompatible type}}

    unchecked_ptr_to_checked_ptr = q;      // expected-error {{incompatible type}}
    checked_ptr_to_checked_ptr = p;        // expected-error {{incompatible type}}
    checked_ptr_to_checked_ptr = q;        // expected-error {{incompatible type}}
    array_ptr_to_checked_ptr = p;          // expected-error {{incompatible type}}
    array_ptr_to_checked_ptr = q;          // expected-error {{incompatible type}}

    // Check assignments of pointers to pointers with the same level of indirection
    // but different checked qualities.

    unchecked_ptr_to_unchecked_ptr = unchecked_ptr_to_unchecked_ptr; // OK
    unchecked_ptr_to_unchecked_ptr = unchecked_ptr_to_checked_ptr;   // expected-error {{incompatible type}}
    unchecked_ptr_to_unchecked_ptr = checked_ptr_to_unchecked_ptr;   // expected-error {{incompatible type}}
    unchecked_ptr_to_unchecked_ptr = checked_ptr_to_checked_ptr;     // expected-error {{incompatible type}}

    unchecked_ptr_to_checked_ptr = unchecked_ptr_to_unchecked_ptr; // expected-error {{incompatible type}}
    unchecked_ptr_to_checked_ptr = unchecked_ptr_to_checked_ptr;   // OK
    unchecked_ptr_to_checked_ptr = checked_ptr_to_unchecked_ptr;   // expected-error {{incompatible type}}
    unchecked_ptr_to_checked_ptr = checked_ptr_to_checked_ptr;     // expected-error {{incompatible type}}

    checked_ptr_to_checked_ptr = unchecked_ptr_to_unchecked_ptr; // expected-error {{incompatible type}}
    // checked_ptr_to_checked_ptr = unchecked_ptr_to_checked_ptr;    not allowed: right-hand side has unknown bounds.
    checked_ptr_to_checked_ptr = checked_ptr_to_unchecked_ptr;   // expected-error {{incompatible type}}
    checked_ptr_to_checked_ptr = checked_ptr_to_checked_ptr;     // OK

    array_ptr_to_checked_ptr = unchecked_ptr_to_unchecked_ptr; // expected-error {{incompatible type}}
    // array_ptr_to_checked_ptr = unchecked_ptr_to_checked_ptr;  not allowed: right-hand side has unknown bounds.
    array_ptr_to_checked_ptr = checked_ptr_to_unchecked_ptr;   // expected-error {{incompatible type}}
    array_ptr_to_checked_ptr = checked_ptr_to_checked_ptr;     // OK, array_ptr<T> = ptr<T>

    array_ptr_to_unchecked_ptr = unchecked_ptr_to_unchecked_ptr; // OK
    array_ptr_to_unchecked_ptr = unchecked_ptr_to_checked_ptr;   // expected-error {{incompatible type}}
    array_ptr_to_unchecked_ptr = checked_ptr_to_unchecked_ptr;   // OK , array_ptr<T> = ptr<T>
    array_ptr_to_unchecked_ptr = checked_ptr_to_checked_ptr;     // expected-error {{incompatible type}}
}

// Test assignments between different kinds of pointers where the
// the source and/or destination pointers are pointers to void.

// Unchecked scope
extern void
check_assign_void_unchecked(int val, int *p, ptr<int> q,
                            array_ptr<int> r : count(1),
                            void *s, ptr<void> t,
                            array_ptr<void> u : byte_count(sizeof(int)),
                            nt_array_ptr<int> v : count(1)) {

    // pointer to void = pointer to integer for the different kinds of pointers
    void *t1 = p;            // void *  = T * OK;
    ptr<void> t2 = &val;     // ptr<void> = T * OK when T * has known bounds
    ptr<void> t3 = q;        // ptr<void> = ptr<T> OK
    ptr<void> t4 = &val;     // ptr<void> = T * OK provided T * has known bounds.
    ptr<void> t4a = r;       // ptr<void> = array_ptr<T> OK.
    ptr<void> t4b = v;       // ptr<void> = nt_array_ptr<T> OK.
    array_ptr<void> t5 = p;  // array_ptr<void> = T * OK
    array_ptr<void> t6 = r;  // array_ptr<void> = array_ptr<T> OK
    array_ptr<void> t6a = v; // array_ptr<void> = nt_array_ptr<T> OK
    // nt_array_ptr<void> is not legal, so we can skip testing it as the
    // left-hand side of an assignment.
    nt_array_ptr<void> t6b = 0; // expected-error {{only integer and pointer types are allowed}}

    // pointer to void = pointer to void for the different kinds of pointers
    void *t7 = s;            // void * = void * OK.
    void *t8 = t;            // expected-error {{incompatible type}}
                             // void * = ptr<void> not OK
    void *t9 = u;            // expected-error {{incompatible type}}
                             // void * = array_ptr<void> not OK.
    ptr<void> t10 = (void *) &val; // ptr<void> = void * OK provided void * has known bounds
    ptr<void> t11 = t;       // ptr<void> = ptr<void> OK
    ptr<void> t12 = u;
                             // ptr<void> = array_ptr<void> OK.
    array_ptr<void> t13 = u; // array_ptr<void> = void * OK when array_ptr has no
                             // bounds
    array_ptr<void> t14 = t; // array_ptr<void> = ptr<void> OK.
    array_ptr<void> t15 = u; // array_ptr<void> = array_ptr<void> OK

    // pointer to integer = pointer to void for the different kinds of pointers.
    int *t17 = s;            // T * = void * OK
    int *t18 = t;            // expected-error {{incompatible type}}
                             // int * = ptr<void> not OK.
    int *t19 = u;            // expected-error {{incompatible type}}
                             // int * = array_ptr<void> not OK.
    ptr<int> t20 = (void *) &val; // ptr<int> = void * OK provided void * has known bounds
    ptr<int> t21 = t;        // expected-error {{incompatible type}}
                             // ptr<int> = ptr<void> not OK.
    ptr<int> t22 = u;
    array_ptr<int> t23 = s;  // array_ptr<int> = void * OK when array_ptr<int> has no
                             // bounds.
    array_ptr<int> t24 = t;  // expected-error {{incompatible type}}
                             // array_ptr<int> = ptr<void> not OK.
    array_ptr<int> t25 = u;
    nt_array_ptr<int> t25a = (void *)&val; // expected-error {{incompatible type}}
                                           // nt_array_ptr<int> = void * not OK.
    int* _Nt_array t25b = s;  // expected-error {{incompatible type}}
                                           // int* _Nt_array = void * not OK, even with obunds
    int* _Nt_array t25c = t;  // expected-error {{incompatible type}}
                                           // int* _Nt_array = void* _Single not OK.
    int* _Nt_array t25d = u;  // expected-error {{incompatible type}}
                                           // int* _Nt_array = void* _Array not OK.
    // conversions between integers and safe void pointers.
    int t26 = t;             // expected-error {{incompatible type}}
                             // int = ptr<void> not OK;
    int t27 = u;             // expected-error {{incompatible type}}
                             // int = array_ptr<void> not OK;
    ptr<void> t28 = val;     // expected-error {{incompatible type}}
                             // array_ptr<void> = int not OK
    array_ptr<void> t29 = val; // expected-error {{incompatible type}}
                               // array_ptr<void> = int not OK

    // spot check converting a pointer to a float
    float t30 = t;           // expected-error {{incompatible type}}
                             // float = ptr<T> not OK
    float t31 = u;           // expected-error {{incompatible type}}
                             // float = array_ptr<T> not OK

    // Implicit conversion of a safe void pointer type to _Bool is OK.
    _Bool t32 = t;
    _Bool t33 = u;

    // _Bool to safe void pointer is not OK.
    ptr<void> t34 = (_Bool)(1);   // expected-error {{incompatible type}}
    array_ptr<void> t35 = (_Bool)(1);   // expected-error {{incompatible type}}

    // Implicit conversion of 0 to a safe void pointer type is OK.
    void* _Single t37 = 0;
    void* _Array t38 = 0;
}

// Test assignments between different kinds of pointers where the
// the source and/or destination pointers are pointers to void.

// Checked scope
struct CheckedData1 {
   int len;
   int* _Array p : count(len);
};

extern void
check_assign_void_checked(int val, ptr<int> p,
                          array_ptr<int> q : count(1),
                          array_ptr<void> r : byte_count(sizeof(int)),
                          array_ptr<void> r2 : byte_count(sizeof(struct CheckedData1)),
                          array_ptr<void> r3 : byte_count(sizeof(ptr<int>)),
                          nt_array_ptr<int> s : count(1),
                          ptr<struct CheckedData1> t,
                          ptr<ptr<int>> u,
                          array_ptr<ptr<int> checked[5]> v : count(1)) checked {
    // pointer to void = pointer to integer for the different kinds of pointers
    // allowed in checked scopes
    array_ptr<void> t1 = p;  // array_ptr<void> = ptr<int> OK
    array_ptr<void> t2 = q;  // array_ptr<void> = array_ptr<int> OK
    array_ptr<void> t3 = s;  // array_ptr<void> = nt_array_ptr<int> OK
    array_ptr<void> t4 = t;  // expected-error {{not allowed in a checked scope}}
                             // array_ptr<void = array_ptr<T> where T contains
                             // a checked pointer
    array_ptr<void> t5 = u;  // expected-error {{not allowed in a checked scope}}
                             // array_ptr<void = array_ptr<T> where T contains
                             // a checked pointer
    array_ptr<void> t6 = v;  // expected-error {{not allowed in a checked scope}}
                             // array_ptr<void = array_ptr<T> where T contains
                             // a checked pointer
    // nt_array_ptr<void> is not legal, so we can skip testing it as the
    // left-hand side of an assignment.
    nt_array_ptr<void> t7 = 0; // expected-error {{only integer and pointer types are allowed}}

    // pointer to void = pointer to void for the different kinds of pointers
    array_ptr<void> t8 = r; // array_ptr<void> = void * OK when array_ptr has no
                             // bounds
    array_ptr<void> t9 = r; // array_ptr<void> = array_ptr<void> OK

    ptr<int> t20 = r;
    array_ptr<int> t21 = r;
    nt_array_ptr<int> t22 = r;  // expected-error {{incompatible type}}
                                 // nt_array_ptr<int> = array_ptr<void> not OK.
    array_ptr<struct CheckedData1> t23 = r2;  // expected-error {{not allowed in a checked scope}}
                                          // array_ptr<T> = array_ptr<void> where T contains
                                          // a checked pointer
    ptr<ptr<int>> t24 = r3; // expected-error {{not allowed in a checked scope}}
                            // array_ptr<T> = array_ptr<void> where T contains
                            // a checked pointer
    ptr<ptr<int> checked[1]> t25 = r3;  // expected-error {{not allowed in a checked scope}}
                                         // array_ptr<T> = array_ptr<void> where T contains
                                         // a checked pointer
    // conversions between integers and safe void pointers.
    int t26 = r;             // expected-error {{incompatible type}}
                             // int = array_ptr<void> not OK;
    array_ptr<void> t27 = val; // expected-error {{incompatible type}}
                               // array_ptr<void> = int not OK

    // spot check converting a pointer to a float
    float t31 = r;           // expected-error {{incompatible type}}
                             // float = array_ptr<T> not OK

    // Implicit conversion of a safe void pointer type to _Bool is OK.
    _Bool t32 = r;

    // _Bool to safe void pointer is not OK.
    array_ptr<void> t33 = (_Bool)(1);   // expected-error {{incompatible type}}

    // Implicit conversion of 0 to a safe void pointer type is OK.
    array_ptr<void> t34 = 0;
}

extern void
check_assign_void_checked_bounds_only(int val, ptr<int> p,
                          array_ptr<int> q : count(1),
                          array_ptr<void> r : byte_count(sizeof(int)),
                          array_ptr<void> r2 : byte_count(sizeof(struct CheckedData1)),
                          array_ptr<void> r3 : byte_count(sizeof(ptr<int>)),
                          nt_array_ptr<int> s : count(1),
                          ptr<struct CheckedData1> t,
                          ptr<ptr<int>> u) checked _Bounds_only {
    // pointer to void = pointer to integer for the different kinds of pointers
    // allowed in checked scopes
    array_ptr<void> t1 = p;  // array_ptr<void> = ptr<int> OK
    array_ptr<void> t2 = q;  // array_ptr<void> = array_ptr<int> OK
    array_ptr<void> t3 = s;  // array_ptr<void> = nt_array_ptr<int> OK
    array_ptr<void> t4 = t;  // array_ptr<void = array_ptr<T> OK where T contains
                             // a checked pointer
    array_ptr<void> t5 = u;  // array_ptr<void = array_ptr<T> OK where T contains
                             // a checked pointer
    // nt_array_ptr<void> is not legal, so we can skip testing it as the
    // left-hand side of an assignment.
    nt_array_ptr<void> t6 = 0; // expected-error {{only integer and pointer types are allowed}}

    // pointer to void = pointer to void for the different kinds of pointers
    array_ptr<void> t7 = r; // array_ptr<void> = void * OK when array_ptr has no
                             // bounds
    array_ptr<void> t8 = r; // array_ptr<void> = array_ptr<void> OK
    ptr<int> t20 = r;
    array_ptr<int> t21 = r;
    nt_array_ptr<int> t22 = r;  // expected-error {{incompatible type}}
                                 // nt_array_ptr<int> = array_ptr<void> not OK.
    array_ptr<struct CheckedData1> t23 = r2;  // array_ptr<T> = array_ptr<void> OK where T contains
                                              // a checked pointer
    ptr<ptr<int>> t24 = r3; // array_ptr<T> = array_ptr<void> OK where T contains
                            // a checked pointer
    // conversions between integers and safe void pointers.
    int t25 = r;             // expected-error {{incompatible type}}
                             // int = array_ptr<void> not OK;
    array_ptr<void> t26 = val; // expected-error {{incompatible type}}
                               // array_ptr<void> = int not OK

    // spot check converting a pointer to a float
    float t31 = r;           // expected-error {{incompatible type}}
                             // float = array_ptr<T> not OK

    // Implicit conversion of a safe void pointer type to _Bool is OK.
    _Bool t32 = r;

    // _Bool to safe void pointer is not OK.
    array_ptr<void> t33 = (_Bool)(1);   // expected-error {{incompatible type}}

    // Implicit conversion of 0 to a safe void pointer type is OK.
    array_ptr<void> t34 = 0;
}


// Test assignments between pointers of different kinds with const/volatile
// attributes on referent types
extern void check_assign_cv(void) {
    int val = 0;
    const int const_val = 0;
    volatile int volatile_val = 0;
    int *p = 0;
    const int *p_const = 0;
    volatile int *p_volatile = 0;
    ptr<int> q = 0;
    ptr<const int> q_const = 0;
    ptr<volatile int> q_volatile = 0;
    array_ptr<int> r = 0;
    array_ptr<const int> r_const = 0;
    array_ptr<volatile int> r_volatile = 0;
    nt_array_ptr<int> s = 0;
    nt_array_ptr<const int> s_const = 0;
    nt_array_ptr<volatile int> s_volatile = 0;

    p_const = p;    // unsafe pointer to const assigned unsafe pointer to non-const OK.
    q_const = q;    // ptr to const assigned ptr to non-const OK.
    r_const = r;    // array_ptr to const assigned array_ptr to non-const OK.
    q_const = &val; // ptr to const assigned unsafe pointer OK, provided unsafe pointer
                    // has known bounds.
    r_const = p_const; // array_ptr to const assigned unsafe pointer to const OK,
                       // provided array_ptr has unknown bounds.
    r_const = &val; // array_ptr to const assigned unsafe pointer OK, provided array_ptr
                    // has unknown bounds.
    s_const = s;    //  nt_array_ptr to const assigned non-const OK.

    p = p_const;    // expected-warning {{discards qualifiers}}
    q = q_const;    // expected-warning {{discards qualifiers}}
                    // ptr assigned to ptr to const int
    r = r_const;    // expected-warning {{discards qualifiers}}
    q = &const_val; // expected-warning {{discards qualifiers}}
    r = &const_val; // expected-warning {{discards qualifiers}}
    r = p_const;    // expected-warning {{discards qualifiers}}
    s = s_const;    // expected-warning {{discards qualifiers}}

    p_volatile = p; // unsafe pointer to volatile assigned unsafe pointer to non-volatile OK.
    q_volatile = q; // ptr to volatile assigned ptr to non-volatile OK.
    r_volatile = r; // array_ptr to volatile assigned array_ptr to non-volatile OK.
    s_volatile = s; // nt_array_ptr to volatile assigned array_ptr to non-volatile OK.

    q_volatile = &val; // ptr to volatile assigned unsafe pointer OK, provided unsafe pointer
                       // has known bounds.
    r_volatile = p_volatile; // array_ptr to volatile assigned unsafe pointer to volatile OK,
                             // provided array_ptr has unknown bounds.
    r_volatile = &val; // array_ptr to volatile assigned unsafe pointer OK, provided array_ptr
                       // has unknown bounds.
    p = p_volatile;    // expected-warning {{discards qualifiers}}
    q = q_volatile;    // expected-warning {{discards qualifiers}}
                       // ptr assigned to ptr to volatile int
    r = r_volatile;    // expected-warning {{discards qualifiers}}
    q = &volatile_val; // expected-warning {{discards qualifiers}}
    r = &volatile_val; // expected-warning {{discards qualifiers}}
    r = p_volatile;    // expected-warning {{discards qualifiers}}
    s = s_volatile;    // expected-warning {{discards qualifiers}}
}

// Test conditional expressions where arms have different kinds of
// pointer types, excluding pointers to void.
extern void check_condexpr(int val, int *p, ptr<int> q, array_ptr<int> r,
                           float *s, ptr<float> t, array_ptr<float> u,
                           nt_array_ptr<int> v) {
   float fval;
   int *t1 = val ? p : p;            // T * and T * OK;
   int *t2 = val ? &val : q;         // expected-error {{initializing 'int *' with an expression of incompatible type '_Ptr<int>'}}
                                     // T * and ptr<T> produce ptr<T>
   int *t3 = val ? q : &val;         // expected-error {{initializing 'int *' with an expression of incompatible type '_Ptr<int>'}}
                                     // T * and ptr<T> produce ptr<T>
   ptr<int> t4 = val ? &val : q;     // T * and ptr<T> OK when T has known bounds
   ptr<int> t5 = val ? q : &val;     // ptr<T> and T * OK when T has known bounds
   int *t6 = val ? p : r;            // expected-error {{initializing 'int *' with an expression of incompatible type '_Array_ptr<int>'}}
                                     // T * and array_ptr<T> produce array_ptr<T>
   int *t7 = val ? r : p;            // expected-error {{initializing 'int *' with an expression of incompatible type '_Array_ptr<int>'}}
                                     // array_ptr<T> and T * produce array_ptr<T>
   array_ptr<int> t8 = val ? p : r;  // T * and array_ptr<T> OK when array_ptr<T> has unknown bounds
   array_ptr<int> t9 = val ? r : p;  // array_ptr<T> and T * OK when array_ptr<T> has unknown bounds
   ptr<int> t10 = val ? q : q;       // ptr<T> and ptr<T> OK
   array_ptr<int> t11 = val ? r : r;  // array_ptr<T> and array_ptr<T> OK
   array_ptr<int> t12 = val ? v : v; // nt_array_ptr<T> and nt_array_ptr<T> OK

   // omit assignment because type of expression is not valid when there is an error.
   val ? &fval : q; // expected-error {{pointer type mismatch}}
                    // S * and ptr<T> not OK
   val ? q : &fval; // expected-error {{pointer type mismatch}}
                    // ptr<T> and S * not OK
   val ? &fval : r; // expected-error {{pointer type mismatch}}
                    // S * and array_ptr<T> not OK;
   val ? r : &fval; // expected-error {{pointer type mismatch}}
                    // array_ptr<T> and S * not OK;
   val ? &fval : v; // expected-error {{pointer type mismatch}}
                    // nt_array_ptr<T> and S * not OK;
   val ? v : &fval; // expected-error {{pointer type mismatch}}
                    // nt_array_ptr<T> and S * not OK;
   val ? q : t;     // expected-error {{pointer type mismatch}}
                    // ptr<T> and ptr<S> not OK
   val ? t : q;     // expected-error {{pointer type mismatch}}
                    // ptr<S> and ptr<T> not OK
   val ? q : r;     // expected-error {{pointer type mismatch}}
                    // ptr<T> and array_ptr<T> not OK
   val ? r : q;     // expected-error {{pointer type mismatch}}
                    // array_ptr<T> and ptr<T> not OK
   val ? q : v;     // expected-error {{pointer type mismatch}}
                    // ptr<T> and nt_array_ptr<T> not OK
   val ? v : q;     // expected-error {{pointer type mismatch}}
                    // nt_array_ptr<T> and ptr<T> not OK
   val ? u : q;     // expected-error {{pointer type mismatch}}
                    // array_ptr<T> and ptr<S> not OK;
   val ? q : u;     // expected-error {{pointer type mismatch}}
                    // ptr<T> and array_ptr<S> not OK;
   val ? r : u;     // expected-error {{pointer type mismatch}}
                    // array_ptr<T> and array_ptr<S> not OK
   val ? u : r;     // expected-error {{pointer type mismatch}}
                    // array_ptr<S> and array_ptr<T> not OK

   // Some C compilers have allowed implicit integer to pointer conversions.
   // These are errors for the new safe pointer types.
   val ? q : val;   // expected-error {{incompatible operand types}}
                    // ptr<T> and int not OK
   val ? val : q;   // expected-error {{incompatible operand types}}
                    // int and ptr<T> not OK
   val ? r : val;   // expected-error {{incompatible operand types}}
                    // array_ptr<T> and int not OK
   val ? val : r;   // expected-error {{incompatible operand types}}
                    // int and array_ptr<T> not OK
   val ? t : val;   // expected-error {{incompatible operand types}}
                    // ptr<T> and int not OK
   val ? val : t;   // expected-error {{incompatible operand types}}
                    // int and ptr<T> not OK
   val ? u : val;   // expected-error {{incompatible operand types}}
                    // array_ptr<T> and int not OK
   val ? val : u;   // expected-error {{incompatible operand types}}
                    // int and array_ptr<T> not OK
   val ? v : val;   // expected-error {{incompatible operand types}}
                    // nt_array_ptr<T> and int not OK
   val ? val : v;   // expected-error {{incompatible operand types}}
                    // int and nt_array_ptr<T> not OK

  // Implicit conversion of 0 to a safe pointer type is OK.
   ptr<int> t30 = val ? q : 0;
   ptr<int> t31 = val ? 0 : q;
   array_ptr<int> t32 = val ? r : 0;
   array_ptr<int> t33 = val ? 0 : r;
   nt_array_ptr<int> t34 = val ? v : 0; // expected-warning {{cannot prove declared bounds for 't34' are valid after initialization}}
   nt_array_ptr<int> t35 = val ? 0 : v; // expected-warning {{cannot prove declared bounds for 't35' are valid after initialization}}
   ptr<float> t36 = val ? t : 0;
   ptr<float> t37 = val ? 0 : t;
   array_ptr<float> t38 = val ? u : 0;
   array_ptr<float> t39 = val ? 0 : u;
}

// Test conditional expressions where arms have different kinds of
// pointer types and one or both of the types of the arms is a
// pointer to void.
extern void check_condexpr_void(int val, int *p, ptr<int> q, array_ptr<int> r,
                                void *s : byte_count(sizeof(struct CheckedData1)),
                                ptr<void> t,
                                array_ptr<void> u, nt_array_ptr<int> v,
                                ptr<struct CheckedData1> w,
                                array_ptr<struct CheckedData1> x,
                                struct CheckedData1 *y) {

    // valid combinations of void pointers for the arms of the expressions.
    void *t1 = val ? s : s;           // void * and void * OK
    ptr<void> t2 = val ? s : t;       // void * and ptr<void>  OK
    ptr<void> t2a = val ? t : s;      // ptr<void> and void * OK
    ptr<void> t3 = val ? t : t;       // ptr<void> and ptr<void> OK
    ptr<void> t4 = val ? t : (void *) &val;  // ptr<void> and void * OK
    array_ptr<void> t5 = val ? u : s; // array_ptr<void> and void * OK
    array_ptr<void> t6 = val ? s : u; // void * and array_ptr<void> OK
    array_ptr<void> t7 = val ? u : u; // array_ptr<void> and array_ptr<void> OK

    // valid combinations of void pointer and int pointers for the arms of the expression
    void *t8 = val ? s : p;            // void * and int * OK
    void *t9 = val ? p : s;            // int * and void * OK
    void *t10 = val ? y : s;           // struct CheckedData1 * and void * OK, where CheckedData1
                                       // contains a checked pointer.
    void *t11 = val ? s : y;           // void * struct CheckedData1 * OK.
    ptr<void> t14 = val ? t : &val;    // ptr<void> and int * OK when int * has bounds of at least 1 byte
    ptr<void> t15 = val ? &val : t;    // int * and ptr<void> OK when int * has bounds of at least 1 byte
    array_ptr<void> t17 = val ? u : p; // array_ptr<void> and int * OK when array_ptr has unknown bounds
    array_ptr<void> t18 = val ? p : u; // int * and array_ptr<void> OK when array_ptr has unknown bounds
    array_ptr<void> t19 = val ? u : y; // array_ptr<void> and struct CheckedData1 * OK when
                                       // array_ptr has unknown bounds.
    array_ptr<void> t20 = val ? y : u; // struct CheckedData1 * and array_ptr<void> OK when
                                       // array_ptr has unknown bounds.
    ptr<void> t21 = val ? t : q;       // ptr<void> and ptr<int> OK
    ptr<void> t22 = val ? q : t;       // ptr<int> and ptr<void> OK
    ptr<void> t23 = val ? w : t;       // ptr<struct CheckedData1> and ptr<void> OK.
    ptr<void> t24 = val ? t : w;       // ptr<void> and ptr<struct CheckedData1> OK.
    array_ptr<void> t25 = val ? u : r; // array_ptr<void> and array_ptr<int> OK
    array_ptr<void> t26 = val ? r : u; // array_ptr<int> and array_ptr<void> OK
    array_ptr<void> t27 = val ? u : x; // array_ptr<struct CheckedData1> and array_ptr<int> OK
    array_ptr<void> t28 = val ? x : u; // array_ptr<void> and array_ptr<struct CheckedData1> OK

    // omit assignment because type of expression is not valid when there is an error

    // invalid combinations of void pointer types
    val ? t : u;   // expected-error {{pointer type mismatch}}
                   // ptr<void> and array_ptr<void> not OK.
    val ? u : t;   // expected-error {{pointer type mismatch}}
                   // array_ptr<void> and ptr<void> not OK.

    // Invalid combinations of pointer to int and void pointer types

    // According to the C11 standard, section 6.5.15 (conditional
    // operator), if one arm of a conditional expression is a pointer
    // to void and the other arm is another pointer type, the type of
    // the entire expression is pointer to void.  That implies that
    // the other arm is implicitly cast to the void pointer type.
    // Checked C only allows implicit conversions of safe types to
    // unsafe types at bounds-safe interfaces.  If one arm is void *
    // and the other is a safe pointer type, this is a typechecking error.
    //
    // Checked C also requires that for implicit conversions between
    // safe types and safe pointers to void, the pointer kind must
    // match.
    val ? (void *) &val : q;   // expected-error {{pointer type mismatch}}
                   // void * and ptr<int> not OK.
    val ? q : (void *) &val;   // expected-error {{pointer type mismatch}}
                   // ptr<int> and void * not OK.
    val ? s : r;   // expected-error {{pointer type mismatch}}
                   // void * and array_ptr<int> not OK
    val ? r : s;   // expected-error {{pointer type mismatch}}
                   // array_ptr<int> and void * not OK.
    val ? s : v;   // expected-error {{pointer type mismatch}}
                   // void * and nt_array_ptr<int> not OK
    val ? v : s;   // expected-error {{pointer type mismatch}}
                   // nt_array_ptr<int> and void * not OK.
    val ? t : r;   // expected-error {{pointer type mismatch}}
                   // ptr<void> and array_ptr<int> not OK
    val ? r : t;   // expected-error {{pointer type mismatch}}
                   // array_ptr<int> and ptr<void> not OK
    val ? t : v;   // expected-error {{pointer type mismatch}}
                   // ptr<void> and nt_array_ptr<int> not OK
    val ? v : t;   // expected-error {{pointer type mismatch}}
                   // nt_array_ptr<int> and ptr<void> not OK
    val ? u : q;   // expected-error {{pointer type mismatch}}
                   // array_ptr<void> and ptr<int> not OK
    val ? q : u;   // expected-error {{pointer type mismatch}}
                   // ptr<int> and array_ptr<void> not OK

     // Spot check invalid combinations of pointer to struct and
     // void pointer data types
     val ? s : w;   // expected-error {{pointer type mismatch}}
                    // void * and ptr<struct CheckedData1> not OK.
     val ? x : s;   // expected-error {{pointer type mismatch}}
                    // array_ptr<struct CheckedData1> and void * not OK.

    // Some C compilers have allowed implicit integer to pointer conversions.
    // These are errors for new safe pointer types to void
    val ? t : val;   // expected-error {{incompatible operand types}}
                     // ptr<void> and int not OK
    val ? val : t;   // expected-error {{incompatible operand types}}
                     // int and ptr<void> not OK
    val ? u : val;   // expected-error {{incompatible operand types}}
                     // array_ptr<void> and int not OK
    val ? val : u;   // expected-error {{incompatible operand types}}
                     // int  and array_ptr<void> not OK

    // Implicit conversion of 0 to a void safe pointer type is OK.
    ptr<void> t40 = val ? t : 0;
    ptr<void> t41 = val ? 0 : t;
    array_ptr<void> t42 = val ? u : 0;
    array_ptr<void> t43 = val ? 0 : u;
}

// Test conditional expressions where arms have different kinds of
// pointer types and one or both of the types of the arms is a
// pointer to void, a checked bounds_only scope.
checked bounds_only extern void
check_condexpr_void_bounds_only(int val, ptr<int> q, array_ptr<int> r,
                                ptr<void> t,
                                array_ptr<void> u, nt_array_ptr<int> v,
                                ptr<struct CheckedData1> w,
                                array_ptr<struct CheckedData1> x) {

    // valid combinations of void pointers for the arms of the expressions.
    ptr<void> t3 = val ? t : t;       // ptr<void> and ptr<void> OK
    array_ptr<void> t7 = val ? u : u; // array_ptr<void> and array_ptr<void> OK

    // valid combinations of void pointer and int pointers for the arms of the expression
    ptr<void> t21 = val ? t : q;       // ptr<void> and ptr<int> OK
    ptr<void> t22 = val ? q : t;       // ptr<int> and ptr<void> OK
    ptr<void> t23 = val ? w : t;       // ptr<struct CheckedData1> and ptr<void> OK.
    ptr<void> t24 = val ? t : w;       // ptr<void> and ptr<struct CheckedData1> OK.
    array_ptr<void> t25 = val ? u : r; // array_ptr<void> and array_ptr<int> OK
    array_ptr<void> t26 = val ? r : u; // array_ptr<int> and array_ptr<void> OK
    array_ptr<void> t27 = val ? u : x; // array_ptr<struct CheckedData1> and array_ptr<int> OK
    array_ptr<void> t28 = val ? x : u; // array_ptr<void> and array_ptr<struct CheckedData1> OK

    // omit assignment because type of expression is not valid when there is an error

    // invalid combinations of void pointer types
    val ? t : u;   // expected-error {{pointer type mismatch}}
                   // ptr<void> and array_ptr<void> not OK.
    val ? u : t;   // expected-error {{pointer type mismatch}}
                   // array_ptr<void> and ptr<void> not OK.

    // Invalid combinations of pointer to int and void pointer types

    // According to the C11 standard, section 6.5.15 (conditional
    // operator), if one arm of a conditional expression is a pointer
    // to void and the other arm is another pointer type, the type of
    // the entire expression is pointer to void.  That implies that
    // the other arm is implicitly cast to the void pointer type.
    // Checked C only allows implicit conversions of safe types to
    // unsafe types at bounds-safe interfaces.  If one arm is void *
    // and the other is a safe pointer type, this is a typechecking error.
    //
    // Checked C also requires that for implicit conversions between
    // safe types and safe pointers to void, the pointer kind must
    // match.
    val ? t : r;   // expected-error {{pointer type mismatch}}
                   // ptr<void> and array_ptr<int> not OK
    val ? r : t;   // expected-error {{pointer type mismatch}}
                   // array_ptr<int> and ptr<void> not OK
    val ? t : v;   // expected-error {{pointer type mismatch}}
                   // ptr<void> and nt_array_ptr<int> not OK
    val ? v : t;   // expected-error {{pointer type mismatch}}
                   // nt_array_ptr<int> and ptr<void> not OK
    val ? u : q;   // expected-error {{pointer type mismatch}}
                   // array_ptr<void> and ptr<int> not OK
    val ? q : u;   // expected-error {{pointer type mismatch}}
                   // ptr<int> and array_ptr<void> not OK

    // Some C compilers have allowed implicit integer to pointer conversions.
    // These are errors for new safe pointer types to void
    val ? t : val;   // expected-error {{incompatible operand types}}
                     // ptr<void> and int not OK
    val ? val : t;   // expected-error {{incompatible operand types}}
                     // int and ptr<void> not OK
    val ? u : val;   // expected-error {{incompatible operand types}}
                     // array_ptr<void> and int not OK
    val ? val : u;   // expected-error {{incompatible operand types}}
                     // int  and array_ptr<void> not OK

    // Implicit conversion of 0 to a void safe pointer type is OK.
    ptr<void> t40 = val ? t : 0;
    ptr<void> t41 = val ? 0 : t;
    array_ptr<void> t42 = val ? u : 0;
    array_ptr<void> t43 = val ? 0 : u;
}

// Test conditional expressions where arms have different kinds of
// pointer types and one or both of the types of the arms is a
// pointer to void, in a checked scope.
checked extern void
check_condexpr_void_checked(int val, ptr<int> q, array_ptr<int> r,
                            ptr<void> t,
                            array_ptr<void> u, nt_array_ptr<int> v,
                            ptr<struct CheckedData1> w,
                            array_ptr<struct CheckedData1> x) {

    // valid combinations of void pointers for the arms of the expressions.
    ptr<void> t3 = val ? t : t;       // ptr<void> and ptr<void> OK
    array_ptr<void> t7 = val ? u : u; // array_ptr<void> and array_ptr<void> OK

    // combinations of void pointer and int pointers for the arms of the expression
    // that are valid in some kinds of checked scopes.
    ptr<void> t21 = val ? t : q;       // ptr<void> and ptr<int> OK
    ptr<void> t22 = val ? q : t;       // ptr<int> and ptr<void> OK
    ptr<void> t23 = val ? w : t;       // expected-error {{not allowed in a checked scope}}
                                       // ptr<struct CheckedData1> and ptr<void> not OK
                                       // because CheckedData1 has a checked pointer in it.
    ptr<void> t24 = val ? t : w;       // expected-error {{not allowed in a checked scope}}
                                       // ptr<void> and ptr<struct CheckedData1> not OK
                                       // because CheckedData1 has a checked pointer in it.
    array_ptr<void> t25 = val ? u : r; // array_ptr<void> and array_ptr<int> OK
    array_ptr<void> t26 = val ? r : u; // array_ptr<int> and array_ptr<void> OK
    array_ptr<void> t27 = val ? u : x; // expected-error {{not allowed in a checked scope}}
                                       // array_ptr<struct CheckedData1> and array_ptr<void> not OK
                                       // because CheckedData1 has a checked pointer in it.
    array_ptr<void> t28 = val ? x : u; // expected-error {{not allowed in a checked scope}}
                                       // array_ptr<void> and array_ptr<struct CheckedData1> not OK

    // omit assignment because type of expression is not valid when there is an error

    // invalid combinations of void pointer types
    val ? t : u;   // expected-error {{pointer type mismatch}}
                   // ptr<void> and array_ptr<void> not OK.
    val ? u : t;   // expected-error {{pointer type mismatch}}
                   // array_ptr<void> and ptr<void> not OK.

    // Invalid combinations of pointer to int and void pointer types

    // According to the C11 standard, section 6.5.15 (conditional
    // operator), if one arm of a conditional expression is a pointer
    // to void and the other arm is another pointer type, the type of
    // the entire expression is pointer to void.  That implies that
    // the other arm is implicitly cast to the void pointer type.
    // Checked C only allows implicit conversions of safe types to
    // unsafe types at bounds-safe interfaces.  If one arm is void *
    // and the other is a safe pointer type, this is a typechecking error.
    //
    // Checked C also requires that for implicit conversions between
    // safe types and safe pointers to void, the pointer kind must
    // match.
    val ? t : r;   // expected-error {{pointer type mismatch}}
                   // ptr<void> and array_ptr<int> not OK
    val ? r : t;   // expected-error {{pointer type mismatch}}
                   // array_ptr<int> and ptr<void> not OK
    val ? t : v;   // expected-error {{pointer type mismatch}}
                   // ptr<void> and nt_array_ptr<int> not OK
    val ? v : t;   // expected-error {{pointer type mismatch}}
                   // nt_array_ptr<int> and ptr<void> not OK
    val ? u : q;   // expected-error {{pointer type mismatch}}
                   // array_ptr<void> and ptr<int> not OK
    val ? q : u;   // expected-error {{pointer type mismatch}}
                   // ptr<int> and array_ptr<void> not OK

    // Some C compilers have allowed implicit integer to pointer conversions.
    // These are errors for new safe pointer types to void
    val ? t : val;   // expected-error {{incompatible operand types}}
                     // ptr<void> and int not OK
    val ? val : t;   // expected-error {{incompatible operand types}}
                     // int and ptr<void> not OK
    val ? u : val;   // expected-error {{incompatible operand types}}
                     // array_ptr<void> and int not OK
    val ? val : u;   // expected-error {{incompatible operand types}}
                     // int  and array_ptr<void> not OK

    // Implicit conversion of 0 to a void safe pointer type is OK.
    ptr<void> t40 = val ? t : 0;
    ptr<void> t41 = val ? 0 : t;
    array_ptr<void> t42 = val ? u : 0;
    array_ptr<void> t43 = val ? 0 : u;
}

// Test conditional expressions where arms have different kinds of
// pointer types and const/volatile modifiers.
extern void check_condexpr_cv(void)
{
  int val = 0;
  const int const_val = 0;
  volatile int volatile_val = 0;
  int *p = 0;
  const int *p_const = 0;
  volatile int *p_volatile = 0;
  ptr<int> q = 0;
  ptr<const int> q_const = 0;
  ptr<volatile int> q_volatile = 0;
  array_ptr<int> r = 0;
  array_ptr<const int> r_const = 0;
  array_ptr<volatile int> r_volatile = 0;
  nt_array_ptr<int> s = 0;
  nt_array_ptr<const int> s_const = 0;
  nt_array_ptr<volatile int> s_volatile = 0;

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

  ptr<const int> t7 = val ? &val : q_const;   // int * and ptr<const int> OK
  ptr<const int> t8 = val ? q_const : &val;   // ptr<const int> and int * OK
  ptr<const int> t9 = val ? &const_val : q;   // const int * and ptr<int> OK
  ptr<const int> t10 = val ? q : &const_val;  // ptr<int> and const int * OK
  ptr<const int> t11 = val ? &const_val : q_const;  // const int * and ptr<const int> OK
  ptr<const int> t12 = val ? q_const : &const_val;  // ptr<const int> and const int * OK
  ptr<const int> t13 = val ? q : q_const;        // ptr<int> and ptr<const int> OK
  ptr<const int> t14 = val ? q_const : q;  // ptr<const int> and ptr<int> OK
  ptr<const int> t15 = val ? q_const : q_const;  // ptr<const int> and ptr<const int> OK

  ptr<int> t16 = val ? &val : q_const;     // expected-warning {{discards qualifiers}}
                                           // int * and ptr<const int> produce ptr<const int>
  ptr<int> t17 = val ? q_const : &val;     // expected-warning {{discards qualifiers}}
                                           // ptr<const int> and int * produce ptr<const int>
  ptr<int> t18 = val ? &const_val : q;     // expected-warning {{discards qualifiers}}
                                           // const int * and ptr<int> produce ptr<const int>
  ptr<int> t19 = val ? q : &const_val;     // expected-warning {{discards qualifiers}}
                                           // ptr<int> and const int * produce ptr<const int>
  ptr<int> t20 = val ? &const_val : q_const;   // expected-warning {{discards qualifiers}}
                                           // const int * and ptr<const int> produce ptr<const int>
  ptr<int> t21 = val ? q_const : &const_val;   // expected-warning {{discards qualifiers}}
                                           // ptr<const int> and const int * produce ptr<const int>
  ptr<int> t22 = val ? q : q_const;        // expected-warning {{discards qualifiers}}
                                           // ptr<int> and ptr<const int> produce ptr<const int>
  ptr<int> t23 = val ? q_const : q;        // expected-warning {{discards qualifiers}}
                                           // ptr<const int> and ptr<int> produce ptr<const int>
  ptr<int> t24 = val ? q_const : q_const;        // expected-warning {{discards qualifiers}}
                                           // ptr<const int> and ptr<const int> produce ptr<const int>

  array_ptr<const int> t25 = val ? p : r_const;       // int * and array_ptr<const int> OK
  array_ptr<const int> t26 = val ? r_const : p;       // array_ptr<const int> and int * OK
  array_ptr<const int> t27 = val ? p_const : r;       // const int * and array_ptr<int> OK
  array_ptr<const int> t28 = val ? r : p_const;       // array_ptr<int> and const int * OK
  array_ptr<const int> t29 = val ? p_const : r_const; // const int * and array_ptr<const int> OK
  array_ptr<const int> t30 = val ? r_const : p_const; // array_ptr<const int> and const int * OK
  array_ptr<const int> t31 = val ? r : r_const;       // array_ptr<int> and array_ptr<const int> OK
  array_ptr<const int> t32 = val ? r_const : r;       // array_ptr<const int> and array_ptr<int> OK
  array_ptr<const int> t33 = val ? r_const : r_const; // array_ptr<const int> and array_ptr<const int> OK
                                                          // nt_array_ptr<int> and nt_array_ptr<const int> OK
  nt_array_ptr<const int> t31a = val ? s : s_const;       // expected-error {{inferred bounds for 't31a' are unknown after initialization}}
                                                          // nt_array_ptr<const int> and nt_array_ptr<int> OK
  nt_array_ptr<const int> t32a = val ? s_const : s;       // expected-error {{inferred bounds for 't32a' are unknown after initialization}}
  nt_array_ptr<const int> t33a = val ? s_const : s_const; // nt_array_ptr<const int> and nt_array_ptr<const int> OK

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
  array_ptr<int> t40a = val ? s : s_const;  // expected-warning {{discards qualifiers}}
                                            // nt_array_ptr<int> and nt_array_ptr<const int> produce nt_array_ptr<const int>
  array_ptr<int> t41a = val ? s_const : s;  // expected-warning {{discards qualifiers}}
                                            // nt_array_ptr<const int> and nt_array_ptr<int> produce array_ptr<const int>
  array_ptr<int> t42a = val ? r_const : r_const;   // expected-warning {{discards qualifiers}}
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

  ptr<volatile int> t56 = val ? &val : q_volatile;       // int * and ptr<volatile int> OK
  ptr<volatile int> t57 = val ? q_volatile : &val;       // ptr<volatile int> and int * OK
  ptr<volatile int> t58 = val ? &volatile_val : q;       // volatile int * and ptr<int> OK
  ptr<volatile int> t59 = val ? q : &volatile_val;       // ptr<int> and volatile int * OK
  ptr<volatile int> t60 = val ? &volatile_val : q_volatile; // volatile int * and ptr<volatile int> OK
  ptr<volatile int> t61 = val ? q_volatile : &volatile_val; // ptr<volatile int> and volatile int * OK
  ptr<volatile int> t62 = val ? q : q_volatile;          // ptr<int> and ptr<volatile int> OK
  ptr<volatile int> t63 = val ? q_volatile : q;          // ptr<volatile int> and ptr<int> OK
  ptr<volatile int> t64 = val ? q_volatile : q_volatile; // ptr<volatile int> and ptr<volatile int> OK

  ptr<int> t65 = val ? &val : q_volatile;          // expected-warning {{discards qualifiers}}
                                                   // int * and ptr<volatile int> produce ptr<volatile int>
  ptr<int> t66 = val ? q_volatile : &volatile_val; // expected-warning {{discards qualifiers}}
                                                   // ptr<volatile int> and int * produce ptr<volatile int>
  ptr<int> t67 = val ? &volatile_val : q;          // expected-warning {{discards qualifiers}}
                                                   // volatile int * and ptr<int> produce ptr<volatile int>
  ptr<int> t68 = val ? q : &volatile_val;          // expected-warning {{discards qualifiers}}
                                                   // ptr<int> and volatile int * produce ptr<volatile int>
  ptr<int> t69 = val ? &volatile_val : q_volatile; // expected-warning {{discards qualifiers}}
                                                   // volatile int * and ptr<volatile int> produce ptr<volatile int>
  ptr<int> t70 = val ? q_volatile : &volatile_val; // expected-warning {{discards qualifiers}}
                                                   // ptr<volatile int> and volatile int * produce ptr<volatile int>
  ptr<int> t71 = val ? q : q_volatile;             // expected-warning {{discards qualifiers}}
                                                   // ptr<int> and ptr<volatile int> produce ptr<volatile int>
  ptr<int> t72 = val ? q_volatile : q;             // expected-warning {{discards qualifiers}}
                                                   // ptr<volatile int> and ptr<int> produce ptr<volatile int>
  ptr<int> t73 = val ? q_volatile : q_volatile;    // expected-warning {{discards qualifiers}}
                                                   // ptr<volatile int> and ptr<volatile int> produce ptr<volatile int>

  array_ptr<volatile int> t74 = val ? p : r_volatile;          // int * and array_ptr<volatile int> OK
  array_ptr<volatile int> t75 = val ? r_volatile : p;          // array_ptr<volatile int> and int * OK
  array_ptr<volatile int> t76 = val ? p_volatile : r;          // volatile int * and array_ptr<int> OK
  array_ptr<volatile int> t77 = val ? r : p_volatile;          // array_ptr<int> and volatile int * OK
  array_ptr<volatile int> t78 = val ? p_volatile : r_volatile; // volatile int * and array_ptr<volatile int> OK
  array_ptr<volatile int> t79 = val ? r_volatile : p_volatile; // array_ptr<volatile int> and volatile int * OK
  array_ptr<volatile int> t80 = val ? r : r_volatile;          // array_ptr<int> and array_ptr<volatile int> OK
  array_ptr<volatile int> t81 = val ? r_volatile : r;          // array_ptr<volatile int> and array_ptr<int> OK
  array_ptr<volatile int> t82 = val ? r_volatile : r_volatile; // array_ptr<volatile int> and array_ptr<volatile int> OK
                                                                   // nt_array_ptr<int> and nt_array_ptr<volatile int> OK
  nt_array_ptr<volatile int> t80a = val ? s : s_volatile;          // expected-error {{inferred bounds for 't80a' are unknown after initialization}}
                                                                   // nt_array_ptr<volatile int> and nt_array_ptr<int> OK
  nt_array_ptr<volatile int> t81a = val ? s_volatile : s;          // expected-error {{inferred bounds for 't81a' are unknown after initialization}}
  nt_array_ptr<volatile int> t82b = val ? s_volatile : s_volatile; // nt_array_ptr<volatile int> and nt_array_ptr<volatile int> OK

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
  nt_array_ptr<int> t89a = val ? s : s_volatile;      // expected-error {{inferred bounds for 't89a' are unknown after initialization}} \
                                                      // expected-warning {{discards qualifiers}}
                                                      // nt_array_ptr<int> and nt_array_ptr<volatile int> produce nt_array_ptr<volatile int>
  nt_array_ptr<int> t90a = val ? s_volatile : s;      // expected-error {{inferred bounds for 't90a' are unknown after initialization}} \
                                                      // expected-warning {{discards qualifiers}}
                                                      // nt_array_ptr<volatile int> and nt_array_ptr<int> produce nt_array_ptr<volatile int>
  nt_array_ptr<int> t92a = val ? s_volatile : s_volatile;  // expected-warning {{discards qualifiers}}
                                                       // nt_array_ptr<volatile int> and nt_array_ptr<volatile int> produce nt_array_ptr<volatile int>
}

// Define functions used to test typechecking of call expressions.

extern void f1(int *p, int y) {
    *p = y;
}

extern void f2(ptr<int> p, int y) {
    *p = y;
}

extern void f3(array_ptr<int> p, int y) {
    // can't dereference p because is has unknown bounds
    // just use p in a compare.
     p != 0;
}

extern void f3a(nt_array_ptr<int> p, int y) {
  p != 0;
}

extern void f4(_Bool p, int y) {
}

extern void f5(ptr<struct CheckedData1> p, int y) {
}

extern void f6(array_ptr<struct CheckedData1> p : count(1), int y) {
}

extern void f1_void(void *p, int y) {
}

extern void f2_void(ptr<void> p, int y) {
}

extern void f3_void(array_ptr<void> p, int y) {
}

extern void f1_const(const int *p, int y) {
}

extern void f2_const(ptr<const int> p, int y) {
}

extern void f3_const(array_ptr<const int> p, int y) {
}

extern void f3a_const(array_ptr<const int> p, int y) {
}
// Second parameter is a new pointer type
//

extern void g1(int y, int *p) {
    *p = y;
}

extern void g2(int y, ptr<int> p) {
    *p = y;
}

extern void g3(int y, array_ptr<int> p) {
    // can't dereference p because is has unknown bounds
    // just use p in a compare.
    p != 0;
}

extern void g3a(int y, nt_array_ptr<int> p) {
  p != 0;
}

extern void g4(int y, _Bool p) {
}

//
// returns a new pointer type
//

extern int *h1(void) : itype(ptr<int>) {
    return 0;
}

extern ptr<int> h2(void) {
   return 0;
}

extern array_ptr<int> h3(void) : count(1) {
   return 0;
}

extern nt_array_ptr<int> h3a(void) : count(1) {
  return 0;
}

extern void check_call(void) {
    int val = 0;
    float fval = 0.0;
    int *p = 0;
    int* _Single q = 0;
    int* _Array r : count(1) = 0;


    float *s = 0;
    ptr<float> t = 0;
    array_ptr<float> u = 0;

    nt_array_ptr<int> v : count(1) = 0;
    nt_array_ptr<ptr<int>> w = 0;

    // Test different kinds of pointers where the referent type matches.
    // Type of first parameter is a pointer type.
    f1(q, 0);      // expected-error {{incompatible type}}
                   // param int *, arg ptr<int> not OK
    f1(r, 0);      // expected-error {{incompatible type}}
                   // param int *, arg array_ptr<int> not OK.
    f1(v, 0);      // expected-error {{incompatible type}}
                   // param int *, arg nt_array_ptr<int> not OK.
    f2(q, val);    // param ptr<int>, arg ptr<int> OK.
    f2(&val, 0);   // param ptr<int>, arg int * OK, provided that arg has known bounds.
    f2(r, 0);      // param ptr<int>, arg array_ptr<int> OK, provided that arg has known bounds.
    f2(v, 0);      // param ptr<int>, arg nt_array_ptr<int> OK, provided that arg has known bounds.
    f3(r, val);    // param array_ptr<int>, arg array_ptr<int> OK.
    f3(p, 0);      // param array_ptr<int>, arg int * OK, provided that param has unknown bounds.
    f3(q, 0);      // param array_ptr<int>, arg ptr<int> OK
    f3(&val, 0);   // param array_ptr<int>, arg int * OK, when param has unknown bounds and arg has known bounds
    f3(v, 0);      // param array_ptr<int>, arg nt_array_ptr<int> OK, when param has unknown bounds and arg has known bounds

    f3a(p, val);   // expected-error {{incompatible type}}
                   // param nt_array_ptr<int>, arg int * not OK
    f3a(q, val);   // expected-error {{incompatible type}}
                   // param nt_array_ptr<int>, arg ptr<int> not OK
    f3a(r, val);   // expected-error {{incompatible type}}
                   // param nt_array_ptr<int>, arg array_ptr<int> not OK
    f3a(v, 0);     // param nt_array_ptr<int>, arg nt_array_ptr<int> OK

    // Test different kinds of pointers where the referent type differs.  These are all
    // expected to fail to typecheck.
    f1(t, val);    // expected-error {{incompatible type}}
                   // param int *, arg ptr<float> not OK.
    f1(u, val);    // expected-error {{incompatible type}}
                   // param int *, arg array_ptr<float> not OK.
    f2(t, val);    // expected-error {{incompatible type}}
                   // param ptr<int>, arg ptr<float> not OK.
    f2(u, val);    // expected-error {{incompatible type}}
                   // param ptr<int>, arg array_ptr<float> not OK.
    f2(&fval, 0);  // expected-error {{incompatible type}}
                   // param ptr<int>, arg float * not OK.
    f3(u, val);    // expected-error {{incompatible type}}
                   // param array_ptr<int>, arg array_ptr<float> not OK.
    f3(s, 0);      // expected-error {{incompatible type}}
                   // param array_ptr<int>, arg float * not OK.
    f3(&fval, 0);  // expected-error {{incompatible type}}

    // Second parameter is a new pointer type.  Spot check a few cases to
    // make sure the parameter order doesn't matter.
    g2(val, q);    // param ptr<int>, arg ptr<int> OK.
    g2(0, &val);   // param ptr<int>, arg int * OK, provided that arg has known bounds.
    g3(val, r);    // param array_ptr<int>, arg array_ptr<int> OK.
    g3(0, p);      // param array_ptr<int>, arg int * OK, provided that param has unknown bounds.
    g3(0, &val);   // param array_ptr<int>, arg int * OK, when param has unknown bounds and arg has known bounds
    g3a(0, p);     // expected-error {{incompatible type}}
                   // param array_ptr<int>, arg int * not OK.
    g3a(0, &val);  // expected-error {{incompatible type}}
                   // param array_ptr<int>, arg int * not OK.
    g1(val, t);    // expected-error {{incompatible type}}
                   // param int *, arg ptr<float> not OK.
    g1(val, u);    // expected-error {{incompatible type}}
                   // param int *, arg array_ptr<float> not OK.
    g2(val, t);    // expected-error {{incompatible type}}
                   // param ptr<int>, arg ptr<float> not OK.

    // Null pointer OK
    f1(0, val);
    f2(0, val);
    f3(0, val);
    f3a(0, val);
    g1(val, 0);
    g2(val, 0);
    g3(val, 0);
    g3a(val, 0);

    // Other integers not OK for safe pointers.
    f2(val, val);  // expected-error {{incompatible type}}
    f3(val, val);  // expected-error {{incompatible type}}
    f3a(val, val);  // expected-error {{incompatible type}}
    g2(val, val);  // expected-error {{incompatible type}}
    g3(val, val);  // expected-error {{incompatible type}}
    g3a(val, val);  // expected-error {{incompatible type}}

    // Pointers OK for _Bool arguments
    f4(p, val);
    f4(q, val);
    f4(v, val);
    g4(val, p);
    g4(val, q);
    g4(val, v);

    //
    // Check return values
    //
    int *t1 = h1();
    ptr<int> t2 = h1();
    array_ptr<int> t3 = h1();  // OK, provided that t3 has unknown bounds.
    ptr<int> t4 = h2();
    array_ptr<int> t5 = h3();
    nt_array_ptr<int> t5a = h3a();

    // Conversion of a pointer to a Boolean.
    _Bool t6 = h1();
    _Bool t7 = h2();
    _Bool t8 = h3();
    _Bool t8a = h3a();

    // expected to fail to typecheck
    int *t9 = h2();            // expected-error {{incompatible type}}
                               // int * = ptr<int> not OK
    int *t10 = h3();           // expected-error {{incompatible type}}
                               // int * = array_ptr<int> not OK
    int *t10a = h3a();         // expected-error {{incompatible type}}
                               // int * = nt_array_ptr<int> not OK
    ptr<int> t11 = h3();       // ptr<int> = array_ptr<int> OK
    ptr<int> t11a = h3a();     // ptr<int> = array_ptr<int> OK
    array_ptr<int> t12 = h2(); // array_ptr<int> = ptr<int> OK
    nt_array_ptr<int> t12a = h2(); // expected-error {{incompatible type}}
                                   // array_ptr<int> = ptr<int> OK

    int t13 = h2();            // expected-error {{incompatible type}}
                               // int = ptr<int>
    int t14 = h3();            // expected-error {{incompatible type}}
                               // int = array_ptr<int>
    int t14a = h3a();          // expected-error {{incompatible type}}
                               // int = array_ptr<int>
}

extern void check_call_void(void) {
    int val = 0;
    float fval = 0.0;
    int *p = 0;
    ptr<int> q = 0;
    array_ptr<int> r : count(1) = 0;

    // TODO: s will need bounds information
    void *s = 0;
    ptr<void> t = 0;
    array_ptr<void> u : byte_count(sizeof(int)) = 0;
    nt_array_ptr<int> v : count(1) = 0;

    // Test different kinds of pointers where the parameter type is a pointer to void and
    // the referent type is not a void pointer.

    // Type of first parameter is a pointer type.
    // Expected to typecheck
    f2_void(q, val);    // param ptr<void>, arg ptr<int> OK.
    f2_void(&val, val); // param ptr<void>, arg int * OK, provided that arg has known bounds.
    f3_void(r, val);    // param array_ptr<void>, arg array_ptr<int> OK.
    f3_void(p, val);    // param array_ptr<void>, arg int * OK, provided that param has unknown bounds.
    f3_void(&val, val); // param array_ptr<void>, arg int * OK, when param has unknown bounds and arg has known bounds

    // Expected to not typecheck
    f1_void(q, val);   // expected-error {{incompatible type}}
                        // param void *, arg ptr<int> not OK
    f1_void(r, val);    // expected-error {{incompatible type}}
                        // param void *, arg array_ptr<int> not OK
    f1_void(v, val);    // expected-error {{incompatible type}}
                        // param void *, arg nt_array_ptr<int> not OK
    f2_void(r, val);    // param ptr<void>, arg array_ptr<int> OK
    f3_void(q, val);    // param array_ptr<void>, arg ptr<int> OK
    f2_void(v, val);    // param ptr<void>, arg nt_array_ptr<int> OK
    f3_void(v, val);    // param array_ptr<void>, arg nt_array_ptr<int> OK
  

    // Test different kinds of pointers where the parameter type is a pointer to void and the
    // referent type is a pointer to void
    f1_void(s, val);  // param void *, arg void * OK
    f2_void((void *) &val, val);  // param ptr<void>, arg void * OK
    f2_void(t, val);  // param ptr<void>, arg ptr<void OK
    f3_void(s, val);  // param array_ptr<void>, arg void * OK.
    f3_void(u, val);  // param array_ptr<void> arg array_ptr<void> OK.

    f1_void(t, val);  // expected-error {{incompatible type}}
    f1_void(u, val);  // expected-error {{incompatible type}}
    f2_void(u, val);  // param ptr<void>, arg array_ptr<void>, OK
    f3_void(t, val);  // param array_ptr<void>, arg ptr<void>, OK

    // Test parameter types that are pointer to integers and argument types that are pointers to void
    f1(s, val);       // param int *, arg void * OK
    f1(t, val);       // expected-error {{incompatible type}}
                      // param int *, arg ptr<void>
    f1(u, val);       // expected-error {{incompatible type}}
                      // param int *, arg array_ptr<void>
    f2((void *) &val, val);       // param ptr<int>, arg void * OK
    f2(t, val);       // expected-error {{incompatible type}}
                      // param ptr<int>, arg ptr<void>
    f2(u, val);       // param ptr<int>, arg array_ptr<void> OK
    f3(s, val);       // param array_ptr<int>, arg void * OK
    f3(t, val);       // expected-error {{incompatible type}}
                      // param array_ptr<int>, arg ptr<void>
    f3(u, val);       // param array_ptr<int>, arg array_ptr<void> OK.
    f3a(s, val);      // expected-error {{incompatible type}}
                      // param nt_array_ptr<int>, arg void * not OK
    f3a(t, val);      // expected-error {{incompatible type}}
                      // param nt_array_ptr<int>, arg ptr<void>
    f3a(u, val);      // expected-error {{incompatible type}}
                      // param nt_array_ptr<int>, arg array_ptr<void> not OK.

   // Test parameters that are integers and argument types that are safe pointers to void
    f1(0, t);         // expected-error {{incompatible type}}
    f1(0, u);         // expected-error {{incompatible type}}
    f1(0, v);         // expected-error {{incompatible type}}

   // Test parameters that are safe pointers to void and argument types that are integers
    f2_void(5, val);  // expected-error {{incompatible type}}
    f3_void(5, val);  // expected-error {{incompatible type}}

    // Pass safe pointers to void to _Bool parameters
    f4(s, val);       // OK
    f4(t, val);       // OK
    f4(u, val);       // OK

    // Pass _Bool to safe pointers to void
    f2_void((_Bool) 1, val);   // expected-error {{incompatible type}}
    f3_void((_Bool) 1, val);   // expected-error {{incompatible type}}

    // Null pointers passed to safe pointers to void .
    f2_void(0, val);
    f3_void(0, val);
}

extern void check_call_void_checked(void) checked {
    int val = 0;
    float fval = 0.0;
    ptr<int> q = 0;
    array_ptr<int> r : count(1) = 0;
    ptr<struct CheckedData1> s = 0;
    ptr<void> t = 0;
    array_ptr<void> u : byte_count(sizeof(struct CheckedData1)) = 0;
    nt_array_ptr<int> v : count(1) = 0;

    // Test different kinds of pointers where the parameter type is a pointer to void and
    // the referent type is not a void pointer.

    // Type of first parameter is a pointer type.
    // Expected to typecheck

    f2_void(q, val);    // param ptr<void>, arg ptr<int> OK.
    f3_void(r, val);    // param array_ptr<void>, arg array_ptr<int> OK.
    f2_void(r, val);    // param ptr<void>, arg array_ptr<int> OK
    f3_void(q, val);    // param array_ptr<void>, arg ptr<int> OK
    f2_void(v, val);    // param ptr<void>, arg nt_array_ptr<int> OK
    f3_void(v, val);    // param array_ptr<void>, arg nt_array_ptr<int> OK

    f3_void(s, val);    // expected-error {{not allowed in a checked scope}}
                        // param array_ptr<void>, arg pointer to struct containing
                        // a checked pointer not OK.

    // Test different kinds of pointers where the parameter type is a pointer to void and the
    // referent type is a pointer to void
    f2_void(t, val);  // param ptr<void>, arg ptr<void OK
    f3_void(u, val);  // param array_ptr<void> arg array_ptr<void> OK.

    f2_void(u, val);  // param ptr<void>, arg array_ptr<void>, OK
    f3_void(t, val);  // param array_ptr<void>, arg ptr<void>, OK

    // Test parameter types that are pointer to integers and argument types that are pointers to void
    f2(t, val);       // expected-error {{incompatible type}}
                      // param ptr<int>, arg ptr<void>
    f2(u, val);       // param ptr<int>, arg array_ptr<void> OK
    f3(t, val);       // expected-error {{incompatible type}}
                      // param array_ptr<int>, arg ptr<void>
    f3(u, val);       // param array_ptr<int>, arg array_ptr<void> OK.
    f3a(t, val);      // expected-error {{incompatible type}}
                      // param nt_array_ptr<int>, arg ptr<void>
    f3a(u, val);      // expected-error {{incompatible type}}
                      // param nt_array_ptr<int>, arg array_ptr<void> not OK.
    f5(u, val);       // expected-error {{not allowed in a checked scope}}
                      // param ptr<T> where T contains a checked pointer, arg
                      // array_ptr<void> not OK.
    f6(u, val);       // expected-error {{not allowed in a checked scope}}
                      // param array_ptr<T> where T contains a checked pointer, arg
                      // array_ptr<void> not OK.

   // Test parameters that are integers and argument types that are safe pointers to void
    f1(0, t);         // expected-error {{incompatible type}}
    f1(0, u);         // expected-error {{incompatible type}}
    f1(0, v);         // expected-error {{incompatible type}}

   // Test parameters that are safe pointers to void and argument types that are integers
    f2_void(5, val);  // expected-error {{incompatible type}}
    f3_void(5, val);  // expected-error {{incompatible type}}

    // Pass safe pointers to void to _Bool parameters
    f4(t, val);       // OK
    f4(u, val);       // OK

    // Pass _Bool to safe pointers to void
    f2_void((_Bool) 1, val);   // expected-error {{incompatible type}}
    f3_void((_Bool) 1, val);   // expected-error {{incompatible type}}

    // Null pointers passed to safe pointers to void .
    f2_void(0, val);
    f3_void(0, val);
}


extern void check_call_void_checked_bounds_only(void) checked bounds_only {
    int val = 0;
    float fval = 0.0;
    int* _Single q = 0;
    int* _Array r : count(1) = 0;
    ptr<struct CheckedData1> s = 0;
    void* _Single t = 0;
    void* _Array u : byte_count(sizeof(struct CheckedData1)) = 0;
    int* _Nt_array v : count(1) = 0;

    // Test different kinds of pointers where the parameter type is a pointer to void and
    // the referent type is not a void pointer.

    // Type of first parameter is a pointer type.
    // Expected to typecheck

    f2_void(q, val);    // param ptr<void>, arg ptr<int> OK.
    f3_void(r, val);    // param array_ptr<void>, arg array_ptr<int> OK.
    f2_void(r, val);    // param ptr<void>, arg array_ptr<int> OK
    f3_void(q, val);    // param array_ptr<void>, arg ptr<int> OK
    f2_void(v, val);    // param ptr<void>, arg nt_array_ptr<int> OK
    f3_void(v, val);    // param array_ptr<void>, arg nt_array_ptr<int> OK
    f3_void(s, val);    // param array_ptr<void>, arg pointer to struct containing
                        // a checked pointer OK.

    // Test different kinds of pointers where the parameter type is a pointer to void and the
    // referent type is a pointer to void
    f2_void(t, val);  // param ptr<void>, arg ptr<void OK
    f3_void(u, val);  // param array_ptr<void> arg array_ptr<void> OK.

    f2_void(u, val);  // param ptr<void>, arg array_ptr<void>, OK
    f3_void(t, val);  // param array_ptr<void>, arg ptr<void>, OK

    // Test parameter types that are pointer to integers and argument types that are pointers to void
    f2(t, val);       // expected-error {{incompatible type}}
                      // param ptr<int>, arg ptr<void>
    f2(u, val);       // param ptr<int>, arg array_ptr<void> OK
    f3(t, val);       // expected-error {{incompatible type}}
                      // param array_ptr<int>, arg ptr<void>
    f3(u, val);       // param array_ptr<int>, arg array_ptr<void> OK.
    f3a(t, val);      // expected-error {{incompatible type}}
                      // param nt_array_ptr<int>, arg ptr<void>
    f3a(u, val);      // expected-error {{incompatible type}}
                      // param nt_array_ptr<int>, arg array_ptr<void> not OK.
    f5(u, val);       // param ptr<T> where T contains a checked pointer, arg
                      // array_ptr<void> OK.
    f6(u, val);       // param array_ptr<T> where T contains a checked pointer, arg
                      // array_ptr<void> OK.

   // Test parameters that are integers and argument types that are safe pointers to void
    f1(0, t);         // expected-error {{incompatible type}}
    f1(0, u);         // expected-error {{incompatible type}}
    f1(0, v);         // expected-error {{incompatible type}}

   // Test parameters that are safe pointers to void and argument types that are integers
    f2_void(5, val);  // expected-error {{incompatible type}}
    f3_void(5, val);  // expected-error {{incompatible type}}

    // Pass safe pointers to void to _Bool parameters
    f4(t, val);       // OK
    f4(u, val);       // OK

    // Pass _Bool to safe pointers to void
    f2_void((_Bool) 1, val);   // expected-error {{incompatible type}}
    f3_void((_Bool) 1, val);   // expected-error {{incompatible type}}

    // Null pointers passed to safe pointers to void .
    f2_void(0, val);
    f3_void(0, val);
}

void check_call_cv(void) {
    int val = 0;
    const int const_val = 0;
    int *p = 0;
    const int *p_const = 0;
    ptr<int> q = 0;
    ptr<const int> q_const = 0;
    array_ptr<int> r = 0;
    array_ptr<const int> r_const = 0;
    nt_array_ptr<int> s = 0;
    nt_array_ptr<const int> s_const = 0;

    // Parameters that are pointers to constants being passed pointers to non-const & const values.
    f1_const(p, val);           // param const int *, arg int * OK
    f1_const(p_const, val);     // param const int *, arg const int * OK
    f2_const(&val, val);        // param ptr<const int>, arg int * OK, provided int * has bounds.
    f2_const(&const_val, val);  // param ptr<const int>, arg const int * OK, provided int * has bounds
    f2_const(q, val);           // param ptr<const int>, arg ptr<int> OK
    f2_const(q_const, val);     // param ptr<const int> arg ptr<const int> OK
    f3_const(&val, val);        // param array_ptr<const int>, arg int * OK, provided int * has bounds.
    f3_const(&const_val, val);  // param array_ptr<const int>, arg const int * OK, provided int * has bounds
    f3_const(r, val);           // param array_ptr<const int>, arg array_ptr<int> OK
    f3_const(r_const, val);     // param array_ptr<const int> arg array_ptr<const int> OK
    f3a_const(s, val);           // param array_ptr<const int>, arg array_ptr<int> OK
    f3a_const(s_const, val);     // param array_ptr<const int> arg array_ptr<const int> OK

    // Parameters that are not pointers to constant, arguments that are pointers to constants.
    f1(p_const, val);     // expected-warning {{discards qualifiers}}
                          // param int *, arg const int * not OK
    f2(&const_val, val);  // expected-warning {{discards qualifiers}}
                          // param ptr<int>, arg const int * not OK.
    f2(q_const, val);     // expected-warning {{discards qualifiers}}
                          // param ptr<int> arg ptr<const int> not OK
    f3(&const_val, val);  // expected-warning {{discards qualifiers}}
                          // param array_ptr<int>, arg const int * not OK
    f3(r_const, val);     // expected-warning {{discards qualifiers}}
                          // param array_ptr<int> arg array_ptr<const int> not OK
    f3a(s_const, val);    // expected-warning {{discards qualifiers}}
                          // param nt_array_ptr<int> arg nt_array_ptr<const int> not OK
}

//
// Check pointer return types with different types of values.
//

// Check 'int *' return types.
int *check_return1(int *p) {
  return p;
}

int *check_return2(float *p) {
  return p;  // expected-warning {{incompatible pointer types}}
}

int *check_return3(void) {
  return 5; // expected-warning {{incompatible integer to pointer conversion}}
}

int *check_return4(ptr<int> p) {
  return p; // expected-error {{incompatible result type}}
}

int *check_return5(array_ptr<int> p : count(1)) {
  return p; // expected-error {{incompatible result type}}
}

int *check_return6(nt_array_ptr<int> p : count(1)) {
  return p; // expected-error {{incompatible result type}}
}

// Check ptr<int> return types.

ptr<int> check_return11(int *p) {
  return p; // expected-error {{expression has unknown bounds}}
}

ptr<int> check_return12(float *p) {
  return p;  // expected-error {{incompatible result type}}
}

ptr<int> check_return13(void) {
  return 5; // expected-error {{incompatible result type}}
}

ptr<int> check_return14(ptr<int> p) {
  return p;
}

ptr<int> checked_return15(ptr<float> p) {
  return p;  // expected-error {{incompatible result type}}
}

ptr<int> check_return16(array_ptr<int> p : count(1)) {
  return p;
}

ptr<int> check_return17(nt_array_ptr<int> p : count(1)) {
  return p;
}

// Check array_ptr<int> return types.

array_ptr<int> check_return21(int *p) {
  return p;
}

array_ptr<int> check_return21a(int *p) : count(1) {
  return p; // expected-error {{return value has unknown bounds, bounds expected because the function 'check_return21a' has bounds}}
}

array_ptr<int> check_return22(float *p) {
  return p;  // expected-error {{incompatible result type}}
}

array_ptr<int> check_return23(void) {
  return 5; // expected-error {{incompatible result type}}
}

array_ptr<int> check_return24(ptr<int> p) {
  return p;
}

array_ptr<int> checked_return25(ptr<float> p) {
  return p;  // expected-error {{incompatible result type}}
}

array_ptr<int> check_return26(array_ptr<int> p : count(1)) {
  return p;
}

array_ptr<int> check_return27(nt_array_ptr<int> p : count(1)) {
  return p;
}


//
// Check pointer return types with void pointer return values
//

// Unchecked scopes

int *check_voidptr_val_return1(void *p) {
  return p;
}

int *check_voidptr_val_return2(array_ptr<void> p : byte_count(sizeof(int))) {
  return p; // expected-error {{incompatible result type}}
}

ptr<int> check_voidptr_val_return11(void *p) {
  return p;  // expected-error {{expression has unknown bounds}}
}

ptr<int> check_voidptr_val_return12(array_ptr<void> p : byte_count(sizeof(int))) {
  return p;
}

array_ptr<int> check_voidptr_val_return21(void *p) {
  return p;
}

array_ptr<int> check_voidptr_val_return22(array_ptr<void> p : byte_count(sizeof(int)))   {
  return p;
}

array_ptr<struct CheckedData1> check_voidptr_val_return23(array_ptr<void> p : byte_count(sizeof(int)))   {
  return p;
}

// Checked scopes

checked ptr<int> check_voidptr_val_return31(array_ptr<void> p : byte_count(sizeof(int))) {
  return p;
}

checked array_ptr<int> check_voidptr_val_return32(array_ptr<void> p : byte_count(sizeof(int)))   {
  return p;
}

checked array_ptr<struct CheckedData1> check_voidptr_val_return23_checked(array_ptr<void> p : byte_count(sizeof(int)))   {
  return p; // expected-error {{not allowed in a checked scope}}
}

// Checked bounds_only scopes

checked bounds_only
ptr<int> check_voidptr_val_return41(array_ptr<void> p : byte_count(sizeof(int))) {
  return p;
}

checked bounds_only
array_ptr<int> check_voidptr_val_return42(array_ptr<void> p : byte_count(sizeof(int)))   {
  return p;
}

checked bounds_only
array_ptr<struct CheckedData1> check_voidptr_val_return43(array_ptr<void> p : byte_count(sizeof(int)))   {
  return p;
}

//
// Check void pointer return types with different types of values
// in unchecked scopes
//

// Check 'void *' return types.
void *check_voidptr_return1(int *p) {
  return p;
}

void *check_voidptr_return2(float *p) {
  return p;
}

void *check_voidptr_return3(void) {
  return 5; // expected-warning {{incompatible integer to pointer conversion}}
}

void *check_voidptr_return4(ptr<int> p) {
  return p; // expected-error {{incompatible result type}}
}

void *check_voidptr_return5(array_ptr<int> p : count(1)) {
  return p; // expected-error {{incompatible result type}}
}

void *check_voidptr_return6(nt_array_ptr<int> p : count(1)) {
  return p; // expected-error {{incompatible result type}}
}

void *check_voidptr_return7(ptr<struct CheckedData1> p) {
  return p; // expected-error {{incompatible result type}}
}


// Check array_ptr<void> return types.

array_ptr<void> check_voidptr_return21(int *p) {
  return p;
}

void* _Array check_voidptr_return21a(int *p) : byte_count(sizeof(int)) {
  return p; // expected-error {{return value has unknown bounds, bounds expected because the function 'check_voidptr_return21a' has bounds}}
}

array_ptr<void> check_voidptr_return21b(void *p) {
  return p;
}

array_ptr<void> check_voidptr_return21c(void *p) : byte_count(sizeof(int)) {
  return p; // expected-error {{return value has unknown bounds, bounds expected because the function 'check_voidptr_return21c' has bounds}}
}


array_ptr<void> check_voidptr_return22(float *p) {
  return p;
}

array_ptr<void> check_voidptr_return23(void) {
  return 5; // expected-error {{incompatible result type}}
}

array_ptr<void> check_voidptr_return24(ptr<int> p) {
  return p;
}

array_ptr<void> checked_voidptr_return25(ptr<float> p) {
  return p;
}

array_ptr<void> check_voidptr_return26(array_ptr<int> p : count(1)) {
  return p;
}

array_ptr<void> check_voidptr_return27(ptr<struct CheckedData1> p) {
  return p;
}


array_ptr<void> check_voidptr_return28(nt_array_ptr<int> p : count(1)) {
  return p;
}

void* _Array check_voidptr_return29(ptr<struct CheckedData1> p) {
  return p;
}

//
// Check void pointer return types with different types of values
// in checked scopes
//

checked void *check_voidptr_return_checked21(ptr<int> p) {  // expected-error {{return in a checked scope must have a checked type}}
  return p; // expected-error {{incompatible result type}}
}

checked array_ptr<void> check_voidptr_return31(ptr<int> p) {
  return p;
}

checked array_ptr<void> check_voidptr_return31a(array_ptr<int> p) : byte_count(sizeof(int)) {
  return p; // expected-error {{return value has unknown bounds, bounds expected because the function 'check_voidptr_return31a' has bounds}}
}

checked array_ptr<void> check_voidptr_return31b(array_ptr<void> p) {
  return p;
}

checked array_ptr<void> check_voidptr_return32(void) {
  return 5; // expected-error {{incompatible result type}}
}

checked array_ptr<void> check_voidptr_return33(ptr<int> p) {
  return p;
}

checked array_ptr<void> checked_voidptr_return34(ptr<float> p) {
  return p;
}

checked array_ptr<void> check_voidptr_return35(array_ptr<int> p : count(1)) {
  return p;
}

_Checked array_ptr<void> check_voidptr_return36(ptr<struct CheckedData1> p) {
  return p;  // expected-error {{not allowed in a checked scope}}
}

checked array_ptr<void> check_voidptr_return39(nt_array_ptr<int> p : count(1)) {
  return p;
}

checked bounds_only void *check_voidptr_return41(ptr<int> p) {  // expected-error {{return in a checked scope must have a checked type}}
  return p; // expected-error {{incompatible result type}}
}

checked bounds_only
array_ptr<void> check_voidptr_return42(ptr<int> p) {
  return p;
}

checked bounds_only
array_ptr<void> check_voidptr_return42a(array_ptr<int> p) : byte_count(sizeof(int)) {
  return p; // expected-error {{return value has unknown bounds, bounds expected because the function 'check_voidptr_return42a' has bounds}}
}

checked bounds_only
array_ptr<void> check_voidptr_return42b(array_ptr<void> p) {
  return p;
}

checked bounds_only
array_ptr<void> check_voidptr_return43(void) {
  return 5; // expected-error {{incompatible result type}}
}

checked bounds_only
array_ptr<void> check_voidptr_return44(ptr<int> p) {
  return p;
}

checked bounds_only
array_ptr<void> checked_voidptr_return45(ptr<float> p) {
  return p;
}

checked bounds_only
array_ptr<void> check_voidptr_return46(array_ptr<int> p : count(1)) {
  return p;
}

checked bounds_only
array_ptr<void> check_voidptr_return47(ptr<struct CheckedData1> p) {
  return p;
}

checked bounds_only
array_ptr<void> check_voidptr_return48(nt_array_ptr<int> p : count(1)) {
  return p;
}



void check_pointer_arithmetic(void)
{
   int val[5];
   int *p = val;
   ptr<int> q = &val[0];
   ptr<void> q_void = &val[0];
   array_ptr<int> r = val;
   array_ptr<void> r_void = val;
   nt_array_ptr<int> s = 0;
   // nt_array_ptr<void> is not allowed, so we don't have to test it.
   int *p_tmp;
   array_ptr<int> r_tmp;
   // By default, s_tmp has bounds of count(0).  We don't want to test checking
   // of bounds declarations at assignments to s_tmp below, so set the bounds to
   // unknown.
   nt_array_ptr<int> s_tmp : bounds(unknown) = 0;

   p_tmp = p + 5;
   p_tmp = 5 + p;
   p_tmp = p_tmp - 2;
   p_tmp = 2 - p_tmp;  // expected-error {{invalid operands to binary expression}}
   p_tmp = p++;
   p_tmp = p--;
   p_tmp = ++p;
   p_tmp = --p;
   p_tmp = (p += 1);
   p_tmp = (p -= 1);

   // 0 interpreted as an integer, not null
   p_tmp = p + 0;
   p_tmp = 0 + p;  
   p_tmp = p - 0;
   p_tmp = 0 - p;  // expected-error {{invalid operands to binary expression}}

   q + 5;  // expected-error {{arithmetic on _Ptr type}}
   5 + q;  // expected-error {{arithmetic on _Ptr type}}
   q++;    // expected-error {{arithmetic on _Ptr type}}
   q--;    // expected-error {{arithmetic on _Ptr type}}
   ++q;    // expected-error {{arithmetic on _Ptr type}}
   --q;    // expected-error {{arithmetic on _Ptr type}}
   q += 1; // expected-error {{arithmetic on _Ptr type}}
   q -= 1; // expected-error {{arithmetic on _Ptr type}}

   q_void + 5;  // expected-error {{arithmetic on _Ptr type}}
   5 + q_void;  // expected-error {{arithmetic on _Ptr type}}
   q_void++;    // expected-error {{arithmetic on _Ptr type}}
   q_void--;    // expected-error {{arithmetic on _Ptr type}}
   ++q_void;    // expected-error {{arithmetic on _Ptr type}}
   --q_void;    // expected-error {{arithmetic on _Ptr type}}
   q_void += 1; // expected-error {{arithmetic on _Ptr type}}
   q_void -= 1; // expected-error {{arithmetic on _Ptr type}}

   r_tmp = r + 5;
   r_tmp = 5 + r;
   r_tmp = r_tmp - 2; 
   r_tmp = 2 - r_tmp; // expected-error {{invalid operands to binary expression}}
   r_tmp = r++;
   r_tmp = r--;
   r_tmp = ++r;
   r_tmp = --r;
   r_tmp = (r += 1);
   r_tmp = (r -= 1);
   // 0 interpreted as an integer, not null
   r_tmp = r + 0;
   r_tmp = 0 + r;
   r_tmp = r - 0;
   r_tmp = 0 - r; // expected-error {{invalid operands to binary expression}}

   s_tmp = s + 5;
   s_tmp = 5 + s;
   s_tmp = s_tmp - 2;
   s_tmp = 2 - s_tmp; // expected-error {{invalid operands to binary expression}}
   s_tmp = s++; // expected-warning {{cannot prove declared bounds for 's' are valid after increment}}
   s_tmp = s--; // expected-warning {{cannot prove declared bounds for 's' are valid after decrement}}
   s_tmp = ++s; // expected-warning {{cannot prove declared bounds for 's' are valid after increment}}
   s_tmp = --s; // expected-warning {{cannot prove declared bounds for 's' are valid after decrement}}
   s_tmp = (s += 1); // expected-warning {{cannot prove declared bounds for 's' are valid after assignment}}
   s_tmp = (s -= 1); // expected-warning {{cannot prove declared bounds for 's' are valid after assignment}}
   // 0 interpreted as an integer, not null
   s_tmp = s + 0;
   s_tmp = 0 + s;
   s_tmp = s - 0;
   s_tmp = 0 - s; // expected-error {{invalid operands to binary expression}}

   // GCC allows arithmetic on void pointers, not allowed for safe pointer types

   r_void + 5;  // expected-error {{arithmetic on a pointer to void}}
   5 + r_void;  // expected-error {{arithmetic on a pointer to void}}
   r_void++;    // expected-error {{arithmetic on a pointer to void}}
   r_void--;    // expected-error {{arithmetic on a pointer to void}}
   ++r_void;    // expected-error {{arithmetic on a pointer to void}} 
   --r_void;    // expected-error {{arithmetic on a pointer to void}}
   r_void += 1; // expected-error {{arithmetic on a pointer to void}}
   r_void -= 1; // expected-error {{arithmetic on a pointer to void}}

   // adding two pointers is not allowed
   q + q; // expected-error {{invalid operands}}
   r + r; // expected-error {{invalid operands}}
   s + s; // expected-error {{invalid operands}}
}

void check_pointer_difference(void)
{
    int count;
    int val_int[5];
    float val_float[5];

    float *p_float = val_float;
    int *p_int = val_int;
    void *p_void = val_int;

    ptr<float> q_float = &val_float[0];
    ptr<int> q_int = &val_int[0];
    ptr<void> q_void = &val_int[0];

    array_ptr<float> r_float = val_float;
    array_ptr<int> r_int = val_int;
    array_ptr<void> r_void = val_int;

    // nt_array_ptr<float> and nt_array_ptr<void> are not allowed.
    nt_array_ptr<int> s_int = 0;
    nt_array_ptr<char> s_char = 0;

    // check pointer difference using different kinds of pointers to float
    count = p_float - p_float; // float * - float * OK
    count = p_float - q_float; // float * - ptr<float> OK
    count = p_float - r_float; // float * - array_ptr<float> OK

    count = q_float - p_float; // ptr<float> - float * OK
    count = q_float - q_float; // ptr<float> - ptr<float> OK
    count = q_float - r_float; // ptr<float> - array_ptr<float> OK.

    count = r_float - p_float; // array_ptr<float> - float * OK.
    count = r_float - q_float; // array_ptr<float> - ptr<float> OK.

   // check pointer difference using different kinds of pointers to int
    count = p_int - p_int; // int * - int * OK
    count = p_int - q_int; // int * - ptr<int> OK
    count = p_int - r_int; // int * - array_ptr<int> OK
    count = p_int - s_int; // int * - nt_array_ptr<int> OK 

    count = q_int - p_int; // ptr<int> - int * OK
    count = q_int - q_int; // ptr<int> - ptr<int> OK
    count = q_int - r_int; // ptr<int> - array_ptr<int> OK.
    count = q_int - s_int; // ptr<int> - nt_array_ptr<int> OK

    count = r_int - p_int; // array_ptr<int> - int * OK.
    count = r_int - q_int; // array_ptr<int> - ptr<int> OK.
    count = r_int - r_int; // array_ptr<int> - array_ptr<int> OK.
    count = r_int - s_int; // array_ptr<int> - nt_array_ptr<int> OK.

    count = s_int - p_int; // nt_array_ptr<int> - int * OK.
    count = s_int - q_int; // nt_array_ptr<int> - ptr<int> OK.
    count = s_int - r_int; // nt_array_ptr<int> - array_ptr<int> OK.
    count = s_int - s_int; // nt_array_ptr<int> - nt_array_ptr<int> OK.

    // invalid pointer differences
    // differences involving safe pointers to different referent types

    // pointers to int and pointers to float (using pointer to char for nt_array_char)
    count = p_int - p_float; // expected-error {{not pointers to compatible types}}
                             // int * - float * not OK
    count = p_int - q_float; // expected-error {{not pointers to compatible types}}
                             // int * - ptr<float> not OK
    count = p_int - r_float; // expected-error {{not pointers to compatible types}}
                             // int * - array_ptr<float> not OK
    count = p_int - s_char;  // expected-error {{not pointers to compatible types}}
                             // int * - nt_array_ptr<char> not OK

    count = q_int - p_float; // expected-error {{not pointers to compatible types}}
                             // ptr<int> - float * not OK
    count = q_int - q_float; // expected-error {{not pointers to compatible types}}
                             // ptr<int> - ptr<float> not OK
    count = q_int - r_float; // expected-error {{not pointers to compatible types}}
                             // ptr<int> - array_ptr<float> not OK.
    count = q_int - s_char;  // expected-error {{not pointers to compatible types}}
                             // ptr<int> - nt_array_ptr<char> not OK.

    count = r_int - p_float; // expected-error {{not pointers to compatible types}}
                             // array_ptr<int> - float * not OK.
    count = r_int - q_float; // expected-error {{not pointers to compatible types}}
                             // array_ptr<int> - ptr<float> not OK.
    count = r_int - r_float; // expected-error {{not pointers to compatible types}}
                             // array_ptr<int> - array_ptr<float> not OK.
    count = r_int - s_char; // expected-error {{not pointers to compatible types}}
                            // array_ptr<int> - nt_array_ptr<char> not OK.
   
    // pointers to int and nt_array_ptr<char>

    count = s_int - p_float; // expected-error {{not pointers to compatible types}}
                             // nt_array_ptr<int> - float * not OK.
    count = s_int - q_float; // expected-error {{not pointers to compatible types}}
                             // nt_array_ptr<int> - ptr<float> not OK.
    count = s_int - r_float; // expected-error {{not pointers to compatible types}}
                             // nt_array_ptr<int> - array_ptr<float> not OK.
    count = s_int - s_char; // expected-error {{not pointers to compatible types}}
                             // nt_array_ptr<int> - nt_array_ptr<char> not OK.

    // pointers to float and pointers to int
    count = p_float - p_int; // expected-error {{not pointers to compatible types}}
                             // float * - int * OK
    count = p_float - q_int; // expected-error {{not pointers to compatible types}}
                             // float * - ptr<int> OK
    count = p_float - r_int; // expected-error {{not pointers to compatible types}}
                             // float * - array_ptr<int> OK

    count = q_float - p_int; // expected-error {{not pointers to compatible types}}
                             // ptr<float> - int * OK
    count = q_float - q_int; // expected-error {{not pointers to compatible types}}
                             // ptr<float> - ptr<int> OK
    count = q_float - r_int; // expected-error {{not pointers to compatible types}}
                             // ptr<float> - array_ptr<int> OK.

    count = r_float - p_int; // expected-error {{not pointers to compatible types}}
                             // array_ptr<float> - int * OK.
    count = r_float - q_int; // expected-error {{not pointers to compatible types}}
                             // array_ptr<float> - ptr<int> OK.
    count = r_float - r_int; // expected-error {{not pointers to compatible types}}
                             // array_ptr<float> - array_ptr<int> OK.

    // differences involving safe pointers to void
    count = p_void - q_void; // expected-error {{arithmetic on pointers to void}}
                             // void * - ptr<void> not OK
    count = p_void - r_void; // expected-error {{arithmetic on pointers to void}}
                             // void * - array_ptr<void> not OK
    count = q_void - p_void; // expected-error {{arithmetic on pointers to void}}
                             // ptr<void> - void * not OK
    count = q_void - q_void; // expected-error {{arithmetic on pointers to void}}
                             // ptr<void> - ptr<void> not OK
    count = q_void - r_void; // expected-error {{arithmetic on pointers to void}}
                             // ptr<void> - array_ptr<void> not  OK.
    count = r_void - p_void; // expected-error {{arithmetic on pointers to void}}
                             // array_ptr<void> - void * not OK.
    count = r_void - q_void; // expected-error {{arithmetic on pointers to void}}
                             // array_ptr<void> - ptr<void> not OK.
    count = r_void - r_void; // expected-error {{arithmetic on pointers to void}}
                             // array_ptr<void> - array_ptr<void> not OK.

    // spot check differences involving pointers to void
	q_int - p_void;          // expected-error {{not pointers to compatible types}}
							 // ptr<int> - void * not OK
    q_int - q_void;          // expected-error {{not pointers to compatible types}}
	                         // ptr<int> - ptr<void> not OK
	q_int - r_void;          // expected-error {{not pointers to compatible types}}
							 // ptr<int> - array_ptr<void> not OK
	r_int - p_void;          // expected-error {{not pointers to compatible types}}
	                         // array_ptr<int> - void * not OK.
	r_int - q_void;          // expected-error {{not pointers to compatible types}}
							 // array_ptr<int> - ptr<void> not OK
	r_int - r_void;          // expected-error {{not pointers to compatible types}}
							 // array_ptr<int> - array_ptr<void> not OK.   
    s_int - p_void;          // expected-error {{not pointers to compatible types}}
                             // nt_array_ptr<int> - void * not OK.
    s_int - q_void;          // expected-error {{not pointers to compatible types}}
                             // nt_array_ptr<int> - ptr<void> not OK
    s_int - r_void;          // expected-error {{not pointers to compatible types}}
                             // nt_array_ptr<int> - array_ptr<void> not OK.

	p_void - q_int;          // expected-error {{not pointers to compatible types}}
							 // ptr<void> - int * not OK
	p_void - r_int;          // expected-error {{not pointers to compatible types}}
							 // ptr<void> - array_ptr<int> not OK
    p_void - s_int;          // expected-error {{not pointers to compatible types}}
                             // ptr<void> - nt_array_ptr<int> not OK
	q_void - p_int;          // expected-error {{not pointers to compatible types}}
							 // ptr<void> - int * not OK
	q_void - q_int;          // expected-error {{not pointers to compatible types}}
							 // ptr<void> - ptr<int> not OK
	q_void - r_int;          // expected-error {{not pointers to compatible types}}
							 // ptr<void> - array_ptr<int> not OK.
    q_void - s_int;          // expected-error {{not pointers to compatible types}}
                             // ptr<void> - nt_array_ptr<int> not OK.
	r_void - p_int;          // expected-error {{not pointers to compatible types}}
							 // array_ptr<void> - int * not OK
	r_void - q_int;          // expected-error {{not pointers to compatible types}}
							 // array_ptr<void> - ptr<int> not OK
	r_void - r_int;          // expected-error {{not pointers to compatible types}}
							 // array_ptr<void> - array_ptr<int> not OK.
    r_void - s_int;          // expected-error {{not pointers to compatible types}}
                             // array_ptr<void> - nt_array_ptr<int> not OK.
}

void check_pointer_relational_compare(void)
{
    int result;
    int val_int[5];
    float val_float[5];

    float *p_float = val_float;
    float *p2_float = val_float;
    int *p_int = val_int;
    int *p2_int = val_int;
    void *p_void = val_int;

    ptr<float> q_float = &val_float[0];
    ptr<float> q2_float = &val_float[0];
    ptr<int> q_int = &val_int[0];
    ptr<int> q2_int = &val_int[0];
    ptr<void> q_void = &val_int[0];
    ptr<void> q2_void = &val_int[0];

    array_ptr<float> r_float = val_float;
    array_ptr<float> r2_float = val_float;
    array_ptr<int> r_int = val_int;
    array_ptr<int> r2_int = val_int;
    array_ptr<void> r_void = val_int;
    array_ptr<void> r2_void = val_int;
    array_ptr<int> s_int = 0;
    array_ptr<int> s2_int = s_int;

    // relational comparisons using different kinds of pointers to float
    result = p_float < p2_float; // float * < float * OK
    result = p_float <= q_float; // float * <= ptr<float> OK
    result = p_float > r_float;  // float * > array_ptr<float> OK

    result = q_float >= p_float; // ptr<float> >= float * OK
    result = q_float < q2_float; // ptr<float> < ptr<float> OK
    result = q_float <= r_float; // ptr<float> <= array_ptr<float> OK.

    result = r_float < p_float;  // array_ptr<float> < float * OK.
    result = r_float <= p_float; // array_ptr<float> <= ptr<float> OK.
    result = r_float > r2_float; // array_ptr<float> > array_ptr<float> OK.

    // relational comparisons using different kinds of pointers to int
    result = p_int >= p2_int; // int * >= int * OK
    result = p_int < q_int;   // int * < ptr<int> OK
    result = p_int <= r_int;  // int * <= array_ptr<int> OK
    result = p_int <= s_int;  // int * <= nt_array_ptr<int> OK

    result = q_int > p_int;   // ptr<int> > int * OK
    result = q_int >= q2_int; // ptr<int> >= ptr<int> OK
    result = q_int < r_int;   // ptr<int> < array_ptr<int> OK.
    result = q_int < s_int;   // ptr<int> < nt_array_ptr<int> OK.

    result = r_int <= p_int;  // array_ptr<int> <= int * OK.
    result = r_int > q_int;   // array_ptr<int> > ptr<int> OK.
    result = r_int >= r2_int; // array_ptr<int> >= array_ptr<int> OK.
    result = r_int >= s_int;  // array_ptr<int> >= nt_array_ptr<int> OK.

    result = s_int <= p_int;  // nt_array_ptr<int> <= int * OK.
    result = s_int > q_int;   // nt_array_ptr<int> > ptr<int> OK.
    result = s_int >= r_int;  // nt_array_ptr<int> >= array_ptr<int> OK.
    result = s_int >= s2_int; // nt_array_ptr<int> >= nt_array_ptr<int> OK.

    // relational comparisons involving safe pointers to void
    result = p_void < q_void;  // void  * < ptr<void> OK
    result = p_void <= r_void; // void * <= array_ptr<void> OK
    result = q_void > p_void;  // ptr<void> > void * OK
    result = q_void >= q2_void;  // ptr<void> >= ptr<void> OK
    result = q_void < r_void; // ptr<void> < array_ptr<void>  OK.
    result = r_void <= p_void; // array_ptr<void> <= void * OK.
    result = r_void > q_void; // array_ptr<void> > ptr<void> OK.
    result = r_void >= r2_void;  // array_ptr<void> >= array_ptr<void> OK.

    // invalid relational comparisons

    // comparisons involving safe pointers to different referent types
    // pointers to int and pointers to float
    result = p_int < p_float;  // expected-warning {{comparison of distinct pointer types}}
                               // int * < float * not OK
    result = p_int <= q_float; // expected-warning {{comparison of distinct pointer types}}
                               // int * <= ptr<float> not OK
    result = p_int > r_float;  // expected-warning {{comparison of distinct pointer types}}
                               // int * > array_ptr<float> not OK
    result = q_int >= p_float; // expected-warning {{comparison of distinct pointer types}}
                               // ptr<int> >= float * not OK
    result = q_int < q_float; // expected-warning {{comparison of distinct pointer types}}
                               // ptr<int> < ptr<float> not OK
    result = q_int <= r_float; // expected-warning {{comparison of distinct pointer types}}
                               // ptr<int> <= array_ptr<float> not OK.

    result = r_int > p_float;  // expected-warning {{comparison of distinct pointer types}}
                               // array_ptr<int> > float * not OK.
    result = r_int >= q_float; // expected-warning {{comparison of distinct pointer types}}
                               // array_ptr<int> >= ptr<float> not OK.
    result = r_int < r_float;  // expected-warning {{comparison of distinct pointer types}}
                               // array_ptr<int> < array_ptr<float> not OK.

    result = s_int > p_float;  // expected-warning {{comparison of distinct pointer types}}
                               // nt_array_ptr<int> > float * not OK.
    result = s_int >= q_float; // expected-warning {{comparison of distinct pointer types}}
                               // nt_array_ptr<int> >= ptr<float> not OK.
    result = s_int < r_float;  // expected-warning {{comparison of distinct pointer types}}
                               // nt_array_ptr<int> < array_ptr<float> not OK.

    // pointers to float and pointers to int
    result = p_float <= p_int; // expected-warning {{comparison of distinct pointer types}}
                               // float * <= int * not OK
    result = p_float > q_int;  // expected-warning {{comparison of distinct pointer types}}
                               // float * > ptr<int> not OK
    result = p_float >= r_int; // expected-warning {{comparison of distinct pointer types}}
                               // float * >= array_ptr<int> not OK
    result = p_float >= s_int; // expected-warning {{comparison of distinct pointer types}}
                               // float * >= nt_array_ptr<int> not OK

    result = q_float < p_int;  // expected-warning {{comparison of distinct pointer types}}
                               // ptr<float> < int * not OK
    result = q_float <= q_int; // expected-warning {{comparison of distinct pointer types}}
                               // ptr<float> <= ptr<int> not OK
    result = q_float > r_int;  // expected-warning {{comparison of distinct pointer types}}
                               // ptr<float> > array_ptr<int> not OK.
    result = q_float > s_int;  // expected-warning {{comparison of distinct pointer types}}
                               // ptr<float> > nt_array_ptr<int> not OK.

    result = r_float >= p_int; // expected-warning {{comparison of distinct pointer types}}
                               // array_ptr<float> >= int * not OK.
    result = r_float < q_int;  // expected-warning {{comparison of distinct pointer types}}
                               // array_ptr<float> < ptr<int> not OK.
    result = r_float <= r_int; // expected-warning {{comparison of distinct pointer types}}
                               // array_ptr<float> <= array_ptr<int> not OK.
    result = r_float <= s_int; // expected-warning {{comparison of distinct pointer types}}
                               // array_ptr<float> <= nt_array_ptr<int> not OK.

    // pointers to void compared to pointers to int
    result = p_void < p_int;  // expected-warning {{comparison of distinct pointer types}}
                              // void  * < int * not OK
    result = p_void <= q_int; // expected-warning {{comparison of distinct pointer types}}
                              // void  * <= ptr<int> not OK
    result = p_void > r_int;  // expected-warning {{comparison of distinct pointer types}}
                              // void * > array_ptr<int> not OK
    result = p_void > s_int;  // expected-warning {{comparison of distinct pointer types}}
                              // void * > nt_array_ptr<int> not OK
    result = q_void >= p_int; // expected-warning {{comparison of distinct pointer types}}
                              // ptr<void> >= int * not OK
    result = q_void < q_int;  // expected-warning {{comparison of distinct pointer types}}
                              // ptr<void> < ptr<int> not OK
    result = q_void <= r_int; // expected-warning {{comparison of distinct pointer types}}
                              // ptr<void> <= array_ptr<int> not OK.
    result = q_void <= s_int; // expected-warning {{comparison of distinct pointer types}}
                              // ptr<void> <= nt_array_ptr<int> not OK.
    result = r_void > p_int;  // expected-warning {{comparison of distinct pointer types}}
                              // array_ptr<void> > int * not OK.
    result = r_void >= q_int; // expected-warning {{comparison of distinct pointer types}}
                              // array_ptr<void> >= ptr<int> not OK.
    result = r_void < r_int;  // expected-warning {{comparison of distinct pointer types}}
                              // array_ptr<void> < array_ptr<int> not OK.
    result = r_void < s_int;  // expected-warning {{comparison of distinct pointer types}}
                              // array_ptr<void> < array_ptr<int> not OK.

    // pointers to int compared to pointers to void
    result = p_int < p_void;  // expected-warning {{comparison of distinct pointer types}}
                              // int  * < void * not OK
    result = p_int <= q_void; // expected-warning {{comparison of distinct pointer types}}
                              // int  * <= ptr<void> not OK
    result = p_int > r_void;  // expected-warning {{comparison of distinct pointer types}}
                              // int * > array_ptr<void> not OK
    result = q_int >= p_void; // expected-warning {{comparison of distinct pointer types}}
                              // ptr<int> >= void * not OK
    result = q_int < q_void;  // expected-warning {{comparison of distinct pointer types}}
                              // ptr<int> < ptr<void> not OK
    result = q_int <= r_void; // expected-warning {{comparison of distinct pointer types}}
                              // ptr<int> <= array_ptr<void> not OK.
    result = r_int > p_void;  // expected-warning {{comparison of distinct pointer types}}
                              // array_ptr<int> > void * not OK.
    result = r_int >= q_void; // expected-warning {{comparison of distinct pointer types}}
                              // array_ptr<int> >= ptr<void> not OK.
    result = r_int < r_void;  // expected-warning {{comparison of distinct pointer types}}
                              // array_ptr<int> < array_ptr<void> not OK.
    result = s_int > p_void;  // expected-warning {{comparison of distinct pointer types}}
                              // nt_array_ptr<int> > void * not OK.
    result = s_int >= q_void; // expected-warning {{comparison of distinct pointer types}}
                              // nt_array_ptr<int> >= ptr<void> not OK.
    result = s_int < r_void;  // expected-warning {{comparison of distinct pointer types}}
                              // mt+array_ptr<int> < array_ptr<void> not OK.

    // Relational comparisons involving 0
    // The C11 specification technically doesn't allow this to typecheck for unsafe pointers.  
    // clang allows it anyway, so it is allowed for Checked C.  Note that according to the
    // Checked C spec, this has well-defined behavior.  According to the C11 spec, the behavior
    // is undefined because 0 doesn't point into the object array.
    result = 0 < p_void; // 0 < void * OK
    result = p_int <= 0; // int  * <= 0 OK
    result = p_int > 0;  // int * > 0  OK
    result = q_int >= 0; // ptr<int> >= 0 OK
    result = 0 < q_void; // 0 < ptr<void> OK
    result = q_int <= 0; // ptr<int> <= 0 OK.
    result = r_int > 0;  // array_ptr<int> > 0 OK.
    result = 0 > r_int;  // 0 < array_ptr<int> OK.
    result = 0 < r_void; // 0 < array_ptr<void> OK.
    result = s_int > 0;  // array_ptr<int> > 0 OK.
    result = 0 > s_int;  // 0 < array_ptr<int> OK.
}

void check_pointer_equality_compare(void)
{
    int result;
    int val_int[5];
    float val_float[5];

    float *p_float = val_float;
    float *p2_float = val_float;
    int *p_int = val_int;
    int *p2_int = val_int;
    void *p_void = val_int;

    ptr<float> q_float = &val_float[0];
    ptr<float> q2_float = &val_float[0];
    ptr<int> q_int = &val_int[0];
    ptr<int> q2_int = &val_int[0];
    ptr<void> q_void = &val_int[0];
    ptr<void> q2_void = &val_int[0];

    array_ptr<float> r_float = val_float;
    array_ptr<float> r2_float = val_float;
    array_ptr<int> r_int = val_int;
    array_ptr<int> r2_int = val_int;
    array_ptr<void> r_void = val_int;
    array_ptr<void> r2_void = val_int;

    int* _Nt_array s_int = 0;
    int* _Nt_array s2_int = 0;
    nt_array_ptr<char> s_char = 0;

    // equality/inequality comparisons using different kinds of pointers to float
    result = p_float == p2_float; // float * == float * OK
    result = p_float != q_float;  // float * != ptr<float> OK
    result = p_float == r_float;  // float * == array_ptr<float> OK

    result = q_float != p_float; // ptr<float> != float * OK
    result = q_float == q2_float; // ptr<float> == ptr<float> OK
    result = q_float != r_float; // ptr<float> != array_ptr<float> OK.

    result = r_float == p_float;  // array_ptr<float> == float * OK.
    result = r_float != p_float; // array_ptr<float> != ptr<float> OK.
    result = r_float == r2_float; // array_ptr<float> == array_ptr<float> OK.

    // equality/inquality comparisons using different kinds of pointers to int
    result = p_int != p2_int; // int * != int * OK
    result = p_int == q_int;  // int * == ptr<int> OK
    result = p_int != r_int;  // int * != array_ptr<int> OK
    result = p_int != s_int;  // int * != nt_array_ptr<int> OK

    result = q_int == p_int;   // ptr<int> == int * OK
    result = q_int != q2_int; // ptr<int> != ptr<int> OK
    result = q_int == r_int;   // ptr<int> == array_ptr<int> OK.
    result = q_int == s_int;   // ptr<int> == nt_array_ptr<int> OK.

    result = r_int != p_int;  // array_ptr<int> != int * OK.
    result = r_int == q_int;  // array_ptr<int> == ptr<int> OK.
    result = r_int == r2_int; // array_ptr<int> == array_ptr<int> OK.
    result = r_int == s_int;  // array_ptr<int> == nt_array_ptr<int> OK.

    result = s_int != p_int;  // nt_array_ptr<int> != int * OK.
    result = s_int == q_int;  // nt_array_ptr<int> == ptr<int> OK.
    result = s_int == r_int;  // nt_array_ptr<int> == array_ptr<int> OK.
    result = s_int == s2_int; // nt_array_ptr<int> == nt_array_ptr<int> OK.

    // equality/inequality comparisons involving safe pointers to void

    result = p_void != q_void;  // void  * != ptr<void> OK
    result = p_void == r_void; // void * == array_ptr<void> OK
    result = q_void != p_void;  // ptr<void> != void * OK
    result = q_void == q2_void;  // ptr<void> == ptr<void> OK
    result = q_void != r_void; // ptr<void> != array_ptr<void>  OK.
    result = r_void == p_void; // array_ptr<void> == void * OK.
    result = r_void != q_void; // array_ptr<void> != ptr<void> OK.
    result = r_void == r2_void;  // array_ptr<void> == array_ptr<void> OK.

    // pointers to void compared to pointers to int
    result = p_void != p_int; // void  * != int * OK
    result = p_void == q_int; // void  * == ptr<int> OK
    result = p_void == r_int; // void * == array_ptr<int> OK
    result = p_void == s_int; // void * == nt_array_ptr<int> OK
    result = q_void != p_int; // ptr<void> != int * OK
    result = q_void == q_int; // ptr<void> == ptr<int> OK
    result = q_void != r_int; // ptr<void> != array_ptr<int>  OK.
    result = q_void != s_int; // ptr<void> != nt_array_ptr<int>  OK.
    result = r_void == p_int; // array_ptr<void> == int * OK.
    result = r_void != q_int; // array_ptr<void> != ptr<int> OK.
    result = r_void == r_int; // array_ptr<void> == array_ptr<int> OK.
    result = r_void == s_int; // array_ptr<void> == nt_array_ptr<int> OK.

    // pointers to int compared to pointers to void
    result = p_int != p_void; // int  * != void * OK
    result = p_int == q_void; // int  * == ptr<void> OK
    result = p_int != r_void; // int * != array_ptr<void> OK
    result = q_int == p_void; // ptr<int> == void * OK
    result = q_int != q_void; // ptr<int> != ptr<void> OK
    result = q_int == r_void; // ptr<int> == array_ptr<void>  OK.
    result = r_int != p_void; // array_ptr<int> != void * OK.
    result = r_int == q_void; // array_ptr<int> == ptr<void> OK.
    result = r_int != r_void; // array_ptr<int> != array_ptr<void> OK.
    result = s_int != p_void; // array_ptr<int> != void * OK.
    result = s_int == q_void; // array_ptr<int> == ptr<void> OK.
    result = s_int != r_void; // array_ptr<int> != array_ptr<void> OK.

    // equality/inequality comparisons using different kinds of pointers to float and 0
    result = p_float == 0; // float * == 0 OK
    result = p_float != 0; // float * != ptr<float> OK

    result = q_float == 0; // ptr<float> == 0 OK
    result = q_float != 0; // ptr<float> != 0 OK

    result = r_float == 0; // array_ptr<float> == 0 OK.
    result = r_float != 0; // array_ptr<float> != 0 OK.

    result = 0 != p_float; // 0 != float * OK
    result = 0 == p_float; // 0 == ptr<float> OK

    result = 0 != q_float; // 0 != ptr<float> OK
    result = 0 == q_float; // 0  == ptr<float> OK

    result = 0 == r_float; // 0 == array_ptr<float> OK.
    result = 0 != r_float; // 0 != array_ptr<float> OK.

    // spot check pointers to int and pointers to void

    result = p_void == 0; // void * == 0 OK
    result = p_int != 0;  // int * != 0 OK

    result = q_int == 0;  // ptr<int> == 0 OK
    result = q_void != 0; // ptr<fvoid> == 0 OK

    result = r_void == 0; // array_ptr<void> == 0 OK.
    result = r_int != 0;  // array_ptr<int> != 0 OK.

    result = s_int != 0;  // nt_array_ptr<int> != 0 OK.

    result = 0 != p_int;  // 0 != int * OK
    result = 0 == p_void; // 0 == void * OK

    result = 0 != q_int;  // 0 == ptr<int> OK
    result = 0 == q_void; // 0 == ptr<void> OK

    result = 0 != r_void; // 0 != array_ptr<void>
    result = 0 == r_int;  // 0  == array_ptr<int>.

    result = 0 == s_int;  // 0  == array_ptr<int>.

    // invalid equality/inquality comparisons

    // comparisons involving safe pointers to different referent types

    // pointers to int and pointers to float (for nt_array_ptr, use char instead
    // of float).
    result = p_int != p_float; // expected-warning {{comparison of distinct pointer types}}
                               // int * != float * not OK
    result = p_int == q_float; // expected-warning {{comparison of distinct pointer types}}
                               // int * == ptr<float> not OK
    result = p_int != r_float; // expected-warning {{comparison of distinct pointer types}}
                               // int * != array_ptr<float> not OK
    result = p_int != s_char;  // expected-warning {{comparison of distinct pointer types}}
                               // int * != nt_array_ptr<char> not OK

    result = q_int == p_float; // expected-warning {{comparison of distinct pointer types}}
                               // ptr<int> == float * not OK
    result = q_int != q_float; // expected-warning {{comparison of distinct pointer types}}
                               // ptr<int> != ptr<float> not OK
    result = q_int == r_float; // expected-warning {{comparison of distinct pointer types}}
                               // ptr<int> <= array_ptr<float> not OK.
    result = q_int == s_char;  // expected-warning {{comparison of distinct pointer types}}
                               // ptr<int> <= nt_array_ptr<char> not OK.

    result = r_int != p_float; // expected-warning {{comparison of distinct pointer types}}
                               // array_ptr<int> != float * not OK.
    result = r_int == q_float; // expected-warning {{comparison of distinct pointer types}}
                               // array_ptr<int> == ptr<float> not OK.
    result = r_int != r_float; // expected-warning {{comparison of distinct pointer types}}
                               // array_ptr<int> != array_ptr<float> not OK.
    result = r_int != s_char;  // expected-warning {{comparison of distinct pointer types}}
                               // array_ptr<int> != nt_array_ptrchar> not OK.

    result = s_int != p_float; // expected-warning {{comparison of distinct pointer types}}
                               // nt_array_ptr<int> != float * not OK.
    result = s_int == q_float; // expected-warning {{comparison of distinct pointer types}}
                               // nt_array_ptr<int> == ptr<float> not OK.
    result = s_int != r_float; // expected-warning {{comparison of distinct pointer types}}
                               // nt_array_ptr<int> != array_ptr<float> not OK.
    result = s_int != s_char;  // expected-warning {{comparison of distinct pointer types}}
                               // nt_array_ptr<int> != nt_array_ptrchar> not OK.

   // pointers to float and pointers to int
    result = p_float == p_int; // expected-warning {{comparison of distinct pointer types}}
                               // float * == int * not OK
    result = p_float != q_int; // expected-warning {{comparison of distinct pointer types}}
                               // float * != ptr<int> not OK
    result = p_float == r_int; // expected-warning {{comparison of distinct pointer types}}
                               // float * == array_ptr<int> not OK
    result = p_float == s_int; // expected-warning {{comparison of distinct pointer types}}
                               // float * == nt_array_ptr<int> not OK

    result = q_float != p_int; // expected-warning {{comparison of distinct pointer types}}
                               // ptr<float> != int * not OK
    result = q_float == q_int; // expected-warning {{comparison of distinct pointer types}}
                               // ptr<float> == ptr<int> not OK
    result = q_float != r_int; // expected-warning {{comparison of distinct pointer types}}
                               // ptr<float> != array_ptr<int> not OK.
    result = q_float != s_int; // expected-warning {{comparison of distinct pointer types}}
                               // ptr<float> != array_ptr<int> not OK.

    result = r_float == p_int; // expected-warning {{comparison of distinct pointer types}}
                               // array_ptr<float> == int * not OK.
    result = r_float != q_int; // expected-warning {{comparison of distinct pointer types}}
                               // array_ptr<float> != ptr<int> not OK.
    result = r_float == r_int; // expected-warning {{comparison of distinct pointer types}}
                               // array_ptr<float> == array_ptr<int> not OK.
    result = r_float == s_int; // expected-warning {{comparison of distinct pointer types}}
                               // array_ptr<float> == mt+array_ptr<int> not OK.

    result = s_char == p_int; // expected-warning {{comparison of distinct pointer types}}
                               // array_ptr<float> == int * not OK.
    result = s_char != q_int; // expected-warning {{comparison of distinct pointer types}}
                               // array_ptr<float> != ptr<int> not OK.
    result = s_char == r_int; // expected-warning {{comparison of distinct pointer types}}
                               // array_ptr<float> == array_ptr<int> not OK.
    result = s_char == s_int; // expected-warning {{comparison of distinct pointer types}}
                               // array_ptr<float> == array_ptr<int> not OK.
}

void check_logical_operators(void)
{
    int val[5];
    int *p = val;
    ptr<int> q = &val[0];
    ptr<void> q_void = &val[0];
    array_ptr<int> r = 0;
    array_ptr<void> r_void = val;
    nt_array_ptr<int> s = 0;
    _Bool b;

    b = !p;
    b = !q;
    b = !q_void;
    b = !r;
    b = !r_void;
    b = !s;

    b = p || b;
    b = q || b;
    b = q_void || b;
    b = r || b;
    b = r_void || b;
    b = s || b;
    b = b || p;
    b = b || q;
    b = b || q_void;
    b = b || r;
    b = b || r_void;
    b = b || s;

    b = p || q;
    b = q || p;
    b = q_void || r_void;
    b = r || r_void;
    b = r_void || p;
    b = r || p;
    b = r || q;
    b = r_void || q_void;
    b = s || p;
    b = s || q;
    b = p || r;
    b = p || r_void;
    b = p || s;

    b = p && b;
    b = q && b;
    b = q_void && b;
    b = r && b;
    b = r_void && b;
    b = s && b;
    b = b && p;
    b = b && q;
    b = b && q_void;
    b = b && r;
    b = b && r_void;
    b = b && s;

    b = p && q;
    b = q && p;
    b = q_void && r_void;
    b = r && r_void;
    b = r_void && p;
    b = r && p;
    b = r && q;
    b = s && q;
    b = r_void && q_void;
    b = p && r;
    b = p && r_void;
    b = p && s;
}

// spot check operators that aren't supposed to be used with pointer types:
//   *, /, %, <<, >>, |, &, ^, ~, unary -, and unary +
void check_illegal_operators(void)
{
    int val[5];
    int *p = val;
    ptr<int> q = &val[0];
    int* _Array r = val;
    int* _Nt_array s = 0;

    p * 5;  // expected-error {{invalid operands to binary expression}}
    5 * p;  // expected-error {{invalid operands to binary expression}}
    p *= 5; // expected-error {{invalid operands to binary expression}}

    q * 5;  // expected-error {{invalid operands to binary expression}}
    5 * q;  // expected-error {{invalid operands to binary expression}}
    q *= 5; // expected-error {{invalid operands to binary expression}}

    r * 5;  // expected-error {{invalid operands to binary expression}}
    5 * r;  // expected-error {{invalid operands to binary expression}}
    r *= 5; // expected-error {{invalid operands to binary expression}}

    s * 5;  // expected-error {{invalid operands to binary expression}}
    5 * s;  // expected-error {{invalid operands to binary expression}}
    s *= 5; // expected-error {{invalid operands to binary expression}}

    p * p;  // expected-error {{invalid operands to binary expression}}
    p *= p; // expected-error {{invalid operands to binary expression}}

    q * q;  // expected-error {{invalid operands to binary expression}}
    q *= q; // expected-error {{invalid operands to binary expression}}

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

    q / 5;  // expected-error {{invalid operands to binary expression}}
    5 / q;  // expected-error {{invalid operands to binary expression}}
    q /= 5; // expected-error {{invalid operands to binary expression}}

    r / 5;  // expected-error {{invalid operands to binary expression}}
    5 / r;  // expected-error {{invalid operands to binary expression}}
    r /= 5; // expected-error {{invalid operands to binary expression}}

    s / 5;  // expected-error {{invalid operands to binary expression}}
    5 / s;  // expected-error {{invalid operands to binary expression}}
    s /= 5; // expected-error {{invalid operands to binary expression}}

    p / p;  // expected-error {{invalid operands to binary expression}}
    p /= p; // expected-error {{invalid operands to binary expression}}

    q / q;  // expected-error {{invalid operands to binary expression}}
    q /= q; // expected-error {{invalid operands to binary expression}}

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

    q % 5;  // expected-error {{invalid operands to binary expression}}
    5 % q;  // expected-error {{invalid operands to binary expression}}
    q %= 5; // expected-error {{invalid operands to binary expression}}

    r % 5;  // expected-error {{invalid operands to binary expression}}
    5 % r;  // expected-error {{invalid operands to binary expression}}
    r %= 5; // expected-error {{invalid operands to binary expression}}

    s % 5;  // expected-error {{invalid operands to binary expression}}
    5 % s;  // expected-error {{invalid operands to binary expression}}
    s %= 5; // expected-error {{invalid operands to binary expression}}

    p % p;  // expected-error {{invalid operands to binary expression}}
    p %= p; // expected-error {{invalid operands to binary expression}}

    q % q;  // expected-error {{invalid operands to binary expression}}
    q %= q; // expected-error {{invalid operands to binary expression}}

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

    q << 5;  // expected-error {{invalid operands to binary expression}}
    5 << q;  // expected-error {{invalid operands to binary expression}}
    q <<= 5; // expected-error {{invalid operands to binary expression}}

    r << 5;  // expected-error {{invalid operands to binary expression}}
    5 << r;  // expected-error {{invalid operands to binary expression}}
    r <<= 5; // expected-error {{invalid operands to binary expression}}

    s << 5;  // expected-error {{invalid operands to binary expression}}
    5 << s;  // expected-error {{invalid operands to binary expression}}
    s <<= 5; // expected-error {{invalid operands to binary expression}}

    p << p;  // expected-error {{invalid operands to binary expression}}
    p <<= p; // expected-error {{invalid operands to binary expression}}

    q << q;  // expected-error {{invalid operands to binary expression}}
    q <<= q; // expected-error {{invalid operands to binary expression}}

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

    q >> 5;  // expected-error {{invalid operands to binary expression}}
    5 >> q;  // expected-error {{invalid operands to binary expression}}
    q >>= 5; // expected-error {{invalid operands to binary expression}}

    r >> 5;  // expected-error {{invalid operands to binary expression}}
    5 >> r;  // expected-error {{invalid operands to binary expression}}
    r >>= 5; // expected-error {{invalid operands to binary expression}}

    s >> 5;  // expected-error {{invalid operands to binary expression}}
    5 >> s;  // expected-error {{invalid operands to binary expression}}
    s >>= 5; // expected-error {{invalid operands to binary expression}}

    p >> p;  // expected-error {{invalid operands to binary expression}}
    p >>= p; // expected-error {{invalid operands to binary expression}}

    q >> q;  // expected-error {{invalid operands to binary expression}}
    q >>= q; // expected-error {{invalid operands to binary expression}}

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

    q | 5;  // expected-error {{invalid operands to binary expression}}
    5 | q;  // expected-error {{invalid operands to binary expression}}
    q |= 5; // expected-error {{invalid operands to binary expression}}

    r | 5;  // expected-error {{invalid operands to binary expression}}
    5 | r;  // expected-error {{invalid operands to binary expression}}
    r |= 5; // expected-error {{invalid operands to binary expression}}

    s | 5;  // expected-error {{invalid operands to binary expression}}
    5 | s;  // expected-error {{invalid operands to binary expression}}
    s |= 5; // expected-error {{invalid operands to binary expression}}

    p | p;  // expected-error {{invalid operands to binary expression}}
    p |= p; // expected-error {{invalid operands to binary expression}}

    q | q;  // expected-error {{invalid operands to binary expression}}
    q |= q; // expected-error {{invalid operands to binary expression}}

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

    q & 5;  // expected-error {{invalid operands to binary expression}}
    5 & q;  // expected-error {{invalid operands to binary expression}}
    q &= 5; // expected-error {{invalid operands to binary expression}}

    r & 5;  // expected-error {{invalid operands to binary expression}}
    5 & r;  // expected-error {{invalid operands to binary expression}}
    r &= 5; // expected-error {{invalid operands to binary expression}}

    s & 5;  // expected-error {{invalid operands to binary expression}}
    5 & s;  // expected-error {{invalid operands to binary expression}}
    s &= 5; // expected-error {{invalid operands to binary expression}}

    p & p;  // expected-error {{invalid operands to binary expression}}
    p &= p; // expected-error {{invalid operands to binary expression}}

    q & q;  // expected-error {{invalid operands to binary expression}}
    q &= q; // expected-error {{invalid operands to binary expression}}

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

    q ^ 5;  // expected-error {{invalid operands to binary expression}}
    5 ^ q;  // expected-error {{invalid operands to binary expression}}
    q ^= 5; // expected-error {{invalid operands to binary expression}}

    r ^ 5;  // expected-error {{invalid operands to binary expression}}
    5 ^ r;  // expected-error {{invalid operands to binary expression}}
    r ^= 5; // expected-error {{invalid operands to binary expression}}

    s ^ 5;  // expected-error {{invalid operands to binary expression}}
    5 ^ s;  // expected-error {{invalid operands to binary expression}}
    s ^= 5; // expected-error {{invalid operands to binary expression}}

    p ^ p;  // expected-error {{invalid operands to binary expression}}
    p ^= p; // expected-error {{invalid operands to binary expression}}

    q ^ q;  // expected-error {{invalid operands to binary expression}}
    q ^= q; // expected-error {{invalid operands to binary expression}}

    r ^ r;  // expected-error {{invalid operands to binary expression}}
    r ^= r; // expected-error {{invalid operands to binary expression}}

    s ^ s;  // expected-error {{invalid operands to binary expression}}
    s ^= s; // expected-error {{invalid operands to binary expression}}

    //
    // Test ~
    //
    ~p;  // expected-error {{invalid argument type}}
    ~q;  // expected-error {{invalid argument type}}
    ~r;  // expected-error {{invalid argument type}}
    ~s;  // expected-error {{invalid argument type}}

    //
    // Test unary -
    //
    -p;  // expected-error {{invalid argument type}}
    -q;  // expected-error {{invalid argument type}}
    -r;  // expected-error {{invalid argument type}}
    -s;  // expected-error {{invalid argument type}}

    //
    // Test unary +
    //
    +p;  // expected-error {{invalid argument type}}
    +q;  // expected-error {{invalid argument type}}
    +r;  // expected-error {{invalid argument type}}
    +s;  // expected-error {{invalid argument type}}
}

//
// Test that address-of dereference and array subscript expressions
// have the correct types.
//

extern void test_sprintf(char *s);

void check_address_of_types(char s[10],
                            char *c : itype(char _Checked[10]),
                            char buf _Nt_checked[],
                            char* _Nt_array str,
                            char* _Array arr : count(10),
                            char* _Single p) {
  _Unchecked {
    test_sprintf(s);
    test_sprintf(&*s);
    test_sprintf(&*(s + 2));
    test_sprintf(&s[2]);
    test_sprintf(&2[s]);

    test_sprintf(c);
    test_sprintf(&*c);
    test_sprintf(&*(c + 3));
    test_sprintf(&c[3]);
    test_sprintf(&3[c]);

    test_sprintf(buf); // expected-error {{passing '_Nt_array_ptr<char>' to parameter of incompatible type 'char *'}}
    test_sprintf(&*buf); // expected-error {{passing '_Nt_array_ptr<char>' to parameter of incompatible type 'char *'}}
    test_sprintf(&*(buf + 0)); // expected-error {{passing '_Nt_array_ptr<char>' to parameter of incompatible type 'char *'}}
    test_sprintf(&buf[0]); // expected-error {{passing '_Nt_array_ptr<char>' to parameter of incompatible type 'char *'}}
    test_sprintf(&0[buf]); // expected-error {{passing '_Nt_array_ptr<char>' to parameter of incompatible type 'char *'}}

    test_sprintf(str); // expected-error {{passing '_Nt_array_ptr<char>' to parameter of incompatible type 'char *'}}
    test_sprintf(&*str); // expected-error {{passing '_Nt_array_ptr<char>' to parameter of incompatible type 'char *'}}
    test_sprintf(&*(str + 0)); // expected-error {{passing '_Nt_array_ptr<char>' to parameter of incompatible type 'char *'}}
    test_sprintf(&str[0]); // expected-error {{passing '_Nt_array_ptr<char>' to parameter of incompatible type 'char *'}}
    test_sprintf(&0[str]); // expected-error {{passing '_Nt_array_ptr<char>' to parameter of incompatible type 'char *'}}

    test_sprintf(arr); // expected-error {{passing '_Array_ptr<char>' to parameter of incompatible type 'char *'}}
    test_sprintf(&*arr); // expected-error {{passing '_Array_ptr<char>' to parameter of incompatible type 'char *'}}
    test_sprintf(&*(arr + 1)); // expected-error {{passing '_Array_ptr<char>' to parameter of incompatible type 'char *'}}
    test_sprintf(&arr[1]); // expected-error {{passing '_Array_ptr<char>' to parameter of incompatible type 'char *'}}
    test_sprintf(&1[arr]); // expected-error {{passing '_Array_ptr<char>' to parameter of incompatible type 'char *'}}

    test_sprintf(p); // expected-error {{passing '_Ptr<char>' to parameter of incompatible type 'char *'}}
    test_sprintf(&*p); // expected-error {{passing '_Ptr<char>' to parameter of incompatible type 'char *'}}
  }

  _Checked {
    test_sprintf(c); // expected-error {{passing '_Array_ptr<char>' to parameter of incompatible type 'char *'}}
    test_sprintf(&*c); // expected-error {{passing '_Array_ptr<char>' to parameter of incompatible type 'char *'}}
    test_sprintf(&*(c + 3)); // expected-error {{passing '_Array_ptr<char>' to parameter of incompatible type 'char *'}}
    test_sprintf(&c[3]); // expected-error {{passing '_Array_ptr<char>' to parameter of incompatible type 'char *'}}
    test_sprintf(&3[c]); // expected-error {{passing '_Array_ptr<char>' to parameter of incompatible type 'char *'}}

    test_sprintf(buf); // expected-error {{passing '_Nt_array_ptr<char>' to parameter of incompatible type 'char *'}}
    test_sprintf(&*buf); // expected-error {{passing '_Nt_array_ptr<char>' to parameter of incompatible type 'char *'}}
    test_sprintf(&*(buf + 0)); // expected-error {{passing '_Nt_array_ptr<char>' to parameter of incompatible type 'char *'}}
    test_sprintf(&buf[0]); // expected-error {{passing '_Nt_array_ptr<char>' to parameter of incompatible type 'char *'}}
    test_sprintf(&0[buf]); // expected-error {{passing '_Nt_array_ptr<char>' to parameter of incompatible type 'char *'}}

    test_sprintf(str); // expected-error {{passing '_Nt_array_ptr<char>' to parameter of incompatible type 'char *'}}
    test_sprintf(&*str); // expected-error {{passing '_Nt_array_ptr<char>' to parameter of incompatible type 'char *'}}
    test_sprintf(&*(str + 0)); // expected-error {{passing '_Nt_array_ptr<char>' to parameter of incompatible type 'char *'}}
    test_sprintf(&str[0]); // expected-error {{passing '_Nt_array_ptr<char>' to parameter of incompatible type 'char *'}}
    test_sprintf(&0[str]); // expected-error {{passing '_Nt_array_ptr<char>' to parameter of incompatible type 'char *'}}

    test_sprintf(arr); // expected-error {{passing '_Array_ptr<char>' to parameter of incompatible type 'char *'}}
    test_sprintf(&*arr); // expected-error {{passing '_Array_ptr<char>' to parameter of incompatible type 'char *'}}
    test_sprintf(&*(arr + 1)); // expected-error {{passing '_Array_ptr<char>' to parameter of incompatible type 'char *'}}
    test_sprintf(&arr[1]); // expected-error {{passing '_Array_ptr<char>' to parameter of incompatible type 'char *'}}
    test_sprintf(&1[arr]); // expected-error {{passing '_Array_ptr<char>' to parameter of incompatible type 'char *'}}

    test_sprintf(p); // expected-error {{passing '_Ptr<char>' to parameter of incompatible type 'char *'}}
    test_sprintf(&*p); // expected-error {{passing '_Ptr<char>' to parameter of incompatible type 'char *'}}
  }
}
