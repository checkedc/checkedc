// Test that lexical equality of expressions is being determined properly.
// It is difficult to test this directly, so we test it indirectly
// by redeclaring functions with different bounds expressions.
//
// RUN: %clang_cc1 -verify -verify-ignore-unexpected=note %s

//--------------------------------------------------------------------------//
// Check the cross product of different kinds of expressions.               //
//                                                                          //
// For each kind of expression, we check different versions of that         //
// expression first.  Then we check against prior kinds of expressions.     //
//--------------------------------------------------------------------------//


//--------------------------------------------------------------------------//
// Constants                                                                //
//--------------------------------------------------------------------------//

// TODO: Handle additional kinds of integer constants (hex, octal, or suffixed 
// with L or U).  

// Integer constant equality.
extern int f1_1(_Array_ptr<int> p : count(0));
extern int f1_1(_Array_ptr<int> p : count(0));
extern int f1_2(_Array_ptr<int> p : count(1));
extern int f1_2(_Array_ptr<int> p : count(1));
extern int f1_3(_Array_ptr<int> p : count(5));
extern int f1_3(_Array_ptr<int> p : count(5));

// Integer constant inequality
extern int f1_4(_Array_ptr<int> p : count(0));
extern int f1_4(_Array_ptr<int> p : count(1));         // expected-error {{conflicting parameter bounds}}
extern int f1_5(_Array_ptr<int> p : count(0));
extern int f1_5(_Array_ptr<int> p : count(5));         // expected-error {{conflicting parameter bounds}}

// Character constant equality
extern int f1_6(_Array_ptr<int> p : count('a'));
extern int f1_6(_Array_ptr<int> p : count('a'));

// Character constant inequality
extern int f1_7(_Array_ptr<int> p : count('a'));
extern int f1_7(_Array_ptr<int> p : count('b'));       // expected-error {{conflicting parameter bounds}}

// Character constant vs. integer constant inequality
extern int f1_8(_Array_ptr<int> p : count(0));
extern int f1_8(_Array_ptr<int> p : count('a'));       // expected-error {{conflicting parameter bounds}}


// Floating-point constants,  
//
// The semantics of floating-point numbers is not specified by the C11 
// Standard, so we compare floating-point constants lexically, not based
// on mathematical values.  We could allow implementation-specific
// constant-folding of floating point expressions, but then we would have to
// allow implementation-specific comparison of floating-point constants.  
// That does not seem like a good idea given how widely different 
// floating-point behavior can be.

// Equality of floating point constants with type double.
extern int f2_1(_Array_ptr<int> p : count((int) 0.0));
extern int f2_1(_Array_ptr<int> p : count((int) 0.0));

extern int f2_2(_Array_ptr<int> p : count((int) 1.0));
extern int f2_2(_Array_ptr<int> p : count((int) 1.0));

// Inequality of floating point constants with type double.
extern int f2_3(_Array_ptr<int> p : count((int) 0.0));
extern int f2_3(_Array_ptr<int> p : count((int) 5.0));   // expected-error {{conflicting parameter bounds}}

// Equality of floating point constants with type float.
extern int f3_1(_Array_ptr<int> p : count((int) 0.0f));
extern int f3_1(_Array_ptr<int> p : count((int) 0.0f));
extern int f3_1(_Array_ptr<int> p : count((int) 0.0F));
extern int f3_1(_Array_ptr<int> p : count((int) 0.0F));

extern int f3_2(_Array_ptr<int> p : count((int) 5.0f));
extern int f3_2(_Array_ptr<int> p : count((int) 5.0f));
extern int f3_2(_Array_ptr<int> p : count((int) 5.0F));
extern int f3_2(_Array_ptr<int> p : count((int) 5.0F));

// Inequality of floating point constants with type float.
extern int f4_1(_Array_ptr<int> p : count((int) 0.0f));
extern int f4_1(_Array_ptr<int> p : count((int) 5.0f));  // expected-error {{conflicting parameter bounds}}
extern int f4_2(_Array_ptr<int> p : count((int) 0.0F));
extern int f4_2(_Array_ptr<int> p : count((int) 5.0F));  // expected-error {{conflicting parameter bounds}}
extern int f4_3(_Array_ptr<int> p : count((int) 0.0f));
extern int f4_3(_Array_ptr<int> p : count((int) 5.0F));  // expected-error {{conflicting parameter bounds}}

// Inequality of floating point constants with type float and type double.

extern int f5_1(_Array_ptr<int> p : count((int) 0.0));
extern int f5_1(_Array_ptr<int> p : count((int) 0.0f));  // expected-error {{conflicting parameter bounds}}
extern int f5_2(_Array_ptr<int> p : count((int) 0.0));
extern int f5_2(_Array_ptr<int> p : count((int) 0.0F));  // expected-error {{conflicting parameter bounds}}

// Equality of floating point constants with type long double.
extern int f6_1(_Array_ptr<int> p : count((int) 0.0l));
extern int f6_1(_Array_ptr<int> p : count((int) 1.0l));  // expected-error {{conflicting parameter bounds}}
extern int f6_2(_Array_ptr<int> p : count((int) 0.0l));
extern int f6_2(_Array_ptr<int> p : count((int) 5.0L));  // expected-error {{conflicting parameter bounds}}
extern int f6_3(_Array_ptr<int> p : count((int) 0.0l));
extern int f6_3(_Array_ptr<int> p : count((int) 5.0l));  // expected-error {{conflicting parameter bounds}}

// Inequality of floating point constants with type double and type
// long double.

extern int f7_1(_Array_ptr<int> p : count((int) 0.0));
extern int f7_1(_Array_ptr<int> p : count((int) 0.0l));  // expected-error {{conflicting parameter bounds}}
extern int f7_2(_Array_ptr<int> p : count((int) 0.0));
extern int f7_2(_Array_ptr<int> p : count((int) 0.0L));  // expected-error {{conflicting parameter bounds}}

// Inequality of floating point constants with type float and 
// type long double.

extern int f8_1(_Array_ptr<int> p : count((int) 0.0f));
extern int f8_1(_Array_ptr<int> p : count((int) 0.0l));  // expected-error {{conflicting parameter bounds}}
extern int f8_2(_Array_ptr<int> p : count((int) 0.0f));
extern int f8_2(_Array_ptr<int> p : count((int) 0.0L));  // expected-error {{conflicting parameter bounds}}

// We omit inequality of floating point constants and integer constants. We
// insert an explicit cast for integers so that the enclosing expressions 
// are identical except for the floating point vs. intger constant.

extern int f9_1(_Array_ptr<int> p : count((int) 0));
extern int f9_1(_Array_ptr<int> p : count((int) 0.0));  // expected-error {{conflicting parameter bounds}}
extern int f9_2(_Array_ptr<int> p : count((int) 5));
extern int f9_2(_Array_ptr<int> p : count((int) 5.0f));  // expected-error {{conflicting parameter bounds}}
extern int f9_3(_Array_ptr<int> p : count((int) 150));
extern int f9_3(_Array_ptr<int> p : count((int) 1.5e2L));  // expected-error {{conflicting parameter bounds}}

//-----------------------------------------------------//
// Variables                                           //
//-----------------------------------------------------//

// Global variable equality.

extern int arr1_len;
extern int arr2_len;
extern _Array_ptr<int> arr1_1 : count(arr1_len);
extern _Array_ptr<int> arr1_1 : count(arr1_len);

// Global variable inequality.
extern _Array_ptr<int> arr1_2 : count(arr1_len);
extern _Array_ptr<int> arr1_2 : count(arr2_len);       // expected-error {{variable redeclaration has conflicting bounds}}

// Parameter variable equality.   Positions of arguments matter, not
// variable names.
extern int f20_1(_Array_ptr<int> arr1 : count(len), int len);
extern int f20_1(_Array_ptr<int> diff_arr1 : count(l), int l);
extern int f20_2(int c, _Array_ptr<int> arr1 : count(c));
extern int f20_2(int d, _Array_ptr<int> arr1 : count(d));

// Parameter variable inequality.
extern int f20_3(_Array_ptr<int> arr1 : count(a), int a, int b);
extern int f20_3(_Array_ptr<int> arr1 : count(b), int a, int b); // expected-error {{conflicting parameter bounds}}

// We can't check parameter variable vs. global variable inequality just
// using redeclarations of functions.  Parameter bounds can only refer to
// addresses of global variables and it is non-sensical for a parameter bound
// to refer to the address of a parameter variable.

// Global variable vs. constant inequality.
extern _Array_ptr<int> arr1_3 : count(5);
extern _Array_ptr<int> arr1_3 : count(arr1_len); // expected-error {{variable redeclaration has conflicting bounds}}

// Parameter variable vs. constant inequality.
extern int f21_1(_Array_ptr<int> arr1 : count(len), int len);  
extern int f21_1(_Array_ptr<int> arr1 : count(5), int len);  // expected-error {{conflicting parameter bounds}}

//-----------------------------------------------------//
// Unary operator expressions.                         //
//-----------------------------------------------------//

// Equality of unary operator expressions. Argument expression and
// enclosing expression are identical; the operators are the same.

