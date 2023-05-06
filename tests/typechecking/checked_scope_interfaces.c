// Feature tests of uses of bounds-safe interfaces with checked scopes.
//
// The following lines are for the LLVM test harness:
//
// RUN: %clang_cc1 -Wno-unused-value -Wno-pointer-bool-conversion -verify -verify-ignore-unexpected=note %s
//

// General outline:
// - Test the different kinds of declarations that can have
//   bounds-safe interfaces. For each kind of declaration, test types
//   that have different typechecking behavior.
//  * Parameter declarations (checked pointer types and array types)
//  * Global variable declarations (checked pointers and array types)
//  * Function declarations.
//  * Struct declarations
//
//   For each kind of declaration.
// - Test simple expressions that use these declarations.
// - Test more complex expressions that rely on the type having
//    been rewritten to be checked (such as pointers to pointers).

#include <stdchecked.h>

// Parameters with simple types.
checked int f1(int *s _Itype(int *_Single)) {
  int t1 = *s;        // Allowed in checked scope.
  int t2 = *(s + 4);  // expected-error {{arithmetic on _Ptr type}}
  int t3 = s[4];      // expected-error {{subscript of '_Ptr<int>'}}
  *(s+ 4) = 0;        // expected-error {{arithmetic on _Ptr type}}
  s[4] = 0;           // expected-error {{subscript of '_Ptr<int>'}}

  array_ptr<int> t4 : count(1) = s;
  s = t4;

  float* _Array t5  _Count(1) = s; // expected-error {{incompatible type}}
  float* _Array t6 = 0;
  s = t6;                              // expected-error {{incompatible type}}

  return 0;
}


checked int f2(int *s : count(len), int len) {
  // Allowed in checked scope.
  int t1 = *s;
  int t2 = *(s + 4);
  int t3 = s[4];
  *(s + 4) = 0;
  s[4] = 0;

  return 0;
}

checked int f3(int *s  _Itype(int *_Array), int len) {
  array_ptr<int> t1 = s + 5; // allowed
  int t2 = *s;                // expected-error {{expression has unknown bounds}}
  int t3 = s[4];              // expected-error {{expression has unknown bounds}}
  *(s + 4) = 0;               // expected-error {{expression has unknown bounds}}
  s[4] = 0;                   // expected-error {{expression has unknown bounds}}

  return 0;
}

checked int f4(int *s  _Itype(int checked[4])){
  int *_Array t1 = s + 4;
  int t2 = *s;
  int t3 = s[3];
  *(s + 3) = 0;
  s[3] = 0;

  return 0;
}

checked int f5(int *s : itype(int checked[])) {
  array_ptr<int> t1 = s + 4;
  int t2 = *s;                // expected-error {{expression has unknown bounds}}
  int t3 = s[4];              // expected-error {{expression has unknown bounds}}
  *(s + 4) = 0;               // expected-error {{expression has unknown bounds}}
  s[4] = 0;                   // expected-error {{expression has unknown bounds}}

  return 0;
}

// Test cases where a bounds-safe interface type is implied by a bounds
// expression on an unchecked type
checked int f6(int *s  _Count(4)) {
  int *_Array t1 = s + 3;
  int t2 = *s;
  int t3 = s[3];
  *(s + 3) = 0;
  s[3] = 0;

  return 0;
}

checked int f7(int *s : bounds(s, s + 4)) {
  array_ptr<int> t1 = s + 3;
  int t2 = *s;
  int t3 = s[3];
  *(s + 3) = 0;
  s[3] = 0;

  return 0;
}

checked int f8(int *s  _Byte_count(4 * sizeof(int))) {
  int *_Array t1 = s + 3;
  int t2 = *s;
  int t3 = s[3];
  *(s + 3) = 0;
  s[3] = 0;

  return 0;
}

checked int* func10(int *_Single p, int *q  _Itype(int *_Single))  _Itype(int *_Single) {
  int a = 5;
  int *_Single pa = &a;
  int *_Single pb = p;
  int *_Single pc = q;
  return 0;
}

//
// Globals with simple types
//
int *g1 : itype(ptr<int>);
int len;
int *g2 : count(len);
int *g3 : itype(array_ptr<int>);
int g4[5] : itype(int checked[5]);
extern int g5[] : itype(int checked[]);

