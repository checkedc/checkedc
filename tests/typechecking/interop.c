// Feature tests of typechecking pf uses of Checked C interoperation
// declarations.
//
// The following lines are for the LLVM test harness:
//
// RUN: %clang_cc1 -verify -verify-ignore-unexpected=note -fcheckedc-extension %s

#include "../../include/stdchecked.h"

//
//
// Pass an argument with a checked pointer type to a parameter with
// an unchecked pointer type and an interop bounds declaration.
//
//

void f1(int *p : type(ptr<int>)) {
}

void f2(int *p : count(len), int len) {
}

void f3(int *p : byte_count(len * sizeof(int)), int len) {
}

void f4(int *p : bounds(p, p + len), int len) {
}

// void * parameters with interop declarations.  Note that count bounds
// expressions are not allowed for void * pointers because they don't
// make sense.
void f1_void(void *p : type(ptr<void>)) {
}

void f3_void(void *p : byte_count(len * sizeof(int)), int len) {
}

void f4_void(void *p : bounds(p, (char *) p + len), int len) {
}


void g1(ptr<int> p) {
  f1(p);
}

void g2(array_ptr<int> ap : count(len), int len) {
   f2(ap, len);
   f3(ap, len);
   f4(ap, len);
}

// Referent pointer types without any qualifiers must match.
void g3(ptr<float> p) {
  f1(p); // expected-error {{incompatible type}}
}

void g4(array_ptr<float> ap : count(len), int len) {
  f2(ap, len);  // expected-error {{incompatible type}}
  f3(ap, len);  // expected-error {{incompatible type}}
  f4(ap, len);  // expected-error {{incompatible type}}
}

// Check passing checked pointers to functions with parameters
// with void * type and bounds-safe interfaces.

void g5(ptr<int> p) {
  f1_void(p);
}

void g6(array_ptr<int> ap : count(len), int len) {
  f3_void(ap, len);
  f4_void(ap, len);
}

void g7(ptr<void> p) {
  f1(p); // expected-error {{incompatible type}}
  f1_void(p);
}

void g8(array_ptr<void> ap : byte_count(len), int len) {
  f2(ap, len / 4);  // expected-error {{incompatible type}}
  f3(ap, len / 4);  // expected-error {{incompatible type}}
  f4(ap, len / 4);  // expected-error {{incompatible type}}
  f3_void(ap, len);
  f4_void(ap, len);
}


// Check that type qualifiers work as expected for parameters.

void f1_const(const int *p : type(ptr<const int>)) {
}

void f2_const(const int *p : count(len), int len) {
}

void f3_const(const int *p : byte_count(len * sizeof(int)), int len) {
}

void f4_const(const int *p : bounds(p, p + len), int len) {
}

// Pointers to non-const qualified data can be passed to parameters that are pointers
// to const-qualified ata.

void g9(ptr<int> p) {
  f1_const(p);
}

void g10(ptr<const int> p) {
  f1_const(p);
}

void g11(array_ptr<int> ap : count(len), int len) {
  f2_const(ap, len);
  f3_const(ap, len);
  f4_const(ap, len);
}

void g12(array_ptr<const int> ap : count(len), int len) {
  f2_const(ap, len);
  f3_const(ap, len);
  f4_const(ap, len);
}

// Pointers to const-qualified data should not be passed to
// parameters that are pointers to non-const-qualified data.

void g13(ptr<const int> p) {
  f1(p);        // expected-warning {{discards qualifiers}}
}

void g14(array_ptr<const int> ap : count(len), int len) {
  f2(ap, len);  // expected-warning {{discards qualifiers}}
  f3(ap, len);  // expected-warning {{discards qualifiers}}
  f4(ap, len);  // expected-warning {{discards qualifiers}}
}

//
//
// Assignment of a value with a checked pointer types to a global variable with
// an unchecked pointer type and an interop bounds declaration.
//
//

int *v1 : type(ptr<int>) = 0;
int *v2 : count(10) = 0;
int *v3 : byte_count(10 * sizeof(int)) = 0;
int *v4 : bounds(v4, v4 + 10) = 0;
void *v1_void : type(ptr<void>);
void *v3_void : byte_count(10 * sizeof(int)) = 0;
void *v4_void : bounds(v4_void, (char *) v4_void + 10 * sizeof(int)) = 0;