static int arr4[100];
extern int f30_1(int a, _Array_ptr<int> p : bounds(&arr4[5], &arr4[50]));
extern int f30_1(int a, _Array_ptr<int> p : bounds(&arr4[5], &arr4[50]));
extern int f30_2(_Ptr<int> plen, _Array_ptr<int> p : count(*plen));
extern int f30_2(_Ptr<int> plen, _Array_ptr<int> p : count(*plen));
extern int f30_3(int a, _Array_ptr<int> p : count(-a));
extern int f30_3(int a, _Array_ptr<int> p : count(-a));
extern int f30_4(int a, _Array_ptr<int> p : count(+a));
extern int f30_4(int a, _Array_ptr<int> p : count(+a));
extern int f30_5(int a, _Array_ptr<int> p : count(~a));
extern int f30_5(int a, _Array_ptr<int> p : count(~a));
extern int f30_6(int a, _Array_ptr<int> p : count(!a));
extern int f30_6(int a, _Array_ptr<int> p : count(!a));
extern int f30_7(int a, _Array_ptr<int> p : byte_count(sizeof(a)));
extern int f30_7(int a, _Array_ptr<int> p : byte_count(sizeof(a)));
extern int f30_8(int a, _Array_ptr<int> p : byte_count(sizeof(int[2])));
extern int f30_8(int a, _Array_ptr<int> p : byte_count(sizeof(int[2])));

// Inequality of integer unary operator expressions.  The argument expression
// and enclosing expression are identical; only the operators
// differ.

// Unary plus vs. unary negation.
extern int f31_1(_Array_ptr<int> p : count(+a), int a);
extern int f31_1(_Array_ptr<int> p : count(-a), int a); // expected-error {{conflicting parameter bounds}}

// Unary ~ vs. preceding unary operators (unary plus and unary negation).

extern int f31_2(_Array_ptr<int> p : count(~a), int a);
extern int f31_2(_Array_ptr<int> p : count(-a), int a); // expected-error {{conflicting parameter bounds}}
extern int f31_3(_Array_ptr<int> p : count(~a), int a);
extern int f31_3(_Array_ptr<int> p : count(+a), int a); // expected-error {{conflicting parameter bounds}}

// Unary ! vs preceding unary operators.
extern int f31_4(_Array_ptr<int> p : count(!a), int a);
extern int f31_4(_Array_ptr<int> p : count(-a), int a); // expected-error {{conflicting parameter bounds}}
extern int f31_5(_Array_ptr<int> p : count(!a), int a);
extern int f31_5(_Array_ptr<int> p : count(+a), int a); // expected-error {{conflicting parameter bounds}}
extern int f31_6(_Array_ptr<int> p : count(!a), int a);
extern int f31_6(_Array_ptr<int> p : count(~a), int a); // expected-error {{conflicting parameter bounds}}

// sizeof(expression) vs. preceding unary operators.
extern int f31_7(_Array_ptr<int> p : count(sizeof(a)), int a);
extern int f31_7(_Array_ptr<int> p : count(-a), int a); // expected-error {{conflicting parameter bounds}}
extern int f31_8(_Array_ptr<int> p : count(sizeof(a)), int a);
extern int f31_8(_Array_ptr<int> p : count(+a), int a); // expected-error {{conflicting parameter bounds}}
extern int f31_9(_Array_ptr<int> p : count(sizeof(a)), int a);
extern int f31_9(_Array_ptr<int> p : count(~a), int a); // expected-error {{conflicting parameter bounds}}
extern int f31_10(_Array_ptr<int> p : count(sizeof(a)), int a);
extern int f31_10(_Array_ptr<int> p : count(!a), int a); // expected-error {{conflicting parameter bounds}}

// inequality of sizeof(expression) vs. size(type).
extern int f31_11(int a, _Array_ptr<int> p : byte_count(sizeof(a)));
extern int f31_11(int a, _Array_ptr<int> p : byte_count(sizeof(int[2]))); // expected-error {{conflicting parameter bounds}}

// Inequality of address-of expressions
extern int f32_1(int a, _Array_ptr<int> p : bounds(&arr4[5], &arr4[50]));
extern int f32_1(int a, _Array_ptr<int> p : bounds(&arr4[6], &arr4[50]));  // expected-error {{conflicting parameter bounds}}

// Inequality of dereference expressions
extern int f32_2(_Ptr<int> plen, _Array_ptr<int> p : count(*plen), _Ptr<int> slen);
extern int f32_2(_Ptr<int> plen, _Array_ptr<int> p : count(*slen), _Ptr<int> slen); // expected-error {{conflicting parameter bounds}}

// Inequality of dereference expressions vs. other unary operators
extern int f32_3(_Ptr<int> plen, _Array_ptr<int> p : count(*plen), int slen);
extern int f32_3(_Ptr<int> plen, _Array_ptr<int> p : count(+slen), int slen); // expected-error {{conflicting parameter bounds}}

// Inequality of unary operator expressions and constants.
extern int f32_4(_Array_ptr<int> p : count(!a), int a);
extern int f32_4(_Array_ptr<int> p : count(5), int a);            // expected-error {{conflicting parameter bounds}}
extern int f32_5(_Array_ptr<int> p : count((int) (!a)), int a);
extern int f32_5(_Array_ptr<int> p : count((int) (5.0)), int a);  // expected-error {{conflicting parameter bounds}}

// Inequality of unary operator expressions and variables.
extern int f32_6(_Array_ptr<int> p : count(!a), int a);
extern int f32_6(_Array_ptr<int> p : count(a), int a);            // expected-error {{conflicting parameter bounds}}

//-----------------------------------------------------//
// Binary operator expressions.                        //
//-----------------------------------------------------//

// Equality of binary operator expressions on integers.

// Multiplicative operators
extern int f40_1(int a, int b, _Array_ptr<int> p : count(a * b));
extern int f40_1(int a, int b, _Array_ptr<int> p : count(a * b));
extern int f40_2(int a, int b, _Array_ptr<int> p : count(a / b));
extern int f40_2(int a, int b, _Array_ptr<int> p : count(a / b));
extern int f40_3(int a, int b, _Array_ptr<int> p : count(a % b));
extern int f40_3(int a, int b, _Array_ptr<int> p : count(a % b));

// Additive operators.
extern int f40_4(int a, int b, _Array_ptr<int> p : count(a + b));
extern int f40_4(int a, int b, _Array_ptr<int> p : count(a + b));
extern int f40_5(int a, int b, _Array_ptr<int> p : count(a - b));
extern int f40_5(int a, int b, _Array_ptr<int> p : count(a - b));

// Shift operators.
extern int f40_6(int a, int b, _Array_ptr<int> p : count(a << b));
extern int f40_6(int a, int b, _Array_ptr<int> p : count(a << b));
extern int f40_7(int a, int b, _Array_ptr<int> p : count(a >> b));
extern int f40_7(int a, int b, _Array_ptr<int> p : count(a >> b));

// Relational operators.
extern int f41_1(int a, int b, _Array_ptr<int> p : count(a < b));
extern int f41_1(int a, int b, _Array_ptr<int> p : count(a < b));
extern int f41_2(int a, int b, _Array_ptr<int> p : count(a > b));
extern int f41_2(int a, int b, _Array_ptr<int> p : count(a > b));
extern int f41_3(int a, int b, _Array_ptr<int> p : count(a <= b));
extern int f41_3(int a, int b, _Array_ptr<int> p : count(a <= b));
extern int f41_4(int a, int b, _Array_ptr<int> p : count(a >= b));
extern int f41_4(int a, int b, _Array_ptr<int> p : count(a >= b));

// Equality operators.
extern int f41_5(int a, int b, _Array_ptr<int> p : count(a == b));
extern int f41_5(int a, int b, _Array_ptr<int> p : count(a == b));
extern int f41_6(int a, int b, _Array_ptr<int> p : count(a != b));
extern int f41_6(int a, int b, _Array_ptr<int> p : count(a != b));

// Bitwise operators.
extern int f41_7(int a, int b, _Array_ptr<int> p : count(a & b));
extern int f41_7(int a, int b, _Array_ptr<int> p : count(a & b));
extern int f41_8(int a, int b, _Array_ptr<int> p : count(a ^ b));
extern int f41_8(int a, int b, _Array_ptr<int> p : count(a ^ b));
extern int f41_9(int a, int b, _Array_ptr<int> p : count(a | b));
extern int f41_9(int a, int b, _Array_ptr<int> p : count(a | b));

// Logical and operators.
extern int f42_1(int a, int b, _Array_ptr<int> p : count(a && b));
extern int f42_1(int a, int b, _Array_ptr<int> p : count(a && b));
extern int f42_2(int a, int b, _Array_ptr<int> p : count(a || b));
extern int f42_2(int a, int b, _Array_ptr<int> p : count(a || b));

