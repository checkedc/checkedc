// Test bounds checking in checked scopes of uses of
// arrays that have declared bounds.  The declared bounds
// override bounds based on the size of the 1st
// dimension of the array.
//
// RUN: %clang %s -o %t1 -Werror -Wno-unused-value %checkedc_target_flags
// RUN: %checkedc_rununder %t1 0 | FileCheck %s --check-prefixes=CHECK,NO-BOUNDS-FAILURES
// RUN: %checkedc_rununder %t1 1 | FileCheck %s --check-prefixes=CHECK
// RUN: %checkedc_rununder %t1 2 | FileCheck %s --check-prefixes=CHECK
// RUN: %checkedc_rununder %t1 3 | FileCheck %s --check-prefixes=CHECK
// RUN: %checkedc_rununder %t1 4 | FileCheck %s --check-prefixes=CHECK
// RUN: %checkedc_rununder %t1 5 | FileCheck %s --check-prefixes=CHECK
// RUN: %checkedc_rununder %t1 6 | FileCheck %s --check-prefixes=CHECK
// RUN: %checkedc_rununder %t1 7 | FileCheck %s --check-prefixes=CHECK
// RUN: %checkedc_rununder %t1 8 | FileCheck %s --check-prefixes=CHECK
// RUN: %checkedc_rununder %t1 9 | FileCheck %s --check-prefixes=CHECK
// RUN: %checkedc_rununder %t1 10 | FileCheck %s --check-prefixes=CHECK
// RUN: %checkedc_rununder %t1 11 | FileCheck %s --check-prefixes=CHECK
// RUN: %checkedc_rununder %t1 12 | FileCheck %s --check-prefixes=CHECK

#include <assert.h>
#include <signal.h>
#include <stdio_checked.h>
#include <string.h>
#include <stdlib_checked.h>
#include <stdchecked.h>

// Handle an out-of-bounds reference by immediately exiting. This causes
// some output to be missing.
void handle_error(int err) {
  _Exit(0);
}

int global_arr1 checked[5] : count(4);
int global_arr2_len = 3;
int global_arr2 checked[5] : count(global_arr2_len);
int global_arr3[5] : itype(int checked[5]);

struct S1 {
  int arr checked[5] : count(4);
};

struct S2 {
  int arr checked[5] : count(len);
  int len;
};

struct S3 {
  int len;
  int arr[] : count(len);  // bounds-safe interface.
};

struct S2 gp_s2 = { { 5, 4, 3, 2, 1}, 3 };

int read_pass(ptr<struct S1> s1, ptr<struct S3> s3);
int write_pass(ptr<struct S1> s1, ptr<struct S3> s3);
int read_fail(int testnum, int pos, ptr<struct S1> s, ptr<struct S3> s3);
int write_fail(int testnum, int pos, ptr<struct S1> s, ptr<struct S3> s3);

// This signature for main is exactly what we want here,
// it also means any uses of argv[i] are checked too!
int main(int argc, array_ptr<char*> argv : count(argc)) {

  // Set up the handler for a failing bounds check.  Currently the Checked C
  // clang implementation raises a SIGILL or SIGTRIP when a bounds check fails,
  // dpending on the target OS.  This may change in the future.
  signal(SIGILL, handle_error);
#if defined(__APPLE__) && defined(__aarch64__)
  signal(SIGTRAP, handle_error);
#endif

  // This makes sure output is not buffered for when
  // we hit errors.
  int err = setvbuf(stdout, NULL, _IONBF, 0);
  if (err) {
    // CHECK-NOT: Error Setting Up Buffering
    puts("Error Setting Up Buffering");
    return EXIT_FAILURE;
  }

  if (argc < 2) {
    // CHECK-NOT: Requires Argument
    puts("Requires Argument");
    return EXIT_FAILURE;
  }

  // CHECK: Starting test
  puts("Starting test");
  struct S1 s1 = { 5, 4, 3, 2, 1 };
  ptr<struct S3> s3 =
    malloc(sizeof(struct S3) + sizeof(int) * 4);
  s3->len = 4;

  int target = atoi(argv[1]);
  if (target == 0) {
    int write_result = write_pass(&s1, s3);
    // NO-BOUNDS-FAILURES: No bounds failures on write
    puts("No bounds failures on write");

    int read_result = read_pass(&s1, s3);
    // NO-BOUNDS-FAILURES: No bounds failures on read
    puts("No bounds failures on read");

    if (write_result != read_result) {
      puts("Mismatch value between read and write");
      return EXIT_FAILURE;
    }
  }
  if (target >= 1 && target <= 7) {
    int pos = atoi(argv[2]);
    read_fail(target, pos, &s1, s3);
    // CHECK-NOT: Expected bounds failure
    puts("Expected read bounds failure");
  } else   if (target >= 8 && target <= 14) {
    int pos = atoi(argv[2]);
    write_fail(target, pos, &s1, s3);
    // CHECK-NOT: Expected write bounds failure
    puts("Expected write bounds failure");
  }

  return EXIT_SUCCESS;
}

