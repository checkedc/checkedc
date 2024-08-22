//--------------------------------------------------------------------//
// Bounds-safe interfaces for functions in errno.h                    //
//                                                                    //
////////////////////////////////////////////////////////////////////////

#ifdef __checkedc
#pragma CHECKED_SCOPE push
#pragma CHECKED_SCOPE off
#endif

#include_next <errno.h>

#ifdef __checkedc
#pragma CHECKED_SCOPE pop
#endif

#ifdef __checkedc
#ifndef __ERRNO_CHECKED_H
#define __ERRNO_CHECKED_H

#pragma CHECKED_SCOPE push
#pragma CHECKED_SCOPE on

#if defined(_WIN32) || defined(_WIN64)
__declspec(dllimport) int* __cdecl _errno(void) : itype(_Ptr<int>);
#elif defined(__APPLE__)
#if defined(__aarch64__)
extern int * __error(void) : itype(_Ptr<int>);
#else
extern int* __errno_location(void) : itype(_Ptr<int>);
#endif
#else
extern int* __errno_location(void) : itype(_Ptr<int>) __THROW __attribute_const__;
#endif

#pragma CHECKED_SCOPE pop

#endif // guards
#endif // Checked C