// Equality of binary operators on pointers/integers.
extern int f43_1(int a, _Array_ptr<int> b, _Array_ptr<int> p : bounds(b, b + a));
extern int f43_1(int a, _Array_ptr<int> b, _Array_ptr<int> p : bounds(b, b + a));
extern int f43_2(int a, _Array_ptr<int> b, _Array_ptr<int> p : bounds(b - a, b));
extern int f43_2(int a, _Array_ptr<int> b, _Array_ptr<int> p : bounds(b - a, b));
extern int f43_3(_Array_ptr<int> a, _Array_ptr<int> b, _Array_ptr<int> p : count(b - a));
extern int f43_3(_Array_ptr<int> a, _Array_ptr<int> b, _Array_ptr<int> p : count(b - a));

// Inequality of binary operators on integers.
//
// Use macros so that we can cut-and-paste identical code.

// Division vs. multiplication.
#define NAME(num, index) f##num##_##index
#define TESTOP /
#define FUNC(index) NAME(50, index)

extern int FUNC(1)(int a, int b, _Array_ptr<int> p : count(a TESTOP b)); 
extern int FUNC(1)(int a, int b, _Array_ptr<int> p : count(a * b));  // expected-error {{conflicting parameter bounds}}

// Remainder vs. prior binary operators.
#undef TESTOP
#undef FUNC
#define TESTOP %
#define FUNC(index) NAME(51, index)

extern int FUNC(1)(int a, int b, _Array_ptr<int> p : count(a TESTOP b));
extern int FUNC(1)(int a, int b, _Array_ptr<int> p : count(a * b));  // expected-error {{conflicting parameter bounds}}

extern int FUNC(2)(int a, int b, _Array_ptr<int> p : count(a TESTOP b));
extern int FUNC(2)(int a, int b, _Array_ptr<int> p : count(a / b));  // expected-error {{conflicting parameter bounds}}

// Plus vs. prior binary operators.
#undef TESTOP
#undef FUNC
#define TESTOP +
#define FUNC(index) NAME(52, index)

extern int FUNC(1)(int a, int b, _Array_ptr<int> p : count(a TESTOP b));
extern int FUNC(1)(int a, int b, _Array_ptr<int> p : count(a * b));  // expected-error {{conflicting parameter bounds}}

extern int FUNC(2)(int a, int b, _Array_ptr<int> p : count(a TESTOP b));
extern int FUNC(2)(int a, int b, _Array_ptr<int> p : count(a / b));  // expected-error {{conflicting parameter bounds}}

extern int FUNC(3)(int a, int b, _Array_ptr<int> p : count(a TESTOP b));
extern int FUNC(3)(int a, int b, _Array_ptr<int> p : count(a % b));  // expected-error {{conflicting parameter bounds}}

// Minus vs. prior binary operators.
#undef TESTOP
#undef FUNC
#define TESTOP -
#define FUNC(index) NAME(53, index)

extern int FUNC(1)(int a, int b, _Array_ptr<int> p : count(a TESTOP b));
extern int FUNC(1)(int a, int b, _Array_ptr<int> p : count(a * b));  // expected-error {{conflicting parameter bounds}}

extern int FUNC(2)(int a, int b, _Array_ptr<int> p : count(a TESTOP b));
extern int FUNC(2)(int a, int b, _Array_ptr<int> p : count(a / b));  // expected-error {{conflicting parameter bounds}}

extern int FUNC(3)(int a, int b, _Array_ptr<int> p : count(a TESTOP b));
extern int FUNC(3)(int a, int b, _Array_ptr<int> p : count(a % b));  // expected-error {{conflicting parameter bounds}}

extern int FUNC(4)(int a, int b, _Array_ptr<int> p : count(a TESTOP b));
extern int FUNC(4)(int a, int b, _Array_ptr<int> p : count(a + b));  // expected-error {{conflicting parameter bounds}}

// << vs. prior binary operators
#undef TESTOP
#undef FUNC
#define TESTOP <<
#define FUNC(index) NAME(54, index)

extern int FUNC(1)(int a, int b, _Array_ptr<int> p : count(a TESTOP b));
extern int FUNC(1)(int a, int b, _Array_ptr<int> p : count(a * b));  // expected-error {{conflicting parameter bounds}}

extern int FUNC(2)(int a, int b, _Array_ptr<int> p : count(a TESTOP b));
extern int FUNC(2)(int a, int b, _Array_ptr<int> p : count(a / b));  // expected-error {{conflicting parameter bounds}}

extern int FUNC(3)(int a, int b, _Array_ptr<int> p : count(a TESTOP b));
extern int FUNC(3)(int a, int b, _Array_ptr<int> p : count(a % b));  // expected-error {{conflicting parameter bounds}}

extern int FUNC(4)(int a, int b, _Array_ptr<int> p : count(a TESTOP b));
extern int FUNC(4)(int a, int b, _Array_ptr<int> p : count(a + b));  // expected-error {{conflicting parameter bounds}}

extern int FUNC(5)(int a, int b, _Array_ptr<int> p : count(a TESTOP b));
extern int FUNC(5)(int a, int b, _Array_ptr<int> p : count(a - b));  // expected-error {{conflicting parameter bounds}}

// >> vs. prior binary operators
#undef TESTOP
#undef FUNC
#define TESTOP >>
#define FUNC(index) NAME(55, index)

extern int FUNC(1)(int a, int b, _Array_ptr<int> p : count(a TESTOP b));
extern int FUNC(1)(int a, int b, _Array_ptr<int> p : count(a * b));  // expected-error {{conflicting parameter bounds}}

extern int FUNC(2)(int a, int b, _Array_ptr<int> p : count(a TESTOP b));
extern int FUNC(2)(int a, int b, _Array_ptr<int> p : count(a / b));  // expected-error {{conflicting parameter bounds}}

extern int FUNC(3)(int a, int b, _Array_ptr<int> p : count(a TESTOP b));
extern int FUNC(3)(int a, int b, _Array_ptr<int> p : count(a % b));  // expected-error {{conflicting parameter bounds}}

extern int FUNC(4)(int a, int b, _Array_ptr<int> p : count(a TESTOP b));
extern int FUNC(4)(int a, int b, _Array_ptr<int> p : count(a + b));  // expected-error {{conflicting parameter bounds}}

extern int FUNC(5)(int a, int b, _Array_ptr<int> p : count(a TESTOP b));
extern int FUNC(5)(int a, int b, _Array_ptr<int> p : count(a - b));  // expected-error {{conflicting parameter bounds}}

extern int FUNC(6)(int a, int b, _Array_ptr<int> p : count(a TESTOP b));
extern int FUNC(6)(int a, int b, _Array_ptr<int> p : count(a << b));  // expected-error {{conflicting parameter bounds}}

// < vs. prior binary operators
#undef TESTOP
#undef FUNC
#define TESTOP <
#define FUNC(index) NAME(56, index)

extern int FUNC(1)(int a, int b, _Array_ptr<int> p : count(a TESTOP b));
extern int FUNC(1)(int a, int b, _Array_ptr<int> p : count(a * b));  // expected-error {{conflicting parameter bounds}}

extern int FUNC(2)(int a, int b, _Array_ptr<int> p : count(a TESTOP b));
extern int FUNC(2)(int a, int b, _Array_ptr<int> p : count(a / b));  // expected-error {{conflicting parameter bounds}}

extern int FUNC(3)(int a, int b, _Array_ptr<int> p : count(a TESTOP b));
extern int FUNC(3)(int a, int b, _Array_ptr<int> p : count(a % b));  // expected-error {{conflicting parameter bounds}}

extern int FUNC(4)(int a, int b, _Array_ptr<int> p : count(a TESTOP b));
extern int FUNC(4)(int a, int b, _Array_ptr<int> p : count(a + b));  // expected-error {{conflicting parameter bounds}}

extern int FUNC(5)(int a, int b, _Array_ptr<int> p : count(a TESTOP b));
extern int FUNC(5)(int a, int b, _Array_ptr<int> p : count(a - b));  // expected-error {{conflicting parameter bounds}}

extern int FUNC(6)(int a, int b, _Array_ptr<int> p : count(a TESTOP b));
extern int FUNC(6)(int a, int b, _Array_ptr<int> p : count(a << b));  // expected-error {{conflicting parameter bounds}}

extern int FUNC(7)(int a, int b, _Array_ptr<int> p : count(a TESTOP b));
extern int FUNC(7)(int a, int b, _Array_ptr<int> p : count(a >> b));  // expected-error {{conflicting parameter bounds}}

// <= vs. prior binary operators
#undef TESTOP
#undef FUNC
#define TESTOP <=
#define FUNC(index) NAME(57, index)

extern int FUNC(1)(int a, int b, _Array_ptr<int> p : count(a TESTOP b));
extern int FUNC(1)(int a, int b, _Array_ptr<int> p : count(a * b));  // expected-error {{conflicting parameter bounds}}

extern int FUNC(2)(int a, int b, _Array_ptr<int> p : count(a TESTOP b));
extern int FUNC(2)(int a, int b, _Array_ptr<int> p : count(a / b));  // expected-error {{conflicting parameter bounds}}

extern int FUNC(3)(int a, int b, _Array_ptr<int> p : count(a TESTOP b));
extern int FUNC(3)(int a, int b, _Array_ptr<int> p : count(a % b));  // expected-error {{conflicting parameter bounds}}

