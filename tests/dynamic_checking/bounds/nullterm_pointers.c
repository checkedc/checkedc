// Test runtime bounds checking in checked scopes of uses of pointers
// and arrays with bounds-safe interfaces.
//
// RUN: %clang %s -o %t1 -Werror -Wno-unused-value -Wno-check-bounds-decls-unchecked-scope %checkedc_target_flags
// RUN: %checkedc_rununder %t1 1 | FileCheck %s --check-prefixes=CHECK,NO-BOUNDS-FAILURES-1
// RUN: %checkedc_rununder %t1 2 | FileCheck %s --check-prefixes=CHECK
// RUN: %checkedc_rununder %t1 3 | FileCheck %s --check-prefixes=CHECK
// RUN: %checkedc_rununder %t1 4 | FileCheck %s --check-prefixes=CHECK
// RUN: %checkedc_rununder %t1 5 | FileCheck %s --check-prefixes=CHECK,NO-BOUNDS-FAILURES-2
// RUN: %checkedc_rununder %t1 6 | FileCheck %s --check-prefixes=CHECK
// RUN: %checkedc_rununder %t1 7 | FileCheck %s --check-prefixes=CHECK
// RUN: %checkedc_rununder %t1 8 | FileCheck %s --check-prefixes=CHECK
// RUN: %checkedc_rununder %t1 9 | FileCheck %s --check-prefixes=CHECK,NO-BOUNDS-FAILURES-3
// RUN: %checkedc_rununder %t1 10 | FileCheck %s --check-prefixes=CHECK
//
// RUN: %checkedc_rununder %t1 21 | FileCheck %s --check-prefixes=CHECK,NO-BOUNDS-FAILURES-4
// RUN: %checkedc_rununder %t1 22 | FileCheck %s --check-prefixes=CHECK
// RUN: %checkedc_rununder %t1 23 | FileCheck %s --check-prefixes=CHECK
// RUN: %checkedc_rununder %t1 24 | FileCheck %s --check-prefixes=CHECK
// RUN: %checkedc_rununder %t1 25 | FileCheck %s --check-prefixes=CHECK,NO-BOUNDS-FAILURES-5
// RUN: %checkedc_rununder %t1 26 | FileCheck %s --check-prefixes=CHECK
// RUN: %checkedc_rununder %t1 27| FileCheck %s --check-prefixes=CHECK
// RUN: %checkedc_rununder %t1 28 | FileCheck %s --check-prefixes=CHECK

#include <assert.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdchecked.h>

struct CountedNullTermString {
  nt_array_ptr<char> s : count(len);
  int len;
};

struct CountedString {
  array_ptr<char> s : count(len);
  int len;
};

int test1(void);
int test2(int k);
void test3(void);
void test4(int k);
void test5(void);
void test6(int k);
void test7(int k);
void test8(int k);
void test9(void);
void test10(int k);

int test21(struct CountedNullTermString *p);
int test22(struct CountedString *p);
int test23(struct CountedNullTermString *p);
int test24(struct CountedString *p);
int test25(struct CountedNullTermString *p);
int test26(struct CountedString *p);
int test27(struct CountedNullTermString *p);
int test28(struct CountedString *p);

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

  char data1 nt_checked[6] = "abcde";
  char data2 checked[6] = "abcde";
  struct CountedNullTermString nullterm = { data1, 5 };
  struct CountedString plain = { data2, 5 };

  // CHECK: Beginning test
  puts("Beginning test");
  int testcase = atoi(argv[1]);
  switch (testcase) {
    case 1:
      test1();
      break;
    case 2:
      test2(0);
      break;
    case 3:
      test3();
      break;
    case 4:
      test4(6);
      break;
    case 5:
      test5();
      break;
    case 6:
      test6(6);
      break;
    case 7:
      test7(5);
      break;
    case 8:
      test8(6);
      break;
    case 9:
      test9();
      break;
    case 10:
      test10(6);
      break;

    case 21:
      test21(&nullterm);
      break;
    case 22:
      test22(&plain);
      break;
    case 23:
      test23(&nullterm);
      break;
    case 24:
      test24(&plain);
      break;
    case 25:
      test25(&nullterm);
      break;
    case 26:
      test26(&plain);
      break;
    case 27:
      test27(&nullterm);
      break;
    case 28:
      test28(&plain);
      break;

    default:
      puts("Unexpected test case");
      return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}

// Read exactly at the upper bound of a null-terminated pointer.
// Should not cause a runtime fault.
int test1(void) {
  nt_array_ptr<char> s : count(0) = "hello";
  int i = 0;
  while (*s) {
    i += *s;
    s++;
  }
  // NO-BOUNDS-FAILURES-1: no bounds failure on nt_array_ptr read at upper bound
  puts("no bounds failure on nt_array_ptr read at upper bound");
  return i;
}

// Read exactly at the upper bound of a plain array_ptr.  Expected
// to cause a runtime fault (k = 0)
int test2(int k) {
  array_ptr<char> s : count(0) = "hello";
  int i = 0;
  while (*(s+k)) {
    i += *(s+k);
    s++;
  }
  // CHECK-NOT: expected bounds failure on read
  puts("expected bounds failure on read");
  return i;
}

// Write a non-zero character at the upper bound of a string.  This
// should cause a runtime fault.
void test3(void) {
  char data nt_checked[6] = "hello";
  nt_array_ptr<char> s : count(0) = data;
  while (*s) {
    // After we have established that the value at the upper bound of an
    // nt_array_ptr is non-null, we widen its bounds by 1. So in this case, the
    // new bounds of s are (s, s+1). So (s+1) now refers to the new upper bound
    // of the string.
    *(s+1) = 'd';
    s++;
  }
  // CHECK-NOT: expected bounds failure on write
  puts("expected bounds failure on write");
  return;
}