void g20(ptr<int> p) {
  v1 = p;
}

void g21(array_ptr<int> ap : count(10)) {
  v2 = ap;
  v3 = ap;
  v4 = ap;
}

// An initializing assignment of a value with a checked pointer type to a global
// variable with an unchecked pointer type and an interop bounds declaration.
int arr1 checked[10];
int *v5 : bounds(v5, v5 + 10) = arr1;
void *v6 : bounds(v6, (char *) v6 + 10 * sizeof(int)) = arr1;

// Pointer referent types must match
void g22(ptr<float> p) {
  v1 = p;  // expected-error {{incompatible type}}
}

void g23(array_ptr<float> ap : count(10)) {
  v2 = ap; // expected-error {{incompatible type}}
  v3 = ap; // expected-error {{incompatible type}}
  v4 = ap; // expected-error {{incompatible type}}
}

// Test assignments to void * pointers with bounds declarations.
// Also test assignments of checked void pointers to non-void *
// pointers with bounds declarations.

void g24(ptr<int> p) {
  v1_void = p;
}

void g25(array_ptr<int> ap : count(10)) {
  v3_void = ap;
  v4_void = ap;
}

void g26(ptr<void> p) {
  v1 = p;  // expected-error {{incompatible type}}
  v1_void = p;
}

void g27(array_ptr<void> ap : byte_count(10 * sizeof(int))) {
  v3 = ap;  // expected-error {{incompatible type}}
  v4 = ap;  // expected-error {{incompatible type}}
  v3_void = ap;
  v4_void = ap;
}

// Check that type qualifiers on pointer referent values work as expected.
const int *const_v1 : type(ptr<int>) = 0;
const int *const_v2 : count(10) = 0;
const int *const_v3 : byte_count(10 * sizeof(int)) = 0;
const int *const_v4 : bounds(v4, v4 + 10) = 0;

int *const v1_const : type(ptr<int>) = 0;
int *const v2_const : count(10) = 0;
int *const v3_const : byte_count(10 * sizeof(int)) = 0;
int *const v4_const : bounds(v4, v4 + 10) = 0;

// Pointers to non-const data can be assigned to pointers to const-qualified
// data.
void g30(ptr<int> p) {
    const_v1 = p;
}

void g31(array_ptr<int> ap : count(10)) {
  const_v2 = ap;
  const_v3 = ap;
  const_v4 = ap;
}

// Pointers to const-data should not be assigned to pointers to non-const qualified
// data
void g32(ptr<const int> p) {
  v1 = p;  // expected-warning {{discards qualifiers}}
}

void g33(array_ptr<const int> ap : count(10)) {
  v2 = ap;  // expected-warning {{discards qualifiers}}
  v3 = ap;  // expected-warning {{discards qualifiers}}
  v4 = ap;  // expected-warning {{discards qualifiers}}
}

// Assignments to const pointer variables are not allowed.
void g34(ptr<int> p) {
  v1_const = p; // expected-error {{cannot assign to variable}}
}

void g35(array_ptr<int> ap : count(10)) {
  v2_const = ap;  // expected-error {{cannot assign to variable 'v2_const' with const-qualified}}
  v3_const = ap;  // expected-error {{cannot assign to variable 'v3_const' with const-qualified}}
  v4_const = ap;  // expected-error {{cannot assign to variable 'v4_const' with const-qualified}}
}

//
//
// Assign a value with a checked pointer type to a structure member with
// an unchecked pointer type and an interop bounds declaration.
//
//

struct S1 {
  int *pint : type(ptr<int>);
  int *arr1 : count(10);
  int *arr2 : byte_count(10 * sizeof(int));
  int *arr3 : bounds(arr3, arr3 + 10);
};

struct S1_void {
  void *pint : type(ptr<void>);
  void *arr2 : byte_count(10 * sizeof(int));
  void *arr3 : bounds(arr3, (char *) arr3 + 10 * sizeof(int));
};

