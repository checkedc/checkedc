// Feature tests for type checking Checked C bounds-safe interface
// type annotations.
//
// The following lines are for the LLVM test harness:
//
// RUN: %clang_cc1 -verify -verify-ignore-unexpected=note %s

#include <stdchecked.h>

struct S {
  int a;
};

union U {
  int a;
};

typedef int t1;
typedef float t2;

//-------------------------------------------------------------
//
// Tests for parameters with interface type annotations.
//
//-------------------------------------------------------------

//
// Test types not allowed to appear in bounds-safe interface type annotations.
//

void f1(int *p : itype(int)) {      // expected-error {{must be a pointer or array type}}
}

void f2(int *p : itype(_Bool)) {     // expected-error {{must be a pointer or array type}}
}

void f3(int *p : itype(char)) {      // expected-error {{must be a pointer or array type}}
}

void f4(int *p : itype(short int)) { // expected-error {{must be a pointer or array type}}
}

void f5(int *p : itype(int)) {       // expected-error {{must be a pointer or array type}}
}

void f6(int *p : itype(long int)) {  // expected-error {{must be a pointer or array type}}
}

void f7(int *p : itype(float)) {     // expected-error {{must be a pointer or array type}}
}

void f8(int *p : itype(double)) {    // expected-error {{must be a pointer or array type}}
}

void f9(int *p : itype(void)) {      // expected-error {{must be a pointer or array type}}
}


void f10(int *p : itype(struct S)) { // expected-error {{must be a pointer or array type}}
}

void f11(int *p : itype(union U)) {  // expected-error {{must be a pointer or array type}}
}

void f12(int *p : itype(int(int))) { // expected-error {{mismatch between interface type}} expected-error {{type must be a checked type}}
}

void f13(int *p : itype(t1)) {      // expected-error {{must be a pointer or array type}}
}

void f14(int *p : itype(t2)) {      // expected-error {{must be a pointer or array type}}
}

//
// Test types not allowed to have bound-safe interface type annotations.
//

void f1a(int p : itype(ptr<int>)) {   // expected-error {{interface type only allowed for a declaration with pointer or array type}}
}

void f2a(_Bool p : itype(ptr<int>)) {   // expected-error {{interface type only allowed for a declaration with pointer or array type}}
}

void f3a(char p : itype(ptr<int>)) {   // expected-error {{interface type only allowed for a declaration with pointer or array type}}
}

void f4a(short int p : itype(ptr<int>)) {   // expected-error {{interface type only allowed for a declaration with pointer or array type}}
}

void f6a(long int p : itype(ptr<int>)) {   // expected-error {{interface type only allowed for a declaration with pointer or array type}}
}

void f7a(float p : itype(ptr<int>)) {   // expected-error {{interface type only allowed for a declaration with pointer or array type}}
}

void f8a(double p : itype(ptr<int>)) {   // expected-error {{interface type only allowed for a declaration with pointer or array type}}
}

void f10a(struct S p : itype(ptr<int>)) {   // expected-error {{interface type only allowed for a declaration with pointer or array type}}
}

void f11a(union U p : itype(ptr<int>)) {   // expected-error {{interface type only allowed for a declaration with pointer or array type}}
}

void f13a(t1 p : itype(ptr<int>)) {   // expected-error {{interface type only allowed for a declaration with pointer or array type}}
}

void f14a(t2 p : itype(ptr<int>)) {   // expected-error {{interface type only allowed for a declaration with pointer or array type}}
}

// interface type must be a checked type.
void f30(int *p : itype(int *)) {  // expected-error {{must be a checked type}}
}

void f31(int p[] : itype(int[])) {  // expected-error {{must be a checked type}}
}

//
// Valid type annotations
// 

// Single pointer or 1-d array

void f32(int *p : itype(ptr<int>)) {
}

void f33(int *p : itype(array_ptr<int>)) {
}

void f34(int p[] : itype(int checked[])) {
}

void f35(int p[] : itype(ptr<int>)) {
}

void f36(int p[] : itype(array_ptr<int>)) {
}

void f37(int *p : itype(int checked[])) {
}

void f38(int *p : itype(int checked[15])) {
}

// Two levels of pointers

void f50(int **p : itype(ptr<ptr<int>>)) {
}

void f51(int **p : itype(ptr<array_ptr<int>>)) {
}

void f52(int **p : itype(array_ptr<ptr<int>>)) {
}

void f53(int **p : itype(array_ptr<array_ptr<int>>)) {
}

void f54(int **p : itype(ptr<int *>)) {
}

void f55(int **p : itype(array_ptr<int *>)) {
}

void f56(int **p : itype(ptr<int> checked[10])) {
}

void f57(int **p : itype(ptr<int> checked[])) {
}

void f58(int **p : itype(array_ptr<int> checked[10])) {
}

void f59(int **p : itype(int *checked[20])) {
}

void f60(ptr<int> *p : itype(ptr<ptr<int>>)) {
}


// Multi-dimensional arrays

void f70(int a[][10] : itype(int checked[][10])) {
}

void f71(int a[10][10] : itype(int checked[10][10])) {
}

// First dimension size is discarded by C type checking rules. 
// The type of ``array of T'' is turned into ``'pointer to T''
// We might want to warn about the situation where the 1st dimension
// sizes disagree between the declared type and the annotated type.
void f72(int a[10][10] : itype(int checked[11][10])) {
}

void f73(int a[10][10] : itype(array_ptr<int checked[10]>)) {
}

void f74(int a[10][10] : itype(array_ptr<int[10]>)) {
}

void f75(int(*a)checked[10][10]: itype(ptr<int checked[10][10]>)) {
}


// Function pointers


void f80(int ((*f)(int *, int *)) : itype(ptr<int (int *, int *)>)) {
}

void f81(int((*f)(int *, int *)) : itype(ptr<int(ptr<int>, ptr<int>)>)) {
}

void f82(int((*f)(int *, int *)) : itype(ptr<int (int checked[10], int checked[10])>)) {
}


// Combinations of interface types and bounds expressions

void f90(int **p : count(10) itype(array_ptr<ptr<int>>)) {
}

void f91(int **p : bounds(p, p + 10) itype(array_ptr<ptr<int>>)) {
}

void f92(int **p : itype(array_ptr<ptr<int>>) byte_count(4 * sizeof(int *))) {
}

void f93(int **p : itype(ptr<int> checked[]) count(10)) {
}

void f94(int **p : itype(ptr<int> checked[10]) count(10)) {
}


// Spot check uses of type qualifiers.  They must be identical for the declared type
// and the bound-safe interface type.

void f200(const int *p : itype(ptr<const int>)) {
}

void f201(volatile int *p : itype(ptr<volatile int>)) {
}

void f202(const volatile int *p : itype(ptr<const volatile int>)) {
}

void f203(const int *p : itype(array_ptr<const int>)) {
}

void f204(volatile int *p : itype(array_ptr<volatile int>)) {
}

void f205(const volatile int *p : itype(array_ptr<const volatile int>)) {
}

void f206(int * const p : itype(const ptr<int>)) {
}

void f207(int * volatile p : itype(volatile ptr<int>)) {
}

void f208(int * restrict p  : itype(restrict ptr<int>)) {
}

