// Simple Example of Checked C, from Section 3.2.1 of the spec

#include <stdchecked.h>

// external-scoped variables that hold a buffer and its length
int buflen = 0;
array_ptr<int> buf : count(buflen) = NULL;

int sum(void) {
  int result = 0;
  for (int i = 0; i < buflen; i++) {
    result += buf[i];
  }
  return result;
}