extern int FUNC(4)(int a, int b, _Array_ptr<int> p : count(a TESTOP b));
extern int FUNC(4)(int a, int b, _Array_ptr<int> p : count(a + b));  // expected-error {{conflicting parameter bounds}}

extern int FUNC(5)(int a, int b, _Array_ptr<int> p : count(a TESTOP b));
extern int FUNC(5)(int a, int b, _Array_ptr<int> p : count(a - b));  // expected-error {{conflicting parameter bounds}}

extern int FUNC(6)(int a, int b, _Array_ptr<int> p : count(a TESTOP b));
extern int FUNC(6)(int a, int b, _Array_ptr<int> p : count(a << b));  // expected-error {{conflicting parameter bounds}}

extern int FUNC(7)(int a, int b, _Array_ptr<int> p : count(a TESTOP b));
extern int FUNC(7)(int a, int b, _Array_ptr<int> p : count(a >> b));  // expected-error {{conflicting parameter bounds}}

extern int FUNC(8)(int a, int b, _Array_ptr<int> p : count(a TESTOP b));
extern int FUNC(8)(int a, int b, _Array_ptr<int> p : count(a < b));  // expected-error {{conflicting parameter bounds}}

// > vs. prior binary operators
#undef TESTOP
#undef FUNC
#define TESTOP >
#define FUNC(index) NAME(58, index)

extern int FUNC(1)(int a, int b, _Array_ptr<int> p : count(a TESTOP b));
extern int FUNC(1)(int a, int b, _Array_ptr<int> p : count(a * b));  // expected-error {{conflicting parameter bounds}}

extern int FUNC(2)(int a, int b, _Array_ptr<int> p : count(a TESTOP b));
extern int FUNC(2)(int a, int b, _Array_ptr<int> p : count(a / b));  // expected-error {{conflicting parameter bounds}}

extern int FUNC(3)(int a, int b, _Array_ptr<int> p : count(a TESTOP b));
extern int FUNC(3)(int a, int b, _Array_ptr<int> p : count(a % b));  // expected-error {{conflicting parameter bounds}}

extern int FUNC(4)(int a, int b, _Array_ptr<int> p : count(a TESTOP b));
extern int FUNC(4)(int a, int b, _Array_ptr<int> p : count(a + b));  // expected-error {{conflicting parameter bounds}}

extern int FUNC(5)(int a, int b, _Array_ptr<int> p : count(a TESTOP b));
extern int FUNC(5)(int a, int b, _Array_ptr<int> p : count(a - b));  // expected-error {{conflicting parameter bounds}}

extern int FUNC(6)(int a, int b, _Array_ptr<int> p : count(a TESTOP b));
extern int FUNC(6)(int a, int b, _Array_ptr<int> p : count(a << b));  // expected-error {{conflicting parameter bounds}}

extern int FUNC(7)(int a, int b, _Array_ptr<int> p : count(a TESTOP b));
extern int FUNC(7)(int a, int b, _Array_ptr<int> p : count(a >> b));  // expected-error {{conflicting parameter bounds}}

extern int FUNC(8)(int a, int b, _Array_ptr<int> p : count(a TESTOP b));
extern int FUNC(8)(int a, int b, _Array_ptr<int> p : count(a < b));  // expected-error {{conflicting parameter bounds}}

extern int FUNC(9)(int a, int b, _Array_ptr<int> p : count(a TESTOP b));
extern int FUNC(9)(int a, int b, _Array_ptr<int> p : count(a <= b));  // expected-error {{conflicting parameter bounds}}

// >= vs. prior binary operators
#undef TESTOP
#undef FUNC
#define TESTOP >=
#define FUNC(index) NAME(59, index)

extern int FUNC(1)(int a, int b, _Array_ptr<int> p : count(a TESTOP b));
extern int FUNC(1)(int a, int b, _Array_ptr<int> p : count(a * b));  // expected-error {{conflicting parameter bounds}}

extern int FUNC(2)(int a, int b, _Array_ptr<int> p : count(a TESTOP b));
extern int FUNC(2)(int a, int b, _Array_ptr<int> p : count(a / b));  // expected-error {{conflicting parameter bounds}}

extern int FUNC(3)(int a, int b, _Array_ptr<int> p : count(a TESTOP b));
extern int FUNC(3)(int a, int b, _Array_ptr<int> p : count(a % b));  // expected-error {{conflicting parameter bounds}}

extern int FUNC(4)(int a, int b, _Array_ptr<int> p : count(a TESTOP b));
extern int FUNC(4)(int a, int b, _Array_ptr<int> p : count(a + b));  // expected-error {{conflicting parameter bounds}}

extern int FUNC(5)(int a, int b, _Array_ptr<int> p : count(a TESTOP b));
extern int FUNC(5)(int a, int b, _Array_ptr<int> p : count(a - b));  // expected-error {{conflicting parameter bounds}}

extern int FUNC(6)(int a, int b, _Array_ptr<int> p : count(a TESTOP b));
extern int FUNC(6)(int a, int b, _Array_ptr<int> p : count(a << b));  // expected-error {{conflicting parameter bounds}}

extern int FUNC(7)(int a, int b, _Array_ptr<int> p : count(a TESTOP b));
extern int FUNC(7)(int a, int b, _Array_ptr<int> p : count(a >> b));  // expected-error {{conflicting parameter bounds}}

extern int FUNC(8)(int a, int b, _Array_ptr<int> p : count(a TESTOP b));
extern int FUNC(8)(int a, int b, _Array_ptr<int> p : count(a < b));  // expected-error {{conflicting parameter bounds}}

extern int FUNC(9)(int a, int b, _Array_ptr<int> p : count(a TESTOP b));
extern int FUNC(9)(int a, int b, _Array_ptr<int> p : count(a <= b));  // expected-error {{conflicting parameter bounds}}

extern int FUNC(10)(int a, int b, _Array_ptr<int> p : count(a TESTOP b));
extern int FUNC(10)(int a, int b, _Array_ptr<int> p : count(a > b));  // expected-error {{conflicting parameter bounds}}

// == vs. prior binary operators
#undef TESTOP
#undef FUNC
#define TESTOP ==
#define FUNC(index) NAME(60, index)

extern int FUNC(1)(int a, int b, _Array_ptr<int> p : count(a TESTOP b));
extern int FUNC(1)(int a, int b, _Array_ptr<int> p : count(a * b));  // expected-error {{conflicting parameter bounds}}

extern int FUNC(2)(int a, int b, _Array_ptr<int> p : count(a TESTOP b));
extern int FUNC(2)(int a, int b, _Array_ptr<int> p : count(a / b));  // expected-error {{conflicting parameter bounds}}

extern int FUNC(3)(int a, int b, _Array_ptr<int> p : count(a TESTOP b));
extern int FUNC(3)(int a, int b, _Array_ptr<int> p : count(a % b));  // expected-error {{conflicting parameter bounds}}

extern int FUNC(4)(int a, int b, _Array_ptr<int> p : count(a TESTOP b));
extern int FUNC(4)(int a, int b, _Array_ptr<int> p : count(a + b));  // expected-error {{conflicting parameter bounds}}

extern int FUNC(5)(int a, int b, _Array_ptr<int> p : count(a TESTOP b));
extern int FUNC(5)(int a, int b, _Array_ptr<int> p : count(a - b));  // expected-error {{conflicting parameter bounds}}

extern int FUNC(6)(int a, int b, _Array_ptr<int> p : count(a TESTOP b));
extern int FUNC(6)(int a, int b, _Array_ptr<int> p : count(a << b));  // expected-error {{conflicting parameter bounds}}

extern int FUNC(7)(int a, int b, _Array_ptr<int> p : count(a TESTOP b));
extern int FUNC(7)(int a, int b, _Array_ptr<int> p : count(a >> b));  // expected-error {{conflicting parameter bounds}}

extern int FUNC(8)(int a, int b, _Array_ptr<int> p : count(a TESTOP b));
extern int FUNC(8)(int a, int b, _Array_ptr<int> p : count(a < b));  // expected-error {{conflicting parameter bounds}}

extern int FUNC(9)(int a, int b, _Array_ptr<int> p : count(a TESTOP b));
extern int FUNC(9)(int a, int b, _Array_ptr<int> p : count(a <= b));  // expected-error {{conflicting parameter bounds}}

extern int FUNC(10)(int a, int b, _Array_ptr<int> p : count(a TESTOP b));
extern int FUNC(10)(int a, int b, _Array_ptr<int> p : count(a > b));  // expected-error {{conflicting parameter bounds}}

extern int FUNC(11)(int a, int b, _Array_ptr<int> p : count(a TESTOP b));
extern int FUNC(11)(int a, int b, _Array_ptr<int> p : count(a >= b));  // expected-error {{conflicting parameter bounds}}

// != vs. prior binary operators
#undef TESTOP
#undef FUNC
#define TESTOP !=
#define FUNC(index) NAME(61, index)

extern int FUNC(1)(int a, int b, _Array_ptr<int> p : count(a TESTOP b));
extern int FUNC(1)(int a, int b, _Array_ptr<int> p : count(a * b));  // expected-error {{conflicting parameter bounds}}