void f209(int * const p : itype(const array_ptr<int>)) {
}

void f210(int * const p : itype(int checked[const 10])) {
}

void f211(int * restrict p : itype(int checked[restrict 10])) {
}

void f212(int * volatile p : itype(int checked[volatile 10])) {
}

void f213(int a[const 10] : itype(int checked[const 10])) {
}

void f214(int a[volatile 10] : itype(int checked[volatile 10])) {
}

void f216(int a[const 10] : itype(const array_ptr<int>) count(10)) {
}

void f217(int a[volatile 10] : count(10) itype(volatile array_ptr<int>)) {
}

void f218(const int a[10] : itype(const int checked[10]) count(10)) {
}

void f219(const int a[10] : count(10) itype(array_ptr<const int>)) {
}

// The C standard allows the first dimension of an array parameter to have the
// static keyword specified for the size of the array.  The C standard requires
// that any array passed to the parameter must have at least that size.  It
// leaves the behavior undefined if the argument does not have that size.  It
// does not specify any checking requirements.  
//
// clang ony issues warning for arguments that are local variables that
// have an array type with the wrong sizes.  It does not issue warnings for
// arguments that had pointer type (or were a parameter with a pointer type
//
// For the purposes of typechecking, this means that the static keyword is
// discarded.  Any size requirements from static will be enforced during the
// checking of bounds declarations.

void f230(int a[static 10][10] : itype(int checked[10][10])) {
}

void f231(int a[static 10][10] : itype(int checked[11][10])) {
}

void f232(int a[static 10][10] : itype(int checked[static 10][10])) {
}

void f233(int a[static 10][10] : itype(int checked[static 11][10])) {
}

void f234(int a[10][10] : itype(int checked[static 10][10])) {
}

void f235(int a[static 10][10] : itype(int checked[static 11][10])) {
}

void f236(int a[static 10][10] : count(10) itype(array_ptr<int checked[10]>)) {
}

// TODO: this will be an error once bounds declarations are checked.
void f237(int a[static 10][10] : itype(int checked[9][10])) {
}

// TODO: this will be an error once bounds declarations are checked.
void f238(int a[static 10][10] : itype(ptr<int checked[10]>)) {
}

//
// Incorrect type annotations.
// 

// Incompatible pointee or element types.

// Pointer types
void f250(float **p : itype(ptr<int *>)) {   // expected-error {{mismatch between interface type '_Ptr<int *>' and declared type 'float **'}}
}

void f251(float **p : itype(ptr<ptr<int>>)) {   // expected-error {{mismatch between interface type '_Ptr<_Ptr<int>>' and declared type 'float **'}}
}

void f252(float a[10] : itype(double checked[10])) { // expected-error {{mismatch between interface type}}
}

// Array types
void f253(int a[10][10] : itype(int checked[10][11])) { // expected-error {{mismatch between interface type '_Array_ptr<int _Checked[11]>' and declared type 'int (*)[10]'}}
}

void f254(int(*a)[10] : itype(ptr<int checked[]>)) {  // expected-error {{mismatch between interface type}}
}

void f255(int(*a)[] : itype(ptr<int checked[10]>)) {  // expected-error {{mismatch between interface type}}
}

// Differing number of parameters for function pointer.
// Note that the function declarator has to be parenthesized so that
// the interface type declaration is not parsed as the interface type for
// the return type of the function declarator.
void f256(int((*f)(int, float, char)) : itype(ptr<int(int, float)>)) { // expected-error {{mismatch between interface type}}
}

// Differing parameter types for function pointer.
// See the earlier comment for f256 about why the function declarator is
// parenthesized.
void f257(int((*f)(int, float, char)) : itype(ptr<int(int, float, double)>)) { // expected-error {{mismatch between interface type}}
}

// Differing return types for function pointer
// See the earlier comment for f256 about why the function declarator is
// parenthesized.
void f258(int((*f)(int, float, char)) : itype(ptr<float(int, float,char)>)) { // expected-error {{mismatch between interface type}}
}

// No special treatement for void pointers
void f259(void *p : itype(ptr<int>)) { // expected-error {{mismatch between interface type}}
}

void f260(int *p : itype(ptr<void>)) { // expected-error {{mismatch between interface type}}
}

// Annotation type loses checking.

// Lost checking on a pointer to a pointer.
void f280(ptr<int> *p : itype(ptr<int *>)) { //expected-error {{type '_Ptr<int *>' loses checking of declared type '_Ptr<int> *'}}
}

// Lost checking on a pointer to an array.
void f281(int(*a)checked[10][10] : itype(ptr<int[10][10]>)) { // expected-error {{loses checking of declared type}}
}

// Test lost checking on an argument type for a function pointer type
void f282(int ((*f)(int checked[10])) : itype(ptr<int (int[10])>)) { // expected-error {{loses checking of declared type}}
}

// Test lost checking on function return type for a function pointer
void f283(ptr<int> ((*f)(int[10])) : itype(ptr<int *(int[10])>)) { // expected-error {{loses checking of declared type}}
}

// Declaring bounds for a _Ptr interface type

void f284(int **p : itype(ptr<ptr<int>>) count(10)) { // expected-error {{cannot declare bounds with a _Ptr interface type}}
}

void f285(int **p : count(10) itype(ptr<ptr<int>>)) { // expected-error {{cannot declare bounds with a _Ptr interface type}}
}

void f286(int **p : itype(ptr<ptr<int>>) bounds(p, p + 10)) {  // expected-error {{cannot declare bounds with a _Ptr interface type}}
}

void f287(int **p :  bounds(p, p + 10) itype(ptr<ptr<int>>)) {  // expected-error {{cannot declare bounds with a _Ptr interface type}}
}

void f288(int **p : itype(ptr<ptr<int>>) byte_count(10 * sizeof(int *))) {  // expected-error {{cannot declare bounds with a _Ptr interface type}}
}

void f289(int **p : byte_count(10 * sizeof(int *)) itype(ptr<ptr<int>>)) {  // expected-error {{cannot declare bounds with a _Ptr interface type}}
}

//------------------------------------------------------------ -
//
// Tests for return types with interface type annotations.
//
//-------------------------------------------------------------

//
// Types that cannot appear in bounds-safe interface type annotations.
//

int *r1(void) : itype(int) {      // expected-error {{must be a pointer type}}
  return 0;
}

int *r2(void) : itype(_Bool) {     // expected-error {{must be a pointer type}}
  return 0;
}

int *r3(void) : itype(char) {      // expected-error {{must be a pointer type}}
  return 0;
}

int *r4(void) : itype(short int) { // expected-error {{must be a pointer type}}
  return 0;
}

int *r5(void) : itype(int) {       // expected-error {{must be a pointer type}}
  return 0;
}

int *r6(void) : itype(long int) {  // expected-error {{must be a pointer type}}
  return 0;
}

int *r7(void) : itype(float) {     // expected-error {{must be a pointer type}}
  return 0;
}

int *r8(void) : itype(double) {    // expected-error {{must be a pointer type}}
  return 0;
}

int *r9(void) : itype(void) {      // expected-error {{must be a pointer type}}
  return 0;
}

int *r10(void) : itype(struct S) { // expected-error {{must be a pointer type}}
  return 0;
}

