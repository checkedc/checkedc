// Test bounds checking of uses of array subscript expressions.
//
// Uses are tested in read, assignment,increment, and compound assignment
// expressions.  The type of use is controlled by the macro names TEST_READ,
// TEST_WRITE, TEST_INCREMENT, and TEST_COMPOUND_ASSIGNMENT.  The file must
// be compiled with exactly one of those macro names defined.
//
// This file is also used to test bounds checking of uses pointer dereference
// expressions where the pointer dereference operator is applied to pointer
// arithmetic expressions.  This is controlled by the macro name
// POINTER_ARITHMETIC.  When this macro is undefined, array subscript is
// used. When it is defined the array subscripting is replaced by equivalent
// expressions that use pointer arithmetic and the dereferences.
//
// To shorten test times (and keep the parallelism more balanced when tests are
// run in parallel), separate test suite driver files are used for pointer
// arithmetic.
//
// The following lines are for the clang automated test suite.
//
// RUN: %clang %s -DTEST_READ -o %t1 -Werror -Wno-unused-value %checkedc_target_flags
// RUN: %checkedc_rununder %t1 0 0 0 0 0 0  0 0   0 0 0  | FileCheck %s
// RUN: %checkedc_rununder %t1 1 2 4 4 2 2  1 2   1 1 1  | FileCheck %s
// RUN: %checkedc_rununder %t1 2 4 8 8 4 4  2 1   2 2 2  | FileCheck %s
// RUN: %checkedc_rununder %t1 2 4 8 8 3 3  0 4   0 3 2  | FileCheck %s
// RUN: %checkedc_rununder %t1 2 4 8 8 3 3  1 3   0 1 5  | FileCheck %s
// RUN: %checkedc_rununder %t1 2 4 8 8 3 3  2 -1  2 -1 2 | FileCheck %s
// RUN: %checkedc_rununder %t1 3            | FileCheck %s --check-prefix=CHECK-FAIL-1
// RUN: %checkedc_rununder %t1 -1           | FileCheck %s --check-prefix=CHECK-FAIL-1
// RUN: %checkedc_rununder %t1 0 5          | FileCheck %s --check-prefix=CHECK-FAIL-1
// RUN: %checkedc_rununder %t1 0 -1         | FileCheck %s --check-prefix=CHECK-FAIL-1
// RUN: %checkedc_rununder %t1 0 0 9        | FileCheck %s --check-prefix=CHECK-FAIL-1
// RUN: %checkedc_rununder %t1 0 0 -1       | FileCheck %s --check-prefix=CHECK-FAIL-1
// RUN: %checkedc_rununder %t1 0 0 0 9      | FileCheck %s --check-prefix=CHECK-FAIL-1
// RUN: %checkedc_rununder %t1 0 0 0 -1     | FileCheck %s --check-prefix=CHECK-FAIL-1
// RUN: %checkedc_rununder %t1 0 0 0 0 5    | FileCheck %s --check-prefix=CHECK-FAIL-1
// RUN: %checkedc_rununder %t1 0 0 0 0 -1   | FileCheck %s --check-prefix=CHECK-FAIL-1
// RUN: %checkedc_rununder %t1 0 0 0 0 0 5  | FileCheck %s --check-prefix=CHECK-FAIL-1
// RUN: %checkedc_rununder %t1 0 0 0 0 0 -1 | FileCheck %s --check-prefix=CHECK-FAIL-1
// RUN: %checkedc_rununder %t1 0 0 0 0 0 0  3 0   | FileCheck %s --check-prefix=CHECK-FAIL-2
// RUN: %checkedc_rununder %t1 0 0 0 0 0 0  2 3   | FileCheck %s --check-prefix=CHECK-FAIL-2
// RUN: %checkedc_rununder %t1 0 0 0 0 0 0  0 9   | FileCheck %s --check-prefix=CHECK-FAIL-2
// RUN: %checkedc_rununder %t1 0 0 0 0 0 0  -1 -1 | FileCheck %s --check-prefix=CHECK-FAIL-2
// RUN: %checkedc_rununder %t1 0 0 0 0 0 0  0 0  3 0 0    | FileCheck %s --check-prefix=CHECK-FAIL-3
// RUN: %checkedc_rununder %t1 0 0 0 0 0 0  0 0  2 9 0    | FileCheck %s --check-prefix=CHECK-FAIL-3
// RUN: %checkedc_rununder %t1 0 0 0 0 0 0  0 0  2 2 3    | FileCheck %s --check-prefix=CHECK-FAIL-3
// RUN: %checkedc_rununder %t1 0 0 0 0 0 0  0 0  0 0 27   | FileCheck %s --check-prefix=CHECK-FAIL-3
// RUN: %checkedc_rununder %t1 0 0 0 0 0 0  0 0  -1 -1 -1 | FileCheck %s --check-prefix=CHECK-FAIL-3
//
// RUN: %clang %s -DTEST_WRITE -o %t2 -Werror %checkedc_target_flags
// RUN: %checkedc_rununder %t2 0 0 0 0 0 0  0 0   0 0 0  | FileCheck %s
// RUN: %checkedc_rununder %t2 1 2 4 4 2 2  1 2   1 1 1  | FileCheck %s
// RUN: %checkedc_rununder %t2 2 4 8 8 4 4  2 1   2 2 2  | FileCheck %s
// RUN: %checkedc_rununder %t2 2 4 8 8 3 3  0 4   0 3 2  | FileCheck %s
// RUN: %checkedc_rununder %t2 2 4 8 8 3 3  1 3   0 1 5  | FileCheck %s
// RUN: %checkedc_rununder %t2 2 4 8 8 3 3  2 -1  2 -1 2 | FileCheck %s
// RUN: %checkedc_rununder %t2 3          | FileCheck %s --check-prefix=CHECK-FAIL-1
// RUN: %checkedc_rununder %t2 -1         | FileCheck %s --check-prefix=CHECK-FAIL-1
// RUN: %checkedc_rununder %t2 0 5        | FileCheck %s --check-prefix=CHECK-FAIL-1
// RUN: %checkedc_rununder %t2 0 -1       | FileCheck %s --check-prefix=CHECK-FAIL-1
// RUN: %checkedc_rununder %t2 0 0 9      | FileCheck %s --check-prefix=CHECK-FAIL-1
// RUN: %checkedc_rununder %t2 0 0 -1     | FileCheck %s --check-prefix=CHECK-FAIL-1
// RUN: %checkedc_rununder %t2 0 0 0 9    | FileCheck %s --check-prefix=CHECK-FAIL-1
// RUN: %checkedc_rununder %t2 0 0 0 -1   | FileCheck %s --check-prefix=CHECK-FAIL-1
// Skip the case for testing string literals.
// RUN: %checkedc_rununder %t2 0 0 0 0 0 5  | FileCheck %s --check-prefix=CHECK-FAIL-1
// RUN: %checkedc_rununder %t2 0 0 0 0 0 -1 | FileCheck %s --check-prefix=CHECK-FAIL-1
// RUN: %checkedc_rununder %t2 0 0 0 0 0 0  3 0   | FileCheck %s --check-prefix=CHECK-FAIL-2
// RUN: %checkedc_rununder %t2 0 0 0 0 0 0  2 3   | FileCheck %s --check-prefix=CHECK-FAIL-2
// RUN: %checkedc_rununder %t2 0 0 0 0 0 0  0 9   | FileCheck %s --check-prefix=CHECK-FAIL-2
// RUN: %checkedc_rununder %t2 0 0 0 0 0 0  -1 -1 | FileCheck %s --check-prefix=CHECK-FAIL-2
// RUN: %checkedc_rununder %t2 0 0 0 0 0 0  0 0  3 0 0    | FileCheck %s --check-prefix=CHECK-FAIL-3
// RUN: %checkedc_rununder %t2 0 0 0 0 0 0  0 0  2 9 0    | FileCheck %s --check-prefix=CHECK-FAIL-3
// RUN: %checkedc_rununder %t2 0 0 0 0 0 0  0 0  2 2 3    | FileCheck %s --check-prefix=CHECK-FAIL-3
// RUN: %checkedc_rununder %t2 0 0 0 0 0 0  0 0  0 0 27   | FileCheck %s --check-prefix=CHECK-FAIL-3
// RUN: %checkedc_rununder %t2 0 0 0 0 0 0  0 0  -1 -1 -1 | FileCheck %s --check-prefix=CHECK-FAIL-3
//
// RUN: %clang %s -DTEST_INCREMENT -o %t3 -Werror %checkedc_target_flags
// RUN: %checkedc_rununder %t3 0 0 0 0 0 0  0 0   0 0 0  | FileCheck %s
// RUN: %checkedc_rununder %t3 1 2 4 4 2 2  1 2   1 1 1  | FileCheck %s
// RUN: %checkedc_rununder %t3 2 4 8 8 4 4  2 1   2 2 2  | FileCheck %s
// RUN: %checkedc_rununder %t3 2 4 8 8 3 3  0 4   0 3 2  | FileCheck %s
// RUN: %checkedc_rununder %t3 2 4 8 8 3 3  1 3   0 1 5  | FileCheck %s
// RUN: %checkedc_rununder %t3 2 4 8 8 3 3  2 -1  2 -1 2 | FileCheck %s
// RUN: %checkedc_rununder %t3 3          | FileCheck %s --check-prefix=CHECK-FAIL-1
// RUN: %checkedc_rununder %t3 -1         | FileCheck %s --check-prefix=CHECK-FAIL-1
// RUN: %checkedc_rununder %t3 0 5        | FileCheck %s --check-prefix=CHECK-FAIL-1
// RUN: %checkedc_rununder %t3 0 -1       | FileCheck %s --check-prefix=CHECK-FAIL-1
// RUN: %checkedc_rununder %t3 0 0 9      | FileCheck %s --check-prefix=CHECK-FAIL-1
// RUN: %checkedc_rununder %t3 0 0 -1     | FileCheck %s --check-prefix=CHECK-FAIL-1
// RUN: %checkedc_rununder %t3 0 0 0 9    | FileCheck %s --check-prefix=CHECK-FAIL-1
// RUN: %checkedc_rununder %t3 0 0 0 -1   | FileCheck %s --check-prefix=CHECK-FAIL-1
// Skip the case for testing string literals.
// RUN: %checkedc_rununder %t3 0 0 0 0 0 5  | FileCheck %s --check-prefix=CHECK-FAIL-1
// RUN: %checkedc_rununder %t3 0 0 0 0 0 -1 | FileCheck %s --check-prefix=CHECK-FAIL-1
// RUN: %checkedc_rununder %t3 0 0 0 0 0 0  3 0   | FileCheck %s --check-prefix=CHECK-FAIL-2
// RUN: %checkedc_rununder %t3 0 0 0 0 0 0  2 3   | FileCheck %s --check-prefix=CHECK-FAIL-2
// RUN: %checkedc_rununder %t3 0 0 0 0 0 0  0 9   | FileCheck %s --check-prefix=CHECK-FAIL-2
// RUN: %checkedc_rununder %t3 0 0 0 0 0 0  -1 -1 | FileCheck %s --check-prefix=CHECK-FAIL-2
// RUN: %checkedc_rununder %t3 0 0 0 0 0 0  0 0  3 0 0    | FileCheck %s --check-prefix=CHECK-FAIL-3
// RUN: %checkedc_rununder %t3 0 0 0 0 0 0  0 0  2 9 0    | FileCheck %s --check-prefix=CHECK-FAIL-3
// RUN: %checkedc_rununder %t3 0 0 0 0 0 0  0 0  2 2 3    | FileCheck %s --check-prefix=CHECK-FAIL-3
// RUN: %checkedc_rununder %t3 0 0 0 0 0 0  0 0  0 0 27   | FileCheck %s --check-prefix=CHECK-FAIL-3
// RUN: %checkedc_rununder %t3 0 0 0 0 0 0  0 0  -1 -1 -1 | FileCheck %s --check-prefix=CHECK-FAIL-3
//
// RUN: %clang %s -DTEST_COMPOUND_ASSIGN -o %t4 -Werror %checkedc_target_flags
// RUN: %checkedc_rununder %t4 0 0 0 0 0 0  0 0   0 0 0  | FileCheck %s
// RUN: %checkedc_rununder %t4 1 2 4 4 2 2  1 2   1 1 1  | FileCheck %s
// RUN: %checkedc_rununder %t4 2 4 8 8 4 4  2 1   2 2 2  | FileCheck %s
// RUN: %checkedc_rununder %t4 2 4 8 8 3 3  0 4   0 3 2  | FileCheck %s
// RUN: %checkedc_rununder %t4 2 4 8 8 3 3  1 3   0 1 5  | FileCheck %s
// RUN: %checkedc_rununder %t4 2 4 8 8 3 3  2 -1  2 -1 2 | FileCheck %s
// RUN: %checkedc_rununder %t4 3          | FileCheck %s --check-prefix=CHECK-FAIL-1
// RUN: %checkedc_rununder %t4 -1         | FileCheck %s --check-prefix=CHECK-FAIL-1
// RUN: %checkedc_rununder %t4 0 5        | FileCheck %s --check-prefix=CHECK-FAIL-1
// RUN: %checkedc_rununder %t4 0 -1       | FileCheck %s --check-prefix=CHECK-FAIL-1
// RUN: %checkedc_rununder %t4 0 0 9      | FileCheck %s --check-prefix=CHECK-FAIL-1
// RUN: %checkedc_rununder %t4 0 0 -1     | FileCheck %s --check-prefix=CHECK-FAIL-1
// RUN: %checkedc_rununder %t4 0 0 0 9    | FileCheck %s --check-prefix=CHECK-FAIL-1
// RUN: %checkedc_rununder %t4 0 0 0 -1   | FileCheck %s --check-prefix=CHECK-FAIL-1
// Skip the case for testing string literals.
// RUN: %checkedc_rununder %t4 0 0 0 0 0 5  | FileCheck %s --check-prefix=CHECK-FAIL-1
// RUN: %checkedc_rununder %t4 0 0 0 0 0 -1 | FileCheck %s --check-prefix=CHECK-FAIL-1
// RUN: %checkedc_rununder %t4 0 0 0 0 0 0  3 0   | FileCheck %s --check-prefix=CHECK-FAIL-2
// RUN: %checkedc_rununder %t4 0 0 0 0 0 0  2 3   | FileCheck %s --check-prefix=CHECK-FAIL-2
// RUN: %checkedc_rununder %t4 0 0 0 0 0 0  0 9   | FileCheck %s --check-prefix=CHECK-FAIL-2
// RUN: %checkedc_rununder %t4 0 0 0 0 0 0  -1 -1 | FileCheck %s --check-prefix=CHECK-FAIL-2
// RUN: %checkedc_rununder %t4 0 0 0 0 0 0  0 0  3 0 0    | FileCheck %s --check-prefix=CHECK-FAIL-3
// RUN: %checkedc_rununder %t4 0 0 0 0 0 0  0 0  2 9 0    | FileCheck %s --check-prefix=CHECK-FAIL-3
// RUN: %checkedc_rununder %t4 0 0 0 0 0 0  0 0  2 2 3    | FileCheck %s --check-prefix=CHECK-FAIL-3
// RUN: %checkedc_rununder %t4 0 0 0 0 0 0  0 0  0 0 27   | FileCheck %s --check-prefix=CHECK-FAIL-3
// RUN: %checkedc_rununder %t4 0 0 0 0 0 0  0 0  -1 -1 -1 | FileCheck %s --check-prefix=CHECK-FAIL-3

