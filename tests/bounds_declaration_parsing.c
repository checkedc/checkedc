// Feature tests of parsing new Checked C bounds declarations.
//
// The following lines are for the LLVM test harness:
//
// RUN: %clang_cc1 -verify -fcheckedc-extension %s

// TODO: typecheck bounds expressions in parameter declarations in a 
// scope with all of the parameters, not just the parameters already
// declared.  
//
// In a number of cases below, a programmer might want to declare 
// the count of elements after declaring the array_ptr.

extern void f1(array_ptr<int> arr : count(5)) {
}

extern void f2(array_ptr<int> arr : count(3 + 2)) {
}

extern void f3(int len, array_ptr<int> arr : count(len)) {
}

extern void f4(array_ptr<int> arr : byte_count(20)) {
}

extern void f5(array_ptr<int> arr : byte_count(5 * sizeof(int))) {
}

extern void f6(array_ptr<int> arr : bounds(arr, arr + 5)) {
}

extern void f7(int start, array_ptr<int> midarr : 
               bounds(midarr - start, midarr - start + 5)) {
}

extern void f8(array_ptr<int> arr : bounds(none)) {
}

// count is a contextual keyword.  It is only a keyword when it
// immediately follows the ':' in a bounds declaration.
extern void f9(int count, array_ptr<int> arr : count(count)) {
}

// none is a contextual keyword.  It is only a keyword when it
// immediately follows the ':' in a bounds declaration.
extern void f10(int none, array_ptr<int> arr : count(none)) {
}

// bounds is a contextual keyword.  It is only a keyword when it
// immediately follows the ':' in a bounds declaration.
extern void f11(int bounds, array_ptr<int> arr : bounds(arr, arr + 5)) {
}

extern void f12(array_ptr<int> lower, array_ptr<int> upper, 
               array_ptr<int> arr : bounds(lower, upper)) {
}

// The first parameter will have bounds(lower, upper)
extern void f13(array_ptr<int> arr : bounds(0, upper),  // expected-error {{undeclared identifier}}
                array_ptr<int> lower, array_ptr<int> upper) {
}

extern void f14(array_ptr<int> arr : 6 + 6) { // expected-error {{expected bounds expression}}
}

extern void f15(array_ptr<int> arr : 6 + 6, array_ptr<int> arr2 : count(5)) { // expected-error {{expected bounds expression}}
}

//
// Parsing of more complicated parameter declarators with bounds declarations
// 

// Incomplete array types with bounds declarations
extern void f16(int arr checked[] : count(5)) {
}

extern void f17(int arr checked[] : count(3 + 2)) {
}

extern void f18(int len, int arr checked[] : count(len)) {
}

// declare arrays of pointers to integers with bounds declarations
extern void f19(int *arr checked[] : count(5)) {
}

extern void f20(int *arr checked[] : count(3 + 2)) {
}

extern void f21(int len, int *arr checked[] : count(len)) {
}

// Checked array of pointers to functions.   Note that this shows a potential
// ambiguity in the grammar. We have to parenthesize the inner declarator
// to avoid : count(len) being parsed aas part of the type of arr, not a bounds
// expression.
extern void f22(int len, int ((*arr checked[])(int, int)) : count(len)) {
}

// checked array of pointers to functions
extern void f23(int len, int ((*arr checked[])(int, int)) : bounds(arr, arr + len)) {
}

