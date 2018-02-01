// Simple Example of Checked C, from Section 2.8 of the spec

#include <stdchecked.h>

/* sum integers stored between start and end, where end is not included */
int sum(array_ptr<int> start : bounds(start, end),
        array_ptr<int> end) {
  int result = 0;
  array_ptr<int> current : bounds(start, end) = start;

  while (current < end) {
    result = *current;
    current += 1;
  }

  return result;
}