#include <signal.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdchecked.h>

#ifdef POINTER_ARITHMETIC
#define ACCESS_DIM1(e1, index1) (*(e1 + index1))
#define ACCESS_DIM2(e1, index1, index2) (*(*(e1 + index1) + index2))
#define ACCESS_DIM3(e1, index1, index2, index3)  (*(*(*(e1 + index1) + index2) + index3))
char *format_dim1 = "*(%s + %d) = %d\n";
char *format_dim2 = "*(*(%s + %d) + %d) = %d\n";
char *format_dim3 = "*(*(*(%s + %d) + %d) + %d) = %d\n";
#else
#define ACCESS_DIM1(e1, index1) (e1[index1])
#define ACCESS_DIM2(e1, index1, index2) (e1[index1][index2])
#define ACCESS_DIM3(e1, index1, index2, index3) (e1[index1][index2][index3])
char *format_dim1 = "%s[%d] = %d\n";
char *format_dim2 = "%s[%d][%d] = %d\n";
char *format_dim3 = "%s[%d][%d][%d] = %d\n";
#endif

#ifdef TEST_READ
#define TEST_OP_DIM1(e1, index1, e2) ACCESS_DIM1(e1,index1)
#define TEST_OP_DIM2(e1, index1, index2, e2) ACCESS_DIM2(e1, index1, index2)
#define TEST_OP_DIM3(e1, index1, index2, index3, e2) ACCESS_DIM3(e1, index1, index2, index3)
#endif

