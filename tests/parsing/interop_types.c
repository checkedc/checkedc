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
extern void f1(int *p : itype(ptr<int>), int y) {
   *p = y;
}

extern void f2(int *p : itype(array_ptr<int>), int y) {
}

extern void f3(int **p : itype(ptr<ptr<int>>), int y) {
  **p = y;
}

extern void f4(int **p : itype(array_ptr<ptr<int>>), int y) {
}

// Second parameter has interop type annotation
extern void g1(int y, int *p : itype(ptr<int>)) {
   *p = y;
}

extern void g2(int y, int *p : itype(array_ptr<int>)) {
}

extern void g3(int y, int **p : itype(ptr<ptr<int>>)) {
   y = **p;
}

extern void g4(int y, int **p : itype(ptr<array_ptr<int>>)) {
}

extern void g5(int y, int **p : itype(array_ptr<ptr<int>>)) {
}

//
// returns an unchecked pointer type with an
// interop type annotation.
//

extern int *h1(int y, ptr<int> p) : itype(ptr<int>) {
   *p = y;
   return 0;
}

extern int *h2 (int y, const ptr<int> p) : itype(array_ptr<int>) {
   return 0;
}

extern int **h3() : itype(ptr<ptr<int>>) {
   return 0;
}

extern int **h4() : itype(array_ptr<ptr<int>>) {
   return 0;
}

//
// Global variables with interop type annotations
//

int *a1 : itype(ptr<int>) = 0;
int *a2 : itype(array_ptr<int>) = 0;
int **a3 : itype(ptr<ptr<int>>) = 0;
int **a4 : itype(ptr<array_ptr<int>>) = 0;
int **a5 : itype(array_ptr<ptr<int>>) = 0;
int **a6 : itype(array_ptr<array_ptr<int>>) = 0;
int ***a7 : itype(ptr<ptr<ptr<int>>>) = 0;

//
// Structure members with interop pointer type annotations
//

struct S1 {
  float *data1 : itype(ptr<float>);
  float *data2 : itype(array_ptr<float>);
  float **data3 : itype(ptr<ptr<float>>);
  float **data4 : itype(ptr<array_ptr<float>>);
  float **data5 : itype(array_ptr<ptr<float>>);
  float ***data6 : itype(ptr<ptr<ptr<float>>>);
};

///
/// The interop type can have modifiers
///
extern void f10(const int * const x : itype(const ptr<const int>)) {
}

extern void f11(const int *x : itype(ptr<const int>)) {
}

extern const int *f12() : itype(ptr<const int>) {
  return 0;
}

const int *a10 : itype(ptr<const int>) = 0;
int *const a11 : itype(const ptr<int>) = 0;

///
/// Typedef'ed names can be used as interop types
///

typedef ptr<int> pint;
typedef ptr<const int> pcint;

extern void f20(int *x : itype(pint)) {
}

extern void f21(const int *x : itype(pcint)) {
}
