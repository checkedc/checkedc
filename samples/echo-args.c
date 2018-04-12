// This program echos its arguments to its output.

#include <stdio_checked.h>
#include <stdchecked.h>

#pragma BOUNDS_CHECKED ON

int main(int argc, nt_array_ptr<char> argv checked[] : count(argc)) {
  for (int i = 1; i < argc; i++) {
    fputc(' ', stdout);
    fputs(argv[i], stdout);
  }
  fputc('\n', stdout);
  return 0;
}
