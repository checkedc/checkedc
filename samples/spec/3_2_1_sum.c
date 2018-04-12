// Simple Example of Checked C, from Section 3.2.1 of the spec

#include <stdchecked.h>

int sum(array_ptr<int> start : bounds(start, end), array_ptr<int> end) {
  int result = 0;
  array_ptr<int> current : bounds(start, end) = start;

  while (current < end) {
    result += *current++;
  }

  return result;
}
