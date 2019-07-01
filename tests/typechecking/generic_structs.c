// Test type checking of generic structs.
//
// RUN: %clang_cc1 -verify -verify-ignore-unexpected=note %s

//
// Test that only the right type variables are in scope.
//
struct Foo _For_any(T) {
  T *x;
  U *y; // expected-error {{unknown type name 'U'}}
};

//
// Test that type variables can only appear under pointers in
// a struct definition (because the size of the underlying type
// is unknown).
//
struct Bar _For_any(T) {
  T *x;
  T  y; // expected-error {{field has incomplete type 'T'}}
};

//
// Test a simple type application where the struct
// isn't recursive.
//
struct L _For_any(T) {
  T *x;
};

void TypeApplicationSimple() {
	struct L<int> li;
	int *ele = li.x;
	char ele2 = li.x; // expected-warning {{incompatible pointer to integer conversion initializing 'char' with an expression of type 'int *'}}
}
