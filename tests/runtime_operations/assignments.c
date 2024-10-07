// The following lines are for the clang automated test suite
//
// RUN: %clang -fcheckedc-extension %s -o %t -Werror
// RUN: %t | FileCheck %s --check-prefixes=CHECK

#include <stdio.h>
#include <stdlib.h>

static char *test : itype(_Ptr<char>);
char p = 5;
static char *init  _Itype(char* _Single) = &p;

int testfn(char *buf : count(len), size_t len)
_Checked {
  test = init;
  return 0;
}


int main(int argc, char**_Array argv : count(argc)) {
  // CHECK: Starting test
  puts("Starting test");
  testfn(init, 1);
  if (test && *test == 5) {
    // CHECK: Finishing test successfully
    puts("Finishing test successfully");
    return EXIT_SUCCESS;
  }
  else
    return EXIT_FAILURE;
}