extern int FUNC(2)(int a, int b, _Array_ptr<int> p : count(a TESTOP b));
extern int FUNC(2)(int a, int b, _Array_ptr<int> p : count(a / b));  // expected-error {{conflicting parameter bounds}}

extern int FUNC(3)(int a, int b, _Array_ptr<int> p : count(a TESTOP b));
extern int FUNC(3)(int a, int b, _Array_ptr<int> p : count(a % b));  // expected-error {{conflicting parameter bounds}}

extern int FUNC(4)(int a, int b, _Array_ptr<int> p : count(a TESTOP b));
extern int FUNC(4)(int a, int b, _Array_ptr<int> p : count(a + b));  // expected-error {{conflicting parameter bounds}}

extern int FUNC(5)(int a, int b, _Array_ptr<int> p : count(a TESTOP b));
extern int FUNC(5)(int a, int b, _Array_ptr<int> p : count(a - b));  // expected-error {{conflicting parameter bounds}}

extern int FUNC(6)(int a, int b, _Array_ptr<int> p : count(a TESTOP b));
extern int FUNC(6)(int a, int b, _Array_ptr<int> p : count(a << b));  // expected-error {{conflicting parameter bounds}}

extern int FUNC(7)(int a, int b, _Array_ptr<int> p : count(a TESTOP b));
extern int FUNC(7)(int a, int b, _Array_ptr<int> p : count(a >> b));  // expected-error {{conflicting parameter bounds}}

extern int FUNC(8)(int a, int b, _Array_ptr<int> p : count(a TESTOP b));
extern int FUNC(8)(int a, int b, _Array_ptr<int> p : count(a < b));  // expected-error {{conflicting parameter bounds}}

extern int FUNC(9)(int a, int b, _Array_ptr<int> p : count(a TESTOP b));
extern int FUNC(9)(int a, int b, _Array_ptr<int> p : count(a <= b));  // expected-error {{conflicting parameter bounds}}

extern int FUNC(10)(int a, int b, _Array_ptr<int> p : count(a TESTOP b));
extern int FUNC(10)(int a, int b, _Array_ptr<int> p : count(a > b));  // expected-error {{conflicting parameter bounds}}

extern int FUNC(11)(int a, int b, _Array_ptr<int> p : count(a TESTOP b));
extern int FUNC(11)(int a, int b, _Array_ptr<int> p : count(a >= b));  // expected-error {{conflicting parameter bounds}}

extern int FUNC(12)(int a, int b, _Array_ptr<int> p : count(a TESTOP b));
extern int FUNC(12)(int a, int b, _Array_ptr<int> p : count(a == b));  // expected-error {{conflicting parameter bounds}}

// & vs. prior binary operators
#undef TESTOP
#undef FUNC
#define TESTOP &
#define FUNC(index) NAME(62, index)

extern int FUNC(1)(int a, int b, _Array_ptr<int> p : count(a TESTOP b));
extern int FUNC(1)(int a, int b, _Array_ptr<int> p : count(a * b));  // expected-error {{conflicting parameter bounds}}

extern int FUNC(2)(int a, int b, _Array_ptr<int> p : count(a TESTOP b));
extern int FUNC(2)(int a, int b, _Array_ptr<int> p : count(a / b));  // expected-error {{conflicting parameter bounds}}

extern int FUNC(3)(int a, int b, _Array_ptr<int> p : count(a TESTOP b));
extern int FUNC(3)(int a, int b, _Array_ptr<int> p : count(a % b));  // expected-error {{conflicting parameter bounds}}

extern int FUNC(4)(int a, int b, _Array_ptr<int> p : count(a TESTOP b));
extern int FUNC(4)(int a, int b, _Array_ptr<int> p : count(a + b));  // expected-error {{conflicting parameter bounds}}

extern int FUNC(5)(int a, int b, _Array_ptr<int> p : count(a TESTOP b));
extern int FUNC(5)(int a, int b, _Array_ptr<int> p : count(a - b));  // expected-error {{conflicting parameter bounds}}

extern int FUNC(6)(int a, int b, _Array_ptr<int> p : count(a TESTOP b));
extern int FUNC(6)(int a, int b, _Array_ptr<int> p : count(a << b));  // expected-error {{conflicting parameter bounds}}

extern int FUNC(7)(int a, int b, _Array_ptr<int> p : count(a TESTOP b));
extern int FUNC(7)(int a, int b, _Array_ptr<int> p : count(a >> b));  // expected-error {{conflicting parameter bounds}}

extern int FUNC(8)(int a, int b, _Array_ptr<int> p : count(a TESTOP b));
extern int FUNC(8)(int a, int b, _Array_ptr<int> p : count(a < b));  // expected-error {{conflicting parameter bounds}}

extern int FUNC(9)(int a, int b, _Array_ptr<int> p : count(a TESTOP b));
extern int FUNC(9)(int a, int b, _Array_ptr<int> p : count(a <= b));  // expected-error {{conflicting parameter bounds}}

extern int FUNC(10)(int a, int b, _Array_ptr<int> p : count(a TESTOP b));
extern int FUNC(10)(int a, int b, _Array_ptr<int> p : count(a > b));  // expected-error {{conflicting parameter bounds}}

extern int FUNC(11)(int a, int b, _Array_ptr<int> p : count(a TESTOP b));
extern int FUNC(11)(int a, int b, _Array_ptr<int> p : count(a >= b));  // expected-error {{conflicting parameter bounds}}

extern int FUNC(12)(int a, int b, _Array_ptr<int> p : count(a TESTOP b));
extern int FUNC(12)(int a, int b, _Array_ptr<int> p : count(a == b));  // expected-error {{conflicting parameter bounds}}

extern int FUNC(13)(int a, int b, _Array_ptr<int> p : count(a TESTOP b));
extern int FUNC(13)(int a, int b, _Array_ptr<int> p : count(a != b));  // expected-error {{conflicting parameter bounds}}

// ^ vs. prior binary operators
#undef TESTOP
#undef FUNC
#define TESTOP ^
#define FUNC(index) NAME(63, index)

extern int FUNC(1)(int a, int b, _Array_ptr<int> p : count(a TESTOP b));
extern int FUNC(1)(int a, int b, _Array_ptr<int> p : count(a * b));  // expected-error {{conflicting parameter bounds}}

extern int FUNC(2)(int a, int b, _Array_ptr<int> p : count(a TESTOP b));
extern int FUNC(2)(int a, int b, _Array_ptr<int> p : count(a / b));  // expected-error {{conflicting parameter bounds}}

extern int FUNC(3)(int a, int b, _Array_ptr<int> p : count(a TESTOP b));
extern int FUNC(3)(int a, int b, _Array_ptr<int> p : count(a % b));  // expected-error {{conflicting parameter bounds}}

extern int FUNC(4)(int a, int b, _Array_ptr<int> p : count(a TESTOP b));
extern int FUNC(4)(int a, int b, _Array_ptr<int> p : count(a + b));  // expected-error {{conflicting parameter bounds}}

extern int FUNC(5)(int a, int b, _Array_ptr<int> p : count(a TESTOP b));
extern int FUNC(5)(int a, int b, _Array_ptr<int> p : count(a - b));  // expected-error {{conflicting parameter bounds}}

extern int FUNC(6)(int a, int b, _Array_ptr<int> p : count(a TESTOP b));
extern int FUNC(6)(int a, int b, _Array_ptr<int> p : count(a << b));  // expected-error {{conflicting parameter bounds}}

extern int FUNC(7)(int a, int b, _Array_ptr<int> p : count(a TESTOP b));
extern int FUNC(7)(int a, int b, _Array_ptr<int> p : count(a >> b));  // expected-error {{conflicting parameter bounds}}

extern int FUNC(8)(int a, int b, _Array_ptr<int> p : count(a TESTOP b));
extern int FUNC(8)(int a, int b, _Array_ptr<int> p : count(a < b));  // expected-error {{conflicting parameter bounds}}

extern int FUNC(9)(int a, int b, _Array_ptr<int> p : count(a TESTOP b));
extern int FUNC(9)(int a, int b, _Array_ptr<int> p : count(a <= b));  // expected-error {{conflicting parameter bounds}}

extern int FUNC(10)(int a, int b, _Array_ptr<int> p : count(a TESTOP b));
extern int FUNC(10)(int a, int b, _Array_ptr<int> p : count(a > b));  // expected-error {{conflicting parameter bounds}}

extern int FUNC(11)(int a, int b, _Array_ptr<int> p : count(a TESTOP b));
extern int FUNC(11)(int a, int b, _Array_ptr<int> p : count(a >= b));  // expected-error {{conflicting parameter bounds}}

extern int FUNC(12)(int a, int b, _Array_ptr<int> p : count(a TESTOP b));
extern int FUNC(12)(int a, int b, _Array_ptr<int> p : count(a == b));  // expected-error {{conflicting parameter bounds}}

extern int FUNC(13)(int a, int b, _Array_ptr<int> p : count(a TESTOP b));
extern int FUNC(13)(int a, int b, _Array_ptr<int> p : count(a != b));  // expected-error {{conflicting parameter bounds}}

