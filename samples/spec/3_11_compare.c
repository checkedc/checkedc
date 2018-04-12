// Simple Example of Checked C, from Section 2.8 of the spec

#include <stdchecked.h>

/* lexicographic comparison of two arrays of integers */
int compare(array_ptr<int> x : bounds(x, x_end),
            array_ptr<int> y : bounds(y, y_end),
            array_ptr<int> x_end, array_ptr<int> y_end) {
  while (x < x_end && y < y_end) {
    if (*x == *y) {
      x++;
      y++;
    }
    else if (*x < *y) {
      return -1;
    }
    else {
      return 1;
    }
  }
  if (x == x_end && y == y_end) {
    return 0;
  }
  else if (x != x_end) {
    return 1;
  }
  else {
    return -1;
  }
}
