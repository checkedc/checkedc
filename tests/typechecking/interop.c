// Feature tests of typechecking of uses of Checked C interoperation
// declarations.
//
// The following lines are for the LLVM test harness:
//
// RUN: %clang_cc1 -Wno-check-bounds-decls -verify -verify-ignore-unexpected=note %s

#include <stdchecked.h>

//
//
// Pass an argument with a checked pointer type to a parameter with
// an unchecked pointer type and an interop bounds declaration.
//
//

void f1(int *p  _Itype(int* _Single)) {
}

void f2(int *p : count(len), int len) {
}

void f3(int *p : byte_count(len * sizeof(int)), int len) {
}

void f4(int *p  _Bounds(p, p + len), int len) {
}

// single-dimensional array parameters
void f1_complete_arr(int a[10] : itype(int checked[10])) {
}

void f2_complete_arr(int p[10] : count(10)) {
}

void f3_complete_arr(int p[10] : byte_count(10 * sizeof(int))) {
}

void f4_complete_arr(int p[10] : bounds(p, p + 10)) {
}

void f1_incomplete_arr(int a[] : itype(int checked[])) {
}

void f2_incomplete_arr(int p[] : count(len), int len) {
}

void f3_incomplete_arr(int p[] : byte_count(len * sizeof(int)), int len) {
}

void f4_incomplete_arr(int p[]  _Bounds(p, p + len), int len) {
}

// multi-dimensional array parameters
void f1_complete_md_arr(int a[10][10] : itype(int checked[10][10])) {
}

void f2_complete_md_arr(int p[10][10] : count(10)) {
}

void f3_complete_md_arr(int p[10][10] : byte_count(10 * sizeof(int [10]))) {
}

void f4_complete_md_arr(int p[10][10] : bounds(p, p + 10)) {
}

void f1_incomplete_md_arr(int a[][10] : itype(int checked[][10])) {
}

void f2_incomplete_md_arr(int p[][10] : count(len), int len) {
}

void f3_incomplete_md_arr(int p[][10] : byte_count(len * sizeof(int[10])), int len) {
}

void f4_incomplete_md_arr(int p[][10]  _Bounds(p, p + len), int len) {
}

// void * parameters with interop declarations.  Note that count bounds
// expressions are not allowed for void * pointers because they don't
// make sense.
void f1_void(void *p : itype(ptr<void>)) {
}

void f2_void(void *p : count(len), int len) { // expected-error {{expected 'p' to have a non-void pointer type}}
}

void f3_void(void *p : byte_count(len * sizeof(int)), int len) {
}

void f4_void(void *p  _Bounds(p, (char *)p + len), int len) {
}

void g1(ptr<int> p) {
  f1(p);
}

// Test typechecking of calls where the called function has a bounds-safe interface.

void g2(array_ptr<int> ap : count(len), int len) {
   f2(ap, len);
   f3(ap, len);
   f4(ap, len);

   if (len >= 10) {
     f1_complete_arr(ap); // expected-error {{it is not possible to prove argument meets declared bounds for 1st parameter}}
     f2_complete_arr(ap); // expected-error {{it is not possible to prove argument meets declared bounds for 1st parameter}}
     f3_complete_arr(ap); // expected-error {{it is not possible to prove argument meets declared bounds for 1st parameter}}
     f4_complete_arr(ap); // expected-error {{it is not possible to prove argument meets declared bounds for 1st parameter}}
   }

   f1_incomplete_arr(ap);
   f2_incomplete_arr(ap, len);
   f3_incomplete_arr(ap, len);
   f4_incomplete_arr(ap, len);

}

void g2_incomplete_array_param(int ap checked[] : count(len), int len) {
  f1_incomplete_arr(ap);
  f2_incomplete_arr(ap, len);
  f3_incomplete_arr(ap, len);
  f4_incomplete_arr(ap, len);

  if (len >= 10) {
    f1_complete_arr(ap); // expected-error {{it is not possible to prove argument meets declared bounds for 1st parameter}}
    f2_complete_arr(ap); // expected-error {{it is not possible to prove argument meets declared bounds for 1st parameter}}
    f3_complete_arr(ap); // expected-error {{it is not possible to prove argument meets declared bounds for 1st parameter}}
    f4_complete_arr(ap); // expected-error {{it is not possible to prove argument meets declared bounds for 1st parameter}}
  }
}

void g2_complete_array_param(int ap checked[10]) {
  f1_complete_arr(ap);
  f2_complete_arr(ap);
  f3_complete_arr(ap);
  f4_complete_arr(ap);

  f1_incomplete_arr(ap);
  f2_incomplete_arr(ap, 10);
  f3_incomplete_arr(ap, 10);
  f4_incomplete_arr(ap, 10);
}

void g2_complete_array_arg(void) {
  int arr checked[10];

  f1_complete_arr(arr);
  f2_complete_arr(arr);
  f3_complete_arr(arr);
  f4_complete_arr(arr);

  f1_incomplete_arr(arr);
  f2_incomplete_arr(arr, 10);
  f3_incomplete_arr(arr, 10);
  f4_incomplete_arr(arr, 10);
}

