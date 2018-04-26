// This program echos its arguments to its output.

#include <stdio_checked.h>
#include <stdchecked.h>

#pragma CHECKED_SCOPE ON

int main(int argc, nt_array_ptr<char> argv checked[] : count(argc)) {
  for (int i = 1; i < argc; i++) {
    if (i > 1) {
      fputc(' ', stdout);
    }
    fputs(argv[i], stdout);
  }
  fputc('\n', stdout);
  return 0;
}

