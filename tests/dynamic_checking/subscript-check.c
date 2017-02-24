// Tests that the Array Subscript Operator works with dynamic checks
//
// The following lines are for the clang automated test suite
//
// RUN: %clang -fcheckedc-extension %s -o %t -Werror
// RUN: %t 0 0 0  0 0   0 0 0  | FileCheck %s
// RUN: %t 1 2 4  1 2   1 1 1  | FileCheck %s
// RUN: %t 2 4 8  2 1   2 2 2  | FileCheck %s
// RUN: %t 2 4 8  0 4   0 3 2  | FileCheck %s
// RUN: %t 2 4 8  1 3   0 1 5  | FileCheck %s
// RUN: %t 2 4 8  2 -1  2 -1 2 | FileCheck %s
// RUN: not --crash %t 3      | FileCheck %s --check-prefix=CHECK-FAIL-1
// RUN: not --crash %t -1     | FileCheck %s --check-prefix=CHECK-FAIL-1
// RUN: not --crash %t 0 5    | FileCheck %s --check-prefix=CHECK-FAIL-1
// RUN: not --crash %t 0 -1   | FileCheck %s --check-prefix=CHECK-FAIL-1
// RUN: not --crash %t 0 0 9  | FileCheck %s --check-prefix=CHECK-FAIL-1
// RUN: not --crash %t 0 0 -1 | FileCheck %s --check-prefix=CHECK-FAIL-1
// RUN: not --crash %t 0 0 0  3 0   | FileCheck %s --check-prefix=CHECK-FAIL-2
// RUN: not --crash %t 0 0 0  2 3   | FileCheck %s --check-prefix=CHECK-FAIL-2
// RUN: not --crash %t 0 0 0  0 9   | FileCheck %s --check-prefix=CHECK-FAIL-2
// RUN: not --crash %t 0 0 0  -1 -1 | FileCheck %s --check-prefix=CHECK-FAIL-2
// RUN: not --crash %t 0 0 0  0 0  3 0 0    | FileCheck %s --check-prefix=CHECK-FAIL-3
// RUN: not --crash %t 0 0 0  0 0  2 9 0    | FileCheck %s --check-prefix=CHECK-FAIL-3
// RUN: not --crash %t 0 0 0  0 0  2 2 3    | FileCheck %s --check-prefix=CHECK-FAIL-3
// RUN: not --crash %t 0 0 0  0 0  0 0 27   | FileCheck %s --check-prefix=CHECK-FAIL-3
// RUN: not --crash %t 0 0 0  0 0  -1 -1 -1 | FileCheck %s --check-prefix=CHECK-FAIL-3


#include <stdlib.h>
#include <stdio.h>
#include "../../include/stdchecked.h"

int a0 checked[9] = { 0, 1, 2, 3, 4, 5, 6, 7, 8 };
int ma1 checked[3][3] = { { 0, 1, 2 }, { 3, 4, 5 }, { 6, 7, 8 } };
int ma2 checked[3][3][3] = {
  { { 0, 1, 2 },{ 3, 4, 5 },{ 6, 7, 8 } },
  { { 0, 1, 2 },{ 3, 4, 5 },{ 6, 7, 8 } },
  { { 0, 1, 2 },{ 3, 4, 5 },{ 6, 7, 8 } }
};

array_ptr<int> a1 : count(3) = a0;

// This signature for main is exactly what we want here,
// it also means any uses of argv[i] are checked too!
int main(int argc, array_ptr<char*> argv : count(argc)) {

  // Unfortunately, using atoi everywhere below isn't super
  // great, as it will return 0 if it can't parse, which is a valid, 
  // non-erroring index. This is why we use CHECK-*-NOT to make sure
  // the tests fail before getting to certain output.

  // This makes sure output is not buffered for when
  // we hit errors.
  int err = setvbuf(stdout, NULL, _IONBF, 0);
  if (err) {
    // CHECK-NOT: Error Settinug Up Buffering
    puts("Error Setting Up Buffering");
    return EXIT_FAILURE;
  }

  // CHECK: Starting Test
  // CHECK-FAIL-1: Starting Test
  // CHECK-FAIL-2: Starting Test
  // CHECK-FAIL-3: Starting Test
  puts("Starting Test");

  array_ptr<int> a2 : count(5) = a0;
  array_ptr<int> a3 : bounds(a0, a0 + 9) = a0;

  int i = atoi(argv[1]);
  printf("a1[%d] = %d;\n", i, a1[i]);

  i = atoi(argv[2]);
  printf("a2[%d] = %d;\n", i, a2[i]);

  i = atoi(argv[3]);
  printf("a3[%d] = %d;\n", i, a3[i]);

  // CHECK: 1-Dimensional Checks Passed
  // CHECK-FAIL-1-NOT: 1-Dimensional Checks Passed
  // CHECK-FAIL-2: 1-Dimensional Checks Passed
  // CHECK-FAIL-3: 1-Dimensional Checks Passed
  puts("1-Dimensional Checks Passed");

  i = atoi(argv[4]);
  int j = atoi(argv[5]);
  printf("ma1[%d][%d] = %d;\n", i, j, ma1[i][j]);

  // CHECK: 2-Dimensional Checks Passed
  // CHECK-FAIL-1-NOT: 2-Dimensional Checks Passed
  // CHECK-FAIL-2-NOT: 2-Dimensional Checks Passed
  // CHECK-FAIL-3: 2-Dimensional Checks Passed
  puts("2-Dimensional Checks Passed");

  i = atoi(argv[6]);
  j = atoi(argv[7]);
  int k = atoi(argv[8]);
  printf("ma2[%d][%d][%d] = %d;\n", i, j, k, ma2[i][j][k]);


  // CHECK: 3-Dimensional Checks Passed
  // CHECK-FAIL-1-NOT: 3-Dimensional Checks Passed
  // CHECK-FAIL-2-NOT: 3-Dimensional Checks Passed
  // CHECK-FAIL-3-NOT: 3-Dimensional Checks Passed
  puts("3-Dimensional Checks Passed");

  // CHECK: All Dynamic Checks Passed
  // CHECK-FAIL-1-NOT: All Dynamic Checks Passed
  // CHECK-FAIL-2-NOT: All Dynamic Checks Passed
  // CHECK-FAIL-3-NOT: All Dynamic Checks Passed
  puts("All Dynamic Checks Passed");

  return EXIT_SUCCESS;
}