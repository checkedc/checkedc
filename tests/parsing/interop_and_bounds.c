// Feature tests of parsing Checked C bounds annotations where an interop type
// and bounds annotation are declared for a variable or member with unchecked
// pointer type.
//
// The following lines are for the LLVM test harness:
//
// RUN: %clang_cc1 -verify %s

#include <stdchecked.h>

//
// parameters with interop type annotations
//

// First parameter has bounds annotations that use second parameter.
extern void f1(int **p : count(y) itype(array_ptr<ptr<int>>), int y) {
}

extern void f2(int **p : itype(array_ptr<ptr<int>>) count(y), int y) {
}

extern void f3(int **p : bounds(p, p + y) itype(array_ptr<ptr<int>>), int y) {
}

extern void f4(int **p : itype(array_ptr<ptr<int>>) bounds(p, p + y), int y) {
}

extern void f5(int **p : byte_count(y) itype(array_ptr<ptr<int>>), int y) {
}

extern void f6(int **p : itype(array_ptr<ptr<int>>) byte_count(y), int y) {
}


extern void f10(int **p : count(y) itype(ptr<int> checked[]), int y) {
}

extern void f11(int **p  _Itype(int* _Single checked[]) count(y), int y) {
}

extern void f12(int **p  _Itype(int* _Single checked[10]) count(10), int y) {
}

extern void f13(int **p _Count(10) _Itype(int* _Single checked[10]), int y) {
}

// Second parameter has interop type annotation
extern void g1(int y, int **p : count(y) itype(array_ptr<ptr<int>>)) {
}

extern void g2(int y, int **p : itype(array_ptr<ptr<int>>) count(y)) {
}

extern void g3(int y, int **p : bounds(p, p + y) itype(array_ptr<ptr<int>>)) {
}

extern void g4(int y, int **p : itype(array_ptr<ptr<int>>) bounds(p, p + y)) {
}

extern void g5(int y, int **p _Byte_count(y) _Itype(int* _Single *_Array)) {
}

extern void g6(int y, int **p  : itype(int* _Single *_Array) byte_count(y)) {
}


extern void g10(int y, int **p : count(y) itype(ptr<int> checked[])) {
}

extern void g11(int y, int **p  _Itype(int* _Single checked[]) count(y)) {
}

extern void g12(int y, int **p  _Itype(int* _Single checked[10]) count(10)) {
}

extern void g13(int y, int **p _Count(10)  itype(int* _Single checked[10])) {
}

//
// returns an unchecked pointer type with an
// interop type annotation.
//

extern int **h1(int y, ptr<int> p) : itype(array_ptr<ptr<int>>) byte_count(y) {
   *p = y;
   return 0;
}

extern int **h2(int y, int* _Single p) _Count(y) _Itype(int* _Single *_Array) {
   *p = y;
   return 0;
}

//
// Global variables with interop type and bounds annotations
//

int size;
int **a1 : itype(array_ptr<ptr<int>>) count(size) = 0;
int **a2 : count(size) itype(array_ptr<array_ptr<int>>) = 0;
int a3[10]  _Itype(int checked[10]) byte_count(10 * sizeof(int));
extern int a4[] _Count(size) _Itype(int checked[]);

//
// Structure members with interop pointer type annotations
//

struct S1 {
  int len;
  float *data1 : count(len) itype(array_ptr<float>);
  float *data2 : itype(array_ptr<float>) count(len);
  float **data3 : bounds(data3, data3 + len) itype(array_ptr<ptr<float>>);
  float **data4 : itype(array_ptr<ptr<float>>) bounds(data3, data3 + len);
  float data5[4] : itype(float checked[4]) count(3);
  float data6[4] _Count(3) _Itype(float checked[4]);
  float *data7[]  _Itype(ptr<float> checked[]) count(len);
};

struct S2 {
  int len;
  // Test another case a flexible array member at the end of struct 
  float *data7[] : count(len) itype(ptr<float> checked[]);
};

///
/// Typedef'ed names can be used as interop types
///

typedef array_ptr<ptr<int>> ppint;
typedef ptr<const int> pcint;

extern void f40(int **x : itype(ppint) bounds(x, x + len), int len) {
}

extern void f41(int **x _Count(len)  itype(ppint), int len) {
}

/// Test some error condtions

extern void f50(int **x : itype(ppint) itype(ppint));   // expected-error {{only one itype expression allowed}}

extern void f51(int **x : count(5) count(6));           // expected-error {{only one bounds expression allowed}}

extern void f53(int ** x  _Itype(ppint) count(5) _Itype(ppint));  // expected-error {{only one itype expression allowed}}

extern void f53(int ** x _Count(5)  itype(ppint) count(5));      // expected-error {{only one bounds expression allowed}}