// Test passing multi-diemensional arrays through bounds-safe
// interfaces.
void g2_md(int (* _Array ap) checked[10] _Count(len), int len) {
  if (len >= 10) {
    f1_complete_md_arr(ap); // expected-error {{it is not possible to prove argument meets declared bounds for 1st parameter}}
    f2_complete_md_arr(ap); // expected-error {{it is not possible to prove argument meets declared bounds for 1st parameter}}
    f3_complete_md_arr(ap); // expected-error {{it is not possible to prove argument meets declared bounds for 1st parameter}}
    f4_complete_md_arr(ap); // expected-error {{it is not possible to prove argument meets declared bounds for 1st parameter}}
  }

  f1_incomplete_md_arr(ap);
  f2_incomplete_md_arr(ap, len);
  f3_incomplete_md_arr(ap, len);
  f4_incomplete_md_arr(ap, len);
}

void g2_incomplete_md_array_param(int ap checked[][10]  _Count(len), int len) {
  f1_incomplete_md_arr(ap);
  f2_incomplete_md_arr(ap, len);
  f3_incomplete_md_arr(ap, len);
  f4_incomplete_md_arr(ap, len);

  if (len >= 10) {
    f1_complete_md_arr(ap); // expected-error {{it is not possible to prove argument meets declared bounds for 1st parameter}}
    f2_complete_md_arr(ap); // expected-error {{it is not possible to prove argument meets declared bounds for 1st parameter}}
    f3_complete_md_arr(ap); // expected-error {{it is not possible to prove argument meets declared bounds for 1st parameter}}
    f4_complete_md_arr(ap); // expected-error {{it is not possible to prove argument meets declared bounds for 1st parameter}}
  }
}

void g2_complete_md_array_param(int ap checked[10][10]) {
  f1_complete_md_arr(ap);
  f2_complete_md_arr(ap);
  f3_complete_md_arr(ap);
  f4_complete_md_arr(ap);

  f1_incomplete_md_arr(ap);
  f2_incomplete_md_arr(ap, 10);
  f3_incomplete_md_arr(ap, 10);
  f4_incomplete_md_arr(ap, 10);
}

void g2_complete_md_array_arg(void) {
  int arr checked[10][10];

  f1_complete_md_arr(arr);
  f2_complete_md_arr(arr);
  f3_complete_md_arr(arr);
  f4_complete_md_arr(arr);

  f1_incomplete_md_arr(arr);
  f2_incomplete_md_arr(arr, 10);
  f3_incomplete_md_arr(arr, 10);
  f4_incomplete_md_arr(arr, 10);
}

// Referent pointer types without any qualifiers must match.
void g3(ptr<float> p) {
  f1(p); // expected-error {{incompatible type}}
}

void g4(float* _Array ap  _Count(len), int len) {
  f2(ap, len);  // expected-error {{incompatible type}}
  f3(ap, len);  // expected-error {{incompatible type}}
  f4(ap, len);  // expected-error {{incompatible type}}
}

// Check passing checked pointers to functions with parameters
// with void * type and bounds-safe interfaces.

void g5(ptr<int> p) {
  f1_void(p);
  f3_void(p, 1);
  f4_void(p, sizeof(int));
}

void g6(array_ptr<int> ap : count(len), int len) {
  if (len >= 1)
    f1_void(ap); // expected-error {{it is not possible to prove argument meets declared bounds for 1st parameter}}
  f3_void(ap, len);
  f4_void(ap, len * sizeof(int));
}

void g7(ptr<void> p) {
  f1(p); // expected-error {{incompatible type}}
  f1_void(p);
  f4_void(p, 1);
}

void g8(array_ptr<void> ap : byte_count(len), int len) {
  f2(ap, len / 4);
  f3(ap, len / 4);
  f4(ap, len / 4);
  f3_void(ap, len);
  f4_void(ap, len);
}

// Check that type qualifiers work as expected for parameters.

void f1_const(const int *p : itype(ptr<const int>)) {
}

void f2_const(const int *p : count(len), int len) {
}

void f3_const(const int *p  _Byte_count(len * sizeof(int)), int len) {
}

void f4_const(const int *p  _Bounds(p, p + len), int len) {
}

// Pointers to non-const qualified data can be passed to parameters that are pointers
// to const-qualified ata.

void g9(ptr<int> p) {
  f1_const(p);
}

void g10(ptr<const int> p) {
  f1_const(p);
}

