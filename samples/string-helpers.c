// This file contains examples that illustrate the use of strings in
// Checked C.  Strings are null-terminated arrays of characters in C.
// The examples are adapted from "The C Programming Language", Second Edition,
// by Brian Kernighan and Dennis Ritchie.
//
// To compile the file using the Checked C version of clang, on Unix/Mac use
//  clang -o string-helpers string-helper.c
// On Windows use:
//  clang -o string-helpers.exe string-helpers.c
//
// Then run the program with 6 string arguments, the 3rd of which should
// be an integer  For example:
//   string-helpers hello meet -125 fed brown red
//
// Overview of using null-terminated arrays in Checked C:
//
//   In Checked C, pointers to null-terminated data are represented using the
//   nt_array_ptr type.  Arrays of null-terminated data are represented
//   using the nt_checked type.
//
//   An nt_array_ptr is like an array_ptr, except that the array that it
//   points to is followed by a sequence of elements that is null-terminated.
//   The bounds on the nt_array_ptr delimit the array.  The sequence begins
//   at the upper bound.
//
//   Because of this,  nt_array_ptr and nt_checked arrays follow slightly
//   different rules than array_ptr and checked arrays do:
//   - Given an nt_array_ptr, the element exactly at the upper bound of the
//     nt_array_ptr can be read (it is the first element of the sequence).
//     A 0 can be written there using an assignment (there must always be
//     enough space in the sequence for at least the null terminator).  No
//     other writes to that element are allowed.
//   - Given an nt_checked array, it converts to an nt_array_ptr whose count
//     excludes the  null terminator (whose count is one less than the declared
//     size).
//   - nt_array_ptrs have a default bounds of count(0). This means that you
//     can dereference an nt_array_ptr that does not have a bounds
//     declaration.
//
//   Some simple examples:
//
//   The following code is valid for an nt_array_ptr.  It would fail at runtime
//   for an array_ptr.
//
//   char fetch(nt_array_ptr<char> p : count(0)) {
//     return *p;
//   }
//
//   The following code is valid for an nt_array_ptr.  It would fail at runtime
//   for an array_ptr.
//
//   int f(nt_array_ptr<char> p : count(n), int n) {
//     if (p[n]) {  // read element at upper bound.
//       ...
//
//    If v != 0, the following code fails at runtime for an nt_array_ptr.  It
//    always fails for an array_ptr.
//
//     int set(nt_array_ptr<char> p : count(n), int n, int v) {
//       p[n] = v;  // fails unless v is zero.
//       ...
//     }
//
//    The following code is valid for an nt_array_ptr and would fail at runtime
//    for an array_ptr:
//
//    int set_zero(nt_array_ptr<char> p : count(n), int n) {
//      p[n] = 0;  // allowed for nt_array_ptr.  Runtime error for array_ptr!
//    }
//
//   The following example illustrates how bounds differ from
//   the count:
//
//   nt_array_ptr<char> : count(5) = "hello";
//
//   The string "hello" contains 6 characters, the last of which is
//   a null terminator.  The valid bounds is count(5).
//
//
// About the example functions.
//
// There three important aspects to using null-terminated character pointers
// in Checked C:
// 1. Nt_array_ptrs with no bounds declared have a default bounds of count(0).
// 2. If you are using array subscripting to access the nt_array_ptr,
//    you'll need to widen the bounds as you determine the last character
//    is not a null terminator.  This is typically done by
//    declaring bounds involving a separate counter:
//        array_ptr<char> p : count(0);
//        int i = 0;
//        array_ptr<char> tmp : count(i) = p;
// 3. While you can read the element at the upper bound, you have
//    to be careful to not try to write to it.  If you have code that
//    modifies a string at the upper bound, for now you must widen
//    the bounds manually by introducing a tempoary variable.
//
//    The following code will break:
//      int i = 0;
//      nt_array_ptr<char> arr : count(i) = ...
//      for ( ; arr[i] !=0; i++) {
//         arr[i] = ... // error, overwriting character at upper bound. i++
//                      // has not changed yet!
//
//    The recommended approach is to introduce a temporary variable with
//    widened bounds.
//      int i = 0;
//      nt_array_ptr<char> arr : count(i) = ..
//      for ( ; arr[i] !=0; i++) {
//         nt_array_ptr<char> : count(i+1) tmp = p;
//         tmp[i] = ...
//
//  The function my_strlen show points 1 and 2.  It is less
//  frequent for string functions to modify strings.  The function squeeze,
//  which does do that, shows point 3.
//
// (We plan to help programmers with point 3 by introducing a compiler analysis that
//  automatically widens bounds for null-terminated arrays base on local
//  information, but that is not done yet).

