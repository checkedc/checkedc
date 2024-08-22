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
	exist.elem = 0; // expected-error {{member reference base type 'Exists(T, struct Foo<T>)' is not a structure or union}}
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

_Exists(A, struct Foo<A>) foo1;
_Exists(A, _Exists(B, struct Foo<A>)) foo2;
_Exists(A, _Exists(B, struct Foo<B>)) foo3;
_Exists(A, _Exists(B, _Exists(C, struct Foo<int>))) foo4;

// Test that existential types can handle different levels of
// depth while still being detected as compatible.
_For_any(T) void TestCanonicalization(T *x) {
    _Exists(A, struct Foo<A>) bar1;
    _Exists(A, _Exists(B, struct Foo<A>)) bar2;
    _Exists(A, _Exists(B, struct Foo<B>)) bar3;
    _Exists(A, _Exists(B, _Exists(C, struct Foo<int>))) bar4;

    bar1 = foo1;
    bar2 = foo2;
    bar3 = foo3;
    bar4 = foo4;

    foo1 = bar1;
    foo2 = bar2;
    foo3 = bar3;
    foo4 = bar4;

    foo1 = foo1;
    foo2 = foo2;
    foo3 = foo3;
    foo4 = foo4;

    bar1 = bar1;
    bar2 = bar2;
    bar3 = bar3;
    bar4 = bar4;

    // TODO: fix 'struct Foo<T>' in canonical types below after we've fixed canonicalization of type applications
    bar1 = foo2; // expected-error {{assigning to 'Exists(A, struct Foo<B>)' from incompatible type 'Exists(A, Exists(B, struct Foo<T>))'}}
    bar2 = foo3; // expected-error {{assigning to 'Exists(A, Exists(B, struct Foo<B>))' (aka 'Exists((0, 0), Exists((1, 0), struct Foo<T>))') from incompatible type 'Exists(A, Exists(B, struct Foo<B>))' (aka 'Exists((0, 0), Exists((1, 0), struct Foo<B>))')}}
    bar3 = foo4; // expected-error {{assigning to 'Exists(A, Exists(B, struct Foo<B>))' from incompatible type 'Exists(A, Exists(B, Exists(C, struct Foo<int>)))'}}
    bar4 = foo1; // expected-error {{assigning to 'Exists(A, Exists(B, Exists(C, struct Foo<int>)))' from incompatible type 'Exists(A, struct Foo<T>)'}}

    _Exists(A, _Exists(B, struct Foo<T>)) zoom1;
    zoom1 = foo2; // expected-error {{assigning to 'Exists(A, Exists(B, struct Foo<T>))' (aka 'Exists((1, 0), Exists((2, 0), struct Foo<T>))') from incompatible type 'Exists(A, Exists(B, struct Foo<T>))' (aka 'Exists((0, 0), Exists((1, 0), struct Foo<T>))')}}
    zoom1 = foo3; // expected-error {{assigning to 'Exists(A, Exists(B, struct Foo<T>))' from incompatible type 'Exists(A, Exists(B, struct Foo<B>))'}}
}

// Test that the names of bound variables inside existential types don't matter
// for compatibility purposes.
void TestAlphaEquivalence() {
    struct Pair _For_any(T1, T2) {
    };

    _Exists(A, struct Foo<A>) a1;
    _Exists(B, struct Foo<B>) a2;
    a1 = a2;
    a2 = a1;

    _Exists(A, _Exists(B, struct Pair<int, char>)) b1;
    _Exists(T1, _Exists(T2, struct Pair<int, char>)) b2;
    b1 = b2;
    b2 = b1;

    _Exists(A, A*****) c1;
    _Exists(B, B*****) c2;
    c1 = c2;
    c2 = c1;

    struct Foo<_Exists(A, A *)> foo1;
    struct Foo<_Exists(B, B *)> foo2;
    foo1 = foo2;
    foo2 = foo1;

    _Exists(A, _Exists(B, struct Foo<_Exists(C, struct Foo<B>)>)) d1;
    _Exists(T1, _Exists(T2, struct Foo<_Exists(T3, struct Foo<T2>)>)) d2;
    d1 = d2;
    d2 = d1;
}

// Test that parsing malformed existential types fails without
// a compiler crash.
void TestParseMalformedExistential() {
  _Exists(int, T) e1; // expected-error {{expected type variable identifier}} expected-warning {{type specifier missing, defaults to 'int'}}
  _Exists(int T) e2;  // expected-error {{expected type variable identifier}} expected-warning {{type specifier missing, defaults to 'int'}}
  _Exists(T int) e3;  // expected-error {{expected ','}} expected-warning {{type specifier missing, defaults to 'int'}}
  _Exists(T, int e4;  // expected-error {{expected ')'}} expected-warning {{type specifier missing, defaults to 'int'}}
  _Exists(T, T*)) e5; // expected-error {{expected identifier or '('}}
}

// Test that while typechecking _Pack expressions we check that the return type
// (which is the second argument to the pack) is an existential type.
// If not, we should display an error.
void TestPackReturnExpectsExistential() {
  struct Foo _For_any(T) {};
  struct Foo<int> fooInt;
  _Exists(T, struct Foo<T>) fooExists = _Pack(fooInt, struct Foo<int>, int); // expected-error {{return type of a pack expression must be an existential type, but got 'struct Foo<int>' instead}}
}

// Test that we display an error message if the user tries
// to unpack multiple type variables (this will be supported in the future).
void TestUnpackWithMultipleTypeVars() {
  struct Foo _For_any(T) {};
  _Exists(T, struct Foo<T>) fooExist;
  _Unpack (A, B, C) struct Foo<A> fooAbs = fooExist; // expected-error {{expected ')'}} expected-error {{expected identifier or '('}}
}

// Test that we check that the initializer to an unpack declaration
// has an existential type.
void TestUnpackRequiresExistentialInit() {
  struct Foo _For_any(T) {};
  struct Foo<int> fooInt;
  _Exists(B, struct Foo<B>) fooExist = _Pack(fooInt, _Exists(C, struct Foo<C>), int);
  _Unpack (A) struct Foo<A> fooA = fooInt; // expected-error {{unpack specifer expects an initializer that has an existential type}}
  _Unpack (B) struct Foo<B> fooB = fooExist;
}

// Test that we can handle a malformed unpack specifier that's missing the type variable.
void TestUnpackMissingTypeVariable() {
  struct Foo _For_any(T) {};
  _Unpack (int) struct Foo<A> fooA; // expected-error {{expected type variable identifier}} expected-error {{unknown type name 'A'}}
}