int *r11(void) : itype(union U) {  // expected-error {{must be a pointer type}}
  return 0;
}

int *r12(void) : itype(int (int)) { // expected-error {{must be a pointer type}}
  return 0;
}

int *r13(void) : itype(t1) {      // expected-error {{must be a pointer type}}
  return 0;
}

int *r14(void) : itype(t2) {      // expected-error {{must be a pointer type}}
  return 0;
}


int *r30(void) : itype(int *) {   // expected-error {{must be a checked type}}
  return 0;
}

int *r31a(void) : itype(int[]) {   // expected-error {{array type not allowed}}
  return 0;
}

int *r31b(void) : itype(int checked[]) {   // expected-error {{array type not allowed}}
  return 0;
}

int *r31c(void) : itype(int[10]) {   // expected-error {{array type not allowed}}
  return 0;
}

int *r31d(void) : itype(int checked[10]) {   // expected-error {{array type not allowed}}
  return 0;
}

// Multi-dimensional arrays

int ((*(r31e(int arg[10][10]) : itype(int[10][10])))[10]) { // expected-error {{array type not allowed}}
  return arg;
}

int(*(r31f(int arg[10][10]) : itype(int checked[10][10])))[10] { // expected-error {{array type not allowed}}
  return arg;
}

// Return types that cannot have interface types

int r1a(void) : itype(ptr<int>) {    // expected-error {{interface type only allowed for a pointer return type}}
  return 0;
}

_Bool r2a(void) : itype(ptr<int>) {  // expected-error {{interface type only allowed for a pointer return type}}
  return 0;
}

char r3a(void) : itype(ptr<int>) {   // expected-error {{interface type only allowed for a pointer return type}}
  return 0;
}

short int r4a(void) : itype(ptr<int>) { // expected-error {{interface type only allowed for a pointer return type}}
  return 0;
}

long int r6a(void) : itype(ptr<int>) {   // expected-error {{interface type only allowed for a pointer return type}}
  return 0;
}

float r7a(void) : itype(ptr<int>) {      // expected-error {{interface type only allowed for a pointer return type}}
  return 0;
}

double r8a(void) : itype(ptr<int>) {    // expected-error {{interface type only allowed for a pointer return type}}
  return 0;
}

void r9a(void) : itype(ptr<int>) {      // expected-error {{interface type only allowed for a pointer return type}}
}

struct S r10a(void) : itype(ptr<int>) { // expected-error {{interface type only allowed for a pointer return type}}
  struct S v;
  v.a = 0;
  return v;
}

union U r11a(void) : itype(ptr<int>) {  // expected-error {{interface type only allowed for a pointer return type}}
  union U v;
  v.a = 0;
  return v;
}

t1 r13a(void) : itype(ptr<int>) {    // expected-error {{interface type only allowed for a pointer return type}}
  return 0;
}

t2 r14a(void) : itype(ptr<int>) {    // expected-error {{interface type only allowed for a pointer return type}}
  return 0;
}
//
// Valid type annotations
// 

// Single pointer
int *r32(void) : itype(ptr<int>) {
  return 0;
}

int *r33(void) : itype(array_ptr<int>) {
  return 0;
}

// Two levels of pointers

int **r50(void) : itype(ptr<ptr<int>>) {
  return 0;
}

int **r51(void) : itype(ptr<array_ptr<int>>) {
  return 0;
}

int **r52(void) : itype(array_ptr<ptr<int>>) {
  return 0;
}

int **r53(void) : itype(array_ptr<array_ptr<int>>) {
  return 0;
}

int **r54(void) : itype(ptr<int *>) {
  return 0;
}

int **r55(void) : itype(array_ptr<int *>) {
  return 0;
}

ptr<int> *r59(void) : itype(ptr<ptr<int>>) {
  return 0;
}

// Returning a function pointer with a bounds-safe interface.




// Combinations of interface types and bounds expressions

int **r70(void) : itype(array_ptr<ptr<int>>) count(10) {
  return 0;
}

int **r71(void) : count(10) itype(array_ptr<ptr<int>>) {
  return 0;
}

int **r72(array_ptr<int> p : count(10)) : itype(array_ptr<ptr<int>>) bounds(p, p + 10) {
  return 0;
}

int **r73(array_ptr<int> p : count(10)) : bounds(p, p + 10) itype(array_ptr<ptr<int>>) {
  return 0;
}

int **r74(void) : itype(array_ptr<ptr<int>>) byte_count(10 * sizeof(int *)) {
  return 0;
}

int **r75(void) : byte_count(10 * sizeof(int *)) itype(array_ptr<ptr<int>>)  {
  return 0;
}


// Function pointers

// r80 is a function that takes no arguments and returns a function
// pointer that takes two integer pointers and returns an integer.

int (*r80(void) : itype(ptr<int(int *, int *)>))(int *,int *) {
  return 0;
}

int (*r81(void) : itype(ptr<int(ptr<int>, ptr<int>)>))(int *, int *) {
  return 0;
}

int (*r82(void) : itype(ptr<int(int checked[10], int checked[10])>))(int *, int *) {
  return 0;
}

// r83 is a function that takes an integer and returns a function pointer that
// takes two integer pointer parameters and return a pointer to a float.

float *(*r83(int) : itype(ptr<float *(int *, int *)>) // bounds-safe interface for return
     )(int *,int *);

float *(*r84(int) : itype(ptr<ptr<float> (int *, int *)>) // bounds-safe interface for return
     )(int *,int *);

float *(*r85(int) : itype(ptr<float *(ptr<int>, ptr<int>)>) // bounds-safe interface for return
     )(int *,int *);

float *(*r86(int) : itype(ptr<ptr<float> (ptr<int>, ptr<int>)>) // bounds-safe interface for return
     )(int *,int *);

// r90 is a functon that takes an integer and a returns a pointer to an array of 10 function pointers.
int (**r90(void) : itype(array_ptr<ptr<int (ptr<int>, ptr<int>)>>) count(10))(int *, int *) {
  return 0;
}

// Spot check uses of type qualifiers.  They must be identical for the declared type
// and the bounds-safe interface type.

const int *r200(void) : itype(ptr<const int>) {
  return 0;
}

volatile int *r201(void) : itype(ptr<volatile int>) {
  return 0;
}

const volatile int *r202(void) : itype(ptr<const volatile int>) {
  return 0;
}

const int *r203(void) : itype(array_ptr<const int>) {
  return 0;
}

volatile int *r204(void) : itype(array_ptr<volatile int>) {
  return 0;
}

const volatile int *r205(void) : itype(array_ptr<const volatile int>) {
  return 0;
}

int * const r206(void) : itype(const ptr<int>) {
  return 0;
}

int * volatile r207(void) : itype(volatile ptr<int>) {
  return 0;
}

int * restrict r208(void) : itype(restrict ptr<int>) {
  return 0;
}

int * const r209(void) : itype(const array_ptr<int>) {
  return 0;
}

//
// Incorrect type annotations.
// 

// Incompatible pointee or element types.

// Pointer types
float **r250(void) : itype(ptr<int *>) {   // expected-error {{mismatch between interface type '_Ptr<int *>' and declared type 'float **'}}
}

