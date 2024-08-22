// Feature tests of static checking for variables with initializers
//
// The following lines are for the LLVM test harness:
//
// RUN: %clang_cc1 -verify -verify-ignore-unexpected=note %s

#include <stdchecked.h>

// Automatic variables with Checked C declarations or _Ptr types must
// always have initializers.
extern void f1() {
  array_ptr<int> v1;
  array_ptr<int> v2 = 0;
  array_ptr<int> v3 : bounds(unknown);
  array_ptr<int> v4 : bounds(unknown) = 0;
  array_ptr<int> v5 : count(5) = 0;
  array_ptr<int> v6 : count(5);                       // expected-error {{automatic variable 'v6' with bounds must have initializer}}
  array_ptr<int> v7 : byte_count(5 * sizeof(int)) = 0;
  array_ptr<int> v8 : byte_count(5 * sizeof(int));    // expected-error {{automatic variable 'v8' with bounds must have initializer}}
  array_ptr<int> v9 : bounds(v9, v9 + 5) = 0;
  int* _Array v10  _Bounds(v10, v10 + 5);          // expected-error {{automatic variable 'v10' with bounds must have initializer}}

  ptr<int> v20 = 0;
  ptr<int> v21;         // expected-error {{automatic variable 'v21' with _Ptr type must have initializer}}
}

//
// Checked arrays with initializers
//

char g1 checked[] = "abc";
int g2 checked[] = { 0, 1, 2 };
int g3 checked[][2] = { { 0, 1}, {2, 3} };

char g4 checked[4] = "abc";
int g5 checked[3] = { 0, 1, 2 };
int g6 checked[2][2] = { { 0, 1 },{ 2, 3 } };

char g20 nt_checked[] = "abc";
int g21 nt_checked[] = { 0, 1, 2, 0 };
char g22 checked[3]nt_checked[3] = { { 0, 1, 0}, { 1, 1, 0}, { 3, 1, 0 } };
char g23 checked[3]nt_checked[4] = { "abc", "def", "fgh" };

//
// Checked pointers with initialized array literals
//

nt_array_ptr<int> g30 : count(4) = (int[]) { 0, [2] = 2, 3, 5, 0 };
nt_array_ptr<int> g31 : count(0) = (int checked[]) { 0, [2] = 2, 3, 5, 0 };
nt_array_ptr<char> g32 : count(5) = "abcde";
array_ptr<char> g33 : count(5) = "abcde";
array_ptr<char> g34 : count(5) = (char[5]) { 'a', 'b', 'c', 'd', 'e' };
char* _Array g35  _Count(5) = (char checked[5]) { 'a', 'b', 'c', 'd', 'e' };

//
// Checked arrays of checked pointers
//
nt_array_ptr<char> g36 checked[3][2] = { [1] = "ab", "cd", "ef", "jk" };
nt_array_ptr<char> g37 nt_checked[] = { "the", "brown", "fox", "jumped",
                                        "over", "the", "fence", 0 };

void callback1(int a);
void callback2(int a);
void(*_Single *_Nt_array callback_table)(int) = (ptr<void(int)>[]) { callback1, callback2, 0 };

void f3(char *escape) {
}

// Checked that the arrays and pointers have checked type.
void f4(void) checked {
  char t1 = g1[0];
  int t2 = g2[0];
  int t3 = g3[0][1];
  char t4 = g4[0];
  char t5 = g5[0];
  int t6 = g6[1][1];
  int t20 = g20[0];
  int t21 = g21[3];
  int t22 = g22[2][2];
  // TODO: bounds declaration checking needs to understand equality after assignment
  nt_array_ptr<char> t23 = g23[0]; // expected-warning {{cannot prove declared bounds for 't23' are valid after initialization}}
  int t30 = g30[1];
  int t31 = *g31;
  char t32 = g32[5];
  char t33 = g33[4];
  char t34 = g34[4];
  char t35 = g35[1];
  nt_array_ptr<char> t36 = g36[1][0];
  nt_array_ptr<char> t37 = g37[1];


  f3("abc");   // expected-error {{passing 'char _Nt_checked[4]' to parameter of incompatible type 'char *'}}
  unchecked{
    f3("abc");
  }

  nt_array_ptr<int> t100 : count(3) = (int[]) { 0, 1, 2, 3 };
}

