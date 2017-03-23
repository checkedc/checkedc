// Feature tests of typechecking of uses of Checked C checked scope/function
//
// The following lines are for the LLVM test harness:
//
// RUN: %clang_cc1 -verify -verify-ignore-unexpected=note -fcheckedc-extension %s

#include "../../include/stdchecked.h"
#if 0
// checked function test
// - check if function declaration (return/param) is checked
// - check if function body (compound statement) is checked
// - check if function with no prototype checking is checked

checked int f0(int len, int *p, ptr<int> q, array_ptr<int> r, int *s : itype(ptr<int>)) { // expected-error {{parameter cannot have an unchecked pointer type}}
}
checked int* f1(int len, int *p, int *q, int *r : itype(array_ptr<int>)) {  // expected-error 2 {{parameter cannot have an unchecked pointer type}} \
                                                                            // expected-error {{return cannot have an unchecked pointer type}}
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
  int *upa = &a; // expected-error {{variable cannot have an unchecked pointer type in a checked scope}}
  ptr<int> pb = &a;
  array_ptr<int> pc = &a;
  int *upd = p; // expected-error {{variable cannot have an unchecked pointer type in a checked scope}}
  return *p;
}

checked int f6(ptr<int> p, int *q) { // expected-error {{parameter cannot have an unchecked pointer type}}
  int a = 5;
  int *upa = &a; // expected-error {{variable cannot have an unchecked pointer type in a checked scope}}
  ptr<int> pb = p;
  return *pb;
}

checked int f7(ptr<int> p, int *q : itype(ptr<int>)) {
  int a = 5;
  int *upa = &a; // expected-error {{variable cannot have an unchecked pointer type in a checked scope}}
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
  return upc;
}

checked int* f9(ptr<int> p, int *q : itype(ptr<int>)) : itype(ptr<int>) {
  int a = 5;
  ptr<int> pa = &a;
  ptr<int> pb = p;
  int *upc = q; // expected-error {{variable cannot have an unchecked pointer type in a checked scope}}
  return upc;
}

// checked function + checked scope keyword test
int* f10(array_ptr<int> x, int len) checked {
  int *upa; // expected-error {{variable cannot have an unchecked pointer type in a checked scope}}
  return upa;
}

array_ptr<int> f11(array_ptr<int> x, int len) : bounds(x,x+len) checked {
  int *upa; // expected-error {{variable cannot have an unchecked pointer type in a checked scope}}
  return upa;
}

array_ptr<int> f12(int *x, int *y) checked {
  int *upa = x; // expected-error {{variable cannot have an unchecked pointer type in a checked scope}}
  int* upb = y; // expected-error {{variable cannot have an unchecked pointer type in a checked scope}}
  return upb;
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
  return upa;
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
  ptr<int> pc = f17(f, f);
  return upa;
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
      b[i][j] = -1;
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
    int *upa = &a; // expected-error {{variable cannot have an unchecked pointer type in a checked scope}}
  checked {
    int b[5][5]; // expected-error {{variable cannot have an unchecked array type in a checked scope}}
  checked {
    for (int i = 0; i < 5; i++) {
      for (int j = 0; j < 5; j++) {
        b[i][j] = -1;
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
      int *upa = &a; // expected-error {{variable cannot have an unchecked pointer type in a checked scope}}
    {
      int b[5][5]; // expected-error {{variable cannot have an unchecked array type in a checked scope}}
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
}

int f24(void) {
  int a = 5;
  checked {
    int *upa = &a; // expected-error {{variable cannot have an unchecked pointer type in a checked scope}}
    int b[5][5]; // expected-error {{variable cannot have an unchecked array type in a checked scope}}
    for (int i = 0; i < 5; i++) {
      for (int j = 0; j < 5; j++) {
        b[i][j] = -1;
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
#endif
// change type produced by address-of operator(&) in checked block
// checked { .... int a; ptr<int> pb = &a; }
void test_addrof_checked_scope(void) checked {
  int a checked[10];
  array_ptr<int> b;
  int i;

  // address-of operator output type _Ptr<T>
  ptr<int> x = &a[i]; // UnaryOperator _Ptr<int>
  ptr<int> y = &b[0]; // UnaryOperator _Ptr<int>
  ptr<int> z = &i; // UnaryOperator _Ptr<int>

  x = &a[i]; // UnaryOperator _Ptr<int>
  y = &b[0]; // UnaryOperator _Ptr<int>
  z = &i; // UnaryOperator _Ptr<int>

  // incompatible type error, assigning to from incompatible type
  x = b;

  // address-of operator output _Ptr<T> is incompatible with _Array_ptr
  // initialization of incompatible type
  array_ptr<int> ax = &a[i];
  array_ptr<int> ay = &b[0];
  array_ptr<int> az = &i;

  ax = &a[i];
  ay = &b[0];
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
}

void test_addrof_unchecked_scope(void) {
  int a checked[10];
  array_ptr<int> b;
  int i;

  // checkSingleAssignmentConstraints(int * -> _Ptr<int> implicit casting)
  ptr<int> x = &a[i]; // ImplicitCastExpr _Ptr<int>(UnaryOperator int * prefix &)
  ptr<int> y = &b[0]; // ImplicitCastExpr _Ptr<int>(UnaryOperator int * prefix &)
  ptr<int> z = &i; // ImplicitCastExpr _Ptr<int>(UnaryOperator int * prefix &)

  x = &a[i]; // ImplicitCastExpr _Ptr<int>(UnaryOperator int * prefix &)
  y = &b[0]; // ImplicitCastExpr _Ptr<int>(UnaryOperator int * prefix &)
  z = &i; // ImplicitCastExpr _Ptr<int>(UnaryOperator int * prefix &)

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
