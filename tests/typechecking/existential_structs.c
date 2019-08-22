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

// Test that we can't directly access fields of an existential:
// we need to unwrap the existential first.
void TestCantAccessFieldsInExistential(_Exists(T, struct Foo<T>) exist) {
	exist.elem = 0; // expected-error {{member reference base type 'Exists((0, 0), struct Foo<T>)' is not a structure or union}}
}

// Test that pack returns an existential type.
void TestPackReturnType() {
	struct Foo<int> fooInt;
	_Exists(T, struct Foo<T>) foo = _Pack(fooInt, _Exists(U, struct Foo<U>), int);
}