float **r251(void) : itype(ptr<ptr<int>>) {   // expected-error {{mismatch between interface type '_Ptr<_Ptr<int>>' and declared type 'float **'}}
}

// Array types

// Returns pointer to array of 10 integers.
int (*r254(void) : itype(ptr<int checked[]>))[10] {  // expected-error {{mismatch between interface type}}
}

// Returns pointer to array of integers with unknown
// size.
int (*r255(void) : itype(ptr<int checked[10]>))[]{  // expected-error {{mismatch between interface type}}
}

// Differing number of parameters for function pointer.
// Note that the function declarator has to be parenthesized so that
// the interface type declaration is not parsed as the interface type for
// the return type of the function declarator.
int (*r256(void) : itype(ptr<int(int, float)>))(int, float, char) { // expected-error {{mismatch between interface type}}
}

// Differing parameter types for function pointer.
// See the earlier comment for r256 about why the function declarator is
// parenthesized.
int (*r257(void) : itype(ptr<int(int, float, double)>))(int, float, char) { // expected-error {{mismatch between interface type}}
}

// Differing return types for function pointer
// See the earlier comment for r256 about why the function declarator is
// parenthesized.
int (*r258(void) : itype(ptr<float (int, float, char)>))(int, float, char) { // expected-error {{mismatch between interface type}}
}

// No special treatement for void pointers
void *r259(void) : itype(ptr<int>) { // expected-error {{mismatch between interface type}}
}

int *r260(void) : itype(ptr<void>) { // expected-error {{mismatch between interface type}}
}

// Annotation type loses checking.

ptr<int> *r280(void) : itype(ptr<int *>) { //expected-error {{type '_Ptr<int *>' loses checking of declared type '_Ptr<int> *'}}
}

// Declared return type is an unchecked pointer to a checked 2-d array, but
// the interop type is a checked pointer to an unchecked 2-d array.
int (*r281(void) : itype(ptr<int[10][10]>))checked[10][10] { // expected-error {{loses checking of declared type}}
}

// Declared return type is an unchecked pointer to a checked 1-d array, but
// the interop type is a checked pointer to an unchecked 1-d array.

int (*r282(void) : itype(ptr<int[10]>)) checked[10] { // expected-error {{loses checking of declared type}}
}

// Declaring bounds for a _Ptr interface type

int *r290(void) : itype(ptr<int>) count(5);        // expected-error {{cannot declare bounds with a _Ptr interface type}}

int **r291(void) : count(5) itype(ptr<ptr<int>>);  // expected-error {{cannot declare bounds with a _Ptr interface type}}

int **r292(int **p : count(5)) : itype(ptr<ptr<int>>) bounds(p, p + 5) {  // expected-error {{cannot declare bounds with a _Ptr interface type}}
  return 0;
}
int **r293(int **p : count(5))  :  bounds(p, p + 5) itype(ptr<ptr<int>>)  { // expected-error {{cannot declare bounds with a _Ptr interface type}}
  return 0;
}

int **r294(int **p : count(5)) : itype(ptr<ptr<int>>) byte_count(5 * sizeof(ptr<int>)) { // expected-error {{cannot declare bounds with a _Ptr interface type}}
  return 0;
}

int **r294(int **p : count(5)) : byte_count(5 * sizeof(ptr<int>)) itype(ptr<ptr<int>>) { // expected-error {{cannot declare bounds with a _Ptr interface type}}
  return 0;
}

//-------------------------------------------------------------
//
// Tests for global variables with interface type annotations.
//
//-------------------------------------------------------------

//
// Types that cannot appear in bounds-safe interface type annotations.
//

int *g1 : itype(int);       // expected-error {{must be a pointer or array type}}
int *g2 : itype(_Bool);     // expected-error {{must be a pointer or array type}}
int *g3 : itype(char);      // expected-error {{must be a pointer or array type}}
int *g4 : itype(short int); // expected-error {{must be a pointer or array type}}
int *g5 : itype(int);       // expected-error {{must be a pointer or array type}}
int *g6 : itype(long int);  // expected-error {{must be a pointer or array type}}
int *g7 : itype(float);     // expected-error {{must be a pointer or array type}}
int *g8 : itype(double);    // expected-error {{must be a pointer or array type}}
int *g9 : itype(void);      // expected-error {{must be a pointer or array type}}
int *g10 : itype(struct S); // expected-error {{must be a pointer or array type}}
int *g11 : itype(union U);  // expected-error {{must be a pointer or array type}}
int *g12 : itype(int(int)); // expected-error {{must be a pointer or array type}}
int *g13 : itype(t1);       // expected-error {{must be a pointer or array type}}
int *g14 : itype(t2);       // expected-error {{must be a pointer or array type}}
int *g30 : itype(int *);    // expected-error {{must be a checked type}}
extern int g31[] : itype(int[]);   // expected-error {{must be a checked type}}

//
// Types that cannot have bounds-safe interface type annotations.
//

int g1a : itype(ptr<int>);  // expected-error {{interface type only allowed for a declaration with pointer or array type}}
_Bool g2a : itype(ptr<int>);  // expected-error {{interface type only allowed for a declaration with pointer or array type}}
char g3a : itype(ptr<int>);  // expected-error {{interface type only allowed for a declaration with pointer or array type}}
short int g4a : itype(ptr<int>);  // expected-error {{interface type only allowed for a declaration with pointer or array type}}
long int  g6a : itype(ptr<int>);  // expected-error {{interface type only allowed for a declaration with pointer or array type}}
float g7a : itype(ptr<int>);  // expected-error {{interface type only allowed for a declaration with pointer or array type}}
double g8a : itype(ptr<int>);  // expected-error {{interface type only allowed for a declaration with pointer or array type}}
struct S g10a : itype(ptr<int>);  // expected-error {{interface type only allowed for a declaration with pointer or array type}}
union U g11a : itype(ptr<int>);  // expected-error {{interface type only allowed for a declaration with pointer or array type}}
t1 g13a : itype(ptr<int>);  // expected-error {{interface type only allowed for a declaration with pointer or array type}}
t2 g14a : itype(ptr<int>);  // expected-error {{interface type only allowed for a declaration with pointer or array type}}

//
// Valid type annotations
// 

// Single pointer and 1-d arrays

int *g32 : itype(ptr<int>);
int *g33 : itype(array_ptr<int>);
extern int g34[] : itype(int checked[]);
extern int g35[10] : itype(int checked[10]);
int *g39  : itype(ptr<int>);
int *g40  : itype(array_ptr<int>);

// Two levels of pointers

int **g50 : itype(ptr<ptr<int>>);
int **g51 : itype(ptr<array_ptr<int>>);
int **g52 : itype(array_ptr<ptr<int>>);
int **g53 : itype(array_ptr<array_ptr<int>>);
int **g54 : itype(ptr<int *>);
int **g55 : itype(array_ptr<int *>);
ptr<int> *g58 : itype(ptr<ptr<int>>);

// Multi-dimensional arrays

extern int g70[][10] : itype(int checked[][10]);
int g71[10][10] : itype(int checked[10][10]);

// Pointer to multi-dimensional arrays.
int (*g75)checked[10][10]: itype(ptr<int checked[10][10]>);

