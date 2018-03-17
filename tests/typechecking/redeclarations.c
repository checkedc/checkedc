// Feature tests of typechecking redeclarations of functions
// and variables
//
// The following lines are for the LLVM test harness:
//
// RUN: %clang_cc1 -verify -verify-ignore-unexpected=note %s

#include <stdchecked.h>

//---------------------------------------------------------------------------//
// Declarations of functions with unchecked parameters are compatible with   //
// declarations that add bounds-safe interfaces.                             //
//---------------------------------------------------------------------------//

void f1(int *p, int len);
void f1(int *p : count(len), int len);

void f2(int *p, int len);
void f2(int *p : bounds(p, p + len), int len);

void f3(int *p, int len);
void f3(int *p : byte_count(len * sizeof(int)), int len);

void f4(int *p, int len);
void f4(int *p : itype(ptr<int>), int len);

void f4a(char *p : itype(nt_array_ptr<char>), int len);

// Order doesn't matter for declarations
void f5(int *p : count(len), int len);
void f5(int *p, int len);

void f6(int *p : bounds(p, p + len), int len);
void f6(int *p, int len);

void f7(int *p : byte_count(len * sizeof(int)), int len);
void f7(int *p, int len);

void f8(int *p : itype(ptr<int>), int len);
void f8(int *p, int len);

void f8a(char * : itype(nt_array_ptr<char>), int len);
void f8a(char *, int len);

//---------------------------------------------------------------------------//
// Declarations of functions that return unchecked types are compatible      //
// with declarations that add bounds-safe interfaces.                        //
//---------------------------------------------------------------------------//

int *f20(int len);
int *f20(int len) : count(len);

int *f21(int len);
int *f21(int len) : byte_count(len * sizeof(int));

int *f22(int len);
int *f22(int len) : itype(ptr<int>);

char *f22a(int len);
char *f22a(int len) : itype(nt_array_ptr<char>);

// Order doesn't matter
int *f23(int len) : count(len);
int *f23(int len);

int *f24(int len) : byte_count(len * sizeof(int));
int *f24(int len);

int *f25(int len) : itype(ptr<int>);
int *f25(int len);

char *f25a(int len) : itype(nt_array_ptr<char>);
char *f25a(int len);

//---------------------------------------------------------------------------//
// Redeclarations of functions that have parameters with unchecked types     //
// that have bounds-safe interfaces must have matching interfaces.            //
//---------------------------------------------------------------------------//

//
// Only bounds declarations
//

void f30(int *p : count(len), int len);
void f30(int *p : count(len), int len);
void f30(int *p : count(len + 1), int len);  // expected-error {{conflicting parameter bounds}}

void f31(int *p : count(len), int len);
// A redeclaration without bounds-safe interface is compatible with the
// original declaration, but the function retains its original bounds-safe
// interface.
void f31(int *p, int len);
void f31(int *p : count(len + 1), int len);  // expected-error {{conflicting parameter bounds}}

void f32(int *p : bounds(p, p + len), int len);
void f32(int *p : bounds(p, p + len), int len);
void f32(int *p : bounds(p, p + len + 1), int len);  // expected-error {{conflicting parameter bounds}}

void f33(int *p : bounds(p, p + len), int len);
void f33(int *p, int len);
// A redeclaration without a bounds-safe interface is compatible with the
// original declaration, but the function retains its original bounds-safe
// interface
void f33(int *p : bounds(p, p + len + 1), int len);  // expected-error {{conflicting parameter bounds}}

void f34(int *p : itype(ptr<int>), int len);
void f34(int *p : count(len), int len);  // expected-error {{added bounds for parameter}}

void f35(int *p : count(len), int len);
void f35(int *p : itype(ptr<int>), int len); // expected-error {{dropped bounds for parameter}}

//
// Bounds declarations plus interface types
//

// Identical declarations and interface types.
void f30a(int **p : count(len) itype(array_ptr<ptr<int>>), int len);
void f30a(int **p : count(len) itype(array_ptr<ptr<int>>), int len);

