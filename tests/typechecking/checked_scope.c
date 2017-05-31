// Feature tests of typechecking of uses of Checked C checked scope/function
//
// The following lines are for the LLVM test harness:
//
// RUN: %clang_cc1 -fcheckedc-extension -Wno-unused-value -Wno-pointer-bool-conversion -verify -verify-ignore-unexpected=note %s
//

#include "../../include/stdchecked.h"

// Test for checked function specifier.
// - check if function declaration (return/param) is checked.
// - check if function body (compound statement) is checked.
// - check if function with no prototype is checked.
// If declaration has an unchecked type error in a checked scope,
// it produces error message for declaration.
// Otherwise, it produces an error message for each use of declaration.

checked int func0(int len, int *p, ptr<int> q, array_ptr<int> r, int *s : itype(ptr<int>)) { // expected-error {{parameter cannot have an unchecked pointer type}}
  int result = *p + *q + *s;
  return result;
}

checked int* func1(int len, int *p, int *q, int *r : itype(array_ptr<int>)) { // expected-error 2 {{parameter cannot have an unchecked pointer type}} \
                                                                              // expected-error {{return cannot have an unchecked pointer type}}
  int result = *p + *q + *r;
  int *presult = &result;   // expected-error {{variable cannot have an unchecked pointer type}}
  return presult;
}

checked ptr<int> func2() {  // expected-error {{function with no prototype cannot have a return type that is a checked type}} \
                            // expected-error {{function without a prototype cannot be used or declared in a checked scope}}
}

checked int* func3() : itype(array_ptr<int>) {  //expected-error {{return cannot have an unchecked pointer type in a checked scope}} \
                                                //expected-error {{function without a prototype cannot be used or declared in a checked scope}}
}

checked int* func4(int len, int *p, int *q, array_ptr<int> r) : itype(ptr<int>) { // expected-error 2 {{parameter cannot have an unchecked pointer type}}
}

checked int func5(int p[]) {  // expected-error {{parameter cannot have an unchecked array type in a checked scope}}
  int a = 5;
  int *upa = &a;  // expected-error {{variable cannot have an unchecked pointer type in a checked scope}}
  ptr<int> pb = &a;
  array_ptr<int> pc = &a;
  int *upd = p;   // expected-error {{variable cannot have an unchecked pointer type in a checked scope}}
  return *p;
}

checked int func6(ptr<int> p, int *q) { // expected-error {{parameter cannot have an unchecked pointer type}}
  int a = 5;
  int *upa = &a;  // expected-error {{variable cannot have an unchecked pointer type in a checked scope}}
  ptr<int> pb = p;
  return *pb;
}

checked int func7(ptr<int> p, int *q : itype(ptr<int>)) {
  int a = 5;
  int *upa = &a;  // expected-error {{variable cannot have an unchecked pointer type in a checked scope}}
  int *upb = q;   // expected-error {{variable cannot have an unchecked pointer type in a checked scope}}
  ptr<int> pc = p;
  int e[5][5];    // expected-error {{variable cannot have an unchecked array type}}
  return *pc;
}

checked int* func8(ptr<int> p, int *q : itype(ptr<int>)) { // expected-error {{return cannot have an unchecked pointer type in a checked scope}}
  int a = 5;
  ptr<int> pa = &a;
  ptr<int> pb = p;
  int *upc = q; // expected-error {{variable cannot have an unchecked pointer type in a checked scope}}
  return upc;
}

checked int* func9(ptr<int> p, int *q : itype(ptr<int>)) : itype(ptr<int>) {
  int a = 5;
  ptr<int> pa = &a;
  ptr<int> pb = p;
  int *upc = q; // expected-error {{variable cannot have an unchecked pointer type in a checked scope}}
  return upc;
}

// Test for checked function specifier & checked block.
int* func10(array_ptr<int> x, int len) checked {
  int *upa;   // expected-error {{variable cannot have an unchecked pointer type in a checked scope}}
  return upa;
}

array_ptr<int> func11(array_ptr<int> x, int len) : bounds(x,x+len) checked {
  int *upa;     // expected-error {{variable cannot have an unchecked pointer type in a checked scope}}
  return upa;
}

array_ptr<int> func12(int *x, int *y) checked {
  int *upa = x; // expected-error {{variable cannot have an unchecked pointer type in a checked scope}} \
                // expected-error {{cannot use a parameter with an unchecked type}}
  int *upb = y; // expected-error {{variable cannot have an unchecked pointer type in a checked scope}} \
                // expected-error {{cannot use a parameter with an unchecked type}}
  return upb;
}

checked ptr<int> func13(void) checked {
  int *upa; // expected-error {{variable cannot have an unchecked pointer type in a checked scope}}
  ptr<int> pb = 0;
  array_ptr<int> pc;
  return pb;
}

checked array_ptr<char> func14(void) checked {
  int *upa; // expected-error {{variable cannot have an unchecked pointer type in a checked scope}}
  ptr<int> pb = 0 ;
  array_ptr<char> pc;
  return pc;
}

checked int* func15(void) checked { // expected-error {{return cannot have an unchecked pointer type in a checked scope}}
  int a[5]; // expected-error {{variable cannot have an unchecked array type in a checked scope}}
  int *upa; // expected-error {{variable cannot have an unchecked pointer type in a checked scope}}
  ptr<int> pb = 0;
  array_ptr<int> pc;
  return upa;
}

checked int* func16(int a checked[][5], int b checked[][5]) : itype(ptr<int>) checked {
}

checked int* func17(int a [][5], int b [][5]) : itype(ptr<int>) checked { // expected-error 2 {{parameter cannot have an unchecked array type in a checked scope}}
}

checked int* func18(int *a : itype(ptr<int>), int *b : itype(array_ptr<int>)) : itype(array_ptr<int>) checked {
  int e checked[5][5];
  int f[5][5];                // expected-error {{variable cannot have an unchecked array type in a checked scope}}
  int *upa = func16(e, e);    // expected-error {{variable cannot have an unchecked pointer type in a checked scope}}
  ptr<int> pb = func16(e, e);
  ptr<int> pc = 0;
  pc = func17(f, f);
  int *upd = func10(upa, 10); // expected-error {{variable cannot have an unchecked pointer type}} \
                              // expected-error {{cannot use a return with an unchecked type}}
  return upa;
}

checked int* func19(int a[] : itype(int *), char b[] : itype(char *)) : itype(int *) checked {// expected-error 3 {{type must be a checked type}} \
                                                                                              // expected-error 2 {{parameter cannot have an unchecked array type in a checked scope}} \
                                                                                              // expected-error {{return cannot have an unchecked pointer type}}
}

#pragma BOUNDS_CHECKED ON

// Test for bounds-safe interface
// - check variable declaration with bounds-safe interface
// - check function declaration(return & parameter) with bounds-safe interface
int *garr : count(10);
int *gptr : bounds(garr, garr + 5);
int gval : bounds(garr, garr+10);

#define size_t int
void *checked_aligned_alloc(size_t alignment, size_t size) : byte_count(size);
void *checked_calloc(size_t nmemb, size_t size) : byte_count(nmemb * size);
void checked_free(void *pointer : itype(ptr<void>));
void *checked_malloc(size_t size) : byte_count(size);
void *checked_realloc(void *pointer  : itype(ptr<void>), size_t size) : byte_count(size);

void *bsearch(const void *key : byte_count(size),
              const void *base : byte_count(nmemb * size),
              size_t nmemb, size_t size,
              int ((*compar)(const void *, const void *)) :
                itype(ptr<int(ptr<const void>, ptr<const void>)>)) :
                byte_count(size);

void qsort(void *base : byte_count(nmemb * size),
           size_t nmemb, size_t size,
           int ((*compar)(const void *, const void *)) :
             itype(ptr<int (ptr<const void>, ptr<const void>)>));

int mblen(const char *s : count(n), size_t n);

void test_bounds_safe_interface(void) {
  array_ptr<int> arr0 : count(4) = checked_calloc(4, sizeof(size_t));

  for (int i = 0; i < 4; i++)
    unchecked {
      checked_malloc(arr0[i]);
    }
  ptr<int> arr1 = checked_malloc(10);
#pragma BOUNDS_CHECKED OFF
  void *unchecked_ptr;
#pragma BOUNDS_CHECKED ON
  array_ptr<int> arr2 = checked_realloc(arr1, 20);
  array_ptr<int> arr3 = checked_realloc(unchecked_ptr, 20);  // expected-error {{cannot use a variable with an unchecked type in a checked scope or function}}
}

// Test for no-prototype function
// Especially, test KNR parameters function, that func(a,b,c) int a,b,c; {...}
// KNR parameter function has valid parameters but declared outside of function
// In function call, it is treated as no-prototype function call
// Therefore, this type of function call SHOULD be prevented in checked scope

int KNR_func1(a, b, c) int a,b,c; {
  return 1;
}

int KNR_func2(a, b) ptr<int> a; int b; {
  return 1;
}

int KNR_func3(a, b) ptr<char> a; ptr<int> b; {
  return 1;
}

void KNR_test(void) {
  ptr<int> px = 0;
  ptr<char> py = 0;
  int a,b,c;
  KNR_func1(a,b,c); // expected-error {{function without a prototype cannot be used or declared in a checked scope}}
  KNR_func2(px,a);  // expected-error {{function without a prototype cannot be used or declared in a checked scope}}
  KNR_func3(py,px); // expected-error {{function without a prototype cannot be used or declared in a checked scope}}
}

#pragma BOUNDS_CHECKED OFF

// Test for checked block.
// - check if compound statments are checked.
// - check if checked property is inherited from parent scope.
int func20(void) checked {
  int a = 5;
  ptr<int> pa = &a;
  int b[5][5]; // expected-error {{variable cannot have an unchecked array type in a checked scope}}
  for (int i = 0; i < 5; i++) {
    for (int j = 0; j < 5; j++) {
      b[i][j] = -1;
    }
  }
}

