// Test type checking of generic structs.
//
// RUN: %clang_cc1 -verify -verify-ignore-unexpected=note %s

// A struct we can use with existential types.
struct Foo _For_any(T) {
	T *elem;
};

// Test that we can parse existential types both as function arguments
// and in (uninitialized) expressions.
void TestParseExistentialType(_Exists(T, struct Foo<T>) exist) {
	_Exists(U, struct Foo<U>) localVar;
}

void TestCantAccessFieldsInExistential(_Exists(T, struct Foo<T>) exist) {
	exist.elem = 0; // expected-error {{}}
}