void f30b(int **p : count(len) itype(nt_array_ptr<ptr<int>>), int len);
void f30b(int **p : count(len) itype(nt_array_ptr<ptr<int>>), int len);

// Implied interface types.
void f30b1(int **p : count(len) itype(array_ptr<int *>), int len);
void f30b1(int **p : count(len), int len);

void f30b2(int **p : count(len), int len);
void f30b2(int **p : count(len) itype(array_ptr<int *>), int len);

// TODO: these should be semantically identical (see Github issue #462).
void f30b3(int *p[5] : count(5));
void f30b3(int *p[5] : count(5) itype(int * checked[5])); // expected-error {{conflicting parameter interop type}}

void f30b4(int *p[5] : count(5) itype(int * checked[5]));
void f30b4(int *p[5] : count(5) itype(array_ptr<int>));   // expected-error {{mismatch between interface type}}

// Mismatched bounds declaration.
void f30e(int **p : count(len) itype(array_ptr<ptr<int>>), int len);
void f30e(int **p : count(len + 1) itype(array_ptr<ptr<int>>), int len);  // expected-error {{conflicting parameter bounds}}

// Mismatched interface type declaration.
void f30f(int **p : count(len) itype(array_ptr<ptr<int>>), int len);
void f30f(int **p : count(len) itype(array_ptr<int *>), int len);  // expected-error {{conflicting parameter interop type}}

// Dropped bounds declaration.
void f30g(int **p : count(len) itype(array_ptr<ptr<int>>), int len);
void f30g(int **p : itype(array_ptr<ptr<int>>), int len);  // expected-error {{dropped bounds for parameter}}

// Added bounds declaration.
void f30h(int **p : itype(array_ptr<ptr<int>>), int len);
void f30h(int **p : count(len) itype(array_ptr<ptr<int>>), int len);  // expected-error {{added bounds for parameter}}

// Dropped interface type.
void f30i(int **p : count(len) itype(array_ptr<ptr<int>>), int len);
void f30i(int **p : count(len), int len);  // expected-error {{conflicting parameter interop type}}

// Added interface type.
void f30j(int **p : count(len), int len);
void f30j(int **p : count(len) itype(array_ptr<ptr<int>>), int len);    // expected-error {{conflicting parameter interop type}}

// Bounds implied by interface types.

// A parameter with an interface type that is a checked array implies
// a bounds of the first dimension.
void f30k(int **p : itype(ptr<int> checked[5]));
void f30k(int **p : itype(ptr<int> checked[5]) count(5));
void f30k(int **p : itype(ptr<int> checked[5]) count(6));   // expected-error {{conflicting parameter bounds}}

void f30l(int **p : itype(ptr<int> checked[5]) count(5));
void f30l(int **p : itype(ptr<int> checked[5]));
void f30l(int **p : itype(ptr<int> checked[5]) count(6));   // expected-error {{conflicting parameter bounds}}

void f30m(int *p[5] : itype(ptr<int> checked[5]) count(5));
void f30m(int *p[5] : itype(ptr<int> checked[5]));
void f30m(int *p[5] : itype(ptr<int> checked[5]) count(6)); // expected-error {{conflicting parameter bounds}}

// nt_array_ptr type implies count(0) if no bounds specified
void f30n(char *p : itype(nt_array_ptr<char>));
void f30n(char *p : itype(nt_array_ptr<char>) count(0));
void f30n(char *p : itype(nt_array_ptr<char>) count(1));  // expected-error {{conflicting parameter bounds}}

void f30p(char *p : itype(nt_array_ptr<char>) count(5));
void f30p(char *p : itype(nt_array_ptr<char>) count(1));  // expected-error {{conflicting parameter bounds}}

void f30q(char *p : itype(char nt_checked[5]));
void f30q(char *p : itype(char nt_checked[5]) count(4));
void f30q(char *p : itype(char nt_checked[5]) count(5));  // expected-error {{conflicting parameter bounds}}

void f30r(char *p : itype(char nt_checked[]));
void f30r(char *p : itype(char nt_checked[]) count(0));

