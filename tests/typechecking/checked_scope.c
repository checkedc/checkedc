// Feature tests of typechecking of uses of Checked C checked scope/function
//
// The following lines are for the LLVM test harness:
//
// RUN: %clang_cc1 -fcheckedc-extension -Wno-unused-value -Wno-pointer-bool-conversion -verify -verify-ignore-unexpected=note %s
//

#include "../../include/stdchecked.h"
// checked function test
// - check if function declaration (return/param) is checked
// - check if function body (compound statement) is checked
// - check if function with no prototype checking is checked
checked int func0(int len, int *p, ptr<int> q, array_ptr<int> r, int *s : itype(ptr<int>)) { // expected-error {{parameter cannot have an unchecked pointer type}}
  int result = *p + *q + *s; // expected-error {{No use of parameter have an unchecked pointer type}}
  return result;
}
checked int* func1(int len, int *p, int *q, int *r : itype(array_ptr<int>)) {  // expected-error 2 {{parameter cannot have an unchecked pointer type}} \
                                                                            // expected-error {{return cannot have an unchecked pointer type}}
  int result = *p + *q + *r; // expected-error 2 {{No use of parameter have an unchecked pointer type}}
  int *presult = &result; // expected-error {{variable cannot have an unchecked pointer type}} expected-error {{initializing 'int *' with an expression of incompatible type}}
  return presult; // expected-error {{No use of variable have an unchecked pointer type }}
}
checked ptr<int> func2() { // expected-error {{function with no prototype cannot have a return type that is a checked type}} expected-error {{function without a prototype cannot be used or declared in a checked scope}}
}
// function without a prototype does not have an interop return type
checked int* func3() : itype(array_ptr<int>) { //expected-error {{return cannot have an unchecked pointer type in a checked scope}} //expected-error {{function without a prototype cannot be used or declared in a checked scope}}
}
checked int* func4(int len, int *p, int *q, array_ptr<int> r) : itype(ptr<int>) { // expected-error 2 {{parameter cannot have an unchecked pointer type}}
}
checked int func5(int p[]) { // expected-error {{parameter cannot have an unchecked array type in a checked scope}}
  int a = 5;
  int *upa = &a; // expected-error {{variable cannot have an unchecked pointer type in a checked scope}} expected-error {{initializing 'int *' with an expression of incompatible type}}
  ptr<int> pb = &a;
  array_ptr<int> pc = &a;
  int *upd = p; // expected-error {{variable cannot have an unchecked pointer type in a checked scope}} expected-error {{No use of parameter have an unchecked array type}}
  return *p; // expected-error {{No use of parameter have an unchecked array type}}
}

checked int func6(ptr<int> p, int *q) { // expected-error {{parameter cannot have an unchecked pointer type}}
  int a = 5;
  int *upa = &a; // expected-error {{variable cannot have an unchecked pointer type in a checked scope}} expected-error {{initializing 'int *' with an expression of incompatible type}}
  ptr<int> pb = p;
  return *pb;
}

checked int func7(ptr<int> p, int *q : itype(ptr<int>)) {
  int a = 5;
  int *upa = &a; // expected-error {{variable cannot have an unchecked pointer type in a checked scope}} expected-error {{initializing 'int *' with an expression of incompatible type}}
  int *upb = q; // expected-error {{variable cannot have an unchecked pointer type in a checked scope}}
  ptr<int> pc = p;
  int e[5][5]; // expected-error {{variable cannot have an unchecked array type}}
  return *pc;
}

checked int* func8(ptr<int> p, int *q : itype(ptr<int>)) { // expected-error {{return cannot have an unchecked pointer type in a checked scope}}
  int a = 5;
  ptr<int> pa = &a;
  ptr<int> pb = p;
  int *upc = q; // expected-error {{variable cannot have an unchecked pointer type in a checked scope}}
  return upc; // expected-error {{No use of variable have an unchecked pointer type}}
}

checked int* func9(ptr<int> p, int *q : itype(ptr<int>)) : itype(ptr<int>) {
  int a = 5;
  ptr<int> pa = &a;
  ptr<int> pb = p;
  int *upc = q; // expected-error {{variable cannot have an unchecked pointer type in a checked scope}}
  return upc; // expected-error {{No use of variable have an unchecked pointer type}}
}

// checked function + checked scope keyword test
int* func10(array_ptr<int> x, int len) checked {
  int *upa; // expected-error {{variable cannot have an unchecked pointer type in a checked scope}}
  return upa; // expected-error {{No use of variable have an unchecked pointer type}}
}

array_ptr<int> func11(array_ptr<int> x, int len) : bounds(x,x+len) checked {
  int *upa; // expected-error {{variable cannot have an unchecked pointer type in a checked scope}}
  return upa; // expected-error {{No use of variable have an unchecked pointer type}}
}

array_ptr<int> func12(int *x, int *y) checked {
  int *upa = x; // expected-error {{variable cannot have an unchecked pointer type in a checked scope}} expected-error {{No use of parameter have an unchecked pointer type}}
  int *upb = y; // expected-error {{variable cannot have an unchecked pointer type in a checked scope}} expected-error {{No use of parameter have an unchecked pointer type}}
  return upb; // expected-error {{No use of variable have an unchecked pointer type}}
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
  return upa; // expected-error {{No use of variable have an unchecked pointer type}}
}

checked int* func16(int a checked[][5], int b checked[][5]) : itype(ptr<int>) checked {
}

checked int* func17(int a [][5], int b [][5]) : itype(ptr<int>) checked { // expected-error 2 {{parameter cannot have an unchecked array type in a checked scope}}
}

checked int* func18(int *a : itype(ptr<int>), int *b : itype(array_ptr<int>)) : itype(array_ptr<int>) checked {
  int e checked[5][5];
  int f[5][5]; // expected-error {{variable cannot have an unchecked array type in a checked scope}}
  int *upa = func16(e, e); // expected-error {{variable cannot have an unchecked pointer type in a checked scope}}
  ptr<int> pb = func16(e, e);
  ptr<int> pc = 0;
  pc = func17(f, f); // expected-error 2 {{No use of variable have an unchecked array type}}
  int *upd = func10(upa, 10); // expected-error {{variable cannot have an unchecked pointer type}} expected-error {{No use of return have an unchecked pointer type}} expected-error {{No use of variable have an unchecked pointer type}}
  return upa; // expected-error {{No use of variable have an unchecked pointer type}}
}

checked int* func19(int a[] : itype(int *), char b[] : itype(char *)) : itype(int *) checked { // expected-error 3 {{type must be a checked type}} expected-error 2 {{parameter cannot have an unchecked array type in a checked scope}} expected-error {{return cannot have an unchecked pointer type}}
}

