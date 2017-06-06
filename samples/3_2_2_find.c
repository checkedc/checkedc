// Simple Example of Checked C, from Section 3.2.2 of the spec

#include <stdchecked.h>

array_ptr<int> find(int key, 
                    array_ptr<int> a : count(len), 
                    int len) : bounds(a, a+len) {
  for (int i = 0; i < len; i++) {
    if (a[i] == key) {
      return &a[i];
    }
  }
  return NULL;
}