// Globals used in bouunds-safe interfaces for other globals.
int *garr : count(10);
int *gptr : bounds(garr, garr + 5);
int gval : bounds(garr, garr + 10);

checked void test_globals(void) {
  // int *_Single
  int t1 = *g1;        // Allowed in checked scope.
  int t2 = *(g1 + 4);  // expected-error {{arithmetic on _Ptr type}}
  int t3 = g1[4];      // expected-error {{subscript of '_Ptr<int>'}}
  *(g1 + 4) = 0;       // expected-error {{arithmetic on _Ptr type}}
  g1[4] = 0;           // expected-error {{subscript of '_Ptr<int>'}}

  int *_Array t4  _Count(1) = g1;
  g1 = t4;

  float* _Array t5  _Count(1) = g1; // expected-error {{incompatible type}}
  float* _Array t6 = 0;
  g1 = t6;                              // expected-error {{incompatible type}}

  // int *_Array with bounds.
  int t11 = *g2;
  int t12 = *(g2 + 4);
  int t13 = g2[4];
  *(g2 + 4) = 0;
  g2[4] = 0;

  // int *_Array without bounds
  int *_Array t21 = g3 + 4; // allowed
  int t22 = *g3;                // expected-error {{expression has unknown bounds}}
  int t23 = g3[4];              // expected-error {{expression has unknown bounds}}
  *(g3 + 4) = 0;                // expected-error {{expression has unknown bounds}}
  g3[4] = 0;                    // expected-error {{expression has unknown bounds}}

  // int checked[5]
  int *_Array t31 = g4 + 4;
  int t32 = *g4;
  int t33 = g4[4];
  *(g4 + 4) = 0;
  g4[4] = 0;

  // int checked[]
  int *_Array t41 = g5+ 4;
  int t42 = *g5;                // expected-error {{expression has unknown bounds}}
  int t43 = g5[4];              // expected-error {{expression has unknown bounds}}
  *(g5 + 4) = 0;               // expected-error {{expression has unknown bounds}}
  g5[4] = 0;                   // expected-error {{expression has unknown bounds}}
}

//
// Function declarations with simple bounds-safe interfaces on parameters.
//
void f10(int *p : itype(ptr<int>));
void f11(int *p : count(4));
void f12(int *p : itype(array_ptr<int>));
void f13(int *p : itype(int checked[]));
void f14(int *p  _Itype(int checked[4]));
void f15(int *_Single p, int *q  _Itype(int *_Single));

extern int empty_global_arr[] : itype(int checked[]);

// Check calls to those functions.
checked int test_call_parameters(void) {
  ptr<int> param1 = 0;
  array_ptr<int> param2 : count(4) = 0;
  int *_Array param3;
  int arr1 checked[4];
  f10(param1);
  f10(param2);
  f10(param3);           // expected-error {{expression has unknown bounds}}
  f10(arr1);
  f10(empty_global_arr); // expected-error {{expression has unknown bounds}}

  f11(param1);           // expected-error {{argument does not meet declared bounds for 1st parameter}}
  f11(param2);
  f11(param3);           // expected-error {{argument has unknown bounds}}
  f11(arr1);
  f11(empty_global_arr); // expected-error {{argument has unknown bounds}}

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

  f14(param1);           // expected-error {{argument does not meet declared bounds for 1st parameter}}
  f14(param2);
  f14(param3);           // expected-error {{argument has unknown bounds}}
  f14(arr1);             
  f14(empty_global_arr); // expected-error {{argument has unknown bounds}}

  f15(param1, param2);
}

// Function declarations in checked scopes
checked void f10a(int *p : itype(ptr<int>));
checked void f11a(int *p : count(4));
checked void f11b(int *p : bounds(p, p + 4));
checked void f11c(int *p : byte_count(4 * sizeof(int)));
checked void f11d(int *lower : itype(array_ptr<int>),
                  int *upper : itype(array_ptr<int>),
                  int buf : bounds(lower, upper));
checked void f11e(char *buf : bounds(buf, end),
                  char *end : itype(_Array_ptr<char>));