extern int FUNC(13)(int a, int b, _Array_ptr<int> p : count(a TESTOP b));
extern int FUNC(13)(int a, int b, _Array_ptr<int> p : count(a & b));  // expected-error {{conflicting parameter bounds}}

// | vs. prior binary operators
#undef TESTOP
#undef FUNC
#define TESTOP |
#define FUNC(index) NAME(64, index)

extern int FUNC(1)(int a, int b, _Array_ptr<int> p : count(a TESTOP b));
extern int FUNC(1)(int a, int b, _Array_ptr<int> p : count(a * b));  // expected-error {{conflicting parameter bounds}}

extern int FUNC(2)(int a, int b, _Array_ptr<int> p : count(a TESTOP b));
extern int FUNC(2)(int a, int b, _Array_ptr<int> p : count(a / b));  // expected-error {{conflicting parameter bounds}}

extern int FUNC(3)(int a, int b, _Array_ptr<int> p : count(a TESTOP b));
extern int FUNC(3)(int a, int b, _Array_ptr<int> p : count(a % b));  // expected-error {{conflicting parameter bounds}}

extern int FUNC(4)(int a, int b, _Array_ptr<int> p : count(a TESTOP b));
extern int FUNC(4)(int a, int b, _Array_ptr<int> p : count(a + b));  // expected-error {{conflicting parameter bounds}}

extern int FUNC(5)(int a, int b, _Array_ptr<int> p : count(a TESTOP b));
extern int FUNC(5)(int a, int b, _Array_ptr<int> p : count(a - b));  // expected-error {{conflicting parameter bounds}}

extern int FUNC(6)(int a, int b, _Array_ptr<int> p : count(a TESTOP b));
extern int FUNC(6)(int a, int b, _Array_ptr<int> p : count(a << b));  // expected-error {{conflicting parameter bounds}}

extern int FUNC(7)(int a, int b, _Array_ptr<int> p : count(a TESTOP b));
extern int FUNC(7)(int a, int b, _Array_ptr<int> p : count(a >> b));  // expected-error {{conflicting parameter bounds}}

extern int FUNC(8)(int a, int b, _Array_ptr<int> p : count(a TESTOP b));
extern int FUNC(8)(int a, int b, _Array_ptr<int> p : count(a < b));  // expected-error {{conflicting parameter bounds}}

extern int FUNC(9)(int a, int b, _Array_ptr<int> p : count(a TESTOP b));
extern int FUNC(9)(int a, int b, _Array_ptr<int> p : count(a <= b));  // expected-error {{conflicting parameter bounds}}

extern int FUNC(10)(int a, int b, _Array_ptr<int> p : count(a TESTOP b));
extern int FUNC(10)(int a, int b, _Array_ptr<int> p : count(a > b));  // expected-error {{conflicting parameter bounds}}

extern int FUNC(11)(int a, int b, _Array_ptr<int> p : count(a TESTOP b));
extern int FUNC(11)(int a, int b, _Array_ptr<int> p : count(a >= b));  // expected-error {{conflicting parameter bounds}}

extern int FUNC(12)(int a, int b, _Array_ptr<int> p : count(a TESTOP b));
extern int FUNC(12)(int a, int b, _Array_ptr<int> p : count(a == b));  // expected-error {{conflicting parameter bounds}}

extern int FUNC(13)(int a, int b, _Array_ptr<int> p : count(a TESTOP b));
extern int FUNC(13)(int a, int b, _Array_ptr<int> p : count(a != b));  // expected-error {{conflicting parameter bounds}}

extern int FUNC(13)(int a, int b, _Array_ptr<int> p : count(a TESTOP b));
extern int FUNC(13)(int a, int b, _Array_ptr<int> p : count(a & b));  // expected-error {{conflicting parameter bounds}}

extern int FUNC(14)(int a, int b, _Array_ptr<int> p : count(a TESTOP b));
extern int FUNC(14)(int a, int b, _Array_ptr<int> p : count(a ^ b));  // expected-error {{conflicting parameter bounds}}

// && vs. prior binary operators
#undef TESTOP
#undef FUNC
#define TESTOP &&
#define FUNC(index) NAME(65, index)

extern int FUNC(1)(int a, int b, _Array_ptr<int> p : count(a TESTOP b));
extern int FUNC(1)(int a, int b, _Array_ptr<int> p : count(a * b));  // expected-error {{conflicting parameter bounds}}

extern int FUNC(2)(int a, int b, _Array_ptr<int> p : count(a TESTOP b));
extern int FUNC(2)(int a, int b, _Array_ptr<int> p : count(a / b));  // expected-error {{conflicting parameter bounds}}

extern int FUNC(3)(int a, int b, _Array_ptr<int> p : count(a TESTOP b));
extern int FUNC(3)(int a, int b, _Array_ptr<int> p : count(a % b));  // expected-error {{conflicting parameter bounds}}

extern int FUNC(4)(int a, int b, _Array_ptr<int> p : count(a TESTOP b));
extern int FUNC(4)(int a, int b, _Array_ptr<int> p : count(a + b));  // expected-error {{conflicting parameter bounds}}

extern int FUNC(5)(int a, int b, _Array_ptr<int> p : count(a TESTOP b));
extern int FUNC(5)(int a, int b, _Array_ptr<int> p : count(a - b));  // expected-error {{conflicting parameter bounds}}

extern int FUNC(6)(int a, int b, _Array_ptr<int> p : count(a TESTOP b));
extern int FUNC(6)(int a, int b, _Array_ptr<int> p : count(a << b));  // expected-error {{conflicting parameter bounds}}

extern int FUNC(7)(int a, int b, _Array_ptr<int> p : count(a TESTOP b));
extern int FUNC(7)(int a, int b, _Array_ptr<int> p : count(a >> b));  // expected-error {{conflicting parameter bounds}}

extern int FUNC(8)(int a, int b, _Array_ptr<int> p : count(a TESTOP b));
extern int FUNC(8)(int a, int b, _Array_ptr<int> p : count(a < b));  // expected-error {{conflicting parameter bounds}}

extern int FUNC(9)(int a, int b, _Array_ptr<int> p : count(a TESTOP b));
extern int FUNC(9)(int a, int b, _Array_ptr<int> p : count(a <= b));  // expected-error {{conflicting parameter bounds}}

extern int FUNC(10)(int a, int b, _Array_ptr<int> p : count(a TESTOP b));
extern int FUNC(10)(int a, int b, _Array_ptr<int> p : count(a > b));  // expected-error {{conflicting parameter bounds}}

extern int FUNC(11)(int a, int b, _Array_ptr<int> p : count(a TESTOP b));
extern int FUNC(11)(int a, int b, _Array_ptr<int> p : count(a >= b));  // expected-error {{conflicting parameter bounds}}

extern int FUNC(12)(int a, int b, _Array_ptr<int> p : count(a TESTOP b));
extern int FUNC(12)(int a, int b, _Array_ptr<int> p : count(a == b));  // expected-error {{conflicting parameter bounds}}

extern int FUNC(13)(int a, int b, _Array_ptr<int> p : count(a TESTOP b));
extern int FUNC(13)(int a, int b, _Array_ptr<int> p : count(a != b));  // expected-error {{conflicting parameter bounds}}

extern int FUNC(13)(int a, int b, _Array_ptr<int> p : count(a TESTOP b));
extern int FUNC(13)(int a, int b, _Array_ptr<int> p : count(a & b));  // expected-error {{conflicting parameter bounds}}

extern int FUNC(14)(int a, int b, _Array_ptr<int> p : count(a TESTOP b));
extern int FUNC(14)(int a, int b, _Array_ptr<int> p : count(a ^ b));  // expected-error {{conflicting parameter bounds}}

extern int FUNC(15)(int a, int b, _Array_ptr<int> p : count(a TESTOP b));
extern int FUNC(15)(int a, int b, _Array_ptr<int> p : count(a | b));  // expected-error {{conflicting parameter bounds}}

// || vs. prior binary operators
#undef TESTOP
#undef FUNC
#define TESTOP ||
#define FUNC(index) NAME(66, index)
extern int FUNC(1)(int a, int b, _Array_ptr<int> p : count(a TESTOP b));
extern int FUNC(1)(int a, int b, _Array_ptr<int> p : count(a * b));  // expected-error {{conflicting parameter bounds}}

extern int FUNC(2)(int a, int b, _Array_ptr<int> p : count(a TESTOP b));
extern int FUNC(2)(int a, int b, _Array_ptr<int> p : count(a / b));  // expected-error {{conflicting parameter bounds}}

extern int FUNC(3)(int a, int b, _Array_ptr<int> p : count(a TESTOP b));
extern int FUNC(3)(int a, int b, _Array_ptr<int> p : count(a % b));  // expected-error {{conflicting parameter bounds}}

extern int FUNC(4)(int a, int b, _Array_ptr<int> p : count(a TESTOP b));
extern int FUNC(4)(int a, int b, _Array_ptr<int> p : count(a + b));  // expected-error {{conflicting parameter bounds}}

extern int FUNC(5)(int a, int b, _Array_ptr<int> p : count(a TESTOP b));
extern int FUNC(5)(int a, int b, _Array_ptr<int> p : count(a - b));  // expected-error {{conflicting parameter bounds}}

