// This program shows how to use a signal handler to catch a runtime fault and
// exit gracefully.  The program reads an integer between 0 and 9 and prints
// out the corresponding argument.  If the integer is larger than the number of
// arguments, the program will crash.

#include <signal_checked.h>
#include <stdio_checked.h>
#include <stdlib_checked.h>
#include <stdchecked.h>

#pragma CHECKED_SCOPE ON

// The signal handling function.
void handle_error(int err) {
  _Exit(1);
}

int main(int argc, nt_array_ptr<char> argv checked[] : count(argc)) {
  // Set up the handler for a failing runtime check.   A SIGILL is raised when a Checked C
  // runtime check fails.

  // BUG: Compiler crashes when this is done in a checked scope, so put it
  // in a unchecked scope.   Checked C clang github issue #478.
  unchecked { signal(SIGILL, handle_error); }

  // Read the integer and print the corresponding argument.
  fputs("Enter a number between 0 and 9:", stdout);
  char c = fgetc(stdin);
  if (c < '0' || c > '9') {
    puts("\nInvalid number");
    return 2;
  }
  fputs("Result = ", stdout);
  puts(argv[c - '0']);
  return 0;
}
