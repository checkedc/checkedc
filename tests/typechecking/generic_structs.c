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
  char *p21 = pair2.u;
  int *p22 = pair2.v;
}

//
// Test a list (this requires support for recursive structs).
//
struct T7_List _For_any(T) {
  T *head;
  struct T7_List<T> *tail;
};

void T7_RecursiveStruct() {
  struct T7_List<char> *list;
  char *l1 = list->head;
  char *l2 = list->tail->head;
  char *l3 = list->tail->tail->head;

  // Test that the pointer types are compatible:
  struct T7_List<char> *list2 = list; 
  list->head = list2->head;
  list->tail->head = list2->tail->head;
  list->tail->tail->head = list2->tail->tail->head;
  list->tail->head = list2->head;
}

//
// Test polymorphic recursion (a type application within a definition which uses
// different type arguments).
//
struct T8_Foo _For_any(U) {
  U *x;	
};

struct T8_Bar _For_any(V) {
  struct T8_Foo<V> *foo;
  struct T8_Foo<char> *foo_char;
  struct T8_Bar<char> *bar_char;
};

void T8_PolyRec() {
  struct T8_Bar<int> *bar;
  struct T8_Foo<int> *foo = bar->foo;
  struct T8_Foo<char> *foo_char = bar->foo_char;
  struct T8_Bar<char> *bar_char = bar->bar_char;
}

//
// Test expanding cycles.
// There are cases where, if we're not careful, dependencies between
// generic structs can lead to the creation of infinitely many record instances (crashing the compiler).
// These are ruled out via a check for "expanding cycles".
// See "On Decidability of Nominal Subtyping with Variance" by Kennedy and Pierce for the similar
// notion of "expanding inheritance".
//
struct T9_List _For_any(T) { // expected-error {{expanding cycle in struct definition}}
  struct T9_List<struct T9_List<T> > *expanding;
};

struct T9_Foo _For_any(T) {
  T *x;
};

struct T9_Bad _For_any(T) { // expected-error {{expanding cycle in struct definition}}
  struct T9_Bad<struct T9_Foo<T> > *bad;
};

struct T9_Good _For_any(T) {
  struct T9_Foo<struct T9_Good<T> > *good;
};

struct T9_Good2 _For_any(T) {
  struct T9_Foo<struct T9_Foo<T> > *good;
};
