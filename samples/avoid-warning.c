// This example illustrates how to use dynamic_bounds_cast
// to avoid a compile-time warning about being unable to 
// prove declared bounds.
#include <stdio_checked.h>
#include <stdchecked.h>

#pragma CHECKED_SCOPE ON

extern void test(array_ptr<int> p : count(len), int len);


int main(int argc, nt_array_ptr<char> argv checked[] : count(argc)) {
  int arr checked[5]  = { 0, 1, 2, 3, 4 };
  test(arr, 4);
  return 0;
}

void test(array_ptr<int> p : count(len), int len) {
  // Code was originally:
  //   array_ptr<int> r : count(len - 1) = p + 1.
  array_ptr<int> r : count(len - 1) = 
    dynamic_bounds_cast<array_ptr<int>>(p + 1, count(len - 1));
}
