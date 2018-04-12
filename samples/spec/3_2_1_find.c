// Simple Example of Checked C, from Section 3.2.1 of the spec

#include <stdchecked.h>

int find(int key, array_ptr<int> a: count(len), int len) {
  for (int i = 0; i < len; i++) {
    if (a[i] == key) {
      return i;
    }
  }
  return -1;
}
