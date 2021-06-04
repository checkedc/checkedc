// Tests for calling functions declared in checkedc_extensions.h
//
// The following lines are for the LLVM test harness:
//
// RUN: %clang -o %t.exe %s %checkedc_target_flags
// RUN: %checkedc_rununder %t.exe

// expected-no-diagnostics

#pragma CHECKED_SCOPE on

#include <stddef.h>
#include <checkedc_extensions.h>
#include <stdio.h>

// A function with an interface similar to snprintf but without the variable
// number of arguments so that such a call interface can be tested in checked
// scope.
void iface(char * restrict s : itype(restrict _Nt_array_ptr<char>) count(n-1),
           size_t n,
           const char * restrict src : itype(restrict _Nt_array_ptr<const char>)) {
    return;
}



void iface_test1(void) {
  char buf _Nt_checked[50];
  iface(buf, 50, "Hello world - 1");
}

void iface_test2(_Nt_array_ptr<char> buf : count(len), size_t len) {
  iface(buf, len + 1, "Hello world - 2");
}

void test3(_Array_ptr<char> buf : count(len), size_t len) {
  _Unchecked{ snprintf_array_ptr(buf, len, "Hello world - 3"); }
}

int main(void) {
  char buf _Checked[50];
  test3(buf, 50) ;
  char buf1 _Checked[50];
  test3(buf1, 50) ;
  int res = strncmp_array_ptr(buf, buf1, 50);
  _Unchecked {printf("res: %d  buf: %s  buf1: %s\n", res, buf, buf1);}
}
