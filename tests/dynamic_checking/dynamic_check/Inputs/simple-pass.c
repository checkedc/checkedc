// Feature tests of dynamic checking in Checked C
//
// expected-no-diagnostics

#include <stdbool.h>
#include <stdchecked.h>

int main(void) {
  dynamic_check(true);

  return 0;
}
