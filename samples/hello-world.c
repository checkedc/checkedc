#include <stdio_checked.h>
#include <stdchecked.h>

#pragma BOUNDS_CHECKED ON

int main(int argc, nt_array_ptr<char> argv checked[] : count(argc)) {
  puts("hello, world");
  return 0;
}
