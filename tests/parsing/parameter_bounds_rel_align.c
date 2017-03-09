// Feature tests of parsing new Checked C bounds declarations.
//
// The following lines are for the LLVM test harness:
//
// RUN: %clang_cc1 -verify -fcheckedc-extension %s

#include "../../include/stdchecked.h"

extern void f6(array_ptr<int> arr : bounds(arr, arr + 5) rel_align(char)) {
}

extern void f7(int start, array_ptr<int> midarr : 
               bounds(midarr - start, midarr - start + 5) rel_align(char)) {
}

extern void f7a(array_ptr<int> midarr :
   bounds(midarr - start, midarr - start + 5) rel_align(char), int start) {
}

extern void f8(array_ptr<int> arr : bounds(none)) {
}

// 'bounds' is a contextual keyword.  It is only a keyword when it
// immediately follows the ':' in a bounds declaration.
extern void f11(int bounds, array_ptr<int> arr : bounds(arr, arr + 5)  rel_align(char)) {
}

extern void f11a(array_ptr<int> arr : bounds(arr, arr + 5) rel_align(char), int bounds) {
}

extern void f12(array_ptr<int> lower, array_ptr<int> upper, 
                array_ptr<int> arr : bounds(lower, upper) rel_align(char)) {
}

extern void f12a(array_ptr<int> arr : bounds(lower, upper) rel_align(char),
                 array_ptr<int> lower, array_ptr<int> upper) {
}

// The first parameter will have bounds(lower, upper)
extern void f13(array_ptr<int> arr : bounds(lower, upper) rel_align(char),
                array_ptr<int> lower, array_ptr<int> upper) {
}


// checked array of pointers to functions
extern void f23(int len, int ((*arr checked[])(int, int)) : bounds(arr, arr + len) rel_align(char)) {
}

extern void f23a(int((*arr checked[])(int, int)) : bounds(arr, arr + len) rel_align(char), int len) {
}

// Misspell bounds to cause a parsing error.
extern void f26(int len,
                int arr : boounds(arr, arr + len) rel_align(char)) { // expected-error {{expected bounds expression}}
}

extern void f27(int len,
                int arr : boounds(arr, arr + len) rel_align(char), // expected-error {{expected bounds expression}}
                int flag) {
}

//  Bounds expressions with parenthesized subexpressions.
extern void f30(int len,
                array_ptr<int> arr : bounds(arr, (arr + len)) rel_align(char)) {
}

extern void f31(int len,
                array_ptr<int> arr : bounds((arr + len - (len)), (arr + len)) rel_align(char)) {
}

// Pointers to functions that have bounds declarations on parameters.
extern void f33(void) {
  // Use 'bounds' instead of 'count'
  ptr<int (array_ptr<int> arg : bounds(arg, arg + 5) rel_align(char))> r1 = 0;
  ptr<int (array_ptr<int> arg : bounds(arg, arg + n) rel_align(char), int n)> r2 = 0;
  ptr<int (int n, array_ptr<int> arg : bounds(arg, arg + n) rel_align(char))> r3 = 0;
  // Unchecked pointers to functions.
}
