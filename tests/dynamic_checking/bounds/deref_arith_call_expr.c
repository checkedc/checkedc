// Test bounds checking of subscripting of call expressions,
// where the call expression produces a pointer-typed value
// and subscripting is implemented using pointer arithmetic
// and pointer dereference expressions.
//
// Uses are tested for read, assignment,increment, and compound assignment 
// expressions. Each test takes two arguments describing the type of bounds
// and the type of operation.  Additional arguments describe test-specific
// information such as array lengths and the index of the element to 
// operate on.
// 
// This builds the file `subscript_call_expr.c` with 
// -DPOINTER_ARITHMETIC defined.
//
// The following lines are for the clang automated test suite.
//
// RUN: %clang %S/subscript_call_expr.c -DPOINTER_ARITHMETIC -o %t1 -Werror -Wno-unused-value %checkedc_target_flags
//
// Test operations on a pointer to 5 integers, where the integers are initialized to 0...4.
// The 3rd argument = element to perform operation on.
//
//
// RUN: %checkedc_rununder %t1 constant_bounds read 5 | FileCheck %S/subscript_call_expr.c --check-prefixes=CB-READ-START,CB-READ-FAIL
// RUN: %checkedc_rununder %t1 constant_bounds read  -1 | FileCheck %S/subscript_call_expr.c --check-prefixes=CB-READ-START,CB-READ-FAIL
// RUN: %checkedc_rununder %t1 constant_bounds read 4 | FileCheck %S/subscript_call_expr.c --check-prefixes=CB-READ-START,CB-READ-SUCCESS
// RUN: %checkedc_rununder %t1 constant_bounds read 0 | FileCheck %S/subscript_call_expr.c --check-prefixes=CB-READ-START,CB-READ-SUCCESS
//
// RUN: %checkedc_rununder %t1 constant_bounds write 5| FileCheck %S/subscript_call_expr.c --check-prefixes=CB-WRITE-START,CB-WRITE-FAIL
// RUN: %checkedc_rununder %t1 constant_bounds write -1 | FileCheck %S/subscript_call_expr.c --check-prefixes=CB-WRITE-START,CB-WRITE-FAIL
// RUN: %checkedc_rununder %t1 constant_bounds write 4 | FileCheck %S/subscript_call_expr.c --check-prefixes=CB-WRITE-START,CB-WRITE-SUCCESS
// RUN: %checkedc_rununder %t1 constant_bounds write 0 | FileCheck %S/subscript_call_expr.c --check-prefixes=CB-WRITE-START,CB-WRITE-SUCCESS
//.
// RUN: %checkedc_rununder %t1 constant_bounds inc 7| FileCheck %S/subscript_call_expr.c --check-prefixes=CB-INC-START,CB-INC-FAIL
// RUN: %checkedc_rununder %t1 constant_bounds inc -2 | FileCheck %S/subscript_call_expr.c --check-prefixes=CB-INC-START,CB-INC-FAIL
// RUN: %checkedc_rununder %t1 constant_bounds inc 4 | FileCheck %S/subscript_call_expr.c --check-prefixes=CB-INC-START,CB-INC-SUCCESS
// RUN: %checkedc_rununder %t1 constant_bounds inc 0 | FileCheck %S/subscript_call_expr.c --check-prefixes=CB-INC-START,CB-INC-SUCCESS
//
// RUN: %checkedc_rununder %t1 constant_bounds compound 1000| FileCheck %S/subscript_call_expr.c --check-prefixes=CB-COMPOUND-START,CB-COMPOUND-FAIL
// RUN: %checkedc_rununder %t1 constant_bounds compound -146 | FileCheck %S/subscript_call_expr.c --check-prefixes=CB-COMPOUND-START,CB-COMPOUND-FAIL
// RUN: %checkedc_rununder %t1 constant_bounds compound 4 | FileCheck %S/subscript_call_expr.c --check-prefixes=CB-COMPOUND-START,CB-COMPOUND-SUCCESS
// RUN: %checkedc_rununder %t1 constant_bounds compound 0 | FileCheck %S/subscript_call_expr.c --check-prefixes=CB-COMPOUND-START,CB-COMPOUND-SUCCESS
//
//
// Test operations on a pointer with bounds dependent on the value of an argument n. The pointer points
// to n integers, where the integers are initialized 0...n-1.
// The 3rd argument = array length and 4th argument = element to perform operation on.
//
//
// RUN: %checkedc_rununder %t1 dependent_bounds read 2 5 | FileCheck %S/subscript_call_expr.c --check-prefixes=DB-READ-START,DB-READ-FAIL
// RUN: %checkedc_rununder %t1 dependent_bounds read 3 -1 | FileCheck %S/subscript_call_expr.c --check-prefixes=DB-READ-START,DB-READ-FAIL
// RUN: %checkedc_rununder %t1 dependent_bounds read 5 4 | FileCheck %S/subscript_call_expr.c --check-prefixes=DB-READ-START,DB-READ-SUCCESS
// RUN: %checkedc_rununder %t1 dependent_bounds read 10 0 | FileCheck %S/subscript_call_expr.c --check-prefixes=DB-READ-START,DB-READ-SUCCESS
//
// RUN: %checkedc_rununder %t1 dependent_bounds write 6 6 | FileCheck %S/subscript_call_expr.c --check-prefixes=DB-WRITE-START,DB-WRITE-FAIL
// RUN: %checkedc_rununder %t1 dependent_bounds write 11 -1 | FileCheck %S/subscript_call_expr.c --check-prefixes=DB-WRITE-START,DB-WRITE-FAIL
// RUN: %checkedc_rununder %t1 dependent_bounds write 3 2 | FileCheck %S/subscript_call_expr.c --check-prefixes=DB-WRITE-START,DB-WRITE-SUCCESS
// RUN: %checkedc_rununder %t1 dependent_bounds write 3 0 | FileCheck %S/subscript_call_expr.c --check-prefixes=DB-WRITE-START,DB-WRITE-SUCCESS
//
// RUN: %checkedc_rununder %t1 dependent_bounds inc 5 -1 | FileCheck %S/subscript_call_expr.c --check-prefixes=DB-INC-START,DB-INC-FAIL
// RUN: %checkedc_rununder %t1 dependent_bounds inc 5 5 | FileCheck %S/subscript_call_expr.c --check-prefixes=DB-INC-START,DB-INC-FAIL
// RUN: %checkedc_rununder %t1 dependent_bounds inc 12 11 | FileCheck %S/subscript_call_expr.c --check-prefixes=DB-INC-START,DB-INC-SUCCESS
// RUN: %checkedc_rununder %t1 dependent_bounds inc 12 0 | FileCheck %S/subscript_call_expr.c --check-prefixes=DB-INC-START,DB-INC-SUCCESS
//
// RUN: %checkedc_rununder %t1 dependent_bounds compound 50 1000| FileCheck %S/subscript_call_expr.c --check-prefixes=DB-COMPOUND-START,DB-COMPOUND-FAIL
// RUN: %checkedc_rununder %t1 dependent_bounds compound 50 -146 | FileCheck %S/subscript_call_expr.c --check-prefixes=DB-COMPOUND-START,DB-COMPOUND-FAIL
// RUN %checkedc_rununder %t1 dependent_bounds compound 10 9 | FileCheck %S/subscript_call_expr.c --check-prefixes=DB-COMPOUND-START,DB-COMPOUND-SUCCESS
// RUN %checkedc_rununder %t1 dependent_bounds compound 10 0 | FileCheck %S/subscript_call_expr.c --check-prefixes=DB-COMPOUND-START,DB-COMPOUND-SUCCESS
//
//
// Test operations on a pointer to null-terminated array of 5 integers, where the integers are
// initialized to 0...4.
// The 3rd argument = element to perform operation on.
//
//
// RUN: %checkedc_rununder %t1 nt_constant_bounds read 6 | FileCheck %S/subscript_call_expr.c --check-prefixes=NT-CB-READ-START,NT-CB-READ-FAIL
// RUN: %checkedc_rununder %t1 nt_constant_bounds read -1 | FileCheck %S/subscript_call_expr.c --check-prefixes=NT-CB-READ-START,NT-CB-READ-FAIL
// RUN: %checkedc_rununder %t1 nt_constant_bounds read 5 | FileCheck %S/subscript_call_expr.c --check-prefixes=NT-CB-READ-START,NT-CB-READ-SUCCESS
// RUN: %checkedc_rununder %t1 nt_constant_bounds read 4 | FileCheck %S/subscript_call_expr.c --check-prefixes=NT-CB-READ-START,NT-CB-READ-SUCCESS
// RUN: %checkedc_rununder %t1 nt_constant_bounds read 0 | FileCheck %S/subscript_call_expr.c --check-prefixes=NT-CB-READ-START,NT-CB-READ-SUCCESS
//
// 4th argument = value to write.
// RUN: %checkedc_rununder %t1 nt_constant_bounds write 6 5| FileCheck %S/subscript_call_expr.c --check-prefixes=NT-CB-WRITE-START,NT-CB-WRITE-FAIL
// RUN: %checkedc_rununder %t1 nt_constant_bounds write -1 5 | FileCheck %S/subscript_call_expr.c --check-prefixes=NT-CB-WRITE-START,NT-CB-WRITE-FAIL
// RUN: %checkedc_rununder %t1 nt_constant_bounds write 5 0| FileCheck %S/subscript_call_expr.c --check-prefixes=NT-CB-WRITE-START,NT-CB-WRITE-SUCCESS
// RUN: %checkedc_rununder %t1 nt_constant_bounds write 4 5| FileCheck %S/subscript_call_expr.c --check-prefixes=NT-CB-WRITE-START,NT-CB-WRITE-SUCCESS
// RUN: %checkedc_rununder %t1 nt_constant_bounds write 0 5| FileCheck %S/subscript_call_expr.c --check-prefixes=NT-CB-WRITE-START,NT-CB-WRITE-SUCCESS
//
// The next test line tries to increment the null terminator.
// RUN: %checkedc_rununder %t1 nt_constant_bounds inc 5 | FileCheck %S/subscript_call_expr.c --check-prefixes=NT-CB-INC-START,NT-CB-INC-FAIL
// RUN: %checkedc_rununder %t1 nt_constant_bounds inc 7| FileCheck %S/subscript_call_expr.c --check-prefixes=NT-CB-INC-START,NT-CB-INC-FAIL
// RUN: %checkedc_rununder %t1 nt_constant_bounds inc -2 | FileCheck %S/subscript_call_expr.c --check-prefixes=NT-CB-INC-START,NT-CB-INC-FAIL
// RUN: %checkedc_rununder %t1 nt_constant_bounds inc 4 | FileCheck %S/subscript_call_expr.c --check-prefixes=NT-CB-INC-START,NT-CB-INC-SUCCESS
// RUN: %checkedc_rununder %t1 nt_constant_bounds inc 0 | FileCheck %S/subscript_call_expr.c --check-prefixes=NT-CB-INC-START,NT-CB-INC-SUCCESS
//
// The next test line tries to do a compound assignment on the null terminator.
// RUN: %checkedc_rununder %t1 nt_constant_bounds compound 5 | FileCheck %S/subscript_call_expr.c --check-prefixes=NT-CB-COMPOUND-START,NT-CB-COMPOUND-FAIL
// RUN: %checkedc_rununder %t1 nt_constant_bounds compound 1000| FileCheck %S/subscript_call_expr.c --check-prefixes=NT-CB-COMPOUND-START,NT-CB-COMPOUND-FAIL
// RUN: %checkedc_rununder %t1 nt_constant_bounds compound -146 | FileCheck %S/subscript_call_expr.c --check-prefixes=NT-CB-COMPOUND-START,NT-CB-COMPOUND-FAIL
// RUN: %checkedc_rununder %t1 nt_constant_bounds compound 4 | FileCheck %S/subscript_call_expr.c --check-prefixes=NT-CB-COMPOUND-START,NT-CB-COMPOUND-SUCCESS
// RUN: %checkedc_rununder %t1 nt_constant_bounds compound 0 | FileCheck %S/subscript_call_expr.c --check-prefixes=NT-CB-COMPOUND-START,NT-CB-COMPOUND-SUCCESS
//
//
// Test operations on a pointer to a null-terminated array with bounds dependent on the value of an argument n. 
// The pointer points an array of n integers, where the integers are initialized to 0, 2 ...2 * (n-1).
// 3rd argument = array length. 4th argument = element to performance operation on.
//
//
// RUN: %checkedc_rununder %t1 nt_dependent_bounds read 2 5 | FileCheck %S/subscript_call_expr.c --check-prefixes=NT-DB-READ-START,NT-DB-READ-FAIL
// RUN: %checkedc_rununder %t1 nt_dependent_bounds read 3 -1 | FileCheck %S/subscript_call_expr.c --check-prefixes=NT-DB-READ-START,NT-DB-READ-FAIL
// Test reading null-terminator.
// RUN: %checkedc_rununder %t1 nt_dependent_bounds read 5 5 | FileCheck %S/subscript_call_expr.c --check-prefixes=NT-DB-READ-START,NT-DB-READ-SUCCESS
// RUN: %checkedc_rununder %t1 nt_dependent_bounds read 5 4 | FileCheck %S/subscript_call_expr.c --check-prefixes=NT-DB-READ-START,NT-DB-READ-SUCCESS
// RUN: %checkedc_rununder %t1 nt_dependent_bounds read 10 0 | FileCheck %S/subscript_call_expr.c --check-prefixes=NT-DB-READ-START,NT-DB-READ-SUCCESS
//
// 5th argument = value to write.
// Test trying to overwrite null terminator with a non-zero value.
// RUN: %checkedc_rununder %t1 nt_dependent_bounds write 6 6 100 | FileCheck %S/subscript_call_expr.c --check-prefixes=NT-DB-WRITE-START,NT-DB-WRITE-FAIL
// RUN: %checkedc_rununder %t1 nt_dependent_bounds write 6 10 15 | FileCheck %S/subscript_call_expr.c --check-prefixes=NT-DB-WRITE-START,NT-DB-WRITE-FAIL
// RUN: %checkedc_rununder %t1 nt_dependent_bounds write 6 10 0  | FileCheck %S/subscript_call_expr.c --check-prefixes=NT-DB-WRITE-START,NT-DB-WRITE-FAIL
// RUN: %checkedc_rununder %t1 nt_dependent_bounds write 11 -1 10 | FileCheck %S/subscript_call_expr.c --check-prefixes=NT-DB-WRITE-START,NT-DB-WRITE-FAIL
// RUN: %checkedc_rununder %t1 nt_dependent_bounds write 11 -1 0 | FileCheck %S/subscript_call_expr.c --check-prefixes=NT-DB-WRITE-START,NT-DB-WRITE-FAIL
// Test overwriting the null terminator with 0
// RUN: %checkedc_rununder %t1 nt_dependent_bounds write 6 6 0 | FileCheck %S/subscript_call_expr.c --check-prefixes=NT-DB-WRITE-START,NT-DB-WRITE-SUCCESS
// RUN: %checkedc_rununder %t1 nt_dependent_bounds write 6 5 25 | FileCheck %S/subscript_call_expr.c --check-prefixes=NT-DB-WRITE-START,NT-DB-WRITE-SUCCESS
// RUN: %checkedc_rununder %t1 nt_dependent_bounds write 3 0 10 | FileCheck %S/subscript_call_expr.c --check-prefixes=NT-DB-WRITE-START,NT-DB-WRITE-SUCCESS
//
// RUN: %checkedc_rununder %t1 nt_dependent_bounds inc 5 -1 | FileCheck %S/subscript_call_expr.c --check-prefixes=NT-DB-INC-START,NT-DB-INC-FAIL
// Try to do a compound assignment on the null terminator.
// RUN: %checkedc_rununder %t1 nt_dependent_bounds inc 5 5 | FileCheck %S/subscript_call_expr.c --check-prefixes=NT-DB-INC-START,NT-DB-INC-FAIL
// RUN: %checkedc_rununder %t1 nt_dependent_bounds inc 20 21 | FileCheck %S/subscript_call_expr.c --check-prefixes=NT-DB-INC-START,NT-DB-INC-FAIL
// RUN: %checkedc_rununder %t1 nt_dependent_bounds inc 12 11 | FileCheck %S/subscript_call_expr.c --check-prefixes=NT-DB-INC-START,NT-DB-INC-SUCCESS
// RUN: %checkedc_rununder %t1 nt_dependent_bounds inc 12 0 | FileCheck %S/subscript_call_expr.c --check-prefixes=NT-DB-INC-START,NT-DB-INC-SUCCESS
//
// Try to do a compound assignment on the null element.
// RUN: %checkedc_rununder %t1 nt_dependent_bounds compound 50 50 | FileCheck %S/subscript_call_expr.c --check-prefixes=NT-DB-COMPOUND-START,NT-DB-COMPOUND-FAIL
// RUN: %checkedc_rununder %t1 nt_dependent_bounds compound 50 1000| FileCheck %S/subscript_call_expr.c --check-prefixes=NT-DB-COMPOUND-START,NT-DB-COMPOUND-FAIL
// RUN: %checkedc_rununder %t1 nt_dependent_bounds compound 50 -146 | FileCheck %S/subscript_call_expr.c --check-prefixes=NT-DB-COMPOUND-START,NT-DB-COMPOUND-FAIL
// RUN %checkedc_rununder %t1 nt_dependent_bounds compound 10 9 | FileCheck %S/subscript_call_expr.c --check-prefixes=NT-DB-COMPOUND-START,NT-DB-COMPOUND-SUCCESS
// RUN %checkedc_rununder %t1 nt_dependent_bounds compound 10 0 | FileCheck %S/subscript_call_expr.c --check-prefixes=NT-DB-COMPOUND-START,NT-DB-COMPOUND-SUCCESS
//
//
// Test operations on a pointer with bounds dependent on the value of an argument n. The pointer points
// to n 3-d integer arrays, where the integers are initialized by the sequence 1, 3, 5 ...  (i - 1) * 3 * 2 + 5),
// i .e. with a stride of 2.
// The 3rd argument = the array length (n), the 4th and 5th argument specify the
// element to perform an operation on.  The 4th argument is the 1st dimension index,
// and the 5th argument is the 2nd dimension index.  
//
// RUN: %checkedc_rununder %t1 md_dependent_bounds read 2 2 0   | FileCheck %S/subscript_call_expr.c --check-prefixes=MD-DB-READ-START,MD-DB-READ-FAIL
// RUN: %checkedc_rununder %t1 md_dependent_bounds read 3 -1 1  | FileCheck %S/subscript_call_expr.c --check-prefixes=MD-DB-READ-START,MD-DB-READ-FAIL
// This results in an access outside of the 2d array, so it fails.
// RUN: %checkedc_rununder %t1 md_dependent_bounds read 3 1 6   | FileCheck %S/subscript_call_expr.c --check-prefixes=MD-DB-READ-START,MD-DB-READ-FAIL
// RUN: %checkedc_rununder %t1 md_dependent_bounds read 5 4 2  | FileCheck %S/subscript_call_expr.c --check-prefixes=MD-DB-READ-START,MD-DB-READ-SUCCESS
// RUN: %checkedc_rununder %t1 md_dependent_bounds read 10 0 3 | FileCheck %S/subscript_call_expr.c --check-prefixes=MD-DB-READ-START,MD-DB-READ-SUCCESS
// This is still within the entire array, it is allowed.
// RUN: %checkedc_rununder %t1 md_dependent_bounds read 10 0 4 | FileCheck %S/subscript_call_expr.c --check-prefixes=MD-DB-READ-START,MD-DB-READ-SUCCESS
//
// RUN: %checkedc_rununder %t1 md_dependent_bounds write 6 6 2 | FileCheck %S/subscript_call_expr.c --check-prefixes=MD-DB-WRITE-START,MD-DB-WRITE-FAIL
// RUN: %checkedc_rununder %t1 md_dependent_bounds write 6 5 3 | FileCheck %S/subscript_call_expr.c --check-prefixes=MD-DB-WRITE-START,MD-DB-WRITE-FAIL
// RUN: %checkedc_rununder %t1 md_dependent_bounds write 11 -1 2 | FileCheck %S/subscript_call_expr.c --check-prefixes=MD-DB-WRITE-START,MD-DB-WRITE-FAIL
// RUN: %checkedc_rununder %t1 md_dependent_bounds write 6 5 2 | FileCheck %S/subscript_call_expr.c --check-prefixes=MD-DB-WRITE-START,MD-DB-WRITE-SUCCESS
// RUN: %checkedc_rununder %t1 md_dependent_bounds write 6 5 0 | FileCheck %S/subscript_call_expr.c --check-prefixes=MD-DB-WRITE-START,MD-DB-WRITE-SUCCESS
// The access is still within the entire array, so it is allowed.
// RUN: %checkedc_rununder %t1 md_dependent_bounds write 3 0 5 | FileCheck %S/subscript_call_expr.c --check-prefixes=MD-DB-WRITE-START,MD-DB-WRITE-SUCCESS
// RUN: %checkedc_rununder %t1 md_dependent_bounds write 3 0 0 | FileCheck %S/subscript_call_expr.c --check-prefixes=MD-DB-WRITE-START,MD-DB-WRITE-SUCCESS
//
// RUN: %checkedc_rununder %t1 md_dependent_bounds inc 5 -1 0 | FileCheck %S/subscript_call_expr.c --check-prefixes=MD-DB-INC-START,MD-DB-INC-FAIL
// RUN: %checkedc_rununder %t1 md_dependent_bounds inc 5 5 0 | FileCheck %S/subscript_call_expr.c --check-prefixes=MD-DB-INC-START,MD-DB-INC-FAIL
// RUN: %checkedc_rununder %t1 md_dependent_bounds inc 5 0 -1 | FileCheck %S/subscript_call_expr.c --check-prefixes=MD-DB-INC-START,MD-DB-INC-FAIL
// RUN: %checkedc_rununder %t1 md_dependent_bounds inc 5 4 4 | FileCheck %S/subscript_call_expr.c --check-prefixes=MD-DB-INC-START,MD-DB-INC-FAIL
// RUN: %checkedc_rununder %t1 md_dependent_bounds inc 12 11 3| FileCheck %S/subscript_call_expr.c --check-prefixes=MD-DB-INC-START,MD-DB-INC-FAIL
// RUN: %checkedc_rununder %t1 md_dependent_bounds inc 12 11 0 | FileCheck %S/subscript_call_expr.c --check-prefixes=MD-DB-INC-START,MD-DB-INC-SUCCESS
// RUN: %checkedc_rununder %t1 md_dependent_bounds inc 12 11 2| FileCheck %S/subscript_call_expr.c --check-prefixes=MD-DB-INC-START,MD-DB-INC-SUCCESS
// RUN: %checkedc_rununder %t1 md_dependent_bounds inc 12 0 0 | FileCheck %S/subscript_call_expr.c --check-prefixes=MD-DB-INC-START,MD-DB-INC-SUCCESS
//
// These fail because the function call returns a null pointer.
// RUN: %checkedc_rununder %t1 md_dependent_bounds compound 50 1000 0| FileCheck %S/subscript_call_expr.c --check-prefixes=MD-DB-COMPOUND-START,MD-DB-COMPOUND-FAIL
// RUN: %checkedc_rununder %t1 md_dependent_bounds compound 50 -146 5 | FileCheck %S/subscript_call_expr.c --check-prefixes=MD-DB-COMPOUND-START,MD-DB-COMPOUND-FAIL
// These fail because of a bounds error.
// RUN: %checkedc_rununder %t1 md_dependent_bounds compound 50 50 0 | FileCheck %S/subscript_call_expr.c --check-prefixes=MD-DB-COMPOUND-START,MD-DB-COMPOUND-FAIL
// RUN: %checkedc_rununder %t1 md_dependent_bounds compound 50 49 4 | FileCheck %S/subscript_call_expr.c --check-prefixes=MD-DB-COMPOUND-START,MD-DB-COMPOUND-FAIL
// RUN: %checkedc_rununder %t1 md_dependent_bounds compound 50 48 6 | FileCheck %S/subscript_call_expr.c --check-prefixes=MD-DB-COMPOUND-START,MD-DB-COMPOUND-FAIL
// RUN: %checkedc_rununder %t1 md_dependent_bounds compound 50 -1 0 | FileCheck %S/subscript_call_expr.c --check-prefixes=MD-DB-COMPOUND-START,MD-DB-COMPOUND-FAIL
// RUN %checkedc_rununder %t1 md_dependent_bounds compound 10 9 3 | FileCheck %S/subscript_call_expr.c --check-prefixes=MD-DB-COMPOUND-START,MD-DB-COMPOUND-SUCCESS
// RUN %checkedc_rununder %t1 md_dependent_bounds compound 10 0 0 | FileCheck %S/subscript_call_expr.c --check-prefixes=MD-DB-COMPOUND-START,MD-DB-COMPOUND-SUCCESS
// RUN %checkedc_rununder %t1 md_dependent_bounds compound 50 1 1 | FileCheck %S/subscript_call_expr.c --check-prefixes=MD-DB-COMPOUND-START,MD-DB-COMPOUND-SUCCESS

#include <stdlib.h>

int main(void) {
  return EXIT_FAILURE;
}
