// Simple Example of Checked C, from Section 3.2.1 of the spec

#include <stdchecked.h>

int add(int a checked[][2][2] : count(len),
        int b checked[][2][2] : count(len),
        int len) {
  for (int i = 0; i < len; i++) {
    a[i][0][0] += b[i][0][0];
    a[i][0][1] += b[i][0][1];
    a[i][1][0] += b[i][1][0];
    a[i][1][1] += b[i][1][1];
  }
}
