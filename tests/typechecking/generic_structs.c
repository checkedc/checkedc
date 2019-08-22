// Test type checking of generic structs.
//
// RUN: %clang_cc1 -verify -verify-ignore-unexpected=note %s

//
// Test that only the right type variables are in scope.
//
void TestParamScope() {
 struct Foo _For_any(T) {
   T *x;
   U *y; // expected-error {{unknown type name 'U'}}
  };
}

//
// Test that type variables can only appear under pointers in
// a struct definition (because the size of the underlying type
// is unknown).
//
void TestOnlyPointersInFields() {
  struct Foo _For_any(T) {
    T *x;
    T  y; // expected-error {{field has incomplete type 'T'}}
  };
}

//
// Test that instantiating a struct with the wrong number of type arguments
// raises an error.
//
void TestWrongNumArgs() {
  struct Foo _For_any(U, V) {
  };

  struct Foo<int, char> FooOk;
  struct Foo<int> FooWrong1; // expected-error {{generic struct takes 2 type arguments, but instantiation provided 1}}
  struct Foo<int, int, int> FooWrong2; // expected-error {{generic struct takes 2 type arguments, but instantiation provided 3}}
}

//
// Test a simple type application where the struct
// isn't recursive.
//
void TestTypeApplicationSimple() {
  struct Foo _For_any(T) {
    T *x;
  };
  struct Foo<int> li;
  int *ele = li.x;
  char ele2 = li.x; // expected-warning {{incompatible pointer to integer conversion initializing 'char' with an expression of type 'int *'}}
}

//
// Test a generic struct instantiating a second generic struct.
//
void TestTwoStructs() {
  struct Box _For_any(T) {
    T *x;
  };

  struct BoxRef _For_any(T) {
    struct Box<T> BoxRef;
    struct Box<int> IntBox;
  };
}

//
// Test that type application applies transitively through
// multiple structs.
//
void TestTypeApplicationNested() {
  struct Box1 _For_any(T) {
    T *x1;
  };

  struct Box2 _For_any(T) {
    struct Box1<T> x2;
  };

  struct Box3 _For_any(T) {
    struct Box2<T> x3;
  };

  struct Box3<int> box3;
  int *x = box3.x3.x2.x1;
  char *y = box3.x3.x2.x1; // expected-warning {{incompatible pointer types initializing 'char *' with an expression of type 'int *'}}
}

//
// Test a struct with multiple type arguments.
//
void TestMultArgs() {
  struct Pair _For_any(U, V) {
    U *u;
    V *v;
  };
  struct Pair<int, char> pair1;
  struct Pair<char, int> pair2;
  int *p11 = pair1.u;
  char *p12 = pair1.v;
  char *p21 = pair2.u;
  int *p22 = pair2.v;
}

//
// Test a list (this requires support for recursive structs).
//
void TestRecursiveStruct() {
  struct List _For_any(T) {
    T *head;
    struct List<T> *tail;
  };
  struct List<char> *list;
  char *l1 = list->head;
  char *l2 = list->tail->head;
  char *l3 = list->tail->tail->head;

  // Test that the pointer types are compatible:
  struct List<char> *list2 = list; 
  list->head = list2->head;
  list->tail->head = list2->tail->head;
  list->tail->tail->head = list2->tail->tail->head;
  list->tail->head = list2->head;
}

//
// Test polymorphic recursion (a type application within a definition which uses
// different type arguments).
//
void TestPolyRec() {
  struct Foo _For_any(U) {
    U *x;	
  };

  struct Bar _For_any(V) {
    struct Foo<V> *foo;
    struct Foo<char> *foo_char;
    struct Bar<char> *bar_char;
  };

  struct Bar<int> *bar;
  struct Foo<int> *foo = bar->foo;
  struct Foo<char> *foo_char = bar->foo_char;
  struct Bar<char> *bar_char = bar->bar_char;
}

//
// Test expanding cycles.
// There are cases where, if we're not careful, dependencies between
// generic structs can lead to the creation of infinitely many record instances (crashing the compiler).
// These are ruled out via a check for "expanding cycles".
// See "On Decidability of Nominal Subtyping with Variance" by Kennedy and Pierce for the similar
// notion of "expanding inheritance".
//
void TestExpandingCycles() {
  struct List _For_any(T) { // expected-error {{expanding cycle in struct definition}}
    struct List<struct List<T> > *expanding;
  };

  struct Foo _For_any(T) {
    T *x;
  };

  struct Bad _For_any(T) { // expected-error {{expanding cycle in struct definition}}
    struct Bad<struct Foo<T> > *bad;
  };

  struct Good _For_any(T) {
    struct Foo<struct Good<T> > *good;
  };

  struct Good2 _For_any(T) {
    struct Foo<struct Foo<T> > *good;
  }; 

  struct Good2<int> *s;
  struct Foo<struct Foo<int> > *s2 = s->good;
  struct Foo<int> *s3 = s2->x;
  int *x = s3->x;
}

