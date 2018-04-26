// This is a buggy version of a program that echos its arguments.
// There's an off-by-one error when looping over the arguments.

#include <stdio_checked.h>
#include <stdchecked.h>

#pragma CHECKED_SCOPE ON

int main(int argc, nt_array_ptr<char> argv checked[] : count(argc)) {
  for (int i = 1; i <= argc; i++) {  // introduce an off-by-one error.
    fputc(' ', stdout);
    fputs(argv[i], stdout);
  }
  fputc('\n', stdout);
  return 0;
}
