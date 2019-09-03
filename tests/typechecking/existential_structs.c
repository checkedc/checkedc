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

// Test that the type of the witness in _Pack(witness-expr, exist-type, subst-type)
// matches the given existential type.
void TestWitnessMismatch() {
  struct Foo<int> fooInt;
  struct Foo<struct Foo<int> > fooNested;
  _Exists(T, struct Foo<T>) foo = _Pack(fooInt, _Exists(T, struct Foo<T>), int); // ok: witness and expected types match
  _Pack(fooInt, _Exists(T, struct Foo<T>), char); // expected-error {{witness type does not match existential type}}
  _Pack(fooNested, _Exists(T, struct Foo<T>), int); // expected-error {{witness type does not match existential type}}
  _Exists(T, struct Foo<struct Foo<T> >) e = _Pack(fooNested, _Exists(T, struct Foo<struct Foo<T> >), int); // ok
  _Exists(T, struct Foo<T>) e2 = _Pack(fooNested, _Exists(T, struct Foo<T>), struct Foo<int>); // ok
}

// Test that the same expression can be packed in different ways,
// if we change the substitution and return types.
void TestMultiplePacks() {
  struct Foo<struct Foo<struct Foo<int> > > fooInt3;
  _Pack(fooInt3, _Exists(T, struct Foo<T>), struct Foo<struct Foo<int> >); // expected-warning {{expression result unused}}
  _Pack(fooInt3, _Exists(T, struct Foo<struct Foo<T> >), struct Foo<int>); // expected-warning {{expression result unused}}
  _Pack(fooInt3, _Exists(T, struct Foo<T>), struct Foo<struct Foo<int> >); // expected-warning {{expression result unused}}
}

// Test different packings of a struct that contains multiple type parameters.
void TestMultipleTypeParams() {
	struct Bar _For_any(A, B, C) {
	};
	struct Cat;
	struct Dog;
	struct Sheep;
	struct Bar<struct Cat, struct Dog, struct Sheep> bar;
	_Pack(bar, _Exists(T, struct Bar<T, struct Dog, struct Sheep>), struct Cat); // expected-warning {{expression result unused}}
	_Pack(bar, _Exists(T, struct Bar<struct Cat, T, struct Sheep>), struct Dog); // expected-warning {{expression result unused}}
	_Pack(bar, _Exists(T, struct Bar<struct Cat, struct Dog, T>), struct Sheep); // expected-warning {{expression result unused}}

	_Pack(bar, _Exists(T, struct Bar<T, T, T>), struct Cat); // expected-error {{witness type does not match existential type}}
}

// Test that we can pack and unpack a simple generic.
void TestSimpleUnpack() {
    struct Foo _For_any(A) {
        A *a;
        void (*op)(A*);
    };
    struct Foo<int> fooInt;
    _Exists(T2, struct Foo<T2>) packedFoo = _Pack(fooInt, _Exists(T2, struct Foo<T2>), int);

    // TODO: the error message should reference 'struct Foo<F>' and not 'struct Foo<T2>'.
    // This is happening because incorrect canonicalization of existential types (and of type applications).
    // Fix once that problem is fixed.
    // Let's do an incorrect unpack.
    // This is incorrect because _Unpack leaves 'U' abstract, so we can't recover the original
    // 'int' type argument.
    _Unpack (F) struct Foo<int> incorrect = packedFoo; // expected-error {{initializing 'struct Foo<int>' with an expression of incompatible type 'struct Foo<T2>'}}

    // Now let's try a correct unpack.
    _Unpack (U) struct Foo<U> unpackedFoo = packedFoo;
    U *a2 = unpackedFoo.a;
    unpackedFoo.op(a2);
}