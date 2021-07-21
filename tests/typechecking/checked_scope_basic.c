// Feature tests of typechecking of uses of checked scopes.  The file
// is parameterized by the BOUND_ONLY #define to allow it to test
// regular checked scopes and checked scopes with modifiers that change
// the checking done within the scope.
//
// BOUNDS_ONLY=0 tests checked scopes with no modifiers.
// BOUNDS_ONLY=1 t tests checked scopes with the _Bounds_only modifier.
//
// In the tests, we intentionally intermix regular checked scopes and
// checked scopes with modifiers.
//
// The following lines are for the LLVM test harness:
//
// RUN: %clang_cc1 -Wno-unused-value -Wno-pointer-bool-conversion -verify -verify-ignore-unexpected=note -DBOUNDS_ONLY=0 %s
//

#include <stdchecked.h>

#if BOUNDS_ONLY==0
#define SCOPE_KIND _Checked
#else
#define SCOPE_KIND _Checked _Bounds_only
#endif

//
// Test types that are allowed for locals in checked scopes.
//

void checked_local_types(void) SCOPE_KIND {
  //
  // Check pointer and array types that use only scalar types.
  //

  int *t1 = 0; // expected-error {{local variable in a checked scope must have a checked type}}
  ptr<int> t2 = 0;
  array_ptr<int> t3 = 0;
  int t4[5];     // expected-error {{local variable in a checked scope must have a checked type}}
  int t5 checked[5];

  //
  // Check pointer and array types that use other constructed types (1 level deep)
  //

  //
  // Check pointers to other types
  // 

  // Unchecked pointers to other pointer types.
  int **t10 = 0;           // expected-error {{local variable in a checked scope must have a checked type}}
  ptr<int> *t11 = 0;       // expected-error {{local variable in a checked scope must have a checked type}}
  array_ptr<int> *t12 = 0; // expected-error {{local variable in a checked scope must have a checked type}}

  // Unchecked pointers to array types
  int(*t13)[10] = 0;          // expected-error {{local variable in a checked scope must have a checked type}}
  int(*t14)checked[10] = 0;  // expected-error {{local variable in a checked scope must have a checked type}}

  // Unchecked pointers to function types
  int(*t15)(int a, int b) = 0;  // expected-error {{local variable in a checked scope must have a checked type}}

  // Checked ptrs to other pointer types
  ptr<int *> t20 = 0;  // expected-error {{local variable in a checked \
scope must have a pointer, array or function type that uses only checked types or parameter/return types with bounds-safe interfaces}}
  ptr<ptr<int>> t21 = 0;
  ptr<array_ptr<int>> t22 = 0;

  // Checked ptrs to array types
  ptr<int[10]> t23 = 0;   // expected-error {{local variable in a checked \
scope must have a pointer, array or function type that uses only checked types or parameter/return types with bounds-safe interfaces}}
  ptr<int checked[10]> t24 = 0;

  // Checked ptrs to function types.
  ptr<int(int a, int b)> t25 = 0;

  // Checked array_ptrs to other pointer types
  array_ptr<int *> t30 = 0;  // expected-error {{local variable in a checked \
scope must have a pointer, array or function type that uses only checked types or parameter/return types with bounds-safe interfaces}}
  array_ptr<ptr<int>> t31 = 0;
  array_ptr<array_ptr<int>> t32 = 0;

  // Checked array_ptrs to array types
  array_ptr<int[10]> t33 = 0;   // expected-error {{local variable in a checked \
scope must have a pointer, array or function type that uses only checked types or parameter/return types with bounds-safe interfaces}}
  array_ptr<int checked[10]> t34 = 0;

  // Checked array_ptrs to function types are not allowed, so don't check them.

  //
  // Check arrays of pointers, including multidimensional arrays.
  //

  int *t40[10];      // expected-error {{local variable in a checked scope must have a checked type}}
  ptr<int> t41[10];  // expected-error {{local variable in a checked scope must have a checked type}}
  array_ptr<int> t42[10]; // expected-error {{local variable in a checked scope must have a checked type}}

  int *t43 checked[10];  // expected-error {{local variable in a checked \
scope must have a pointer, array or function type that uses only checked types or parameter/return types with bounds-safe interfaces}}
  ptr<int> t44 checked[10] = {0};
  array_ptr<int> t45 checked[10] = {0};

  int *t46 checked[10][15];  // expected-error {{local variable in a checked \
scope must have a pointer, array or function type that uses only checked types or parameter/return types with bounds-safe interfaces}}
  ptr<int> *t47 checked[10][15];  // expected-error {{local variable in a checked \
scope must have a pointer, array or function type that uses only checked types or parameter/return types with bounds-safe interfaces}}
  array_ptr<int> t48 checked[3][2] = {0};

  //
  // Checked pointers to function types that use constructed types.
  //

  // Functions with different kinds of pointer return types.
  ptr<int *(int, int)> t60 =  0;  // expected-error {{local variable in a checked \
scope must have a pointer, array or function type that uses only checked types or parameter/return types with bounds-safe interfaces}}
  ptr<ptr<int>(int, int)> t61 = 0;
  ptr<array_ptr<int>(int, int)> t62 = 0;

  // Function with different kinds of pointer arguments.
  ptr<void(int *)> t63 = 0; // expected-error {{local variable in a checked \
scope must have a pointer, array or function type that uses only checked types or parameter/return types with bounds-safe interfaces}}
  ptr<void(ptr<int>)> t64 = 0;
  ptr<void(array_ptr<int>)> t65 = 0;

  ptr<int (int arg[10])> t66 = 0; // expected-error {{local variable in a checked \
scope must have a pointer, array or function type that uses only checked types or parameter/return types with bounds-safe interfaces}}
  ptr<int(int arg[])> t67 = 0; // expected-error {{local variable in a checked \
scope must have a pointer, array or function type that uses only checked types or parameter/return types with bounds-safe interfaces}}
  ptr<int (int arg checked[10])> t68 = 0;
  ptr<int (int arg checked[])> t69 = 0;
}
// Test for checked function specifier.
// - check if function declaration (return/param) is checked.
// - check if function body (compound statement) is checked.
// - check if function with no prototype is checked.
// - check that if a declaration is declared in a check scope with an unchecked type,
//    an error message is produced for the declaration.  
// - check that if a declaration is declared in an unchecked scope with an unchecked type
//   and it is used in a checked, scope, there is an error message at uses of the declared
//   entity.

//
// Test types that are allowed for parameters and returns in checked scopes.
//

SCOPE_KIND int func4(int p[]) {  // expected-error {{parameter in a checked scope must have a checked type or a bounds-safe interface}}
  return 0;
}

SCOPE_KIND int func5(ptr<int> p, int *q) { // expected-error {{parameter in a checked scope must have a checked type or a bounds-safe interface}}
  return 0;
}

SCOPE_KIND int* func6(int a[][5], int b[][5]) : itype(ptr<int>) SCOPE_KIND { // expected-error 2 {{parameter in a checked scope must have a checked type or a bounds-safe interface}}
}

SCOPE_KIND int* func7(void) : itype(ptr<int>) SCOPE_KIND {
 return 0;
}

SCOPE_KIND int* func8(int len) {       // expected-error {{return in a checked scope must have a checked type or a bounds-safe interface}}
  return 0;
}

SCOPE_KIND int func9(ptr<int> p) {
  int e[5][5];    // expected-error {{local variable in a checked scope must have a checked type}}
  return 0;
}

// Test for checked function specifier & checked block.
int* func11(array_ptr<int> x, int len) SCOPE_KIND {
  int *upa;   // expected-error {{local variable in a checked scope must have a checked type}}
  return upa;
}

array_ptr<int> func12(array_ptr<int> x, int len) : bounds(x,x+len) SCOPE_KIND {
  return 0;
}

array_ptr<int> func13(int *x, int *y) SCOPE_KIND {
  *y = 0;       // expected-error {{parameter used in a checked scope must have a checked type or a bounds-safe interface}}
  int z = *y;   // expected-error {{parameter used in a checked scope must have a checked type or a bounds-safe interface}}
  return 0;
}

void func14(int checked[5][5], int b checked[5][5]);