//---------------------------------------------------------------------------//
// Redeclarations of functions that have parameters that have bounds         //
// declarations must have matching declarations.                             //
//---------------------------------------------------------------------------//
void f40(array_ptr<int> p : count(len), int len);
void f40(array_ptr<int> p : count(len), int len);
void f40(array_ptr<int> p : count(len + 1), int len); // expected-error {{conflicting parameter bounds}}

void f41(array_ptr<int> p : bounds(p, p + len), int len);
void f41(array_ptr<int> p : bounds(p, p + len), int len);
void f41(array_ptr<int> p : bounds(p, p + len + 1), int len); // expected-error {{conflicting parameter bounds}}

// Add a parameter bounds declaration.
void f42(array_ptr<int> p, int len);
void f42(array_ptr<int> p : count(len), int len);  // expected-error {{function redeclaration added bounds for parameter}}

// Drop a parameter bounds declaration.
void f43(array_ptr<int> p : count(len), int len);
void f43(array_ptr<int> p, int len);               // expected-error {{function redeclaration dropped bounds for parameter}}

void f44(array_ptr<int> p);
void f44(array_ptr<int> p : bounds(unknown));

void f45(int p checked[5]);
void f45(int p  checked[5] : count(5));

// Pointers with a null-terminated pointer type have a default bounds of count(0)
void f46(nt_array_ptr<int> p);
void f46(nt_array_ptr<int> p : count(0));
void f46(nt_array_ptr<int> p : count(1));   // expected-error {{conflicting parameter bounds}}

//---------------------------------------------------------------------------//
// Redeclarations of functions that have bounds-safe interfaces for returns  //
// must have matching interfaces.                                            //
//---------------------------------------------------------------------------//
int *f50(int len) : count(len);
int *f50(int len) : count(len);
int *f50(int len) : count(len + 1);  // expected-error {{conflicting return bounds}}
int *f51(int len) : count(len);
// A redeclaration without bounds-safe interface is compatible with the
// original declaration, but the function retains its original bounds-safe
// interface.
int *f51(int len);
int *f51(int len) : count(len + 1);  // expected-error {{conflicting return bounds}}

int *f52(int *p : bounds(p, p + len), int len) : bounds(p, p + len);
int *f52(int *p : bounds(p, p + len), int len) : bounds(p, p + len);
int *f52(int *p : bounds(p, p + len), int len) : bounds(p, p + len + 1);  // expected-error {{conflicting return bounds}}

int *f53(int *p : bounds(p, p + len), int len) : bounds(p, p + len);
int *f53(int *p, int len);
// A redeclaration without a bounds-safe interface is compatible with the
// original declaration, but the function retains its original bounds-safe
// interface.
int *f53(int *p : bounds(p, p + len), int len) : bounds(p, p + len + 1);  // expected-error {{conflicting return bounds}}

int *f54(int len) : itype(ptr<int>);
int *f54(int len) : count(len);  // expected-error {{added return bounds}}

//
// Bounds declarations plus interface types
//

// Identical declarations and interface types.
int **f50a(int len) : count(len) itype(array_ptr<ptr<int>>);
int **f50a(int len) : count(len) itype(array_ptr<ptr<int>>);

int **f50b(int len) : count(len) itype(nt_array_ptr<ptr<int>>);
int **f50b(int len) : count(len) itype(nt_array_ptr<ptr<int>>);

// Implied interface type.
int **f50c(int len) : count(len);
int **f50c(int len) : count(len) itype(array_ptr<int *>);

int **f50d(int len) : count(len) itype(array_ptr<int *>);
int **f50d(int len) : count(len);

// Mismatched bounds declaration.
int **f50e(int len) : count(len) itype(array_ptr<ptr<int>>);
int **f50e(int len) : count(len+1) itype(array_ptr<ptr<int>>);  // expected-error {{conflicting return bounds}}