extern int FUNC(6)(int a, int b, _Array_ptr<int> p : count(a TESTOP b));
extern int FUNC(6)(int a, int b, _Array_ptr<int> p : count(a << b));  // expected-error {{conflicting parameter bounds}}

extern int FUNC(7)(int a, int b, _Array_ptr<int> p : count(a TESTOP b));
extern int FUNC(7)(int a, int b, _Array_ptr<int> p : count(a >> b));  // expected-error {{conflicting parameter bounds}}

extern int FUNC(8)(int a, int b, _Array_ptr<int> p : count(a TESTOP b));
extern int FUNC(8)(int a, int b, _Array_ptr<int> p : count(a < b));  // expected-error {{conflicting parameter bounds}}

extern int FUNC(9)(int a, int b, _Array_ptr<int> p : count(a TESTOP b));
extern int FUNC(9)(int a, int b, _Array_ptr<int> p : count(a <= b));  // expected-error {{conflicting parameter bounds}}

extern int FUNC(10)(int a, int b, _Array_ptr<int> p : count(a TESTOP b));
extern int FUNC(10)(int a, int b, _Array_ptr<int> p : count(a > b));  // expected-error {{conflicting parameter bounds}}

extern int FUNC(11)(int a, int b, _Array_ptr<int> p : count(a TESTOP b));
extern int FUNC(11)(int a, int b, _Array_ptr<int> p : count(a >= b));  // expected-error {{conflicting parameter bounds}}

extern int FUNC(12)(int a, int b, _Array_ptr<int> p : count(a TESTOP b));
extern int FUNC(12)(int a, int b, _Array_ptr<int> p : count(a == b));  // expected-error {{conflicting parameter bounds}}

extern int FUNC(13)(int a, int b, _Array_ptr<int> p : count(a TESTOP b));
extern int FUNC(13)(int a, int b, _Array_ptr<int> p : count(a != b));  // expected-error {{conflicting parameter bounds}}

extern int FUNC(13)(int a, int b, _Array_ptr<int> p : count(a TESTOP b));
extern int FUNC(13)(int a, int b, _Array_ptr<int> p : count(a & b));  // expected-error {{conflicting parameter bounds}}

extern int FUNC(14)(int a, int b, _Array_ptr<int> p : count(a TESTOP b));
extern int FUNC(14)(int a, int b, _Array_ptr<int> p : count(a ^ b));  // expected-error {{conflicting parameter bounds}}

extern int FUNC(15)(int a, int b, _Array_ptr<int> p : count(a TESTOP b));
extern int FUNC(15)(int a, int b, _Array_ptr<int> p : count(a | b));  // expected-error {{conflicting parameter bounds}}

extern int FUNC(15)(int a, int b, _Array_ptr<int> p : count(a TESTOP b));
extern int FUNC(15)(int a, int b, _Array_ptr<int> p : count(a && b));  // expected-error {{conflicting parameter bounds}}

// Additive pointer operators.
extern int f67_1(int a, int b, _Array_ptr<int> p : bounds(p, p + a));
extern int f67_1(int a, int b, _Array_ptr<int> p : bounds(p, p - a));  // expected-error {{conflicting parameter bounds}}

// Binary operators vs. prior kinds of expressions.

extern int f70_1(int a, int b, _Array_ptr<int> p : count(a + b)); 
extern int f70_1(int a, int b, _Array_ptr<int> p : count(5));     // expected-error {{conflicting parameter bounds}}   

extern int f70_2(int a, int b, _Array_ptr<int> p : count(a + b));
extern int f70_2(int a, int b, _Array_ptr<int> p : count(a));     // expected-error {{conflicting parameter bounds}}    

extern int f70_3(int a, int b, _Array_ptr<int> p : count(a + b));
extern int f70_3(int a, int b, _Array_ptr<int> p : count(!a));    // expected-error {{conflicting parameter bounds}}    

//-----------------------------------------------------//
// C-style cast operator expressions.                  //
//-----------------------------------------------------//

typedef _Array_ptr<int> apint;
typedef _Array_ptr<char> apchar;

// Equality of C-style cast operators
extern int f80_1(int a, _Array_ptr<int> b, _Array_ptr<char> p : bounds(b, (_Array_ptr<int>) b + a));
extern int f80_1(int a, _Array_ptr<int> b, _Array_ptr<char> p : bounds(b, (_Array_ptr<int>) b + a));

extern int f80_2(int a, _Array_ptr<int> b, _Array_ptr<char> p : bounds((apint) b, (apint) b + a));
extern int f80_2(int a, _Array_ptr<int> b, _Array_ptr<char> p : bounds((apint) b, (apint) b + a));
extern int f80_2(int a, _Array_ptr<int> b, _Array_ptr<char> p : bounds((_Array_ptr<int>) b, (_Array_ptr<int>) b +  a));

// Inequality of C-style cast operators.
// We embed the expression being varied inside another
// cast expression so that the function declaration type checks.

extern int f80_3(int a, _Array_ptr<int> b,
                 _Array_ptr<char> p : bounds(b, (_Array_ptr<int>) ((_Array_ptr<int>) b + a)));
extern int f80_3(int a, _Array_ptr<int> b,
                 _Array_ptr<char> p : bounds(b, (_Array_ptr<int>) ((_Array_ptr<char>) b + a))); // expected-error {{conflicting parameter bounds}}

extern int f80_4(int a, _Array_ptr<int> b,
                 _Array_ptr<char> p : bounds(b, (_Array_ptr<int>) ((apint) b + a)));
extern int f80_4(int a, _Array_ptr<int> b,
                 _Array_ptr<char> p : bounds(b, (_Array_ptr<int>) ((_Array_ptr<char>) b + a))); // expected-error {{conflicting parameter bounds}}

extern int f80_5(int a, _Array_ptr<int> b,
                 _Array_ptr<char> p : bounds(b, (_Array_ptr<int>) ((_Array_ptr<int>)b + a)));
extern int f80_5(int a, _Array_ptr<int> b,
                 _Array_ptr<char> p : bounds(b, (_Array_ptr<int>) ((apchar) b + a))); // expected-error {{conflicting parameter bounds}}

extern int f80_5(int a, _Array_ptr<int> b,
                 _Array_ptr<char> p : bounds(b, (_Array_ptr<int>) ((apint) b + a)));
extern int f80_5(int a, _Array_ptr<int> b,
                 _Array_ptr<char> p : bounds(b, (_Array_ptr<int>) ((apchar) b + a))); // expected-error {{conflicting parameter bounds}}

// Inequality of C-style cast operators vs. other expressions

// Constants
extern int f81_1(int a, _Array_ptr<int> b : count((short)a)); 
extern int f81_1(int a, _Array_ptr<int> b : count(5));             // expected-error {{conflicting parameter bounds}}

// Variables
extern int f81_2(int a, _Array_ptr<int> b : count((short)a));
extern int f81_2(int a, _Array_ptr<int> b : count(a));             // expected-error {{conflicting parameter bounds}}

extern int f81_3(int a, _Array_ptr<int> b : count((short)a));
extern int f81_3(int a, _Array_ptr<int> b : count(!a));            // expected-error {{conflicting parameter bounds}}

// Binary operators
extern int f81_4(int a, _Array_ptr<int> b : count((short)a));
extern int f81_4(int a, _Array_ptr<int> b : count(a + 2));         // expected-error {{conflicting parameter bounds}}

// Check bounds expressions involving member accesses

struct S {
   int f1;
   int f2;
};

// Equality of member accesses
extern int f90_1(struct S a, _Array_ptr<int> b : count(a.f1));
extern int f90_1(struct S a, _Array_ptr<int> b : count(a.f1));

extern int f90_2(struct S a, _Array_ptr<int> b : count(c.f1), struct S c);
extern int f90_2(struct S a, _Array_ptr<int> b : count(c.f1), struct S c);

extern int f90_3(struct S a, _Array_ptr<int> b : count(a.f2), struct S c);
extern int f90_3(struct S a, _Array_ptr<int> b : count(a.f2), struct S c);

extern int f91_1(struct S *a, _Array_ptr<int> b : count(a->f1));
extern int f91_1(struct S *a, _Array_ptr<int> b : count(a->f1));

extern int f91_2(struct S *a, _Array_ptr<int> b : count(c->f1), struct S *c);
extern int f91_2(struct S *a, _Array_ptr<int> b : count(c->f1), struct S *c);

extern int f91_3(struct S *a, _Array_ptr<int> b : count(a->f2), struct S *c);
extern int f91_3(struct S *a, _Array_ptr<int> b : count(a->f2), struct S *c);

// Inequality of member accesses
extern int f92_1(struct S a, _Array_ptr<int> b : count(a.f1));
extern int f92_1(struct S a, _Array_ptr<int> b : count(a.f2));  // expected-error {{conflicting parameter bounds}}

extern int f92_2(struct S a, _Array_ptr<int> b : count(a.f1), struct S c); 
extern int f92_2(struct S a, _Array_ptr<int> b : count(c.f1), struct S c);  // expected-error {{conflicting parameter bounds}}