// checked scope test
// - compound statments are checked
// - nested checked scope inherits parent checked property
int func20(void) checked {
  int a = 5;
  ptr<int> pa = &a;
  int b[5][5]; // expected-error {{variable cannot have an unchecked array type in a checked scope}}
  for (int i = 0; i < 5; i++) {
    for (int j = 0; j < 5; j++) {
      b[i][j] = -1; // expected-error {{No use of variable have an unchecked array type}}
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
  checked {
    int *upa = &a; // expected-error {{variable cannot have an unchecked pointer type in a checked scope}} expected-error {{initializing 'int *' with an expression of incompatible type}}
  checked {
    int b[5][5]; // expected-error {{variable cannot have an unchecked array type in a checked scope}}
  checked {
    for (int i = 0; i < 5; i++) {
      for (int j = 0; j < 5; j++) {
        b[i][j] = -1; // expected-error {{No use of variable have an unchecked array type}}
      }
    }
  }
  }
  }
}

int func23(void) {
  int a = 5;
  checked {
    {
      int *upa = &a; // expected-error {{variable cannot have an unchecked pointer type in a checked scope}} expected-error {{initializing 'int *' with an expression of incompatible type}}
    {
      int b[5][5]; // expected-error {{variable cannot have an unchecked array type in a checked scope}}
    {
      int c checked[5][5];
      for (int i = 0; i < 5; i++) {
        for (int j = 0; j < 5; j++) {
          b[i][j] += c[i][j];  // expected-error {{No use of variable have an unchecked array type}}
        }
      }
    }
    }
    }
  }
}

int func24(void) {
  int a = 5;
  checked {
    int *upa = &a; // expected-error {{variable cannot have an unchecked pointer type in a checked scope}} expected-error {{initializing 'int *' with an expression of incompatible type}}
    int b[5][5]; // expected-error {{variable cannot have an unchecked array type in a checked scope}}
    for (int i = 0; i < 5; i++) {
      for (int j = 0; j < 5; j++) {
        b[i][j] = -1; // expected-error {{No use of variable have an unchecked array type}}
      }
    }
  }
}

int func25(void) {
  int a = 5;
  checked {
    {
      // checked scope inheritance
      struct s0 {
        int *a; // expected-error {{member cannot have an unchecked pointer type}}
        char *b; // expected-error {{member cannot have an unchecked pointer type}}
        ptr<int> pc;
        array_ptr<int> pd : count(len);
        int len;
        short e[10]; // expected-error {{member cannot have an unchecked array type}}
      } a;
    }
  }
}

void func30(void) {
  int a = 5;
  int len = 10;
  array_ptr<int> pa : count(len) = 0;
  // checked scope keyword parsing error case
  checked(len); // expected-error {{expected compound statement after checked scope keyword}}
  checked [5][5]; // expected-error {{expected compound statement after checked scope keyword}}
}

struct s0 checked {
  int *a; // expected-error {{member cannot have an unchecked pointer type}}
  char *b; // expected-error {{member cannot have an unchecked pointer type}}
  ptr<int> pc;
  array_ptr<int> pd : count(len);
  int len;
  // checked scope inheritance
  struct s1 {
    int *a; // expected-error {{member cannot have an unchecked pointer type}}
    char *b; // expected-error {{member cannot have an unchecked pointer type}}
    ptr<int> pc;
    array_ptr<int> pd : count(len);
    int len;
  } d;
};

struct s2 {
  int *a;
  char *b;
  ptr<int> pc;
  array_ptr<int> pd : count(len);
  int len;
  // checked scope inheritance
  struct s3 {
    int *a;
    char *b;
    ptr<int> pc;
    array_ptr<int> pd : count(len);
    int len;
  } d;
};


checked int func31(ptr<struct s0> st0, ptr<struct s1> st1) {
  // struct s0 member a & b is an error of declaration
  int sum = *(st0->a) + *(st1->a);
  sum += *(st0->b) + *(st1->b);
  sum += *(st0->pc) + *(st1->pc);
  sum += *(st0->pd) + *(st1->pd);

  struct s2 sta;
  ptr<struct s2> pstb = 0;
  sum += *(sta.a) + *(sta.b) + *(sta.pc) + *(sta.pd); // expected-error 2 {{No use of member have an unchecked pointer type}}
  sum += *(pstb->a) + *(pstb->b); // expected-error 2 {{No use of member have an unchecked pointer type}}
  sum += *(pstb.a) + *(pstb.b); // expected-error 2 {{No use of member have an unchecked pointer type}} expected-error 2 {{member reference type}}
  sum += *(sta->a) + *(sta->b); // expected-error 2 {{No use of member have an unchecked pointer type}} expected-error 2 {{member reference type}}

  ptr<struct s2> st2 = 0;
  ptr<struct s2> st3 = 0;
  sum += *(st2->a)  + *(st3->a); // expected-error 2 {{No use of member have an unchecked pointer type}}
  sum += *(st2->b)  + *(st3->b); // expected-error 2 {{No use of member have an unchecked pointer type}}
  sum += *(st2->pc) + *(st3->pc);
  sum += *(st2->pd) + *(st3->pd);
  sum += *(st2->d.a) + *(st3->d.a); // expected-error 2 {{No use of member have an unchecked pointer type}}
  sum += *(st2->d.b) + *(st3->d.b); // expected-error 2 {{No use of member have an unchecked pointer type}}
  return sum;
}

// change type produced by address-of operator(&) in checked block
// checked { .... int a; ptr<int> pb = &a; }
void test_addrof_checked_scope(void) checked {
  int a checked[10];
  array_ptr<int> b;
  int i;

  // in checked scope, address-of operator produces _Ptr<T>
  // ptr<T> == array_ptr<T> : count(1) <- has bounds information
  // array_ptr<T> <- has no bounds information

  // VisitBinaryOperator - check if LHS has bounds and RHS has bounds
  // LHS has bounds (ptr<T>=array_ptr<T> : count(1))
  // expected that ptr<T> automatic variable has initialization
  ptr<int> x = &a[i]; // UnaryOperator _Ptr<int>
  ptr<int> y = &b[0]; // UnaryOperator _Ptr<int> expected-error {{expression has no bounds, cast to ptr<T> require a source /w bounds}}
  ptr<int> z = &i; // UnaryOperator _Ptr<int>

  x = &a[i]; // UnaryOperator _Ptr<int>
  y = &b[1]; // UnaryOperator _Ptr<int>, inferred RHS bounds (x) expected-error {{expression has no bounds, lvalue /w bounds require a rvalue /w bounds}} expected-error {{expression has no bounds, cast to ptr<T> require a source /w bounds}}
  z = &i; // UnaryOperator _Ptr<int>

  // NOTE
  // implicit cast between checked pointer types is not allowed
  // - do not allow array_ptr to be converted to ptr -> needs to be allowed
  // - do not allow ptr to be converted to array_ptr -> needs to be allowed
  // incompatible type error, assigning to from incompatible type
  // CheckAssignmentConstraints -> checkPointerTypesForAssignment
  // err_expected_bounds - binary operator, inferred RHS bounds(x)
  // err_expected_bounds - implicit cast operator, source operand bounds (x)
  x = b; // BinaryOperator (ImplicitCastExpr _Ptr (_Array_ptr)) expected-error {{expression has no bounds, lvalue /w bounds require a rvalue /w bounds}} expected-error {{expression has no bounds, cast to ptr<T> require a source /w bounds}}

  // address-of operator output _Ptr<T> is incompatible with _Array_ptr
  // incompatible pointer type between checked pointer types
  array_ptr<int> ax = &a[i];
  array_ptr<int> ay = &b[2];
  array_ptr<int> az = &i;

  // VisitBinaryOperator - since LHS is array_ptr having no bounds, ignore RHS bounds checking
  ax = &a[i];
  ay = &b[3];
  az = &i;

  // &*(b+i) is different from &b[i]
  // &*(b+i) result type = type of (b+i) = array_ptr<int>
  // &b[i] result type = pointer type of int = ptr<int>
  array_ptr<int> px = &(*(b+i));
  array_ptr<int> py = &b[i];
  array_ptr<int> pz = &(*(a+i));

  px = &(*(b+i));
  py = &b[i];
  pz = &(*(a+i));

  int aa[5]; // expected-error {{variable cannot have an unchecked array type}}
  x = &aa[i]; // expected-error {{No use of variable have an unchecked array type}}
  y = &*(aa+i); // expected-error {{No use of variable have an unchecked array type}}
}

void test_addrof_unchecked_scope(void) {
  int a checked[10];
  array_ptr<int> b;
  int i;

  // checkSingleAssignmentConstraints(int * -> _Ptr<int> implicit casting)
  ptr<int> x = &a[i]; // ImplicitCastExpr _Ptr<int>(UnaryOperator int * prefix &)
  ptr<int> y = &b[0]; // ImplicitCastExpr _Ptr<int>(UnaryOperator int * prefix &) expected-error {{expression has no bounds, cast to ptr<T> require a source /w bounds}}
  ptr<int> z = &i; // ImplicitCastExpr _Ptr<int>(UnaryOperator int * prefix &)

  // implicit cast for _Ptr<T> requires source bounds
  x = &a[i]; // BinaryOperator(ImplicitCastExpr _Ptr<int>(UnaryOperator int * prefix &))
  y = &b[0]; // BinaryOperator(ImplicitCastExpr _Ptr<int>(UnaryOperator int * prefix &)) expected-error {{expression has no bounds, lvalue /w bounds require a rvalue /w bounds}} expected-error {{expression has no bounds, cast to ptr<T> require a source /w bounds}}
  z = &i; // BinaryOperator(ImplicitCastExpr _Ptr<int>(UnaryOperator int * prefix &))

  x = b; // BinaryOperator(ImplicitCastExpr()) expected-error {{expression has no bounds, lvalue /w bounds require a rvalue /w bounds}} expected-error {{expression has no bounds, cast to ptr<T> require a source /w bounds}}

  // checkSingleAssignmentConstraints(int * -> _Array_ptr<int> implicit casting)
  array_ptr<int> ax = &a[i]; // ImplicitCastExpr _Array_ptr<int>(UnaryOperator)
  array_ptr<int> ay = &b[0]; // ImplicitCastExpr _Array_ptr<int>(UnaryOperator)
  array_ptr<int> az = &i; // ImplicitCastExpr _Array_ptr<int>(UnaryOperator)

  ax = &a[i]; // ImplicitCastExpr _Array_ptr<int>(UnaryOperator)
  ay = &b[0]; // ImplicitCastExpr _Array_ptr<int>(UnaryOperator)
  az = &i; // ImplicitCastExpr _Array_ptr<int>(UnaryOperator)

  // &E, T(type of E)
  // address of operand = UnaryOperator(UO_AddrOf, E)
  // address of type
  // if E is *SE, &E = &*SE = type of SE
  // if E is in unchecked block, it is unchecked pointer type of E(T *)
  // if E is in checked block, it is checked pointer type of E(_Ptr<T>)

  array_ptr<int> px = &(*(b+i)); // UnaryOperator _Array_ptr<int>(ParenExpr(UnaryOperator(*))
  array_ptr<int> py = &b[i]; // ImplicitCastExpr _Array_ptr<int>(UnaryOperator)
  array_ptr<int> pz = &(*(a+i)); // UnaryOperator _Array_ptr<int>(ParenExpr(UnaryOperator(*)))

  px = &(*(b+i)); // UnaryOperator _Array_ptr<int>()
  py = &b[i]; // ImplicitCastExpr _Array_ptr<int>(UnaryOperator)
  pz = &(*(a+i)); // UnaryOperator _Array_ptr<int>()
}

// casting operation is restricted in checked scope
// No C-style casts to unchecked pointer type

int *gptr0 : itype(ptr<int>);
int *gptr1 : itype(array_ptr<int>);
int *gptr2;

//////////////////////////////////////////////////////////////////////////////////////////
/////////////////////// checked_arrays.c + checked scope test code
//////////////////////////////////////////////////////////////////////////////////////////

// checked scope use error precedes other error or warning
// previous checked error is not generated in checked block tests if checked scope error occurred
// To represent non-error that was an error in unchecked block, expected-no is used
// expected-no-error, expected-no-warning is previous error or warning but not in checked block
// since checked scope error precedes those
// To represent special case where previous error is not overrided, we specified NOTE for that case
extern void check_indirection_unchecked(int p[10], const int const_p[10], int y) checked {
  *p = y;       // expected-error {{No use of parameter have an unchecked array type}}
  y = *p;       // expected-error {{No use of parameter have an unchecked array type}}
  *const_p = y; // expected-error {{No use of parameter have an unchecked array type}} expected-no-error {{read-only variable is not assignable}}
  y = *const_p; // expected-error {{No use of parameter have an unchecked array type}}
}

extern void check_indirection_checked(int p checked[10], const int const_p checked[10], int y) checked {
  *p = y;
  y = *p;
  *const_p = y; // expected-error {{read-only variable is not assignable}}
  y = *const_p;
}

extern void check_indirection_checked_incomplete(int p checked[] : count(len),
                                                 const int const_p checked[] : count(len),
                                                 int len, int y) checked {
  *p = y;
  y = *p;
  *const_p = y; // expected-error {{read-only variable is not assignable}}
  y = *const_p;
}

extern void check_subscript_unchecked(int p[10], int y) checked {
  p[0] = y;     // expected-error {{No use of parameter have an unchecked array type}}
  y = p[0];     // expected-error {{No use of parameter have an unchecked array type}}
  0[p] = y;     // expected-error {{No use of parameter have an unchecked array type}}
  y = 0[p];     // expected-error {{No use of parameter have an unchecked array type}}
}

extern void check_subscript_checked(int p checked[10], const int p_const[10], int y) checked {
  p[0] = y;  // OK
  y = p[0];  // OK
  0[p] = y;  // OK
  y = 0[p];  // OK
  p_const[0] = y;  // expected-error {{No use of parameter have an unchecked array type}} expected-no-error {{read-only variable is not assignable}}
  y = p_const[0];  // expected-error {{No use of parameter have an unchecked array type}}
  0[p_const] = y;  // expected-error {{No use of parameter have an unchecked array type}} expected-no-error {{read-only variable is not assignable}}
  y = 0[p_const];  // expected-error {{No use of parameter have an unchecked array type}}
}

extern void check_subscript_checked_incomplete(int p checked[] : count(len),
                                               const int p_const[] : count(len),
                                               int len, int y) checked {
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

typedef int unchecked_arr_type[10];
typedef int checked_arr_type[10];


// Test assignments between pointers and arrays, excluding const/volatile attributes.
extern void check_assign(int val, int p[10], int q[], int r checked[10], int s checked[],
                         int s2d checked[10][10]) checked {
  int t[10];              // expected-error {{variable cannot have an unchecked array type}}
  int t2d[10][10];        // expected-error {{variable cannot have an unchecked array type}}
  int u checked[10];
  int u2d checked[10][10]; // This is a checked array of checked arrays. checked propagates
                            // to immediately nested array types in array declarators.  It does
                            // not propagate through typedefs

  // Single-dimensional array type conversions to pointer types.
  int *t1 = p;          // expected-error {{variable cannot have an unchecked pointer type}} expected-error {{No use of parameter have an unchecked array type}}
                        // T *  = T[constant] OK
  int *t2 = q;          // expected-error {{variable cannot have an unchecked pointer type}} expected-error {{No use of parameter have an unchecked array type}}
                        // T *  = T[] OK
  int *t3 = t;          // expected-error {{variable cannot have an unchecked pointer type}} expected-error {{No use of variable have an unchecked array type}}
                        // T *  = T[constant] OK;
  int *t4 = r;          // expected-error {{variable cannot have an unchecked pointer type}} expected-error {{expression of incompatible type}}
                        // Assignment of checked pointer to unchecked pointer not allowed
  int *t5 = s;          // expected-error {{variable cannot have an unchecked pointer type}} expected-error {{expression of incompatible type}}
  int *t6 = u;          // expected-error {{variable cannot have an unchecked pointer type}} expected-error {{expression of incompatible type 'int checked[10]'}}

  // Various forms of array_ptr<T> = T[]. Note that the rhs does not need to have known bounds
  // because the lhs pointers have no bounds (and cannot be dereferenced).
  //
  // Note if there need to be known bounds, the bounds of p and q are unknown
  // because C does not guarantee that array sizes match for parameter passing
  array_ptr<int> t7 = p;        // expected-error {{No use of parameter have an unchecked array type}}
  array_ptr<int> t8 = q;        // expected-error {{No use of parameter have an unchecked array type}}
  array_ptr<int> t9 = r;
  array_ptr<int> t10  = s;
  array_ptr<int> t11 = t;       // expected-error {{No use of variable have an unchecked array type}}
  array_ptr<int> t12 = u;
  array_ptr<int> t13 = s2d[0];
  array_ptr<int> t14 = t2d[0];  // expected-error {{No use of variable have an unchecked array type}}
  array_ptr<int> t15 = u2d[0];


  // Multi-dimensional array type conversions to pointer types.
  int *t16 = s2d[0];        // expected-error {{variable cannot have an unchecked pointer type}} expected-error {{expression of incompatible type 'int checked[10]'}}
  int *t17 = t2d[0];        // expected-error {{variable cannot have an unchecked pointer type}} expected-error {{No use of variable have an unchecked array type}}
  int *t18 = u2d[0];        // expected-error {{variable cannot have an unchecked pointer type}} expected-error {{expression of incompatible type 'int checked[10]'}}
  int(*t19)[10] = s2d;      // expected-error {{variable cannot have an unchecked pointer type}} expected-error {{expression of incompatible type '_Array_ptr<int checked[10]>'}}
                            // assignment of checked array to unchecked array not allowed
  int (*t20)[10] = t2d;     // expected-error {{variable cannot have an unchecked pointer type}} expected-error {{No use of variable have an unchecked array type}}
  int (*t21)[10] = u2d;     // expected-error {{variable cannot have an unchecked pointer type}} expected-error {{expression of incompatible type 'int checked[10][10]'}}
                            // assignment of checked array to unchecked array not allowed
  array_ptr<int[10]> t22 = s2d; // expected-error {{expression of incompatible type '_Array_ptr<int checked[10]>'}}
                                // assignment of checked to unchecked not allowed
  array_ptr<int[10]> t23 = t2d; // expected-error {{No use of variable have an unchecked array type}}
  array_ptr<int[10]> t24 = u2d; // expected-error {{expression of incompatible type 'int checked[10][10]'}}
                                // assignment of checked to unchecked not allowed
  array_ptr<int checked[10]> t25 = s2d;
  array_ptr<int checked[10]> t26 = t2d; // expected-error {{No use of variable have an unchecked array type}}
  array_ptr<int checked[10]> t27 = u2d;

  // Assignments to array-typed parameters are allowed.  The outermost array modifier
  // is converted to a pointer type.
  p = q;  // expected-error 2 {{No use of parameter have an unchecked array type}}
  q = p;  // expected-error 2 {{No use of parameter have an unchecked array type}}
  s = r;
  r = t;  // expected-error {{No use of variable have an unchecked array type}}
  p = r;  // expected-error {{No use of parameter have an unchecked array type}} expected-no-error {{assigning to 'int *' from incompatible type '_Array_ptr<int>'}}
          // assignment of checked pointer to unchecked pointer not allowed

  // Assignments to array-typed local and global variables are not allowed
  t = p;  // expected-error {{No use of parameter have an unchecked array type}} expected-error {{No use of variable have an unchecked array type}} expected-no-error {{array type 'int [10]' is not assignable}}
  t = r;  // expected-error {{No use of variable have an unchecked array type}} expected-no-error {{array type 'int [10]' is not assignable}}
  u = r;  // expected-error {{array type 'int checked[10]' is not assignable}}
  global_arr = p; // expected-error {{No use of variable have an unchecked array type}} expected-error {{No use of parameter have an unchecked array type}} expected-no-error {{array type 'int [10]' is not assignable}}
  global_arr = r; // expected-error {{No use of variable have an unchecked array type}} expected-no-error {{array type 'int [10]' is not assignable}}
  global_checked_arr = r; // expected-error {{array type 'int checked[10]' is not assignable}}
}

// Test that dimensions in multi-dimensional arrays are either all checked or unchecked arrays.
extern void check_dimensions1(void) checked {
  int t1 checked[10][5]checked[5];     // multiple checked modifiers are allowed
  int t2 checked[10][5][5]checked[5];

  // checked mixing of checked/unchecked array dimensions
  int t3[10]checked[10];               // expected-error {{unchecked array of checked array not allowed}}
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
  dim_unchecked (t10 checked[10])[10]; // expected-error {{checked array of unchecked array not allowed \
('dim_unchecked' is an unchecked array)}}
  dim_checked (t11[10])[10];           // expected-error {{unchecked array of checked array not allowed \
('dim_checked' is a checked array)}}

  // make sure checked-ness propagated
  // NOTE : checked scope error fails to precede
  int *t12 = t6[0];                    // expected-error {{variable cannot have an unchecked pointer type}} expected-error {{expression of incompatible type 'int checked[10]'}}
  array_ptr<int> t13 = t6[0];
}