// Function pointers
int ((*g80)(int *, int *)) : itype(ptr<int(int *, int *)>);
int ((*g81)(int *, int *)) : itype(ptr<int(ptr<int>, ptr<int>)>);
int ((*g82)(int *, int *)) : itype(ptr<int(int checked[10], int checked[10])>);

// Spot check uses of type qualifiers.  They must be identical for the declared type
// and the bounds-safe interface type.

const int *g200 : itype(ptr<const int>);
volatile int *g201 : itype(ptr<volatile int>);
const volatile int *g202 : itype(ptr<const volatile int>);
const int *g203 : itype(array_ptr<const int>);
volatile int *g204 : itype(array_ptr<volatile int>);
const volatile int *g205 : itype(array_ptr<const volatile int>);
int * const g206 : itype(const ptr<int>);
int * volatile g207 : itype(volatile ptr<int>);
int * restrict g208  : itype(restrict ptr<int>);
int * const g209 : itype(const array_ptr<int>);
const int g218[10] : itype(const int checked[10]);

// Combinations of interface types and bounsd.

int global_len;
int *g301 : itype(array_ptr<int>) count(5);
int *g302 : count(5) itype(array_ptr<int>);
int **g303 : itype(array_ptr<ptr<int>>) bounds(g303, g303 + global_len);
int **g304 : bounds(g304, g304 + global_len) itype(array_ptr<ptr<int>>);
int **g305 : itype(array_ptr<ptr<int>>) byte_count(global_len * sizeof(ptr<int>));
int **g306 : byte_count(global_len * sizeof(ptr<int>)) itype(array_ptr<ptr<int>>);
int global_len;
int g307[10][10] : itype(int checked[10][10]) count(10);
int g308[10][10] : count(10) itype(int checked[10][10]);

// Incomplete array
extern int g309[][10] : itype(int checked[][10]) count(global_len);
 
//
// Incorrect type annotations.
// 

// Array/pointer types are must be compatible for global variables

extern int g230[] : itype(ptr<int>);         // expected-error {{mismatch between interface type}}  
extern int g231[] : itype(array_ptr<int>);   // expected-error {{mismatch between interface type}}
int *g232  : itype(int checked[]);           // expected-error {{mismatch between interface type}}
int *g233 : itype(int checked[15]);         // expected-error {{mismatch between interface type}}

int g234[10][10] : itype(array_ptr<int checked[10]>);  // expected-error {{mismatch between interface type}}
int g235[10][10] : itype(array_ptr<int[10]>);          // expected-error {{mismatch between interface type}}
int **g236 : itype(ptr<int> checked[10]);               // expected-error {{mismatch between interface type}}
int **g237 : itype(array_ptr<int> checked[10]);         // expected-error {{mismatch between interface type}}
int **g238 : itype(int *checked[20]);                   // expected-error {{mismatch between interface type}}

// Incompatible pointee or element types

// Pointer types
float **g250 : itype(ptr<int *>);     // expected-error {{mismatch between interface type '_Ptr<int *>' and declared type 'float **'}}
float **g251 : itype(ptr<ptr<int>>);  // expected-error {{mismatch between interface type '_Ptr<_Ptr<int>>' and declared type 'float **'}}
float g252[10] : itype(double checked[10]); // expected-error {{mismatch between interface type}}

// Array types
int g253[10][10] : itype(int checked[10][11]); // expected-error {{mismatch between interface type 'int _Checked[10][11]' and declared type 'int [10][10]'}}
int (*g254)[10] : itype(ptr<int checked[]>);   // expected-error {{mismatch between interface type}}
int(*g255)[] : itype(ptr<int checked[10]>);    // expected-error {{mismatch between interface type}}

// Differing number of parameters for function pointer.
// See the earlier comment for f256 about why the function declarator is
// parenthesized.
int ((*g256)(int, float, char)) : itype(ptr<int (int, float)>);  // expected-error {{mismatch between interface type}}

// Differing parameter types for a function pointer.
// See the earlier comment for f256 about why the function declarator is
// parenthesized.
int ((*g257)(int, float, char)) : itype(ptr<int (int, float, double)>); // expected-error {{mismatch between interface type}}

// Differing return types for a function pointer.
// See the earlier comment for f256 about why the function declarator is
// parenthesized.
int ((*g258)(int, float, char)) : itype(ptr<float (int, float, char)>); // expected-error {{mismatch between interface type}}

// No special treatement for void pointers
void *g259 : itype(ptr<int>);   // expected-error {{mismatch between interface type}}
int *g260 : itype(ptr<void>);   // expected-error {{mismatch between interface type}}

// Incompatible array types

extern int g261[] : itype(int checked[10]);         // expected-error {{mismatch between interface type}}
int g262[10] : itype(int checked[]);                // expected-error {{mismatch between interface type}}
extern int g263[][10] : itype(int checked[10][10]); // expected-error {{mismatch between interface type}}
int g264[10][10] : itype(int checked[][10]);        // expected-error {{mismatch between interface type}}
int g265[9][10] : itype(int checked[10][10]);       // expected-error {{mismatch between interface type}}


// Annotation type loses checking.

ptr<int> *g280 : itype(ptr<int *>);  //expected-error {{type '_Ptr<int *>' loses checking of declared type '_Ptr<int> *'}}
int (*g281) checked[10][10] : itype(ptr<int[10][10]>);    // expected-error {{loses checking of declared type}}
int ((*g282)(int checked[10])) : itype(ptr<int (int[10])>); // expected-error {{loses checking of declared type}}


// Declaring bounds for a _Ptr type.

int *g320 : itype(ptr<int>) count(5); //expected-error {{cannot declare bounds with a _Ptr interface type}}
int *g321 : count(5) itype(ptr<int>);  //expected-error {{cannot declare bounds with a _Ptr interface type}}
int **g322 : itype(ptr<ptr<int>>) bounds(g322, g322 + global_len);  //expected-error {{cannot declare bounds with a _Ptr interface type}}
int **g323 : bounds(g323, g323 + global_len) itype(ptr<ptr<int>>);  //expected-error {{cannot declare bounds with a _Ptr interface type}}
int **g324 : itype(ptr<ptr<int>>) byte_count(global_len * sizeof(ptr<int>));   //expected-error {{cannot declare bounds with a _Ptr interface type}}
int **g325 : byte_count(global_len * sizeof(ptr<int>)) itype(ptr<ptr<int>>);    //expected-error {{cannot declare bounds with a _Ptr interface type}}

//-------------------------------------------------------------
//
// Tests for local variables with interface type annotations.
//
// Local variables cannot have interface type annotations.
//-------------------------------------------------------------

