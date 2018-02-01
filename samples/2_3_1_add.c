// Simple Example of checked arrays, from Section 2.3.1 of the spec
//

#include <stdchecked.h>

void add(int a checked[2][2], int b checked[2][2]) {
  for (int i = 0; i < 2; i++) {
    for (int j = 0; j < 2; j++) {
      a[i][j] += b[i][j];
    }
  }
}
