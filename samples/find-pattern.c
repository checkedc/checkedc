// This file contains an example that illustrates the use of strings in
// Checked C.  The example is adapted from "The C Programming Language,
// Second Edition", by Brian Kernighan and Dennis Ritchie, p. 69.
// It reads a series of lines and check whether a string occurs in a line.
// If it does, it prints the line.
//
// To compile the file using the Checked C version of clang, on Unix/Mac use
//  clang -o find-pattern find-pattern.c
// On Windows use:
//  clang -o find-pattern.exe find-pattern.c
//
// To run it, create a file with some lines of text in it that contain a
// pattern you wish to match.  Then run:
//    find-pattern {your pattern} < {your file name}
// where you use your pattern and file name in replace of {your pattern}
// and {your file name}.
//
// This file illustrates two important points about using strings in Checked C.
// - Sometimes you need to allocate a fixed size buffer that will be treated
//   as both a null-terminated string and a regular array.  The buffer should
//   be given a null-terminated array type, not a regular array type.
// - You will need to either allocate an extra character, or avoid writes to the
//   last character in the buffer.
// - Take care when using array subscripting on strings.  It is easy to accidentally
//   march beyond the end of the declared bounds.

#include <stdchecked.h>
#include <stdio_checked.h>

// In the original code, this is 1000.  Make this 50 so we can easily test
// exceeding the maximum number of characters allowed in a line.
#define MAXLINE 50

#pragma CHECKED_SCOPE ON

int getline(char line checked[] : count(max), int max);
int strindex(char source nt_checked[], char searchfor nt_checked[]);
void print_string(char str nt_checked[]);

int main(int argc, nt_array_ptr<char> argv checked[] : count(argc)) {
  if (argc < 2) {
    // Th original code used printf.  Calls to variable argument functions
    // aren't allowed in checked scopes, so use separate statements.
    print_string("Usage: ");
    print_string(argv[0]);
    print_string(" pattern\n");
    return -1;
  }
  nt_array_ptr<char> pattern = argv[1];
  // Line is treated is both a null-terminated array and a regular array. 
  // We don't allow array_ptrs to be converted to nt_array_ptrs, but we allow
  // the reverse. For this reason, we make this a null-terminated array.  Add
  // an extra byte so that we have a place for the null terminator and retain
  // the original size (the count of  accessible elements is one less than the
  // dimension of the array).
  char line nt_checked[MAXLINE + 1] = {};
  int found = 0;
  while (getline(line, MAXLINE) > 0)
    if (strindex(line, pattern) >= 0) {
      // Avoid using printf again;
      print_string(line);
      found++;
    }
  return 0;
}

// Get line into s, return length.
int getline(char s checked[] : count(lim), int lim) {
  int c, i;
  // The original code modifies lim, which we are using as the
  // bounds.  Introduce a new variable for counting the available
  // space.
  int available = lim;

  i = 0;
  while (--available > 0 && (c = getchar()) != EOF && c != '\n')
    s[i++] = c;
  if (c == '\n')
    s[i++] = c;
  // We could have made s a nt_checked array.  We'd have to check
  // here to make sure we aren't trying to write to the null terminator.
  s[i] = '\0';
  return i;
}

// Return index of t in s, -1 if none.
int strindex(char source nt_checked[] : count(0),
             char searchfor nt_checked[] : count(0)) {
  int i = 0, k = 0;

  nt_array_ptr<char> s : count(i) = source;
  nt_array_ptr<char> t : count(k) = searchfor;

  for (i = 0; s[i] != '\0'; i++) {
    // The original code was:
    //  for (j = i, k = 0; t[k] != '\0' && s[j] == t[k]; j++, k++)
    // The problem with this approach is that on the second iteration, j > i,
    // which will cause a runtime bounds failure for s[j].
    // We just introduce a temporary instead and remove s[j].
    nt_array_ptr<char> tmp_s = s + i;
    for (k = 0; t[k] != '\0' && *tmp_s == t[k]; tmp_s++, k++)
      ;
     if (k > 0 && t[k] == '\0')
       return i;
  }
  return -1;
}

void print_string(char str nt_checked[]) {
  // Fputs has a bounds-safe interface and can be used in checked scopes.
  fputs(str, stdout);
}