extern void test_locals() {
  int x1a : itype(ptr<int>);  // expected-error {{bounds-safe interface type annotation not allowed for local variable}}
  _Bool x2a : itype(ptr<int>);  // expected-error {{bounds-safe interface type annotation not allowed for local variable}}
  char x3a : itype(ptr<int>);  // expected-error {{bounds-safe interface type annotation not allowed for local variable}}
  short int x4a : itype(ptr<int>);  // expected-error {{bounds-safe interface type annotation not allowed for local variable}}
  long int x6a : itype(ptr<int>);  // expected-error {{bounds-safe interface type annotation not allowed for local variable}}
  float x7a : itype(ptr<int>);  // expected-error {{bounds-safe interface type annotation not allowed for local variable}}
  double x8a : itype(ptr<int>);  // expected-error {{bounds-safe interface type annotation not allowed for local variable}}
  struct S x10a : itype(ptr<int>);  // expected-error {{bounds-safe interface type annotation not allowed for local variable}}
  union U x11a : itype(ptr<int>);  // expected-error {{bounds-safe interface type annotation not allowed for local variable}}
  t1 x13a : itype(ptr<int>);  // expected-error {{bounds-safe interface type annotation not allowed for local variable}}
  t2 x14a : itype(ptr<int>);  // expected-error {{bounds-safe interface type annotation not allowed for local variable}}

  // Single pointer and 1-d arrays

  int *x32 : itype(ptr<int>);        // expected-error {{bounds-safe interface type annotation not allowed for local variable}}
  int *x33 : itype(array_ptr<int>);  // expected-error {{bounds-safe interface type annotation not allowed for local variable}}
  int x35[10] : itype(int checked[10]); // expected-error {{bounds-safe interface type annotation not allowed for local variable}}
  int *x39  : itype(ptr<int>); // expected-error {{bounds-safe interface type annotation not allowed for local variable}}
  int *x40  : itype(array_ptr<int>); // expected-error {{bounds-safe interface type annotation not allowed for local variable}}

  // Two levels of pointers

  int **x50 : itype(ptr<ptr<int>>);  // expected-error {{bounds-safe interface type annotation not allowed for local variable}}
  int **x51 : itype(ptr<array_ptr<int>>);  // expected-error {{bounds-safe interface type annotation not allowed for local variable}}
  int **x52 : itype(array_ptr<ptr<int>>);  // expected-error {{bounds-safe interface type annotation not allowed for local variable}}
  int **x53 : itype(array_ptr<array_ptr<int>>);  // expected-error {{bounds-safe interface type annotation not allowed for local variable}}
  int **x54 : itype(ptr<int *>);  // expected-error {{bounds-safe interface type annotation not allowed for local variable}}
  int **x55 : itype(array_ptr<int *>);  // expected-error {{bounds-safe interface type annotation not allowed for local variable}}
  ptr<int> *x58 : itype(ptr<ptr<int>>); // expected-error {{bounds-safe interface type annotation not allowed for local variable}}

  // Multi-dimensional arrays

  int x71[10][10] : itype(int checked[10][10]);  // expected-error {{bounds-safe interface type annotation not allowed for local variable}}

  // Pointer to multi-dimensional arrays.
  int (*x75)checked[10][10]: itype(ptr<int checked[10][10]>);  // expected-error {{bounds-safe interface type annotation not allowed for local variable}}

  // Function pointers
  int ((*x80)(int *, int *)) : itype(ptr<int(int *, int *)>);  // expected-error {{bounds-safe interface type annotation not allowed for local variable}}
  int ((*x81)(int *, int *)) : itype(ptr<int(ptr<int>, ptr<int>)>);  // expected-error {{bounds-safe interface type annotation not allowed for local variable}}
  int ((*x82)(int *, int *)) : itype(ptr<int(int checked[10], int checked[10])>);  // expected-error {{bounds-safe interface type annotation not allowed for local variable}}

  // Spot check uses of type qualifiers.  They must be identical for the declared type
  // and the bounds-safe interface type.

  const int *x200 : itype(ptr<const int>);  // expected-error {{bounds-safe interface type annotation not allowed for local variable}}
  volatile int *x201 : itype(ptr<volatile int>);  // expected-error {{bounds-safe interface type annotation not allowed for local variable}}
}


//-------------------------------------------------------------
//
// Tests for structure members with interface type annotations.
//
//-------------------------------------------------------------

//
// Test types not allowed to appear in bounds-safe interface type annotations.
//

struct S1 {
  int *g1 : itype(int);       // expected-error {{must be a pointer or array type}}
  int *g2 : itype(_Bool);     // expected-error {{must be a pointer or array type}}
  int *g3 : itype(char);      // expected-error {{must be a pointer or array type}}
  int *g4 : itype(short int); // expected-error {{must be a pointer or array type}}
  int *g5 : itype(int);       // expected-error {{must be a pointer or array type}}
  int *g6 : itype(long int);  // expected-error {{must be a pointer or array type}}
  int *g7 : itype(float);     // expected-error {{must be a pointer or array type}}
  int *g8 : itype(double);    // expected-error {{must be a pointer or array type}}
  int *g9 : itype(void);      // expected-error {{must be a pointer or array type}}
  int *g10 : itype(struct S); // expected-error {{must be a pointer or array type}}
  int *g11 : itype(union U);  // expected-error {{must be a pointer or array type}}
  int *g12 : itype(int(int)); // expected-error {{must be a pointer or array type}}
  int *g13 : itype(t1);       // expected-error {{must be a pointer or array type}}
  int *g14 : itype(t2);       // expected-error {{must be a pointer or array type}}
  int *g30 : itype(int *);    // expected-error {{must be a checked type}}
  int g31[] : itype(int[]);   // expected-error {{must be a checked type}}
};

//
// Types that cannot have bounds-safe interface type annotations.
//

struct S1a {;
  int g1a : itype(ptr<int>);  // expected-error {{interface type only allowed for a declaration with pointer or array type}}
  _Bool g2a : itype(ptr<int>);  // expected-error {{interface type only allowed for a declaration with pointer or array type}}
  char g3a : itype(ptr<int>);  // expected-error {{interface type only allowed for a declaration with pointer or array type}}
  short int g4a : itype(ptr<int>);  // expected-error {{interface type only allowed for a declaration with pointer or array type}}
  long int  g6a : itype(ptr<int>);  // expected-error {{interface type only allowed for a declaration with pointer or array type}}
  float g7a : itype(ptr<int>);  // expected-error {{interface type only allowed for a declaration with pointer or array type}}
  double g8a : itype(ptr<int>);  // expected-error {{interface type only allowed for a declaration with pointer or array type}}
  struct S g10a : itype(ptr<int>);  // expected-error {{interface type only allowed for a declaration with pointer or array type}}
  union U g11a : itype(ptr<int>);  // expected-error {{interface type only allowed for a declaration with pointer or array type}}
  t1 g13a : itype(ptr<int>);  // expected-error {{interface type only allowed for a declaration with pointer or array type}}
  t2 g14a : itype(ptr<int>);  // expected-error {{interface type only allowed for a declaration with pointer or array type}}
};

//
// Valid type annotations
// 

// Single pointer and 1-d arrays

struct S2 {
  int *g32 : itype(ptr<int>);
  int *g33 : itype(array_ptr<int>);
  int g35[10] : itype(int checked[10]);
  int *g39  : itype(ptr<int>);
  int *g40  : itype(array_ptr<int>);
  // Incomplete array type allowed for the last member of a structure.
  int g34[] : itype(int checked[]);
};


// Two levels of pointers

struct S3 {
  int **g50 : itype(ptr<ptr<int>>);
  int **g51 : itype(ptr<array_ptr<int>>);
  int **g52 : itype(array_ptr<ptr<int>>);
  int **g53 : itype(array_ptr<array_ptr<int>>);
  int **g54 : itype(ptr<int *>);
  int **g55 : itype(array_ptr<int *>);
  ptr<int> *g58 : itype(ptr<ptr<int>>);
};