checked void f12a(int *p : itype(array_ptr<int>));
checked void f13a(int *p  _Itype(int checked[]));
checked void f14a(int *p  _Itype(int checked[4]));
checked void f15a(ptr<int> p, int *q : itype(ptr<int>));


//
// Function declarations with bounds-safe interfaces on return values.
//
checked int* f20(int a checked[][5], int b checked[][5]) : itype(ptr<int>) {
  return 0;
}

checked int* f21(int *a  _Itype(int *_Single), int *b  _Itype(int *_Array))
    _Itype(int *_Array) checked {
  int e checked[5][5];
  _Unchecked {
    int *upa = f20(e, e);
  }
  int *upa = f20(e, e);     // expected-error {{local variable in a checked scope must have a checked type}}
  int *_Single pb = f20(e, e);
  return pb;
}

checked int *f21a(int *b : count(4)) : count(4) {
  return b;
}

checked int *f21b(int *b : count(4)) : byte_count(4 * sizeof(int)) {
  return b;
}

checked int *f21c(int *b : count(4)) _Bounds(return_value, return_value + 4) {
  return b;
}

checked int *f21d(int *b : count(4)) :
   bounds((array_ptr<char>)return_value,
          (array_ptr<char>)return_value + (4 * sizeof(int))) {
  return b;
}


// Test checked return types implied by a bounds-safe interface
checked void test_checked_returns(void) {
  int arr1 checked[5][5];
  int *_Single t1 = f20(arr1, arr1);

  int arr2 checked[4];
  array_ptr<int> t2 = f21(0, arr2);

  array_ptr<int> t3 = 0;
  t3 = f21a(arr2);
  t3 = f21b(arr2);
  t3 = f21c(arr2);
  t3 = f21d(arr2);
}

// No-prototype functions with a bounds-safe interface on the return type
// not allowed.

checked int* f22() : itype(array_ptr<int>);  // expected-error {{function without a prototype cannot be used or declared in a checked scope}}

//
// Illegal interface types for parameters in a checked scope.
//

checked int* f23(int a[]  _Itype(int *), char b[]  _Itype(char *))  _Itype(int *) checked {// expected-error 3 {{type must be a checked type}}
}

//-----------------------------------------------------------------------
// Test return statements in checked blocks where there is a bounds-safe
// interface on the return type.
//-----------------------------------------------------------------------

// Return a ptr from a checked scope for a function with a
// a bounds-safe interface return type of ptr.
int *f30(void)  _Itype(int *_Single)  {
  checked{
    ptr<int> p = 0;
  return p;
  }
  return 0;
}

// Return an array_ptr from a checked scope for a function with a
// a bounds-safe interface return type of array_ptr.
int *f31(int len) : count(len) {
  checked{
    array_ptr<int> p = 0;
    return p; // expected-error {{return value has unknown bounds, bounds expected because the function 'f31' has bounds}}
  }
  return 0;
}

// Return an ptr from a checked scope for a function with a
// a bounds-safe interface return type of ptr, where the
// returned value has a bounds-safe interface type.
int *f32(int * p  _Itype(int *_Single))  _Itype(ptr<int >)  {
  checked{
    return p;
  }
}

//
// Return a checked value from a checked scope, with the return
// type missing a bounds-safe interface.
//

int *f33(void) {
  checked{
    ptr<int> p = 0;
  return p;  // expected-error {{returning '_Ptr<int>' from a function with incompatible result type 'int *'}}
  }
  return 0;
}

int *f34(int len) {
  checked{
    array_ptr<int> p = 0;
  return p; // expected-error {{returning '_Array_ptr<int>' from a function with incompatible result type 'int *'}}
  }
  return 0;
}

int *f35(int *p : itype(ptr<int>)) {
  checked{
    return p; // expected-error {{returning '_Ptr<int>' from a function with incompatible result type 'int *'}}
  }
}

// Omit returning a value when one is expected.
int *f36(void) : itype(ptr<int>) {
  checked{
    return; // expected-error {{non-void function 'f36' must return a value when there are return bounds}}
  }
  return 0;
}

// Test returning unchecked values from an unchecked scope for a return with a 
// bounds-safe interface type declared in a checked scope.