int func21(void) {
  checked {
    int a = 5;
    ptr<int> pa = &a;
    int b checked[5][5];
    for (int i = 0; i < 5; i++) {
      for (int j = 0; j < 5; j++) {
        b[i][j] = -1;
      }
    }
  }
}

int func22(void) {
  int a = 5;
  int *upa = &a;
  checked {
    int b[5][5];    // expected-error {{variable cannot have an unchecked array type in a checked scope}}
  checked {
    for (int i = 0; i < 5; i++) checked {
      for (int j = 0; j < 5; j++) checked {
        b[i][j] = *upa; // expected-error {{cannot use a variable with an unchecked type}}
      }
    }
  }
  }
}

int func23(void) checked {
  int a = 5;
  {
    int *upa = &a;  // expected-error {{variable cannot have an unchecked pointer type in a checked scope}}
  {
    int b[5][5];    // expected-error {{variable cannot have an unchecked array type in a checked scope}}
  {
    int c checked[5][5];
    for (int i = 0; i < 5; i++) {
      for (int j = 0; j < 5; j++) {
        b[i][j] += c[i][j];
      }
    }
  }
  }
  }
}

int func24(void) {
  int a = 5;
  checked {
    int *upa = &a;  // expected-error {{variable cannot have an unchecked pointer type in a checked scope}}
    int b[5][5];    // expected-error {{variable cannot have an unchecked array type in a checked scope}}
    for (int i = 0; i < 5; i++) {
      for (int j = 0; j < 5; j++) {
        b[i][j] = -1;
      }
    }
  }
}

int func25(void) {
  int a = 5;
  int *upa = &a;
  int b[5][5];
  for (int i = 0; i < 5; i++) checked {
    for (int j = 0; j < 5; j++) {
      b[i][j] = -1; // expected-error {{cannot use a variable with an unchecked type}}
    }
  }
}

int func26(ptr<int> p, int *q) {
  int a = 5;
  int *upa;
  array_ptr<int> pc;
  upa = pc;           // expected-error {{assigning to 'int *' from incompatible type '_Array_ptr<int>'}}
  checked {
    upa = &a;         // expected-error {{cannot use a variable with an unchecked type}}
  }
  ptr<int> pb = p;
  return *pb;
}

checked void func27(void) {
  int *upa;   // expected-error {{variable cannot have an unchecked pointer type}}
  array_ptr<int> pb;
  ptr<int> pc = 0;
  upa = pb;
  upa = pc;
}

void func28(void) {
  int *upa;
  array_ptr<int> pb;
  ptr<int> pc = 0;
  upa = pb; // expected-error {{assigning to 'int *' from incompatible type '_Array_ptr<int>'}}
  upa = pc; // expected-error {{assigning to 'int *' from incompatible type '_Ptr<int>'}}
}

int func29(void) {
  int a = 5;
  checked {
    {
      struct s0 {
        int *a;       // expected-error {{member cannot have an unchecked pointer type}}
        char *b;      // expected-error {{member cannot have an unchecked pointer type}}
        ptr<int> pc;
        array_ptr<int> pd : count(len);
        int len;
        short e[10];  // expected-error {{member cannot have an unchecked array type}}
        int *f : itype(ptr<int>);
        char *g : itype(array_ptr<char>);
      } a;
    }
  }
}

void func30(void) {
  int a = 5;
  int len = 10;
  array_ptr<int> pa : count(len) = 0;
  checked(len);     // expected-error {{expected compound statement after checked scope keyword}}
  checked [5][5];   // expected-error {{expected compound statement after checked scope keyword}}
}

// Test for unchecked scope.
// - unchecked scope clears checked property & prevents inheritance of checked property.
// - unchecked scope keyword clears checked function specifier.
checked int * func40(int *p, int *q) unchecked {// expected-error {{return cannot have an unchecked pointer type}} \
                                                // expected-error 2 {{parameter cannot have an unchecked pointer type}}
  int a = 5;
  *p = *q = 0;
  ptr<int> pa = &a;
  int b[5][5];
  for (int i = 0; i < 5; i++) {
    for (int j = 0; j < 5; j++) {
      b[i][j] = -1;
    }
  }
  return 0;
}

checked int * func41(int *p, ptr<int> q, array_ptr<int> r, array_ptr<int> s : count(2)) unchecked { // expected-error {{return cannot have an unchecked pointer type}} \
                                                                                                    // expected-error {{parameter cannot have an unchecked pointer type}}
  int a = 5;
  checked {
    *p = 1;
    *q = 2;
    *r = 3; // expected-error {{expression has no bounds}}
  *s = 4;
  unchecked {
    ptr<int> pa = &a;
    int b checked[5][5];
    for (int i = 0; i < 5; i++) {
      for (int j = 0; j < 5; j++) {
        b[i][j] += *q + *r; // expected-error {{expression has no bounds}}
        b[i][j] += *p + *q + *r + *s;
      }
    }
  }
  }
  return 0;
}

checked int func42(ptr<int> p, int q[], int r[5]) unchecked {  // expected-error 2 {{parameter cannot have an unchecked array type}}
  int a = 5;
  checked {
    int *upa = &a;  // expected-error {{variable cannot have an unchecked pointer type in a checked scope}}
  unchecked {
    int b[5][5];
    checked {
      for (int i = 0; i < 5; i++) {
        for (int j = 0; j < 5; j++) {
          b[i][j] = -1;   // expected-error {{cannot use a variable with an unchecked type}}
        }
      }
    }
  }
  }
  return 0;
}

checked int * func43(void) : itype(array_ptr<int>) unchecked {
  int a = 5;
  {
    {
      int *upa = &a;
      unchecked {
        int b[5][5];
        checked {
          int c checked[5][5];
          for (int i = 0; i < 5; i++) {
            for (int j = 0; j < 5; j++) {
              b[i][j] += c[i][j];   // expected-error {{cannot use a variable with an unchecked type}}
            }
          }
          for (int i = 0; i < 5; i++) {
            for (int j = 0; j < 5; j++) unchecked {
              c[i][j] += b[i][j];
            }
          }
        }
      }
      return upa;
    }
  }
}

checked int * func44(void) unchecked {  // expected-error {{return cannot have an unchecked pointer type}}
  int a = 5;
  checked {
    int *upa = &a;  // expected-error {{variable cannot have an unchecked pointer type in a checked scope}}
  unchecked {
    int b[5][5];
    checked {
      int c checked[5][5];
      int d[5][5];  // expected-error {{variable cannot have an unchecked array type}}
    for (int i = 0; i < 5; i++) {
      for (int j = 0; j < 5; j++) {
        b[i][j] += c[i][j] - d[i][j];   // expected-error {{cannot use a variable with an unchecked type}}
      }
    }
    }
    return 0;
  }
  }
}

checked int func45(int *p, int *q, int *r : itype(ptr<int>), int *s : itype(array_ptr<int>)) unchecked { // expected-error 2 {{parameter cannot have an unchecked pointer type}}
  int sum = 0;
  int a = 5;
  int *upa = &a;
  int b[5][5];
  for (int i = 0; i < 5; i++) checked {
    sum += b[i][0] + *upa; // expected-error 2 {{cannot use a variable with an unchecked type}}
    for (int j = 1; j < 5; j++) unchecked {
      sum += b[i][j];
    }
  }
  return sum;
}

int func46(ptr<int> p, int *q) unchecked {
  int a = 5;
  int *upa;
  array_ptr<int> pc;
  upa = pc;   // expected-error {{assigning to 'int *' from incompatible type '_Array_ptr<int>'}}
  checked {
    upa = &a; // expected-error {{cannot use a variable with an unchecked type}}
    pc = &a;
  unchecked {
    upa = &a;
    pc = &a;
  }
    upa = &a; // expected-error {{cannot use a variable with an unchecked type}}
    pc = &a;
  }
  ptr<int> pb = p;
  return *pb;
}

checked array_ptr<int> func47(void) {
  int *upa;   // expected-error {{variable cannot have an unchecked pointer type}}
  unchecked {
  int *upb;
  array_ptr<int> pb;
  ptr<int> pc = 0;
  upa = pb;
  upa = pc;
  upb = pb;   // expected-error {{assigning to 'int *' from incompatible type '_Array_ptr<int>'}}
  upb = pc;   // expected-error {{assigning to 'int *' from incompatible type '_Ptr<int>'}}
  return upb; // array_ptr<int> = int *, OK
  }
}

checked int * func48(void) : itype(ptr<int>) unchecked {
  int *upa;
  array_ptr<int> pb;
  ptr<int> pc = 0;
  checked {
    unchecked {
  upa = pb; // expected-error {{assigning to 'int *' from incompatible type '_Array_ptr<int>'}}
  upa = pc; // expected-error {{assigning to 'int *' from incompatible type '_Ptr<int>'}}
    return upa;
    }
  }
}

int func49(void) {
  int a = 5;
  checked {
    {
      struct s0 {
        int *a;       // expected-error {{member cannot have an unchecked pointer type}}
        char *b;      // expected-error {{member cannot have an unchecked pointer type}}
        ptr<int> pc;
        array_ptr<int> pd : count(len);
        int len;
        short e[10];  // expected-error {{member cannot have an unchecked array type}}
        char f[10];   // expected-error {{member cannot have an unchecked array type}}
        int *g : itype(ptr<int>);
        char *h : itype(array_ptr<char>);
      } a;
      struct s1 unchecked {
        int *a;
        char *b;
        ptr<int> pc;
        array_ptr<int> pd : count(len);
        int len;
        short e[10];
        char f[10];
        int *g : itype(ptr<int>);
        char *h : itype(array_ptr<char>);
      } b;

    }
  }
  return 0;
}