// Mismatched interface type declaration.
int **f50f(int len) : count(len) itype(array_ptr<ptr<int>>);
int **f50f(int len) : count(len) itype(array_ptr<int *>);  // expected-error {{conflicting return interop type}}

// Dropped bounds declaration.
int **f50g(int len) : count(len) itype(array_ptr<ptr<int>>);
int **f50g(int len) : itype(array_ptr<ptr<int>>);  // expected-error {{dropped return bounds}}

// Added bounds declaration.
int **f50h(int len) : itype(array_ptr<ptr<int>>);
int **f50h(int len) : count(len) itype(array_ptr<ptr<int>>);  // expected-error {{added return bounds}}

// Dropped interface type.
int **f50i(int len) : count(len) itype(array_ptr<ptr<int>>);
int **f50i(int len) : count(len);  // expected-error {{conflicting return interop type}}

// Added interface type.
int **f50j(int len) : count(len);
int **f50j(int len) : count(len) itype(array_ptr<ptr<int>>); // expected-error {{conflicting return interop type}}

// nt_array_ptr type implies count(0)
char *f50k(int len) : itype(nt_array_ptr<char>) count(0);
char *f50k(int len) : itype(nt_array_ptr<char>);
char *f50k(int len) : itype(nt_array_ptr<char>) count(1);  // expected-error {{conflicting return bounds}}

char *f50l(int len) : itype(nt_array_ptr<char>);
char *f50l(int len) : itype(nt_array_ptr<char>) count(0);
char *f50l(int len) : itype(nt_array_ptr<char>) count(1);  // expected-error {{conflicting return bounds}}

//---------------------------------------------------------------------------//
// Redeclarations of functions that have bounds declarations for returns     //
// must have matching declarations.                                          //
//---------------------------------------------------------------------------//
array_ptr<int> f60(int len) : count(len);
array_ptr<int> f60(int len) : count(len);
array_ptr<int> f60(int len) : count(len + 1); // expected-error {{conflicting return bounds}}

array_ptr<int> f61(array_ptr<int> p : count(len), int len) : bounds(p, p + len);
array_ptr<int> f61(array_ptr<int> p : count(len), int len) : bounds(p, p + len);
array_ptr<int> f61(array_ptr<int> p : count(len), int len) : bounds(p, p + len - 1); // expected-error {{conflicting return bounds}}

// Add a bounds declaration.
array_ptr<int> f62(int len);
array_ptr<int> f62(int len) : count(len);   // expected-error {{function redeclaration added return bounds}}

// Drop a bounds declaration.
array_ptr<int> f63(int len) : count(len);
array_ptr<int> f63(int len);                // expected-error {{function redeclaration dropped return bound}}

//---------------------------------------------------------------------------//
// Redeclarations of functions that have parameters with function pointer    //
// types that have bounds-safe interfaces must have matching bounds-safe     //
// interfaces.                                                               //
//---------------------------------------------------------------------------//

// Interface type on parameter with function pointer type.
void f70(int * fn(int *, int *));
void f70(int * (fn(int *, int *)) :
           itype(array_ptr<int> (ptr<int>, ptr<int>)));
// identical redeclaration.
void f70(int * (fn(int *, int *)) :
           itype(array_ptr<int> (ptr<int>, ptr<int>)));
// Add interface types for parameters, still valid
void f70(int * (fn(int * : itype(ptr<int>), int * : itype(ptr<int>))) :
  itype(array_ptr<int>(ptr<int>, ptr<int>)));
// return type of itype differs.
void f70(int (*fn(int *, int *)) : itype(ptr<int> (ptr<int>, ptr<int>))); // expected-error {{function redeclaration has conflicting parameter interop type}}
// changed interface types for parameters of function pointer
void f70(int * (fn(int * : itype(array_ptr<int>), int * : itype(array_ptr<int>)))); // expected-error {{conflicting bounds}}

// Interface type on parameters of a function pointer type
void f71(int * fn(int *, int *));
void f71(int * fn(int * : count(5), int *: count(5)));
void f71(int * fn(int * : count(6), int * : count(6))); // expected-error {{conflicting bounds annotations for 'f71'}}

