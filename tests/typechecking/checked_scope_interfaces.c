// Feature tests of uses of bounds-safe interfaces with checked scopes.
//
// The following lines are for the LLVM test harness:
//
// RUN: %clang_cc1 -fcheckedc-extension -Wno-unused-value -Wno-pointer-bool-conversion -verify -verify-ignore-unexpected=note %s
//

// General outline:
// - Test the different kinds of declarations that can have
//   bounds-safe interfaces.  For each declaration, test types
//   that have different typechecking behavior.
//  * Parameter declarations (checked pointer types and array types)
//  * Global variable declarations (checked pointers and array types)
//  * Function declarations.
// - Test simple expressions that use these declarations.
// - Test more complex expressions that rely on the type having
//    been rewritten to be checked (such as pointers to pointers).
//  * Struct declarations

// Parameters with simple types.
_Checked int f1(int *s : itype(_Ptr<int>)) {
  int t1 = *s;        // Allowed in checked scope.
  int t2 = *(s + 4);  // expected-error {{arithmetic on _Ptr type}}
  int t3 = s[4];      // expected-error {{subscript of '_Ptr<int>'}}
  *(s+ 4) = 0;        // expected-error {{arithmetic on _Ptr type}}
  s[4] = 0;           // expected-error {{subscript of '_Ptr<int>'}}

  _Array_ptr<int> t4 : count(1) = s;
  s = t4;

  _Array_ptr<float> t5 : count(1) = s; // expected-error {{incompatible type}}
  _Array_ptr<float> t6 = 0;
  s = t6;                              // expected-error {{incompatible type}}

  return 0;
}


_Checked int f2(int *s : count(len), int len) {
  // Allowed in checked scope.
  int t1 = *s;
  int t2 = *(s + 4);
  int t3 = s[4];
  *(s + 4) = 0;
  s[4] = 0;

  return 0;
}

_Checked int f3(int *s : itype(_Array_ptr<int>), int len) {
  _Array_ptr<int> t1 = s + 5; // allowed
  int t2 = *s;                // expected-error {{expression has no bounds}}
  int t3 = s[4];              // expected-error {{expression has no bounds}}
  *(s + 4) = 0;               // expected-error {{expression has no bounds}}
  s[4] = 0;                   // expected-error {{expression has no bounds}}

  return 0;
}

_Checked int f4(int *s : itype(int _Checked[4])) {
  _Array_ptr<int> t1 = s + 4;
  int t2 = *s;
  int t3 = s[4];
  *(s + 4) = 0;
  s[4] = 0;

  return 0;
}

_Checked int f5(int *s : itype(int _Checked[])) {
  _Array_ptr<int> t1 = s + 4;
  int t2 = *s;                // expected-error {{expression has no bounds}}
  int t3 = s[4];              // expected-error {{expression has no bounds}}
  *(s + 4) = 0;               // expected-error {{expression has no bounds}}
  s[4] = 0;                   // expected-error {{expression has no bounds}}

  return 0;
}

_Checked int* func10(_Ptr<int> p, int *q : itype(_Ptr<int>)) : itype(_Ptr<int>) {
  int a = 5;
  _Ptr<int> pa = &a;
  _Ptr<int> pb = p;
  _Ptr<int> pc = q;
  return 0;
}

//
// Globals with simple types
//
int *g1 : itype(_Ptr<int>);
int len;
int *g2 : count(len);
int *g3 : itype(_Array_ptr<int>);
int g4[5] : itype(int _Checked[5]);
extern int g5[] : itype(int _Checked[]);

// Globals used in bouunds-safe interfaces for other globals.
int *garr : count(10);
int *gptr : bounds(garr, garr + 5);
int gval : bounds(garr, garr + 10);

_Checked void test_globals(void) {
  // _Ptr<int>
  int t1 = *g1;        // Allowed in checked scope.
  int t2 = *(g1 + 4);  // expected-error {{arithmetic on _Ptr type}}
  int t3 = g1[4];      // expected-error {{subscript of '_Ptr<int>'}}
  *(g1 + 4) = 0;       // expected-error {{arithmetic on _Ptr type}}
  g1[4] = 0;           // expected-error {{subscript of '_Ptr<int>'}}

  _Array_ptr<int> t4 : count(1) = g1;
  g1 = t4;

  _Array_ptr<float> t5 : count(1) = g1; // expected-error {{incompatible type}}
  _Array_ptr<float> t6 = 0;
  g1 = t6;                              // expected-error {{incompatible type}}

  // _Array_ptr<int> with bounds.
  int t11 = *g2;
  int t12 = *(g2 + 4);  
  int t13 = g2[4];
  *(g2 + 4) = 0;
  g2[4] = 0;

  // _Array_ptr<int> without bounds
  _Array_ptr<int> t21 = g3 + 4; // allowed
  int t22 = *g3;                // expected-error {{expression has no bounds}}
  int t23 = g3[4];              // expected-error {{expression has no bounds}}
  *(g3 + 4) = 0;                // expected-error {{expression has no bounds}}
  g3[4] = 0;                    // expected-error {{expression has no bounds}}

  // int _Checked[5]
  _Array_ptr<int> t31 = g4 + 4;
  int t32 = *g4;
  int t33 = g4[4];
  *(g4 + 4) = 0;
  g4[4] = 0;

  // int _Checked[]
  _Array_ptr<int> t41 = g5+ 4;
  int t42 = *g5;                // expected-error {{expression has no bounds}}
  int t43 = g5[4];              // expected-error {{expression has no bounds}}
  *(g5 + 4) = 0;               // expected-error {{expression has no bounds}}
  g5[4] = 0;                   // expected-error {{expression has no bounds}}
}

