// Feature tests of typechecking of uses of Checked C checked scope/function
//
// The following lines are for the LLVM test harness:
//
// RUN: %clang_cc1 -verify -verify-ignore-unexpected=note -fcheckedc-extension %s

#include "../../include/stdchecked.h"
// checked function test
// - check if function declaration (return/param) is checked
// - check if function body (compound statement) is checked
// - check if function with no prototype checking is checked

checked int f0(int len, int *p, ptr<int> q, array_ptr<int> r, int *s : itype(ptr<int>)) { // expected-error {{parameter cannot have an unchecked pointer type}}
  int result = *p + *q + *s; // expected-error {{No use of parameter have an unchecked pointer type}}
  return result;
}
checked int* f1(int len, int *p, int *q, int *r : itype(array_ptr<int>)) {  // expected-error 2 {{parameter cannot have an unchecked pointer type}} \
                                                                            // expected-error {{return cannot have an unchecked pointer type}}
  int result = *p + *q + *r; // expected-error 2 {{No use of parameter have an unchecked pointer type}}
  int *presult = &result; // expected-error {{variable cannot have an unchecked pointer type}} expected-error {{initializing 'int *' with an expression of incompatible type}}
  return presult; // expected-error {{No use of variable have an unchecked pointer type }}
}
checked ptr<int> f2() { // expected-error {{function with no prototype cannot have a return type that is a checked type}} expected-error {{function without a prototype cannot be used or declared in a checked scope}}
}
// function without a prototype does not have an interop return type
checked int* f3() : itype(array_ptr<int>) { //expected-error {{return cannot have an unchecked pointer type in a checked scope}} //expected-error {{function without a prototype cannot be used or declared in a checked scope}}
}
checked int* f4(int len, int *p, int *q, array_ptr<int> r) : itype(ptr<int>) { // expected-error 2 {{parameter cannot have an unchecked pointer type}}
}
checked int f5(int p[]) { // expected-error {{parameter cannot have an unchecked array type in a checked scope}}
  int a = 5;
  int *upa = &a; // expected-error {{variable cannot have an unchecked pointer type in a checked scope}} expected-error {{initializing 'int *' with an expression of incompatible type}}
  ptr<int> pb = &a;
  array_ptr<int> pc = &a;
  int *upd = p; // expected-error {{variable cannot have an unchecked pointer type in a checked scope}} expected-error {{No use of parameter have an unchecked array type}}
  return *p; // expected-error {{No use of parameter have an unchecked array type}}
}

checked int f6(ptr<int> p, int *q) { // expected-error {{parameter cannot have an unchecked pointer type}}
  int a = 5;
  int *upa = &a; // expected-error {{variable cannot have an unchecked pointer type in a checked scope}} expected-error {{initializing 'int *' with an expression of incompatible type}}
  ptr<int> pb = p;
  return *pb;
}

checked int f7(ptr<int> p, int *q : itype(ptr<int>)) {
  int a = 5;
  int *upa = &a; // expected-error {{variable cannot have an unchecked pointer type in a checked scope}} expected-error {{initializing 'int *' with an expression of incompatible type}}
  int *upb = q; // expected-error {{variable cannot have an unchecked pointer type in a checked scope}}
  ptr<int> pc = p;
  int e[5][5]; // expected-error {{variable cannot have an unchecked array type}}
  return *pc;
}

checked int* f8(ptr<int> p, int *q : itype(ptr<int>)) { // expected-error {{return cannot have an unchecked pointer type in a checked scope}}
  int a = 5;
  ptr<int> pa = &a;
  ptr<int> pb = p;
  int *upc = q; // expected-error {{variable cannot have an unchecked pointer type in a checked scope}}
  return upc; // expected-error {{No use of variable have an unchecked pointer type}}
}

checked int* f9(ptr<int> p, int *q : itype(ptr<int>)) : itype(ptr<int>) {
  int a = 5;
  ptr<int> pa = &a;
  ptr<int> pb = p;
  int *upc = q; // expected-error {{variable cannot have an unchecked pointer type in a checked scope}}
  return upc; // expected-error {{No use of variable have an unchecked pointer type}}
}