void g40(ptr<struct S1> p, ptr<int> p1, array_ptr<int> p2 : count(10)) {
  p->pint = p1;
  p->arr1 = p2;
  p->arr2 = p2;
  p->arr3 = p2;
}

// Pointers to referent types must be compatible.
void g41(ptr<struct S1> p, ptr<float> p1, array_ptr<float> p2 : count(10)) {
  p->pint = p1;  // expected-error {{incompatible type}}
  p->arr1 = p2;  // expected-error {{incompatible type}}
  p->arr2 = p2;  // expected-error {{incompatible type}}
  p->arr3 = p2;  // expected-error {{incompatible type}}
}

// Test assignments to void * members with bounds declarations and assignments
// of checked void pointers to void to non-void * members with bounds declarations.
void g42(ptr<struct S1> p, ptr<void> p1, array_ptr<void> p2 : byte_count(10 * sizeof(int))) {
  p->pint = p1;  // expected-error {{incompatible type}}
  p->arr1 = p2;  // expected-error {{incompatible type}}
  p->arr2 = p2;  // expected-error {{incompatible type}}
  p->arr3 = p2;  // expected-error {{incompatible type}}
}

// Check assignments to void * members with bounds declarations
void g43(ptr<struct S1_void> p, ptr<int> p1, array_ptr<int> p2 : count(10)) {
  p->pint = p1;
  p->arr2 = p2;
  p->arr3 = p2;
}

void g44(ptr<struct S1_void> p, ptr<void> p1, array_ptr<void> p2 : byte_count(10 * sizeof(int))) {
  p->pint = p1;
  p->arr2 = p2;
  p->arr3 = p2;
}

// Check that type qualifiers work as expected.

struct S2 {
  const int *pint : type(ptr<int>);
  const int *arr1 : count(10);
  const int *arr2 : byte_count(10 * sizeof(int));
  const int *arr3 : bounds(arr3, arr3 + 10);
};

// Pointers to non-const data can be assigned to pointers to const-data.
void g45(ptr<struct S2> p, ptr<int> p1, array_ptr<int> p2 : count(10)) {
  p->pint = p1;
  p->arr1 = p2;
  p->arr2 = p2;
  p->arr3 = p2;
}

// Pointers to const data should not be assigned to pointers to non-const-data.
void g46(ptr<struct S1> p, ptr<const int> p1, array_ptr<const int> p2 : count(10)) {
  p->pint = p1;  // expected-warning {{discards qualifiers}}
  p->arr1 = p2;  // expected-warning {{discards qualifiers}}
  p->arr2 = p2;  // expected-warning {{discards qualifiers}}
  p->arr3 = p2;  // expected-warning {{discards qualifiers}}
}

struct S3 {
  int *const pint : type(ptr<int>);
  int *const arr1 : count(10);
  int *const arr2 : byte_count(10 * sizeof(int));
  int *const arr3 : bounds(arr3, arr3 + 10);
};

void g47(ptr<struct S3> p, ptr<int> p1, array_ptr<int> p2 : count(10)) {
  p->pint = p1; // expected-error {{cannot assign to non-static data member 'pint' with const-qualified type }}
  p->arr1 = p2; // expected-error {{cannot assign to non-static data member 'arr1' with const-qualified type}}
  p->arr2 = p2; // expected-error {{cannot assign to non-static data member 'arr2' with const-qualified type}}
  p->arr3 = p2; // expected-error {{cannot assign to non-static data member 'arr3' with const-qualified type}}
}

//
// Test union members.
//

union U1 {
  const int *arr1 : count(10);
  const int *arr2 : byte_count(10 * sizeof(int));
  const int *arr3 : bounds(arr3, arr3 + 10);
};

void g48(ptr<union U1> p, array_ptr<int> p1 : count(10)) {
  p->arr1 = p1;
  p->arr2 = p1;
  p->arr3 = p1;
 }

void g49(ptr<union U1> p, array_ptr<float> p1 : count(10)) {
  p->arr1 = p1;  // expected-error {{incompatible type}}
  p->arr2 = p1;  // expected-error {{incompatible type}}
  p->arr3 = p1;  // expected-error {{incompatible type}}
}
