// Test bounds checking of uses of pointer dereference expressions
// where the pointer derefence operator is applied to a pointer arithmetic
// expressions.  An example:
//
// int *p;
// *(p + 5) = 0
//
// This builds the file `subscript.c` for different operations with -DPOINTER_ARITHMETIC
// defined and /O3 producing a different executable for each operation. -DPOINTER_ARITHMEMTIC
// causes the file to be built using pointer arithmetic and dereference in place of subscripting.  
// We then run these executables with the same arguments that we run the original subscripting
// versions of the executables with.
//
// The following lines are for the clang automated test suite.
//
// RUN: %clang %S/subscript.c -DTEST_READ -DPOINTER_ARITHMETIC -o %t1 -Werror -Wno-unused-value -O3 %checkedc_target_flags
// RUN: %checkedc_rununder %t1 0 0 0 0 0 0  0 0   0 0 0  | FileCheck %S/subscript.c
// RUN: %checkedc_rununder %t1 1 2 4 4 2 2  1 2   1 1 1  | FileCheck %S/subscript.c
// RUN: %checkedc_rununder %t1 2 4 8 8 4 4  2 1   2 2 2  | FileCheck %S/subscript.c
// RUN: %checkedc_rununder %t1 2 4 8 8 3 3  0 4   0 3 2  | FileCheck %S/subscript.c
// RUN: %checkedc_rununder %t1 2 4 8 8 3 3  1 3   0 1 5  | FileCheck %S/subscript.c
// RUN: %checkedc_rununder %t1 2 4 8 8 3 3  2 -1  2 -1 2 | FileCheck %S/subscript.c
// RUN: %checkedc_rununder %t1 3            | FileCheck %S/subscript.c --check-prefix=CHECK-FAIL-1
// RUN: %checkedc_rununder %t1 -1           | FileCheck %S/subscript.c --check-prefix=CHECK-FAIL-1
// RUN: %checkedc_rununder %t1 0 5          | FileCheck %S/subscript.c --check-prefix=CHECK-FAIL-1
// RUN: %checkedc_rununder %t1 0 -1         | FileCheck %S/subscript.c --check-prefix=CHECK-FAIL-1
// RUN: %checkedc_rununder %t1 0 0 9        | FileCheck %S/subscript.c --check-prefix=CHECK-FAIL-1
// RUN: %checkedc_rununder %t1 0 0 -1       | FileCheck %S/subscript.c --check-prefix=CHECK-FAIL-1
// RUN: %checkedc_rununder %t1 0 0 0 9      | FileCheck %S/subscript.c --check-prefix=CHECK-FAIL-1
// RUN: %checkedc_rununder %t1 0 0 0 -1     | FileCheck %S/subscript.c --check-prefix=CHECK-FAIL-1
// RUN: %checkedc_rununder %t1 0 0 0 0 5    | FileCheck %S/subscript.c --check-prefix=CHECK-FAIL-1
// RUN: %checkedc_rununder %t1 0 0 0 0 -1   | FileCheck %S/subscript.c --check-prefix=CHECK-FAIL-1
// RUN: %checkedc_rununder %t1 0 0 0 0 0 5  | FileCheck %S/subscript.c --check-prefix=CHECK-FAIL-1
// RUN: %checkedc_rununder %t1 0 0 0 0 0 -1 | FileCheck %S/subscript.c --check-prefix=CHECK-FAIL-1
// RUN: %checkedc_rununder %t1 0 0 0 0 0 0  3 0   | FileCheck %S/subscript.c --check-prefix=CHECK-FAIL-2
// RUN: %checkedc_rununder %t1 0 0 0 0 0 0  2 3   | FileCheck %S/subscript.c --check-prefix=CHECK-FAIL-2
// RUN: %checkedc_rununder %t1 0 0 0 0 0 0  0 9   | FileCheck %S/subscript.c --check-prefix=CHECK-FAIL-2
// RUN: %checkedc_rununder %t1 0 0 0 0 0 0  -1 -1 | FileCheck %S/subscript.c --check-prefix=CHECK-FAIL-2
// RUN: %checkedc_rununder %t1 0 0 0 0 0 0  0 0  3 0 0    | FileCheck %S/subscript.c --check-prefix=CHECK-FAIL-3
// RUN: %checkedc_rununder %t1 0 0 0 0 0 0  0 0  2 9 0    | FileCheck %S/subscript.c --check-prefix=CHECK-FAIL-3
// RUN: %checkedc_rununder %t1 0 0 0 0 0 0  0 0  2 2 3    | FileCheck %S/subscript.c --check-prefix=CHECK-FAIL-3
// RUN: %checkedc_rununder %t1 0 0 0 0 0 0  0 0  0 0 27   | FileCheck %S/subscript.c --check-prefix=CHECK-FAIL-3
// RUN: %checkedc_rununder %t1 0 0 0 0 0 0  0 0  -1 -1 -1 | FileCheck %S/subscript.c --check-prefix=CHECK-FAIL-3
//
// RUN: %clang %S/subscript.c -DTEST_WRITE -DPOINTER_ARITHMETIC -o %t2 -Werror -O3 %checkedc_target_flags
// RUN: %checkedc_rununder %t2 0 0 0 0 0 0  0 0   0 0 0  | FileCheck %S/subscript.c
// RUN: %checkedc_rununder %t2 1 2 4 4 2 2  1 2   1 1 1  | FileCheck %S/subscript.c
// RUN: %checkedc_rununder %t2 2 4 8 8 4 4  2 1   2 2 2  | FileCheck %S/subscript.c
// RUN: %checkedc_rununder %t2 2 4 8 8 3 3  0 4   0 3 2  | FileCheck %S/subscript.c
// RUN: %checkedc_rununder %t2 2 4 8 8 3 3  1 3   0 1 5  | FileCheck %S/subscript.c
// RUN: %checkedc_rununder %t2 2 4 8 8 3 3  2 -1  2 -1 2 | FileCheck %S/subscript.c
// RUN: %checkedc_rununder %t2 3          | FileCheck %S/subscript.c --check-prefix=CHECK-FAIL-1
// RUN: %checkedc_rununder %t2 -1         | FileCheck %S/subscript.c --check-prefix=CHECK-FAIL-1
// RUN: %checkedc_rununder %t2 0 5        | FileCheck %S/subscript.c --check-prefix=CHECK-FAIL-1
// RUN: %checkedc_rununder %t2 0 -1       | FileCheck %S/subscript.c --check-prefix=CHECK-FAIL-1
// RUN: %checkedc_rununder %t2 0 0 9      | FileCheck %S/subscript.c --check-prefix=CHECK-FAIL-1
// RUN: %checkedc_rununder %t2 0 0 -1     | FileCheck %S/subscript.c --check-prefix=CHECK-FAIL-1
// RUN: %checkedc_rununder %t2 0 0 0 9    | FileCheck %S/subscript.c --check-prefix=CHECK-FAIL-1
// RUN: %checkedc_rununder %t2 0 0 0 -1   | FileCheck %S/subscript.c --check-prefix=CHECK-FAIL-1
// Skip the case for testing string literals.
// RUN: %checkedc_rununder %t2 0 0 0 0 0 5  | FileCheck %S/subscript.c --check-prefix=CHECK-FAIL-1
// RUN: %checkedc_rununder %t2 0 0 0 0 0 -1 | FileCheck %S/subscript.c --check-prefix=CHECK-FAIL-1
// RUN: %checkedc_rununder %t2 0 0 0 0 0 0  3 0   | FileCheck %S/subscript.c --check-prefix=CHECK-FAIL-2
// RUN: %checkedc_rununder %t2 0 0 0 0 0 0  2 3   | FileCheck %S/subscript.c --check-prefix=CHECK-FAIL-2
// RUN: %checkedc_rununder %t2 0 0 0 0 0 0  0 9   | FileCheck %S/subscript.c --check-prefix=CHECK-FAIL-2
// RUN: %checkedc_rununder %t2 0 0 0 0 0 0  -1 -1 | FileCheck %S/subscript.c --check-prefix=CHECK-FAIL-2
// RUN: %checkedc_rununder %t2 0 0 0 0 0 0  0 0  3 0 0    | FileCheck %S/subscript.c --check-prefix=CHECK-FAIL-3
// RUN: %checkedc_rununder %t2 0 0 0 0 0 0  0 0  2 9 0    | FileCheck %S/subscript.c --check-prefix=CHECK-FAIL-3
// RUN: %checkedc_rununder %t2 0 0 0 0 0 0  0 0  2 2 3    | FileCheck %S/subscript.c --check-prefix=CHECK-FAIL-3
// RUN: %checkedc_rununder %t2 0 0 0 0 0 0  0 0  0 0 27   | FileCheck %S/subscript.c --check-prefix=CHECK-FAIL-3
// RUN: %checkedc_rununder %t2 0 0 0 0 0 0  0 0  -1 -1 -1 | FileCheck %S/subscript.c --check-prefix=CHECK-FAIL-3
//
// RUN: %clang %S/subscript.c -DTEST_INCREMENT -DPOINTER_ARITHMETIC -o %t3 -Werror -O3 %checkedc_target_flags
// RUN: %checkedc_rununder %t3 0 0 0 0 0 0  0 0   0 0 0  | FileCheck %S/subscript.c
// RUN: %checkedc_rununder %t3 1 2 4 4 2 2  1 2   1 1 1  | FileCheck %S/subscript.c
// RUN: %checkedc_rununder %t3 2 4 8 8 4 4  2 1   2 2 2  | FileCheck %S/subscript.c
// RUN: %checkedc_rununder %t3 2 4 8 8 3 3  0 4   0 3 2  | FileCheck %S/subscript.c
// RUN: %checkedc_rununder %t3 2 4 8 8 3 3  1 3   0 1 5  | FileCheck %S/subscript.c
// RUN: %checkedc_rununder %t3 2 4 8 8 3 3  2 -1  2 -1 2 | FileCheck %S/subscript.c
// RUN: %checkedc_rununder %t3 3          | FileCheck %S/subscript.c --check-prefix=CHECK-FAIL-1
// RUN: %checkedc_rununder %t3 -1         | FileCheck %S/subscript.c --check-prefix=CHECK-FAIL-1
// RUN: %checkedc_rununder %t3 0 5        | FileCheck %S/subscript.c --check-prefix=CHECK-FAIL-1
// RUN: %checkedc_rununder %t3 0 -1       | FileCheck %S/subscript.c --check-prefix=CHECK-FAIL-1
// RUN: %checkedc_rununder %t3 0 0 9      | FileCheck %S/subscript.c --check-prefix=CHECK-FAIL-1
// RUN: %checkedc_rununder %t3 0 0 -1     | FileCheck %S/subscript.c --check-prefix=CHECK-FAIL-1
// RUN: %checkedc_rununder %t3 0 0 0 9    | FileCheck %S/subscript.c --check-prefix=CHECK-FAIL-1
// RUN: %checkedc_rununder %t3 0 0 0 -1   | FileCheck %S/subscript.c --check-prefix=CHECK-FAIL-1
// Skip the case for testing string literals.
// RUN: %checkedc_rununder %t3 0 0 0 0 0 5  | FileCheck %S/subscript.c --check-prefix=CHECK-FAIL-1
// RUN: %checkedc_rununder %t3 0 0 0 0 0 -1 | FileCheck %S/subscript.c --check-prefix=CHECK-FAIL-1
// RUN: %checkedc_rununder %t3 0 0 0 0 0 0  3 0   | FileCheck %S/subscript.c --check-prefix=CHECK-FAIL-2
// RUN: %checkedc_rununder %t3 0 0 0 0 0 0  2 3   | FileCheck %S/subscript.c --check-prefix=CHECK-FAIL-2
// RUN: %checkedc_rununder %t3 0 0 0 0 0 0  0 9   | FileCheck %S/subscript.c --check-prefix=CHECK-FAIL-2
// RUN: %checkedc_rununder %t3 0 0 0 0 0 0  -1 -1 | FileCheck %S/subscript.c --check-prefix=CHECK-FAIL-2
// RUN: %checkedc_rununder %t3 0 0 0 0 0 0  0 0  3 0 0    | FileCheck %S/subscript.c --check-prefix=CHECK-FAIL-3
// RUN: %checkedc_rununder %t3 0 0 0 0 0 0  0 0  2 9 0    | FileCheck %S/subscript.c --check-prefix=CHECK-FAIL-3
// RUN: %checkedc_rununder %t3 0 0 0 0 0 0  0 0  2 2 3    | FileCheck %S/subscript.c --check-prefix=CHECK-FAIL-3
// RUN: %checkedc_rununder %t3 0 0 0 0 0 0  0 0  0 0 27   | FileCheck %S/subscript.c --check-prefix=CHECK-FAIL-3
// RUN: %checkedc_rununder %t3 0 0 0 0 0 0  0 0  -1 -1 -1 | FileCheck %S/subscript.c --check-prefix=CHECK-FAIL-3
//
// RUN: %clang %S/subscript.c -DTEST_COMPOUND_ASSIGN -DPOINTER_ARITHMETIC -o %t4 -Werror -O3 %checkedc_target_flags
// RUN: %checkedc_rununder %t4 0 0 0 0 0 0  0 0   0 0 0  | FileCheck %S/subscript.c
// RUN: %checkedc_rununder %t4 1 2 4 4 2 2  1 2   1 1 1  | FileCheck %S/subscript.c
// RUN: %checkedc_rununder %t4 2 4 8 8 4 4  2 1   2 2 2  | FileCheck %S/subscript.c
// RUN: %checkedc_rununder %t4 2 4 8 8 3 3  0 4   0 3 2  | FileCheck %S/subscript.c
// RUN: %checkedc_rununder %t4 2 4 8 8 3 3  1 3   0 1 5  | FileCheck %S/subscript.c
// RUN: %checkedc_rununder %t4 2 4 8 8 3 3  2 -1  2 -1 2 | FileCheck %S/subscript.c
// RUN: %checkedc_rununder %t4 3          | FileCheck %S/subscript.c --check-prefix=CHECK-FAIL-1
// RUN: %checkedc_rununder %t4 -1         | FileCheck %S/subscript.c --check-prefix=CHECK-FAIL-1
// RUN: %checkedc_rununder %t4 0 5        | FileCheck %S/subscript.c --check-prefix=CHECK-FAIL-1
// RUN: %checkedc_rununder %t4 0 -1       | FileCheck %S/subscript.c --check-prefix=CHECK-FAIL-1
// RUN: %checkedc_rununder %t4 0 0 9      | FileCheck %S/subscript.c --check-prefix=CHECK-FAIL-1
// RUN: %checkedc_rununder %t4 0 0 -1     | FileCheck %S/subscript.c --check-prefix=CHECK-FAIL-1
// RUN: %checkedc_rununder %t4 0 0 0 9    | FileCheck %S/subscript.c --check-prefix=CHECK-FAIL-1
// RUN: %checkedc_rununder %t4 0 0 0 -1   | FileCheck %S/subscript.c --check-prefix=CHECK-FAIL-1
// Skip the case for testing string literals.
// RUN: %checkedc_rununder %t4 0 0 0 0 0 5  | FileCheck %S/subscript.c --check-prefix=CHECK-FAIL-1
// RUN: %checkedc_rununder %t4 0 0 0 0 0 -1 | FileCheck %S/subscript.c --check-prefix=CHECK-FAIL-1
// RUN: %checkedc_rununder %t4 0 0 0 0 0 0  3 0   | FileCheck %S/subscript.c --check-prefix=CHECK-FAIL-2
// RUN: %checkedc_rununder %t4 0 0 0 0 0 0  2 3   | FileCheck %S/subscript.c --check-prefix=CHECK-FAIL-2
// RUN: %checkedc_rununder %t4 0 0 0 0 0 0  0 9   | FileCheck %S/subscript.c --check-prefix=CHECK-FAIL-2
// RUN: %checkedc_rununder %t4 0 0 0 0 0 0  -1 -1 | FileCheck %S/subscript.c --check-prefix=CHECK-FAIL-2
// RUN: %checkedc_rununder %t4 0 0 0 0 0 0  0 0  3 0 0    | FileCheck %S/subscript.c --check-prefix=CHECK-FAIL-3
// RUN: %checkedc_rununder %t4 0 0 0 0 0 0  0 0  2 9 0    | FileCheck %S/subscript.c --check-prefix=CHECK-FAIL-3
// RUN: %checkedc_rununder %t4 0 0 0 0 0 0  0 0  2 2 3    | FileCheck %S/subscript.c --check-prefix=CHECK-FAIL-3
// RUN: %checkedc_rununder %t4 0 0 0 0 0 0  0 0  0 0 27   | FileCheck %S/subscript.c --check-prefix=CHECK-FAIL-3
// RUN: %checkedc_rununder %t4 0 0 0 0 0 0  0 0  -1 -1 -1 | FileCheck %S/subscript.c --check-prefix=CHECK-FAIL-3


#import <stdlib.h>

int main(void) {
  return EXIT_FAILURE;
}