//
// Function declarations with simple bounds-safe interfaces on parameters.
//
void f10(int *p : itype(_Ptr<int>));
void f11(int *p : count(4));
void f12(int *p : itype(_Array_ptr<int>));
void f13(int *p : itype(int _Checked[]));
void f14(int *p : itype(int _Checked[4]));
void f15(_Ptr<int> p, int *q : itype(_Ptr<int>));

extern int empty_global_arr[] : itype(int _Checked[]);

// Check calls to those functions.
_Checked int test_call_parameters(void) {
  _Ptr<int> param1 = 0;
  _Array_ptr<int> param2 : count(4) = 0;
  _Array_ptr<int> param3;
  int arr1 _Checked[4];
  f10(param1);
  f10(param2);
  f10(param3);           // expected-error {{expression has no bounds}}
  f10(arr1);
  f10(empty_global_arr); // expected-error {{expression has no bounds}}

  f11(param1);           // TODO: this should fail with checking of bounds declarations.
  f11(param2);
  f11(param3);           // expected-error {{argument has no bounds}}
  f11(arr1);
  f11(empty_global_arr); // expected-error {{argument has no bounds}}

  f12(param1);
  f12(param2);
  f12(param3);
  f12(arr1);
  f12(empty_global_arr);

  f13(param1);
  f13(param2);
  f13(param3);
  f13(arr1);
  f13(empty_global_arr);

  f14(param1);           // TODO: this should fail with checking of bounds declarations.
  f14(param2);
  f14(param3);           // expected-error {{argument has no bounds}}
  f14(arr1);             
  f14(empty_global_arr); // expected-error {{argument has no bounds}}

  f15(param1, param2);
}

//
// Function declarations with bounds-safe interfaces on return values.
//
_Checked int* f20(int a _Checked[][5], int b _Checked[][5]) : itype(_Ptr<int>) {
  return 0;
}

_Checked int* f21(int *a : itype(_Ptr<int>), int *b : itype(_Array_ptr<int>)) : itype(_Array_ptr<int>) _Checked {
  int e _Checked[5][5];
  _Unchecked {
    int *upa = f20(e, e);
  }
  int *upa = f20(e, e);     // expected-error {{local variable in a checked scope must have a checked type}}
  _Ptr<int> pb = f20(e, e);
  return pb;
}

// No-prototype functionswith a bounds-safe interface on the return type
// not allowed.

_Checked int* f22() : itype(_Array_ptr<int>);  // expected-error {{function without a prototype cannot be used or declared in a checked scope}}

//
// Illegal interface types for parameters in a checked scope.
//

_Checked int* f23(int a[] : itype(int *), char b[] : itype(char *)) : itype(int *) _Checked {// expected-error 3 {{type must be a checked type}}
}

//-----------------------------------------------------------------------
// Test return statements in checked blocks where there is a bounds-safe
// interface on the return type.
//-----------------------------------------------------------------------

// Return a _Ptr from a checked scope for a function with a
// a bounds-safe interface return type of _Ptr.
int *f30(void) : itype(_Ptr<int>) {
  _Checked{
    _Ptr<int> p = 0;
  return p;
  }
  return 0;
}

// Return an _Array_ptr from a checked scope for a function with a
// a bounds-safe interface return type of _Array_ptr.
int *f31(int len) : count(len) {
  _Checked{
    _Array_ptr<int> p = 0;
  return p;
  }
  return 0;
}

// Return an _Ptr from a checked scope for a function with a
// a bounds-safe interface return type of _Ptr, where the
// returned value has a bounds-safe interface type.
int *f32(int * p : itype(_Ptr<int>)) : itype(_Ptr<int >) {
  _Checked{
    return p;
  }
}

//
// Return a checked value from a checked scope, with the return
// type missing a bounds-safe interface.
//

int *f33(void) {
  _Checked{
    _Ptr<int> p = 0;
  return p;  // expected-error {{returning '_Ptr<int>' from a function with incompatible result type 'int *'}}
  }
  return 0;
}