void TestExpandingCyclesWithForwardDecls() {
  // Mutually-recursive structs that are valid because
  // there are no expanding cycles.
  struct A _For_any(T);
  struct B _For_any(T);

  struct A _For_any(T) {
    struct B<T> *other;
  };

  struct B _For_any(T) {
    struct A<T> *other;
  };

  struct A<int>* a;
  struct B<int>* b = a->other;
  struct A<int>* b2 = a->other->other;

  // Invalid because of expanding cyclec.
  struct Bad1 _For_any(T);
  struct Bad2 _For_any(T);

  struct Bad1 _For_any(T) {
    struct Bad2<T> *x;
  };

  struct Bad2 _For_any(T) { // expected-error {{expanding cycle in struct definition}}
    struct Bad1<struct Bad2<T> > *x;
  };
}

void TestExpandingCyclesLongCycle() {
  // No expanding cycle here since there are no expanding edges.
  struct A1 _For_any(T);
  struct A2 _For_any(T);
  struct A3 _For_any(T);
  struct A4 _For_any(T);
  struct A5 _For_any(T);

  struct A1 _For_any(T) { struct A2<T> *x; };
  struct A2 _For_any(T) { struct A3<T> *x; };
  struct A3 _For_any(T) { struct A4<T> *x; };
  struct A4 _For_any(T) { struct A5<T> *x; };
  struct A5 _For_any(T) { struct A1<T> *x; };

  struct A1<int> *a1;
  struct A2<int> *a2 = a1->x;
  struct A3<int> *a3 = a2->x;
  struct A4<int> *a4 = a3->x;
  struct A5<int> *a5 = a4->x;
  a1 = a5->x;

  // B3 has an expanding edge
  struct B1 _For_any(T);
  struct B2 _For_any(T);
  struct B3 _For_any(T);
  struct B4 _For_any(T);
  struct B5 _For_any(T);

  struct B1 _For_any(T) { struct B2<T> *x; };
  struct B2 _For_any(T) { struct B3<T> *x; };
  struct B3 _For_any(T) { struct B4<struct B5<T> > *x; };
  struct B4 _For_any(T) { struct B5<T> *x; };
  struct B5 _For_any(T) { struct B1<T> *x; }; // expected-error {{expanding cycle in struct definition}}
}

//
// Test that the type application completion logic trigers in the right places
//
void TestApplicationOrder() {
  struct B _For_any(U);

  struct A _For_any(T) {
    T *t;
    struct B<T> *b; // Can't be completed at this point
  };

  struct B _For_any(U) {
    struct A<U> *a; // Needs to be completed, even though it's a field
  };

  struct A<char> *a;
  char *c = a->t;
  struct B<char> *b = a->b;
  struct A<char> *a2 = b->a;
}

//
// Test that we can detect expanding cycles involving function types.
//
void TestExpandingCycleInFunctionTypes() {
  struct A _For_any(T) { // ok: no expanding cycles
    void (*f1)(struct A<struct A<char> >);
    struct A<struct A<char> > (*f2)();
    void (*f3)(struct A<T>);
    struct A<T> (*f4)();
    struct A<void (*)(struct A<int>)> *a;
  };

  struct B _For_any(T) { // expected-error {{expanding cycle in struct definition}}
    void (*f)(struct B<struct B<T> >); // expanding cycle in param
  };

  struct C _For_any(T) { // expected-error {{expanding cycle in struct definition}}
    struct C<struct C<T> > (*f)(void); // expanding cycle in return type
  };

  // Test K&R style functions
  struct C2 _For_any(T) { // expected-error {{expanding cycle in struct definition}}
    struct C2<struct C2<T> > (*f)(); // expanding cycle in return type
  };

  struct D _For_any(T) { // expected-error {{expanding cycle in struct definition}}
    struct D<void (*)(T)> *d; // expanding cycle in type application
  };

  struct E _For_any(T) { // expected-error {{expanding cycle in struct definition}}
    struct E<T* (*)(int)> *e; // expanding cycle in type application
  };
}

//
// Test that we can detect expanding cycles involving array types.
//
void TestExpandinCycleInArrayType() {
  struct A _For_any(T) { // expected-error {{expanding cycle in struct definition}}
    struct A<T*[]> *a; // array within generic leads to expanding cycle
  };

  struct A2 _For_any(T) {
    T* a[10]; // allowed
  };

  struct B _For_any(T) { // expected-error {{expanding cycle in struct definition}}
    struct B<struct B<T> > *b[10]; // generic inside array leads to expanding cycle
  };

  struct B2 _For_any(T) {
      struct B2<T> *b[10]; // allowed
  };
}

// Test that we can handle typedefs used as arguments
// to type applications.
void TestTypedefArg() {
  typedef int int1;
  typedef int int2;
  struct Foo _For_any(T) {
    T *x;
  };
  struct Foo<int1> f1;
  struct Foo<int2> f2;
  f1 = f2; // ok
  f2 = f1; // ok
}
//
// Test that we raise an error if the type arguments are missing from a generic
// struct instance.
//
void TestMissingTypeArguments() {
  struct A _For_any(T) {
    T *x;
  };
  struct A a; // expected-error {{expected a type argument list for a generic struct type}}
  struct A<int> a2; // ok
}