// Test that dimensions for incomplete array types are either all checked or unchecked arrays
checked void check_dimensions2(int r2d checked[][10] : count(len), int len) {
}

checked void check_dimensions3(int (r2d checked[])[10] : count(len), int len) {
}

checked void check_dimensions4(int r2d []checked[10] : count(len), int len) { // expected-error {{unchecked array of checked array not allowed}}
}

checked void check_dimensions5(int (r2d[])checked[10] : count(len), int len) { // expected-error {{unchecked array of checked array not allowed}}
}

// Test that qualifiers on the outermost dimension of a checked array-typed parameter are preserved.
checked void check_dimensions6(int r2d checked[const][10] : count(len), int len) {
  r2d = 0;          // expected-error {{cannot assign to variable 'r2d' with const-qualified type}}
  int *t1 = r2d[0]; // expected-error {{variable cannot have an unchecked pointer type }} expected-error {{expression of incompatible type 'int checked[10]'}}
}

checked void check_dimensions7(int (r2d checked[const])[10] : count(len), int len) {
  r2d = 0;          // expected-error {{cannot assign to variable 'r2d' with const-qualified type}}
  int *t1 = r2d[0]; // expected-error {{variable cannot have an unchecked pointer type }} expected-error {{expression of incompatible type 'int checked[10]'}}
}

checked void check_dimensions8(int (r2d) checked[const][10] : count(len), int len) {
  r2d = 0;          // expected-error {{cannot assign to variable 'r2d' with const-qualified type}}
  int *t1 = r2d[0]; // expected-error {{variable cannot have an unchecked pointer type }} expected-error {{expression of incompatible type 'int checked[10]'}}
}