int ga = 5;

checked int * func37(void)  _Itype(int *_Array) _Unchecked  {
   int *upa = &ga;
   return upa;
}

checked int * func38(void) : itype(ptr<int>) _Unchecked {
  int *upa = &ga;
  checked{
    _Unchecked {
      return upa;
    }
  }
}

#pragma CHECKED_SCOPE ON
struct S1 {
  int *f1 : itype(ptr<int>);
  int *f2  _Count(5);
  int *f3  _Count(len);
  int *f4 : bounds(f5, f5 + 5);  // use of f5 here is intentional.
  int *f5 : bounds(f5, f5 + len);
  int len;
  int *f6 : itype(array_ptr<int>);

  int arr[5] : itype(int checked[5]);
  void ((*fp1)(int *param : itype(ptr<int>))) :
    itype(ptr<void(int *param : itype(ptr<int>))>);
};
#pragma CHECKED_SCOPE OFF

// Use in checked scope
checked int test_struct1(struct S1 *p : itype(ptr<struct S1>)) {
  int t1 = *(p->f1 + 4);   // expected-error {{arithmetic on _Ptr type}}
  int t2 = *(p->f2 + 4);
  int t3 = *(p->f3 + 4);
  int t4 = *(p->f4 + 4);
  int t5 = *(p->f5 + 4);
  int t6 = *(p->f6 + 4);   // expected-error {{expression has unknown bounds}}
  int t7 = *(p->arr + 4);
  (*(p->fp1))(p->f1);
  (*(p->fp1))(0x5000);     // expected-error {{passing 'int' to parameter of incompatible type '_Ptr<int>'}}
  return 0;
}

// Use in unchecked scope
int test_struct2(struct S1 *p : itype(ptr<struct S1>)) {
  int t1 = *(p->f1 + 4);
  int t2 = *(p->f2 + 4);
  int t3 = *(p->f3 + 4);
  int t4 = *(p->f4 + 4);
  int t5 = *(p->f5 + 4);
  int t6 = *(p->f6 + 4);
  int t7 = *(p->arr + 4);
  (*(p->fp1))(p->f1);
  (*(p->fp1))(0x5000);    // expected-warning {{incompatible integer to pointer conversion passing 'int' to parameter of type 'int *'}}
  return 0;
}


//
// Test for complex constructed types (pointer to pointers,
// pointers to functions, and so on) that the entire type
// has been converted to be a checked type.
//

// Bounds-safe interfaces for checked pointers to checked pointers.

checked int f50(int **s : itype(ptr<ptr<int>>)) {
  ptr<int> t1 = *s;  // Allowed in checked scope.
  int t2 = **s;       // Allowed in checked scope
  int t3 = *(s + 5);  // expected-error {{arithmetic on _Ptr type}}
  int t4 = s[5];      // expected-error {{subscript of '_Ptr<_Ptr<int>>'}}
  *(s + 5) = 0;       // expected-error {{arithmetic on _Ptr type}}
  s[5] = 0;           // expected-error {{subscript of '_Ptr<_Ptr<int>>'}}
  *s += 5;            // expected-error {{arithmetic on _Ptr type}}
  s = s + 5;          // expected-error {{arithmetic on _Ptr type}}

  array_ptr<int> t5 : count(1) = *s;
  *s = t5;

  array_ptr<float> t6 : count(1) = *s; // expected-error {{incompatible type}}
  array_ptr<float> t7 = 0;
  *s = t7;                              // expected-error {{incompatible type}}
  return 0;
}

// Bounds-safe interfaces for checked pointers to function types,
// where the function types have bounds-safe interfaces on returns.
// that themselves have bounds-safe interfaces.

//
// Given a pointer to a function type with bounds-safe interfaces on its parameters,
// make sure that the bounds-safe interface is used at an indirect call in a checked
// scope.

typedef ptr<int(int *a : count(n), int n)> callback_fn1;

checked void test_function_pointer_parameter(callback_fn1 fn) {
  int arr checked[10];
  (*fn)(arr, 10);
  (*fn)(arr, 10);
}

// Make sure that bounds-safe interfaces on returns are used when
// checking function pointers in checked scopes

