// Simple Example of Checked C, based on code in Section 2.7 of the spec

#include <stdchecked.h>

void update(ptr<int> a, int b checked[5][5]) {
  for (int i = 0; i < 5; i++)
    for (int j = 0; j < 5; j++)
      b[i][j] = *a;
}