// Test for unchecked function specifier
unchecked int * func50(int *p, int *q) {
  int a = 5;
  *p = *q = 0;
  ptr<int> pa = &a;
  int b[5][5];
  for (int i = 0; i < 5; i++) {
    for (int j = 0; j < 5; j++) {
      b[i][j] = -1;
    }
  }
  return 0;
}

unchecked int * func51(int *p, ptr<int> q, array_ptr<int> r, array_ptr<int> s : count(2)) {
  int a = 5;
  checked {
    *p = 1; // expected-error {{cannot use a parameter with an unchecked type}}
    *q = 2;
    *r = 3; // expected-error {{expression has no bounds}}
  *s = 4;
  unchecked {
    ptr<int> pa = &a;
    int b checked[5][5];
    for (int i = 0; i < 5; i++) {
      for (int j = 0; j < 5; j++) {
        b[i][j] = *p + *q + *r + *s;  // expected-error {{expression has no bounds}}
      }
    }
  }
  }
  return 0;
}

unchecked int func52(ptr<int> p, int q[], int r[5]) unchecked {
  int a = 5;
  checked {
    int *upa = &a;  // expected-error {{variable cannot have an unchecked pointer type in a checked scope}}
  unchecked {
    int b[5][5];
    checked {
      for (int i = 0; i < 5; i++) {
        for (int j = 0; j < 5; j++) {
          b[i][j] = -1;   // expected-error {{cannot use a variable with an unchecked type}}
        }
      }
    }
  }
  }
  return 0;
}

unchecked int * func53(void) : itype(array_ptr<int>) unchecked {
  int a = 5;
  {
    {
      int *upa = &a;
      unchecked {
        int b[5][5];
        checked {
          int c checked[5][5];
          for (int i = 0; i < 5; i++) {
            for (int j = 0; j < 5; j++) {
              b[i][j] += c[i][j];   // expected-error {{cannot use a variable with an unchecked type}}
            }
          }
          for (int i = 0; i < 5; i++) {
            for (int j = 0; j < 5; j++) unchecked {
              c[i][j] += b[i][j];
            }
          }
        }
      }
      return upa;
    }
  }
}

unchecked int * func54(void) unchecked {
  int a = 5;
  checked {
    int *upa = &a;  // expected-error {{variable cannot have an unchecked pointer type in a checked scope}}
  unchecked {
    int b[5][5];
    checked {
      int c checked[5][5];
      int d[5][5];  // expected-error {{variable cannot have an unchecked array type}}
    for (int i = 0; i < 5; i++) {
      for (int j = 0; j < 5; j++) {
        b[i][j] += c[i][j] - d[i][j];   // expected-error {{cannot use a variable with an unchecked type}}
      }
    }
    }
    return 0;
  }
  }
}

unchecked int func55(int *p, int *q, int *r : itype(ptr<int>), int *s : itype(array_ptr<int>)) unchecked {
  int sum = 0;
  int a = 5;
  int *upa = &a;
  int b[5][5];
  for (int i = 0; i < 5; i++) checked {
    sum += b[i][0] + *upa; // expected-error 2 {{cannot use a variable with an unchecked type}}
    for (int j = 1; j < 5; j++) unchecked {
      sum += b[i][j];
    }
  }
  return sum;
}

unchecked int func56(ptr<int> p, int *q) unchecked {
  int a = 5;
  int *upa;
  array_ptr<int> pc;
  upa = pc;   // expected-error {{assigning to 'int *' from incompatible type '_Array_ptr<int>'}}
  checked {
    upa = &a; // expected-error {{cannot use a variable with an unchecked type}}
    pc = &a;
  unchecked {
    upa = &a;
    pc = &a;
  }
    upa = &a; // expected-error {{cannot use a variable with an unchecked type}}
    pc = &a;
  }
  ptr<int> pb = p;
  return *pb;
}

unchecked array_ptr<int> func57(void) {
  int *upa;
  unchecked {
  int *upb;
  array_ptr<int> pb;
  ptr<int> pc = 0;
  upa = pb;   // expected-error {{assigning to 'int *' from incompatible type '_Array_ptr<int>'}}
  upa = pc;   // expected-error {{assigning to 'int *' from incompatible type '_Ptr<int>'}}
  upb = pb;   // expected-error {{assigning to 'int *' from incompatible type '_Array_ptr<int>'}}
  upb = pc;   // expected-error {{assigning to 'int *' from incompatible type '_Ptr<int>'}}
  return upb; // array_ptr<int> = int *, OK
  }
}

unchecked int * func58(void) : itype(ptr<int>) unchecked {
  int *upa;
  array_ptr<int> pb;
  ptr<int> pc = 0;
  checked {
    unchecked {
  upa = pb; // expected-error {{assigning to 'int *' from incompatible type '_Array_ptr<int>'}}
  upa = pc; // expected-error {{assigning to 'int *' from incompatible type '_Ptr<int>'}}
    return upa;
    }
  }
}

unchecked int func59(void) {
  int a = 5;
  checked {
    {
      struct s0 {
        int *a;       // expected-error {{member cannot have an unchecked pointer type}}
        char *b;      // expected-error {{member cannot have an unchecked pointer type}}
        ptr<int> pc;
        array_ptr<int> pd : count(len);
        int len;
        short e[10];  // expected-error {{member cannot have an unchecked array type}}
        char f[10];   // expected-error {{member cannot have an unchecked array type}}
      } a;
      struct s1 unchecked {
        int *a;
        char *b;
        ptr<int> pc;
        array_ptr<int> pd : count(len);
        int len;
        short e[10];
        char f[10];
      } b;

    }
  }
  return 0;
}

// Test for structure checked block.
struct s0 checked {
  int *a;     // expected-error {{member cannot have an unchecked pointer type}}
  char *b;    // expected-error {{member cannot have an unchecked pointer type}}
  ptr<int> pc;
  array_ptr<int> pd : count(len);
  int *e : itype(array_ptr<int>);
  int len;
  struct s1 {
    int *a;   // expected-error {{member cannot have an unchecked pointer type}}
    char *b;  // expected-error {{member cannot have an unchecked pointer type}}
    ptr<int> pc;
    array_ptr<int> pd : count(len);
    int *e : itype(ptr<int>);
    int len;
  } d;
};

struct s2 {
  int *a;
  char *b;
  ptr<int> pc;
  array_ptr<int> pd : count(len);
  int *e : itype(ptr<int>);
  int len;
  struct s3 {
    int *a;
    char *b;
    ptr<int> pc;
    array_ptr<int> pd : count(len);
    int *e : itype(array_ptr<int>);
    int len;
  } d;
};

checked int func60(ptr<struct s0> st0, ptr<struct s1> st1) {
  int sum = *(st0->a) + *(st1->a);
  sum += *(st0->b) + *(st1->b);
  sum += *(st0->pc) + *(st1->pc);
  sum += *(st0->pd) + *(st1->pd);
  sum += *(st0->e) + *(st1->e);

  struct s2 sta;
  ptr<struct s2> pstb = 0;
  sum += *(sta.a) + *(sta.b) + *(sta.pc) + *(sta.pd); // expected-error 2 {{cannot use a member with an unchecked type}}
  sum += *(sta.e);
  sum += *(pstb->a) + *(pstb->b); // expected-error 2 {{cannot use a member with an unchecked type}}
  sum += *(pstb.a) + *(pstb.b);   // expected-error 2 {{cannot use a member with an unchecked type}} expected-error 2 {{member reference type}}
  sum += *(sta->a) + *(sta->b);   // expected-error 2 {{cannot use a member with an unchecked type}} expected-error 2 {{member reference type}}

  ptr<struct s2> st2 = 0;
  ptr<struct s2> st3 = 0;
  sum += *(st2->a)  + *(st3->a); // expected-error 2 {{cannot use a member with an unchecked type}}
  sum += *(st2->b)  + *(st3->b); // expected-error 2 {{cannot use a member with an unchecked type}}
  sum += *(st2->pc) + *(st3->pc);
  sum += *(st2->pd) + *(st3->pd);
  sum += *(st2->e) + *(st2->e);
  sum += *(st2->d.a) + *(st3->d.a); // expected-error 2 {{cannot use a member with an unchecked type}}
  sum += *(st2->d.b) + *(st3->d.b); // expected-error 2 {{cannot use a member with an unchecked type}}
  sum += *(st2->d.e) + *(st3->d.e);
  return sum;
}

// Change type produced by address-of operator(&) in checked block.
// ex) checked { .... int a; ptr<int> pb = &a; }
void test_addrof_checked_scope(void) checked {
  int a checked[10];
  array_ptr<int> b;
  int i;

  // In checked scope, address-of operator produces _Array_ptr<T>
  // VisitBinaryOperator - check if LHS has bounds and RHS has bounds
  ptr<int> x = &a[i]; // ImplicitCastExpr _Ptr (UnaryOperator _Array_Ptr<int>)
  ptr<int> y = &b[0]; // ImplicitCastExpr _Ptr (UnaryOperator _Array_Ptr<int>) \
                      // expected-error {{expression has no bounds, cast to ptr<T> expects source to have bounds}}
  ptr<int> z = &i;    // ImplicitCastExpr _Ptr (UnaryOperator _Array_Ptr<int>)

  x = &a[i];  // ImplicitCastExpr _Ptr (UnaryOperator _Array_ptr<int>)
  y = &b[1];  // ImplicitCastExpr _Ptr (UnaryOperator _Array_ptr<int>) \
              // expected-error {{expression has no bounds, right-hand side of assignment expected to have bounds}} \
              // expected-error {{expression has no bounds, cast to ptr<T> expects source to have bounds}}
  z = &i;     // ImplicitCastExpr _Ptr (UnaryOperator _Array_Ptr<int>)

  x = b;      // BinaryOperator (ImplicitCastExpr _Ptr (_Array_ptr)) \
              // expected-error {{expression has no bounds, right-hand side of assignment expected to have bounds}} \
              // expected-error {{expression has no bounds, cast to ptr<T> expects source to have bounds}}

  array_ptr<int> ax = &a[i];
  array_ptr<int> ay = &b[2];
  array_ptr<int> az = &i;

  ax = &a[i];
  ay = &b[3];
  az = &i;

  // &*(b+i) is different from &b[i]
  // &*(b+i) result type = type of (b+i) = array_ptr<int>
  // &b[i] result type = array_ptr<int>
  array_ptr<int> px = &(*(b+i));
  array_ptr<int> py = &b[i];
  array_ptr<int> pz = &(*(a+i));

  px = &(*(b+i));
  py = &b[i];
  pz = &(*(a+i));

  int aa[5];  // expected-error {{variable cannot have an unchecked array type}}
  x = &aa[i];
  y = &*(aa+i);
}