struct S4 {
  // Multi-dimensional arrays
  int g71[10][10] : itype(int checked[10][10]);

  // Pointer to multi-dimensional arrays.
  int (*g75)checked[10][10]: itype(ptr<int checked[10][10]>);

  // Function pointers
  int ((*g80)(int *, int *)) : itype(ptr<int(int *, int *)>);
  int ((*g81)(int *, int *)) : itype(ptr<int(ptr<int>, ptr<int>)>);
  int ((*g82)(int *, int *)) : itype(ptr<int(int checked[10], int checked[10])>);

  // Incomplete array type allowed for the last member of a structure.
  int g70[][10] : itype(int checked[][10]);
};

// Conbinations of interfaces and bounds expressions.

struct Combo1 {
   int *g301 : itype(array_ptr<int>) count(5);
   int *g302 : count(5) itype(array_ptr<int>);
   int **g303 : itype(array_ptr<ptr<int>>) bounds(g303, g303 + len);
   int **g304 : bounds(g304, g304 + len) itype(array_ptr<ptr<int>>);
   int **g305 : itype(array_ptr<ptr<int>>) byte_count(len * sizeof(ptr<int>));
   int **g306 : byte_count(len * sizeof(ptr<int>)) itype(array_ptr<ptr<int>>);
   int len;
   int g307[10][10] : itype(int checked[10][10]) count(10);
   int g308[10][10] : count(10) itype(int checked[10][10]);

   // Incomplete array type allowed for the last member of a structure.
   int g309[][10] : itype(int checked[][10]) count(len);
};


// Spot check uses of type qualifiers.  They must be identical for the declared type
// and the bounds-safe interface type.

struct S5 {
  const int *g200 : itype(ptr<const int>);
  volatile int *g201 : itype(ptr<volatile int>);
  const volatile int *g202 : itype(ptr<const volatile int>);
  const int *g203 : itype(array_ptr<const int>);
  volatile int *g204 : itype(array_ptr<volatile int>);
  const volatile int *g205 : itype(array_ptr<const volatile int>);
  int * const g206 : itype(const ptr<int>);
  int * volatile g207 : itype(volatile ptr<int>);
  int * restrict g208  : itype(restrict ptr<int>);
  int * const g209 : itype(const array_ptr<int>);
  const int g218[10] : itype(const int checked[10]);
};
 
//
// Incorrect type annotations.
// 

// Array/pointer types are must be compatible for structure members

struct S6 {
  int *g232 : itype(int checked[]);           // expected-error {{mismatch between interface type}}
  int *g233 : itype(int checked[15]);         // expected-error {{mismatch between interface type}}
  int g234[10][10] : itype(array_ptr<int checked[10]>);  // expected-error {{mismatch between interface type}}
  int g235[10][10] : itype(array_ptr<int[10]>);          // expected-error {{mismatch between interface type}}
  int **g236 : itype(ptr<int> checked[10]);               // expected-error {{mismatch between interface type}}
  int **g237 : itype(array_ptr<int> checked[10]);         // expected-error {{mismatch between interface type}}
  int **g238 : itype(int *checked[20]);                   // expected-error {{mismatch between interface type}}
  // Incomplete array type allowed as last member of structure.
  int g230[] : itype(ptr<int>);         // expected-error {{mismatch between interface type}}  
};

struct S7 {
  int a;
  int g231[] : itype(array_ptr<int>);   // expected-error {{mismatch between interface type}}
};

struct S8 {
  int a;
  int *g232  : itype(int checked[]); // expected-error {{mismatch between interface type}}
};

struct S9 {
  // Incompatible pointee or element types

  // Pointer types
  float **g250 : itype(ptr<int *>);     // expected-error {{mismatch between interface type '_Ptr<int *>' and declared type 'float **'}}
  float **g251 : itype(ptr<ptr<int>>);  // expected-error {{mismatch between interface type '_Ptr<_Ptr<int>>' and declared type 'float **'}}
  float g252[10] : itype(double checked[10]); // expected-error {{mismatch between interface type}}

  // Array types
  int g253[10][10] : itype(int checked[10][11]); // expected-error {{mismatch between interface type 'int _Checked[10][11]' and declared type 'int [10][10]'}}
  int (*g254)[10] : itype(ptr<int checked[]>);   // expected-error {{mismatch between interface type}}
  int(*g255)[] : itype(ptr<int checked[10]>);    // expected-error {{mismatch between interface type}}

  // Differing number of parameters for function pointer.
  // See the earlier comment for f256 about why the function declarator is
  // parenthesized.
  int ((*g256)(int, float, char)) : itype(ptr<int (int, float)>);  // expected-error {{mismatch between interface type}}

  // Differing parameter types for a function pointer.
  // See the earlier comment for f256 about why the function declarator is
  // parenthesized.
  int ((*g257)(int, float, char)) : itype(ptr<int (int, float, double)>); // expected-error {{mismatch between interface type}}

  // Differing return types for a function pointer.
  // See the earlier comment for f256 about why the function declarator is
  // parenthesized.
  int ((*g258)(int, float, char)) : itype(ptr<float (int, float, char)>); // expected-error {{mismatch between interface type}}

  // No special treatement for void pointers
  void *g259 : itype(ptr<int>);   // expected-error {{mismatch between interface type}}
  int *g260 : itype(ptr<void>);   // expected-error {{mismatch between interface type}}
};

struct S10 {
  int a;
  int g261[] : itype(int checked[10]);         // expected-error {{mismatch between interface type}}
};

struct S11 {
  int a;
  int g263[][10] : itype(int checked[10][10]); // expected-error {{mismatch between interface type}}
};

struct S12 {
  int g262[10] : itype(int checked[]);                // expected-error {{mismatch between interface type}}
  int g264[10][10] : itype(int checked[][10]);        // expected-error {{mismatch between interface type}}
  int g265[9][10] : itype(int checked[10][10]);       // expected-error {{mismatch between interface type}}
};

// Annotation type loses checking.

struct S13 {
  ptr<int> *g280 : itype(ptr<int *>);  //expected-error {{type '_Ptr<int *>' loses checking of declared type '_Ptr<int> *'}}
  int (*g281) checked[10][10] : itype(ptr<int[10][10]>);    // expected-error {{loses checking of declared type}}
  int ((*g282)(int checked[10])) : itype(ptr<int (int[10])>); // expected-error {{loses checking of declared type}}
};


// Declaring bounds for a _Ptr interface type
struct Combo2 {
   int *g320 : itype(ptr<int>) count(5); //expected-error {{cannot declare bounds with a _Ptr interface type}}
   int *g321 : count(5) itype(ptr<int>);  //expected-error {{cannot declare bounds with a _Ptr interface type}}
   int **g322 : itype(ptr<ptr<int>>) bounds(g322, g322 + len);  //expected-error {{cannot declare bounds with a _Ptr interface type}}
   int **g323 : bounds(g323, g323 + len) itype(ptr<ptr<int>>);  //expected-error {{cannot declare bounds with a _Ptr interface type}}
   int **g324 : itype(ptr<ptr<int>>) byte_count(len * sizeof(ptr<int>));   //expected-error {{cannot declare bounds with a _Ptr interface type}}
   int **g325 : byte_count(len * sizeof(ptr<int>)) itype(ptr<ptr<int>>);    //expected-error {{cannot declare bounds with a _Ptr interface type}}
   int len;
};


