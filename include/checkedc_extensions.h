//--------------------------------------------------------------------------//
// Additional bounds-safe interface options for functions that have         //
// different interfaces for array_ptr and nt_array_ptr args. Programmer can //
// choose to use these over the default bounds-safe interfaces.             //
//////////////////////////////////////////////////////////////////////////////

#include <stdarg.h>
#include <stdlib_checked.h>
#include <string_checked.h>

#ifdef __checkedc

#ifndef __CHECKED_C_EXTENSIONS_H
#define __CHECKED_C_EXTENSIONS_H

#pragma CHECKED_SCOPE push
#pragma CHECKED_SCOPE on

// default strncmp has a bounds-safe interface nt_array_ptr;
// this option is for array_ptr
inline int __attribute__((__always_inline__))
strncmp_array_ptr(const char *src : count(n), const char *s2 : count(n), size_t n) {
  _Unchecked { return strncmp(src, s2, n); }
}

// default snprintf assumes nt_array_ptr for bounds-safe interface
// this option is for array_ptr
// clang does not inline functions that use va_list/va_start/va_end to
// access variable number of arguments.
_Unchecked static int
snprintf_array_ptr(char * restrict s : itype(restrict _Array_ptr<char>) count(n),
                       size_t n,
                       const char * restrict format : itype(restrict _Nt_array_ptr<const char>),
                       ...) {
  va_list argptr;
  va_start(argptr, format);
  // The call to snprintf needs to be in an unchecked block as the compiler,
  // in checked scope, will not allow passing an argument of type
  // _Array_ptr<char> count(n) whose corresponding parameter has the type
  // _Nt_array_ptr<char> count(n-1).
  snprintf(s, n, format, argptr);
  va_end(argptr);
}

#pragma CHECKED_SCOPE pop

#endif // guard 
#endif // Checked C 
