// Feature tests of static checking for variables with initializers
//
// The following lines are for the LLVM test harness:
//
// RUN: %clang_cc1 -verify -verify-ignore-unexpected=note -fcheckedc-extension %s

#include <stdchecked.h>

// Automatic variables with Checked C declarations or _Ptr types must
// always have initializers.
extern void f1() {
  array_ptr<int> v1;
  array_ptr<int> v2 = 0;
  array_ptr<int> v3 : bounds(none);
  array_ptr<int> v4 : bounds(none) = 0;
  array_ptr<int> v5 : count(5) = 0;
  array_ptr<int> v6 : count(5);                       // expected-error {{automatic variable 'v6' with bounds must have initializer}}
  array_ptr<int> v7 : byte_count(5 * sizeof(int)) = 0;
  array_ptr<int> v8 : byte_count(5 * sizeof(int));    // expected-error {{automatic variable 'v8' with bounds must have initializer}}
  array_ptr<int> v9 : bounds(v9, v9 + 5) = 0;
  array_ptr<int> v10 : bounds(v10, v10 + 5);          // expected-error {{automatic variable 'v10' with bounds must have initializer}}

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
nt_array_ptr<int> g31 : count(0)  = (int checked[]) { 0, [2] = 2, 3, 5, 0};
nt_array_ptr<char> g32 : count(5) = "abcde";
array_ptr<char> g33 : count(5) = "abcde";
array_ptr<char> g34 : count(5) = (char[5]) { 'a', 'b', 'c', 'd', 'e' };
array_ptr<char> g35 : count(5) = (char checked[5]) { 'a', 'b', 'c', 'd', 'e' };

//
// Checked arrays of checked pointers
//
nt_array_ptr<char> g36 checked[3][2] = { [1] = "ab", "cd", "ef", "jk" };
nt_array_ptr<char> g37 nt_checked[] = { "the", "brown", "fox", "jumped",
                                        "over", "the", "fence", 0 };

void callback1(int a);
void callback2(int a);
nt_array_ptr<ptr<void (int)>> callback_table = (ptr<void (int)>[]) { callback1, callback2, 0 };

void f3(char *escape) {
}

// Checked that the arrays and pointers have checked type.
void f4(void) checked {
  char t1 = g1[0];
  int t2  = g2[0];
  int t3 = g3[0][1];
  char t4 = g4[0];
  char t5 = g5[0];
  int t6 = g6[1][1];
  int t20 = g20[0];
  int t21 = g21[3];
  int t22 = g22[2][2];
  nt_array_ptr<char> t23 = g23[0];
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
  array_ptr<char> t34 = (char[5]) { 'a', 'b', 'c', 'd', 'e' };
  array_ptr<char> t35 : count(5) = (char checked[5]) { 'a', 'b', 'c', 'd', 'e' };

  //
  // Make sure parentheses are ignored.
  //
  nt_array_ptr<int> t36 : count(4) = ((int[]) { 0, [2] = 2, 3, 5, 0 });
  nt_array_ptr<char> t37 : count(5) = ("abcde");

  //
  // Checked arrays of checked pointers.
  //
  nt_array_ptr<char> t38 checked[3][2] = { [1] = "ab", "cd", "ef", "jk" };
  nt_array_ptr<char> t39 nt_checked[] = { "the", "brown", "fox", "jumped",
    "over", "the", "fence", 0 };

  char c = ((char *[2]) { "abc", "def" })[0][0];  // expected-error {{type in a checked\
 scope must use only checked types or parameter/return types with bounds-safe interfaces}}
  nt_array_ptr<ptr<void(int)>> callback_table = (ptr<void(int)>[]) { callback1, callback2, 0 };

}