// Interface type on return value of a function pointer type
void f72(int * fn(int *, int *));
void f72(int * fn(int *, int *) : itype(ptr<int>));

//---------------------------------------------------------------------------//
// The bounds declarations must be syntactically identical for now, modulo   //
// parameter names.                                                          //
//---------------------------------------------------------------------------//

void f80(int *p : count(len), int len);
// Rename parameters
void f80(int *p : count(mylen), int mylen);
void f80(int *r : count(i), int i);
void f80(int *p : count((len)), int len); // expected-error {{conflicting parameter bounds}}

void f81(int *p : count(len), int len);
void f81(int *p : bounds(p, p + len), int len);  // expected-error {{conflicting parameter bounds}}

//---------------------------------------------------------------------------//
// Redeclarations of functions that have parameters with function pointer    //
// types that have bounds declarations must have matching bounds declarations//
//---------------------------------------------------------------------------//

void f90(void (*fnptr)(array_ptr<int> p1 : count(5)));
void f90(void (*fnptr)(array_ptr<int> p2 : count(5)));
void f90(void (*fnptr)(array_ptr<int> p1 : count(6)));  // expected-error {{conflicting bounds annotations for 'f90'}}

void f91(ptr<int(array_ptr<int> mid : bounds(p1, p1 + 5), array_ptr<int> p1)> fnptr);
void f91(ptr<int(array_ptr<int> mid : bounds(p1, p1 + 5), array_ptr<int> p1)> fnptr);
void f91(ptr<int(array_ptr<int> mid : bounds(p1, p1 + 6), array_ptr<int> p1)> fnptr);  // expected-error {{conflicting bounds annotations for 'f91'}}

void f92(array_ptr<int>(*fnptr)(int i, int k) : count(i));
void f92(array_ptr<int>(*fnptr)(int j, int k) : count(j));
void f92(array_ptr<int>(*fnptr)(int j, int k) : count(k)); // expected-error {{conflicting bounds annotations for 'f92'}}

void f93(array_ptr<int>(*fnptr)(void) : count(5));
void f93(array_ptr<int>(*f)(void) : count(5));
void f93(array_ptr<int>(*f)(void) : count(6));          // expected-error {{conflicting bounds annotations for 'f93'}}

//---------------------------------------------------------------------------//
// Declarations of variables with unchecked pointer or array types are       //
// compatible with declarations that add bounds-safe interfaces.             //
//---------------------------------------------------------------------------//

// Unchecked pointer types
int *g1;
int *g1 : count(5);

int *g2;
int *g2 : bounds(g2, g2 + 5);

int *g3;
int *g3 :  byte_count(5 * sizeof(int));

int *g4;
int *g4 : itype(ptr<int>);

// Order doesn't matter for declarations
int *g5 : count(5);
int *g5;

int *g6 : bounds(g6, g6 + 5);
int *g6;

int *g7: byte_count(5 * sizeof(int));
int *g7;

int *g8 : itype(ptr<int>);
int *g8;

// Umchecked array types
extern int g10[];
extern int g10[] : count(5);

int g11[5];
int g11[5] : bounds(g11, g11 + 5);

extern int g12[];
extern int g12[] :  byte_count(5 * sizeof(int));

int g13[10];
int g13[10] : itype(int checked[10]);

// Order doesn't matter for declarations
int g14[10] : count(5);
int g14[10];

int g15[5] : bounds(g15, g15 + 5);
int g15[5];

int g16[5]: byte_count(5 * sizeof(int));
int g16[];

int g17[5] : itype(int checked[5]);
int g17[5];

//---------------------------------------------------------------------------//
// Redeclarations of variables that have bounds-safe interfaces must have    //
// matching interfaces.                                                      //
//---------------------------------------------------------------------------//

// Unchecked pointers
int  len;
int *g30 : count(len);
int *g30 : count(len);
int *g30 : count(len + 1);  // expected-error {{conflicting bounds}}