int write_pass(ptr<struct S1> s1, ptr<struct S3> s3) checked {
  global_arr1[3] = 3;
  global_arr2[1] = 4;
  global_arr3[2] = 5;
  s1->arr[2] = 6;
  gp_s2.arr[2] = 7;
  s3->arr[1] = 8;
  return 3 + 4 + 5 + 6 + 7 + 8;
}

int read_pass(ptr<struct S1> s1, ptr<struct S3> s3) checked {
  int result = global_arr1[3];
  result += global_arr2[1];
  result += global_arr3[2];
  result += s1->arr[2];
  result += gp_s2.arr[2];
  result += s3->arr[1];
  return result;
}

int read_fail(int testnum, int pos, ptr<struct S1> s, ptr<struct S3> s3) checked {
  int result = 0;
  switch (testnum) {
    case 1:
      result = global_arr1[pos]; // pos = 4 causes failure
      goto unexpected_success;
    case 2:
      result = global_arr2[pos]; // pos = -1 causes failure
      goto unexpected_success;
    case 3:
      result = global_arr2[pos]; // pos = 3 causes failure
      goto unexpected_success;
    case 4:
      result = global_arr3[pos]; // pos = 5 causes failure
      goto unexpected_success;
    case 5:
      result = gp_s2.arr[pos];   // pos = 4 causes failure
      goto unexpected_success;
    case 6:
      result = *(gp_s2.arr + pos); // pos = 3 causes failure.
      goto unexpected_success;
    case 7:
      result = s3->arr[pos];       // pos = 5 causes failure.
      goto unexpected_success;
    default:
      break;
  }
  // CHECK-NOT: Unknown read-fail test case
  puts("Unknown read-fail test case");
  exit(EXIT_FAILURE);
  return result;

unexpected_success:
  // CHECK-NOT: Read did not fail as expected
  puts("Read did not fail as expected");
  exit(EXIT_FAILURE);
  return result;
}

int write_fail(int testnum, int pos, ptr<struct S1> s,
               ptr<struct S3> s3) checked {
  int result = 0;
  switch (testnum) {
    case 8:
      global_arr1[pos] = 5;  // pos = 4 causes failure
      goto unexpected_success;
    case 9:
      global_arr2[pos] = 5;  // pos = -1 causes failure
      goto unexpected_success;
    case 10:
      global_arr2[pos] = 5;  // pos = 3 causes failure
      goto unexpected_success;
    case 11:
      global_arr3[pos] = 5;  // pos = 5 causes failure
      goto unexpected_success;
    case 12:
      gp_s2.arr[pos] = 5;    // pos = 4 causes failure
      goto unexpected_success;
    case 13:
      *(gp_s2.arr + pos) = 5; // pos = 3 causes failure.
      goto unexpected_success;
    case 14:
      s3->arr[pos] = 5;     // pos = 4 causes failure.
      goto unexpected_success;
    default:
      break;
  }
  // CHECK-NOT: Unknown write-fail test case
  puts("Unknown wrote-fail test case");
  exit(EXIT_FAILURE);
  return result;

unexpected_success:
  // CHECK-NOT: Read did not fail as expected
  puts("Read did not fail as expected");
  exit(EXIT_FAILURE);
  return result;
}
