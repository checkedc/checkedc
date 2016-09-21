// Feature tests of parsing Checked C interop annotations that declare
// an alternate checked pointer type for a variable or member with
// unchecked pointer type.
//
// The following lines are for the LLVM test harness:
//
// RUN: %clang_cc1 -verify -fcheckedc-extension %s
// expected-no-diagnostics

#include "../../include/stdchecked.h"

//
// parameters with interop type annotations
//

// first parameter has interop type annotation
extern void f1(int *p : type(ptr<int>), int y) {
   *p = y;
}

extern void f2(int *p : type(array_ptr<int>), int y) {
}

extern void f3(int **p : type(ptr<ptr<int>>), int y) {
  **p = y;
}

extern void f4(int **p : type(array_ptr<ptr<int>>), int y) {
}

// Second parameter has interop type annotation
extern void g1(int y, int *p : type(ptr<int>)) {
   *p = y;
}

extern void g2(int y, int *p : type(array_ptr<int>)) {
}

extern void g3(int y, int **p : type(ptr<ptr<int>>)) {
   y = **p;
}

extern void g4(int y, int **p : type(ptr<array_ptr<int>>)) {
}

extern void g5(int y, int **p : type(array_ptr<ptr<int>>)) {
}

//
// returns an unchecked pointer type with an
// interop type annotation.
//

extern int *h1(int y, ptr<int> p) : type(ptr<int>) {
   *p = y;
   return 0;
}

extern int *h2 (int y, const ptr<int> p) : type(array_ptr<int>) {
   return 0;
}

extern int **h3() : type(ptr<ptr<int>>) {
   return 0;
}

extern int **h4() : type(array_ptr<ptr<int>>) {
   return 0;
}

//
// Global variables with interop type annotations
//

int *a1 : type(ptr<int>) = 0;
int *a2 : type(array_ptr<int>) = 0;
int **a3 : type(ptr<ptr<int>>) = 0;
int **a4 : type(ptr<array_ptr<int>>) = 0;
int **a5 : type(array_ptr<ptr<int>>) = 0;
int **a6 : type(array_ptr<array_ptr<int>>) = 0;
int ***a7 : type(ptr<ptr<ptr<int>>>) = 0;

//
// Structure members with interop pointer type annotations
//

struct S1 {
  float *data1 : type(ptr<float>);
  float *data2 : type(array_ptr<float>);
  float **data3 : type(ptr<ptr<float>>);
  float **data4 : type(ptr<array_ptr<float>>);
  float **data5 : type(array_ptr<ptr<float>>);
  float ***data6 : type(ptr<ptr<ptr<float>>>);
};

///
/// The interop type can have modifiers
///
extern void f10(const int * const x : type(const ptr<const int>)) {
}

extern void f11(const int *x : type(ptr<const int>)) {
}

extern const int *f12() : type(ptr<const int>) {
  return 0;
}

const int *a10 : type(ptr<const int>) = 0;
int *const a11 : type(const ptr<int>) = 0;

///
/// Typedef'ed names can be used as interop types
///

typedef ptr<int> pint;
typedef ptr<const int> pcint;

extern void f20(int *x : type(pint)) {
}

extern void f21(const int *x : type(pcint)) {
}
