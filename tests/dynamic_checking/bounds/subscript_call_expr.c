// Test bounds checking of array subscripting of call expressions,
// where the call expressions produce values with pointer types.
//
// Uses are tested for read, assignment,increment, and compound assignment
// operations. Each test takes two arguments describing the type of bounds
// and the type of operation.  Additional arguments describe test-specific
// information such as the index of the element to operate on and array lengths.
//
// This file is also used to test bounds checking of subscripting of call
// expressions where:
// - Subscripting is implemented using pointer arithmetic and pointer
//   dereference.  This is controlled by the macro POINTER_ARITHMETIC.
// - The called function has a return bounds-safe interface. This is
// controlled by the macro BOUNDS_INTERFACE.
//
// To shorten test times (and keep the parallelism more balanced when tests are
// run in parallel), separate test suite driver files are used for these
// additional cases.
//
//
// The following lines are for the clang automated test suite.
//
// RUN: %clang %s -o %t1 -Werror -Wno-unused-value %checkedc_target_flags
//
//
// Test operations on a pointer to 5 integers, where the integers are initialized to 0...4.
// The 3rd argument = element to do operation on.
//
//
// RUN: %checkedc_rununder %t1 constant_bounds read 5 | FileCheck %s --check-prefixes=CB-READ-START,CB-READ-FAIL
// RUN: %checkedc_rununder %t1 constant_bounds read  -1 | FileCheck %s --check-prefixes=CB-READ-START,CB-READ-FAIL
// RUN: %checkedc_rununder %t1 constant_bounds read 4 | FileCheck %s --check-prefixes=CB-READ-START,CB-READ-SUCCESS
// RUN: %checkedc_rununder %t1 constant_bounds read 0 | FileCheck %s --check-prefixes=CB-READ-START,CB-READ-SUCCESS
// Make sure we aren't accidentally running the bounds-safe interface verison of this test.
// RUN: %checkedc_rununder %t1 constant_bounds read 0 | FileCheck %s --check-prefixes=CB-READ-START,CB-READ-SUCCESS,CHECK-BSI-NOT
//
// RUN: %checkedc_rununder %t1 constant_bounds write 5| FileCheck %s --check-prefixes=CB-WRITE-START,CB-WRITE-FAIL
// RUN: %checkedc_rununder %t1 constant_bounds write -1 | FileCheck %s --check-prefixes=CB-WRITE-START,CB-WRITE-FAIL
// RUN: %checkedc_rununder %t1 constant_bounds write 4 | FileCheck %s --check-prefixes=CB-WRITE-START,CB-WRITE-SUCCESS
// RUN: %checkedc_rununder %t1 constant_bounds write 0 | FileCheck %s --check-prefixes=CB-WRITE-START,CB-WRITE-SUCCESS
//.
// RUN: %checkedc_rununder %t1 constant_bounds inc 7| FileCheck %s --check-prefixes=CB-INC-START,CB-INC-FAIL
// RUN: %checkedc_rununder %t1 constant_bounds inc -2 | FileCheck %s --check-prefixes=CB-INC-START,CB-INC-FAIL
// RUN: %checkedc_rununder %t1 constant_bounds inc 4 | FileCheck %s --check-prefixes=CB-INC-START,CB-INC-SUCCESS
// RUN: %checkedc_rununder %t1 constant_bounds inc 0 | FileCheck %s --check-prefixes=CB-INC-START,CB-INC-SUCCESS
//
// RUN: %checkedc_rununder %t1 constant_bounds compound 1000| FileCheck %s --check-prefixes=CB-COMPOUND-START,CB-COMPOUND-FAIL
// RUN: %checkedc_rununder %t1 constant_bounds compound -146 | FileCheck %s --check-prefixes=CB-COMPOUND-START,CB-COMPOUND-FAIL
// RUN: %checkedc_rununder %t1 constant_bounds compound 4 | FileCheck %s --check-prefixes=CB-COMPOUND-START,CB-COMPOUND-SUCCESS
// RUN: %checkedc_rununder %t1 constant_bounds compound 0 | FileCheck %s --check-prefixes=CB-COMPOUND-START,CB-COMPOUND-SUCCESS
//
//
// Test operations on a pointer with bounds dependent on the value of an argument n. The pointer points
// to n integers, where the integers are initialized 0...n-1.
// The 3rd argument = array length and 4th argument = element to do the operation on.
//
//
// RUN: %checkedc_rununder %t1 dependent_bounds read 2 5 | FileCheck %s --check-prefixes=DB-READ-START,DB-READ-FAIL
// RUN: %checkedc_rununder %t1 dependent_bounds read 3 -1 | FileCheck %s --check-prefixes=DB-READ-START,DB-READ-FAIL
// RUN: %checkedc_rununder %t1 dependent_bounds read 5 4 | FileCheck %s --check-prefixes=DB-READ-START,DB-READ-SUCCESS
// RUN: %checkedc_rununder %t1 dependent_bounds read 10 0 | FileCheck %s --check-prefixes=DB-READ-START,DB-READ-SUCCESS
//
// RUN: %checkedc_rununder %t1 dependent_bounds write 6 6 | FileCheck %s --check-prefixes=DB-WRITE-START,DB-WRITE-FAIL
// RUN: %checkedc_rununder %t1 dependent_bounds write 11 -1 | FileCheck %s --check-prefixes=DB-WRITE-START,DB-WRITE-FAIL
// RUN: %checkedc_rununder %t1 dependent_bounds write 3 2 | FileCheck %s --check-prefixes=DB-WRITE-START,DB-WRITE-SUCCESS
// RUN: %checkedc_rununder %t1 dependent_bounds write 3 0 | FileCheck %s --check-prefixes=DB-WRITE-START,DB-WRITE-SUCCESS
//
// RUN: %checkedc_rununder %t1 dependent_bounds inc 5 -1 | FileCheck %s --check-prefixes=DB-INC-START,DB-INC-FAIL
// RUN: %checkedc_rununder %t1 dependent_bounds inc 5 5 | FileCheck %s --check-prefixes=DB-INC-START,DB-INC-FAIL
// RUN: %checkedc_rununder %t1 dependent_bounds inc 12 11 | FileCheck %s --check-prefixes=DB-INC-START,DB-INC-SUCCESS
// RUN: %checkedc_rununder %t1 dependent_bounds inc 12 0 | FileCheck %s --check-prefixes=DB-INC-START,DB-INC-SUCCESS
//
// RUN: %checkedc_rununder %t1 dependent_bounds compound 50 1000| FileCheck %s --check-prefixes=DB-COMPOUND-START,DB-COMPOUND-FAIL
// RUN: %checkedc_rununder %t1 dependent_bounds compound 50 -146 | FileCheck %s --check-prefixes=DB-COMPOUND-START,DB-COMPOUND-FAIL
// RUN %checkedc_rununder %t1 dependent_bounds compound 10 9 | FileCheck %s --check-prefixes=DB-COMPOUND-START,DB-COMPOUND-SUCCESS
// RUN %checkedc_rununder %t1 dependent_bounds compound 10 0 | FileCheck %s --check-prefixes=DB-COMPOUND-START,DB-COMPOUND-SUCCESS
//
//
// Test operations on a pointer to null-terminated array of 5 integers, where the integers are
// initialized to 0...4.
// The 3rd argument = element to do the operation on.
//
//
// RUN: %checkedc_rununder %t1 nt_constant_bounds read 6 | FileCheck %s --check-prefixes=NT-CB-READ-START,NT-CB-READ-FAIL
// RUN: %checkedc_rununder %t1 nt_constant_bounds read -1 | FileCheck %s --check-prefixes=NT-CB-READ-START,NT-CB-READ-FAIL
// RUN: %checkedc_rununder %t1 nt_constant_bounds read 5 | FileCheck %s --check-prefixes=NT-CB-READ-START,NT-CB-READ-SUCCESS
// RUN: %checkedc_rununder %t1 nt_constant_bounds read 4 | FileCheck %s --check-prefixes=NT-CB-READ-START,NT-CB-READ-SUCCESS
// RUN: %checkedc_rununder %t1 nt_constant_bounds read 0 | FileCheck %s --check-prefixes=NT-CB-READ-START,NT-CB-READ-SUCCESS
//
// 4th argument = value to write.
// RUN: %checkedc_rununder %t1 nt_constant_bounds write 6 5| FileCheck %s --check-prefixes=NT-CB-WRITE-START,NT-CB-WRITE-FAIL
// RUN: %checkedc_rununder %t1 nt_constant_bounds write -1 5 | FileCheck %s --check-prefixes=NT-CB-WRITE-START,NT-CB-WRITE-FAIL
// RUN: %checkedc_rununder %t1 nt_constant_bounds write 5 0| FileCheck %s --check-prefixes=NT-CB-WRITE-START,NT-CB-WRITE-SUCCESS
// RUN: %checkedc_rununder %t1 nt_constant_bounds write 4 5| FileCheck %s --check-prefixes=NT-CB-WRITE-START,NT-CB-WRITE-SUCCESS
// RUN: %checkedc_rununder %t1 nt_constant_bounds write 0 5| FileCheck %s --check-prefixes=NT-CB-WRITE-START,NT-CB-WRITE-SUCCESS
//
// The next test line tries to increment the null terminator.
// RUN: %checkedc_rununder %t1 nt_constant_bounds inc 5 | FileCheck %s --check-prefixes=NT-CB-INC-START,NT-CB-INC-FAIL
// RUN: %checkedc_rununder %t1 nt_constant_bounds inc 7| FileCheck %s --check-prefixes=NT-CB-INC-START,NT-CB-INC-FAIL
// RUN: %checkedc_rununder %t1 nt_constant_bounds inc -2 | FileCheck %s --check-prefixes=NT-CB-INC-START,NT-CB-INC-FAIL
// RUN: %checkedc_rununder %t1 nt_constant_bounds inc 4 | FileCheck %s --check-prefixes=NT-CB-INC-START,NT-CB-INC-SUCCESS
// RUN: %checkedc_rununder %t1 nt_constant_bounds inc 0 | FileCheck %s --check-prefixes=NT-CB-INC-START,NT-CB-INC-SUCCESS
//
// The next test line tries to do a compound assignment on the null terminator.
// RUN: %checkedc_rununder %t1 nt_constant_bounds compound 5 | FileCheck %s --check-prefixes=NT-CB-COMPOUND-START,NT-CB-COMPOUND-FAIL
// RUN: %checkedc_rununder %t1 nt_constant_bounds compound 1000| FileCheck %s --check-prefixes=NT-CB-COMPOUND-START,NT-CB-COMPOUND-FAIL
// RUN: %checkedc_rununder %t1 nt_constant_bounds compound -146 | FileCheck %s --check-prefixes=NT-CB-COMPOUND-START,NT-CB-COMPOUND-FAIL
// RUN: %checkedc_rununder %t1 nt_constant_bounds compound 4 | FileCheck %s --check-prefixes=NT-CB-COMPOUND-START,NT-CB-COMPOUND-SUCCESS
// RUN: %checkedc_rununder %t1 nt_constant_bounds compound 0 | FileCheck %s --check-prefixes=NT-CB-COMPOUND-START,NT-CB-COMPOUND-SUCCESS
//
//
// Test operations on a pointer to a null-terminated array with bounds dependent on the value of an argument n.
// The pointer points an array of n integers, where the integers are initialized to 0, 2 ...2 * (n-1).
// 3rd argument = array length. 4th argument = element to do operation on.
//
//
// RUN: %checkedc_rununder %t1 nt_dependent_bounds read 2 5 | FileCheck %s --check-prefixes=NT-DB-READ-START,NT-DB-READ-FAIL
// RUN: %checkedc_rununder %t1 nt_dependent_bounds read 3 -1 | FileCheck %s --check-prefixes=NT-DB-READ-START,NT-DB-READ-FAIL
// Test reading null-terminator.
// RUN: %checkedc_rununder %t1 nt_dependent_bounds read 5 5 | FileCheck %s --check-prefixes=NT-DB-READ-START,NT-DB-READ-SUCCESS
// RUN: %checkedc_rununder %t1 nt_dependent_bounds read 5 4 | FileCheck %s --check-prefixes=NT-DB-READ-START,NT-DB-READ-SUCCESS
// RUN: %checkedc_rununder %t1 nt_dependent_bounds read 10 0 | FileCheck %s --check-prefixes=NT-DB-READ-START,NT-DB-READ-SUCCESS
//
// 5th argument = value to write.
// Test trying to overwrite null terminator with a non-zero value.
// RUN: %checkedc_rununder %t1 nt_dependent_bounds write 6 6 100 | FileCheck %s --check-prefixes=NT-DB-WRITE-START,NT-DB-WRITE-FAIL
// RUN: %checkedc_rununder %t1 nt_dependent_bounds write 6 10 15 | FileCheck %s --check-prefixes=NT-DB-WRITE-START,NT-DB-WRITE-FAIL
// RUN: %checkedc_rununder %t1 nt_dependent_bounds write 6 10 0  | FileCheck %s --check-prefixes=NT-DB-WRITE-START,NT-DB-WRITE-FAIL
// RUN: %checkedc_rununder %t1 nt_dependent_bounds write 11 -1 10 | FileCheck %s --check-prefixes=NT-DB-WRITE-START,NT-DB-WRITE-FAIL
// RUN: %checkedc_rununder %t1 nt_dependent_bounds write 11 -1 0 | FileCheck %s --check-prefixes=NT-DB-WRITE-START,NT-DB-WRITE-FAIL
// Test overwriting the null terminator with 0
// RUN: %checkedc_rununder %t1 nt_dependent_bounds write 6 6 0 | FileCheck %s --check-prefixes=NT-DB-WRITE-START,NT-DB-WRITE-SUCCESS
// RUN: %checkedc_rununder %t1 nt_dependent_bounds write 6 5 25 | FileCheck %s --check-prefixes=NT-DB-WRITE-START,NT-DB-WRITE-SUCCESS
// RUN: %checkedc_rununder %t1 nt_dependent_bounds write 3 0 10 | FileCheck %s --check-prefixes=NT-DB-WRITE-START,NT-DB-WRITE-SUCCESS
//
// RUN: %checkedc_rununder %t1 nt_dependent_bounds inc 5 -1 | FileCheck %s --check-prefixes=NT-DB-INC-START,NT-DB-INC-FAIL
// Try to do a compound assignment on the null terminator.
// RUN: %checkedc_rununder %t1 nt_dependent_bounds inc 5 5 | FileCheck %s --check-prefixes=NT-DB-INC-START,NT-DB-INC-FAIL
// RUN: %checkedc_rununder %t1 nt_dependent_bounds inc 20 21 | FileCheck %s --check-prefixes=NT-DB-INC-START,NT-DB-INC-FAIL
// RUN: %checkedc_rununder %t1 nt_dependent_bounds inc 12 11 | FileCheck %s --check-prefixes=NT-DB-INC-START,NT-DB-INC-SUCCESS
// RUN: %checkedc_rununder %t1 nt_dependent_bounds inc 12 0 | FileCheck %s --check-prefixes=NT-DB-INC-START,NT-DB-INC-SUCCESS
//
// Try to do a compound assignment on the null element.
// RUN: %checkedc_rununder %t1 nt_dependent_bounds compound 50 50 | FileCheck %s --check-prefixes=NT-DB-COMPOUND-START,NT-DB-COMPOUND-FAIL
// RUN: %checkedc_rununder %t1 nt_dependent_bounds compound 50 1000| FileCheck %s --check-prefixes=NT-DB-COMPOUND-START,NT-DB-COMPOUND-FAIL
// RUN: %checkedc_rununder %t1 nt_dependent_bounds compound 50 -146 | FileCheck %s --check-prefixes=NT-DB-COMPOUND-START,NT-DB-COMPOUND-FAIL
// RUN %checkedc_rununder %t1 nt_dependent_bounds compound 10 9 | FileCheck %s --check-prefixes=NT-DB-COMPOUND-START,NT-DB-COMPOUND-SUCCESS
// RUN %checkedc_rununder %t1 nt_dependent_bounds compound 10 0 | FileCheck %s --check-prefixes=NT-DB-COMPOUND-START,NT-DB-COMPOUND-SUCCESS
//
//
// Test operations on a pointer with bounds dependent on the value of an argument n. The pointer points
// to n 3-d integer arrays, where the integers are initialized by the sequence 1, 3, 5 ...  (i - 1) * 3 * 2 + 5),
// i.e. with a stride of 2.
// The 3rd argument = the array length (n), the 4th and 5th argument specify the
// element to do the operation on.  The 4th argument is the 1st dimension index,
// and the 5th argument is the 2nd dimension index.
//
// RUN: %checkedc_rununder %t1 md_dependent_bounds read 2 2 0   | FileCheck %s --check-prefixes=MD-DB-READ-START,MD-DB-READ-FAIL
// RUN: %checkedc_rununder %t1 md_dependent_bounds read 3 -1 1  | FileCheck %s --check-prefixes=MD-DB-READ-START,MD-DB-READ-FAIL
// This results in an access outside of the 2d array, so it fails.
// RUN: %checkedc_rununder %t1 md_dependent_bounds read 3 1 6   | FileCheck %s --check-prefixes=MD-DB-READ-START,MD-DB-READ-FAIL
// RUN: %checkedc_rununder %t1 md_dependent_bounds read 5 4 2  | FileCheck %s --check-prefixes=MD-DB-READ-START,MD-DB-READ-SUCCESS
// RUN: %checkedc_rununder %t1 md_dependent_bounds read 10 0 3 | FileCheck %s --check-prefixes=MD-DB-READ-START,MD-DB-READ-SUCCESS
// This is still within the entire array, it is allowed.
// RUN: %checkedc_rununder %t1 md_dependent_bounds read 10 0 4 | FileCheck %s --check-prefixes=MD-DB-READ-START,MD-DB-READ-SUCCESS
//
// RUN: %checkedc_rununder %t1 md_dependent_bounds write 6 6 2 | FileCheck %s --check-prefixes=MD-DB-WRITE-START,MD-DB-WRITE-FAIL
// RUN: %checkedc_rununder %t1 md_dependent_bounds write 6 5 3 | FileCheck %s --check-prefixes=MD-DB-WRITE-START,MD-DB-WRITE-FAIL
// RUN: %checkedc_rununder %t1 md_dependent_bounds write 11 -1 2 | FileCheck %s --check-prefixes=MD-DB-WRITE-START,MD-DB-WRITE-FAIL
// RUN: %checkedc_rununder %t1 md_dependent_bounds write 6 5 2 | FileCheck %s --check-prefixes=MD-DB-WRITE-START,MD-DB-WRITE-SUCCESS
// RUN: %checkedc_rununder %t1 md_dependent_bounds write 6 5 0 | FileCheck %s --check-prefixes=MD-DB-WRITE-START,MD-DB-WRITE-SUCCESS
// The access is still within the entire array, so it is allowed.
// RUN: %checkedc_rununder %t1 md_dependent_bounds write 3 0 5 | FileCheck %s --check-prefixes=MD-DB-WRITE-START,MD-DB-WRITE-SUCCESS
// RUN: %checkedc_rununder %t1 md_dependent_bounds write 3 0 0 | FileCheck %s --check-prefixes=MD-DB-WRITE-START,MD-DB-WRITE-SUCCESS
//
// RUN: %checkedc_rununder %t1 md_dependent_bounds inc 5 -1 0 | FileCheck %s --check-prefixes=MD-DB-INC-START,MD-DB-INC-FAIL
// RUN: %checkedc_rununder %t1 md_dependent_bounds inc 5 5 0 | FileCheck %s --check-prefixes=MD-DB-INC-START,MD-DB-INC-FAIL
// RUN: %checkedc_rununder %t1 md_dependent_bounds inc 5 0 -1 | FileCheck %s --check-prefixes=MD-DB-INC-START,MD-DB-INC-FAIL
// RUN: %checkedc_rununder %t1 md_dependent_bounds inc 5 4 4 | FileCheck %s --check-prefixes=MD-DB-INC-START,MD-DB-INC-FAIL
// RUN: %checkedc_rununder %t1 md_dependent_bounds inc 12 11 3| FileCheck %s --check-prefixes=MD-DB-INC-START,MD-DB-INC-FAIL
// RUN: %checkedc_rununder %t1 md_dependent_bounds inc 12 11 0 | FileCheck %s --check-prefixes=MD-DB-INC-START,MD-DB-INC-SUCCESS
// RUN: %checkedc_rununder %t1 md_dependent_bounds inc 12 11 2| FileCheck %s --check-prefixes=MD-DB-INC-START,MD-DB-INC-SUCCESS
// RUN: %checkedc_rununder %t1 md_dependent_bounds inc 12 0 0 | FileCheck %s --check-prefixes=MD-DB-INC-START,MD-DB-INC-SUCCESS
//
// These fail because the function call returns a null pointer.
// RUN: %checkedc_rununder %t1 md_dependent_bounds compound 50 1000 0| FileCheck %s --check-prefixes=MD-DB-COMPOUND-START,MD-DB-COMPOUND-FAIL
// RUN: %checkedc_rununder %t1 md_dependent_bounds compound 50 -146 5 | FileCheck %s --check-prefixes=MD-DB-COMPOUND-START,MD-DB-COMPOUND-FAIL
// These fail because of a bounds error.
// RUN: %checkedc_rununder %t1 md_dependent_bounds compound 50 50 0 | FileCheck %s --check-prefixes=MD-DB-COMPOUND-START,MD-DB-COMPOUND-FAIL
// RUN: %checkedc_rununder %t1 md_dependent_bounds compound 50 49 4 | FileCheck %s --check-prefixes=MD-DB-COMPOUND-START,MD-DB-COMPOUND-FAIL
// RUN: %checkedc_rununder %t1 md_dependent_bounds compound 50 48 6 | FileCheck %s --check-prefixes=MD-DB-COMPOUND-START,MD-DB-COMPOUND-FAIL
// RUN: %checkedc_rununder %t1 md_dependent_bounds compound 50 -1 0 | FileCheck %s --check-prefixes=MD-DB-COMPOUND-START,MD-DB-COMPOUND-FAIL
// RUN %checkedc_rununder %t1 md_dependent_bounds compound 10 9 3 | FileCheck %s --check-prefixes=MD-DB-COMPOUND-START,MD-DB-COMPOUND-SUCCESS
// RUN %checkedc_rununder %t1 md_dependent_bounds compound 10 0 0 | FileCheck %s --check-prefixes=MD-DB-COMPOUND-START,MD-DB-COMPOUND-SUCCESS
// RUN %checkedc_rununder %t1 md_dependent_bounds compound 50 1 1 | FileCheck %s --check-prefixes=MD-DB-COMPOUND-START,MD-DB-COMPOUND-SUCCESS

