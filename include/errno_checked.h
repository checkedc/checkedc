//--------------------------------------------------------------------//
// Bounds-safe interfaces for functions in errno.h                    //
//                                                                    //
////////////////////////////////////////////////////////////////////////

#include <errno.h>

#ifndef __cplusplus
#ifndef __ERRNO_CHECKED_H
#define __ERRNO_CHECKED_H


#pragma CHECKED_SCOPE ON

#if defined(_WIN32) || defined(_WIN64)
__declspec(dllimport) int* __cdecl _errno(void) : itype(_Ptr<int>);
#else
extern int* __errno_location(void) : itype(_Ptr<int>) __THROW __attribute_const__;
#endif

#pragma CHECKED_SCOPE OFF

#endif // guards
#endif // c++
