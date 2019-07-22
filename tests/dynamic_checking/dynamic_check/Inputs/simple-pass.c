// Feature tests of dynamic checking in Checked C.
//
// expected-no-diagnostics
// CHECK: hello

#include <stdio.h>
#include <stdbool.h>
#include <stdchecked.h>

int main(void) {
  printf("hello");
  dynamic_check(true);

  return 0;
}