// checked function + checked scope keyword test
int* f10(array_ptr<int> x, int len) checked {
  int *upa; // expected-error {{variable cannot have an unchecked pointer type in a checked scope}}
  return upa; // expected-error {{No use of variable have an unchecked pointer type}}
}

array_ptr<int> f11(array_ptr<int> x, int len) : bounds(x,x+len) checked {
  int *upa; // expected-error {{variable cannot have an unchecked pointer type in a checked scope}}
  return upa; // expected-error {{No use of variable have an unchecked pointer type}}
}

array_ptr<int> f12(int *x, int *y) checked {
  int *upa = x; // expected-error {{variable cannot have an unchecked pointer type in a checked scope}} expected-error {{No use of parameter have an unchecked pointer type}}
  int *upb = y; // expected-error {{variable cannot have an unchecked pointer type in a checked scope}} expected-error {{No use of parameter have an unchecked pointer type}}
  return upb; // expected-error {{No use of variable have an unchecked pointer type}}
}

checked ptr<int> f13(void) checked {
  int *upa; // expected-error {{variable cannot have an unchecked pointer type in a checked scope}}
  ptr<int> pb = 0;
  array_ptr<int> pc;
  return pb;
}

checked array_ptr<char> f14(void) checked {
  int *upa; // expected-error {{variable cannot have an unchecked pointer type in a checked scope}}
  ptr<int> pb = 0 ;
  array_ptr<char> pc;
  return pc;
}

checked int* f15(void) checked { // expected-error {{return cannot have an unchecked pointer type in a checked scope}}
  int a[5]; // expected-error {{variable cannot have an unchecked array type in a checked scope}}
  int *upa; // expected-error {{variable cannot have an unchecked pointer type in a checked scope}}
  ptr<int> pb = 0;
  array_ptr<int> pc;
  return upa; // expected-error {{No use of variable have an unchecked pointer type}}
}

checked int* f16(int a checked[][5], int b checked[][5]) : itype(ptr<int>) checked {
}

checked int* f17(int a [][5], int b [][5]) : itype(ptr<int>) checked { // expected-error 2 {{parameter cannot have an unchecked array type in a checked scope}}
}

checked int* f18(int *a : itype(ptr<int>), int *b : itype(array_ptr<int>)) : itype(array_ptr<int>) checked {
  int e checked[5][5];
  int f[5][5]; // expected-error {{variable cannot have an unchecked array type in a checked scope}}
  int *upa = f16(e, e); // expected-error {{variable cannot have an unchecked pointer type in a checked scope}}
  ptr<int> pb = f16(e, e);
  ptr<int> pc = 0;
  pc = f17(f, f); // expected-error 2 {{No use of variable have an unchecked array type}}
  int *upd = f10(upa, 10); // expected-error {{variable cannot have an unchecked pointer type}} expected-error {{No use of return have an unchecked pointer type}} expected-error {{No use of variable have an unchecked pointer type}}
  return upa; // expected-error {{No use of variable have an unchecked pointer type}}
}

checked int* f19(int a[] : itype(int *), char b[] : itype(char *)) : itype(int *) checked { // expected-error 3 {{type must be a checked type}} expected-error 2 {{parameter cannot have an unchecked array type in a checked scope}} expected-error {{return cannot have an unchecked pointer type}}
}

// checked scope test
// - compound statments are checked
// - nested checked scope inherits parent checked property
int f20(void) checked {
  int a = 5;
  ptr<int> pa = &a;
  int b[5][5]; // expected-error {{variable cannot have an unchecked array type in a checked scope}}
  for (int i = 0; i < 5; i++) {
    for (int j = 0; j < 5; j++) {
      b[i][j] = -1; // expected-error {{No use of variable have an unchecked array type}}
    }
  }
}

int f21(void) {
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

int f22(void) {
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

int f23(void) {
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

int f24(void) {
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

int f25(void) {
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

void f30(void) {
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


checked int f31(ptr<struct s0> st0, ptr<struct s1> st1) {
  // struct s0 member a & b is an error of declaration
  int sum = *(st0->a) + *(st1->a);
  sum += *(st0->b) + *(st1->b);
  sum += *(st0->pc) + *(st1->pc);
  sum += *(st0->pd) + *(st1->pd);

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

void check_cast_operator(void) checked {
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