#ifdef TEST_WRITE
#define TEST_OP_DIM1(e1, index1, e2) ACCESS_DIM1(e1,index1) = e2
#define TEST_OP_DIM2(e1, index1, index2, e2) ACCESS_DIM2(e1,index1, index2) = e2
#define TEST_OP_DIM3(e1, index1, index2, index3, e2) ACCESS_DIM3(e1, index1, index2, index3) = e2
#endif

#ifdef TEST_INCREMENT
#define TEST_OP_DIM1(e1, index1, e2) (ACCESS_DIM1(e1,index1))++
#define TEST_OP_DIM2(e1, index1, index2, e2) (ACCESS_DIM2(e1,index1, index2))++
#define TEST_OP_DIM3(e1, index1, index2, index3, e2) (ACCESS_DIM3(e1, index1, index2, index3))++
#endif

#ifdef TEST_COMPOUND_ASSIGN
#define TEST_OP_DIM1(e1, index1, e2) ACCESS_DIM1(e1,index1) -= e2
#define TEST_OP_DIM2(e1, index1, index2, e2) ACCESS_DIM2(e1,index1, index2) -= e2
#define TEST_OP_DIM3(e1, index1, index2, index3, e2) ACCESS_DIM3(e1, index1, index2, index3) -= e2
#endif