int *f34(int len) {
  _Checked{
    _Array_ptr<int> p = 0;
  return p; // expected-error {{returning '_Array_ptr<int>' from a function with incompatible result type 'int *'}}
  }
  return 0;
}

int *f35(int *p : itype(_Ptr<int>)) {
  _Checked{
    return p; // expected-error {{returning '_Ptr<int>' from a function with incompatible result type 'int *'}}
  }
}

// Omit returning a value when one is expected.
int *f36(void) : itype(_Ptr<int>) {
  _Checked{
    return; // expected-error {{non-void function 'f36' should return a value}}
  }
  return 0;
}

// Test returning unchecked values from an unchecked scope for a return with a 
// bounds-safe interface type declared in a checked scope.

int ga = 5;

_Checked int * func37(void) : itype(_Array_ptr<int>) _Unchecked {
   int *upa = &ga;
   return upa;
}

_Checked int * func38(void) : itype(_Ptr<int>) _Unchecked {
  int *upa = &ga;
  _Checked{
    _Unchecked {
      return upa;
    }
  }
}

//
// Test constructed types and check that the entire
// type has been connverted to be a checked type.
//

// Bounds-safe interfaces that are checked pointers to checked pointers.

_Checked int f50(int **s : itype(_Ptr<_Ptr<int>>)) {
  _Ptr<int> t1 = *s;  // Allowed in checked scope.
  int t2 = **s;       // Allowed in checked scope
  int t3 = *(s + 5);  // expected-error {{arithmetic on _Ptr type}}
  int t4 = s[5];      // expected-error {{subscript of '_Ptr<_Ptr<int>>'}}
  *(s + 5) = 0;       // expected-error {{arithmetic on _Ptr type}}
  s[5] = 0;           // expected-error {{subscript of '_Ptr<_Ptr<int>>'}}
  *s += 5;            // expected-error {{arithmetic on _Ptr type}}
  s = s + 5;          // expected-error {{arithmetic on _Ptr type}}

  _Array_ptr<int> t5 : count(1) = *s;
  *s = t5;

  _Array_ptr<float> t6 : count(1) = *s; // expected-error {{incompatible type}}
  _Array_ptr<float> t7 = 0;
  *s = t7;                              // expected-error {{incompatible type}}
  return 0;
}

// Bounds-safe interfaces that are checked pointers to function types,
// where the function types have bounds-safe interfaces on returns.
// that themselves has bounds-safe interfaces.

//
// Given a pointer to a function type with bounds-safe interfaces on its parameters,
// make sure that the bounds-safe interface is used at an indirect call in a checked
// scope.

typedef _Ptr<int(int *a : count(n), int n)> callback_fn1;

_Checked void test_function_pointer_parameter(callback_fn1 fn) {
  int arr _Checked[10];
  (*fn)(arr, 10);
}

// Make sure that bounds-safe interfaces on returns are used when
// checking function pointers in checked scopes

typedef _Ptr<int *(void) : itype(_Ptr<int>)> callback_fn2;

_Checked void test_function_pointer_return(callback_fn2 fn) {
  _Ptr<int> p = (*fn)();
}


// Test some bounds-safe interfaces for real-world library functions.

#define size_t int
void *checked_aligned_alloc(size_t alignment, size_t size) : byte_count(size);
void *checked_calloc(size_t nmemb, size_t size) : byte_count(nmemb * size);
void checked_free(void *pointer : itype(_Ptr<void>));
void *checked_malloc(size_t size) : byte_count(size);
void *checked_realloc(void *pointer  : itype(_Ptr<void>), size_t size) : byte_count(size);

void *bsearch(const void *key : byte_count(size),
              const void *base : byte_count(nmemb * size),
              size_t nmemb, size_t size,
              int((*compar)(const void *, const void *)) :
              itype(_Ptr<int(_Ptr<const void>, _Ptr<const void>)>)) :
  byte_count(size);

void qsort(void *base : byte_count(nmemb * size),
           size_t nmemb, size_t size,
           int((*compar)(const void *, const void *)) :
           itype(_Ptr<int(_Ptr<const void>, _Ptr<const void>)>));

int mblen(const char *s : count(n), size_t n);

void test_bounds_safe_interface(void) {
  _Array_ptr<int> arr0 : count(4) = checked_calloc(4, sizeof(size_t));

  for (int i = 0; i < 4; i++)
    _Unchecked{
    checked_malloc(arr0[i]);
  }
  _Ptr<int> arr1 = checked_malloc(10);
#pragma BOUNDS_CHECKED OFF
  void *unchecked_ptr;
#pragma BOUNDS_CHECKED ON
  _Array_ptr<int> arr2 = checked_realloc(arr1, 20);
  _Array_ptr<int> arr3 = checked_realloc(unchecked_ptr, 20);  // expected-error {{local variable used in a checked scope must have a checked type}}
}
#pragma BOUNDS_CHECKED OFF