void f5(void) checked {
  char t1 checked[] = "abc";
  int t2 checked[] = { 0, 1, 2 };
  int t3 checked[][2] = { { 0, 1 },{ 2, 3 } };

  char t4 checked[4] = "abc";
  int t5 checked[3] = { 0, 1, 2 };
  int t6 checked[2][2] = { { 0, 1 },{ 2, 3 } };

  char t20 nt_checked[] = "abc";
  int t21 nt_checked[] = { 0, 1, 2, 0 };
  char t22 checked[3]nt_checked[3] = { { 0, 1, 0 },{ 1, 1, 0 },{ 3, 1, 0 } };
  char t23 checked[3]nt_checked[4] = { "abc", "def", "fgh" };
  char t24 checked[3]nt_checked[4] = { ("abc"), "def", ("fgh") };

  //
  // Checked pointers with initialized array literals.
  //

  nt_array_ptr<int> t30 : count(4) = (int[]) { 0, [2] = 2, 3, 5, 0 };

  nt_array_ptr<int> t31 = (int checked[]) { 0, [2] = 2, 3, 5, 0 };
  nt_array_ptr<char> t32 : count(5) = "abcde";

  array_ptr<char> t33 : count(5) = "abcde";
  char* _Array t34 = (char[5]) { 'a', 'b', 'c', 'd', 'e' };
  char* _Array t35  _Count(5) = (char checked[5]) { 'a', 'b', 'c', 'd', 'e' };

  //
  // Make sure parentheses are ignored.
  //
  nt_array_ptr<int> t36 : count(4) = ((int[]) { 0, [2] = 2, 3, 5, 0 });
  nt_array_ptr<char> t37 : count(5) = ("abcde");

  //
  // Checked arrays of checked pointers.
  //
  nt_array_ptr<char> t38 checked[3][2] = { [1] = "ab", "cd", "ef", "jk" };
  char* _Nt_array t39 nt_checked[] = { "the", "brown", "fox", "jumped",
                                      "over", "the", "fence", 0 };

  char c = ((char *[2]) { "abc", "def" })[0][0];  // expected-error {{type in a checked\
 scope must use only checked types or parameter/return types with bounds-safe interfaces}}
  nt_array_ptr<ptr<void(int)>> callback_table = (ptr<void(int)>[]) { callback1, callback2, 0 };

}