// Test assignments between pointers of different kinds with const/volatile
// attributes on referent types
extern void check_assign_cv(int param[10],
                            int checked_param checked[10],
                            int param_const_ptr[const 10],
                            int checked_param_const_ptr checked[const 10],
                            const int const_param[10],
                            const int const_checked_param checked[10],
                            const int const_param_const_ptr[const 10],
                            const int const_checked_param_const_ptr checked[const 10],
                            volatile int volatile_param[10],
                            volatile int checked_volatile_param checked[10]) checked {
  int a[10];    // expected-error {{variable cannot have an unchecked array type}}
  int b checked[10];
  const int const_a[10];    // expected-error {{variable cannot have an unchecked array type}}
  const int const_b checked[10];
  volatile int volatile_a[10];    // expected-error {{variable cannot have an unchecked array type}}

  // NOTE : checked scope error fails to precede
  int a_const_ptr[const 10]; // expected-error {{variable cannot have an unchecked array type}} expected-error {{type qualifier used in array declarator}}

  //
  // check assignments to parameters
  //
  // assign an unchecked array,where the element type does not have modifiers
  param = a;                          // expected-error {{No use of variable have an unchecked array type}} expected-error {{No use of parameter have an unchecked array type}}
  checked_param = a;                  // expected-error {{No use of variable have an unchecked array type}}
  param_const_ptr = a;                // expected-error {{No use of variable have an unchecked array type}} expected-error {{No use of parameter have an unchecked array type}} expected-no-error {{cannot assign to variable}}
  checked_param_const_ptr = a;        // expected-error {{No use of variable have an unchecked array type}} expected-no-error {{cannot assign to variable}}
  const_param = a;                    // expected-error {{No use of variable have an unchecked array type}} expected-error {{No use of parameter have an unchecked array type}}
  const_checked_param = a;            // expected-error {{No use of variable have an unchecked array type}}
  const_param_const_ptr = a;          // expected-error {{No use of variable have an unchecked array type}} expected-error {{No use of parameter have an unchecked array type}} expected-no-error {{cannot assign to variable}}
  const_checked_param_const_ptr = a;  // expected-error {{No use of variable have an unchecked array type}} expected-no-error {{cannot assign to variable}}
  volatile_param = a;                 // expected-error {{No use of variable have an unchecked array type}} expected-error {{No use of parameter have an unchecked array type}}
  checked_volatile_param = a;         // expected-error {{No use of variable have an unchecked array type}}

  // assign a checked array, where the element type does not have modifiers
  param = b;                          // expected-error {{No use of parameter have an unchecked array type}} expected-no-error {{incompatible type}}
  checked_param = b;
  param_const_ptr = b;                // expected-error {{No use of parameter have an unchecked array type}} expected-no-error {{cannot assign to variable}}
  checked_param_const_ptr = b;        // expected-error {{cannot assign to variable}}
  const_param = b;                    // expected-error {{No use of parameter have an unchecked array type}} expected-no-error {{incompatible type}}
  const_checked_param = b;
  const_param_const_ptr = b;          // expected-error {{No use of parameter have an unchecked array type}} expected-no-error {{cannot assign to variable}}
  const_checked_param_const_ptr = b;  // expected-error {{cannot assign to variable}}
  volatile_param = b;                 // expected-error {{No use of parameter have an unchecked array type}} expected-no-error {{incompatible type}}

  // check assigning an unchecked array where the element type has modifiers
  param = const_a;                    // expected-error {{No use of variable have an unchecked array type}} expected-error {{No use of parameter have an unchecked array type}} expected-no-warning {{discards qualifiers}}
  checked_param = const_a;            // expected-error {{No use of variable have an unchecked array type}} expected-no-warning {{discards qualifiers}}
  const_param = const_a;              // expected-error {{No use of variable have an unchecked array type}} expected-error {{No use of parameter have an unchecked array type}}
  const_checked_param = const_a;      // expected-error {{No use of variable have an unchecked array type}}
  volatile_param = const_a;           // expected-error {{No use of variable have an unchecked array type}} expected-error {{No use of parameter have an unchecked array type}} expected-no-warning {{discards qualifiers}}
  checked_volatile_param = const_a;   // expected-error {{No use of variable have an unchecked array type}} expected-no-warning {{discards qualifiers}}

  // spot check assigning a checked array where the element type has modifiers
  checked_param = const_b;            // expected-warning {{discards qualifiers}}
  const_checked_param = const_b;
  volatile_param = const_b;           // expected-error {{No use of parameter have an unchecked array type}} expected-no-error {{incompatible type}}
  checked_volatile_param = const_b;   // expected-warning {{discards qualifiers}}

  //
  // check assignments to local variable pointers
  //

  // the rhs is a parameter with array type
  int *p = 0;                     // expected-error {{variable cannot have an unchecked pointer type}}
  const int *p_const = 0;         // expected-error {{variable cannot have an unchecked pointer type}}
  volatile int *p_volatile = 0;   // expected-error {{variable cannot have an unchecked pointer type}}
  ptr<int> q = 0;
  ptr<const int> q_const = 0;
  ptr<volatile int> q_volatile = 0;
  array_ptr<int> r = 0;
  array_ptr<const int> r_const = 0;
  array_ptr<volatile int> r_volatile = 0;

  // the rhs is an unchecked array where the element type does not have modifiers
  p = param;            // expected-error {{No use of variable have an unchecked pointer type}} expected-error {{No use of parameter have an unchecked array type}}
  p_const = param;      // expected-error {{No use of variable have an unchecked pointer type}} expected-error {{No use of parameter have an unchecked array type}}
  p_volatile = param;   // expected-error {{No use of variable have an unchecked pointer type}} expected-error {{No use of parameter have an unchecked array type}}
  // q = param;          not allowed: param has unknown bounds
  // q_const = param;    not allowed: param has unknown bounds
  // q_volatile = param; not allowed: param has unknown bounds
  r = param;            // expected-error {{No use of parameter have an unchecked array type}}
  r_const = param;      // expected-error {{No use of parameter have an unchecked array type}}
  r_volatile = param;   // expected-error {{No use of parameter have an unchecked array type}}

  // the rhs is a checked array where the element type does not have modifiers
  p = checked_param;              // expected-error {{No use of variable have an unchecked pointer type}} expected-no-error {{incompatible type}}
  p_const = checked_param;        // expected-error {{No use of variable have an unchecked pointer type}} expected-no-error {{incompatible type}}
  p_volatile = checked_param;     // expected-error {{No use of variable have an unchecked pointer type}} expected-no-error {{incompatible type}}
  q = checked_param;              // expected-correct {{incompatible type}}, ptr<T> = array_ptr<T> OK
  q_const = checked_param;        // expected-correct {{incompatible type}}, ptr<T> = array_ptr<T> OK
  q_volatile = checked_param;     // expected-correct {{incompatible type}}, ptr<T> = array_ptr<T> OK
  r = checked_param;
  r_const = checked_param;
  r_volatile = checked_param;

  // the rhs is an unchecked array where the element type has modifiers
  p = const_param;              // expected-error {{No use of variable have an unchecked pointer type}} expected-error {{No use of parameter have an unchecked array type}} expected-no-warning {{discards qualifiers}}
  p_const = const_param;        // expected-error {{No use of variable have an unchecked pointer type}} expected-error {{No use of parameter have an unchecked array type}}
  p_volatile = const_param;     // expected-error {{No use of variable have an unchecked pointer type}} expected-error {{No use of parameter have an unchecked array type}} expected-no-warning {{discards qualifiers}}
  // q = const_param;           not allowed: param has unknown bounds
  // q_const = const_param;     not allowed: param has unknown bounds
  // q_volatile = const_param;  not allowed: param has unknown bounds
  r = const_param;              // expected-error {{No use of parameter have an unchecked array type}} expected-no-warning {{discards qualifiers}}
  r_const = const_param;        // expected-error {{No use of parameter have an unchecked array type}}
  r_volatile = const_param;     // expected-error {{No use of parameter have an unchecked array type}} expected-no-warning {{discards qualifiers}}

  // the rhs is an checked array where the element type has modifiers
  q = const_checked_param;      // expected-correct {{incompatible type}}, expected-warning {{discards qualifiers}}
                                // ptr<T> = array_ptr<T> OK
  r = const_checked_param;      // expected-warning {{discards qualifiers}}
  r_const = const_checked_param;
  r_volatile = const_checked_param; // expected-warning {{discards qualifiers}}

  //
  // the rhs is a local array
  //

  // the rhs is an unchecked array where the element type does not have modifiers
  p = a;            // expected-error {{No use of variable have an unchecked pointer type}} expected-error {{No use of variable have an unchecked array type}}
  p_const = a;      // expected-error {{No use of variable have an unchecked pointer type}} expected-error {{No use of variable have an unchecked array type}}
  p_volatile = a;   // expected-error {{No use of variable have an unchecked pointer type}} expected-error {{No use of variable have an unchecked array type}}
  q = a;            // expected-error {{No use of variable have an unchecked array type}}
  q_const = a;      // expected-error {{No use of variable have an unchecked array type}}
  q_volatile = a;   // expected-error {{No use of variable have an unchecked array type}}
  r = a;            // expected-error {{No use of variable have an unchecked array type}}
  r_const = a;      // expected-error {{No use of variable have an unchecked array type}}

  // the rhs is an checked array where the element type does not have modifiers
  p = b;              // expected-error {{No use of variable have an unchecked pointer type}} expected-no-error {{incompatible type}}
  p_const = b;        // expected-error {{No use of variable have an unchecked pointer type}} expected-no-error {{incompatible type}}
  p_volatile = b;     // expected-error {{No use of variable have an unchecked pointer type}} expected-no-error {{incompatible type}}
  q = b;              // expected-correct {{incompatible type}}, ptr<T> = array_ptr<T> OK
  q_const = b;        // expected-correct {{incompatible type}}, ptr<T> = array_ptr<T> OK
  q_volatile = b;     // expected-correct {{incompatible type}}, ptr<T> = array_ptr<T> OK
  r = b;
  r_const = b;
  r_volatile = b;

  // the rhs is an unchecked array where the element type has modifiers
  p = const_a;              // expected-error {{No use of variable have an unchecked pointer type}} expected-error {{No use of variable have an unchecked array type}} expected-no-warning {{discards qualifiers}}
  p_const = const_a;        // expected-error {{No use of variable have an unchecked pointer type}} expected-error {{No use of variable have an unchecked array type}}
  p_volatile = const_a;     // expected-error {{No use of variable have an unchecked pointer type}} expected-error {{No use of variable have an unchecked array type}} expected-no-warning {{discards qualifiers}}
  q = const_a;              // expected-error {{No use of variable have an unchecked array type}} expected-no-warning {{discards qualifiers}}
  q_const = const_a;        // expected-error {{No use of variable have an unchecked array type}}
  q_volatile = const_a;     // expected-error {{No use of variable have an unchecked array type}} expected-no-warning {{discards qualifiers}}
  r = const_a;              // expected-error {{No use of variable have an unchecked array type}} expected-no-warning {{discards qualifiers}}
  r_const = const_a;        // expected-error {{No use of variable have an unchecked array type}}
  r_volatile = const_a;     // expected-error {{No use of variable have an unchecked array type}} expected-no-warning {{discards qualifiers}}

  // the rhs is an checked array where the element type has modifiers
  q = const_b;      // expected-correct {{incompatible type}} expected-warning {{discards qualifiers}}
                    // ptr<T> = array_ptr<T> OK
  r = const_b;      // expected-warning {{discards qualifiers}}
  r_const = const_b;
  r_volatile = const_b; // expected-warning {{discards qualifiers}}
}

// Test conditional expressions where arms have different
// kinds of checked and unchecked arrays.
extern void check_condexpr(int val) checked {
  int p [5];        // expected-error {{variable cannot have an unchecked array type}}
  int r checked[5];
  float s[5];       // expected-error {{variable cannot have an unchecked array type}}
  float u checked[5];

  int *t1 = val ? p : p;            // T[5] and T[5] OK, expected-error {{variable cannot have an unchecked pointer type}} expected-error 2 {{No use of variable have an unchecked array type}}
  array_ptr<int> t2 = val ? p : r;  // T[5] and T checked[5] OK, expected-error {{No use of variable have an unchecked array type}}
  array_ptr<int> t3 = val ? r : p;  // T checked[5] and T[5] OK, expected-error {{No use of variable have an unchecked array type}}
  array_ptr<int> t4 = val ? r : r;  // T checked[5] and T checked[5] OK.

  // omit assignment because type of expression is not valid when there is an error.
  val ? s : r;     // expected-error {{No use of variable have an unchecked array type}} expected-no-error {{pointer type mismatch}}
                   // S[5] and T checked[5] not OK;
  val ? r : s;     // expected-error {{No use of variable have an unchecked array type}} expected-no-error {{pointer type mismatch}}
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
  array_ptr<int> t6 = val ? 0 : r;
  array_ptr<float> t7 = val ? u : 0;
  array_ptr<float> t8 = val ? 0 : u;
}

extern void check_condexpr_2d(int val) checked {
  int p[5][6];    // expected-error {{variable cannot have an unchecked array type}}
  int r checked[5][6];
  float s[5][6];  // expected-error {{variable cannot have an unchecked array type}}
  float u checked[5][6];
  int y checked[5][20];

  int (*t1)[6] = val ? p : p;                  // T[5][6] and T[5][6] OK, expected-error {{variable cannot have an unchecked pointer type}} expected-error 2 {{No use of variable have an unchecked array type}}
  array_ptr<int checked[6]> t2 = val ? p : r;  // T[5][6] and T checked[5][6] OK, expected-error {{No use of variable have an unchecked array type}}
  array_ptr<int checked[6]> t3 = val ? r : p;  // T checked[5][6] and T[5][6] OK, expected-error {{No use of variable have an unchecked array type}}
  array_ptr<int checked[6]> t4 = val ? r : r;  // T checked[5][6] and T checked[5][6] OK.

  array_ptr<int [6]> t5 = val ? p : r;  // expected-error {{No use of variable have an unchecked array type}} expected-no-error {{incompatible type}}
                                        // T[5][6] and T checked[5][6] produce a checked array
  array_ptr<int [6]> t6 = val ? r : p;  // expected-error {{No use of variable have an unchecked array type}} expected-no-error {{incompatible type}}
                                        // T checked[5][6] and T[5][6] produce a checked array
  array_ptr<int [6]> t7 = val ? r : r;  // expected-error {{incompatible type}}
                                        // T checked[5][6] and T checked[5][6] produce a checked array

  // omit assignment because type of expression is not valid when there is an error.
  val ? s : r;     // expected-error {{No use of variable have an unchecked array type}} expected-no-error {{pointer type mismatch}}
                    // S[5][6] and T checked[5][6] not OK;
  val ? r : s;     // expected-error {{No use of variable have an unchecked array type}} expected-no-error {{pointer type mismatch}}
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

  // Implicit conversion of 0 to a checked pointer type is OK.
  array_ptr<int checked[6]> t11 = val ? r : 0;
  array_ptr<int checked[6]> t12 = val ? 0 : r;
  array_ptr<float checked[6]> t15 = val ? u : 0;
  array_ptr<float checked[6]> t16 = val ? 0 : u;
}