typedef ptr<int *(void) : itype(ptr<int>)> callback_fn2;

checked void test_function_return_of_ptr(callback_fn2 fn) {
  ptr<int> p = (*fn)();
}

// Test calls to returned function pointers

// Unchecked function pointer type.
typedef int (*callback_fn3)(int *a : count(n), int n);
// Corresponding version with bounds-safe interface.
typedef  ptr<int (int *a : count(n), int n)> bsi_callback_fn3;

checked callback_fn3 return_function_pointer(void) : itype(bsi_callback_fn3);

checked void test_function_pointer_return(void) {
  int arr checked[10];
  (*return_function_pointer())(arr, 10);
  ptr<int> p = 0;
  (*return_function_pointer())(p, 1);
}

// Test call through a function pointer with a bounds-safe interface,
// where the return type is a pointer to a pointer.
struct S2 {
  int *f1 : itype(ptr<int>);
  int *f2 : count(5);
  int *f3 : count(len);
  int len;
  int *f4 : itype(array_ptr<int>);
  int arr[5] : itype(int checked[5]);
  // pointer to function that returns a ptr<ptr<int>>
  int **((*fp1)(int *param  _Itype(int *_Single))) :
    itype(int *_Single *_Single (*_Single)(int *param  _Itype(int *_Single)));

  // pointer to function that returns an array_ptr<ptr<int>>
  int **((*fp2)(int *param : itype(ptr<int>),int len)) :
    itype(ptr<array_ptr<ptr<int>>(int *param : itype(ptr<int>), int len) : count(len)>);
};

checked int test_struct3(struct S2 *p  _Itype(ptr<struct S2>)) {
  (*(p->fp1))(p->f1) + 5;    // expected-error {{arithmetic on _Ptr type}}
  *((*(p->fp1))(p->f1)) + 5; // expected-error {{arithmetic on _Ptr type}}
  (*(p->fp2))(p->f1, 5) + 4;
  // Subscript the result returned by the indirect function call by 4.  This
  // should return a _Ptr<int>.  Then dereference the _Ptr<int>
  int i = *(*(p->fp2))(p->f1, 5)[4];
  // Subscript the result returned by the indirect function call by 4.  This
  // should return a _Ptr<int>.  Then try to do pointter arithmetic on the resulting
  // pointer.
  int j = *((*(p->fp2))(p->f1, 5)[4] + 5);   // expected-error {{arithmetic on _Ptr type}}
}

// Unchecked version - there should be no error messages.
unchecked int test_struct4(struct S2 *p  _Itype(ptr<struct S2>))  {
  (*(p->fp1))(p->f1) + 5;
  *((*(p->fp1))(p->f1)) + 5;
  (*(p->fp2))(p->f1, 5) + 4;
  // Subscript the result returned by the indirect function call by 4.  This
  // should return an int *.  Then dereference the resulting Ptr.
  int i = *(*(p->fp2))(p->f1, 5)[4];
  // Subscript the result returned by the indirect function call by 4.  This
  // should return a _Ptr<int>.  Then do pionter arithmetic on the int *
  int j = *((*(p->fp2))(p->f1, 5)[4] + 5);
}

// Test a single-dimensional array of function pointers with bounds-safe interfaces.
// A function pointer using just unchecked pointer types
typedef int *(*base_fnptr)(int *, int **j);
// An unchecked function pointer with bounds-safe interfaces on the parameters and
// and return type.
typedef int *(*base_with_interfaces)(int * : itype(ptr<int>),
                                     int **j : itype(ptr<ptr<int>>)) : itype(ptr<int>);
// A checked function pointer with bounds-safe interfaces on the parameters and
// return type.   This can be used in a checked scope because the function pointer 
// itself is checked.
typedef ptr<int *(int *i : itype(ptr<int>), int **j : itype(ptr<ptr<int>>)) : itype(ptr<int>)> fnptr_interface;

// Declare an unchecked array of function pointers and put a bounds-safe interface on it.
base_fnptr table1[10] : itype(fnptr_interface checked[10]);
base_with_interfaces table2[10] : itype(fnptr_interface checked[10]);