void test_addrof_unchecked_scope(void) unchecked {
  int a checked[10];
  array_ptr<int> b;
  int i;

  // checkSingleAssignmentConstraints(int * -> _Ptr<int> implicit casting)
  ptr<int> x = &a[i]; // ImplicitCastExpr _Ptr<int>(UnaryOperator int * prefix &)
  ptr<int> y = &b[0]; // ImplicitCastExpr _Ptr<int>(UnaryOperator int * prefix &) \
                      // expected-error {{expression has no bounds, cast to ptr<T> expects source to have bounds}}
  ptr<int> z = &i;    // ImplicitCastExpr _Ptr<int>(UnaryOperator int * prefix &)

  // implicit cast for _Ptr<T> requires source bounds
  x = &a[i];  // BinaryOperator(ImplicitCastExpr _Ptr<int>(UnaryOperator int * prefix &))
  y = &b[0];  // BinaryOperator(ImplicitCastExpr _Ptr<int>(UnaryOperator int * prefix &)) \
              // expected-error {{expression has no bounds, right-hand side of assignment expected to have bounds}} \
              // expected-error {{expression has no bounds, cast to ptr<T> expects source to have bounds}}
  z = &i;     // BinaryOperator(ImplicitCastExpr _Ptr<int>(UnaryOperator int * prefix &))

  x = b;      // BinaryOperator(ImplicitCastExpr()) \
              // expected-error {{expression has no bounds, right-hand side of assignment expected to have bounds}} \
              // expected-error {{expression has no bounds, cast to ptr<T> expects source to have bounds}}

  // checkSingleAssignmentConstraints(int * -> _Array_ptr<int> implicit casting)
  array_ptr<int> ax = &a[i];  // ImplicitCastExpr _Array_ptr<int>(UnaryOperator)
  array_ptr<int> ay = &b[0];  // ImplicitCastExpr _Array_ptr<int>(UnaryOperator)
  array_ptr<int> az = &i;     // ImplicitCastExpr _Array_ptr<int>(UnaryOperator)

  ax = &a[i]; // ImplicitCastExpr _Array_ptr<int>(UnaryOperator)
  ay = &b[0]; // ImplicitCastExpr _Array_ptr<int>(UnaryOperator)
  az = &i; // ImplicitCastExpr _Array_ptr<int>(UnaryOperator)

  // &E, T(type of E)
  // address of operand = UnaryOperator(UO_AddrOf, E)
  // address of type
  // if E is *SE, &E = &*SE = type of SE
  // if E is in unchecked block, it is unchecked pointer type of E(T *)
  // if E is in checked block, it is checked pointer type of E(_Ptr<T>)

  array_ptr<int> px = &(*(b+i));  // UnaryOperator _Array_ptr<int>(ParenExpr(UnaryOperator(*))
  array_ptr<int> py = &b[i];      // ImplicitCastExpr _Array_ptr<int>(UnaryOperator)
  array_ptr<int> pz = &(*(a+i));  // UnaryOperator _Array_ptr<int>(ParenExpr(UnaryOperator(*)))

  px = &(*(b+i)); // UnaryOperator _Array_ptr<int>()
  py = &b[i];     // ImplicitCastExpr _Array_ptr<int>(UnaryOperator)
  pz = &(*(a+i)); // UnaryOperator _Array_ptr<int>()
}

int *gptr0 : itype(ptr<int>);
int *gptr1 : itype(array_ptr<int>);
int *gptr2;

extern void check_indirection_unchecked(int p[10], const int const_p[10], int y) checked {
  *p = y;       // expected-error {{cannot use a parameter with an unchecked type}}
  y = *p;       // expected-error {{cannot use a parameter with an unchecked type}}
  *const_p = y; // expected-error {{cannot use a parameter with an unchecked type}}
  y = *const_p; // expected-error {{cannot use a parameter with an unchecked type}}
}

extern void check_subscript_unchecked(int p[10], int y) checked {
  p[0] = y;     // expected-error {{cannot use a parameter with an unchecked type}}
  y = p[0];     // expected-error {{cannot use a parameter with an unchecked type}}
  0[p] = y;     // expected-error {{cannot use a parameter with an unchecked type}}
  y = 0[p];     // expected-error {{cannot use a parameter with an unchecked type}}
}

extern void check_subscript_checked(int p checked[10], const int p_const[10], int y) checked {
  p[0] = y;  // OK
  y = p[0];  // OK
  0[p] = y;  // OK
  y = 0[p];  // OK
  p_const[0] = y;  // expected-error {{cannot use a parameter with an unchecked type}}
  y = p_const[0];  // expected-error {{cannot use a parameter with an unchecked type}}
  0[p_const] = y;  // expected-error {{cannot use a parameter with an unchecked type}}
  y = 0[p_const];  // expected-error {{cannot use a parameter with an unchecked type}}
}

static int global_arr[10];
static int global_checked_arr checked[10];

typedef int unchecked_arr_type[10];
typedef int checked_arr_type[10];

// Test assignments between pointers and arrays, excluding const/volatile attributes.
extern void check_assign(int val, int p[10], int q[], int r checked[10], int s checked[],
                         int s2d checked[10][10]) {
  int t[10];
  int t2d[10][10];
  int u checked[10];
  int u2d checked[10][10];  // This is a checked array of checked arrays. checked propagates
                            // to immediately nested array types in array declarators.  It does
                            // not propagate through typedefs

  int *t1, *t2, *t3, *t4, *t5, *t6;
  array_ptr<int> t7, t8, t9, t10, t11, t12, t13, t14, t15;
  int *t16, *t17, *t18;
  int (*t19)[10];
  int (*t20)[10];
  int (*t21)[10];

  array_ptr<int[10]> t22, t23, t24;
  array_ptr<int checked[10]> t25, t26, t27;

  checked {
  // Single-dimensional array type conversions to pointer types.
  t1 = p; // expected-error {{cannot use a parameter with an unchecked type}} \
          // expected-error {{cannot use a variable with an unchecked type}}
          // T *  = T[constant] OK
  t2 = q; // expected-error {{cannot use a parameter with an unchecked type}} \
          // expected-error {{cannot use a variable with an unchecked type}}
          // T *  = T[] OK
  t3 = t; // expected-error 2 {{cannot use a variable with an unchecked type}} \
          // T *  = T[constant] OK;
  t4 = r; // expected-error {{cannot use a variable with an unchecked type}} \
          // Assignment of checked pointer to unchecked pointer not allowed
  t5 = s; // expected-error {{cannot use a variable with an unchecked type}}
  t6 = u; // expected-error {{cannot use a variable with an unchecked type}}

  // Various forms of array_ptr<T> = T[]. Note that the rhs does not need to have known bounds
  // because the lhs pointers have no bounds (and cannot be dereferenced).
  //
  // Note if there need to be known bounds, the bounds of p and q are unknown
  // because C does not guarantee that array sizes match for parameter passing
  t7 = p;         // expected-error {{cannot use a parameter with an unchecked type}}
  t8 = q;         // expected-error {{cannot use a parameter with an unchecked type}}
  t9 = r;
  t10 = s;
  t11 = t;        // expected-error {{cannot use a variable with an unchecked type}}
  t12 = u;
  t13 = s2d[0];
  t14 = t2d[0];   // expected-error {{cannot use a variable with an unchecked type}}
  t15 = u2d[0];


  // Multi-dimensional array type conversions to pointer types.
  t16 = s2d[0]; // expected-error {{cannot use a variable with an unchecked type}}
  t17 = t2d[0]; // expected-error 2 {{cannot use a variable with an unchecked type}}
  t18 = u2d[0]; // expected-error {{cannot use a variable with an unchecked type}}
  t19 = s2d;    // expected-error {{cannot use a variable with an unchecked type}}
  t20 = t2d;    // expected-error 2 {{cannot use a variable with an unchecked type}}
  t21 = u2d;    // expected-error {{cannot use a variable with an unchecked type}}

  // Constructed type that contains unchecked pointer/array types in a checked scope
  // Checked pointer type to unchecked pointer/array type is not allowed
  t22 = s2d;  // expected-error {{cannot use a variable with an unchecked type}} array_ptr<unchecked array type>, not OK
  t23 = t2d;  // expected-error 2 {{cannot use a variable with an unchecked type}} array_ptr<unchecked array type>, not OK
  t24 = u2d;  // expected-error {{cannot use a variable with an unchecked type}} array_ptr<unchecked array type>, not OK
  t25 = s2d;
  t26 = t2d;  // expected-error {{cannot use a variable with an unchecked type}}
  t27 = u2d;

  // Assignments to array-typed parameters are allowed.  The outermost array modifier
  // is converted to a pointer type.
  p = q;  // expected-error 2 {{cannot use a parameter with an unchecked type}}
  q = p;  // expected-error 2 {{cannot use a parameter with an unchecked type}}
  s = r;
  r = t;  // expected-error {{cannot use a variable with an unchecked type}}
  p = r;  // expected-error {{cannot use a parameter with an unchecked type}}
          // assignment of checked pointer to unchecked pointer not allowed

  // Assignments to array-typed local and global variables are not allowed
  t = p;  // expected-error {{cannot use a parameter with an unchecked type}} \
          // expected-error {{cannot use a variable with an unchecked type}}
  t = r;  // expected-error {{cannot use a variable with an unchecked type}}
  u = r;  // expected-error {{array type 'int checked[10]' is not assignable}}
  global_arr = p; // expected-error {{cannot use a variable with an unchecked type}} \
                  // expected-error {{cannot use a parameter with an unchecked type}}
  global_arr = r; // expected-error {{cannot use a variable with an unchecked type}}
  global_checked_arr = r; // expected-error {{array type 'int checked[10]' is not assignable}}
  }
}

