// Test runtime bounds checking in checked scopes of uses of pointers
// and arrays with bounds-safe interfaces.
//
// RUN: %clang -fcheckedc-extension %s -o %t1 -Werror -Wno-unused-value -Wno-check-memory-accesses
// RUN:  %t1 1 | FileCheck %s --check-prefixes=CHECK,NO-BOUNDS-FAILURES-1
// RUN:  %t1 2 | FileCheck %s --check-prefixes=CHECK
// RUN:  %t1 3 | FileCheck %s --check-prefixes=CHECK
// RUN:  %t1 4 | FileCheck %s --check-prefixes=CHECK
//
// RUN:  %t1 21 | FileCheck %s --check-prefixes=CHECK,NO-BOUNDS-FAILURES-2
// RUN:  %t1 22 | FileCheck %s --check-prefixes=CHECK
// RUN:  %t1 23 | FileCheck %s --check-prefixes=CHECK
// RUN:  %t1 24 | FileCheck %s --check-prefixes=CHECK

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
int test2(void);
void test3(void);
void test4(void);

int test21(struct CountedNullTermString *p);
int test22(struct CountedString *p);
int test23(struct CountedNullTermString *p);
int test24(struct CountedString *p);


// Handle an out-of-bounds reference by immediately exiting. This causes
// some output to be missing.
void handle_error(int err) {
  _Exit(0);
}




// This signature for main is exactly what we want here,
// it also means any uses of argv[i] are checked too!
int main(int argc, array_ptr<char*> argv : count(argc)) {

  // Set up the handler for a failing bounds check.  Currently the Checked C
  // clang implementation raises a SIGILL when a bounds check fails.  This
  // may change in the future.
  signal(SIGILL, handle_error);

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

  struct CountedNullTermString nullterm = { "abcde", 5 };
  struct CountedString plain = { "abcde", 5 };

  // CHECK: Beginning test
  puts("Beginning test");
  int testcase = atoi(argv[1]);
  switch (testcase) {
    case 1: 
      test1();
      break;
    case 2:
      test2();
      break;
    case 3:
      test3();
      break;
    case 4:
      test4();
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
// to cause a runtime fault.
int test2(void) {
  array_ptr<char> s : count(0) = "hello";
  int i = 0;
  while (*s) {
    i += *s;
    s++;
  }
  // CHECK-NOT: expected bounds failure on read
  puts("expected bounds failure on read");
  return i;
}

void test3(void) {
  nt_array_ptr<char> s : count(0) = "hello";
  while (*s) {
    *s = 'd';
  }
  // CHECK-NOT: expected bounds failure on write
  puts("expected bounds failure on write");
  return;
}

void test4(void) {
  array_ptr<char> s : count(0) = "hello";
  while (*s) {
    *s = 'd';
  }
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
    // NO-BOUNDS-FAILURES-2: found null terminator at nt_array_ptr upper bound  
    puts("found null terminator at nt_array_ptr upper bound");
  return 0;
}

// Read exactly at the upper bound of a plain array_ptr.  Expected
// to cause a runtime fault.
int test22(struct CountedString *p) {
  char result = p->s[p->len];
  // CHECK-NOT: expected bounds failure on read
  puts("expected bounds failure on read");
  return result;
}

int test23(struct CountedNullTermString *p) {
  char result = 'a';
  p->s[p->len] = result;
  // CHECK-NOT: expected bounds failure on write
  puts("expected bounds failure on write");
  return result;
}

int test24(struct CountedString *p) {
  char result = 'a';
  p->s[p->len] = result;
  // CHECK-NOT: expected bounds failure on write
  puts("expected bounds failure on write");
  return result;
}