void f6(void) {
  ptr<int> p = 0;                   // initializer required.
  array_ptr<int> q : count(5) = 0;  // has bounds; initializer required.
  int* _Array lower,* _Array upper;      // no bounds; initializer not required.
  lower = q;
  upper = q + 5;

  struct VariableBuffer {
    array_ptr<int> buf : count(len);
    int len;
  };

  struct Range {
    array_ptr<int> lower;
    array_ptr<int> upper;
  };

  struct VariableBuffer buf = { 0 };  // initializer for struct required.
  struct Range pair;                  // no bounds on members; initializer not required.
  ptr<int> data checked[10] = { 0 };  // initializer for array required.
  struct VariableBuffer stack checked[10] = { 0 }; // initializer for array required.

  struct VariableBuffer buf_missing_init;  // expected-error {{containing a checked pointer must have an initializer}}
  ptr<int> data_missing_init checked[10];  // expected-error {{elements containing checked pointers must have an initializer}}

 // Check { 0 } initialization idiom where first member is a floating point number.
  struct FloatWithVariableBuffer {
    float weight;
    int* _Array buf  _Count(len);
    int len;
  };

  struct FloatWithVariableBuffer w = { 0 };

  // test cases for checking the array/struct/union variables initializers
  // array with/without initializers
  ptr<int> data_with_init checked[10] = { 0 };  	  // initialized, pass.

  ptr<char> data_no_init checked[20];             // expected-error {{elements containing checked pointers must have an initializer}}

  struct has_uninitialized_ptr_member {
    int x;
    ptr<int> uninitialized_ptr_member;
    float y;
  };
  struct has_uninitialized_ptr_member uninit_S;   // expected-error {{containing a checked pointer must have an initializer}}

  struct checked_value_no_bounds {
    int x;
    int* _Array lower;
    int* _Array upper;
    float y;
  };

  struct checked_value_no_bounds init_S2; // no bounds; initializer not required, should pass
  struct checked_value_no_bounds uninit_S2; // no bounds; initializer not required, should pass

  ptr<struct checked_value_no_bounds> arr_init   checked[20] = { 0 }; // initialized arry, should pass
  ptr<struct checked_value_no_bounds> arr_uninit checked[20]; // expected-error {{elements containing checked pointers must have an initializer}}

  struct checked_value_has_bounds {
    int x;
    array_ptr<char> lower  _Count(5);
    array_ptr<char> upper  _Count(5);
    float y;
  };

  struct checked_value_has_bounds init_S3 = { 0 }; // has bounds; initializer required and we did, should pass
  struct checked_value_has_bounds uninit_S3; // expected-error {{containing a checked pointer must have an initializer}}

  struct struct_with_checked_field_has_bounds {
    int x;
    struct checked_value_has_bounds s;
  };
  struct struct_with_checked_field_has_bounds init_nested_S = { 0 }; // has bounds, initializer required and we did, should pass
  struct struct_with_checked_field_has_bounds uninit_nested_S; // expected-error {{containing a checked pointer must have an initializer}}

  union u_checked_value_no_bounds {
    int x;
    array_ptr<int> lower;
    array_ptr<int> upper;
    float y;
  };
  union u_checked_value_no_bounds init_U; // no bounds; initializer not required, should pass
  union u_checked_value_no_bounds uninit_U; // no bounds; initializer not required, should pass

  union u_checked_value_has_bounds {
    int x;
    char* _Array lower : count(5);
    char* _Array upper : count(5);
    float y;
  };
  union u_checked_value_has_bounds init_U2 = { 0 }; // has bounds; initializer required and we did, should pass
  union u_checked_value_has_bounds uninit_U2; // expected-error {{containing a checked pointer must have an initializer}}


  struct struct_with_checked_union_field_has_bounds {
    int x;
    union u_checked_value_has_bounds u;
  };
  struct struct_with_checked_union_field_has_bounds init_SU = { 0 }; // has bounds, initializer required and we did, should pass
  struct struct_with_checked_union_field_has_bounds uninit_SU; // expected-error {{containing a checked pointer must have an initializer}}


  typedef struct {
    int data;
    array_ptr<char> name _Count(20);
    struct Node* next;
  } Node;

  Node n_err; // expected-error {{containing a checked pointer must have an initializer}}
  Node n = { 0 };


  typedef struct {
    struct Range r;
    Node center;
  } Circle;

  Circle C_err; // expected-error {{containing a checked pointer must have an initializer}}
  Circle C = { 0 };

  typedef struct {
    Circle Outer;
    Circle Inner;
  } Annulus;

  Annulus a_err; // expected-error {{containing a checked pointer must have an initializer}}
  Annulus a = { 0 };

  // array of struct with array_ptr member with bounds
  Annulus anls_arr checked[100]; // expected-error {{elements containing checked pointers must have an initializer}}
  Annulus anls checked[100] = {0};

  // nested structs
typedef struct NA {
  int data;
  struct NA *next;
} NA;

}

void f7() {
  int a;
  float b;
  // integer with a bounds expression must have an initializer
  int x : count(5); // expected-error {{with a bounds expression must have an initializer}} expected-error {{have a pointer or array type}}
  int y : count(6) = 0; // expected-error {{have a pointer or array type}}
}

// integer with bounds expression needs to be checked
void f8 (void) {
  // for bounds expr kind like "bounds((array_ptr) i, (array_ptr) i + 10)"
  typedef struct {
    int a;
    unsigned long long b : bounds( (array_ptr<int>) b, (array_ptr<int>) b + 10);
  } S0;
  S0 s0; // expected-error {{containing an integer member with a bounds expression must have an initializer}}

  typedef struct {
    int a;
    S0 s;
  } SS0;
  SS0 ss0; // expected-error {{containing an integer member with a bounds expression must have an initializer}}

  // for bounds expr kind like "int i : count(len)"
  typedef struct {
    int a;
    int b : count(10);  // expected-error {{have a pointer or array type}}
  } S;
  S s1; // expected-error {{containing an integer member with a bounds expression must have an initializer}}

  typedef struct {
    int aa;
    float ff;
    S s;
  } SS;
  SS ss; // expected-error {{containing an integer member with a bounds expression must have an initializer}}

  typedef struct {
    int aaa;
    float fff;
    SS ss;
  } SSS;
  SSS sss; // expected-error {{containing an integer member with a bounds expression must have an initializer}}
}