#include <assert.h>
#include <limits.h>
#include <signal.h>
#include <stdlib_checked.h>
#include <stdio_checked.h>
#include <string_checked.h>
#include <stdchecked.h>

#define SIZE 100  // pre-allocated array size.  We return pointers to
                  // pre-allocated static arrays so that we can check
                  // operations that use the pointer to modify memory.
#define CONSTANT_SIZE 5  // constant bound value.


#ifdef POINTER_ARITHMETIC
#define ACCESS_DIM1(e1, index1) (*(e1 + index1))
#define ACCESS_DIM2(e1, index1, index2) (*(*(e1 + index1) + index2))
#define ACCESS_DIM3(e1, index1, index2, index3)  (*(*(*(e1 + index1) + index2) + index3))
#else
#define ACCESS_DIM1(e1, index1) (e1[index1])
#define ACCESS_DIM2(e1, index1, index2) (e1[index1][index2])
#endif

#define TEST_READ1(e1, index1) ACCESS_DIM1(e1,index1)
#define TEST_READ2(e1, index1, index2) ACCESS_DIM2(e1, index1, index2)

#define TEST_WRITE1(e1, index1, e2) ACCESS_DIM1(e1,index1) = e2
#define TEST_WRITE2(e1, index1, index2, e2) ACCESS_DIM2(e1,index1, index2) = e2

