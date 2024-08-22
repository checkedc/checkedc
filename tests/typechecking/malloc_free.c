// Feature tests for type checking Checked C bounds-safe interface
// type annotations.
//
// The following lines are for the LLVM test harness:
//
// RUN: %clang -fsyntax-only -Werror %s

#include <stdchecked.h>
#include <stdlib_checked.h>

#pragma CHECKED_SCOPE ON

// Test you can always `free` a `malloc`d unchecked pointer
void f1(void) unchecked {
    int *x = malloc(sizeof(int));
    free(x);
}

// Test you can always `free` a `calloc`d unchecked pointer
void f2(void) unchecked {
    int *x = calloc(1, sizeof(int));
    free(x);
}

// Test you can always `free` a `realloc`d unchecked pointer
void f3(void) unchecked {
    int *x = malloc(sizeof(int));
    int *y = realloc(x, 2 * sizeof(int));
    free(y);
}

// Test you can always `free` a `aligned_alloc`d unchecked pointer
void f4(void) unchecked {
    int *x = aligned_alloc(_Alignof(int), sizeof(int));
    free(x);
}

// Test you can always `free` a `malloc`d ptr
void f11(void) {
    int* _Single x = malloc _TyArgs(int) (sizeof(int));
    free<int>(x);
}

// Test you can always `free` a `calloc`d ptr
void f12(void) unchecked {
    // Intentionally vary test from f11: type argument can be omitted
    // in an unchecked scope.
    ptr<int> x = calloc(1, sizeof(int));
    free(x);
}

// Test you can always `free` a `realloc`d ptr
void f13(void) unchecked {
    // Intentionally vary test from f11: type argument can be omitted
    // in an unchecked scope.
    ptr<int> x = malloc(sizeof(int));
    ptr<int> y = realloc<int>(x, 2 * sizeof(int));
    free(y);
}

// Test you can always `free` a `aligned_alloc`d ptr
void f14(void) {
    ptr<int> x = aligned_alloc(_Alignof(int), sizeof(int));
    free<int>(x);
}

// Test you can always `free` a `malloc`d array_ptr
void f21(void) {
    array_ptr<int> x : count(4) = malloc<int>(4 * sizeof(int));
    free<int>(x);
}

// Test you can always `free` a `calloc`d array_ptr
void f22(void) {
    array_ptr<int> x : count(4) = calloc<int>(4, sizeof(int));
    free<int>(x);
}

// Test you can always `free` a `realloc`d array_ptr
void f23(void) {
    int* _Array x  _Count(4) = malloc _TyArgs(int) (4 * sizeof(int));
    int* _Array y  _Count(8) = realloc _TyArgs(int) (x, 8 * sizeof(int));
    free _TyArgs(int) (y);
}

// Test you can always `free` a `aligned_alloc`d array_ptr
void f24(void) {
    array_ptr<int> x : count(4) = aligned_alloc(_Alignof(int), 4 * sizeof(int));
    free<int>(x);
}
