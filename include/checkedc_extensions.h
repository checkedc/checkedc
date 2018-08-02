//--------------------------------------------------------------------------//
// Additional bounds-safe interface options for functions that have         //
// different interfaces for array_ptr and nt_array_ptr args. Programmer can //
// choose to use these over the default bounds-safe interfaces.             //
//////////////////////////////////////////////////////////////////////////////

#ifndef __CHECKED_C_EXTENSIONS_H
#define __CHECKED_C_EXTENSIONS_H

#include <stdlib_checked.h>
#include <string_checked.h>

// default strncmp has a bounds-safe interface nt_array_ptr;
// this option is for array_ptr
extern inline int strncmp_array_ptr(const char *src : count(n), const char *s2 : count(n), size_t n) {
  _Unchecked { return strncmp(src, s2, n); }
}

#endif /* __CHECKED_C_EXTENSIONS_H */