#define TEST_INC1(e1, index1) (ACCESS_DIM1(e1,index1))++
#define TEST_INC2(e1, index1, index2) (ACCESS_DIM2(e1,index1, index2))++

#define TEST_COMPOUND_ASSIGN1(e1, index1, e2) ACCESS_DIM1(e1,index1) -= e2
#define TEST_COMPOUND_ASSIGN2(e1, index1, index2, e2) ACCESS_DIM2(e1,index1, index2) -= e2

// Two global arrays to return. That way we can check the result afterwward
// for operations that modify memory.

int arr_1d checked[SIZE];
int arr_2d checked[SIZE][3];
int arr_nt nt_checked[SIZE + 1];

// The test functions to call.
#ifdef BOUNDS_INTERFACE
int *g_const_bounds(void) : count(CONSTANT_SIZE);
int *g_dependent_bounds(unsigned int i) : count(i);
int *g_nt_const_bounds(void) : itype(nt_array_ptr<int>) count(CONSTANT_SIZE);
int *g_nt_dependent_bounds(unsigned int i) : itype(nt_array_ptr<int>) count(i);
typedef int arrty[3];
arrty *g_md_dependent_bounds(unsigned int i) : itype(array_ptr<int checked[3]>) count(i);
#else
array_ptr<int> g_const_bounds(void) : count(CONSTANT_SIZE);
array_ptr<int> g_dependent_bounds(unsigned int i) : count(i);
nt_array_ptr<int> g_nt_const_bounds(void) : count(CONSTANT_SIZE);
nt_array_ptr<int> g_nt_dependent_bounds(unsigned int i) : count(i);
array_ptr<int checked[3]> g_md_dependent_bounds(unsigned int i) : count(i);
#endif