// In a checked scope, table1 and table2 are retyped as having checked types
// based on their bounds-safe interfaces. That means that the result of an
// indirect function call through table1 or table2 has typr Ptr.
checked void test1_array_of_function_pointers(int *_Single arg1, ptr<int *_Single> arg2, int num) {
  ptr<int> result1 = (*(table1[num]))(arg1, arg2);
  (*(table1[num]))(arg1, arg2) + 5;  // expected-error {{arithmetic on _Ptr type}}
  (*(table2[num]))(arg1, arg2);
  (*(table2[num]))(arg1, arg2) + 5;  // expected-error {{arithmetic on _Ptr type}}
  ptr<int> result2 = (*(table2[num]))(arg1, arg2);
}

// In an unchecked scope, table1 and table2 are not retyped based on their
// bounds-safe interfaces.  This means that:
// - the indirect function call through table1 can't be passed checked type.  There is
// no bounds-safe interface for the function type returned by table1[num].
// - the indirect function calls return unchecked pointers, so adding to the return
//   values is OK.
// - the unchecked pointer returned in the indirect cal through table2 does have
//   bounds, so it can be assigned a checked pointer type.
unchecked void test2_array_of_function_pointers(ptr<int> arg1, ptr<ptr<int>> arg2, int num) {
  (*(table1[num]))(arg1, arg2); // expected-error {{passing '_Ptr<int>' to parameter of incompatible type 'int *'}}
  ptr<int> result1 = (*(table1[num]))(0, 0) + 5; // expected-error {{expression has unknown bounds}}
  (*(table1[num]))(0, 0) + 5;
  (*(table2[num]))(arg1, arg2);
  (*(table2[num]))(arg1, arg2) + 5;
  ptr<int> result = (*(table2[num]))(arg1, arg2);
}

// Test some bounds-safe interfaces for real-world library functions.

#define size_t int
void *checked_aligned_alloc(size_t alignment, size_t size) : byte_count(size);
void *checked_calloc(size_t nmemb, size_t size) : byte_count(nmemb * size);
void checked_free(void *pointer : itype(ptr<void>));
void *checked_malloc(size_t size) : byte_count(size);
void *checked_realloc(void *pointer  : itype(ptr<void>), size_t size) : byte_count(size);

void *bsearch(const void *key : byte_count(size),
              const void *base : byte_count(nmemb * size),
              size_t nmemb, size_t size,
              int((*compar)(const void *, const void *)) :
              itype(ptr<int(ptr<const void>, ptr<const void>)>)) :
  byte_count(size);

void qsort(void *base : byte_count(nmemb * size),
           size_t nmemb, size_t size,
           int((*compar)(const void *, const void *)) :
           itype(ptr<int(ptr<const void>, ptr<const void>)>));

int mblen(const char *s : count(n), size_t n);

void (*signal(int sig,
              void ((*func)(int)) :
              itype(_Ptr<void (int)>) // bound-safe interface for func
              )  _Itype(_Ptr<void (int)>) // bounds-safe interface for signal return
      )(int);

void handler(int);  // for testing signal.

void test_bounds_safe_interface(void) {
  int *_Array arr0  _Count(4) = checked_calloc(4, sizeof(size_t));

  for (int i = 0; i < 4; i++)
    _Unchecked{
      checked_malloc(arr0[i]);
    }
  int *_Single arr1 = checked_malloc(10);
#pragma CHECKED_SCOPE OFF
  void *unchecked_ptr;
#pragma CHECKED_SCOPE ON
  int *_Array arr2 = checked_realloc(arr1, 20);
  int *_Array arr3 = checked_realloc(unchecked_ptr, 20);  // expected-error {{local variable used in a checked scope must have a checked type}}

  _Unchecked {
    typedef void (*return_fn)(int);
    return_fn rfn = signal(0, handler);

    typedef void (*wrong_fn)(int, int);
    wrong_fn wrong = signal(0, handler);   // expected-warning {{incompatible function pointer types}}
  }

  typedef ptr<void (int)> checked_return_fn;
  checked_return_fn rfn = signal(0, handler);

  typedef ptr<void (int, int)> wrong_fn;
  wrong_fn wrong = signal(0, handler);   // expected-error {{incompatible type}}

}
#pragma CHECKED_SCOPE OFF