// Declaring checked/unchecked multi-dimensional arrays in a checked scope and attempting to pass them
// as parameter.
void func15(void) {
  int e _Checked[5][5];
  int f[5][5];
  SCOPE_KIND {
    int g[5][5];                 // expected-error {{local variable in a checked scope must have a checked type}}
    func14(e, e);
    func14(f, f); // expected-error 2 {{local variable used in a checked scope must have a checked type}}
    func14(g, g);
  }
  return;
}

SCOPE_KIND int func16() {               // expected-error {{function without a prototype cannot be used or declared in a checked scope}}
  return 0;
}

// Test that functions declared with old-style K&R parameter lists cannot
// be declared or used in checked scopes, unless they are preceded by
// prototype declarations.
//
// First, test definitions of functions with K&R parameter lists.
// According to the C11 specification, a function defined with an old-style
// K&R parameter lists does not create a prototype for the function.  This
// means that there may be no typechecking of actual arguments against
// formal arguments for subsequent uses of the function.  That could lead
// to uncaught errors at runtime.  We only allow such definitions
// in checked scopes when they are preceded by prototype declarations.

#pragma CHECKED_SCOPE ON
// First test definitions without preceding prototypes.
int KNR_func1(a, b, c) // expected-error {{function without a prototype cannot be used or declared in a checked scope}}
  int a, b, c;
{
  return 1;
}

int KNR_func2(a, b)    // expected-error {{function without a prototype cannot be used or declared in a checked scope}}
  ptr<int> a;
  int b;
 {
  return 1;
}

int KNR_func3(a, b)    // expected-error {{function without a prototype cannot be used or declared in a checked scope}}
  ptr<char> a;
  ptr<int> b;
{
  return 1;
}

// Test the case where a function with a K&R style parameter list is preceded
// by a prototype declaration.  In this case, the function definition is
// compatible with the prototype and all subsequent uses will use the prototype,
// so it is allowed.
//
// However, the parameter types cannot be checked pointers because
// no-prototype function types are not compatible with prototype function
// types with checked pointers.
int KNR_with_proto(int, int b);
int KNR_with_proto(a, b)
int a;
int b;
{
  return 1;
}
#pragma CHECKED_SCOPE OFF

// Now test uses within checked scopes.
// First we have to declared some K&R style functions.

int KNR_func4(a, b, c)
int a, b, c;
{
  return 1;
}

int KNR_func5(a, b)
ptr<int> a;
int b;
{
  return 1;
}

int KNR_func6(a, b)
ptr<char> a;
ptr<int> b;
{
  return 1;
}

#pragma CHECKED_SCOPE ON
void KNR_test(void) {
  ptr<int> px = 0;
  ptr<char> py = 0;
  int a,b,c;
  KNR_func4(a,b,c); // expected-error {{function without a prototype cannot be used or declared in a checked scope}}
  KNR_func5(px,a);  // expected-error {{function without a prototype cannot be used or declared in a checked scope}}
  KNR_func6(py,px); // expected-error {{function without a prototype cannot be used or declared in a checked scope}}
}
#pragma CHECKED_SCOPE OFF

// Test for checked block.
// - check if compound statments are checked.
// - check if checked property is inherited from parent scope.
int func20(void) SCOPE_KIND {
  int a = 5;
  ptr<int> pa = &a;
  int b[5][5]; // expected-error {{local variable in a checked scope must have a checked type}}
  for (int i = 0; i < 5; i++) {
    for (int j = 0; j < 5; j++) {
      b[i][j] = -1;
    }
  }
}