// Initialize an integer array with a sequence of increasing nummbers
void int_array_init(array_ptr<int> p : count(i), unsigned int i, int stride) {
  int val = 0;
  for (unsigned int k = 0; k < i; k++) {
    p[k] = val;
    val += stride;
  }
}

// Initialize a 2-dimensional array with a sequence of increasing numbers.
void int_md_array_init(array_ptr<int checked[3]> p : count(i), unsigned int i, int stride) {
  int val = 1;
  for (unsigned int k = 0; k < i; k++) {
    for (unsigned int m = 0; m < 3; m++) {
      p[k][m] = val;
      val += stride;
    }
  }
}

// Allocate and initialize an integer array of size i,
// with the array initialized with values from 1...i
#ifdef BOUNDS_INTERFACE
int *g_dependent_bounds(unsigned int i) : count(i)
#else
array_ptr<int> g_dependent_bounds(unsigned int i) : count(i)
#endif
{
  if (i >= SIZE)
    return NULL;
  // TODO: after incorporating dataflow information into bounds declaration
  // checking, this dynamic_bounds_cast won't be needed.
  array_ptr<int> result : count(i) =
    dynamic_bounds_cast<array_ptr<int>>(arr_1d, count(i));
  int_array_init(result, i, 1);
  return result;
}