// Test that dimensions in multi-dimensional arrays are either all checked or unchecked arrays.
extern void check_dimensions1(void) checked {
  int t1 checked[10][5]checked[5];     // multiple checked modifiers are allowed
  int t2 checked[10][5][5]checked[5];

  // checked mixing of checked/unchecked array dimensions
  int t3[10]checked[10];                // expected-error {{variable cannot have an unchecked array type}} \
                                        // expected-error {{unchecked array of checked array not allowed}}
  typedef int dim_unchecked[10];
  dim_unchecked t4 checked[10];         // expected-error {{variable cannot have an unchecked array type}} \
                                        // expected-error {{checked array of unchecked array not allowed ('dim_unchecked' is an unchecked array)}}

  typedef int dim_checked checked[10];
  dim_checked t5[10];                   // expected-error {{variable cannot have an unchecked array type}} \
                                        // expected-error {{unchecked array of checked array not allowed ('dim_checked' is a checked array)}}

  // checked parenthesized declarators
  int (t6 checked[10])[10];             // checked propagates to enclosing array declarators
  int(t7 checked[10])[5][5]checked[5];  // multiple checked modifiers are allowed
  int (t8[10])checked[10];              // expected-error {{variable cannot have an unchecked array type}} expected-error {{unchecked array of checked array not allowed}}
  int ((t9[10]))checked[10];            // expected-error {{variable cannot have an unchecked array type}} expected-error {{unchecked array of checked array not allowed}}
  dim_unchecked (t10 checked[10])[10];  // expected-error {{variable cannot have an unchecked array type}} expected-error {{checked array of unchecked array not allowed ('dim_unchecked' is an unchecked array)}}
  dim_checked (t11[10])[10];            // expected-error {{variable cannot have an unchecked array type}} expected-error {{unchecked array of checked array not allowed ('dim_checked' is a checked array)}}

  // make sure checked-ness propagated
  // NOTE : checked scope error fails to precede
  int *t12 = t6[0];                    // expected-error {{variable cannot have an unchecked pointer type}}
  array_ptr<int> t13 = t6[0];
}

// Test conditional expressions where arms have different
// kinds of checked and unchecked arrays.
extern void check_condexpr(int val) {
  int p [5];
  int r checked[5];
  float s[5];
  float u checked[5];
  int *t1;
  array_ptr<int> t2, t3, t4;

  checked {
  t1 = val ? p : p; // expected-error 3 {{cannot use a variable with an unchecked type}} T[5] and T[5] OK
  t2 = val ? p : r; // expected-error {{cannot use a variable with an unchecked type}} T[5] and T checked[5] OK
  t3 = val ? r : p; // expected-error {{cannot use a variable with an unchecked type}} T checked[5] and T[5] OK
  t4 = val ? r : r; // T checked[5] and T checked[5] OK.

  // omit assignment because type of expression is not valid when there is an error.
  val ? s : r;    // expected-error {{cannot use a variable with an unchecked type}}
  val ? r : s;    // expected-error {{cannot use a variable with an unchecked type}}
  val ? r : u;    // expected-error {{pointer type mismatch}}
  val ? u : r;    // expected-error {{pointer type mismatch}}

  // Some C compilers have allowed implicit integer to pointer conversions.
  // These are errors for the new safe pointer types.
  val ? r : val;   // expected-error {{incompatible operand types}}
  val ? val : r;   // expected-error {{incompatible operand types}}
  val ? u : val;   // expected-error {{incompatible operand types}}
  val ? val : u;   // expected-error {{incompatible operand types}}

  // Implicit conversion of 0 to a safe pointer type is OK.
  array_ptr<int> t5 = val ? r : 0;
  array_ptr<int> t6 = val ? 0 : r;
  array_ptr<float> t7 = val ? u : 0;
  array_ptr<float> t8 = val ? 0 : u;
  }
}

// Define functions used to test typechecking of call expressions.

extern void f1(int *p, int y) {
}

extern void f2(int p[10], int y) {
}

extern void f3(int p checked[10], int y) {
  *p = y;
}

extern void f4(int **p, int y) {
}

checked extern void f5(int(*p)[10], int y) {  // expected-error {{parameter cannot have an unchecked pointer type}}
}

checked extern void f6(ptr<int[10]> p, int y) { // expected-error {{parameter cannot have an unchecked array type}} \
                                                // ptr<unchecked array> not OK in checked scope
}

checked extern void f7(array_ptr<int[10]> p, int y) { // expected-error {{parameter cannot have an unchecked array type}} \
                                                      // array_ptr<unchecked array> not OK in checked scope
}

extern void f8(int(*p) checked[10], int y) {
}

extern void f9(ptr<int checked[10]> p, int y) {
}

extern void f10(array_ptr<int checked[10]> p, int y) {
}

extern void f11(int p[10][10], int y) {
}

