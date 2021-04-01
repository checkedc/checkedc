//---------------------------------------------------------------------//
// Bounds-safe interfaces for functions in inttypes.h that             //
// take pointer arguments.                                             //
//                                                                     //
// These are listed in the same order that they occur in the C11       //
// specification.                                                      //
/////////////////////////////////////////////////////////////////////////

#ifdef __checkedc
#pragma CHECKED_SCOPE push
#pragma CHECKED_SCOPE off
#endif

#include <stddef.h> // define wchar_t for wcstoimax and wcstoumax

#ifdef __checkedc
#pragma CHECKED_SCOPE pop
#endif

#ifdef __checkedc
#ifndef __INTTYPES_CHECKED_H
#define __INTTYPES_CHECKED_H

#pragma CHECKED_SCOPE push
#pragma CHECKED_SCOPE on

_Unchecked
intmax_t strtoimax(const char * restrict nptr :
                     itype(restrict _Nt_array_ptr<const char>),
                   char ** restrict endptr :
                      itype(restrict _Ptr<_Nt_array_ptr<char>>),
                   int base);
_Unchecked
uintmax_t strtoumax(const char * restrict nptr :
                      itype(restrict _Nt_array_ptr<const char>),
                    char ** restrict endptr :
                      itype(restrict _Ptr<_Nt_array_ptr<char >>),
                    int base);

_Unchecked
intmax_t wcstoimax(const wchar_t * restrict nptr :
                     itype(restrict _Nt_array_ptr<const wchar_t>),
                   wchar_t ** restrict endptr :
                     itype(restrict _Ptr<_Nt_array_ptr<wchar_t>>),
                   int base);

_Unchecked
uintmax_t wcstoumax(const wchar_t * restrict nptr :
                      itype(restrict _Nt_array_ptr<const wchar_t>),
                    wchar_t ** restrict endptr :
                      itype(restrict _Ptr<_Nt_array_ptr<wchar_t>>),
                    int base);

#pragma CHECKED_SCOPE pop

#endif // guard
#endif // Checked C