// Allocate and initialize an integer array of size CONSTANT_SIZE, with
// the array initialized with values from 1..CONSTANT_SIZE.
#ifdef BOUNDS_INTERFACE
int *g_const_bounds(void) : count(CONSTANT_SIZE)
#else
array_ptr<int> g_const_bounds(void) : count(CONSTANT_SIZE)
#endif
{
  return g_dependent_bounds(CONSTANT_SIZE);
}

// Allocate and initialize a null-terminated integer array of
// size i + 1, with the array initialized with values from 1, 2, ... 2*i,
// followed by 0.
#ifdef BOUNDS_INTERFACE
int *g_nt_dependent_bounds(unsigned int i) : itype(nt_array_ptr<int>) count(i)
#else
nt_array_ptr<int> g_nt_dependent_bounds(unsigned int i) : count(i)
#endif
unchecked {
  if (i >= SIZE + 1)
    return NULL;
  // TODO: after incorporating dataflow information into bounds declaration
  // checking, this dynamic_bounds_cast won't be needed.
  nt_array_ptr<int> result : count(i) =
    dynamic_bounds_cast<nt_array_ptr<int>>(arr_nt, count(i));
  int_array_init(result, i, 2);
  return result;
}

// Allocate and initialize a zero-terminated integer array of
// size i + 1, with the array initialized with
// values 1, 2, .. CONSTANT_SIZE - 1, 0.
#ifdef BOUNDS_INTERFACE
int *g_nt_const_bounds(void) : itype(nt_array_ptr<int>) count(CONSTANT_SIZE)
#else
nt_array_ptr<int> g_nt_const_bounds(void) : count(CONSTANT_SIZE)
#endif
{
  return g_nt_dependent_bounds(CONSTANT_SIZE);
}

// Allocate and initialize an array of size i of 3-element arrays.
// with the array elements initialized by the sequence
// 1, 3, 5 ...  (i - 1) * 3 * 2 + 5 (i.e. with a stride of 2).
#ifdef BOUNDS_INTERFACE
arrty *g_md_dependent_bounds(unsigned int i) : itype(array_ptr<int checked[3]>) count(i)
#else
array_ptr<int checked[3]> g_md_dependent_bounds(unsigned int i) : count(i)
#endif
{
   if (i >= SIZE)
     return NULL;
  // TODO: after incorporating dataflow information into bounds declaration
  // checking, this dynamic_bounds_cast won't be needed.
  array_ptr<int checked[3]> result : count(i) =
    dynamic_bounds_cast<array_ptr<int checked[3]>>(arr_2d, count(i));
  int_md_array_init(result, i, 2);
  return result;
}

// Handle an out-of-bounds reference by immediately exiting. This causes
// some output to be missing.
void handle_error(int err) {
  _Exit(0);
}

#define CHECK(e) { if (!(e)) unchecked { printf("check failed at line %d", __LINE__); _Exit(1); } }

enum OpKind {
   READ,
   WRITE,
   INC,
   COMPOUND
};

#pragma CHECKED_SCOPE on