// An unchecked pointer with a bounds expression in a checked scope must have an initializer
// TODO: remove the restriction to allow local variables with unchecked pointer type to be declared
void f9 (void) checked {
  int a;
  float b;
  int* p : count(1) = &a; // expected-error {{bounds declaration not allowed for local variable with unchecked pointer type}}
  char* s : count(10); // expected-error {{bounds declaration not allowed for local variable with unchecked pointer type}}
}

// test unchecked pointer with bounds expression, in a checked scope
void f10 (void) checked {
  char* p : count(5); // expected-error {{bounds declaration not allowed for local variable with unchecked pointer type}}
}

// For unchecked_pointer_with_bounds_expr_in_checked_scope, we need to consider struct also
void f11 (void) checked {
  int a;
  int* p : count(5); // expected-error {{bounds declaration not allowed for local variable with unchecked pointer type}}
  void* src : count(10);  // expected-error {{bounds declaration not allowed for local variable with unchecked pointer type}}

   //a struct with unchecked pointers with bounds exprs in a checked scope
  typedef struct {
    int x;
    int* p  _Count(1);
    char* cp : count(5);
  } S;
  S s1; // expected-error {{containing an unchecked pointer with a bounds expression in a checked scope must have an initializer}}

  typedef struct {
    int x;
    S s; // contains an unchecked pointer with bounds expr
  } SS;
  SS ss; // expected-error {{containing an unchecked pointer with a bounds expression in a checked scope must have an initializer}}

  typedef struct {
    int x;
    SS ss;
  } SSS;
  SSS sss; // expected-error {{containing an unchecked pointer with a bounds expression in a checked scope must have an initializer}}
}

// Test if _Nt_checked array initializers are null terminated
void f12 (void) {
  struct EmployeeNTChecker {
    int age;
    char name nt_checked[10];
  };

  struct EmployeeManagerNTChecker {
    struct EmployeeNTChecker emp;
    struct EmployeeNTChecker manager;
  };

  struct NumberListNTChecker {
    int integer_list nt_checked[5];
  };

  typedef struct {
      int integer_list nt_checked[5];
  } ListChecker;

  char string_literal_initializer nt_checked[] = "abcde";
  char string_literal_initializer_with_braces nt_checked[] = {"abcde"};
  char string_literal_initializer_with_braces_empty nt_checked[] = {}; // expected-error {{null terminator expected in _Nt_checked array initializer}}
  char string_literal_initializer_too_long nt_checked[3] = "abcde"; // expected-warning {{initializer-string for char array is too long}} expected-error {{initializer-string for _Nt_checked char array is too long}}
  char string_literal_initializer_too_long2 nt_checked[1] = "a"; // expected-error {{initializer-string for _Nt_checked char array is too long}}
  char string_literal_initializer_by_chars nt_checked[6] = {'a', 'b', 'c', 'd', 'e', '\0'};
  char string_literal_initializer_by_chars2 nt_checked[5] = {'a', '\0'};
  char string_literal_initializer_by_chars_no_null_term nt_checked[6] = {'a', 'b', 'c', 'd', 'e', 'f'}; // expected-error {{null terminator expected in _Nt_checked array initializer}}
  struct EmployeeNTChecker struct_field_nt_check = {32, {"John"}};
  struct EmployeeManagerNTChecker team =   {{32, {"John"}}, {32, {"Matt"}}};
  struct EmployeeManagerNTChecker team2 =   {{32, {"John"}}, {32, {"Joseph Matthew Nash"}}}; // expected-warning {{initializer-string for char array is too long}} expected-error {{initializer-string for _Nt_checked char array is too long}}
  struct NumberListNTChecker list1 = {{1,2,3,0}};
  ListChecker list2 = {{1,2,3,4,0}};
  ListChecker list3 = {{1,2,3,4}}; // expected-error {{null terminator expected in _Nt_checked array initializer}}
  int integer_array_checker nt_checked[] = {1, 2, 3, 4, 0};
}
