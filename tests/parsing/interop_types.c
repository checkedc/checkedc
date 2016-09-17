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
extern void f1(int *p : ptr<int>, int y) {
   *p = y;
}

extern void f2(int *p : array_ptr<int>, int y) {
}

extern void f3(int **p : ptr<ptr<int>>, int y) {
  **p = y;
}

extern void f4(int **p : array_ptr<ptr<int>>, int y) {
}

// Second parameter has interop type annotation
extern void g1(int y, int *p : ptr<int>) {
   *p = y;
}

extern void g2(int y, int *p : array_ptr<int>) {
}

extern void g3(int y, int **p : ptr<ptr<int>>) {
   y = **p;
}

extern void g4(int y, int **p : ptr<array_ptr<int>>) {
}

extern void g5(int y, int **p : array_ptr<ptr<int>>) {
}

//
// returns an unchecked pointer type with an
// interop type annotation.
//

extern int *h1(int y, ptr<int> p) : ptr<int> {
   *p = y;
   return 0;
}

extern int *h2 (int y, const ptr<int> p) : array_ptr<int> {
   return 0;
}

extern int **h3() : ptr<ptr<int>> {
   return 0;
}

extern int **h4() : array_ptr<ptr<int>> {
   return 0;
}

//
// Global variables with interop type annotations
//

int *a1 : ptr<int> = 0;
int *a2 : array_ptr<int> = 0;
int **a3 : ptr<ptr<int>> = 0;
int **a4 : ptr<array_ptr<int>> = 0;
int **a5 : array_ptr<ptr<int>> = 0;
int **a6 : array_ptr<array_ptr<int>> = 0;
int ***a7 : ptr<ptr<ptr<int>>> = 0;

//
// Structure members with interop pointer type annotations
//

struct S1 {
  float *data1 : ptr<float>;
  float *data2 : array_ptr<float>;
  float **data3 : ptr<ptr<float>>;
  float **data4 : ptr<array_ptr<float>>;
  float **data5 : array_ptr<ptr<float>>;
  float ***data6 : ptr<ptr<ptr<float>>>;
};
