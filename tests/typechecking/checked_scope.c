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
// if definition has an unchecked type error in a checked scope, it produces error message for declaration
// otherwise, it produces an error message for each use of declaration
checked int func0(int len, int *p, ptr<int> q, array_ptr<int> r, int *s : itype(ptr<int>)) { // expected-error {{parameter cannot have an unchecked pointer type}}
  int result = *p + *q + *s; // remove redundant use error
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

// checked function + checked scope keyword test
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

// checked scope test
// - compound statments are checked
// - nested checked scope inherits parent checked property
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
  checked {
    int *upa = &a;  // expected-error {{variable cannot have an unchecked pointer type in a checked scope}}
  checked {
    int b[5][5];    // expected-error {{variable cannot have an unchecked array type in a checked scope}}
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

int func23(void) {
  int a = 5;
  checked {
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

// unchecked scope test
// - unchecked scope clears checked property & prevents inheritance of checked property
// - unchecked scope keyword clears checked function specifier
checked int * func40(void) unchecked {  // expected-error {{return cannot have an unchecked pointer type}}
  int a = 5;
  ptr<int> pa = &a;
  int b[5][5];
  for (int i = 0; i < 5; i++) {
    for (int j = 0; j < 5; j++) {
      b[i][j] = -1;
    }
  }
  return &b[4][4];  // expected-warning {{address of stack memory}}
}

checked int * func41(void) unchecked {  // expected-error {{return cannot have an unchecked pointer type}}
  int a = 5;
  checked {
    unchecked {
      ptr<int> pa = &a;
      int b checked[5][5];
      for (int i = 0; i < 5; i++) {
        for (int j = 0; j < 5; j++) {
          b[i][j] = -1;
        }
      }
    }
  }
  return &a;  // expected-warning {{address of stack memory}}
}

checked int func42(void) unchecked {
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

checked int * func43(void) unchecked {  // expected-error {{return cannot have an unchecked pointer type}}
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
    int d[5][5];    // expected-error {{variable cannot have an unchecked array type}}
    for (int i = 0; i < 5; i++) {
      for (int j = 0; j < 5; j++) {
        b[i][j] += c[i][j] - d[i][j];   // expected-error {{cannot use a variable with an unchecked type}}
      }
    }
  }
  return &b[4][4];  // expected-warning {{address of stack memory}}
  }
  }
}

int func45(void) {
  int a = 5;
  int *upa = &a;
  int b[5][5];
  int sum = 0;
  for (int i = 0; i < 5; i++) checked {
    sum += b[i][0]; // expected-error {{cannot use a variable with an unchecked type}}
    for (int j = 1; j < 5; j++) unchecked {
      sum += b[i][j];
    }
  }
  return sum;
}

int func46(ptr<int> p, int *q) {
  int a = 5;
  int *upa;
  array_ptr<int> pc;
  upa = pc;           // expected-error {{assigning to 'int *' from incompatible type '_Array_ptr<int>'}}
  checked {
    upa = &a;         // expected-error {{cannot use a variable with an unchecked type}}
  unchecked {
    upa = &a;
  }
  checked {
    upa = &a;         // expected-error {{cannot use a variable with an unchecked type}}
  }
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
  upb = pb; // expected-error {{assigning to 'int *' from incompatible type '_Array_ptr<int>'}}
  upb = pc; // expected-error {{assigning to 'int *' from incompatible type '_Ptr<int>'}}
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

struct s0 checked {
  int *a;     // expected-error {{member cannot have an unchecked pointer type}}
  char *b;    // expected-error {{member cannot have an unchecked pointer type}}
  ptr<int> pc;
  array_ptr<int> pd : count(len);
  int len;
  struct s1 {
    int *a;   // expected-error {{member cannot have an unchecked pointer type}}
    char *b;  // expected-error {{member cannot have an unchecked pointer type}}
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
  struct s3 {
    int *a;
    char *b;
    ptr<int> pc;
    array_ptr<int> pd : count(len);
    int len;
  } d;
};

checked int func60(ptr<struct s0> st0, ptr<struct s1> st1) {
  int sum = *(st0->a) + *(st1->a);
  sum += *(st0->b) + *(st1->b);
  sum += *(st0->pc) + *(st1->pc);
  sum += *(st0->pd) + *(st1->pd);

  struct s2 sta;
  ptr<struct s2> pstb = 0;
  sum += *(sta.a) + *(sta.b) + *(sta.pc) + *(sta.pd); // expected-error 2 {{cannot use a member with an unchecked type}}
  sum += *(pstb->a) + *(pstb->b); // expected-error 2 {{cannot use a member with an unchecked type}}
  sum += *(pstb.a) + *(pstb.b);   // expected-error 2 {{cannot use a member with an unchecked type}} expected-error 2 {{member reference type}}
  sum += *(sta->a) + *(sta->b);   // expected-error 2 {{cannot use a member with an unchecked type}} expected-error 2 {{member reference type}}

  ptr<struct s2> st2 = 0;
  ptr<struct s2> st3 = 0;
  sum += *(st2->a)  + *(st3->a); // expected-error 2 {{cannot use a member with an unchecked type}}
  sum += *(st2->b)  + *(st3->b); // expected-error 2 {{cannot use a member with an unchecked type}}
  sum += *(st2->pc) + *(st3->pc);
  sum += *(st2->pd) + *(st3->pd);
  sum += *(st2->d.a) + *(st3->d.a); // expected-error 2 {{cannot use a member with an unchecked type}}
  sum += *(st2->d.b) + *(st3->d.b); // expected-error 2 {{cannot use a member with an unchecked type}}
  return sum;
}

// change type produced by address-of operator(&) in checked block
// checked { .... int a; ptr<int> pb = &a; }
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

void test_addrof_unchecked_scope(void) {
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
  int *t1 = p;          // expected-error {{variable cannot have an unchecked pointer type}} \
                        // expected-error {{cannot use a parameter with an unchecked type}}
                        // T *  = T[constant] OK
  int *t2 = q;          // expected-error {{variable cannot have an unchecked pointer type}} \
                        // expected-error {{cannot use a parameter with an unchecked type}}
                        // T *  = T[] OK
  int *t3 = t;          // expected-error {{variable cannot have an unchecked pointer type}}
                        // T *  = T[constant] OK;
  int *t4 = r;          // expected-error {{variable cannot have an unchecked pointer type}}
                        // Assignment of checked pointer to unchecked pointer not allowed
  int *t5 = s;          // expected-error {{variable cannot have an unchecked pointer type}}
  int *t6 = u;          // expected-error {{variable cannot have an unchecked pointer type}}

  // Various forms of array_ptr<T> = T[]. Note that the rhs does not need to have known bounds
  // because the lhs pointers have no bounds (and cannot be dereferenced).
  //
  // Note if there need to be known bounds, the bounds of p and q are unknown
  // because C does not guarantee that array sizes match for parameter passing
  array_ptr<int> t7 = p;        // expected-error {{cannot use a parameter with an unchecked type}}
  array_ptr<int> t8 = q;        // expected-error {{cannot use a parameter with an unchecked type}}
  array_ptr<int> t9 = r;
  array_ptr<int> t10  = s;
  array_ptr<int> t11 = t;
  array_ptr<int> t12 = u;
  array_ptr<int> t13 = s2d[0];
  array_ptr<int> t14 = t2d[0];
  array_ptr<int> t15 = u2d[0];


  // Multi-dimensional array type conversions to pointer types.
  int *t16 = s2d[0];        // expected-error {{variable cannot have an unchecked pointer type}}
  int *t17 = t2d[0];        // expected-error {{variable cannot have an unchecked pointer type}}
  int *t18 = u2d[0];        // expected-error {{variable cannot have an unchecked pointer type}}
  int(*t19)[10] = s2d;      // expected-error {{variable cannot have an unchecked pointer type}}
                            // assignment of checked array to unchecked array not allowed
  int (*t20)[10] = t2d;     // expected-error {{variable cannot have an unchecked pointer type}}
  int (*t21)[10] = u2d;     // expected-error {{variable cannot have an unchecked pointer type}}
                            // assignment of checked array to unchecked array not allowed
  array_ptr<int[10]> t22 = s2d; // expected-error {{expression of incompatible type '_Array_ptr<int checked[10]>'}}
                                // assignment of checked to unchecked not allowed
  array_ptr<int[10]> t23 = t2d;
  array_ptr<int[10]> t24 = u2d; // expected-error {{expression of incompatible type 'int checked[10][10]'}}
                                // assignment of checked to unchecked not allowed
  array_ptr<int checked[10]> t25 = s2d;
  array_ptr<int checked[10]> t26 = t2d;
  array_ptr<int checked[10]> t27 = u2d;

  // Assignments to array-typed parameters are allowed.  The outermost array modifier
  // is converted to a pointer type.
  p = q;  // expected-error 2 {{cannot use a parameter with an unchecked type}}
  q = p;  // expected-error 2 {{cannot use a parameter with an unchecked type}}
  s = r;
  r = t;
  p = r;  // expected-error {{cannot use a parameter with an unchecked type}}
          // assignment of checked pointer to unchecked pointer not allowed

  // Assignments to array-typed local and global variables are not allowed
  t = p;  // expected-error {{cannot use a parameter with an unchecked type}}
  t = r;  // use of unchecked type, invalid declaration ignore others
  u = r;  // expected-error {{array type 'int checked[10]' is not assignable}}
  global_arr = p; // expected-error {{cannot use a variable with an unchecked type}} \
                  // expected-error {{cannot use a parameter with an unchecked type}}
  global_arr = r; // expected-error {{cannot use a variable with an unchecked type}}
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
  int *t12 = t6[0];                    // expected-error {{variable cannot have an unchecked pointer type}}
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
  int *t1 = r2d[0]; // expected-error {{variable cannot have an unchecked pointer type }}
}

checked void check_dimensions7(int (r2d checked[const])[10] : count(len), int len) {
  r2d = 0;          // expected-error {{cannot assign to variable 'r2d' with const-qualified type}}
  int *t1 = r2d[0]; // expected-error {{variable cannot have an unchecked pointer type }}
}

checked void check_dimensions8(int (r2d) checked[const][10] : count(len), int len) {
  r2d = 0;          // expected-error {{cannot assign to variable 'r2d' with const-qualified type}}
  int *t1 = r2d[0]; // expected-error {{variable cannot have an unchecked pointer type }}
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
  int a[10];                          // expected-error {{variable cannot have an unchecked array type}}
  int b checked[10];
  const int const_a[10];              // expected-error {{variable cannot have an unchecked array type}}
  const int const_b checked[10];
  volatile int volatile_a[10];        // expected-error {{variable cannot have an unchecked array type}}

  // NOTE : checked scope error fails to precede
  int a_const_ptr[const 10]; // expected-error {{variable cannot have an unchecked array type}} expected-error {{type qualifier used in array declarator}}

  //
  // check assignments to parameters
  //
  // assign an unchecked array,where the element type does not have modifiers
  param = a;                          //  expected-error {{cannot use a parameter with an unchecked type}}
  checked_param = a;
  param_const_ptr = a;                //  expected-error {{cannot use a parameter with an unchecked type}}
  checked_param_const_ptr = a;
  const_param = a;                    //  expected-error {{cannot use a parameter with an unchecked type}}
  const_checked_param = a;
  const_param_const_ptr = a;          //  expected-error {{cannot use a parameter with an unchecked type}}
  const_checked_param_const_ptr = a;
  volatile_param = a;                 //  expected-error {{cannot use a parameter with an unchecked type}}
  checked_volatile_param = a;

  // assign a checked array, where the element type does not have modifiers
  param = b;                          // expected-error {{cannot use a parameter with an unchecked type}}
  checked_param = b;
  param_const_ptr = b;                // expected-error {{cannot use a parameter with an unchecked type}}
  checked_param_const_ptr = b;        // expected-error {{cannot assign to variable}}
  const_param = b;                    // expected-error {{cannot use a parameter with an unchecked type}}
  const_checked_param = b;
  const_param_const_ptr = b;          // expected-error {{cannot use a parameter with an unchecked type}}
  const_checked_param_const_ptr = b;  // expected-error {{cannot assign to variable}}
  volatile_param = b;                 // expected-error {{cannot use a parameter with an unchecked type}}

  // check assigning an unchecked array where the element type has modifiers
  param = const_a;                    // expected-error {{cannot use a parameter with an unchecked type}}
  checked_param = const_a;
  const_param = const_a;              // expected-error {{cannot use a parameter with an unchecked type}}
  const_checked_param = const_a;
  volatile_param = const_a;           // expected-error {{cannot use a parameter with an unchecked type}}
  checked_volatile_param = const_a;

  // spot check assigning a checked array where the element type has modifiers
  checked_param = const_b;            // expected-warning {{discards qualifiers}}
  const_checked_param = const_b;
  volatile_param = const_b;           // expected-error {{cannot use a parameter with an unchecked type}}
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
  p = param;            // expected-error {{cannot use a parameter with an unchecked type}}
  p_const = param;      // expected-error {{cannot use a parameter with an unchecked type}}
  p_volatile = param;   // expected-error {{cannot use a parameter with an unchecked type}}
  // q = param;          not allowed: param has unknown bounds
  // q_const = param;    not allowed: param has unknown bounds
  // q_volatile = param; not allowed: param has unknown bounds
  r = param;            // expected-error {{cannot use a parameter with an unchecked type}}
  r_const = param;      // expected-error {{cannot use a parameter with an unchecked type}}
  r_volatile = param;   // expected-error {{cannot use a parameter with an unchecked type}}

  // the rhs is a checked array where the element type does not have modifiers
  p = checked_param;
  p_const = checked_param;
  p_volatile = checked_param;
  q = checked_param;              // ptr<T> = array_ptr<T> OK
  q_const = checked_param;        // ptr<T> = array_ptr<T> OK
  q_volatile = checked_param;     // ptr<T> = array_ptr<T> OK
  r = checked_param;
  r_const = checked_param;
  r_volatile = checked_param;

  // the rhs is an unchecked array where the element type has modifiers
  p = const_param;              // expected-error {{cannot use a parameter with an unchecked type}}
  p_const = const_param;        // expected-error {{cannot use a parameter with an unchecked type}}
  p_volatile = const_param;     // expected-error {{cannot use a parameter with an unchecked type}}
  // q = const_param;           not allowed: param has unknown bounds
  // q_const = const_param;     not allowed: param has unknown bounds
  // q_volatile = const_param;  not allowed: param has unknown bounds
  r = const_param;              // expected-error {{cannot use a parameter with an unchecked type}}
  r_const = const_param;        // expected-error {{cannot use a parameter with an unchecked type}}
  r_volatile = const_param;     // expected-error {{cannot use a parameter with an unchecked type}}

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
  p = b;
  p_const = b;
  p_volatile = b;
  q = b;              // ptr<T> = array_ptr<T> OK
  q_const = b;        // ptr<T> = array_ptr<T> OK
  q_volatile = b;     // ptr<T> = array_ptr<T> OK
  r = b;
  r_const = b;
  r_volatile = b;

  // the rhs is an unchecked array where the element type has modifiers
  p = const_a;
  p_const = const_a;
  p_volatile = const_a;
  q = const_a;
  q_const = const_a;
  q_volatile = const_a;
  r = const_a;
  r_const = const_a;
  r_volatile = const_a;

  // the rhs is an checked array where the element type has modifiers
  q = const_b;      // expected-warning {{discards qualifiers}}
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

  int *t1 = val ? p : p;            // T[5] and T[5] OK, expected-error {{variable cannot have an unchecked pointer type}}
  array_ptr<int> t2 = val ? p : r;  // T[5] and T checked[5] OK
  array_ptr<int> t3 = val ? r : p;  // T checked[5] and T[5] OK
  array_ptr<int> t4 = val ? r : r;  // T checked[5] and T checked[5] OK.

  // omit assignment because type of expression is not valid when there is an error.
  val ? s : r;
                   // S[5] and T checked[5] not OK;
  val ? r : s;
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

  int (*t1)[6] = val ? p : p;                  // T[5][6] and T[5][6] OK, expected-error {{variable cannot have an unchecked pointer type}}
  array_ptr<int checked[6]> t2 = val ? p : r;  // T[5][6] and T checked[5][6] OK
  array_ptr<int checked[6]> t3 = val ? r : p;  // T checked[5][6] and T[5][6] OK
  array_ptr<int checked[6]> t4 = val ? r : r;  // T checked[5][6] and T checked[5][6] OK.

  array_ptr<int [6]> t5 = val ? p : r;
                                        // T[5][6] and T checked[5][6] produce a checked array
  array_ptr<int [6]> t6 = val ? r : p;
                                        // T checked[5][6] and T[5][6] produce a checked array
  array_ptr<int [6]> t7 = val ? r : r;  // expected-error {{incompatible type}}
                                        // T checked[5][6] and T checked[5][6] produce a checked array

  // omit assignment because type of expression is not valid when there is an error.
  val ? s : r;
                    // S[5][6] and T checked[5][6] not OK;
  val ? r : s;
                    // T checked[5][6] and S[5][6] not OK.
  val ? r : u;      // expected-error {{pointer type mismatch}}
                    // T checked[5][6] and S checked[5][6] not OK
  val ? u : r;      // expected-error {{pointer type mismatch}}
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
  const int *t1 = val ? p : p_const;      // expected-error {{variable cannot have an unchecked pointer type}}
                                          // int * and const int * OK
  const int *t2 = val ? p_const : p;      // expected-error {{variable cannot have an unchecked pointer type}}
                                          // const int * and int * OK
  const int *t3 = val ? p_const : p_const;// expected-error {{variable cannot have an unchecked pointer type}}
                                          // const int * and const int * OK

  int *t4 = val ? p : p_const;            // expected-error {{variable cannot have an unchecked pointer type}}
                                          // int * and const int * produce const int *
  int *t5 = val ? p_const : p;            // expected-error {{variable cannot have an unchecked pointer type}}
                                          // const int * and int * produce const int *
  int *t6 = val ? p_const : p_const;      // expected-error {{variable cannot have an unchecked pointer type}}
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

  array_ptr<int> t34 = val ? p : r_const;
                                                  // int * and array_ptr<const int> produce array_ptr<const int>
  array_ptr<int> t35 = val ? r_const : p;
                                                  // array_ptr<const int> and int * produce array_ptr<const int>
  array_ptr<int> t36 = val ? p_const : r;
                                                  // const int * and array_ptr<int> produce array_ptr<const int>
  array_ptr<int> t37 = val ? r : p_const;
                                                  // array_ptr<int> and const int * produce array_ptr<const int>
  array_ptr<int> t38 = val ? p_const : r_const;
                                                  // const int * and array_ptr<const int> produce array_ptr<const int>
  array_ptr<int> t39 = val ? r_const : p_const;
                                                  // array_ptr<const int> and const int * produce array_ptr<const int>
  array_ptr<int> t40 = val ? r : r_const;         // expected-warning {{discards qualifiers}}
                                                  // array_ptr<int> and array_ptr<const int> produce array_ptr<const int>
  array_ptr<int> t41 = val ? r_const : r;         // expected-warning {{discards qualifiers}}
                                                  // array_ptr<const int> and array_ptr<int> produce array_ptr<const int>
  array_ptr<int> t42 = val ? r_const : r_const;   // expected-warning {{discards qualifiers}}
                                                  // array_ptr<const int> and array_ptr<const int> produce array_ptr<const int>

  // test different kinds of pointers with volatile modifers
  volatile int *t50 = val ? p : p_volatile;           // expected-error {{variable cannot have an unchecked pointer type}}
                                                      // int * and volatile int * OK
  volatile int *t51 = val ? p_volatile : p;           // expected-error {{variable cannot have an unchecked pointer type}}
                                                      // volatile int * and int * OK
  volatile int *t52 = val ? p_volatile : p_volatile;  // expected-error {{variable cannot have an unchecked pointer type}}
                                                      // volatile int * and volatile int * OK

  int *t53 = val ? p : p_volatile;                    // expected-error {{variable cannot have an unchecked pointer type}}
                                                      // int * and volatile int * produce volatile int *
  int *t54 = val ? p_volatile : p;                    // expected-error {{variable cannot have an unchecked pointer type}}
                                                      // volatile int * and int * produce volatile int *
  int *t55 = val ? p_volatile : p_volatile;           // expected-error {{variable cannot have an unchecked pointer type}}
                                                      // volatile int * and volatile int * produce volatile int *

  array_ptr<volatile int> t74 = val ? p : r_volatile;         // int * and array_ptr<volatile int> OK
  array_ptr<volatile int> t75 = val ? r_volatile : p;         // array_ptr<volatile int> and int * OK
  array_ptr<volatile int> t76 = val ? p_volatile : r;         // volatile int * and array_ptr<int> OK
  array_ptr<volatile int> t77 = val ? r : p_volatile;         // array_ptr<int> and volatile int * OK
  array_ptr<volatile int> t78 = val ? p_volatile : r_volatile;// volatile int * and array_ptr<volatile int> OK
  array_ptr<volatile int> t79 = val ? r_volatile : p_volatile;// array_ptr<volatile int> and volatile int * OK
  array_ptr<volatile int> t80 = val ? r : r_volatile;         // array_ptr<int> and array_ptr<volatile int> OK
  array_ptr<volatile int> t81 = val ? r_volatile : r;         // array_ptr<volatile int> and array_ptr<int> OK
  array_ptr<volatile int> t82 = val ? r_volatile : r_volatile;// array_ptr<volatile int> and array_ptr<volatile int> OK

  array_ptr<int> t83 = val ? p : r_volatile;
                                                      // int * and array_ptr<volatile int> produce array_ptr<volatile int>
  array_ptr<int> t84 = val ? r_volatile : p;
                                                      // array_ptr<volatile int> and int * produce array_ptr<volatile int>
  array_ptr<int> t85 = val ? p_volatile : r;          // pected-no-warning {{discards qualifiers}}
                                                      // volatile int * and array_ptr<int> produce array_ptr<volatile int>
  array_ptr<int> t86 = val ? r : p_volatile;          // pected-no-warning {{discards qualifiers}}
                                                      // array_ptr<int> and volatile int * produce array_ptr<volatile int>
  array_ptr<int> t87 = val ? p_volatile : r_volatile;
                                                      // volatile int * and array_ptr<volatile int> produce array_ptr<volatile int>
  array_ptr<int> t88 = val ? r_volatile : p_volatile;
                                                      // array_ptr<volatile int> and volatile int * produce array_ptr<volatile int>
  array_ptr<int> t89 = val ? r : r_volatile;          // pected-no-warning {{discards qualifiers}}
                                                      // array_ptr<int> and array_ptr<volatile int> produce array_ptr<volatile int>
  array_ptr<int> t90 = val ? r_volatile : r;          // pected-no-warning {{discards qualifiers}}
                                                      // array_ptr<volatile int> and array_ptr<int> produce array_ptr<volatile int>
  array_ptr<int> t92 = val ? r_volatile : r_volatile;
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
  f1(x, 0);
  f1(y, 0);               // expected-error {{parameter of incompatible type 'int *'}}
  f1(x2d, 0);
  f1(y2d, 0);             // expected-error {{passing 'int checked[10][10]' to parameter of incompatible type 'int *'}}

  // f2(int p[10], int y)
  f2(x, 0);
  f2(y, 0);               // expected-error {{parameter of incompatible type 'int *'}}
  f2(x2d, 0);
  f2(y2d, 0);             // expected-error {{passing 'int checked[10][10]' to parameter of incompatible type 'int *'}}

  // f3(int p checked[10], int y)
  f3(x, 0);
  f3(y, 0);
  f3(x2d, 0);
  f3(y2d, 0);             // expected-error {{passing 'int checked[10][10]' to parameter of incompatible type '_Array_ptr<int>'}}

  // f4(int **p, int y);
  f4(x, 0);
  f4(y, 0);               // expected-error {{passing 'int checked[10]' to parameter of incompatible type 'int **'}}
  f4(x2d, 0);
  f4(y2d, 0);             // expected-error {{passing 'int checked[10][10]' to parameter of incompatible type 'int **'}}

  // f5(int (*p)[10], int y);
  f5(x, 0);
  f5(y, 0);               // expected-error {{passing 'int checked[10]' to parameter of incompatible type 'int (*)[10]'}}
  f5(x2d, 0);
  f5(y2d, 0);             // expected-error {{passing 'int checked[10][10]' to parameter of incompatible type 'int (*)[10]'}}

   // f6(ptr<int[10]>, int y);
  f6(x, 0);
  f6(y, 0);               // expected-error {{passing 'int checked[10]' to parameter of incompatible type '_Ptr<int [10]>'}}
  f6(x2d, 0);
  f6(y2d, 0);             // expected-error {{passing 'int checked[10][10]' to parameter of incompatible type '_Ptr<int [10]>'}}

   // f7(array_ptr<int[10]>, int y);
  f7(x, 0);
  f7(y, 0);               // expected-error {{parameter of incompatible type}}
  f7(x2d, 0);
  f7(y2d, 0);             // expected-error {{parameter of incompatible type}}

  // f8(int (*p) checked[10], int y);
  f8(x, 0);
  f8(y, 0);               // expected-error {{parameter of incompatible type}}
  f8(x2d, 0);
  f8(y2d, 0);             // expected-error {{parameter of incompatible type}}

  // f9(ptr<int checked[10]> p, int y);
  f8(x, 0);
  f8(y, 0);               // expected-error {{parameter of incompatible type}}
  f8(x2d, 0);
  f8(y2d, 0);             // expected-error {{parameter of incompatible type}}

  // f10(array_ptr<int checked[10]> p, int y);
  f10(x, 0);
  f10(y, 0);               // expected-error {{parameter of incompatible type}}
  f10(x2d, 0);
  f10(y2d, 0);             // OK

  // f11(int p[10][10], int y);
  f11(x, 0);
  f11(y, 0);              // expected-error {{parameter of incompatible type}}
  f11(x2d, 0);
  f11(y2d, 0);            // expected-error {{parameter of incompatible type}}

  // f12(int p checked[10][10], int y);
  f12(x, 0);
  f12(y, 0);              // expected-error {{parameter of incompatible type}}
  f12(x2d, 0);
  f12(y2d, 0);            // OK

  // f13(_Bool b, int y);
  f13(x, 0);
  f13(y, 0);              // OK
  f13(x2d, 0);
  f13(y2d, 0);            // OK

  // spot check calls where an array is the second argument
  g1(0, x);
  g1(0, y);   // expected-error {{parameter of incompatible type}}
  g2(0, x);
  g2(0, y);   // expected-error {{parameter of incompatible type}}
  g3(0, x);
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
  f1_void(p, val);    // param ptr<void>, arg int[10] OK.
  f3_void(r, val);    // param array_ptr<void>, arg int checked[10] OK.
  f3_void(p, val);    // param array_ptr<void>, arg int[10] OK, provided that param has no bounds.

  // Expected to not typecheck
  f1_void(r, val);    // expected-error {{incompatible type}}
                      // param void *, arg int checked[10] not OK
  f2_void(r, val);    // expected-error {{incompatible type}}
                      // param ptr<void>, arg int checked[10] not OK

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
  f3(s, 0);
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
checked ptr<int[10]> h20(void) {
  return global_arr2; // expected-error {{cannot use a variable with an unchecked type}}
}

checked array_ptr<int[10]> h21(int arr[10][10]) { // expected-error {{parameter cannot have an unchecked array type}}
  return arr;
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
  return arr;  // ptr<T> = array_ptr<T> OK
}

checked array_ptr<int checked[10]> h27(int arr checked[10][10]) {
  return arr;
}



checked void check_pointer_arithmetic(void) {
  int p[5];           // expected-error {{variable cannot have an unchecked array type}}
  int r checked[5];

  int *p_tmp;         // expected-error {{variable cannot have an unchecked pointer type}}
  array_ptr<int> r_tmp;

  p_tmp = p + 5;
  p_tmp = 5 + p;
  p_tmp = p_tmp - 2;
  p_tmp = 2 - p;
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
  p_tmp = 0 - p;

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

checked void check_pointer_difference(int flag) {
  int count;
  int val_int[5];                 // expected-error {{variable cannot have an unchecked array type}}
  float val_float[5];             // expected-error {{variable cannot have an unchecked array type}}
  int *p_int = val_int;           // expected-error {{variable cannot have an unchecked pointer type}}
  float *p_float = val_float;     // expected-error {{variable cannot have an unchecked pointer type}}
  array_ptr<int> r_int = val_int;

  int a_int[5];                   // expected-error {{variable cannot have an unchecked array type}}
  int checked_a_int checked[5];

  float a_float[5];               // expected-error {{variable cannot have an unchecked array type}}
  float checked_a_float checked[5];

  if (flag) {
      p_int = a_int;
      r_int = checked_a_int;
  }

  // pointer - array
  count = p_int - a_int;
  count = p_int - checked_a_int;
  count = r_int - a_int;
  count = r_int - checked_a_int;

  // array - pointer
  count = a_int - p_int;
  count = checked_a_int - p_int;
  count = a_int - r_int;
  count = checked_a_int - r_int;

  // spot check mismatched types
  count = a_float - p_int;
  count = p_int - checked_a_float;
  count = checked_a_float - r_int;  // expected-error {{not pointers to compatible types}}
}

checked void check_pointer_relational_compare(void) {
  int result;

  int val_int[5];                 // expected-error {{variable cannot have an unchecked array type}}
  float val_float[5];             // expected-error {{variable cannot have an unchecked array type}}
  int checked_val_int checked[5];
  float checked_val_float checked[5];

  int *p_int = val_int;           // expected-error {{variable cannot have an unchecked pointer type}}
  float *p_float = val_float;     // expected-error {{variable cannot have an unchecked pointer type}}

  ptr<int> q_int = 0;
  q_int = val_int;
  ptr<float> q_float = 0;
  q_float = val_float;

  array_ptr<int> r_int = val_int;
  array_ptr<float> r_float = val_float;

  // relational comparisons between pointers and unchecked arrays;
  result = val_int < p_int;
  result = val_int <= q_int;
  result = val_int >= r_int;

  result = p_int > val_int;
  result = q_int < val_int;
  result = r_int <= val_int;


  // relational comparisons between pointers and checked arrays;
  result = checked_val_int < p_int;
  result = checked_val_int <= q_int;
  result = checked_val_int >= r_int;

  result = p_int > checked_val_int;
  result = q_int < checked_val_int;
  result = r_int <= checked_val_int;

  // invalid relational comparisons

  // spot check comparisons between pointers and unchecked arrays;
  result = val_int < p_float;
  result = val_float <= q_int;
  result = val_int >= r_float;

  result = p_int > val_float;
  result = q_float < val_int;
  result = r_int <= val_float;

  // spot check comparisons between pointers and checked arrays;
  result = checked_val_int < p_float;
  result = checked_val_float <= q_int; // expected-warning {{comparison of distinct pointer types}}
  result = checked_val_int >= r_float; // expected-warning {{comparison of distinct pointer types}}

  result = p_int > checked_val_float;
  result = q_float < checked_val_int;  // expected-warning {{comparison of distinct pointer types}}
  result = r_int <= checked_val_float; // expected-warning {{comparison of distinct pointer types}}
}

checked void check_pointer_equality_compare(void) {
  int result;

  int val_int[5];                 // expected-error {{variable cannot have an unchecked array type}}
  float val_float[5];             // expected-error {{variable cannot have an unchecked array type}}
  int checked_val_int checked[5];
  float checked_val_float checked[5];

  int *p_int = val_int;           // expected-error {{variable cannot have an unchecked pointer type}}
  float *p_float = val_float;     // expected-error {{variable cannot have an unchecked pointer type}}

  ptr<int> q_int = 0;
  q_int = val_int;
  ptr<float> q_float = 0;
  q_float = val_float;

  array_ptr<int> r_int = val_int;
  array_ptr<float> r_float = val_float;

  // equality/inequality comparisons between pointers and unchecked arrays;
  result = val_int == p_int;
  result = val_int != q_int;
  result = val_int == r_int;

  result = p_int != val_int;
  result = q_int == val_int;
  result = r_int != val_int;

  // equality/inequality comparisons between pointers and checked arrays;
  result = checked_val_int == p_int;
  result = checked_val_int != q_int;
  result = checked_val_int == r_int;

  result = p_int != checked_val_int;
  result = q_int == checked_val_int;
  result = r_int != checked_val_int;

  // invalid equality/inequality comparisons

  // spot check equality comparisons between pointers and unchecked arrays;
  result = val_int == p_float;
  result = val_float != q_int;
  result = val_int == r_float;

  result = p_int != val_float;
  result = q_float == val_int;
  result = r_int != val_float;

  // spot check equality comparisons between pointers and checked arrays;
  result = checked_val_int == p_float;
  result = checked_val_float != q_int;  // expected-warning {{comparison of distinct pointer types}}
  result = checked_val_int != r_float;  // expected-warning {{comparison of distinct pointer types}}

  result = p_int == checked_val_float;
  result = q_float != checked_val_int;  // expected-warning {{comparison of distinct pointer types}}
  result = r_int == checked_val_float;  // expected-warning {{comparison of distinct pointer types}}
}

checked void check_logical_operators(void) {
  int p[5]; // expected-error {{variable cannot have an unchecked array type}}
  int r checked[5];

  _Bool b;

  b = !p;
  b = !r;

  b = p || b;
  b = r || b;
  b = b || p;
  b = b || r;

  b = r || p;
  b = p || r;

  b = p && b;
  b = r && b;
  b = b && p;
  b = b && r;

  b = r && p;
  b = p && r;
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
  parr = (int(*)checked[3]) &arr;   // expected-error {{incompatible type}}
  parr = (int(*)[5]) &arr;          // expected-error {{invalid casting to unchecked pointer type}}

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

  int *upa;                         // expected-error {{variable cannot have an unchecked pointer type}}
  upa = arr;
  upa = (int *)(array_ptr<int>)arr; // expected-error {{invalid casting to unchecked pointer type in a checked scope}}
  upa = &x;
  upa = (int [])&x;                 // expected-error {{cast to incomplete type}}
  upa = (int(*)[5])&x;              // expected-error {{invalid casting to unchecked}}

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
}
checked void check_illegal_operators(void) {
  int p[5]; // expected-error {{variable cannot have an unchecked array type}}
  int r checked[5];
  p * 5;
  5 * p;
  p *= 5;

  r * 5;  // expected-error {{invalid operands to binary expression}}
  5 * r;  // expected-error {{invalid operands to binary expression}}
  r *= 5; // expected-error {{invalid operands to binary expression}}

  p * p;
  p *= p;

  r * r;  // expected-error {{invalid operands to binary expression}}
  r *= r; // expected-error {{invalid operands to binary expression}}

  //
  // Test /
  //

  p / 5;
  5 / p;
  p /= 5;

  r / 5;  // expected-error {{invalid operands to binary expression}}
  5 / r;  // expected-error {{invalid operands to binary expression}}
  r /= 5; // expected-error {{invalid operands to binary expression}}

  p / p;
  p /= p;

  r / r;  // expected-error {{invalid operands to binary expression}}
  r /= r; // expected-error {{invalid operands to binary expression}}

  //
  // Test %
  //

  p % 5;
  5 % p;
  p %= 5;

  r % 5;  // expected-error {{invalid operands to binary expression}}
  5 % r;  // expected-error {{invalid operands to binary expression}}
  r %= 5; // expected-error {{invalid operands to binary expression}}

  p % p;
  p %= p;

  r % r;  // expected-error {{invalid operands to binary expression}}
  r %= r; // expected-error {{invalid operands to binary expression}}

  //
  // Test <<
  //

  p << 5;
  5 << p;
  p <<= 5;

  r << 5;  // expected-error {{invalid operands to binary expression}}
  5 << r;  // expected-error {{invalid operands to binary expression}}
  r <<= 5; // expected-error {{invalid operands to binary expression}}

  p << p;
  p <<= p;

  r << r;  // expected-error {{invalid operands to binary expression}}
  r <<= r; // expected-error {{invalid operands to binary expression}}

  //
  // Test >>
  //
  p >> 5;
  5 >> p;
  p >>= 5;

  r >> 5;  // expected-error {{invalid operands to binary expression}}
  5 >> r;  // expected-error {{invalid operands to binary expression}}
  r >>= 5; // expected-error {{invalid operands to binary expression}}

  p >> p;
  p >>= p;

  r >> r;  // expected-error {{invalid operands to binary expression}}
  r >>= r; // expected-error {{invalid operands to binary expression}}

  //
  // Test |
  //

  p | 5;
  5 | p;
  p |= 5;

  r | 5;  // expected-error {{invalid operands to binary expression}}
  5 | r;  // expected-error {{invalid operands to binary expression}}
  r |= 5; // expected-error {{invalid operands to binary expression}}

  p | p;
  p |= p;

  r | r;  // expected-error {{invalid operands to binary expression}}
  r |= r; // expected-error {{invalid operands to binary expression}}

  //
  // Test &
  //

  p & 5;
  5 & p;
  p &= 5;

  r & 5;  // expected-error {{invalid operands to binary expression}}
  5 & r;  // expected-error {{invalid operands to binary expression}}
  r &= 5; // expected-error {{invalid operands to binary expression}}

  p & p;
  p &= p;

  r & r;  // expected-error {{invalid operands to binary expression}}
  r &= r; // expected-error {{invalid operands to binary expression}}

  //
  // Test ^
  //

  p ^ 5;
  5 ^ p;
  p ^= 5;

  r ^ 5;  // expected-error {{invalid operands to binary expression}}
  5 ^ r;  // expected-error {{invalid operands to binary expression}}
  r ^= 5; // expected-error {{invalid operands to binary expression}}

  p ^ p;
  p ^= p;

  r ^ r;  // expected-error {{invalid operands to binary expression}}
  r ^= r; // expected-error {{invalid operands to binary expression}}

  //
  // Test ~
  //
  ~p;
  ~r;  // expected-error {{invalid argument type}}

  //
  // Test unary -
  //
  -p;
  -r;  // expected-error {{invalid argument type}}
  //
  // Test unary +
  //
  +p;
  +r;  // expected-error {{invalid argument type}}
}
