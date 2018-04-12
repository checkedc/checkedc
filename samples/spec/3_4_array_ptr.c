// Simple Example of Checked C, from Section 3.4 of the spec

#include <stdchecked.h>

// Original Function
int deref_orig(array_ptr<int> x : bounds(x, x + c), int c) {
  int z = x[5];
  return z;
}

// Function Equivalent after Code Generation
int deref_code(ptr<int> x : bounds(x, x + c), int c) {
  dynamic_check(x != NULL);
  int* t1 = x + 5;
  dynamic_check(t1 != NULL && x <= t1 && t1 < x + c); // simplifies to 5 < c
  int z = *t1;
  return z;
}