#include <ctype.h>
#include <stdchecked.h>
#include <stdio_checked.h>

// Return length of p (adapted from p. 39, K&R 2nd Edition).
// p implicitly has count(0).
checked int my_strlen(nt_array_ptr<char> p) {
  int i = 0;
  // Create a temporary whose count of elements
  // can change.
  nt_array_ptr<char> s : count(i) = p;
  // s[i] != 0 implies that the count can increase
  // by 1.
  while (s[i] != '\0')
    ++i;
  return i;
}

// Delete all c from p (adapted from p. 47, K&R 2nd Edition)
// p implicitly has count(0).
checked void squeeze(nt_array_ptr<char> p, char c) {
  int i = 0, j = 0;
  // Create a temporary whose count of elements can
  // change.
  nt_array_ptr<char> s : count(i) = p;
  for ( ; s[i] != '\0'; i++) {
    // We need to widen the bounds of s so that we
    // can assign to s[j] when j == i.
    // (In the long run, this will be taken care of by the
    // flow-sensitive bounds widening).
    nt_array_ptr<char> tmp : count(i + 1) = s;
    if (tmp[i] != c)
      tmp[j++] = tmp[i];
  }
  // if i==j, this writes a 0 at the upper bound.  Writing a 0 at the upper bound
  // is allowed for pointers to null-terminated arrays.  It is not allowed for
  // regular arrays.
  s[j] = 0;
}

// Convert p to integer (adapted from p. 61, K&R 2nd Edition).
checked int my_atoi(char p nt_checked[] : count(0)) {
  int i = 0;
  int n = 0;
  // Bounds of s grow as we establish that s[i] != 0
  nt_array_ptr<char> s : count(i) = p;
  for (i = 0; isspace(s[i]) && s[i] != 0; i++)  // skip white space
    ;
  int sign = (s[i] == '-') ? -1 : 1;
  if (s[i] == '+' || s[i] == '-') // skip sign
    i++;

  for (; s[i] >= '0' && s[i] <= '9'; ++i)
    n = 10 * n + (s[i] - '0');

  return sign * n;
}

// Reverse a string in place (p. 62, K&R 2nd Edition).
// p implicitly has count(0).
checked void reverse(nt_array_ptr<char> p) {
  int len = 0;
  // Calculate the length of the string.
  nt_array_ptr<char> s : count(len) = p;
  for (; s[len]; len++);

  // Now that we know the length, use s just like we would use an array_ptr.
  for (int i = 0, j = len - 1; i < j; i++, j--) {
    int c = s[i];
    s[i] = s[j];
    s[j] = c;
  }
}

// Return < 0 if s < t, 0 if s == t, > 0 if s > t.
// Adapted from p.106, K&R 2nd Edition.
// s and t implicitly have count(0).
checked int my_strcmp(nt_array_ptr<char> s, nt_array_ptr<char> t) {
  // Reading *s and *t is allowed for count(0)
  for (; *s == *t; s++, t++) // Incrementing s, t allowed because *s, *t != `\0`
    if (*s == '\0')
      return 0;
  return *s - *t;
}

int main(int argc, nt_array_ptr<char> argv checked[] : count(argc)) {
  if (argc < 7) {
    printf("Usage: %s arg1 arg2 <integer> ... arg6", argv[0]);
    return 0;
  }

  nt_array_ptr<char> arg1 = assume_bounds_cast<nt_array_ptr<char>>(argv[1], count(0));
  nt_array_ptr<char> arg2 = assume_bounds_cast<nt_array_ptr<char>>(argv[2], count(0));
  nt_array_ptr<char> arg3 = assume_bounds_cast<nt_array_ptr<char>>(argv[3], count(0));
  nt_array_ptr<char> arg4 = assume_bounds_cast<nt_array_ptr<char>>(argv[4], count(0));
  nt_array_ptr<char> arg5 = assume_bounds_cast<nt_array_ptr<char>>(argv[5], count(0));
  nt_array_ptr<char> arg6 = assume_bounds_cast<nt_array_ptr<char>>(argv[6], count(0));
  printf("strlen(\"%s\") = %d\n", arg1, my_strlen(arg1));
  printf("squeeze(\"%s\",'e') = ", arg2);
  squeeze(arg2, 'e');
  printf("\"%s\"\n", arg2);
  printf("atoi(\"%s\") = %d\n", arg3, my_atoi(arg3));
  printf("reverse(\"%s\") = ", arg4);
  reverse(arg4);
  printf("\"%s\"\n", arg4);
  printf("strcmp(\"%s\", \"%s\") = %d\n", arg5, arg6, my_strcmp(arg5, arg6));
  return 0;
}