enum OpKind get_operation(nt_array_ptr<char>i, nt_array_ptr<char>name) {
  if (strcmp(i, "read") == 0)
    return READ;
  else if (strcmp(i, "write") == 0)
    return WRITE;
  else if (strcmp(i, "inc") == 0)
    return INC;
  else if (strcmp(i, "compound") == 0)
    return COMPOUND;
  else {
    puts("Unknown ");
    puts(name);
    puts(" operation");
    _Exit(EXIT_FAILURE);
  }
}

void test_constant_bounds(int argc, array_ptr<nt_array_ptr<char>> argv : count(argc),
                          int idx) {
  // g_const_bounds returns a pointer to CONSTANT_SIZE integers filled with 0...4
  // The pointer value is arr_1d.
  enum OpKind op = get_operation(argv[idx], "constant bounds");
  idx++; // TODO: fold back into prior line
  switch (op) {
    case READ: {
      puts("Starting constant bounds read");
      // CB-READ-START: Starting constant bounds read
      int i = atoi(argv[idx]);
      idx++; // TODO: fold back into prior line once bounds declaration checking handle side-effecting call args.
      int r = TEST_READ1(g_const_bounds(), i);
      CHECK(r == i);
      puts("Passed constant bounds read");
      // CB-READ-FAIL-NOT: Passed constant bounds read
      // CB-READ-SUCCESS: Passed constant bounds read
      break;
    }
    case WRITE: {
      puts("Starting constant bounds write");
      // CB-WRITE-START: Starting constant bounds write
      int i = atoi(argv[idx]);
      idx++; // TODO: fold back into prior line
      int r = TEST_WRITE1(g_const_bounds(), i, 6);
      CHECK(arr_1d[i] == 6);
      puts("Passed constant bounds write");
      // CB-WRITE-FAIL-NOT: Passed constant bounds write
      // CB-WRITE-SUCCESS: Passed constant bounds write
      break;
    }
    case INC: {
      puts("Starting constant bounds increment");
      // CB-INC-START: Starting constant bounds increment
      int i = atoi(argv[idx]);
      idx++; // TODO: fold back into prior line
      int r = TEST_INC1(g_const_bounds(), i);
      CHECK(r == i);
      CHECK(arr_1d[i] == i + 1);
      puts("Passed constant bounds inc");
      // CB-INC-FAIL-NOT: Passed constant bounds inc
      // CB-INC-SUCCESS: Passed constant bounds inc
      break;
    }
    case COMPOUND: {
      puts("Starting constant bounds compound assign");
      // CB-COMPOUND-START: Starting constant bounds compound assign
      int i = atoi(argv[idx]);
      idx++; // TODO: fold back into prior line
      int r = TEST_COMPOUND_ASSIGN1(g_const_bounds(), i, 2);
      CHECK(r == i - 2);
      CHECK(arr_1d[i] == i - 2);
      puts("Passed constant bounds compound assign");
      // CB-COMPOUND-FAIL-NOT: Passed constant bounds compound assign
      // CB-COMPOUND-SUCCESS: Passed constant bounds compound assign
      break;
    }
  }
}

void test_dependent_bounds(int argc, array_ptr<nt_array_ptr<char>> argv : count(argc),
                       int idx) {
  // g_dependent_bounds(i) returns a pointer to i integers,
  // where the integers are initialized from 0...i-1.
  // The pointer value is arr_1d. i must be < SIZE
  // (the size of arr_1d)

  enum OpKind op = get_operation(argv[idx], "dependent bounds");
  idx++; // TODO: fold back into prior line
  switch (op) {
    case READ: {
      puts("Starting dependent bounds read");
      // DB-READ-START: Starting dependent bounds read
      int i = atoi(argv[idx]);
      idx++; // TODO: fold back into prior line
      int j = atoi(argv[idx]);
      idx++; // TODO: fold back into prior line
      int r = TEST_READ1(g_dependent_bounds(i), j);
      puts("Passed dependent bounds read");
      CHECK(r == j);
      // DB-READ-FAIL-NOT: Passed dependent bounds read
      // DB-READ-SUCCESS: Passed dependent bounds read
      break;
    }
    case WRITE: {
      puts("Starting dependent bounds write");
      // DB-WRITE-START: Starting dependent bounds write
      int i = atoi(argv[idx]);
      idx++; // TODO: fold back into prior line
      int j = atoi(argv[idx]);
      idx++; // TODO: fold back into prior line
      int r = TEST_WRITE1(g_dependent_bounds(i), j, 7);
      CHECK(arr_1d[j] == 7);
      puts("Passed dependent bounds write");
      // DB-WRITE-FAIL-NOT: Passed dependent bounds write
      // DB-WRITE-SUCCESS: Passed dependent bounds write
      break;
    }
    case INC: {
      puts("Starting dependent bounds increment");
      // BB-INC-START: Starting dependent bounds increment
      int i = atoi(argv[idx]);
      idx++; // TODO: fold back into prior line
      int j = atoi(argv[idx]);
      idx++; // TODO: fold back into prior line
      int r = TEST_INC1(g_dependent_bounds(2*i), 2*j);
      CHECK(r == 2*j);
      CHECK(arr_1d[2*j] == 2*j + 1);
      puts("Passed dependent bounds inc");
      // DB-INC-FAIL-NOT: Passed dependent bounds inc
      // DB-INC-SUCCESS: Passed dependent bounds inc
    }
    case COMPOUND: {
      puts("Starting dependent bounds compound assign");
      // DB-COMPOUND-START: Starting dependent bounds compound assign
      int i = atoi(argv[idx]);
      idx++; // TODO: fold back into prior line
      int j = atoi(argv[idx]);
      idx++; // TODO: fold back into prior line
      int r = TEST_COMPOUND_ASSIGN1(g_dependent_bounds(i + 3), j, 2);
      CHECK(r == j - 2);
      CHECK(arr_1d[i] == j - 2);
      puts("Passed dependent bounds compound assign");
      // DB-COMPOUND-FAIL-NOT: Passed dependent bounds compound assign
      // DB-COMPOUND-SUCCESS: Passed dependent bounds compound assign
      break;
    }
  }
}