int func21(void) {
  SCOPE_KIND {
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
  SCOPE_KIND {
    int b[5][5];    // expected-error {{local variable in a checked scope must have a checked type}}
  checked {
    for (int i = 0; i < 5; i++) checked {
      for (int j = 0; j < 5; j++) checked {
        b[i][j] = *upa; // expected-error {{local variable used in a checked scope must have a checked type}}
      }
    }
  }
  }
}

int func23(void) checked {
  int a = 5;
  {
    int *upa = &a;  // expected-error {{local variable in a checked scope must have a checked type}}
  {
    int b[5][5];    // expected-error {{local variable in a checked scope must have a checked type}}
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
    int *upa = &a;  // expected-error {{local variable in a checked scope must have a checked type}}
    int b[5][5];    // expected-error {{local variable in a checked scope must have a checked type}}
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
  for (int i = 0; i < 5; i++) SCOPE_KIND {
    for (int j = 0; j < 5; j++) {
      b[i][j] = -1; // expected-error {{local variable used in a checked scope must have a checked type}}
    }
  }
}

int func26(ptr<int> p, int *q) {
  int a = 5;
  int *upa;
  SCOPE_KIND {
    upa = &a;         // expected-error {{local variable used in a checked scope must have a checked type}}
  }
  ptr<int> pb = p;
  return *pb;
}

checked void func27(void) {
  int *upa;   // expected-error {{local variable in a checked scope must have a checked type}}
  array_ptr<int> pb;
  ptr<int> pc = 0;
  upa = pb;
  upa = pc;
}

void func28(void) {
  int *upa;
  array_ptr<int> pb;
  ptr<int> pc = 0;
}

int func29(void) {
  int a = 5;
  SCOPE_KIND {
    {
      struct s0 {
        int *a;       // expected-error {{member in a checked scope must have a checked type}}
        char *b;      // expected-error {{member in a checked scope must have a checked type}}
        ptr<int> pc;
        array_ptr<int> pd : count(len);
        int len;
        short e[10];  // expected-error {{member in a checked scope must have a checked type}}
        int *f : itype(ptr<int>);
        char *g : itype(array_ptr<char>);
      } a = {0};
    }
  }
}

void func30(void) {
  int a = 5;
  int len = 10;
  array_ptr<int> pa : count(len) = 0;
  /// checked is a function declaration specifier unless followed by '{' or '['
  checked int len2;   // expected-error {{'_Checked' can only appear on functions}}
  checked [5][5];     // expected-error {{expected identifier}}
}

// Test for unchecked scope.
// - unchecked scope clears checked property & prevents inheritance of checked property.
// - unchecked scope keyword clears checked function specifier.
SCOPE_KIND int * func40(int *p, int *q) unchecked {// expected-error {{return in a checked scope must have a checked type or a bounds-safe interface}} \
                                                // expected-error 2 {{parameter in a checked scope must have a checked type or a bounds-safe interface}}
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

SCOPE_KIND int * func41(int *p, ptr<int> q, array_ptr<int> r, array_ptr<int> s : count(2)) unchecked { // expected-error {{return in a checked scope must have a checked type or a bounds-safe interface}} \
                                                                                                    // expected-error {{parameter in a checked scope must have a checked type or a bounds-safe interface}}
  int a = 5;
  checked {
    *p = 1;
    *q = 2;
    *r = 3; // expected-error {{expression has unknown bounds}}
  *s = 4;
  unchecked {
    ptr<int> pa = &a;
    int b checked[5][5];
    for (int i = 0; i < 5; i++) {
      for (int j = 0; j < 5; j++) {
        b[i][j] += *q + *r; // expected-error {{expression has unknown bounds}}
        b[i][j] += *p + *q + *r + *s;
      }
    }
  }
  }
  return 0;
}

SCOPE_KIND int func42(ptr<int> p, int q[], int r[5]) unchecked {  // expected-error 2 {{parameter in a checked scope must have a checked type or a bounds-safe interface}}
  int a = 5;
  SCOPE_KIND {
    int *upa = &a;  // expected-error {{local variable in a checked scope must have a checked type}}
  unchecked {
    int b[5][5];
    checked {
      for (int i = 0; i < 5; i++) {
        for (int j = 0; j < 5; j++) {
          b[i][j] = -1;   // expected-error {{local variable used in a checked scope must have a checked type}}
        }
      }
    }
  }
  }
  return 0;
}

int * func43(void) unchecked {
  int a = 5;
  {
    {
      int *upa = &a;
      unchecked {
        int b[5][5];
        SCOPE_KIND {
          int c checked[5][5];
          for (int i = 0; i < 5; i++) {
            for (int j = 0; j < 5; j++) {
              b[i][j] += c[i][j];   // expected-error {{local variable used in a checked scope must have a checked type}}
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

SCOPE_KIND int * func44(void) unchecked {  // expected-error {{return in a checked scope must have a checked type or a bounds-safe interface}}
  int a = 5;
  checked {
    int *upa = &a;  // expected-error {{local variable in a checked scope must have a checked type}}
  unchecked {
    int b[5][5];
    SCOPE_KIND {
      int c checked[5][5];
      int d[5][5];  // expected-error {{local variable in a checked scope must have a checked type}}
    for (int i = 0; i < 5; i++) {
      for (int j = 0; j < 5; j++) {
        b[i][j] += c[i][j] - d[i][j];   // expected-error {{local variable used in a checked scope must have a checked type}}
      }
    }
    }
    return 0;
  }
  }
}

SCOPE_KIND int func45(int *p, int *q) unchecked { // expected-error 2 {{parameter in a checked scope must have a checked type or a bounds-safe interface}}
  int sum = 0;
  int a = 5;
  int *upa = &a;
  int b[5][5];
  for (int i = 0; i < 5; i++) SCOPE_KIND {
    sum += b[i][0] + *upa; // expected-error 2 {{local variable used in a checked scope must have a checked type}}
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
  SCOPE_KIND {
    upa = &a; // expected-error {{local variable used in a checked scope must have a checked type}}
    pc = &a;
  unchecked {
    upa = &a;
    pc = &a;
  }
    upa = &a; // expected-error {{local variable used in a checked scope must have a checked type}}
    pc = &a;
  }
  ptr<int> pb = p;
  return *pb;
}

SCOPE_KIND array_ptr<int> func47(void) {
  int *upa;   // expected-error {{local variable in a checked scope must have a checked type}}
  unchecked {
  int *upb;
  array_ptr<int> pb;
  ptr<int> pc = 0;
  upa = pb;
  upa = pc;
  return upb; // array_ptr<int> = int *, OK
  }
}

int func49(void) {
  int a = 5;
  SCOPE_KIND {
    {
      struct s0 {
        int *a;       // expected-error {{member in a checked scope must have a checked type}}
        char *b;      // expected-error {{member in a checked scope must have a checked type}}
        ptr<int> pc;
        array_ptr<int> pd : count(len);
        int len;
        short e[10];  // expected-error {{member in a checked scope must have a checked type}}
        char f[10];   // expected-error {{member in a checked scope must have a checked type}}
        int *g : itype(ptr<int>);
        char *h : itype(array_ptr<char>);
      } a = {0};
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
      } b; // expected-error {{containing a checked pointer must have an initializer}}

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
  SCOPE_KIND {
    *p = 1; // expected-error {{parameter used in a checked scope must have a checked type or a bounds-safe interface}}
    *q = 2;
    *s = 4;
    unchecked {
      ptr<int> pa = &a;
      int b checked[5][5];
      for (int i = 0; i < 5; i++) {
        for (int j = 0; j < 5; j++) {
          b[i][j] = *p + *q +  *s;
        }
      }
    }
  }
  return 0;
}

unchecked int func52(ptr<int> p, int q[], int r[5]) unchecked {
  int a = 5;
  checked {
    int *upa = &a;  // expected-error {{local variable in a checked scope must have a checked type}}
  unchecked {
    int b[5][5];
    SCOPE_KIND {
      for (int i = 0; i < 5; i++) {
        for (int j = 0; j < 5; j++) {
          b[i][j] = -1;   // expected-error {{local variable used in a checked scope must have a checked type}}
        }
      }
    }
  }
  }
  return 0;
}

unchecked int * func53(void) unchecked {
  int a = 5;
  {
    {
      int *upa = &a;
      unchecked {
        int b[5][5];
        SCOPE_KIND {
          int c checked[5][5];
          for (int i = 0; i < 5; i++) {
            for (int j = 0; j < 5; j++) {
              b[i][j] += c[i][j];   // expected-error {{local variable used in a checked scope must have a checked type}}
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
  SCOPE_KIND {
    int *upa = &a;  // expected-error {{local variable in a checked scope must have a checked type}}
  unchecked {
    int b[5][5];
    SCOPE_KIND {
      int c checked[5][5];
      int d[5][5];  // expected-error {{local variable in a checked scope must have a checked type}}
    for (int i = 0; i < 5; i++) {
      for (int j = 0; j < 5; j++) {
        b[i][j] += c[i][j] - d[i][j];   // expected-error {{local variable used in a checked scope must have a checked type}}
      }
    }
    }
    return 0;
  }
  }
}

unchecked int func55(int *p, int *q) unchecked {
  int sum = 0;
  int a = 5;
  int *upa = &a;
  int b[5][5];
  for (int i = 0; i < 5; i++) SCOPE_KIND {
    sum += b[i][0] + *upa; // expected-error 2 {{local variable used in a checked scope must have a checked type}}
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
  SCOPE_KIND {
    upa = &a; // expected-error {{local variable used in a checked scope must have a checked type}}
    pc = &a;
  unchecked {
    upa = &a;
    pc = &a;
  }
    upa = &a; // expected-error {{local variable used in a checked scope must have a checked type}}
    pc = &a;
  }
  ptr<int> pb = p;
  return *pb;
}

unchecked array_ptr<int> func57(void) {
  int *upa;
  unchecked {
  int *upb = 0;
  array_ptr<int> pb;
  ptr<int> pc = 0;
  return upb; // array_ptr<int> = int *, OK
  }
}

unchecked int * func58(void) unchecked {
  int *upa;
  array_ptr<int> pb;
  ptr<int> pc = 0;
  SCOPE_KIND {
    unchecked {
    return upa;
    }
  }
}

unchecked int func59(void) {
  int a = 5;
  SCOPE_KIND {
    {
      struct s0 {
        int *a;       // expected-error {{member in a checked scope must have a checked type}}
        char *b;      // expected-error {{member in a checked scope must have a checked type}}
        ptr<int> pc;
        array_ptr<int> pd : count(len);
        int len;
        short e[10];  // expected-error {{member in a checked scope must have a checked type}}
        char f[10];   // expected-error {{member in a checked scope must have a checked type}}
      } a = {0};
      struct s1 unchecked {
        int *a;
        char *b;
        ptr<int> pc;
        array_ptr<int> pd : count(len);
        int len;
        short e[10];
        char f[10];
      } b; // expected-error {{containing a checked pointer must have an initializer}}

    }
  }
  return 0;
}

// Test for structure checked block.
struct s0 SCOPE_KIND {
  int *a;     // expected-error {{member in a checked scope must have a checked type}}
  char *b;    // expected-error {{member in a checked scope must have a checked type}}
  ptr<int> pc;
  array_ptr<int> pd : count(len);
  int *e : itype(array_ptr<int>);
  int len;
  struct s1 {
    int *a;   // expected-error {{member in a checked scope must have a checked type}}
    char *b;  // expected-error {{member in a checked scope must have a checked type}}
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

SCOPE_KIND int func60(ptr<struct s0> st0, ptr<struct s1> st1) {
  int sum = *(st0->a) + *(st1->a);
  sum += *(st0->b) + *(st1->b);
  sum += *(st0->pc) + *(st1->pc);
  sum += *(st0->pd) + *(st1->pd);
  sum += *(st0->e) + *(st1->e);   // expected-error {{expression has unknown bounds}}

  struct s2 sta; // expected-error {{containing a checked pointer must have an initializer}}
  ptr<struct s2> pstb = 0;
  sum += *(sta.a) + *(sta.b) + *(sta.pc) + *(sta.pd); // expected-error 2 {{member used in a checked scope must have a checked type or a bounds-safe interface}}
  sum += *(sta.e);
  sum += *(pstb->a) + *(pstb->b); // expected-error 2 {{member used in a checked scope must have a checked type or a bounds-safe interface}}
  sum += *(pstb.a) + *(pstb.b);   // expected-error 2 {{member used in a checked scope must have a checked type or a bounds-safe interface}} expected-error 2 {{member reference type}}
  sum += *(sta->a) + *(sta->b);   // expected-error 2 {{member used in a checked scope must have a checked type or a bounds-safe interface}} expected-error 2 {{member reference type}}

  ptr<struct s2> st2 = 0;
  ptr<struct s2> st3 = 0;
  sum += *(st2->a)  + *(st3->a); // expected-error 2 {{member used in a checked scope must have a checked type or a bounds-safe interface}}
  sum += *(st2->b)  + *(st3->b); // expected-error 2 {{member used in a checked scope must have a checked type or a bounds-safe interface}}
  sum += *(st2->pc) + *(st3->pc);
  sum += *(st2->pd) + *(st3->pd);
  sum += *(st2->e) + *(st2->e);
  sum += *(st2->d.a) + *(st3->d.a); // expected-error 2 {{member used in a checked scope must have a checked type or a bounds-safe interface}}
  sum += *(st2->d.b) + *(st3->d.b); // expected-error 2 {{member used in a checked scope must have a checked type or a bounds-safe interface}}
  sum += *(st2->d.e) + *(st3->d.e); // expected-error 2 {{expression has unknown bounds}}
  return sum;
}



// Change type produced by address-of operator(&) in checked block.
// ex) checked { .... int a; ptr<int> pb = &a; }
void test_addrof_checked_scope(void) SCOPE_KIND {
  int a checked[10];
  array_ptr<int> b;
  int i = 0;

  // In checked scope, address-of operator produces _Array_ptr<T>
  // VisitBinaryOperator - check if LHS has bounds and RHS has bounds
  ptr<int> x = &a[i]; // expected-warning {{cannot prove cast source bounds are wide enough for '_Ptr<int>'}} \
                         ImplicitCastExpr _Ptr (UnaryOperator _Array_Ptr<int>)
  ptr<int> y = &b[0]; // ImplicitCastExpr _Ptr (UnaryOperator _Array_Ptr<int>) \
                      // expected-error {{expression has unknown bounds, cast to ptr<T> expects source to have bounds}}
  ptr<int> z = &i;    // ImplicitCastExpr _Ptr (UnaryOperator _Array_Ptr<int>)

  x = &a[i];  // expected-warning {{cannot prove cast source bounds are wide enough for '_Ptr<int>'}} \
              // ImplicitCastExpr _Ptr (UnaryOperator _Array_ptr<int>)
  y = &b[1];  // ImplicitCastExpr _Ptr (UnaryOperator _Array_ptr<int>) \
              // expected-error {{expression has unknown bounds, cast to ptr<T> expects source to have bounds}}
  z = &i;     // ImplicitCastExpr _Ptr (UnaryOperator _Array_Ptr<int>)

  x = b;      // BinaryOperator (ImplicitCastExpr _Ptr (_Array_ptr)) \
              // expected-error {{expression has unknown bounds, cast to ptr<T> expects source to have bounds}}

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

  int aa[5];  // expected-error {{local variable in a checked scope must have a checked type}}
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
                      // expected-error {{expression has unknown bounds, cast to ptr<T> expects source to have bounds}}
  ptr<int> z = &i;    // ImplicitCastExpr _Ptr<int>(UnaryOperator int * prefix &)

  // implicit cast for _Ptr<T> requires source bounds
  x = &a[i];  // BinaryOperator(ImplicitCastExpr _Ptr<int>(UnaryOperator int * prefix &))
  y = &b[0];  // BinaryOperator(ImplicitCastExpr _Ptr<int>(UnaryOperator int * prefix &)) \
              // expected-error {{expression has unknown bounds, cast to ptr<T> expects source to have bounds}}
  z = &i;     // BinaryOperator(ImplicitCastExpr _Ptr<int>(UnaryOperator int * prefix &))

  x = b;      // BinaryOperator(ImplicitCastExpr()) \
              // expected-error {{expression has unknown bounds, cast to ptr<T> expects source to have bounds}}

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

extern void check_indirection_unchecked(int p[10], const int const_p[10], int y) SCOPE_KIND {
  *p = y;       // expected-error {{parameter used in a checked scope must have a checked type or a bounds-safe interface}}
  y = *p;       // expected-error {{parameter used in a checked scope must have a checked type or a bounds-safe interface}}
  *const_p = y; // expected-error {{parameter used in a checked scope must have a checked type or a bounds-safe interface}}
  y = *const_p; // expected-error {{parameter used in a checked scope must have a checked type or a bounds-safe interface}}
}

extern void check_subscript_unchecked(int p[10], int y) SCOPE_KIND {
  p[0] = y;     // expected-error {{parameter used in a checked scope must have a checked type or a bounds-safe interface}}
  y = p[0];     // expected-error {{parameter used in a checked scope must have a checked type or a bounds-safe interface}}
  0[p] = y;     // expected-error {{parameter used in a checked scope must have a checked type or a bounds-safe interface}}
  y = 0[p];     // expected-error {{parameter used in a checked scope must have a checked type or a bounds-safe interface}}
}

extern void check_subscript_checked(int p checked[10], const int p_const[10], int y) SCOPE_KIND {
  p[0] = y;  // OK
  y = p[0];  // OK
  0[p] = y;  // OK
  y = 0[p];  // OK
  p_const[0] = y;  // expected-error {{parameter used in a checked scope must have a checked type or a bounds-safe interface}}
  y = p_const[0];  // expected-error {{parameter used in a checked scope must have a checked type or a bounds-safe interface}}
  0[p_const] = y;  // expected-error {{parameter used in a checked scope must have a checked type or a bounds-safe interface}}
  y = 0[p_const];  // expected-error {{parameter used in a checked scope must have a checked type or a bounds-safe interface}}
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

  SCOPE_KIND {
  // Single-dimensional array type conversions to pointer types.
  t1 = p; // expected-error {{parameter used in a checked scope must have a checked type or a bounds-safe interface}} \
          // expected-error {{local variable used in a checked scope must have a checked type}}
          // T *  = T[constant] OK
  t2 = q; // expected-error {{parameter used in a checked scope must have a checked type or a bounds-safe interface}} \
          // expected-error {{local variable used in a checked scope must have a checked type}}
          // T *  = T[] OK
  t3 = t; // expected-error 2 {{local variable used in a checked scope must have a checked type}} \
          // T *  = T[constant] OK;
  t4 = r; // expected-error {{local variable used in a checked scope must have a checked type}} \
          // Assignment of checked pointer to unchecked pointer not allowed
  t5 = s; // expected-error {{local variable used in a checked scope must have a checked type}}
  t6 = u; // expected-error {{local variable used in a checked scope must have a checked type}}

  // Various forms of array_ptr<T> = T[]. Note that the rhs does not need to have known bounds
  // because the lhs pointers have unknown bounds (and cannot be dereferenced).
  //
  // Note if there need to be known bounds, the bounds of p and q are unknown
  // because C does not guarantee that array sizes match for parameter passing
  t7 = p;         // expected-error {{parameter used in a checked scope must have a checked type or a bounds-safe interface}}
  t8 = q;         // expected-error {{parameter used in a checked scope must have a checked type or a bounds-safe interface}}
  t9 = r;
  t10 = s;
  t11 = t;        // expected-error {{local variable used in a checked scope must have a checked type}}
  t12 = u;
  t13 = s2d[0];
  t14 = t2d[0];   // expected-error {{local variable used in a checked scope must have a checked type}}
  t15 = u2d[0];


  // Multi-dimensional array type conversions to pointer types.
  t16 = s2d[0]; // expected-error {{local variable used in a checked scope must have a checked type}}
  t17 = t2d[0]; // expected-error 2 {{local variable used in a checked scope must have a checked type}}
  t18 = u2d[0]; // expected-error {{local variable used in a checked scope must have a checked type}}
  t19 = s2d;    // expected-error {{local variable used in a checked scope must have a checked type}}
  t20 = t2d;    // expected-error 2 {{local variable used in a checked scope must have a checked type}}
  t21 = u2d;    // expected-error {{local variable used in a checked scope must have a checked type}}

  // Constructed type that contains unchecked pointer/array types in a checked scope
  // Checked pointer type to unchecked pointer/array type is not allowed
  t22 = s2d;  // expected-error {{must have a pointer, array or function type that uses only checked types or parameter/return types with bounds-safe interfaces}} array_ptr<unchecked array type>, not OK
  t23 = t2d;  // expected-error {{must have a pointer, array or function type that uses only checked types or parameter/return types with bounds-safe interfaces}} \
                 expected-error {{local variable used in a checked scope must have a checked type}} \
                 array_ptr<unchecked array type>, not OK
  t24 = u2d;  // expected-error {{local variable used in a checked scope must have a pointer, array or function type that uses only checked types or parameter/return types with bounds-safe interfaces}} array_ptr<unchecked array type>, not OK
  t25 = s2d;
  t26 = t2d;  // expected-error {{local variable used in a checked scope must have a checked type}}
  t27 = u2d;

  // Assignments to array-typed parameters are allowed.  The outermost array modifier
  // is converted to a pointer type.
  p = q;  // expected-error 2 {{parameter used in a checked scope must have a checked type or a bounds-safe interface}}
  q = p;  // expected-error 2 {{parameter used in a checked scope must have a checked type or a bounds-safe interface}}
  s = r;
  r = t;  // expected-error {{local variable used in a checked scope must have a checked type}}
  p = r;  // expected-error {{parameter used in a checked scope must have a checked type or a bounds-safe interface}}
          // assignment of checked pointer to unchecked pointer not allowed

  // Assignments to array-typed local and global variables are not allowed
  t = p;  // expected-error {{parameter used in a checked scope must have a checked type or a bounds-safe interface}} \
          // expected-error {{local variable used in a checked scope must have a checked type}}
  t = r;  // expected-error {{local variable used in a checked scope must have a checked type}}
  global_arr = p; // expected-error {{variable used in a checked scope must have a checked type}} \
                  // expected-error {{parameter used in a checked scope must have a checked type or a bounds-safe interface}}
  global_arr = r; // expected-error {{variable used in a checked scope must have a checked type}}
  }
}

// Test conditional expressions where arms have different
// kinds of checked and unchecked arrays.
extern void check_condexpr(int val) {
  float r checked[5];
  float s[5];
  float u checked[5];
  int *t1;
  array_ptr<int> t2, t3, t4;

  SCOPE_KIND {
  // omit assignment because type of expression is not valid when there is an error.
  val ? s : r;    // expected-error {{local variable used in a checked scope must have a checked type}}
  val ? r : s;    // expected-error {{local variable used in a checked scope must have a checked type}}
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

checked extern void f5(int(*p)[10], int y) {  // expected-error {{parameter in a checked scope must have a checked type or a bounds-safe interface}}
}

SCOPE_KIND extern void f6(ptr<int[10]> p, int y) { // expected-error {{must have a pointer, array or function type that uses only checked types or parameter/return types with bounds-safe interfaces}} \
                                                // ptr<unchecked array> not OK in checked scope
}

SCOPE_KIND extern void f7(array_ptr<int[10]> p, int y) { // expected-error {{must have a pointer, array or function type that uses only checked types or parameter/return types with bounds-safe interfaces}} \
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

  SCOPE_KIND {
  // checked scope error precedes function call type error/warning
  // f1(int *p, int y)
  f1(x, 0);               // expected-error {{local variable used in a checked scope must have a checked type}}
  f1(x2d, 0);             // expected-error {{local variable used in a checked scope must have a checked type}}

  // f2(int p[10], int y)
  f2(x, 0);               // expected-error {{local variable used in a checked scope must have a checked type}}
  f2(x2d, 0);             // expected-error {{local variable used in a checked scope must have a checked type}}

  // f3(int p checked[10], int y)
  f3(x, 0);               // expected-error {{local variable used in a checked scope must have a checked type}}
  f3(y, 0);
  f3(x2d, 0);             // expected-error {{local variable used in a checked scope must have a checked type}}

  // f4(int **p, int y);
  f4(x, 0);               // expected-error {{local variable used in a checked scope must have a checked type}}
  f4(x2d, 0);             // expected-error {{local variable used in a checked scope must have a checked type}}

  // f5(int (*p)[10], int y);
  f5(x, 0);               // expected-error {{local variable used in a checked scope must have a checked type}}
  f5(x2d, 0);             // expected-error {{local variable used in a checked scope must have a checked type}}

   // f6(ptr<int[10]>, int y);
  f6(x, 0);               // expected-error {{local variable used in a checked scope must have a checked type}}
  f6(x2d, 0);             // expected-error {{local variable used in a checked scope must have a checked type}}

   // f7(array_ptr<int[10]>, int y);
  f7(x, 0);               // expected-error {{local variable used in a checked scope must have a checked type}}
  f7(x2d, 0);             // expected-error {{local variable used in a checked scope must have a checked type}}

  // f8(int (*p) checked[10], int y);
  f8(x, 0);               // expected-error {{local variable used in a checked scope must have a checked type}}
  f8(x2d, 0);             // expected-error {{local variable used in a checked scope must have a checked type}}

  // f9(ptr<int checked[10]> p, int y);
  f8(x, 0);               // expected-error {{local variable used in a checked scope must have a checked type}}
  f8(x2d, 0);             // expected-error {{local variable used in a checked scope must have a checked type}}

  // f10(array_ptr<int checked[10]> p, int y);
  f10(x, 0);              // expected-error {{local variable used in a checked scope must have a checked type}}
  f10(x2d, 0);            // expected-error {{local variable used in a checked scope must have a checked type}}

  // f11(int p[10][10], int y);
  f11(x, 0);              // expected-error {{local variable used in a checked scope must have a checked type}}
  f11(x2d, 0);            // expected-error {{local variable used in a checked scope must have a checked type}}

  // f12(int p checked[10][10], int y);
  f12(x, 0);              // expected-error {{local variable used in a checked scope must have a checked type}}
  f12(x2d, 0);            // expected-error {{local variable used in a checked scope must have a checked type}}
  f12(y2d, 0);            // OK

  // f13(_Bool b, int y);
  f13(x, 0);              // expected-error {{local variable used in a checked scope must have a checked type}}
  f13(y, 0);              // OK
  f13(x2d, 0);            // expected-error {{local variable used in a checked scope must have a checked type}}
  f13(y2d, 0);            // OK

  // spot check calls where an array is the second argument
  g1(0, x);   // expected-error {{local variable used in a checked scope must have a checked type}}
  g2(0, x);   // expected-error {{local variable used in a checked scope must have a checked type}}
  g3(0, x);   // expected-error {{local variable used in a checked scope must have a checked type}}
  g3(0, y);
  }
}

int global[10];
int checked_global checked[10];
int global_arr1[10];

SCOPE_KIND int *h3(void) {   // expected-error {{return in a checked scope must have a checked type or a bounds-safe interface}}
  return global;    // expected-error {{variable used in a checked scope must have a checked type}}
}

SCOPE_KIND ptr<int> h4(void) {
  return global;    // expected-error {{variable used in a checked scope must have a checked type}}
}

SCOPE_KIND array_ptr<int> h5(void) {
  return global;    // expected-error {{variable used in a checked scope must have a checked type}}
}

SCOPE_KIND int *h6(void) {   // expected-error {{return in a checked scope must have a checked type or a bounds-safe interface}}
  return 0;
}

SCOPE_KIND ptr<int> h7(void) {
  return checked_global; // ptr<T> = array_ptr<T> OK
}

SCOPE_KIND array_ptr<int> h8(void) {
  return checked_global;
}

SCOPE_KIND int *h9(int arr[10]) {    // expected-error {{return in a checked scope must have a checked type or a bounds-safe interface}} expected-error {{parameter in a checked scope must have a checked type or a bounds-safe interface}}
  return arr;
}

SCOPE_KIND ptr<int> h10(void) {
  return global_arr1;   // expected-error {{variable used in a checked scope must have a checked type}}
}

SCOPE_KIND array_ptr<int> h11(int arr checked[10]) {
  return arr;
}

SCOPE_KIND int *h12(int arr checked[10]) {   // expected-error {{return in a checked scope must have a checked type or a bounds-safe interface}}
  return 0;
}

SCOPE_KIND ptr<int> h13(int arr checked[10]) {
  return arr;  // ptr<T> = array_ptr<T> OK
}

SCOPE_KIND array_ptr<int> h14(int arr checked[10]) {
  return arr;
}

SCOPE_KIND int *h15(int arr checked[]) {   // expected-error {{return in a checked scope must have a checked type or a bounds-safe interface}}
  return 0;
}

SCOPE_KIND array_ptr<int> h18(int arr checked[]) {
  return arr;
}

// h19 is a function that returns a pointer to a 10-element array of integers.
SCOPE_KIND int(*h19(int arr[10][10]))[10]{   // expected-error {{return in a checked scope must have a checked type or a bounds-safe interface}} expected-error {{parameter in a checked scope must have a checked type or a bounds-safe interface}}
  return arr;
}

int global_arr2[10][10];
SCOPE_KIND ptr<int[10]> h20(void) {  // expected-error {{must have a pointer, array or function type that uses only checked types or parameter/return types with bounds-safe interfaces}}
  return global_arr2; // expected-error {{variable used in a checked scope must have a checked type}}
}

SCOPE_KIND array_ptr<int[10]> h21(int arr[10][10]) { // expected-error {{must have a pointer, array or function type that uses only checked types or parameter/return types with bounds-safe interfaces}} \
                                                  // expected-error {{parameter in a checked scope must have a checked type or a bounds-safe interface}}
  return arr;
}

// h22 is a function that returns a pointer to a 10-element array of integers.
SCOPE_KIND int (*h22(int arr checked[10][10]))[10] { // expected-error {{return in a checked scope must have a checked type or a bounds-safe interface}}
  return 0;
}

SCOPE_KIND ptr<int[10]> h23(int arr checked[10][10]) { // expected-error {{must have a pointer, array or function type that uses only checked types or parameter/return types with bounds-safe interfaces}}
  return 0;
}

SCOPE_KIND array_ptr<int[10]> h24(int arr checked[10][10]) { // expected-error {{must have a pointer, array or function type that uses only checked types or parameter/return types with bounds-safe interfaces}}
  return 0;
}

// h25 is a function that returns a pointer to 10-element array of integers.
SCOPE_KIND int (*h25(int arr checked[10][10])) checked[10]{  // expected-error {{return in a checked scope must have a checked type or a bounds-safe interface}} \
                                                          // return int checked[10]
  return 0;
}

SCOPE_KIND ptr<int checked[10]> h26(int arr checked[10][10]) {
  return arr;  // ptr<T> = array_ptr<T> OK
}

SCOPE_KIND array_ptr<int checked[10]> h27(int arr checked[10][10]) {
  return arr;
}

// Test for type restrictions on various kinds of expression
// such as pointer arithmetic, logical operator, pointer difference or relational/equal compare
void check_pointer_arithmetic(void) {
  int p[5];
  int r checked[5];

  int *p_tmp = 0;
  array_ptr<int> r_tmp = 0;

  SCOPE_KIND {
    p_tmp = p + 5;    // expected-error 2 {{local variable used in a checked scope must have a checked type}}
    p_tmp = 5 + p;    // expected-error 2 {{local variable used in a checked scope must have a checked type}}
    p_tmp = p_tmp - 2;// expected-error 2 {{local variable used in a checked scope must have a checked type}}
    p_tmp = 2 - p;    // expected-error 2 {{local variable used in a checked scope must have a checked type}}
    p_tmp = p++;      // expected-error 2 {{local variable used in a checked scope must have a checked type}}
    p_tmp = p--;      // expected-error 2 {{local variable used in a checked scope must have a checked type}}
    p_tmp = ++p;      // expected-error 2 {{local variable used in a checked scope must have a checked type}}
    p_tmp = --p;      // expected-error 2 {{local variable used in a checked scope must have a checked type}}
    p_tmp = (p += 1); // expected-error 2 {{local variable used in a checked scope must have a checked type}}
    p_tmp = (p -= 1); // expected-error 2 {{local variable used in a checked scope must have a checked type}}

    // 0 interpreted as an integer, not null
    p_tmp = p + 0;    // expected-error 2 {{local variable used in a checked scope must have a checked type}}
    p_tmp = 0 + p;    // expected-error 2 {{local variable used in a checked scope must have a checked type}}
    p_tmp = p - 0;    // expected-error 2 {{local variable used in a checked scope must have a checked type}}
    p_tmp = 0 - p;    // expected-error 2 {{local variable used in a checked scope must have a checked type}}

    r_tmp = r + 5;
    r_tmp = 5 + r;
    r_tmp = r_tmp - 2;
    // 0 interpreted as an integer, not null
    r_tmp = r + 0;
    r_tmp = 0 + r;
    r_tmp = r - 0;
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

  SCOPE_KIND {
  if (flag) {
      p_int = a_int;  // expected-error 2 {{local variable used in a checked scope must have a checked type}}
      r_int = checked_a_int;
  }

  // pointer - array
  count = p_int - a_int;        // expected-error 2 {{local variable used in a checked scope must have a checked type}}
  count = p_int - checked_a_int;// expected-error {{local variable used in a checked scope must have a checked type}}
  count = r_int - a_int;        // expected-error {{local variable used in a checked scope must have a checked type}}
  count = r_int - checked_a_int;

  // array - pointer
  count = a_int - p_int;        // expected-error 2 {{local variable used in a checked scope must have a checked type}}
  count = checked_a_int - p_int;// expected-error {{local variable used in a checked scope must have a checked type}}
  count = a_int - r_int;        // expected-error {{local variable used in a checked scope must have a checked type}}
  count = checked_a_int - r_int;
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

  SCOPE_KIND {
  // relational comparisons between pointers and unchecked arrays;
  result = val_int < p_int; // expected-error 2 {{local variable used in a checked scope must have a checked type}}
  result = val_int <= q_int;// expected-error {{local variable used in a checked scope must have a checked type}}
  result = val_int >= r_int;// expected-error {{local variable used in a checked scope must have a checked type}}

  result = p_int > val_int; // expected-error 2 {{local variable used in a checked scope must have a checked type}}
  result = q_int < val_int; // expected-error {{local variable used in a checked scope must have a checked type}}
  result = r_int <= val_int;// expected-error {{local variable used in a checked scope must have a checked type}}


  // relational comparisons between pointers and checked arrays;
  result = checked_val_int < p_int; // expected-error {{local variable used in a checked scope must have a checked type}}
  result = checked_val_int <= q_int;
  result = checked_val_int >= r_int;

  result = p_int > checked_val_int; // expected-error {{local variable used in a checked scope must have a checked type}}
  result = q_int < checked_val_int;
  result = r_int <= checked_val_int;

  // equality/inequality comparisons between pointers and unchecked arrays;
  result = val_int == p_int;  // expected-error 2 {{local variable used in a checked scope must have a checked type}}
  result = val_int != q_int;  // expected-error {{local variable used in a checked scope must have a checked type}}
  result = val_int == r_int;  // expected-error {{local variable used in a checked scope must have a checked type}}

  result = p_int != val_int;  // expected-error 2 {{local variable used in a checked scope must have a checked type}}
  result = q_int == val_int;  // expected-error {{local variable used in a checked scope must have a checked type}}
  result = r_int != val_int;  // expected-error {{local variable used in a checked scope must have a checked type}}

  // equality/inequality comparisons between pointers and checked arrays;
  result = checked_val_int == p_int;  // expected-error {{local variable used in a checked scope must have a checked type}}
  result = checked_val_int != q_int;
  result = checked_val_int == r_int;

  result = p_int != checked_val_int;  // expected-error {{local variable used in a checked scope must have a checked type}}
  result = q_int == checked_val_int;
  result = r_int != checked_val_int;

  // spot check equality comparisons between pointers and checked arrays;
  result = checked_val_int == p_float;  // expected-error {{local variable used in a checked scope must have a checked type}}

  result = p_int == checked_val_float;  // expected-error {{local variable used in a checked scope must have a checked type}}

  }
}

void check_logical_operators(void) {
  int p[5];
  int r checked[5];

  _Bool b;

  checked {
  b = !p;     // expected-error {{local variable used in a checked scope must have a checked type}}
  b = !r;

  b = p || b;     // expected-error {{local variable used in a checked scope must have a checked type}}
  b = r || b;
  b = b || p;     // expected-error {{local variable used in a checked scope must have a checked type}}
  b = b || r;

  b = r || p;     // expected-error {{local variable used in a checked scope must have a checked type}}
  b = p || r;     // expected-error {{local variable used in a checked scope must have a checked type}}

  b = p && b;     // expected-error {{local variable used in a checked scope must have a checked type}}
  b = r && b;
  b = b && p;     // expected-error {{local variable used in a checked scope must have a checked type}}
  b = b && r;

  b = r && p;     // expected-error {{local variable used in a checked scope must have a checked type}}
  b = p && r;     // expected-error {{local variable used in a checked scope must have a checked type}}
  }
}

SCOPE_KIND void check_cast_operator(void) {
  int x = 0;
  int arr checked[5];

  // casts involving array types
  array_ptr<int> pax = (array_ptr<int>) &x;
  pax = &x;

  // casts involving pointers to array types

  // unchecked pointer to array
  ptr<int checked[5]> parr = 0;
  parr = (int(*)checked[5]) &arr; // expected-error {{type in a checked scope must be a checked pointer or array type}}
  parr = (int(*)checked[5]) ((int(*)checked[]) &arr); // expected-error {{type in a checked scope must be a checked pointer or array type}}
  parr = (int(*)checked[3]) &arr;   // expected-error {{type in a checked scope must be a checked pointer or array type}}
  parr = (int(*)[5]) &arr;          // expected-error {{type in a checked scope must be a checked pointer or array type}}
  parr = (int**) &arr;              // expected-error {{type in a checked scope must be a checked pointer or array type}}

  // ptr to array, ptr to unchecked array
  parr = (ptr<int checked[5]>) &arr;
  parr = (ptr<int checked[5]>) ((ptr<int checked[]>) &arr); // expected-warning {{cannot prove cast source bounds are wide enough for '_Ptr<int _Checked[5]>'}}
  parr = (ptr<int [5]>) &arr;   // expected-error {{type in a checked scope must use only checked types or parameter/return types with bounds-safe interfaces}}
  parr = (ptr<int *>) &arr;     // expected-error {{type in a checked scope must use only checked types or parameter/return types with bounds-safe interfaces}}

  // array_ptr to array, array_ptr to unchecked array
  array_ptr<int checked[5]> aparr = 0;
  aparr = (array_ptr<int checked[5]>) &arr;
  aparr = (array_ptr<int checked[5]>) ((array_ptr<int checked[]>) &arr);
  aparr = (array_ptr<int [5]>) &arr;  // expected-error {{type in a checked scope must use only checked types or parameter/return types with bounds-safe interfaces}}

  // ptr to variadic/unchecked func pointer
  ptr<int(int,int)> vpa = 0;
  vpa = (ptr<int(int,...)>) &arr;  // expected-error {{type in a checked scope cannot have variable arguments}}
  vpa = (ptr<int(int,ptr<int(int, ...)>)>) &arr;  // expected-error {{type in a checked scope cannot have variable arguments}}
  vpa = (ptr<int*(int,int)>) &arr;  // expected-error {{type in a checked scope must use only checked types or parameter/return types with bounds-safe interfaces}}
  vpa = (ptr<int(int,ptr<int(int, ...)>, ...)>) &arr;  // expected-error {{type in a checked scope cannot have variable arguments}}

  int *upa;                         // expected-error {{local variable in a checked scope must have a checked type}}
  upa = arr;
  upa = (int *)(array_ptr<int>)arr; // expected-error {{type in a checked scope must be a checked pointer or array type}}
  upa = &x;
  upa = (int [])&x;                 // expected-error {{cast to incomplete type}}
  upa = (int(*)[5])&x;              // expected-error {{type in a checked scope must be a checked pointer or array type}}

  upa = pax;
  upa = (int *)(array_ptr<int>)pax; // expected-error {{type in a checked scope must be a checked pointer or array type}}

  upa = aparr;
  upa = (int *)aparr;               // expected-error {{type in a checked scope must be a checked pointer or array type}}

  gptr0 = upa;
  gptr0 = (ptr<int>)upa;
  gptr0 = (array_ptr<int>)upa;

  gptr1 = upa;
  gptr1 = (ptr<int>)upa;
  gptr1 = (array_ptr<int>)upa;

  gptr2 = upa;                      // expected-error {{variable used in a checked scope must have a checked type}}
  gptr2 = (ptr<int>)upa;            // expected-error {{variable used in a checked scope must have a checked type}}
  gptr2 = (array_ptr<int>)upa;      // expected-error {{variable used in a checked scope must have a checked type}}

  unchecked {
  int *p;
  ptr<int> q = 0;
  array_ptr<int> r : count(5) = 0;
  SCOPE_KIND {
  p = _Dynamic_bounds_cast<int *>(q);           // expected-error {{local variable used in a checked scope must have a checked type}} \
                                                // expected-error {{type in a checked scope must be a checked pointer or array type}} \

  p = _Dynamic_bounds_cast<int *>(r);           // expected-error {{local variable used in a checked scope must have a checked type}} \
                                                // expected-error {{type in a checked scope must be a checked pointer or array type}} \

  q = _Assume_bounds_cast<ptr<int>>(r);             // expected-error {{_Assume_bounds_cast not allowed in a checked scope or function}}
  r = _Assume_bounds_cast<array_ptr<int>>(r,count(4));     // expected-error {{_Assume_bounds_cast not allowed in a checked scope or function}}
  r = _Assume_bounds_cast<array_ptr<int>>(r,bounds(r,r+1)); // expected-error {{_Assume_bounds_cast not allowed in a checked scope or function}}

  vpa = _Dynamic_bounds_cast<ptr<int(int,...)>>(r);   // expected-error {{type in a checked scope cannot have variable arguments}}
  vpa = _Dynamic_bounds_cast<ptr<int(int,ptr<int(int, ...)>)>>(r);  // expected-error {{type in a checked scope cannot have variable arguments}}
  vpa = _Dynamic_bounds_cast<ptr<int*(int,int)>>(r);  // expected-error {{type in a checked scope must use only checked types or parameter/return types with bounds-safe interfaces}}
  vpa = _Dynamic_bounds_cast<ptr<int(int,ptr<int(int, ...)>, ...)>>(r);  // expected-error {{type in a checked scope cannot have variable arguments}}

  vpa = _Assume_bounds_cast<ptr<int(int,...)>>(r);   // expected-error {{_Assume_bounds_cast not allowed in a checked scope or function}}
  vpa = _Assume_bounds_cast<ptr<int(int,ptr<int(int, ...)>)>>(r);  // expected-error {{_Assume_bounds_cast not allowed in a checked scope or function}}
  vpa = _Assume_bounds_cast<ptr<int*(int,int)>>(r);  // expected-error {{_Assume_bounds_cast not allowed in a checked scope or function}}
  vpa = _Assume_bounds_cast<ptr<int(int,ptr<int(int, ...)>, ...)>>(r);  // expected-error {{_Assume_bounds_cast not allowed in a checked scope or function}}
  }
  }
}

// Test for constructed type
// In checked block, constructed types (pointers, arrays, and function types) cannot contain
// unchecked type. To check if a declaration is valid in checked block,
// we SHOULD check checked pointer type recursively.
SCOPE_KIND void check_checked_constructed_type(void) {
  ptr<int*> a = 0;                                  // expected-error {{must have a pointer, array or function type that uses only checked types or parameter/return types with bounds-safe interfaces}}
  array_ptr<ptr<char*>> b : count(5) = 0;           // expected-error {{must have a pointer, array or function type that uses only checked types or parameter/return types with bounds-safe interfaces}}
  array_ptr<ptr<ptr<ptr<int*>>>> c : count(10) = 0; // expected-error {{must have a pointer, array or function type that uses only checked types or parameter/return types with bounds-safe interfaces}}
  ptr<ptr<ptr<int*>>> d = 0;                        // expected-error {{must have a pointer, array or function type that uses only checked types or parameter/return types with bounds-safe interfaces}}
  int sum = 0;

  sum += **a;
  sum += ***b;
  sum += *****c;
  sum += ****d;

  ptr<int[10]> pa = 0;              // expected-error {{must have a pointer, array or function type that uses only checked types or parameter/return types with bounds-safe interfaces}}
  ptr<int checked[10]> pb = 0;
  ptr<ptr<int[5]>> pc = 0;          // expected-error {{must have a pointer, array or function type that uses only checked types or parameter/return types with bounds-safe interfaces}}
  array_ptr<ptr<int[5]>> pd = 0;    // expected-error {{must have a pointer, array or function type that uses only checked types or parameter/return types with bounds-safe interfaces}}
  ptr<ptr<int checked[5]>> pe = 0;
  ptr<ptr<ptr<int[]>>> pf = 0;      // expected-error {{must have a pointer, array or function type that uses only checked types or parameter/return types with bounds-safe interfaces}}
  ptr<ptr<ptr<int checked[]>>> pg = 0;
}

int g(ptr<int *> p) {
  return **p;
}

SCOPE_KIND int f(ptr<int *> p) { // expected-error {{must have a pointer, array or function type that uses only checked types or parameter/return types with bounds-safe interfaces}}
  int r = g(p);
  return r;
}

int ff(ptr<int *> p) SCOPE_KIND {
  int r = g(p); // expected-error {{}}
  return r;
}

// Test for constructed type having variable arguments
// In checked block, checked pointer having variable arguments is not allowed
void check_checked_constructed_type_variadic(void) SCOPE_KIND {
  ptr<int> pa = 0;
  array_ptr<int> apa;

  ptr<void(int,int)> a = 0;
  ptr<int(int,int)> b = 0;
  ptr<int(int, ptr<int>, ...)> c = 0;           // expected-error {{variable in a checked scope cannot have variable arguments}}
  ptr<int(int, ptr<int(int, ...)>)> d = 0;      // expected-error {{variable in a checked scope cannot have variable arguments}}
  ptr<int(int, ptr<int(int,int)>, ...)> e = 0;  // expected-error {{variable in a checked scope cannot have variable arguments}}
  ptr<int(int, ptr<int(int,...)>, ...)> f = 0;  // expected-error {{variable in a checked scope cannot have variable arguments}}
  int (*g)(int, ...);                           // expected-error {{local variable in a checked scope must have a checked type}} //expected-error {{local variable in a checked scope cannot have variable arguments}}
  ptr<int*(int, int)> h = 0;                    // expected-error {{must have a pointer, array or function type that uses only checked types or parameter/return types with bounds-safe interfaces}}

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
    SCOPE_KIND {
    (*var_c)(2, pa, pa);  // expected-error {{local variable used in a checked scope cannot have variable arguments}}
    (*var_d)(3, var_b);   // expected-error 2 {{local variable used in a checked scope cannot have variable arguments}}
    (*var_e)(4, var_c, var_d);  // expected-error 3 {{local variable used in a checked scope cannot have variable arguments}}
    (*var_f)(6, var_d, var_e, a, b, g, h);// expected-error 4 {{local variable used in a checked scope cannot have variable arguments}} \
                                          // expected-error {{local variable used in a checked scope must have a checked type}} \
                                          // expected-error {{must have a pointer, array or function type that uses only checked types or parameter/return types with bounds-safe interfaces}}
    }
  }
}

SCOPE_KIND void checked_check_variadic1(int (*fptr)(int, ptr<int>, array_ptr<int>, ...)) { // expected-error {{parameter in a checked scope must have a checked type or a bounds-safe interface}} \
                                                                                        // expected-error {{parameter in a checked scope cannot have variable arguments}}
  ptr<int> a = 0;
  array_ptr<int> b;
  (*fptr)(5, a, b, a, b, a);
}

void checked_check_variadic2(int (*fptr)(int, ptr<int>, array_ptr<int>, ...)) SCOPE_KIND {
  ptr<int> a = 0;
  array_ptr<int> b;
  (*fptr)(5, a, b, a, b, a);  // expected-error {{parameter used in a checked scope must have a checked type or a bounds-safe interface}} \
                              // expected-error {{parameter used in a checked scope cannot have variable arguments}}
}

SCOPE_KIND void checked_check_variadic3(ptr<int(int, ptr<int>, array_ptr<int>, ...)> fptr) { // expected-error {{parameter in a checked scope cannot have variable arguments}}
  ptr<int> a = 0;
  array_ptr<int> b;
  (*fptr)(5, a, b, a, b, a);
}

void checked_check_variadic4(ptr<int(int, ptr<int>, array_ptr<int>, ...)> fptr) SCOPE_KIND {
  ptr<int> a = 0;
  array_ptr<int> b;
  (*fptr)(5, a, b, a, b, a);  // expected-error {{parameter used in a checked scope cannot have variable arguments}}
}

SCOPE_KIND void checked_check_variadic5(int cnt, ...) {  // expected-error {{variable arguments function cannot be made}}
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

extern void test_function_pointer(void) SCOPE_KIND {
  // address-of (&) operator produces ptr<T> for function type
  ptr<int(int)> fn0 = &id_int;
  ptr<int(int)> fn1 = id_int;

  int (*fn2)(int) = &id_int;   // expected-error {{local variable in a checked scope must have a checked type}}
  int (*fn3)(int) = id_int;  // expected-error {{local variable in a checked scope must have a checked type}}

  ptr<int(int*)> fn4 = &id_intp;   // expected-error {{must have a pointer, array or function type that uses only checked types or parameter/return types with bounds-safe interfaces}}
  ptr<int(int*)> fn5 = id_intp;  // expected-error {{must have a pointer, array or function type that uses only checked types or parameter/return types with bounds-safe interfaces}}

  ptr<ptr<int>(ptr<int>)> fn8 = &id_checked_intp;
  ptr<ptr<int>(ptr<int>)> fn9 = id_checked_intp;

  int val0;
  // address-of (&) operator produces array_ptr<T> except for function type
  ptr<int> pa = &val0;
  array_ptr<int> apa = &val0;
}


//
// Test for casting from ptr or array_ptr to nt_array_ptr.
// These two types of casting are forbidden in checked regions.
//
extern void test_cast_to_nt_array_ptr(void) checked {
  int i = 0;
  char c = 'c';
  int arr_i checked[5];
  char arr_c checked[5];

  // Test casting ptr to nt_array_ptr.
  ptr<int> p0 = (ptr<int>)&i;
  nt_array_ptr<int> p1 = (nt_array_ptr<int>)p0; // expected-error {{'_Ptr<int>' cannot be cast to '_Nt_array_ptr<int>' in a checked scope because '_Ptr<int>' might not point to a null-terminated array}} 
  ptr<char> p2 = (ptr<char>)&c;
  nt_array_ptr<char> p3 = (nt_array_ptr<char>)p2; // expected-error {{'_Ptr<char>' cannot be cast to '_Nt_array_ptr<char>' in a checked scope because '_Ptr<char>' might not point to a null-terminated array}} 
  ptr<char> p4 = "hello";
  p2 = (nt_array_ptr<char>)p4;  // expected-error {{'_Ptr<char>' cannot be cast to '_Nt_array_ptr<char>' in a checked scope because '_Ptr<char>' might not point to a null-terminated array}}

  // Test casting array_ptr to nt_array_ptr.
  array_ptr<int> p5 : count(5) = arr_i;
  nt_array_ptr<int> p6 = (nt_array_ptr<int>)arr_i; // expected-error {{'_Array_ptr<int>' cannot be cast to '_Nt_array_ptr<int>' in a checked scope because '_Array_ptr<int>' might not point to a null-terminated array}}
  p6 = (nt_array_ptr<int>)p5; // expected-error {{'_Array_ptr<int>' cannot be cast to '_Nt_array_ptr<int>' in a checked scope because '_Array_ptr<int>' might not point to a null-terminated array}}
  array_ptr<char> p7 : count(5) = arr_c;
  nt_array_ptr<char> p8 = (nt_array_ptr<char>)arr_c; // expected-error {{'_Array_ptr<char>' cannot be cast to '_Nt_array_ptr<char>' in a checked scope because '_Array_ptr<char>' might not point to a null-terminated array}}
  p8 = (nt_array_ptr<char>)p7; // expected-error {{'_Array_ptr<char>' cannot be cast to '_Nt_array_ptr<char>' in a checked scope because '_Array_ptr<char>' might not point to a null-terminated array}}

  // Test casting nt_array_ptr to nt_array_ptr
  nt_array_ptr<char> p9 = "hello";
  nt_array_ptr<char> p10 = (nt_array_ptr<char>)p9;  // legal casting

  // Test casting to nt_array_ptr in a unchecked scope.
  unchecked {
    // Test casting ptr to nt_array_ptr.
    p1 = (nt_array_ptr<int>)p0;   // allowed here; disallowed in a checked scope.
    p3 = (nt_array_ptr<char>)p2;  // allowed here; disallowed in a checked scope.
    p2 = (nt_array_ptr<char>)p4;  // allowed here; disallowed in a checked scope.
  
    // Test casting array_ptr to nt_array_ptr.
    p6 = (nt_array_ptr<int>)arr_i; // allowed here; disallowed in a checked scope.
    p6 = (nt_array_ptr<int>)p5;    // allowed here; disallowed in a checked scope.
    p8 = (nt_array_ptr<char>)p7;   // allowed here; disallowed in a checked scope.

    // Test casting nt_array_ptr to nt_array_ptr
    p10 = (nt_array_ptr<char>)p9;  // allowed in both checked and unchecked scopes.
    
    // Test casting from unchecked pointers to nt_array_ptr.
    int arr_i_unchecked[5];
    p6 = (nt_array_ptr<int>)arr_i_unchecked;  // allowed
    char arr_c_unchecked[5];
    p8 = (nt_array_ptr<char>)arr_c_unchecked; // allowed
  }
}