int *g31 : count(len);
// A redeclaration without a bounds-safe interface is compatible with the
// original declaration, but the variable retains its original bounds-safe
// interface.
int *g31;
int *g31 : count(len + 1); // expected-error {{conflicting bounds}}

int *g32 : bounds(g32, g32 + len);
int *g32 : bounds(g32, g32 + len);
int *g32 : bounds(g32, g32 + len + 1);   // expected-error {{conflicting bounds}}

int *g33 : bounds(g33, g33 + len);
int *g33;
// A redeclaration without a bounds-safe interface is compatible with the
// original declaration, but the function retains its original bounds-safe
// interface
int *g33 : bounds(g33, g33 + len + 1);  // expected-error {{conflicting bounds}}

int *g34 : itype(ptr<int>);
int *g34 : count(1);      // expected-error {{added bounds}}

// Unchecked arrays
extern int g35[] : count(len);
extern int g35[] : count(len);
extern int g35[] : count(len + 1);  // expected-error {{conflicting bounds}}

extern int g36[] : count(len);



// A redeclaration without a bounds-safe interface is compatible with the
// original declaration, but the variable retains its original bounds-safe
// interface.
extern int g36[];
extern int g36[] : count(len + 1); // expected-error {{conflicting bounds}}

extern int g37[] : bounds(g37, g37 + len);
extern int g37[] : bounds(g37, g37 + len);
extern int g37[] : bounds(g37, g37 + len + 1);   // expected-error {{conflicting bounds}}

int g38[5] : bounds(g38, g38 + 5);
int g38[];
// A redeclaration without a bounds-safe interface is compatible with the
// original declaration, but the function retains its original bounds-safe
// interface
int g38[] : bounds(g38, g38 + 6);  // expected-error {{conflicting bounds}}

int g39[5] : itype(int checked[5]);  // Implies count(5)
int g39[5] : count(5);               // Implies itype(int checked[5]);

//
// Bounds declarations plus interface types
//

// Identical declarations and interface types.
int **g31a : count(len) itype(array_ptr<ptr<int>>);
int **g31a : count(len) itype(array_ptr<ptr<int>>);

char *g31b : count(len) itype(nt_array_ptr<char>);
char *g31b : count(len) itype(nt_array_ptr<char>);

// Implied interface type.
int **g31c : count(len);
int **g31c : count(len) itype(array_ptr<int *>);

int **g31d : bounds(g31c, g31c + len) itype(array_ptr<int *>);
int **g31d : bounds(g31c, g31c + len);

// Mismatched bounds declaration.
int **g31e : count(len) itype(array_ptr<ptr<int>>);
int **g31e : count(len+1) itype(array_ptr<ptr<int>>);  // expected-error {{conflicting bounds}}

// Mismatched interface type declaration.
int **g31f : bounds(g31e, g31e + len) itype(array_ptr<ptr<int>>);
int **g31f : bounds(g31e, g31e + len) itype(array_ptr<int *>);  // expected-error {{conflicting interop type}}

// Dropped bounds declaration.
int **g31g : byte_count(len * sizeof(int *)) itype(array_ptr<ptr<int>>);
int **g31g : itype(array_ptr<ptr<int>>);  // expected-error {{dropped bounds}}

// Added bounds declaration.
int **g31h : itype(array_ptr<ptr<int>>);
int **g31h: count(len) itype(array_ptr<ptr<int>>);  // expected-error {{added bounds}}

// Dropped interface type.
int **g31i : count(len) itype(array_ptr<ptr<int>>);
int **g31i : count(len);  // expected-error {{conflicting interop type}}

// Added interface type.
int **g31j : count(len);
int **g31j : count(len) itype(array_ptr<ptr<int>>); // expected-error {{conflicting interop type}}

// nt_array_ptr type implies count(0)
char *g31k : itype(nt_array_ptr<char>);
char *g31k : itype(nt_array_ptr<char>) count(0);
char *g31k : itype(nt_array_ptr<char>) count(1);  // expected-error {{conflicting bounds}}

