// Test type checking of generic structs.
//
// RUN: %clang_cc1 -verify -verify-ignore-unexpected=note %s

//
// Test that only the right type variables are in scope.
//
struct T1_Foo _For_any(T) {
  T *x;
  U *y; // expected-error {{unknown type name 'U'}}
};

//
// Test that type variables can only appear under pointers in
// a struct definition (because the size of the underlying type
// is unknown).
//
struct T2_Foo _For_any(T) {
  T *x;
  T  y; // expected-error {{field has incomplete type 'T'}}
};

//
// Test a simple type application where the struct
// isn't recursive.
//
struct T3_Foo _For_any(T) {
  T *x;
};

void T3_TypeApplicationSimple() {
	struct T3_Foo<int> li;
	int *ele = li.x;
	char ele2 = li.x; // expected-warning {{incompatible pointer to integer conversion initializing 'char' with an expression of type 'int *'}}
}

//
// Test a generic struct instantiating a second generic struct.
//
struct T4_Box _For_any(T) {
  T *x;
};

struct T4_BoxRef _For_any(T) {
  struct T4_Box<T> BoxRef;
  struct T4_Box<int> IntBox;
};

//
// Test that type application applies transitively through
// multiple structs.
//
struct T5_Box1 _For_any(T) {
  T *x1;
};

struct T5_Box2 _For_any(T) {
  struct T5_Box1<T> x2;
};

struct T5_Box3 _For_any(T) {
  struct T5_Box2<T> x3;
};

void T5_TypeApplicationNested() {
  struct T5_Box3<int> box3;
  int *x = box3.x3.x2.x1;
  char *y = box3.x3.x2.x1; // expected-warning {{incompatible pointer types initializing 'char *' with an expression of type 'int *'}}
}

//
// Test a struct with multiple type arguments.
//
struct T6_Pair _For_any(U, V) {
  U *u;
  V *v;
};

void T6_MultArgs() {
  struct T6_Pair<int, char> pair1;
  struct T6_Pair<char, int> pair2;
  int *p11 = pair1.u;
  char *p12 = pair1.v;
  int *p21 = pair2.u;
  int *p22 = pair2.v;
}
