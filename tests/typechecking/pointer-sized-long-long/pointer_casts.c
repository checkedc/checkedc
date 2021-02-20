// Check Pointer Casts on platforms where pointers and `long long`s are the same size
// In particular, this includes 64-bit windows.

// To configure the list of platforms, change lit.local.cfg in this directory
// to make the 'pointer-sized-long-long' feature available

// RUN: %clang_cc1 -std=c11 -verify -verify-ignore-unexpected=note -fcheckedc-extension %s

#include <stdchecked.h>

_Static_assert(sizeof(void*) == sizeof(long long),
  "Pointers and long longs must be the same size");

struct S1 {
  int len;
  // bound declaration on a long-typed member.
  long long p : bounds((char *)p, (char *)p + len);
};

extern struct S1 f1();    // expected-error {{function with no prototype cannot have a return type that is a structure with a member with a checked type}}

extern void f2();
extern void f2(struct S1); // expected-error {{cannot redeclare a function with no prototype to have an argument type that is a structure with a member with a checked type}}

extern void f3();
extern void f3(long long p : bounds((char *)p, (char *)p + len), int len);

extern void f4(long long p : bounds((char *)p, (char *)p + len), int len);
extern void f4();

//
// Spot-check other attempts at creating no prototype functions that return
// a checked value.
//

enum E1 {
  EnumVal1,
  EnumVal2
};

//
// Valid bounds declarations for integer-typed global variables.
//

int s1 checked[16];

// byte_count
long long g21 : byte_count(5 * sizeof(int)) = (long long)s1;
long long int g22 : byte_count(5 * sizeof(int)) = (long long int)s1;
unsigned long long int g23 : byte_count(5 * sizeof(int)) = (unsigned long long int) s1;
// TODO: Enum size is implementation-defined
// enum E1 g24 : byte_count(8) = EnumVal1;

// bounds
long long g25 : bounds(s1, s1 + 5) = (long long)s1;
long long int g26 : bounds(s1, s1 + 5) = (long long)s1;
unsigned long long int g27 : bounds(s1, s1 + 5) = (long long)s1;
// TODO: Enum size is implementation-defined
// enum E1 g28 : bounds(s1, s1 + 5) = (int)s1;

extern int f10(int a, _Array_ptr<int> b, _Array_ptr<char> p : bounds(b, b + a));
extern int f10(int a, _Array_ptr<int> b,
                _Array_ptr<char> p : bounds(b, (_Array_ptr<int>) (long long) b + a));
extern int f10(int a, _Array_ptr<int> b,
               _Array_ptr<char> p : bounds(b, (_Array_ptr<int>) (short) b + a)); // expected-error {{function redeclaration has conflicting parameter bounds}} \
                                                                                 // expected-warning 2 {{cast to smaller integer type 'short' from '_Array_ptr<int>'}} \
                                                                                 // expected-warning 2 {{cast to '_Array_ptr<int>' from smaller integer type 'short'}}

extern int f11(int a, _Array_ptr<int> b, _Array_ptr<char> p : bounds(b, b + a));
extern int f11(int a, _Array_ptr<int> b,
                _Array_ptr<char> p : bounds(b, (_Array_ptr<int>) (void *) (long long) b + a));