void test_nt_constant_bounds(int argc, array_ptr<nt_array_ptr<char>> argv : count(argc),
                             int idx) {
  // g_nt_const_bounds returns a pointer to a null-terminated
  // array of integers.  There are CONSTANT_SIZE integers and a null-terminator integer.
  // The CONSTANT_SIZE integers filled with 0...(CONSTANT_SIZE -1)*2.
  // The pointer value is arr_nt.

  enum OpKind op = get_operation(argv[idx], "nt constant bounds");
  idx++; // TODO: fold back into prior line
  switch (op) {
    case READ: {
      puts("Starting nt constant bounds read");
      // NT-CB-READ-START: Starting nt constant bounds read
      int i = atoi(argv[idx]);
      idx++; // TODO: fold back into prior line
      int r = TEST_READ1(g_nt_const_bounds(), i);
      CHECK(r == 2 * i || (i == CONSTANT_SIZE && r == 0));
      puts("Passed nt constant bounds read");
      // NT-CB-READ-FAIL-NOT: Passed nt constant bounds read
      // NT-CB-READ-SUCCESS: Passed nt constant bounds read
      break;
    }
    case WRITE: {
      puts("Starting nt constant bounds write");
      // NT-CB-WRITE-START: Starting nt constant bounds write
      int i = atoi(argv[idx]);
      idx++; // TODO: fold back into prior line
      int j = atoi(argv[idx]);
      idx++; // TODO: fold back into prior line
      int r = TEST_WRITE1(g_nt_const_bounds(), i, j);
      CHECK(arr_nt[i] == j);
      puts("Passed nt constant bounds write");
      // NT-CB-WRITE-FAIL-NOT: Passed nt constant bounds write
      // NT-CB-WRITE-SUCCESS: Passed nt constant bounds write
      break;
    }
    case INC: {
      puts("Starting nt constant bounds increment");
      // NT-CB-INC-START: Starting nt constant bounds increment
      int i = atoi(argv[idx]);
      idx++; // TODO: fold back into prior line
      int r = TEST_INC1(g_nt_const_bounds(), i);
      CHECK(r == 2 * i);
      CHECK(arr_nt[i] == 2 * i + 1);
      puts("Passed nt constant bounds inc");
      // NT-CB-INC-FAIL-NOT: Passed nt constant bounds inc
      // NT-CB-INC-SUCCESS: Passed nt constant bounds inc
      break;
    }
    case COMPOUND: {
      puts("Starting nt constant bounds compound assign");
      // NT-CB-COMPOUND-START: Starting nt constant bounds compound assign
      int i = atoi(argv[idx]);
      idx++; // TODO: fold back into prior line
      int r = TEST_COMPOUND_ASSIGN1(g_nt_const_bounds(), i, 2);
      CHECK(r == 2 * i - 2);
      CHECK(arr_nt[i] == 2 * i - 2);
      puts("Passed nt constant bounds compound assign");
      // NT-CB-COMPOUND-FAIL-NOT: Passed nt constant bounds compound assign
      // NT-CB-COMPOUND-SUCCESS: Passed nt constant bounds compound assign
      break;
    }
  }
}

void test_nt_dependent_bounds(int argc, array_ptr<nt_array_ptr<char>> argv : count(argc),
                              int idx) {
  // g_nt_dependent_bounds(i) returns a pointer to i integers,
  // where the integers are initialized from 0...(i-1) * 2
  // The pointer value is arr_1d. i must be < SIZE
  // (the size of arr_1d)
  enum OpKind op = get_operation(argv[idx], "nt dependent bounds");
  idx++; // TODO: fold back into prior line
  switch (op) {
    case READ: {
      puts("Starting nt dependent bounds read");
      // NT-DB-READ-START: Starting nt dependent bounds read
      int i = atoi(argv[idx]);
      idx++; // TODO: fold back into prior line
      int j = atoi(argv[idx]);
      idx++; // TODO: fold back into prior line
      int r = TEST_READ1(g_nt_dependent_bounds(i), j);
      puts("Passed nt dependent bounds read");
      CHECK(r == 2 * j || (i == j && r == 0));
      // NT-DB-READ-FAIL-NOT: Passed nt dependent bounds read
      // NT-DB-READ-SUCCESS: Passed nt dependent bounds read
      break;
    }
    case WRITE: {
      puts("Starting nt dependent bounds write");
      // NT-DB-WRITE-START: Starting nt dependent bounds write
      int i = atoi(argv[idx]);
      idx++; // TODO: fold back into prior line
      int j = atoi(argv[idx]);
      idx++; // TODO: fold back into prior line
      int val = atoi(argv[idx]);
      idx++; // TODO: fold back into prior line
      int r = TEST_WRITE1(g_nt_dependent_bounds(i), j, val);
      CHECK(arr_nt[j] == val);
      puts("Passed nt dependent bounds write");
      // NT-DB-WRITE-FAIL-NOT: Passed nt dependent bounds write
      // NT-DB-WRITE-SUCCESS: Passed nt dependent bounds write
      break;
    }
    case INC: {
      puts("Starting nt dependent bounds increment");
      // BB-INC-START: Starting nt dependent bounds increment
      int i = atoi(argv[idx]);
      idx++; // TODO: fold back into prior line
      int j = atoi(argv[idx]);
      idx++; // TODO: fold back into prior line
      int r = TEST_INC1(g_nt_dependent_bounds(2*i), 2*j);
      CHECK(r == 4 * j);
      CHECK(arr_nt[2 * j] == 4 * j + 1);
      puts("Passed nt dependent bounds inc");
      // NT-DB-INC-FAIL-NOT: Passed nt dependent bounds inc
      // NT-DB-INC-SUCCESS: Passed nt dependent bounds inc
      break;
    }
    case COMPOUND: {
      puts("Starting nt dependent bounds compound assign");
      // NT-DB-COMPOUND-START: Starting nt dependent bounds compound assign
      int i = atoi(argv[idx]);
      idx++; // TODO: fold back into prior line
      int j = atoi(argv[idx]);
      idx++; // TODO: fold back into prior line
      int r = TEST_COMPOUND_ASSIGN1(g_nt_dependent_bounds(i + 3), j + 3, 2);
      CHECK(r == j * 2 - 2);
      CHECK(arr_nt[i] == j  * 2 - 2);
      puts("Passed nt dependent bounds compound assign");
      // NT-DB-COMPOUND-FAIL-NOT: Passed nt dependent bounds compound assign
      // NT-DB-COMPOUND-SUCCESS: Passed nt dependent bounds compound assign
      break;
    }
  }
}

