// Check Pointer Casts on platforms where pointers and `long`s are the same size
// This pretty much only excludes 64-bit windows

// To configure the list of platforms, change lit.local.cfg in this directory
// to make the 'pointer-sized-long' feature available

// RUN: %clang_cc1 -verify -verify-ignore-unexpected=note %s

#include <stdchecked.h>

_Static_assert(sizeof(void*) == sizeof(long),
  "Pointers and longs must be the same size");

struct S1 {
  int len;
  // bound declaration on a long-typed member.
  long p  _Bounds((char *)p, (char *)p + len);
};

extern struct S1 f1();    // expected-error {{function with no prototype cannot have a return type that is a structure with a member with a checked type}}

extern void f2();
extern void f2(struct S1); // expected-error {{cannot redeclare a function with no prototype to have an argument type that is a structure with a member with a checked type}}

extern void f3();
extern void f3(long p : bounds((char *)p, (char *)p + len), int len);

extern void f4(long p  _Bounds((char *)p, (char *)p + len), int len);
extern void f4();

//
// Valid bounds declarations for integer-typed global variables.
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
// Cannot initialize this at compile time.
// short int g20 : byte_count(5 * sizeof(int)) = (short int) s1;
long g21 : byte_count(5 * sizeof(int)) = (long)s1;
long int g22 : byte_count(5 * sizeof(int)) = (long int)s1;
unsigned long int g23 : byte_count(5 * sizeof(int)) = (unsigned long int) s1;
// TODO: Enum size is implementation-defined
// enum E1 g24 : byte_count(8) = EnumVal1;

// bounds
long g25 : bounds(s1, s1 + 5) = (long)s1;
long int g26 : bounds(s1, s1 + 5) = (long)s1;
unsigned long int g27 : bounds(s1, s1 + 5) = (long)s1;
// TODO: Enum size is implementation-defined
// enum E1 g28 : bounds(s1, s1 + 5) = (int)s1;

// Check that casts between pointers and long ints do not affect
// equivalence of bounds

extern int f10(int a, _Array_ptr<int> b, _Array_ptr<char> p : bounds(b, b + a));
extern int f10(int a, _Array_ptr<int> b,
                _Array_ptr<char> p : bounds(b, (_Array_ptr<int>) (long) b + a));
extern int f10(int a, _Array_ptr<int> b,
               _Array_ptr<char> p : bounds(b, (_Array_ptr<int>) (short) b + a)); // expected-error {{function redeclaration has conflicting parameter bounds}} \
                                                                                 // expected-warning 2 {{cast to '_Array_ptr<int>' from smaller integer type 'short'}} \
                                                                                 // expected-warning 2 {{cast to smaller integer type 'short' from '_Array_ptr<int>'}}

extern int f11(int a, int* _Array b, char* _Array p  _Bounds(b, b + a));
extern int f11(int a, int* _Array b,
               char* _Array p  _Bounds(b, (int* _Array) (void *) (long) b + a));