//---------------------------------------------------------------------------//
// Redeclarations of variables that have bounds declarations must have       //
// matching declarations.                                                    //
//---------------------------------------------------------------------------//

// Checked pointer
array_ptr<int> g40 : count(len);
array_ptr<int> g40 : count(len);
array_ptr<int> g40 : count(len + 1);   // expected-error {{conflicting bounds}}

array_ptr<int> g41 : bounds(g41, g41 + 5);
array_ptr<int> g41 : bounds(g41, g41 + 5);
array_ptr<int> g41 : bounds(g41, g41 + 6); // expected-error {{conflicting bounds}}

// Add a bounds declaration.
array_ptr<int> g42;
array_ptr<int> g42 : count(5);  // expected-error {{variable redeclaration added bounds}}

// Drop a bounds declaration.
array_ptr<int> g43 : count(len);
array_ptr<int> g43;             // expected-error {{variable redeclaration dropped bounds}}

// Checked arrays with bounds.
// This is useful for incomplete array types.
extern float g50 checked[] : count(5);
float g50 checked[5] : count(5);

float g51 checked[10] : count(len);
float g51 checked[10] : count(len);
float g51 checked[10] : count(len + 1);   // expected-error {{conflicting bounds}}

extern float g52 checked[] : bounds(g52, g52 + 5);
extern float g52 checked[] : bounds(g52, g52 + 5);
extern float g52 checked[] : bounds(g52, g52 + 6); // expected-error {{conflicting bounds}}


// Add a bounds declaration.
extern float g53 checked[];
extern float g53 checked[] : count(5);  // expected-error {{variable redeclaration added bounds}}

// Drop a bounds declaration.
extern float g54 checked[] : count(len);
extern float g54 checked[];   // expected-error {{variable redeclaration dropped bounds}}

//---------------------------------------------------------------------------//
// Variables that hide global variables can be declared and have             //
// different bounds than the hidden variables.                               //
//---------------------------------------------------------------------------//

array_ptr<int> g100 : count(5);
array_ptr<int> g101 : bounds(g2, g2 + 3);
array_ptr<char> g102 : byte_count(len);
int g103;

// Locals
void f100(void) {
  int g100 = 0;
  array_ptr<float> g101 : byte_count(sizeof(float)*5) = 0;
  array_ptr<int> g102 = 0;
}

// Parameters
void f102(array_ptr<int> g100, array_ptr<int> g101 : count(5)) {
}

array_ptr<int> f103(array_ptr<int> g103 : count(5)) : bounds(g103, g103 + 2) {
  return 0;
}


//---------------------------------------------------------------------------//
// Variables that hide local variables can be declared in nested scopes      //
// and have different bounds.                                                //
//---------------------------------------------------------------------------//

void f104(int len) {
  array_ptr<int> g102 = 0;
  {
    int g102;
    {
      array_ptr<int> g102 : count(len) = 0;
      {
         int len = 5;
         array_ptr<int> g102  : byte_count(len * sizeof(int)) = 0;
      }
    }
  }
}

//---------------------------------------------------------------------------//
// Local extern declarations with different bounds than external (top-level) //
// extern declarations are not allowed.                                      //
//---------------------------------------------------------------------------//

// External-scoped extern declarations followed by locally-scoped extern
// declarations
extern int buf1_count;
extern array_ptr<int> buf1 : count(buf1_count);

void f105(void) {
  extern int buf1_count;
  extern array_ptr<int> buf1;  // expected-error {{dropped bounds}}
}

// Local-scoped extern declarations followed by externally-scoped extern
// declarations
void f106(void) {
  extern int buf2_count;
  extern array_ptr<int> buf2;
}

extern int buf2_count;
extern array_ptr<int> buf2 : count(buf2_count);  // expected-error {{added bounds}}

// Sibling locally-scoped extern declarations.
void f107(void) {
  extern int buf3_count;
  extern array_ptr<int> buf3;
}

void f108(void) {
  extern int buf3_count;
  extern array_ptr<int> buf3 : count(buf3_count); // expected-error {{added bounds}}
}