void g11(int* _Array ap  _Count(len), int len) {
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
// There is a bounds-safe interface implicit conversion when a value with a
// checked pointer type is returned from a function that has an unchecked
// return pointer type with an interop bounds declaration.
//

int *g15(ptr<int> p) : itype(ptr<int>) {
  return p;
}

int *g16(array_ptr<int> p : count(10)) : count(10) {
  return p;
}

int *g17(array_ptr<int> p : count(10)) : byte_count(10 * sizeof(int)) {
  return p;
}

void *g18(array_ptr<int> p : count(10)) : byte_count(10 * sizeof(int)) {
  return p;
}

//
//
// Assignment of a value with a checked pointer types to a global variable with
// an unchecked pointer type and an interop bounds declaration.
//
//

int *v1 : itype(ptr<int>) = 0;
int *v2 : count(10) = 0;
int *v3 : byte_count(10 * sizeof(int)) = 0;
int *v4 : bounds(v4, v4 + 10) = 0;
void *v1_void : itype(ptr<void>);
void *v3_void : byte_count(10 * sizeof(int)) = 0;
void *v4_void : bounds(v4_void, (char *) v4_void + 10 * sizeof(int)) = 0;


void g20(ptr<int> p) {
  v1 = p;
}

void g21(int* _Array ap  _Count(10)) {
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

void g23(float* _Array ap  _Count(10)) {
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

void g27(void* _Array ap  _Byte_count(10 * sizeof(int))) {
  v3 = ap;
  v4 = ap;
  v3_void = ap;
  v4_void = ap;
}

// Check that type qualifiers on pointer referent values work as expected.
const int *const_v1 : itype(ptr<const int>) = 0;
const int *const_v2 : count(10) = 0;
const int *const_v3 : byte_count(10 * sizeof(int)) = 0;
const int *const_v4 : bounds(v4, v4 + 10) = 0;

int *const v1_const : itype(const ptr<int>) = 0;
int *const v2_const : count(10) = 0;
int *const v3_const : byte_count(10 * sizeof(int)) = 0;
int *const v4_const : bounds(v4, v4 + 10) = 0;

// Pointers to non-const data can be assigned to pointers to const-qualified
// data.
void g30(ptr<int> p) {
    const_v1 = p;
}

void g31(int* _Array ap  _Count(10)) {
    const_v2 = ap;
    const_v3 = ap;
    const_v4 = ap; // expected-error {{it is not possible to prove that the inferred bounds of 'const_v4' imply the declared bounds of 'const_v4' after assignment}}
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

void g35(int* _Array ap  _Count(10)) {
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
  int *pint : itype(ptr<int>);
  int *arr1 : count(10);
  int *arr2 : byte_count(10 * sizeof(int));
  int *arr3 : bounds(arr3, arr3 + 10);
};

struct S1_void {
  void *pint : itype(ptr<void>);
  void *arr2 : byte_count(10 * sizeof(int));
  void *arr3 : bounds(arr3, (char *) arr3 + 10 * sizeof(int));
};

void g40(struct S1* _Single p, int* _Single p1, int* _Array p2  _Count(10)) {
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
  p->arr1 = p2;
  p->arr2 = p2;
  p->arr3 = p2;
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
  const int *pint : itype(ptr<const int>);
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
  int *const pint : itype(const ptr<int>);
  int *const arr1 : count(10);
  int *const arr2 : byte_count(10 * sizeof(int));
  int *const arr3 : bounds(arr3, arr3 + 10);
};

void g47(struct S3* _Single p, void* _Single p1, int* _Array p2  _Count(10)) {
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

//
// Test function pointers
//

//
// Function pointers with parameters with bounds-safe interfaces.
//

// Bounds-safe interface type is an array_ptr.
typedef int(*callback_fn1) (int *a : count(n), int n);

void g60(callback_fn1 fn, array_ptr<int> arr : count(k), int k) {
  (*fn)(arr, k);
}

void g61(callback_fn1 fn, array_ptr<float> arr : count(k), int k) {
  (*fn)(arr, k); // expected-error {{incompatible type}}
}

// Bounds-safe interface type is 1-D checked array.
typedef int(*callback_fn2) (int *a : itype(int checked[10]));
void g70(callback_fn2 fn, int arr checked[10]) {
  (*fn)(arr);
}

void g71(callback_fn2 fn, int arr checked[11]) {
  (*fn)(arr);
}

void g72(callback_fn2 fn) {
  int arr checked[10];
  (*fn)(arr);
}


// Bounds-safe interface type is 2-D checked array.
typedef int(*callback_fn3) (int(*a)[10] : itype(int checked[10][10]));
void g80(callback_fn3 fn, int arr checked[10][10]) {
  (*fn)(arr);
}

void g81(callback_fn3 fn, int arr checked[10][11]) {
  (*fn)(arr);  // expected-error {{incompatible type}}
}

void g82(callback_fn3 fn) {
  int arr checked[10][10];
  (*fn)(arr);
}

//
// Test assignments involving non-variable lvalues
// and bounds-safe interfaces
//

// Assign a checked pointer to the dereference of an
// unchecked pointer with a bounds-safe interface.
void g90(int **interop_ptr  _Itype(array_ptr<ptr<int>>) count(3), int* _Single checked_ptr) {
  *interop_ptr = checked_ptr;
  *(interop_ptr - 1) = checked_ptr;
  *(2 + interop_ptr) = checked_ptr;
}

// Assign a checked pointer to an element of an
// unchecked array with a bounds-safe interface.
void g91(int *arr[3] : itype(ptr<int> checked [3]), ptr<int> checked_ptr) {
  arr[0] = checked_ptr;
  2[arr] = checked_ptr;
}