// Test conditional expressions where arms have different kinds of
// array types and const/volatile modifiers.
extern void check_condexpr_cv(void) checked {
  int val = 0;
  int p[5];   // expected-error {{variable cannot have an unchecked array type}}
  const int p_const[5] = { 0, 1, 2, 3, 4};    // expected-error {{variable cannot have an unchecked array type}}
  volatile int p_volatile[5];   // expected-error {{variable cannot have an unchecked array type}}
  int r checked[5];
  const int r_const checked[5] = { 0, 1, 2, 3, 4};
  volatile int r_volatile[5];   // expected-error {{variable cannot have an unchecked array type}}

  // test different kinds of pointers with const modifiers
  const int *t1 = val ? p : p_const;      // expected-error {{variable cannot have an unchecked pointer type}} expected-error 2 {{No use of variable have an unchecked array type}}
                                          // int * and const int * OK
  const int *t2 = val ? p_const : p;      // expected-error {{variable cannot have an unchecked pointer type}} expected-error 2 {{No use of variable have an unchecked array type}}
                                          // const int * and int * OK
  const int *t3 = val ? p_const : p_const;// expected-error {{variable cannot have an unchecked pointer type}} expected-error 2 {{No use of variable have an unchecked array type}}
                                          // const int * and const int * OK

  int *t4 = val ? p : p_const;            // expected-error {{variable cannot have an unchecked pointer type}} expected-error 2 {{No use of variable have an unchecked array type}}, expected-no-warning {{discards qualifiers}}
                                          // int * and const int * produce const int *
  int *t5 = val ? p_const : p;            // expected-error {{variable cannot have an unchecked pointer type}} expected-error 2 {{No use of variable have an unchecked array type}}, expected-no-warning {{discards qualifiers}}
                                          // const int * and int * produce const int *
  int *t6 = val ? p_const : p_const;      // expected-error {{variable cannot have an unchecked pointer type}} expected-error 2 {{No use of variable have an unchecked array type}}, expected-no-warning {{discards qualifiers}}
                                          // const int * and const int * produce const int *

  array_ptr<const int> t25 = val ? p : r_const;       // expected-error {{No use of variable have an unchecked array type}}
                                                      // int * and array_ptr<const int> OK
  array_ptr<const int> t26 = val ? r_const : p;       // expected-error {{No use of variable have an unchecked array type}}
                                                      // array_ptr<const int> and int * OK
  array_ptr<const int> t27 = val ? p_const : r;       // expected-error {{No use of variable have an unchecked array type}}
                                                      // const int * and array_ptr<int> OK
  array_ptr<const int> t28 = val ? r : p_const;       // expected-error {{No use of variable have an unchecked array type}}
                                                      // array_ptr<int> and const int * OK
  array_ptr<const int> t29 = val ? p_const : r_const; // expected-error {{No use of variable have an unchecked array type}}
                                                      // const int * and array_ptr<const int> OK
  array_ptr<const int> t30 = val ? r_const : p_const; // expected-error {{No use of variable have an unchecked array type}}
                                                      // array_ptr<const int> and const int * OK
  array_ptr<const int> t31 = val ? r : r_const;       // array_ptr<int> and array_ptr<const int> OK
  array_ptr<const int> t32 = val ? r_const : r;       // array_ptr<const int> and array_ptr<int> OK
  array_ptr<const int> t33 = val ? r_const : r_const; // array_ptr<const int> and array_ptr<const int> OK

  array_ptr<int> t34 = val ? p : r_const;         // expected-error {{No use of variable have an unchecked array type}} expected-no-warning {{discards qualifiers}}
                                                  // int * and array_ptr<const int> produce array_ptr<const int>
  array_ptr<int> t35 = val ? r_const : p;         // expected-error {{No use of variable have an unchecked array type}} expected-no-warning {{discards qualifiers}}
                                                  // array_ptr<const int> and int * produce array_ptr<const int>
  array_ptr<int> t36 = val ? p_const : r;         // expected-error {{No use of variable have an unchecked array type}} expected-no-warning {{discards qualifiers}}
                                                  // const int * and array_ptr<int> produce array_ptr<const int>
  array_ptr<int> t37 = val ? r : p_const;         // expected-error {{No use of variable have an unchecked array type}} expected-no-warning {{discards qualifiers}}
                                                  // array_ptr<int> and const int * produce array_ptr<const int>
  array_ptr<int> t38 = val ? p_const : r_const;   // expected-error {{No use of variable have an unchecked array type}} expected-no-warning {{discards qualifiers}}
                                                  // const int * and array_ptr<const int> produce array_ptr<const int>
  array_ptr<int> t39 = val ? r_const : p_const;   // expected-error {{No use of variable have an unchecked array type}} expected-no-warning {{discards qualifiers}}
                                                  // array_ptr<const int> and const int * produce array_ptr<const int>
  array_ptr<int> t40 = val ? r : r_const;         // expected-warning {{discards qualifiers}}
                                                  // array_ptr<int> and array_ptr<const int> produce array_ptr<const int>
  array_ptr<int> t41 = val ? r_const : r;         // expected-warning {{discards qualifiers}}
                                                  // array_ptr<const int> and array_ptr<int> produce array_ptr<const int>
  array_ptr<int> t42 = val ? r_const : r_const;   // expected-warning {{discards qualifiers}}
                                                  // array_ptr<const int> and array_ptr<const int> produce array_ptr<const int>

  // test different kinds of pointers with volatile modifers
  volatile int *t50 = val ? p : p_volatile;           // expected-error {{variable cannot have an unchecked pointer type}} expected-error 2 {{No use of variable have an unchecked array type}}
                                                      // int * and volatile int * OK
  volatile int *t51 = val ? p_volatile : p;           // expected-error {{variable cannot have an unchecked pointer type}} expected-error 2 {{No use of variable have an unchecked array type}} 
                                                      // volatile int * and int * OK
  volatile int *t52 = val ? p_volatile : p_volatile;  // expected-error {{variable cannot have an unchecked pointer type}} expected-error 2 {{No use of variable have an unchecked array type}}
                                                      // volatile int * and volatile int * OK

  int *t53 = val ? p : p_volatile;                    // expected-error {{variable cannot have an unchecked pointer type}} expected-error 2 {{No use of variable have an unchecked array type}} expected-no-warning {{discards qualifiers}}
                                                      // int * and volatile int * produce volatile int *
  int *t54 = val ? p_volatile : p;                    // expected-error {{variable cannot have an unchecked pointer type}} expected-error 2 {{No use of variable have an unchecked array type}} expected-no-warning {{discards qualifiers}}
                                                      // volatile int * and int * produce volatile int *
  int *t55 = val ? p_volatile : p_volatile;           // expected-error {{variable cannot have an unchecked pointer type}} expected-error 2 {{No use of variable have an unchecked array type}} expected-no-warning {{discards qualifiers}}
                                                      // volatile int * and volatile int * produce volatile int *

  array_ptr<volatile int> t74 = val ? p : r_volatile;         // expected-error 2 {{No use of variable have an unchecked array type}}
                                                              // int * and array_ptr<volatile int> OK
  array_ptr<volatile int> t75 = val ? r_volatile : p;         // expected-error 2 {{No use of variable have an unchecked array type}}
                                                              // array_ptr<volatile int> and int * OK
  array_ptr<volatile int> t76 = val ? p_volatile : r;         // expected-error {{No use of variable have an unchecked array type}}
                                                              // volatile int * and array_ptr<int> OK
  array_ptr<volatile int> t77 = val ? r : p_volatile;         // expected-error {{No use of variable have an unchecked array type}}
                                                              // array_ptr<int> and volatile int * OK
  array_ptr<volatile int> t78 = val ? p_volatile : r_volatile;// expected-error 2 {{No use of variable have an unchecked array type}}
                                                              // volatile int * and array_ptr<volatile int> OK
  array_ptr<volatile int> t79 = val ? r_volatile : p_volatile;// expected-error 2 {{No use of variable have an unchecked array type}}
                                                              // array_ptr<volatile int> and volatile int * OK
  array_ptr<volatile int> t80 = val ? r : r_volatile;         // expected-error {{No use of variable have an unchecked array type}}
                                                              // array_ptr<int> and array_ptr<volatile int> OK
  array_ptr<volatile int> t81 = val ? r_volatile : r;         // expected-error {{No use of variable have an unchecked array type}}
                                                              // array_ptr<volatile int> and array_ptr<int> OK
  array_ptr<volatile int> t82 = val ? r_volatile : r_volatile;// expected-error 2 {{No use of variable have an unchecked array type}}
                                                              // array_ptr<volatile int> and array_ptr<volatile int> OK

  array_ptr<int> t83 = val ? p : r_volatile;          // expected-error 2 {{No use of variable have an unchecked array type}} expected-no-warning {{discards qualifiers}}
                                                      // int * and array_ptr<volatile int> produce array_ptr<volatile int>
  array_ptr<int> t84 = val ? r_volatile : p;          // expected-error 2 {{No use of variable have an unchecked array type}} expected-no-warning {{discards qualifiers}}
                                                      // array_ptr<volatile int> and int * produce array_ptr<volatile int>
  array_ptr<int> t85 = val ? p_volatile : r;          // expected-error {{No use of variable have an unchecked array type}} expected-no-warning {{discards qualifiers}}
                                                      // volatile int * and array_ptr<int> produce array_ptr<volatile int>
  array_ptr<int> t86 = val ? r : p_volatile;          // expected-error {{No use of variable have an unchecked array type}} expected-no-warning {{discards qualifiers}}
                                                      // array_ptr<int> and volatile int * produce array_ptr<volatile int>
  array_ptr<int> t87 = val ? p_volatile : r_volatile; // expected-error 2 {{No use of variable have an unchecked array type}} expected-no-warning {{discards qualifiers}}
                                                      // volatile int * and array_ptr<volatile int> produce array_ptr<volatile int>
  array_ptr<int> t88 = val ? r_volatile : p_volatile; // expected-error 2 {{No use of variable have an unchecked array type}} expected-no-warning {{discards qualifiers}}
                                                      // array_ptr<volatile int> and volatile int * produce array_ptr<volatile int>
  array_ptr<int> t89 = val ? r : r_volatile;          // expected-error {{No use of variable have an unchecked array type}} expected-no-warning {{discards qualifiers}}
                                                      // array_ptr<int> and array_ptr<volatile int> produce array_ptr<volatile int>
  array_ptr<int> t90 = val ? r_volatile : r;          // expected-error {{No use of variable have an unchecked array type}} expected-no-warning {{discards qualifiers}}
                                                      // array_ptr<volatile int> and array_ptr<int> produce array_ptr<volatile int>
  array_ptr<int> t92 = val ? r_volatile : r_volatile; // expected-error 2 {{No use of variable have an unchecked array type}} expected-no-warning {{discards qualifiers}}
                                                      // array_ptr<volatile int> and array_ptr<volatile int> produce array_ptr<volatile int>
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

extern void f5(int(*p)[10], int y) {
}

extern void f6(ptr<int[10]> p, int y) {
}

extern void f7(array_ptr<int[10]> p, int y) {
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

extern void check_call(void) checked {
  int x[10];            // expected-error {{variable cannot have an unchecked array type}};
  int y checked[10];
  int x2d[10][10];      // expected-error {{variable cannot have an unchecked array type}};
  int y2d checked[10][10];


  // checked scope error precedes function call type error/warning
  // f1(int *p, int y)
  f1(x, 0);               // expected-error {{No use of variable have an unchecked array type}}
  f1(y, 0);               // expected-error {{parameter of incompatible type 'int *'}}
  f1(x2d, 0);             // expected-error {{No use of variable have an unchecked array type}} expected-no-warning {{incompatible pointer types passing}}
  f1(y2d, 0);             // expected-error {{passing 'int checked[10][10]' to parameter of incompatible type 'int *'}}

  // f2(int p[10], int y)
  f2(x, 0);               // expected-error {{No use of variable have an unchecked array type}}
  f2(y, 0);               // expected-error {{parameter of incompatible type 'int *'}}
  f2(x2d, 0);             // expected-error {{No use of variable have an unchecked array type}} expected-no-warning {{incompatible pointer types passing}}
  f2(y2d, 0);             // expected-error {{passing 'int checked[10][10]' to parameter of incompatible type 'int *'}}

  // f3(int p checked[10], int y)
  f3(x, 0);               // expected-error {{No use of variable have an unchecked array type}}
  f3(y, 0);
  f3(x2d, 0);             // expected-error {{No use of variable have an unchecked array type}} expected-no-error {{parameter of incompatible type}}
  f3(y2d, 0);             // expected-error {{passing 'int checked[10][10]' to parameter of incompatible type '_Array_ptr<int>'}}

  // f4(int **p, int y);
  f4(x, 0);               // expected-error {{No use of variable have an unchecked array type}} expected-no-warning {{incompatible pointer types passing}}
  f4(y, 0);               // expected-error {{passing 'int checked[10]' to parameter of incompatible type 'int **'}}
  f4(x2d, 0);             // expected-error {{No use of variable have an unchecked array type}} expected-no-warning {{incompatible pointer types passing}}
  f4(y2d, 0);             // expected-error {{passing 'int checked[10][10]' to parameter of incompatible type 'int **'}}

  // f5(int (*p)[10], int y);
  f5(x, 0);               // expected-error {{No use of variable have an unchecked array type}} expected-no-warning {{incompatible pointer types passing}}
  f5(y, 0);               // expected-error {{passing 'int checked[10]' to parameter of incompatible type 'int (*)[10]'}}
  f5(x2d, 0);             // expected-error {{No use of variable have an unchecked array type}}
  f5(y2d, 0);             // expected-error {{passing 'int checked[10][10]' to parameter of incompatible type 'int (*)[10]'}}

   // f6(ptr<int[10]>, int y);
  f6(x, 0);               // expected-error {{No use of variable have an unchecked array type}} expected-no-error {{parameter of incompatible type}}
  f6(y, 0);               // expected-error {{passing 'int checked[10]' to parameter of incompatible type '_Ptr<int [10]>'}}
  f6(x2d, 0);             // expected-error {{No use of variable have an unchecked array type}}
  f6(y2d, 0);             // expected-error {{passing 'int checked[10][10]' to parameter of incompatible type '_Ptr<int [10]>'}}

   // f7(array_ptr<int[10]>, int y);
  f7(x, 0);               // expected-error {{No use of variable have an unchecked array type}} expected-no-error {{parameter of incompatible type}}
  f7(y, 0);               // expected-error {{parameter of incompatible type}}
  f7(x2d, 0);             // expected-error {{No use of variable have an unchecked array type}}
  f7(y2d, 0);             // expected-error {{parameter of incompatible type}}

  // f8(int (*p) checked[10], int y);
  f8(x, 0);               // expected-error {{No use of variable have an unchecked array type}} expected-no-error {{parameter of incompatible type}}
  f8(y, 0);               // expected-error {{parameter of incompatible type}}
  f8(x2d, 0);             // expected-error {{No use of variable have an unchecked array type}}
  f8(y2d, 0);             // expected-error {{parameter of incompatible type}}

  // f9(ptr<int checked[10]> p, int y);
  f8(x, 0);               // expected-error {{No use of variable have an unchecked array type}} expected-no-error {{parameter of incompatible type}}
  f8(y, 0);               // expected-error {{parameter of incompatible type}}
  f8(x2d, 0);             // expected-error {{No use of variable have an unchecked array type}}
  f8(y2d, 0);             // expected-error {{parameter of incompatible type}}

  // f10(array_ptr<int checked[10]> p, int y);
  f10(x, 0);               // expected-error {{No use of variable have an unchecked array type}} expected-no-error {{parameter of incompatible type}}
  f10(y, 0);               // expected-error {{parameter of incompatible type}}
  f10(x2d, 0);             // expected-error {{No use of variable have an unchecked array type}}
  f10(y2d, 0);             // OK

  // f11(int p[10][10], int y);
  f11(x, 0);              // expected-error {{No use of variable have an unchecked array type}} expected-no-warning {{incompatible pointer types}}
  f11(y, 0);              // expected-error {{parameter of incompatible type}}
  f11(x2d, 0);            // expected-error {{No use of variable have an unchecked array type}}
  f11(y2d, 0);            // expected-error {{parameter of incompatible type}}

  // f12(int p checked[10][10], int y);
  f12(x, 0);              // expected-error {{No use of variable have an unchecked array type}} expected-no-error {{parameter of incompatible type}}
  f12(y, 0);              // expected-error {{parameter of incompatible type}}
  f12(x2d, 0);            // expected-error {{No use of variable have an unchecked array type}}
  f12(y2d, 0);            // OK

  // f13(_Bool b, int y);
  f13(x, 0);              // expected-error {{No use of variable have an unchecked array type}}
  f13(y, 0);              // OK
  f13(x2d, 0);            // expected-error {{No use of variable have an unchecked array type}}
  f13(y2d, 0);            // OK

  // spot check calls where an array is the second argument
  g1(0, x);   // expected-error {{No use of variable have an unchecked array type}}
  g1(0, y);   // expected-error {{parameter of incompatible type}}
  g2(0, x);   // expected-error {{No use of variable have an unchecked array type}}
  g2(0, y);   // expected-error {{parameter of incompatible type}}
  g3(0, x);   // expected-error {{No use of variable have an unchecked array type}}
  g3(0, y);

}

extern void check_call_void(void) checked {
  int val = 0;
  int p[10];        // expected-error {{variable cannot have an unchecked array type}}
  int r checked[10];

  // TODO: s will need bounds information
  void *s = 0;      // expected-error {{variable cannot have an unchecked pointer type}}
  ptr<void> t = 0;
  array_ptr<void> u = 0;

  // Test different kinds of pointers where the parameter type is a pointer to void and
  // the referent type is not a void pointer.

  // Type of first parameter is a pointer type.
  // Expected to typecheck
  f1_void(p, val);    // expected-error {{No use of variable have an unchecked array type}} param ptr<void>, arg int[10] OK.
  f3_void(r, val);    // param array_ptr<void>, arg int checked[10] OK.
  f3_void(p, val);    // expected-error {{No use of variable have an unchecked array type}} param array_ptr<void>, arg int[10] OK, provided that param has no bounds.

  // Expected to not typecheck
  f1_void(r, val);    // expected-error {{incompatible type}}
                      // param void *, arg int checked[10] not OK
  f2_void(r, val);    // expected-error {{incompatible type}}
                      // param ptr<void>, arg int checked[10] not OK

  // Try passing void pointers to functions expected array types
  // f1(int *, int)
  f1(s, 0);           // expected-error {{No use of variable have an unchecked pointer type}}
  f1(t, 0);           // expected-error {{incompatible type}}
  f1(u, 0);           // expected-error {{incompatible type}}

  // f2(int p[10], int)
  f2(s, 0);           // expected-error {{No use of variable have an unchecked pointer type}}
  f2(t, 0);           // expected-error {{incompatible type}}
  f2(u, 0);           // expected-error {{incompatible type}}

  // f3(int p checked[10], int)
  f3(s, 0);           // expected-error {{No use of variable have an unchecked pointer type}}
  f3(t, 0);           // expected-error {{incompatible type}}
  f3(u, 0);           // expected-error {{incompatible type}}
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
  return global;    // expected-error {{No use of variable have an unchecked array type}}
}

checked ptr<int> h4(void) {
  return global;    // expected-error {{No use of variable have an unchecked array type}}
}

checked array_ptr<int> h5(void) {
  return global;    // expected-error {{No use of variable have an unchecked array type}}
}

checked int *h6(void) {   // expected-error {{return cannot have an unchecked pointer type}}
  return checked_global;  // expected-error {{incompatible result type}}
}

checked ptr<int> h7(void) {
  return checked_global; // expected-no-error {{incompatible result type}}, ptr<T> = array_ptr<T> OK
}

checked array_ptr<int> h8(void) {
  return checked_global;
}

checked int *h9(int arr[10]) {    // expected-error {{return cannot have an unchecked pointer type}} expected-error {{parameter cannot have an unchecked array type}}
  return arr;   // expected-error {{No use of parameter have an unchecked array type}}
}

checked ptr<int> h10(void) {
  return global_arr1;   // expected-error {{No use of variable have an unchecked array type}}
}

checked array_ptr<int> h11(int arr checked[10]) {
  return arr;
}

checked int *h12(int arr checked[10]) {   // expected-error {{return cannot have an unchecked pointer type}}
  return arr;  // expected-error {{incompatible result type}}
}

checked ptr<int> h13(int arr checked[10]) {
  return arr;  // expected-no-error {{incompatible result type}}, ptr<T> = array_ptr<T> OK
}

checked array_ptr<int> h14(int arr checked[10]) {
  return arr;
}

checked int *h15(int arr checked[]) {   // expected-error {{return cannot have an unchecked pointer type}}
  return arr;  // expected-error {{incompatible result type}}
}

checked ptr<int> h17(int arr checked[]) {
  return arr;  // expected-no-error {{incompatible result type}}, ptr<T> = array_ptr<T> OK expected-error {{expression has no bounds}}
}

checked array_ptr<int> h18(int arr checked[]) {
  return arr;
}

// h19 is a function that returns a pointer to a 10-element array of integers.
checked int(*h19(int arr[10][10]))[10]{   // expected-error {{return cannot have an unchecked pointer type}} expected-error {{parameter cannot have an unchecked array type}}
  return arr;   // expected-error {{No use of parameter have an unchecked array type}}
}

int global_arr2[10][10];
checked ptr<int[10]> h20(void) {
  return global_arr2; // expected-error {{No use of variable have an unchecked array type}}
}

checked array_ptr<int[10]> h21(int arr[10][10]) { // expected-error {{parameter cannot have an unchecked array type}}
  return arr; // expected-error {{No use of parameter have an unchecked array type}}
}

// h22 is a function that returns a pointer to a 10-element array of integers.
checked int (*h22(int arr checked[10][10]))[10] { // expected-error {{return cannot have an unchecked pointer type}}
  return arr;  // expected-error {{incompatible result type}}
}

checked ptr<int[10]> h23(int arr checked[10][10]) {
  return arr;  // expected-error {{incompatible result type}}
}

checked array_ptr<int[10]> h24(int arr checked[10][10]) {
  return arr;  // expected-error {{incompatible result type}}
}

// h25 is a function that returns a pointer to 10-element array of integers.
checked int (*h25(int arr checked[10][10])) checked[10]{
  return arr;  // expected-error {{incompatible result type}}
}

checked ptr<int checked[10]> h26(int arr checked[10][10]) {
  return arr;  // expected-no-error {{incompatible result type}}, ptr<T> = array_ptr<T> OK
}

checked array_ptr<int checked[10]> h27(int arr checked[10][10]) {
  return arr;
}



checked void check_pointer_arithmetic(void) {
  int p[5];           // expected-error {{variable cannot have an unchecked array type}}
  int r checked[5];

  int *p_tmp;         // expected-error {{variable cannot have an unchecked pointer type}}
  array_ptr<int> r_tmp;

  p_tmp = p + 5;      // expected-error {{No use of variable have an unchecked array type}} expected-error {{No use of variable have an unchecked pointer type}}
  p_tmp = 5 + p;      // expected-error {{No use of variable have an unchecked array type}} expected-error {{No use of variable have an unchecked pointer type}}
  p_tmp = p_tmp - 2;  // expected-error 2 {{No use of variable have an unchecked pointer type}}
  p_tmp = 2 - p;      // expected-error {{No use of variable have an unchecked array type in a checked scope}} expected-error {{No use of variable have an unchecked pointer type}} expected-no-error {{invalid operands}}
  p_tmp = p++;        // expected-error {{No use of variable have an unchecked array type in a checked scope}} expected-error {{No use of variable have an unchecked pointer type}} expected-no-error {{cannot increment value}}
  p_tmp = p--;        // expected-error {{No use of variable have an unchecked array type in a checked scope}} expected-error {{No use of variable have an unchecked pointer type}} expected-no-error {{cannot decrement value}}
  p_tmp = ++p;        // expected-error {{No use of variable have an unchecked array type in a checked scope}} expected-error {{No use of variable have an unchecked pointer type}} expected-no-error {{cannot increment value}}
  p_tmp = --p;        // expected-error {{No use of variable have an unchecked array type in a checked scope}} expected-error {{No use of variable have an unchecked pointer type}} expected-no-error {{cannot decrement value}}
  p_tmp = (p += 1);   // expected-error {{No use of variable have an unchecked array type in a checked scope}} expected-error {{No use of variable have an unchecked pointer type}} expected-no-error {{invalid operands}}
  p_tmp = (p -= 1);   // expected-error {{No use of variable have an unchecked array type in a checked scope}} expected-error {{No use of variable have an unchecked pointer type}} expected-no-error {{invalid operands}}

  // 0 interpreted as an integer, not null
  p_tmp = p + 0;  // expected-error {{No use of variable have an unchecked array type}} expected-error {{No use of variable have an unchecked pointer type}}
  p_tmp = 0 + p;  // expected-error {{No use of variable have an unchecked array type}} expected-error {{No use of variable have an unchecked pointer type}}
  p_tmp = p - 0;  // expected-error {{No use of variable have an unchecked array type}} expected-error {{No use of variable have an unchecked pointer type}}
  p_tmp = 0 - p;  // expected-error {{No use of variable have an unchecked array type}} expected-error {{No use of variable have an unchecked pointer type}} expected-no-error {{invalid operands to binary expression}}

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
}

checked void check_pointer_difference(int flag) {
  int count;
  int val_int[5];                 // expected-error {{variable cannot have an unchecked array type}}
  float val_float[5];             // expected-error {{variable cannot have an unchecked array type}}
  int *p_int = val_int;           // expected-error {{variable cannot have an unchecked pointer type}} expected-error {{No use of variable have an unchecked array type}}
  float *p_float = val_float;     // expected-error {{variable cannot have an unchecked pointer type}} expected-error {{No use of variable have an unchecked array type}}
  array_ptr<int> r_int = val_int; // expected-error {{No use of variable have an unchecked array type}}

  int a_int[5];                   // expected-error {{variable cannot have an unchecked array type}}
  int checked_a_int checked[5];

  float a_float[5];               // expected-error {{variable cannot have an unchecked array type}}
  float checked_a_float checked[5];

  if (flag) {
      p_int = a_int;              // expected-error {{No use of variable have an unchecked array type}} expected-error {{No use of variable have an unchecked pointer type}}
      r_int = checked_a_int;
  }

  // pointer - array
  count = p_int - a_int;          // expected-error {{No use of variable have an unchecked array type}} expected-error {{No use of variable have an unchecked pointer type}}
  count = p_int - checked_a_int;  // expected-error {{No use of variable have an unchecked pointer type}}
  count = r_int - a_int;          // expected-error {{No use of variable have an unchecked array type}}
  count = r_int - checked_a_int;

  // array - pointer
  count = a_int - p_int;            // expected-error {{No use of variable have an unchecked pointer type}} expected-error {{No use of variable have an unchecked array type}}
  count = checked_a_int - p_int;    // expected-error {{No use of variable have an unchecked pointer type}}
  count = a_int - r_int;            // expected-error {{No use of variable have an unchecked array type}}
  count = checked_a_int - r_int;

  // spot check mismatched types
  count = a_float - p_int;          // expected-error {{No use of variable have an unchecked pointer type}} expected-error {{No use of variable have an unchecked array type}} expected-no-error {{not pointers to compatible types}}
  count = p_int - checked_a_float;  // expected-error {{No use of variable have an unchecked pointer type}} expected-no-error {{not pointers to compatible types}}
  count = checked_a_float - r_int;  // expected-error {{not pointers to compatible types}}
}

checked void check_pointer_relational_compare(void) {
  int result;

  int val_int[5];                 // expected-error {{variable cannot have an unchecked array type}}
  float val_float[5];             // expected-error {{variable cannot have an unchecked array type}}
  int checked_val_int checked[5];
  float checked_val_float checked[5];

  int *p_int = val_int;           // expected-error {{variable cannot have an unchecked pointer type}} expected-error {{No use of variable have an unchecked array type}}
  float *p_float = val_float;     // expected-error {{variable cannot have an unchecked pointer type}} expected-error {{No use of variable have an unchecked array type}}

  ptr<int> q_int = 0;
  q_int = val_int;                // expected-error {{No use of variable have an unchecked array type}}
  ptr<float> q_float = 0;
  q_float = val_float;            // expected-error {{No use of variable have an unchecked array type}}

  array_ptr<int> r_int = val_int;         // expected-error {{No use of variable have an unchecked array type}}
  array_ptr<float> r_float = val_float;   // expected-error {{No use of variable have an unchecked array type}}

  // relational comparisons between pointers and unchecked arrays;
  result = val_int < p_int;           // expected-error {{No use of variable have an unchecked array type}} expected-error {{No use of variable have an unchecked pointer type}}
  result = val_int <= q_int;          // expected-error {{No use of variable have an unchecked array type}}
  result = val_int >= r_int;          // expected-error {{No use of variable have an unchecked array type}}

  result = p_int > val_int;           // expected-error {{No use of variable have an unchecked array type}} expected-error {{No use of variable have an unchecked pointer type}}
  result = q_int < val_int;           // expected-error {{No use of variable have an unchecked array type}}
  result = r_int <= val_int;          // expected-error {{No use of variable have an unchecked array type}}


  // relational comparisons between pointers and checked arrays;
  result = checked_val_int < p_int;   // expected-error {{No use of variable have an unchecked pointer type}}
  result = checked_val_int <= q_int;
  result = checked_val_int >= r_int;

  result = p_int > checked_val_int;   // expected-error {{No use of variable have an unchecked pointer type}}
  result = q_int < checked_val_int;
  result = r_int <= checked_val_int;

  // invalid relational comparisons

  // spot check comparisons between pointers and unchecked arrays;
  result = val_int < p_float;  // expected-error {{No use of variable have an unchecked array type}} expected-error {{No use of variable have an unchecked pointer type}} expected-no-warning {{comparison of distinct pointer types}}
  result = val_float <= q_int; // expected-error {{No use of variable have an unchecked array type}} expected-no-warning {{comparison of distinct pointer types}}
  result = val_int >= r_float; // expected-error {{No use of variable have an unchecked array type}} expected-no-warning {{comparison of distinct pointer types}}

  result = p_int > val_float;  // expected-error {{No use of variable have an unchecked array type}} expected-error {{No use of variable have an unchecked pointer type}} expected-no-warning {{comparison of distinct pointer types}}
  result = q_float < val_int;  // expected-error {{No use of variable have an unchecked array type}} expected-no-warning {{comparison of distinct pointer types}}
  result = r_int <= val_float; // expected-error {{No use of variable have an unchecked array type}} expected-no-warning {{comparison of distinct pointer types}}

  // spot check comparisons between pointers and checked arrays;
  result = checked_val_int < p_float;  // expected-error {{No use of variable have an unchecked pointer type}} expected-no-warning {{comparison of distinct pointer types}}
  result = checked_val_float <= q_int; // expected-warning {{comparison of distinct pointer types}}
  result = checked_val_int >= r_float; // expected-warning {{comparison of distinct pointer types}}

  result = p_int > checked_val_float;  // expected-error {{No use of variable have an unchecked pointer type}} expected-no-warning {{comparison of distinct pointer types}}
  result = q_float < checked_val_int;  // expected-warning {{comparison of distinct pointer types}}
  result = r_int <= checked_val_float; // expected-warning {{comparison of distinct pointer types}}
}

checked void check_pointer_equality_compare(void) {
  int result;

  int val_int[5];                 // expected-error {{variable cannot have an unchecked array type}}
  float val_float[5];             // expected-error {{variable cannot have an unchecked array type}}
  int checked_val_int checked[5];
  float checked_val_float checked[5];

  int *p_int = val_int;           // expected-error {{variable cannot have an unchecked pointer type}} expected-error {{No use of variable have an unchecked array type}}
  float *p_float = val_float;     // expected-error {{variable cannot have an unchecked pointer type}} expected-error {{No use of variable have an unchecked array type}}

  ptr<int> q_int = 0;
  q_int = val_int;                // expected-error {{No use of variable have an unchecked array type}}
  ptr<float> q_float = 0;
  q_float = val_float;            // expected-error {{No use of variable have an unchecked array type}}

  array_ptr<int> r_int = val_int;         // expected-error {{No use of variable have an unchecked array type}}
  array_ptr<float> r_float = val_float;   // expected-error {{No use of variable have an unchecked array type}}

  // equality/inequality comparisons between pointers and unchecked arrays;
  result = val_int == p_int;    // expected-error {{No use of variable have an unchecked array type}} expected-error {{No use of variable have an unchecked pointer type}}
  result = val_int != q_int;    // expected-error {{No use of variable have an unchecked array type}}
  result = val_int == r_int;    // expected-error {{No use of variable have an unchecked array type}}

  result = p_int != val_int;    // expected-error {{No use of variable have an unchecked array type}} expected-error {{No use of variable have an unchecked pointer type}}
  result = q_int == val_int;    // expected-error {{No use of variable have an unchecked array type}}
  result = r_int != val_int;    // expected-error {{No use of variable have an unchecked array type}}

  // equality/inequality comparisons between pointers and checked arrays;
  result = checked_val_int == p_int;    // expected-error {{No use of variable have an unchecked pointer type}}
  result = checked_val_int != q_int;
  result = checked_val_int == r_int;

  result = p_int != checked_val_int;    // expected-error {{No use of variable have an unchecked pointer type}}
  result = q_int == checked_val_int;
  result = r_int != checked_val_int;

  // invalid equality/inequality comparisons

  // spot check equality comparisons between pointers and unchecked arrays;
  result = val_int == p_float;  // expected-error {{No use of variable have an unchecked array type}} expected-error {{No use of variable have an unchecked pointer type}} expected-no-warning {{comparison of distinct pointer types}}
  result = val_float != q_int;  // expected-error {{No use of variable have an unchecked array type}} expected-no-warning {{comparison of distinct pointer types}}
  result = val_int == r_float;  // expected-error {{No use of variable have an unchecked array type}} expected-no-warning {{comparison of distinct pointer types}}

  result = p_int != val_float;  // expected-error {{No use of variable have an unchecked array type}} expected-error {{No use of variable have an unchecked pointer type}} expected-no-warning {{comparison of distinct pointer types}}
  result = q_float == val_int;  // expected-error {{No use of variable have an unchecked array type}} expected-no-warning {{comparison of distinct pointer types}}
  result = r_int != val_float;  // expected-error {{No use of variable have an unchecked array type}} expected-no-warning {{comparison of distinct pointer types}}

  // spot check equality comparisons between pointers and checked arrays;
  result = checked_val_int == p_float;  // expected-error {{No use of variable have an unchecked pointer type}} expected-no-warning {{comparison of distinct pointer types}}
  result = checked_val_float != q_int;  // expected-warning {{comparison of distinct pointer types}}
  result = checked_val_int != r_float;  // expected-warning {{comparison of distinct pointer types}}

  result = p_int == checked_val_float;  // expected-error {{No use of variable have an unchecked pointer type}} expected-no-warning {{comparison of distinct pointer types}}
  result = q_float != checked_val_int;  // expected-warning {{comparison of distinct pointer types}}
  result = r_int == checked_val_float;  // expected-warning {{comparison of distinct pointer types}}
}

checked void check_logical_operators(void) {
  int p[5]; // expected-error {{variable cannot have an unchecked array type}}
  int r checked[5];

  _Bool b;

  b = !p;     // expected-error {{No use of variable have an unchecked array type}}
  b = !r;

  b = p || b; // expected-error {{No use of variable have an unchecked array type}}
  b = r || b;
  b = b || p; // expected-error {{No use of variable have an unchecked array type}}
  b = b || r;

  b = r || p; // expected-error {{No use of variable have an unchecked array type}}
  b = p || r; // expected-error {{No use of variable have an unchecked array type}}

  b = p && b; // expected-error {{No use of variable have an unchecked array type}}
  b = r && b;
  b = b && p; // expected-error {{No use of variable have an unchecked array type}}
  b = b && r;

  b = r && p; // expected-error {{No use of variable have an unchecked array type}}
  b = p && r; // expected-error {{No use of variable have an unchecked array type}}
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
  parr = (int(*)checked[5]) &arr;
  parr = (int(*)checked[5]) ((int(*)checked[]) &arr);
  parr = (int(*)checked[3]) &arr; // expected-error {{incompatible type}}
  parr = (int(*)[5]) &arr; // expected-error {{invalid casting to unchecked pointer type}}

  // ptr to array
  parr = (ptr<int checked[5]>) &arr;
  parr = (ptr<int checked[5]>) ((ptr<int checked[]>) &arr);
  parr = (ptr<int checked[3]>) &arr; // expected-error {{incompatible type}}
  parr = (ptr<int [5]>) &arr;

  // array_ptr to array
  array_ptr<int checked[5]> aparr = 0;
  aparr = (array_ptr<int checked[5]>) &arr;
  aparr = (array_ptr<int checked[5]>) ((array_ptr<int checked[]>) &arr);
  aparr = (array_ptr<int checked[3]>) &arr; // expected-error {{incompatible type}}
  aparr = (array_ptr<int [5]>) &arr;

  int *upa; // expected-error {{variable cannot have an unchecked pointer type}}
  upa = arr; // expected-error {{No use of variable have an unchecked pointer type}}
  upa = (int *)(array_ptr<int>)arr; // expected-error {{No use of variable have an unchecked pointer type}} expected-error {{invalid casting to unchecked pointer type in a checked scope}}
  upa = &x; // expected-error {{No use of variable have an unchecked pointer type}}
  upa = (int [])&x; // expected-error {{No use of variable have an unchecked pointer type}} expected-error {{cast to incomplete type}}
  upa = (int(*)[5])&x; // expected-error {{No use of variable}} expected-error {{invalid casting to unchecked}}

  upa = pax; // expected-error {{No use of variable have an unchecked pointer type}}
  upa = (int *)(array_ptr<int>)pax; // expected-error {{No use of variable have an unchecked pointer type}} expected-error {{invalid casting to unchecked pointer type in a checked scope}}

  upa = aparr; // expected-error {{No use of variable have an unchecked pointer type}} 
  upa = (int *)aparr; // expected-error {{No use of variable have an unchecked pointer type}} expected-error {{invalid casting to unchecked pointer type}}

  gptr0 = upa; // expected-error {{No use of variable have an unchecked pointer type}}
  gptr0 = (ptr<int>)upa; // expected-error {{No use of variable have an unchecked pointer type}}
  gptr0 = (array_ptr<int>)upa; // expected-error {{No use of variable have an unchecked pointer type}}

  gptr1 = upa; // expected-error {{No use of variable have an unchecked pointer type}}
  gptr1 = (ptr<int>)upa; // expected-error {{No use of variable have an unchecked pointer type}}
  gptr1 = (array_ptr<int>)upa; // expected-error {{No use of variable have an unchecked pointer type}}

  gptr2 = upa; // expected-error 2 {{No use of variable have an unchecked pointer type}}
  gptr2 = (ptr<int>)upa; // expected-error 2 {{No use of variable have an unchecked pointer type}}
  gptr2 = (array_ptr<int>)upa; // expected-error 2 {{No use of variable have an unchecked pointer type}}
}

checked void check_illegal_operators(void) {
  int p[5]; // expected-error {{variable cannot have an unchecked array type}}
  int r checked[5];
  p * 5;  // expected-error {{No use of variable have an unchecked array type in a checked scope}}
  5 * p;  // expected-error {{No use of variable have an unchecked array type in a checked scope}}
  p *= 5; // expected-error {{No use of variable have an unchecked array type in a checked scope}}

  r * 5;  // expected-error {{invalid operands to binary expression}}
  5 * r;  // expected-error {{invalid operands to binary expression}}
  r *= 5; // expected-error {{invalid operands to binary expression}}

  p * p;  // expected-error 2 {{No use of variable have an unchecked array type in a checked scope}}
  p *= p; // expected-error 2 {{No use of variable have an unchecked array type in a checked scope}}

  r * r;  // expected-error {{invalid operands to binary expression}}
  r *= r; // expected-error {{invalid operands to binary expression}}

  //
  // Test /
  //

  p / 5;  // expected-error {{No use of variable have an unchecked array type in a checked scope}}
  5 / p;  // expected-error {{No use of variable have an unchecked array type in a checked scope}}
  p /= 5; // expected-error {{No use of variable have an unchecked array type in a checked scope}}

  r / 5;  // expected-error {{invalid operands to binary expression}}
  5 / r;  // expected-error {{invalid operands to binary expression}}
  r /= 5; // expected-error {{invalid operands to binary expression}}

  p / p;  // expected-error 2 {{No use of variable have an unchecked array type in a checked scope}}
  p /= p; // expected-error 2 {{No use of variable have an unchecked array type in a checked scope}}

  r / r;  // expected-error {{invalid operands to binary expression}}
  r /= r; // expected-error {{invalid operands to binary expression}}

  //
  // Test %
  //

  p % 5;  // expected-error {{No use of variable have an unchecked array type in a checked scope}}
  5 % p;  // expected-error {{No use of variable have an unchecked array type in a checked scope}}
  p %= 5; // expected-error {{No use of variable have an unchecked array type in a checked scope}}

  r % 5;  // expected-error {{invalid operands to binary expression}}
  5 % r;  // expected-error {{invalid operands to binary expression}}
  r %= 5; // expected-error {{invalid operands to binary expression}}

  p % p;  // expected-error 2 {{No use of variable have an unchecked array type in a checked scope}}
  p %= p; // expected-error 2 {{No use of variable have an unchecked array type in a checked scope}}

  r % r;  // expected-error {{invalid operands to binary expression}}
  r %= r; // expected-error {{invalid operands to binary expression}}

  //
  // Test <<
  //

  p << 5;  // expected-error {{No use of variable have an unchecked array type in a checked scope}}
  5 << p;  // expected-error {{No use of variable have an unchecked array type in a checked scope}}
  p <<= 5; // expected-error {{No use of variable have an unchecked array type in a checked scope}}

  r << 5;  // expected-error {{invalid operands to binary expression}}
  5 << r;  // expected-error {{invalid operands to binary expression}}
  r <<= 5; // expected-error {{invalid operands to binary expression}}

  p << p;  // expected-error 2 {{No use of variable have an unchecked array type in a checked scope}}
  p <<= p; // expected-error 2 {{No use of variable have an unchecked array type in a checked scope}}

  r << r;  // expected-error {{invalid operands to binary expression}}
  r <<= r; // expected-error {{invalid operands to binary expression}}

  //
  // Test >>
  //
  p >> 5;  // expected-error {{No use of variable have an unchecked array type in a checked scope}}
  5 >> p;  // expected-error {{No use of variable have an unchecked array type in a checked scope}}
  p >>= 5; // expected-error {{No use of variable have an unchecked array type in a checked scope}}

  r >> 5;  // expected-error {{invalid operands to binary expression}}
  5 >> r;  // expected-error {{invalid operands to binary expression}}
  r >>= 5; // expected-error {{invalid operands to binary expression}}

  p >> p;  // expected-error 2 {{No use of variable have an unchecked array type in a checked scope}}
  p >>= p; // expected-error 2 {{No use of variable have an unchecked array type in a checked scope}}

  r >> r;  // expected-error {{invalid operands to binary expression}}
  r >>= r; // expected-error {{invalid operands to binary expression}}

  //
  // Test |
  //

  p | 5;  // expected-error {{No use of variable have an unchecked array type in a checked scope}}
  5 | p;  // expected-error {{No use of variable have an unchecked array type in a checked scope}}
  p |= 5; // expected-error {{No use of variable have an unchecked array type in a checked scope}}

  r | 5;  // expected-error {{invalid operands to binary expression}}
  5 | r;  // expected-error {{invalid operands to binary expression}}
  r |= 5; // expected-error {{invalid operands to binary expression}}

  p | p;  // expected-error 2 {{No use of variable have an unchecked array type in a checked scope}}
  p |= p; // expected-error 2 {{No use of variable have an unchecked array type in a checked scope}}

  r | r;  // expected-error {{invalid operands to binary expression}}
  r |= r; // expected-error {{invalid operands to binary expression}}

  //
  // Test &
  //

  p & 5;  // expected-error {{No use of variable have an unchecked array type in a checked scope}}
  5 & p;  // expected-error {{No use of variable have an unchecked array type in a checked scope}}
  p &= 5; // expected-error {{No use of variable have an unchecked array type in a checked scope}}

  r & 5;  // expected-error {{invalid operands to binary expression}}
  5 & r;  // expected-error {{invalid operands to binary expression}}
  r &= 5; // expected-error {{invalid operands to binary expression}}

  p & p;  // expected-error 2 {{No use of variable have an unchecked array type in a checked scope}}
  p &= p; // expected-error 2 {{No use of variable have an unchecked array type in a checked scope}}

  r & r;  // expected-error {{invalid operands to binary expression}}
  r &= r; // expected-error {{invalid operands to binary expression}}

  //
  // Test ^
  //

  p ^ 5;  // expected-error {{No use of variable have an unchecked array type in a checked scope}}
  5 ^ p;  // expected-error {{No use of variable have an unchecked array type in a checked scope}}
  p ^= 5; // expected-error {{No use of variable have an unchecked array type in a checked scope}}

  r ^ 5;  // expected-error {{invalid operands to binary expression}}
  5 ^ r;  // expected-error {{invalid operands to binary expression}}
  r ^= 5; // expected-error {{invalid operands to binary expression}}

  p ^ p;  // expected-error 2 {{No use of variable have an unchecked array type in a checked scope}}
  p ^= p; // expected-error 2 {{No use of variable have an unchecked array type in a checked scope}}

  r ^ r;  // expected-error {{invalid operands to binary expression}}
  r ^= r; // expected-error {{invalid operands to binary expression}}

  //
  // Test ~
  //
  ~p;  // expected-error {{No use of variable have an unchecked array type in a checked scope}}
  ~r;  // expected-error {{invalid argument type}}

  //
  // Test unary -
  //
  -p;  // expected-error {{No use of variable have an unchecked array type in a checked scope}}
  -r;  // expected-error {{invalid argument type}}
  //
  // Test unary +
  //
  +p;  // expected-error {{No use of variable have an unchecked array type in a checked scope}}
  +r;  // expected-error {{invalid argument type}}
}