// Write a non-zero character exactly at the upper bound of an array_ptr (k = 6).
void test4(int k) {
  char data checked[6] = "hello";
  array_ptr<char> s : count(6) = data;
  *(s + k) = 'd';
  // CHECK-NOT: expected bounds failure on write
  puts("expected bounds failure on write");
  return;
}

// Write a zero character at the upper bound of a string.  This should
// not cause a runtime fault.
void test5(void) {
  char data nt_checked[6] = "hello";
  nt_array_ptr<char> s : count(5) = data;
  s[5] = 0;
  // NO-BOUNDS-FAILURES-2: wrote nul at the upper bound of a string
  puts("wrote nul at the upper bound of a string");
  return;
}

// Write 0 at the upper bound of an array_ptr<char>.  Should cause
// a runtime fault (k = 6).
void test6(int k) {
  char data checked[6] = "hello";
  array_ptr<char> s : count(6) = data;
  char result = 0;
  s[k] = result;
  // CHECK-NOT: expected bounds failure on write
  puts("expected bounds failure on write");
  return;
}


// Write 0 at memory location one past the upper bound of a string.
// Expected to cause a runtime fault (k = 5).
void test7(int k) {
  char data nt_checked[6] = "hello";
  array_ptr<char> s : count(5) = data;
  char result = 0;
  s[k + 1] = result;
  // CHECK-NOT: expected bounds failure on write
  puts("expected bounds failure on write");
  return;
}

// Write 0 at memory location one past the upper bound of an array_ptr<char>.
// Expected to a cause a runtime fault (k = 6).
void test8(int k) {
  char data checked[6] = "hello";
  array_ptr<char> s : count(6) = data;
  char result = 0;
  s[k + 1] = result;
  // CHECK-NOT: expected bounds failure on write
  puts("expected bounds failure on write");
  return;
}

// Write 0 exactly at upper bound of a range for a null-terminated pointer,
// when the lower bound == the upper bound.  Expected to succeed.
void test9(void) {
  char data nt_checked[6] = "hello";
  nt_array_ptr<char> s : bounds(data + 5, data + 5) = data;
  s[5] = 0;
  // NO-BOUNDS-FAILURES-3: expected write at range with lower == upper to succeed
  puts("expected write at range with lower == upper to succeed");
  return;
}


// Write 0 at the upper bound of a range for a null-terminated pointer,  where
// the lower bound is above the upper bound.  Expected to cause a runtime fault (k = 6).
void test10(int k) {
  char data nt_checked[6] = "hello";
  nt_array_ptr<char> s : bounds(data + 6, data + 5) = data;
  s[k] = 0;
  // CHECK-NOT: expected bounds failure on write
  puts("expected bounds failure on write");
  return;
}

// Read exactly at the upper bound of a null-terminated pointer.
// Should not cause a runtime fault.
int test21(struct CountedNullTermString *p) {
  if (p->s[p->len])
    // CHECK-NOT: expected null terminator
    puts("expected null terminator");
  else
    // NO-BOUNDS-FAILURES-4: found null terminator at nt_array_ptr upper bound
    puts("found null terminator at nt_array_ptr upper bound");
  return 0;
}

// Read exactly at the upper bound of an array_ptr.  Expected
// to cause a runtime fault.
int test22(struct CountedString *p) {
  char result = p->s[p->len];
  // CHECK-NOT: expected bounds failure on read
  puts("expected bounds failure on read");
  return result;
}

// Write a non-zero value at exactly the upper bound of a string.  Should not
// cause a runtime fault.
int test23(struct CountedNullTermString *p) {
  char result = 'a';
  p->s[p->len] = result;
  // CHECK-NOT: expected bounds failure on write
  puts("expected bounds failure on write");
  return result;
}

// Write a non-zero value at exactly the upper bound of an array_ptr<char>.
// Expected to a cause a runtime fault.
int test24(struct CountedString *p) {
  char result = 'a';
  p->s[p->len] = result;
  // CHECK-NOT: expected bounds failure on write
  puts("expected bounds failure on write");
  return result;
}

// Write 0 at exactly the upper bound of a string.  Not expected to cause a runtime
// fault.
int test25(struct CountedNullTermString *p) {
  char result = 0;
  p->s[p->len] = result;
  // NO-BOUNDS-FAILURES-5: wrote nul at the upper bound of a string
  puts("wrote nul at the upper bound of a string");
  return result;
}

// Write 0 at exactly the upper bound of an array_ptr<char>.
int test26(struct CountedString *p) {
  char result = 0;
  p->s[p->len] = result;
  // CHECK-NOT: expected bounds failure on write
  puts("expected bounds failure on write");
  return result;
}


// Write 0 at memory location one past the upper bound of a string.  Expected
// to a cause a runtime fault.
int test27(struct CountedNullTermString *p) {
  char result = 0;
  p->s[p->len + 1] = result;
  // CHECK-NOT: expected bounds failure on write
  puts("expected bounds failure on write");
  return result;
}

// Write 0 at memory location one past the upper bound of a character array.
// Expected to a cause a runtime fault.
int test28(struct CountedString *p) {
  char result = 0;
  p->s[p->len + 1] = result;
  // CHECK-NOT: expected bounds failure on write
  puts("expected bounds failure on write");
  return result;
}
