// Feature tests of uses of bounds-safe interfaces with checked scopes.
//
// The following lines are for the LLVM test harness:
//
// RUN: %clang_cc1 -fcheckedc-extension -Wno-unused-value -Wno-pointer-bool-conversion -verify -verify-ignore-unexpected=note %s
//

_Checked int f1(int *s : itype(_Ptr<int>)) {
  int t1 = *s;        // Allowed in checked scope.
  int t2 = *(s + 5);  // expected-error {{arithmetic on _Ptr type}}
  int t3 = s[5];      // expected-error {{subscript of '_Ptr<int>'}}
  *(s+ 5) = 0;        // expected-error {{arithmetic on _Ptr type}}
  s[5] = 0;           // expected-error {{subscript of '_Ptr<int>'}}

  _Array_ptr<int> t4 : count(1) = s;
  s = t4;

  _Array_ptr<float> t5 : count(1) = s; // expected-error {{incompatible type}}
  _Array_ptr<float> t6 = 0;
  s = t6;                              // expected-error {{incompatible type}}

  return 0;
}

_Checked int f2(int **s : itype(_Ptr<_Ptr<int>>)) {
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

_Checked int f3(int *s : count(len), int len) {
  // Allowed in checked scope.
  int t1 = *s;
  int t2 = *(s + 5);  
  int t3 = s[5];
  *(s + 5) = 0;
  s[5] = 0;

  return 0;
}

_Checked int f4(int *s : itype(_Array_ptr<int>), int len) {
  _Array_ptr<int> t = s + 5;  // allowed
  int t1 = *s;                // expected-error {{expression has no bounds}}
  int t3 = s[5];              // expected-error {{expression has no bounds}}
  *(s + 5) = 0;               // expected-error {{expression has no bounds}}
  s[5] = 0;                   // expected-error {{expression has no bounds}}

  return 0;
}

_Checked int f5(int *s : itype(int _Checked[5])) {
  _Array_ptr<int> t = s + 5;  // allowed
  int t1 = *s;
  int t3 = s[5];
  *(s + 5) = 0;
  s[5] = 0;

  return 0;
}

_Checked int* func6(int a _Checked[][5], int b _Checked[][5]) : itype(_Ptr<int>) {
}

_Checked int func9(_Ptr<int> p, int *q : itype(_Ptr<int>)) {
  return 0;
}


_Checked int* func10(_Ptr<int> p, int *q : itype(_Ptr<int>)) : itype(_Ptr<int>) {
  int a = 5;
  _Ptr<int> pa = &a;
  _Ptr<int> pb = p;
  return 0;
}

_Checked int* func14(int *a : itype(_Ptr<int>), int *b : itype(_Array_ptr<int>)) : itype(_Array_ptr<int>) _Checked {
  int e _Checked[5][5];
  int f[5][5];                // expected-error {{local variable in a checked scope must have a checked type}}
  int *upa = func6(e, e);     // expected-error {{local variable in a checked scope must have a checked type}}
  _Ptr<int> pb = func6(e, e);
  return upa;
}

_Checked int* func15(int a[] : itype(int *), char b[] : itype(char *)) : itype(int *) _Checked {// expected-error 3 {{type must be a checked type}}
}

_Checked int* func17() : itype(_Array_ptr<int>);  // expected-error {{function without a prototype cannot be used or declared in a checked scope}}

// Test for bounds-safe interface
// - check variable declaration with bounds-safe interface
// - check function declaration(return & parameter) with bounds-safe interface
int *garr : count(10);
int *gptr : bounds(garr, garr + 5);
int gval : bounds(garr, garr + 10);

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

_Checked int * func43(void) : itype(_Array_ptr<int>) _Unchecked {
  int a = 5;
  {
    {
      int *upa = &a;
      _Unchecked{
        int b[5][5];
      _Checked{
        int c _Checked[5][5];
      for (int i = 0; i < 5; i++) {
        for (int j = 0; j < 5; j++) {
          b[i][j] += c[i][j];   // expected-error {{local variable used in a checked scope must have a checked type}}
        }
      }
      for (int i = 0; i < 5; i++) {
        for (int j = 0; j < 5; j++) _Unchecked{
          c[i][j] += b[i][j];
        }
      }
      }
      }
      return upa;
    }
  }
}

_Checked int * func48(void) : itype(_Ptr<int>) _Unchecked {
  int *upa;
  _Array_ptr<int> pb;
  _Ptr<int> pc = 0;
  _Checked{
    _Unchecked{
    return upa;
  }
  }
}

// Make sure that bounds-safe interfaces on parameters are used when
// checking function pointers in _Checked blocks.

typedef _Ptr<int(int *a : count(n), int n)> callback_fn1;

_Checked void test_function_pointer_parameter(callback_fn1 fn) {
  int arr _Checked[10];
  (*fn)(arr, 10);
}

// Make sure that bounds-safe interfaces on returns are used when
// checking function pointers in checked blocks.

typedef _Ptr<int *(void) : itype(_Ptr<int>)> callback_fn2;

_Checked void test_function_pointer_return(callback_fn2 fn) {
  _Ptr<int> p = (*fn)();
}

//-----------------------------------------------------------------------
// Test return statements in checked blocks where there is a bounds-safe
// interface on the return type.
//-----------------------------------------------------------------------

// Return a _Ptr from a checked scope for a function with a
// a bounds-safe interface return type of _Ptr.
int *f100(void) : itype(_Ptr<int>) {
  _Checked{
    _Ptr<int> p = 0;
  return p;
  }
  return 0;
}

// Return an _Array_ptr from a checked scope for a function with a
// a bounds-safe interface return type of _Array_ptr.
int *f101(int len) : count(len) {
  _Checked{
    _Array_ptr<int> p = 0;
  return p;
  }
  return 0;
}

// Return an _Ptr from a checked scope for a function with a
// a bounds-safe interface return type of _Ptr, where the
// returned value has a bounds-safe interface type.
int *f102(int * p : itype(_Ptr<int>)) : itype(_Ptr<int >) {
  _Checked{
    return p;
  }
}

//
// Return a checked value from a checked scope, with the return
// type missing a bounds-safe interface.
//

int *f103(void) {
  _Checked{
    _Ptr<int> p = 0;
  return p;  // expected-error {{returning '_Ptr<int>' from a function with incompatible result type 'int *'}}
  }
  return 0;
}

int *f104(int len) {
  _Checked{
    _Array_ptr<int> p = 0;
  return p; // expected-error {{returning '_Array_ptr<int>' from a function with incompatible result type 'int *'}}
  }
  return 0;
}

int *f105(int *p : itype(_Ptr<int>)) {
  _Checked {
    return p; // expected-error {{returning '_Ptr<int>' from a function with incompatible result type 'int *'}}
  }
}

// Omit returning a value when one is expected.
int *f106(void) : itype(_Ptr<int>) {
  _Checked{
    return; // expected-error {{non-void function 'f106' should return a value}}
  }
  return 0;
}
