// These are some platform-specific tests to go with
// the tests in ../function_casts.c
//
// The following line is for the LLVM test harness:
// RUN: %clang_cc1 -verify %s
//

#include <stdchecked.h>

int f0(int a) {
  return a;
}

void local_convert(int(*f1)(int), ptr<int(int)> f2) {
  // There's no good reason to do this to any function pointers
  // and it's definitely not safe.
  ptr<int(int)> local_weird_unsafe1 = (ptr<int(int)>)~(long)f1; // expected-error {{can only cast function names or null pointers to checked function pointer type '_Ptr<int (int)>'}}
  ptr<int(int)> local_weird_unsafe2 = (ptr<int(int)>)~(long)f2; // expected-error {{can only cast function names or null pointers to checked function pointer type '_Ptr<int (int)>'}}
  ptr<int(int)> local_weird_unsafe3 = (ptr<int(int)>)~(long)f0; // expected-error {{can only cast function names or null pointers to checked function pointer type '_Ptr<int (int)>'}}
  ptr<int(int)> local_weird_unsafe4 = (ptr<int(int)>)(long)!(long)f1; // expected-error {{can only cast function names or null pointers to checked function pointer type '_Ptr<int (int)>'}}
  ptr<int(int)> local_weird_unsafe5 = (ptr<int(int)>)(long)!(long)f2; // expected-error {{can only cast function names or null pointers to checked function pointer type '_Ptr<int (int)>'}}
  ptr<int(int)> local_weird_unsafe6 = (ptr<int(int)>)(long)!(long)f0; // expected-error {{can only cast function names or null pointers to checked function pointer type '_Ptr<int (int)>'}}
  ptr<int(int)> local_weird_unsafe7 = (ptr<int(int)>) + (long)f1; // expected-error {{can only cast function names or null pointers to checked function pointer type '_Ptr<int (int)>'}}
  ptr<int(int)> local_weird_unsafe8 = (ptr<int(int)>) + (long)f2; // expected-error {{can only cast function names or null pointers to checked function pointer type '_Ptr<int (int)>'}}
  ptr<int(int)> local_weird_unsafe9 = (ptr<int(int)>) + (long)f0; // expected-error {{can only cast function names or null pointers to checked function pointer type '_Ptr<int (int)>'}}
  ptr<int(int)> local_weird_unsafe10 = (ptr<int(int)>) - (long)f1; // expected-error {{can only cast function names or null pointers to checked function pointer type '_Ptr<int (int)>'}}
  ptr<int(int)> local_weird_unsafe11 = (ptr<int(int)>) - (long)f2; // expected-error {{can only cast function names or null pointers to checked function pointer type '_Ptr<int (int)>'}}
  ptr<int(int)> local_weird_unsafe12 = (ptr<int(int)>) - (long)f0; // expected-error {{can only cast function names or null pointers to checked function pointer type '_Ptr<int (int)>'}}

}