extern void f12(int p checked[10][10],int y) {
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

extern void check_call(void) {
  int x[10];
  int y checked[10];
  int x2d[10][10];
  int y2d checked[10][10];

  checked {
  // checked scope error precedes function call type error/warning
  // f1(int *p, int y)
  f1(x, 0);               // expected-error {{cannot use a variable with an unchecked type}}
  f1(y, 0);               // expected-error {{parameter of incompatible type 'int *'}}
  f1(x2d, 0);             // expected-error {{cannot use a variable with an unchecked type}}
  f1(y2d, 0);             // expected-error {{passing 'int checked[10][10]' to parameter of incompatible type 'int *'}}

  // f2(int p[10], int y)
  f2(x, 0);               // expected-error {{cannot use a variable with an unchecked type}}
  f2(y, 0);               // expected-error {{parameter of incompatible type 'int *'}}
  f2(x2d, 0);             // expected-error {{cannot use a variable with an unchecked type}}
  f2(y2d, 0);             // expected-error {{passing 'int checked[10][10]' to parameter of incompatible type 'int *'}}

  // f3(int p checked[10], int y)
  f3(x, 0);               // expected-error {{cannot use a variable with an unchecked type}}
  f3(y, 0);
  f3(x2d, 0);             // expected-error {{cannot use a variable with an unchecked type}}
  f3(y2d, 0);             // expected-error {{passing 'int checked[10][10]' to parameter of incompatible type '_Array_ptr<int>'}}

  // f4(int **p, int y);
  f4(x, 0);               // expected-error {{cannot use a variable with an unchecked type}}
  f4(y, 0);               // expected-error {{passing 'int checked[10]' to parameter of incompatible type 'int **'}}
  f4(x2d, 0);             // expected-error {{cannot use a variable with an unchecked type}}
  f4(y2d, 0);             // expected-error {{passing 'int checked[10][10]' to parameter of incompatible type 'int **'}}

  // f5(int (*p)[10], int y);
  f5(x, 0);               // expected-error {{cannot use a variable with an unchecked type}}
  f5(y, 0);               // expected-error {{passing 'int checked[10]' to parameter of incompatible type 'int (*)[10]'}}
  f5(x2d, 0);             // expected-error {{cannot use a variable with an unchecked type}}
  f5(y2d, 0);             // expected-error {{passing 'int checked[10][10]' to parameter of incompatible type 'int (*)[10]'}}

   // f6(ptr<int[10]>, int y);
  f6(x, 0);               // expected-error {{cannot use a variable with an unchecked type}}
  f6(y, 0);               // expected-error {{passing 'int checked[10]' to parameter of incompatible type '_Ptr<int [10]>'}}
  f6(x2d, 0);             // expected-error {{cannot use a variable with an unchecked type}}
  f6(y2d, 0);             // expected-error {{passing 'int checked[10][10]' to parameter of incompatible type '_Ptr<int [10]>'}}

   // f7(array_ptr<int[10]>, int y);
  f7(x, 0);               // expected-error {{cannot use a variable with an unchecked type}}
  f7(y, 0);               // expected-error {{parameter of incompatible type}}
  f7(x2d, 0);             // expected-error {{cannot use a variable with an unchecked type}}
  f7(y2d, 0);             // expected-error {{parameter of incompatible type}}

  // f8(int (*p) checked[10], int y);
  f8(x, 0);               // expected-error {{cannot use a variable with an unchecked type}}
  f8(y, 0);               // expected-error {{parameter of incompatible type}}
  f8(x2d, 0);             // expected-error {{cannot use a variable with an unchecked type}}
  f8(y2d, 0);             // expected-error {{parameter of incompatible type}}

  // f9(ptr<int checked[10]> p, int y);
  f8(x, 0);               // expected-error {{cannot use a variable with an unchecked type}}
  f8(y, 0);               // expected-error {{parameter of incompatible type}}
  f8(x2d, 0);             // expected-error {{cannot use a variable with an unchecked type}}
  f8(y2d, 0);             // expected-error {{parameter of incompatible type}}

  // f10(array_ptr<int checked[10]> p, int y);
  f10(x, 0);              // expected-error {{cannot use a variable with an unchecked type}}
  f10(y, 0);              // expected-error {{parameter of incompatible type}}
  f10(x2d, 0);            // expected-error {{cannot use a variable with an unchecked type}}
  f10(y2d, 0);            // OK

  // f11(int p[10][10], int y);
  f11(x, 0);              // expected-error {{cannot use a variable with an unchecked type}}
  f11(y, 0);              // expected-error {{parameter of incompatible type}}
  f11(x2d, 0);            // expected-error {{cannot use a variable with an unchecked type}}
  f11(y2d, 0);            // expected-error {{parameter of incompatible type}}

  // f12(int p checked[10][10], int y);
  f12(x, 0);              // expected-error {{cannot use a variable with an unchecked type}}
  f12(y, 0);              // expected-error {{parameter of incompatible type}}
  f12(x2d, 0);            // expected-error {{cannot use a variable with an unchecked type}}
  f12(y2d, 0);            // OK

  // f13(_Bool b, int y);
  f13(x, 0);              // expected-error {{cannot use a variable with an unchecked type}}
  f13(y, 0);              // OK
  f13(x2d, 0);            // expected-error {{cannot use a variable with an unchecked type}}
  f13(y2d, 0);            // OK

  // spot check calls where an array is the second argument
  g1(0, x);   // expected-error {{cannot use a variable with an unchecked type}}
  g1(0, y);   // expected-error {{parameter of incompatible type}}
  g2(0, x);   // expected-error {{cannot use a variable with an unchecked type}}
  g2(0, y);   // expected-error {{parameter of incompatible type}}
  g3(0, x);   // expected-error {{cannot use a variable with an unchecked type}}
  g3(0, y);
  }
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

int global[10];
int checked_global checked[10];
int global_arr1[10];

checked int *h3(void) {   // expected-error {{return cannot have an unchecked pointer type}}
  return global;    // expected-error {{cannot use a variable with an unchecked type}}
}

checked ptr<int> h4(void) {
  return global;    // expected-error {{cannot use a variable with an unchecked type}}
}

checked array_ptr<int> h5(void) {
  return global;    // expected-error {{cannot use a variable with an unchecked type}}
}

checked int *h6(void) {   // expected-error {{return cannot have an unchecked pointer type}}
  return checked_global;  // expected-error {{incompatible result type}}
}

checked ptr<int> h7(void) {
  return checked_global; // ptr<T> = array_ptr<T> OK
}

checked array_ptr<int> h8(void) {
  return checked_global;
}

checked int *h9(int arr[10]) {    // expected-error {{return cannot have an unchecked pointer type}} expected-error {{parameter cannot have an unchecked array type}}
  return arr;
}

checked ptr<int> h10(void) {
  return global_arr1;   // expected-error {{cannot use a variable with an unchecked type}}
}

checked array_ptr<int> h11(int arr checked[10]) {
  return arr;
}

checked int *h12(int arr checked[10]) {   // expected-error {{return cannot have an unchecked pointer type}}
  return arr;  // expected-error {{incompatible result type}}
}

checked ptr<int> h13(int arr checked[10]) {
  return arr;  // ptr<T> = array_ptr<T> OK
}

checked array_ptr<int> h14(int arr checked[10]) {
  return arr;
}

checked int *h15(int arr checked[]) {   // expected-error {{return cannot have an unchecked pointer type}}
  return arr;  // expected-error {{incompatible result type}}
}

checked ptr<int> h17(int arr checked[]) {
  return arr;  // ptr<T> = array_ptr<T> OK, expected-error {{expression has no bounds}}
}

checked array_ptr<int> h18(int arr checked[]) {
  return arr;
}

// h19 is a function that returns a pointer to a 10-element array of integers.
checked int(*h19(int arr[10][10]))[10]{   // expected-error {{return cannot have an unchecked pointer type}} expected-error {{parameter cannot have an unchecked array type}}
  return arr;
}

int global_arr2[10][10];
checked ptr<int[10]> h20(void) {  // expected-error {{return cannot have an unchecked array type}}
  return global_arr2; // expected-error {{cannot use a variable with an unchecked type}}
}

checked array_ptr<int[10]> h21(int arr[10][10]) { // expected-error {{return cannot have an unchecked array type}} \
                                                  // expected-error {{parameter cannot have an unchecked array type}}
  return arr;
}

// h22 is a function that returns a pointer to a 10-element array of integers.
checked int (*h22(int arr checked[10][10]))[10] { // expected-error {{return cannot have an unchecked pointer type}}
  return arr;  // expected-error {{incompatible result type}}
}

checked ptr<int[10]> h23(int arr checked[10][10]) { // expected-error {{return cannot have an unchecked array type}}
  return arr;  // expected-error {{incompatible result type}}
}

checked array_ptr<int[10]> h24(int arr checked[10][10]) { // expected-error {{return cannot have an unchecked array type}}
  return arr;  // expected-error {{incompatible result type}}
}

// h25 is a function that returns a pointer to 10-element array of integers.
checked int (*h25(int arr checked[10][10])) checked[10]{  // expected-error {{return cannot have an unchecked pointer type}} \
                                                          // return int checked[10]
  return arr;  // expected-error {{incompatible result type}}
}

checked ptr<int checked[10]> h26(int arr checked[10][10]) {
  return arr;  // ptr<T> = array_ptr<T> OK
}

checked array_ptr<int checked[10]> h27(int arr checked[10][10]) {
  return arr;
}

// Test for type restrictions on various kinds of expression
// such as pointer arithmetic, logical operator, pointer difference or relational/equal compare
void check_pointer_arithmetic(void) {
  int p[5];
  int r checked[5];

  int *p_tmp;
  array_ptr<int> r_tmp;

  checked {
  p_tmp = p + 5;    // expected-error 2 {{cannot use a variable with an unchecked type}}
  p_tmp = 5 + p;    // expected-error 2 {{cannot use a variable with an unchecked type}}
  p_tmp = p_tmp - 2;// expected-error 2 {{cannot use a variable with an unchecked type}}
  p_tmp = 2 - p;    // expected-error 2 {{cannot use a variable with an unchecked type}}
  p_tmp = p++;      // expected-error 2 {{cannot use a variable with an unchecked type}}
  p_tmp = p--;      // expected-error 2 {{cannot use a variable with an unchecked type}}
  p_tmp = ++p;      // expected-error 2 {{cannot use a variable with an unchecked type}}
  p_tmp = --p;      // expected-error 2 {{cannot use a variable with an unchecked type}}
  p_tmp = (p += 1); // expected-error 2 {{cannot use a variable with an unchecked type}}
  p_tmp = (p -= 1); // expected-error 2 {{cannot use a variable with an unchecked type}}

  // 0 interpreted as an integer, not null
  p_tmp = p + 0;    // expected-error 2 {{cannot use a variable with an unchecked type}}
  p_tmp = 0 + p;    // expected-error 2 {{cannot use a variable with an unchecked type}}
  p_tmp = p - 0;    // expected-error 2 {{cannot use a variable with an unchecked type}}
  p_tmp = 0 - p;    // expected-error 2 {{cannot use a variable with an unchecked type}}

  r_tmp = r + 5;
  r_tmp = 5 + r;
  r_tmp = r_tmp - 2;
  r_tmp = 2 - r;      // expected-error {{invalid operands}}
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
  r_tmp = 0 - r;      // expected-error {{invalid operands to binary expression}}

  // adding two pointers is not allowed
  r + r; // expected-error {{invalid operands}}
  }
}

void check_pointer_difference(int flag) {
  int count;
  int val_int[5];
  float val_float[5];
  int *p_int = val_int;
  float *p_float = val_float;
  array_ptr<int> r_int = val_int;

  int a_int[5];
  int checked_a_int checked[5];

  float a_float[5];
  float checked_a_float checked[5];

  checked {
  if (flag) {
      p_int = a_int;  // expected-error 2 {{cannot use a variable with an unchecked type}}
      r_int = checked_a_int;
  }

  // pointer - array
  count = p_int - a_int;        // expected-error 2 {{cannot use a variable with an unchecked type}}
  count = p_int - checked_a_int;// expected-error {{cannot use a variable with an unchecked type}}
  count = r_int - a_int;        // expected-error {{cannot use a variable with an unchecked type}}
  count = r_int - checked_a_int;

  // array - pointer
  count = a_int - p_int;        // expected-error 2 {{cannot use a variable with an unchecked type}}
  count = checked_a_int - p_int;// expected-error {{cannot use a variable with an unchecked type}}
  count = a_int - r_int;        // expected-error {{cannot use a variable with an unchecked type}}
  count = checked_a_int - r_int;

  // spot check mismatched types
  count = a_float - p_int;          // expected-error 2 {{cannot use a variable with an unchecked type}}
  count = p_int - checked_a_float;  // expected-error {{cannot use a variable with an unchecked type}}
  count = checked_a_float - r_int;  // expected-error {{not pointers to compatible types}}
  }
}

void check_pointer_compare(void) {
  int result;

  int val_int[5];
  float val_float[5];
  int checked_val_int checked[5];
  float checked_val_float checked[5];

  int *p_int = val_int;
  float *p_float = val_float;

  ptr<int> q_int = 0;
  q_int = val_int;
  ptr<float> q_float = 0;
  q_float = val_float;

  array_ptr<int> r_int = val_int;
  array_ptr<float> r_float = val_float;

  checked {
  // relational comparisons between pointers and unchecked arrays;
  result = val_int < p_int; // expected-error 2 {{cannot use a variable with an unchecked type}}
  result = val_int <= q_int;// expected-error {{cannot use a variable with an unchecked type}}
  result = val_int >= r_int;// expected-error {{cannot use a variable with an unchecked type}}

  result = p_int > val_int; // expected-error 2 {{cannot use a variable with an unchecked type}}
  result = q_int < val_int; // expected-error {{cannot use a variable with an unchecked type}}
  result = r_int <= val_int;// expected-error {{cannot use a variable with an unchecked type}}


  // relational comparisons between pointers and checked arrays;
  result = checked_val_int < p_int; // expected-error {{cannot use a variable with an unchecked type}}
  result = checked_val_int <= q_int;
  result = checked_val_int >= r_int;

  result = p_int > checked_val_int; // expected-error {{cannot use a variable with an unchecked type}}
  result = q_int < checked_val_int;
  result = r_int <= checked_val_int;

  // invalid relational comparisons

  // spot check comparisons between pointers and unchecked arrays;
  result = val_int < p_float;   // expected-error 2 {{cannot use a variable with an unchecked type}}
  result = val_float <= q_int;  // expected-error {{cannot use a variable with an unchecked type}}
  result = val_int >= r_float;  // expected-error {{cannot use a variable with an unchecked type}}

  result = p_int > val_float;   // expected-error 2 {{cannot use a variable with an unchecked type}}
  result = q_float < val_int;   // expected-error {{cannot use a variable with an unchecked type}}
  result = r_int <= val_float;  // expected-error {{cannot use a variable with an unchecked type}}

  // spot check comparisons between pointers and checked arrays;
  result = checked_val_int < p_float; // expected-error {{cannot use a variable with an unchecked type}}
  result = checked_val_float <= q_int;// expected-warning {{comparison of distinct pointer types}}
  result = checked_val_int >= r_float;// expected-warning {{comparison of distinct pointer types}}

  result = p_int > checked_val_float; // expected-error {{cannot use a variable with an unchecked type}}
  result = q_float < checked_val_int; // expected-warning {{comparison of distinct pointer types}}
  result = r_int <= checked_val_float;// expected-warning {{comparison of distinct pointer types}}

  // equality/inequality comparisons between pointers and unchecked arrays;
  result = val_int == p_int;  // expected-error 2 {{cannot use a variable with an unchecked type}}
  result = val_int != q_int;  // expected-error {{cannot use a variable with an unchecked type}}
  result = val_int == r_int;  // expected-error {{cannot use a variable with an unchecked type}}

  result = p_int != val_int;  // expected-error 2 {{cannot use a variable with an unchecked type}}
  result = q_int == val_int;  // expected-error {{cannot use a variable with an unchecked type}}
  result = r_int != val_int;  // expected-error {{cannot use a variable with an unchecked type}}

  // equality/inequality comparisons between pointers and checked arrays;
  result = checked_val_int == p_int;  // expected-error {{cannot use a variable with an unchecked type}}
  result = checked_val_int != q_int;
  result = checked_val_int == r_int;

  result = p_int != checked_val_int;  // expected-error {{cannot use a variable with an unchecked type}}
  result = q_int == checked_val_int;
  result = r_int != checked_val_int;

  // invalid equality/inequality comparisons

  // spot check equality comparisons between pointers and unchecked arrays;
  result = val_int == p_float;  // expected-error 2 {{cannot use a variable with an unchecked type}}
  result = val_float != q_int;  // expected-error {{cannot use a variable with an unchecked type}}
  result = val_int == r_float;  // expected-error {{cannot use a variable with an unchecked type}}

  result = p_int != val_float;  // expected-error 2 {{cannot use a variable with an unchecked type}}
  result = q_float == val_int;  // expected-error {{cannot use a variable with an unchecked type}}
  result = r_int != val_float;  // expected-error {{cannot use a variable with an unchecked type}}

  // spot check equality comparisons between pointers and checked arrays;
  result = checked_val_int == p_float;  // expected-error {{cannot use a variable with an unchecked type}}
  result = checked_val_float != q_int;  // expected-warning {{comparison of distinct pointer types}}
  result = checked_val_int != r_float;  // expected-warning {{comparison of distinct pointer types}}

  result = p_int == checked_val_float;  // expected-error {{cannot use a variable with an unchecked type}}
  result = q_float != checked_val_int;  // expected-warning {{comparison of distinct pointer types}}
  result = r_int == checked_val_float;  // expected-warning {{comparison of distinct pointer types}}

  }
}

void check_logical_operators(void) {
  int p[5];
  int r checked[5];

  _Bool b;

  checked {
  b = !p;     // expected-error {{cannot use a variable with an unchecked type}}
  b = !r;

  b = p || b;     // expected-error {{cannot use a variable with an unchecked type}}
  b = r || b;
  b = b || p;     // expected-error {{cannot use a variable with an unchecked type}}
  b = b || r;

  b = r || p;     // expected-error {{cannot use a variable with an unchecked type}}
  b = p || r;     // expected-error {{cannot use a variable with an unchecked type}}

  b = p && b;     // expected-error {{cannot use a variable with an unchecked type}}
  b = r && b;
  b = b && p;     // expected-error {{cannot use a variable with an unchecked type}}
  b = b && r;

  b = r && p;     // expected-error {{cannot use a variable with an unchecked type}}
  b = p && r;     // expected-error {{cannot use a variable with an unchecked type}}
  }
}

checked void check_cast_operator(void) {
  int x = 0;
  int arr checked[5];

  // casts involving array types
  array_ptr<int> pax = (array_ptr<int>) &x;
  pax = (int checked[]) &x;   // expected-error {{cast to incomplete type}}
  pax = (int checked[1]) &x;  // expected-error {{arithmetic or pointer type is required}}
  pax = &x;

  // casts involving pointers to array types

  // unchecked pointer to array
  ptr<int checked[5]> parr = 0;
  parr = (int(*)checked[5]) &arr; // expected-error {{invalid casting to unchecked pointer type}}
  parr = (int(*)checked[5]) ((int(*)checked[]) &arr); // expected-error {{invalid casting to unchecked pointer type}}
  parr = (int(*)checked[3]) &arr;   // expected-error {{invalid casting to unchecked pointer type}}
  parr = (int(*)[5]) &arr;          // expected-error {{invalid casting to unchecked pointer type}}
  parr = (int**) &arr;              // expected-error {{invalid casting to unchecked pointer type}}

  // ptr to array, ptr to unchecked array
  parr = (ptr<int checked[5]>) &arr;
  parr = (ptr<int checked[5]>) ((ptr<int checked[]>) &arr);
  parr = (ptr<int checked[3]>) &arr;  // expected-error {{incompatible type}}
  parr = (ptr<int [5]>) &arr;   // expected-error {{invalid casting to unchecked array type}}
  parr = (ptr<int *>) &arr;     // expected-error {{invalid casting to unchecked pointer type}}
  parr = (ptr<ptr<int>>) &arr;  // expected-error {{incompatible type}}

  // array_ptr to array, array_ptr to unchecked array
  array_ptr<int checked[5]> aparr = 0;
  aparr = (array_ptr<int checked[5]>) &arr;
  aparr = (array_ptr<int checked[5]>) ((array_ptr<int checked[]>) &arr);
  aparr = (array_ptr<int checked[3]>) &arr; // expected-error {{incompatible type}}
  aparr = (array_ptr<int [5]>) &arr;  // expected-error {{invalid casting to unchecked array type}}

  // constructed type compatibility check, base type SHOULD be equal
  ptr<int> pa = 0;
  ptr<ptr<int>> ppa = 0;
  ptr<array_ptr<int>> papa = 0;

  ppa = (ptr<ptr<int>>) &pa;
  ppa = (ptr<array_ptr<int>>) &pa;      // expected-error {{incompatible type}} \
                                        // ptr<ptr<T>> = ptr<array_ptr<T>>, not OK
  ppa = (array_ptr<ptr<int>>) &pa;      // ptr<ptr<T>> = array_ptr<ptr<T>>, OK
  ppa = (array_ptr<array_ptr<int>>) &pa;// expected-error {{incompatible type}} \
                                        // ptr<ptr<T>> = array_ptr<array_ptr<T>>, not OK

  papa = (ptr<ptr<int>>) &pa;           // expected-error {{incompatible type}} \
                                        // ptr<array_ptr<T>> = ptr<ptr<T>>, not OK
  papa = (ptr<array_ptr<int>>) &pa;
  papa = (array_ptr<ptr<int>>) &pa;     // expected-error {{incompatible type}} \
                                        // ptr<array_ptr<T>> = array_ptr<ptr<T>>, not OK
  papa = (array_ptr<array_ptr<int>>) &pa;


  // ptr to variadic/unchecked func pointer
  ptr<int(int,int)> vpa = 0;
  vpa = (ptr<int(int,...)>) &arr;  // expected-error {{invalid casting to type having variable arguments}}
  vpa = (ptr<int(int,ptr<int(int, ...)>)>) &arr;  // expected-error {{invalid casting to type having variable arguments}}
  vpa = (ptr<int*(int,int)>) &arr;  // expected-error {{invalid casting to unchecked pointer type}}
  vpa = (ptr<int(int,ptr<int(int, ...)>, ...)>) &arr;  // expected-error {{invalid casting to type having variable arguments}}

  int *upa;                         // expected-error {{variable cannot have an unchecked pointer type}}
  upa = arr;
  upa = (int *)(array_ptr<int>)arr; // expected-error {{invalid casting to unchecked pointer type in a checked scope}}
  upa = &x;
  upa = (int [])&x;                 // expected-error {{cast to incomplete type}}
  upa = (int(*)[5])&x;              // expected-error {{invalid casting to unchecked pointer type}}

  upa = pax;
  upa = (int *)(array_ptr<int>)pax; // expected-error {{invalid casting to unchecked pointer type in a checked scope}}

  upa = aparr;
  upa = (int *)aparr;               // expected-error {{invalid casting to unchecked pointer type}}

  gptr0 = upa;
  gptr0 = (ptr<int>)upa;
  gptr0 = (array_ptr<int>)upa;

  gptr1 = upa;
  gptr1 = (ptr<int>)upa;
  gptr1 = (array_ptr<int>)upa;

  gptr2 = upa;                      // expected-error {{cannot use a variable with an unchecked type}}
  gptr2 = (ptr<int>)upa;            // expected-error {{cannot use a variable with an unchecked type}}
  gptr2 = (array_ptr<int>)upa;      // expected-error {{cannot use a variable with an unchecked type}}

  unchecked {
  int *p;
  ptr<int> q = 0;
  array_ptr<int> r : count(5) = 0;
  checked {
  p = _Dynamic_bounds_cast<int *>(q);           // expected-error {{cannot use a variable with an unchecked type}} \
                                                // expected-error {{invalid casting to unchecked pointer type}} \

  p = _Dynamic_bounds_cast<int *>(r);           // expected-error {{cannot use a variable with an unchecked type}} \
                                                // expected-error {{invalid casting to unchecked pointer type}} \

  q = _Assume_bounds_cast<ptr<int>>(r);             // expected-error {{_Assume_bounds_cast not allowed in a checked scope or function}}
  r = _Assume_bounds_cast<array_ptr<int>>(r,4);     // expected-error {{_Assume_bounds_cast not allowed in a checked scope or function}}
  r = _Assume_bounds_cast<array_ptr<int>>(r,r,r+1); // expected-error {{_Assume_bounds_cast not allowed in a checked scope or function}}

  vpa = _Dynamic_bounds_cast<ptr<int(int,...)>>(r);   // expected-error {{invalid casting to type having variable arguments}}
  vpa = _Dynamic_bounds_cast<ptr<int(int,ptr<int(int, ...)>)>>(r);  // expected-error {{invalid casting to type having variable arguments}}
  vpa = _Dynamic_bounds_cast<ptr<int*(int,int)>>(r);  // expected-error {{invalid casting to unchecked pointer type}}
  vpa = _Dynamic_bounds_cast<ptr<int(int,ptr<int(int, ...)>, ...)>>(r);  // expected-error {{invalid casting to type having variable arguments}}

  vpa = _Assume_bounds_cast<ptr<int(int,...)>>(r);   // expected-error {{_Assume_bounds_cast not allowed in a checked scope or function}}
  vpa = _Assume_bounds_cast<ptr<int(int,ptr<int(int, ...)>)>>(r);  // expected-error {{_Assume_bounds_cast not allowed in a checked scope or function}}
  vpa = _Assume_bounds_cast<ptr<int*(int,int)>>(r);  // expected-error {{_Assume_bounds_cast not allowed in a checked scope or function}}
  vpa = _Assume_bounds_cast<ptr<int(int,ptr<int(int, ...)>, ...)>>(r);  // expected-error {{_Assume_bounds_cast not allowed in a checked scope or function}}
  }
  }
}

// Test for constructed type
// In checked block, checked pointer to unchecked pointer is not checked type
// To check if a declaration is valid in checked block,
// it SHOULD check checked pointer type recursively.
checked void check_checked_constructed_type(void) {
  ptr<int*> a = 0;                                  // expected-error {{variable cannot have an unchecked pointer type}}
  array_ptr<ptr<char*>> b : count(5) = 0;           // expected-error {{variable cannot have an unchecked pointer type}}
  array_ptr<ptr<ptr<ptr<int*>>>> c : count(10) = 0; // expected-error {{variable cannot have an unchecked pointer type}}
  ptr<ptr<ptr<int*>>> d = 0;                        // expected-error {{variable cannot have an unchecked pointer type}}
  int sum = 0;

  sum += **a;
  sum += ***b;
  sum += *****c;
  sum += ****d;

  ptr<int[10]> pa = 0;              // expected-error {{variable cannot have an unchecked array type}}
  ptr<int checked[10]> pb = 0;
  ptr<ptr<int[5]>> pc = 0;          // expected-error {{variable cannot have an unchecked array type}}
  array_ptr<ptr<int[5]>> pd = 0;    // expected-error {{variable cannot have an unchecked array type}}
  ptr<ptr<int checked[5]>> pe = 0;
  ptr<ptr<ptr<int[]>>> pf = 0;      // expected-error {{variable cannot have an unchecked array type}}
  ptr<ptr<ptr<int checked[]>>> pg = 0;
}

int g(ptr<int *> p) {
  return **p;
}

checked int f(ptr<int *> p) { // expected-error {{parameter cannot have an unchecked pointer type}}
  int r = g(p);
  return r;
}

int ff(ptr<int *> p) checked {
  int r = g(p); // expected-error {{cannot use a parameter with an unchecked type}}
  return r;
}

// Test for constructed type having variable arguments
// In checked block, checked pointer having variable arguments is not allowed
void check_checked_constructed_type_variadic(void) checked {
  ptr<int> pa = 0;
  array_ptr<int> apa;

  ptr<void(int,int)> a = 0;
  ptr<int(int,int)> b = 0;
  ptr<int(int, ptr<int>, ...)> c = 0;           // expected-error {{variable cannot have variable arguments}}
  ptr<int(int, ptr<int(int, ...)>)> d = 0;      // expected-error {{variable cannot have variable arguments}}
  ptr<int(int, ptr<int(int,int)>, ...)> e = 0;  // expected-error {{variable cannot have variable arguments}}
  ptr<int(int, ptr<int(int,...)>, ...)> f = 0;  // expected-error {{variable cannot have variable arguments}}
  int (*g)(int, ...);                           // expected-error {{variable cannot have an unchecked}}
  ptr<int*(int, int)> h = 0;                    // expected-error {{variable cannot have an unchecked}}

  unchecked {
    ptr<int(int,...)> var_b = 0;
    ptr<int(int,ptr<int>,...)> var_c = 0;
    ptr<int(int,ptr<int(int,...)>)> var_d = 0;
    ptr<int(int,ptr<int(int,ptr<int>,...)>, ...)> var_e = 0;
    ptr<int(int,ptr<int(int,ptr<int(int,...)>)>,ptr<int(int,ptr<int(int,ptr<int>,...)>, ...)>,...)> var_f = 0;
    int (*g)(int, ...);
    ptr<int*(int, int)> h = 0;
    (*var_c)(2, pa, pa);
    (*var_d)(3, var_b);
    (*var_e)(4, var_c, var_d);
    (*var_f)(6, var_d, var_e, a, b, g, h);
    checked {
    (*var_c)(2, pa, pa);  // expected-error {{cannot use a variable having variable arguments}}
    (*var_d)(3, var_b);   // expected-error 2 {{cannot use a variable having variable arguments}}
    (*var_e)(4, var_c, var_d);  // expected-error 3 {{cannot use a variable having variable arguments}}
    (*var_f)(6, var_d, var_e, a, b, g, h);// expected-error 3 {{cannot use a variable having variable arguments}} \
                                          // expected-error 2 {{cannot use a variable with an unchecked type}}
    }
  }
}

checked void checked_check_variadic1(int (*fptr)(int, ptr<int>, array_ptr<int>, ...)) { // expected-error {{parameter cannot have an unchecked pointer type}}
  ptr<int> a = 0;
  array_ptr<int> b;
  (*fptr)(5, a, b, a, b, a);
}

void checked_check_variadic2(int (*fptr)(int, ptr<int>, array_ptr<int>, ...)) checked {
  ptr<int> a = 0;
  array_ptr<int> b;
  (*fptr)(5, a, b, a, b, a);  // expected-error {{cannot use a parameter with an unchecked type}}
}

checked void checked_check_variadic3(ptr<int(int, ptr<int>, array_ptr<int>, ...)> fptr) { // expected-error {{parameter cannot have variable arguments in a checked scope}}
  ptr<int> a = 0;
  array_ptr<int> b;
  (*fptr)(5, a, b, a, b, a);
}

void checked_check_variadic4(ptr<int(int, ptr<int>, array_ptr<int>, ...)> fptr) checked {
  ptr<int> a = 0;
  array_ptr<int> b;
  (*fptr)(5, a, b, a, b, a);  // expected-error {{cannot use a parameter having variable arguments}}
}

checked void checked_check_variadic5(int cnt, ...) {  // expected-error {{variable arguments function cannot be made}}
}

void checked_check_variadic6(int cnt, ...) {
}


// Test for function pointer in checked block
// Change type produced by address-of operator(&) in checked block.
// By default, address-of (&) operator produces array_ptr<T>
// For function type, it produces ptr<T> not array_ptr<T>
// Also, FunctionToPointerDecay implicit casting works as same

int id_int(int x) { return 0; }
int id_intp(int *x) { return 0; }
ptr<int> id_checked_intp(ptr<int> x) { return 0; }

extern void test_function_pointer(void) checked {
  // address-of (&) operator produces ptr<T> for function type
  ptr<int(int)> fn0 = &id_int;
  ptr<int(int)> fn1 = id_int;

  int (*fn2)(int) = &id_int;   // expected-error {{variable cannot have an unchecked pointer type}}
  int (*fn3)(int) = id_int;  // expected-error {{variable cannot have an unchecked pointer type}}

  ptr<int(int*)> fn4 = &id_intp;   // expected-error {{variable cannot have an unchecked pointer type}}
  ptr<int(int*)> fn5 = id_intp;  // expected-error {{variable cannot have an unchecked pointer type}}

  ptr<ptr<int>(ptr<int>)> fn6 = &id_intp;  // expected-error {{incompatible type}}
  ptr<ptr<int>(ptr<int>)> fn7 = id_intp; // expected-error {{incompatible type}}

  ptr<ptr<int>(ptr<int>)> fn8 = &id_checked_intp;
  ptr<ptr<int>(ptr<int>)> fn9 = id_checked_intp;

  int val0;
  // address-of (&) operator produces array_ptr<T> except for function type
  ptr<int> pa = &val0;
  array_ptr<int> apa = &val0;
}