int compute_val(int dim1, int dim2) {
   return 1 + dim1 * 6 + dim2 * 2;
}

void test_md_dependent_bounds(int argc, array_ptr<nt_array_ptr<char>> argv : count(argc),
                       int idx) {
  // g_md_dependent_bounds(i) returns a pointer to i 3-element arrays.
  // where the array elements are initialzied to the sequence
  // 1, 3, 5 ...  3 * 2 * i (i.e.  with a stride of 2).
  // The pointer value is arr_2d. i must be < SIZE
  // (the size of arr_1d)
  enum OpKind op = get_operation(argv[idx], "md dependent bounds");
  idx++; // TODO: fold back into prior line
  switch (op) {
    case READ: {
      puts("Starting md dependent bounds read");
      // MD-DB-READ-START: Starting md dependent bounds read
      int i = atoi(argv[idx]);
      idx++; // TODO: fold back into prior line
      int j = atoi(argv[idx]);
      idx++; // TODO: fold back into prior line
      int k = atoi(argv[idx]);
      idx++; // TODO: fold back into prior line
      int r = TEST_READ2(g_md_dependent_bounds(i), j, k);
      CHECK(r == compute_val(j, k));
      CHECK(arr_2d[j][k] == compute_val(j, k));
      puts("Passed md dependent bounds read");
      // MD-DB-READ-FAIL-NOT: Passed md dependent bounds read
      // MD-DB-READ-SUCCESS: Passed md dependent bounds read
      break;
    }
    case WRITE: {
      puts("Starting md dependent bounds write");
      // MD-DB-WRITE-START: Starting md dependent bounds write
      int i = atoi(argv[idx]);
      idx++; // TODO: fold back into prior line
      int j = atoi(argv[idx]);
      idx++; // TODO: fold back into prior line
      int k = atoi(argv[idx]);
      idx++; // TODO: fold back into prior line
      int r = TEST_WRITE2(g_md_dependent_bounds(i), j, k, 7);
      CHECK(arr_2d[j][k] == 7);
      puts("Passed md dependent bounds write");
      // MD-DB-WRITE-FAIL-NOT: Passed md dependent bounds write
      // MD-DB-WRITE-SUCCESS: Passed md dependent bounds write
      break;
    }
    case INC: {
      puts("Starting md dependent bounds increment");
      // MD-DB-INC-START: Starting md dependent bounds increment
      int i = atoi(argv[idx]);
      idx++; // TODO: fold back into prior line
      int j = atoi(argv[idx]);
      idx++; // TODO: fold back into prior line
      int k = atoi(argv[idx]);
      idx++; // TODO: fold back into prior line
      int r = TEST_INC2(g_md_dependent_bounds(i), j, k);
      CHECK(r == compute_val(j, k));
      CHECK(arr_2d[j][k] == compute_val(j, k) + 1);
      puts("Passed md dependent bounds inc");
      // MD-DB-INC-FAIL-NOT: Passed md dependent bounds inc
      // MD-DB-INC-SUCCESS: Passed md dependent bounds inc
      break;
    }
    case COMPOUND: {
      puts("Starting md dependent bounds compound assign");
      // MD-DB-COMPOUND-START: Starting md dependent bounds compound assign
      int i = atoi(argv[idx]);
      idx++; // TODO: fold back into prior line
      int j = atoi(argv[idx]);
      idx++; // TODO: fold back into prior line
      int k = atoi(argv[idx]);
      idx++; // TODO: fold back into prior line
      int r = TEST_COMPOUND_ASSIGN2(g_md_dependent_bounds(i), j, k, 2);
      CHECK(r == compute_val(j, k) - 2);
      CHECK(arr_2d[j][k] == compute_val(j, k) - 2);
      puts("Passed md dependent bounds compound assign");
      // MD-DB-COMPOUND-FAIL-NOT: Passed md dependent bounds compound assign
      // MD-DB-COMPOUND-SUCCESS: Passed md dependent bounds compound assign
      break;
    }
  }
}
#pragma CHECKED_SCOPE off

int main(int argc, array_ptr<nt_array_ptr<char>> argv : count(argc)) {

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
    // CHECK-NOT: Error Setting Up Buffering
    puts("Error Setting Up Buffering");
    return EXIT_FAILURE;
  }

  int idx = 1;
  nt_array_ptr<char> test = argv[idx];
  idx++; // TODO: fold back into prior line.
  if (strcmp(test, "constant_bounds") == 0) {
    test_constant_bounds(argc, argv, idx);
  } else if (strcmp(test, "dependent_bounds") == 0) {
    test_dependent_bounds(argc, argv, idx);
  } else if (strcmp(test, "nt_constant_bounds") == 0) {
    test_nt_constant_bounds(argc, argv, idx);
  } else if (strcmp(test, "nt_dependent_bounds") == 0) {
    test_nt_dependent_bounds(argc, argv, idx);
  } else if (strcmp(test, "md_dependent_bounds") == 0) {
    test_md_dependent_bounds(argc, argv, idx);
  } else {
    puts("Unknown test case");
    return EXIT_FAILURE;
  }
  puts("Dynamic Checks Passed");

#ifdef BOUNDS_INTERFACE
  // This output has to be after any output for tests because of the way
  // FileCheck works when multiple prefixes are specified.  FileCheck
  // handles multiple prefixes by constructing a regular expression that
  // is the 'or' of all the prefixes, so order of output among different
  // prefixes must match order of checks in this file.  The checks for
  // tests precede the check below.
  puts("Testing interfaces");
  // CHECK-BSI: Testing interfaces
#endif

  return EXIT_SUCCESS;
}