extern int f92_3(struct S a, _Array_ptr<int> b : count(a.f1), struct S c);
extern int f92_3(struct S a, _Array_ptr<int> b : count(c.f2), struct S c);  // expected-error {{conflicting parameter bounds}}

extern int f93_1(struct S *a, _Array_ptr<int> b : count(a->f1));
extern int f93_1(struct S *a, _Array_ptr<int> b : count(a->f2));  // expected-error {{conflicting parameter bounds}}

extern int f93_2(struct S *a, _Array_ptr<int> b : count(a->f1), struct S *c);
extern int f93_2(struct S *a, _Array_ptr<int> b : count(c->f1), struct S *c);  // expected-error {{conflicting parameter bounds}}

extern int f93_3(struct S *a, _Array_ptr<int> b : count(a->f1), struct S *c);
extern int f93_3(struct S *a, _Array_ptr<int> b : count(c->f2), struct S *c);  // expected-error {{conflicting parameter bounds}}

//
// Check equality of canonicalized expressions
//

// Casts that preserve values are ignored.

// Identity casts to/from the same type.
extern int f200_1(int a, _Array_ptr<int> b,
                  _Array_ptr<char> p : bounds(b, b + a));
extern int f200_1(int a, _Array_ptr<int> b,
                  _Array_ptr<char> p : bounds(b, (_Array_ptr<int>) b + a));

extern int f200_2(int a, _Array_ptr<int> b,
                  _Array_ptr<char> p : bounds(b, b + a));
extern int f200_2(int a, _Array_ptr<int> b,
                  _Array_ptr<char> p : bounds(b, b + (int) a));

// Casts between pointer types.
extern int f201_1(int a, _Array_ptr<int> b,
                  _Array_ptr<char> p : bounds(b, b + a));
extern int f201_1(int a, _Array_ptr<int> b,
                  _Array_ptr<char> p : bounds(b, (_Array_ptr<int>) (int *) b + a));

extern int f201_2(int a, _Array_ptr<int> b,
                  _Array_ptr<char> p : bounds(b, (_Array_ptr<int>) (void *) b + a));
extern int f201_2(int a, _Array_ptr<int> b,
                  _Array_ptr<char> p : bounds(b, (_Array_ptr<int>) (int *) b + a));

// Casts between signed/unsigned integers that have the same bit size.
//
// Note: it is an implementation-specific assumption that casts between signed
// and unsigned integers with the same bit size preserve values.  It's correct
// in the following cases:
// - signed-to-unsigned conversions, and integers are represented in
//   two's complement (based on the C specification).
// - unsigned-to-signed, and integers are represented in two's complement,
//   and the implementation chooses to preserve the bits for unsigned integers
//   too large to fit into the signed integer.
// C does not require that integers be two's complement.  It allows 1's complement
// and sign/magnitude representations.  We don't have an implementation of Checked C
// that uses these representations. If we did we'd likely have to make these
// tests implementation specific.
extern int f202_1(int a, _Array_ptr<int> b,
                  _Array_ptr<char> p : bounds(b, b + a));
extern int f202_1(int a, _Array_ptr<int> b,
                  _Array_ptr<char> p : bounds(b, b + (int) (unsigned) a));

extern int f202_2(unsigned int a,
                   _Array_ptr<int> b, _Array_ptr<char> p : bounds(b, b + a));
extern int f202_2(unsigned int a, _Array_ptr<int> b,
                  _Array_ptr<char> p : bounds(b, b + (unsigned) (int) a));

// C-style casts and implicit casts to/from the same types.
extern int f203_1(short int a, _Array_ptr<int> b : count(a + 1));
extern int f203_1(short int a, _Array_ptr<int> b : count((int) a + 1));

extern int f203_2(short int a, _Array_ptr<int> b, _Array_ptr<char> p : bounds(b, b + a));
extern int f203_2(short int a, _Array_ptr<int> b, _Array_ptr<char> p : bounds(b, b + (int) a));

//
// & and * operations that cancel are ignored.
//
// & of an lvalue
extern int f210_1(int a, _Array_ptr<int> b : count(*&a));
extern int f210_1(int a, _Array_ptr<int> b : count(a));

extern _Checked int f210_2(_Ptr<int> lb, _Ptr<int> ub,
                           _Array_ptr<int> b : bounds(lb, ub));
extern _Checked int f210_2(_Ptr<int> lb, _Ptr<int> ub,
                           _Array_ptr<int> b : bounds(&*lb, ub));
// & of an array.
int arr _Checked[10];
extern int f211_1(_Array_ptr<int> b : bounds(arr, arr + 5));
extern int f211_1(_Array_ptr<int> b : bounds(*&arr, arr + 5));
extern int f211_1(_Array_ptr<int> b : bounds(*&(arr), arr + 5));
extern int f211_1(_Array_ptr<int> b : bounds(*(&arr), arr + 5));

// address-of an array does nothing at runtime  However, we have to make sure
// the types are compatible.
extern int f211_2(_Array_ptr<int> b : bounds(&arr, &arr));
extern int f211_2(_Array_ptr<int> b : bounds((int (*) _Checked[10]) arr, (int (*) _Checked[10]) arr));
extern int f211_2(_Array_ptr<int> b : bounds(&arr, (int (*) _Checked[10]) arr));

//-----------------------------------------------------//
// Checked C bounds cast expressions.                  //
//-----------------------------------------------------// 

// _Dynamic_bounds_cast and _Assume_bounds_cast should not be treated as value-preserving casts for unevaluated expressions.

_Array_ptr<char> ga;

extern void f212_1(_Array_ptr<char> a : bounds(_Dynamic_bounds_cast<_Array_ptr<int>>(a, count(1)), _Dynamic_bounds_cast<_Array_ptr<int>>(a, count(2)) + 3));
extern void f212_1(_Array_ptr<char> a : bounds(_Dynamic_bounds_cast<_Array_ptr<int>>(a, count(1)), _Dynamic_bounds_cast<_Array_ptr<int>>(a, count(2)) + 3));

extern void f212_2(_Array_ptr<char> a : bounds(_Assume_bounds_cast<_Array_ptr<int>>(a, count(1)), _Assume_bounds_cast<_Array_ptr<int>>(ga, count(2)) + 3));
extern void f212_2(_Array_ptr<char> a : bounds(_Assume_bounds_cast<_Array_ptr<int>>(a, count(1)), _Assume_bounds_cast<_Array_ptr<int>>(ga, count(2)) + 3));

extern void f212_3(_Array_ptr<char> a : bounds(_Assume_bounds_cast<_Array_ptr<int>>(a, count(1)), _Assume_bounds_cast<_Array_ptr<int>>(a, count(2)) + 3));
extern void f212_3(_Array_ptr<char> a : bounds(_Dynamic_bounds_cast<_Array_ptr<int>>(a, count(1)), _Assume_bounds_cast<_Array_ptr<int>>(a, count(2)) + 3)); // expected-error {{conflicting parameter bounds}}

extern void f212_4(_Array_ptr<char> a : bounds(_Dynamic_bounds_cast<_Array_ptr<int>>(a, count(1)), _Assume_bounds_cast<_Array_ptr<int>>(a, count(2)) + 3));
extern void f212_4(_Array_ptr<char> a : bounds(_Dynamic_bounds_cast<_Array_ptr<int>>(a, count(1)), _Dynamic_bounds_cast<_Array_ptr<int>>(a, count(2)) + 3)); // expected-error {{conflicting parameter bounds}}

extern void f212_5(_Array_ptr<char> a : bounds(_Dynamic_bounds_cast<_Array_ptr<int>>(a, count(1)), _Assume_bounds_cast<_Array_ptr<int>>(a, count(2)) + 3));
extern void f212_5(_Array_ptr<char> a : bounds(_Dynamic_bounds_cast<_Array_ptr<int>>(a, count(5)), _Assume_bounds_cast<_Array_ptr<int>>(a, count(2)) + 3)); // expected-error {{conflicting parameter bounds}}

extern void f212_6(_Array_ptr<char> a : bounds(_Dynamic_bounds_cast<_Array_ptr<int>>(a, count(1)), _Assume_bounds_cast<_Array_ptr<int>>(a, count(2)) + 3));
extern void f212_6(_Array_ptr<char> a : bounds(_Dynamic_bounds_cast<_Array_ptr<int>>(a, count(1)), _Assume_bounds_cast<_Array_ptr<int>>(a, bounds(a, a + 2)) + 3)); // expected-error {{conflicting parameter bounds}}

extern void f212_7(_Array_ptr<char> a : bounds(_Dynamic_bounds_cast<_Array_ptr<int>>(b, count(1)), b + 3), _Array_ptr<int> b : count(1));
extern void f212_7(_Array_ptr<char> a : bounds(b, b + 3), _Array_ptr<int> b : count(1)); // expected-error {{conflicting parameter bounds}}

extern void f212_8(_Array_ptr<char> a : bounds(b, _Assume_bounds_cast<_Array_ptr<int>>(b, count(1)) + 3), _Array_ptr<int> b : count(1));
extern void f212_8(_Array_ptr<char> a : bounds(b, b + 3), _Array_ptr<int> b : count(1)); // expected-error {{conflicting parameter bounds}}
