// Simple Example of Checked C, from Section 3.4 of the spec

#include <stdchecked.h>

// Original Function
int deref_orig(ptr<int> x : bounds(x, x + c), int c) {
  int z;
  z = *x;
  return z;
}

// Function Equivalent after Code Generation
int deref_code(ptr<int> x : bounds(x, x + c), int c) {
  int z;
  dynamic_check(x != NULL);
  dynamic_check(x <= x && x < x + c);
  z = *x;
  return z;
}