int a0 checked[9] = { 0, 1, 2, 3, 4, 5, 6, 7, 8 };
int ma1 checked[3][3] = { { 0, 1, 2 }, { 3, 4, 5 }, { 6, 7, 8 } };
int ma2 checked[3][3][3] = {
  { { 0, 1, 2 },{ 3, 4, 5 },{ 6, 7, 8 } },
  { { 0, 1, 2 },{ 3, 4, 5 },{ 6, 7, 8 } },
  { { 0, 1, 2 },{ 3, 4, 5 },{ 6, 7, 8 } }
};

array_ptr<int> a1 : count(3) = a0;

// Handle an out-of-bounds reference by immediately exiting. This causes
// some output to be missing.
void handle_error(int err) {
  _Exit(0);
}

// This signature for main is exactly what we want here,
// it also means any uses of argv[i] are checked too!
int main(int argc, array_ptr<char*> argv : count(argc)) {

  // Set up the handler for a failing bounds check.  Currently the Checked C
  // clang implementation raises a SIGILL or SIGTRAP when a bounds check fails,
  // depending on the target platform.  This may change in the future.
  signal(SIGILL, handle_error);
#if defined(__APPLE__) && defined(__aarch64__)
  signal(SIGTRAP, handle_error);
#endif

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

  int idx = 1;

  // CHECK: Starting Test
  // CHECK-FAIL-1: Starting Test
  // CHECK-FAIL-2: Starting Test
  // CHECK-FAIL-3: Starting Test
  puts("Starting Test");

  array_ptr<int> a2 : count(5) = a0;
  int a3 checked[9] = { 0, 1, 2, 3, 4, 5, 6, 7, 8 };

  int i = atoi(argv[idx++]);
  TEST_OP_DIM1(a1, i, i);
  printf(format_dim1, "a1", i, ACCESS_DIM1(a1, i));

  i = atoi(argv[idx++]);
  TEST_OP_DIM1(a2, i, i);
  printf(format_dim1, "a2", i, ACCESS_DIM1(a2, i));

  i = atoi(argv[idx++]);
  TEST_OP_DIM1(a3, i, i);
  printf(format_dim1, "a3", i, ACCESS_DIM1(a3, i));

  i = atoi(argv[idx++]);
  TEST_OP_DIM1(a0, i, i);
  printf(format_dim1, "a0", i, ACCESS_DIM1(a0, i));

  i = atoi(argv[idx++]);
#ifdef TEST_READ
  // Test accesses to checked string literal reads.  These
  // can only be read, and can't be written.
  _Checked {
    TEST_OP_DIM1("abcd", i, i);
  }
  printf(format_dim1, "\"abcd\"", i, ACCESS_DIM1("abcd", i));
#endif

  i = atoi(argv[idx++]);
  // Test accesses to compound array literals.  You can't
  // access the compound array literal after you directly assign
  // to an element of the array.  We'll still test that operation.
  TEST_OP_DIM1(((int checked[]) { 0, 1, 2, 3, 4}), i, i);
  printf(format_dim1, "(int[]) { 0, 1, 2, 3, 4}", i,
         ACCESS_DIM1(((int checked[]) { 0, 1, 2, 3, 4}), i));

  // CHECK: 1-Dimensional Checks Passed
  // CHECK-FAIL-1-NOT: 1-Dimensional Checks Passed
  // CHECK-FAIL-2: 1-Dimensional Checks Passed
  // CHECK-FAIL-3: 1-Dimensional Checks Passed
  puts("1-Dimensional Checks Passed");

  i = atoi(argv[idx++]);
  int j = atoi(argv[idx++]);
  TEST_OP_DIM2(ma1, i, j, i + j);
  printf(format_dim2, "ma1", i, j, ACCESS_DIM2(ma1, i, j));

  // CHECK: 2-Dimensional Checks Passed
  // CHECK-FAIL-1-NOT: 2-Dimensional Checks Passed
  // CHECK-FAIL-2-NOT: 2-Dimensional Checks Passed
  // CHECK-FAIL-3: 2-Dimensional Checks Passed
  puts("2-Dimensional Checks Passed");

  i = atoi(argv[idx++]);
  j = atoi(argv[idx++]);
  int k = atoi(argv[idx++]);
  TEST_OP_DIM3(ma2, i, j, k, i + j + k);
  printf(format_dim3, "ma2", i, j, k, ACCESS_DIM3(ma2, i, j, k));


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