//-------------------------------------------------------------
//
// Tests for union members with interface type annotations.
//
//-------------------------------------------------------------

struct U1 {
  int *g1 : itype(int);       // expected-error {{must be a pointer or array type}}
  int *g2 : itype(_Bool);     // expected-error {{must be a pointer or array type}}
  int *g3 : itype(char);      // expected-error {{must be a pointer or array type}}
  int *g4 : itype(short int); // expected-error {{must be a pointer or array type}}
  int *g5 : itype(int);       // expected-error {{must be a pointer or array type}}
  int *g6 : itype(long int);  // expected-error {{must be a pointer or array type}}
  int *g7 : itype(float);     // expected-error {{must be a pointer or array type}}
  int *g8 : itype(double);    // expected-error {{must be a pointer or array type}}
  int *g9 : itype(void);      // expected-error {{must be a pointer or array type}}
  int *g10 : itype(struct S); // expected-error {{must be a pointer or array type}}
  int *g11 : itype(union U);  // expected-error {{must be a pointer or array type}}
  int *g12 : itype(int(int)); // expected-error {{must be a pointer or array type}}
  int *g13 : itype(t1);       // expected-error {{must be a pointer or array type}}
  int *g14 : itype(t2);       // expected-error {{must be a pointer or array type}}
  int *g30 : itype(int *);    // expected-error {{must be a checked type}}
  int g31[] : itype(int[]);   // expected-error {{must be a checked type}}
};

//
// Valid type annotations
// 

// Single pointer and 1-d arrays

union U2 {
  int *g32 : itype(ptr<int>);
  int *g33 : itype(array_ptr<int>);
  int g35[10] : itype(int checked[10]);
  int *g39  : itype(ptr<int>);
  int *g40  : itype(array_ptr<int>);
};


// Two levels of pointers

union U3 {
  int **g50 : itype(ptr<ptr<int>>);
  int **g51 : itype(ptr<array_ptr<int>>);
  int **g52 : itype(array_ptr<ptr<int>>);
  int **g53 : itype(array_ptr<array_ptr<int>>);
  int **g54 : itype(ptr<int *>);
  int **g55 : itype(array_ptr<int *>);
  ptr<int> *g58 : itype(ptr<ptr<int>>);
};

union U4 {
  // Multi-dimensional arrays
  int g71[10][10] : itype(int checked[10][10]);

  // Pointer to multi-dimensional arrays.
  int(*g75)checked[10][10]: itype(ptr<int checked[10][10]>);

  // Function pointers
  int ((*g80)(int *, int *)) : itype(ptr<int(int *, int *)>);
  int ((*g81)(int *, int *)) : itype(ptr<int(ptr<int>, ptr<int>)>);
  int ((*g82)(int *, int *)) : itype(ptr<int(int checked[10], int checked[10])>);
};

// Spot check uses of type qualifiers.  They must be identical for the declared type
// and the bounds-safe interface type.

union U5 {
  const int *g200 : itype(ptr<const int>);
  volatile int *g201 : itype(ptr<volatile int>);
  const volatile int *g202 : itype(ptr<const volatile int>);
  const int *g203 : itype(array_ptr<const int>);
  volatile int *g204 : itype(array_ptr<volatile int>);
  const volatile int *g205 : itype(array_ptr<const volatile int>);
  int * const g206 : itype(const ptr<int>);
  int * volatile g207 : itype(volatile ptr<int>);
  int * restrict g208  : itype(restrict ptr<int>);
  int * const g209 : itype(const array_ptr<int>);
  const int g218[10] : itype(const int checked[10]);
};

//
// Incorrect type annotations.
// 

// Array/pointer types are must be compatible for structure members

union U6 {
  int *g232 : itype(int checked[]);           // expected-error {{mismatch between interface type}}
  int *g233 : itype(int checked[15]);         // expected-error {{mismatch between interface type}}
  int g234[10][10] : itype(array_ptr<int checked[10]>);  // expected-error {{mismatch between interface type}}
  int g235[10][10] : itype(array_ptr<int[10]>);          // expected-error {{mismatch between interface type}}
  int **g236 : itype(ptr<int> checked[10]);               // expected-error {{mismatch between interface type}}
  int **g237 : itype(array_ptr<int> checked[10]);         // expected-error {{mismatch between interface type}}
  int **g238 : itype(int *checked[20]);                   // expected-error {{mismatch between interface type}}
};


union U9 {
  // Incompatible pointee or element types

  // Pointer types
  float **g250 : itype(ptr<int *>);     // expected-error {{mismatch between interface type '_Ptr<int *>' and declared type 'float **'}}
  float **g251 : itype(ptr<ptr<int>>);  // expected-error {{mismatch between interface type '_Ptr<_Ptr<int>>' and declared type 'float **'}}
  float g252[10] : itype(double checked[10]); // expected-error {{mismatch between interface type}}

  // Array types
  int g253[10][10] : itype(int checked[10][11]); // expected-error {{mismatch between interface type 'int _Checked[10][11]' and declared type 'int [10][10]'}}
  int(*g254)[10] : itype(ptr<int checked[]>);   // expected-error {{mismatch between interface type}}
  int(*g255)[] : itype(ptr<int checked[10]>);    // expected-error {{mismatch between interface type}}

  // Differing number of parameters for function pointer.
  // See the earlier comment for f256 about why the function declarator is
  // parenthesized.
  int((*g256)(int, float, char)) : itype(ptr<int(int, float)>);  // expected-error {{mismatch between interface type}}

  // Differing parameter types for a function pointer.
  // See the earlier comment for f256 about why the function declarator is
  // parenthesized.
  int((*g257)(int, float, char)) : itype(ptr<int(int, float, double)>); // expected-error {{mismatch between interface type}}

  // Differing return types for a function pointer.
  // See the earlier comment for f256 about why the function declarator is
  // parenthesized.
  int((*g258)(int, float, char)) : itype(ptr<float(int, float, char)>); // expected-error {{mismatch between interface type}}

                                                                        // No special treatement for void pointers
  void *g259 : itype(ptr<int>);   // expected-error {{mismatch between interface type}}
  int *g260 : itype(ptr<void>);   // expected-error {{mismatch between interface type}}
};

union U12 {
  int g262[10] : itype(int checked[]);                // expected-error {{mismatch between interface type}}
  int g264[10][10] : itype(int checked[][10]);        // expected-error {{mismatch between interface type}}
  int g265[9][10] : itype(int checked[10][10]);       // expected-error {{mismatch between interface type}}
};

// Annotation type loses checking.

union U13 {
  ptr<int> *g280 : itype(ptr<int *>);  //expected-error {{type '_Ptr<int *>' loses checking of declared type '_Ptr<int> *'}}
  int(*g281) checked[10][10] : itype(ptr<int[10][10]>);    // expected-error {{loses checking of declared type}}
  int((*g282)(int checked[10])) : itype(ptr<int(int[10])>); // expected-error {{loses checking of declared type}}
};
