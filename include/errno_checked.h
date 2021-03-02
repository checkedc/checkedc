//--------------------------------------------------------------------//
// Bounds-safe interfaces for functions in errno.h                    //
//                                                                    //
////////////////////////////////////////////////////////////////////////

#ifndef __cplusplus
#pragma CHECKED_SCOPE push
#pragma CHECKED_SCOPE off
#endif

#include_next <errno.h>

#ifndef __cplusplus
#pragma CHECKED_SCOPE pop
#endif

#ifndef __cplusplus
#ifndef __ERRNO_CHECKED_H
#define __ERRNO_CHECKED_H

#pragma CHECKED_SCOPE push
#pragma CHECKED_SCOPE ON

#if defined(_WIN32) || defined(_WIN64)
__declspec(dllimport) int* __cdecl _errno(void) : itype(_Ptr<int>);
#elif defined(__APPLE__)
extern int* __errno_location(void) : itype(_Ptr<int>);
#else
extern int* __errno_location(void) : itype(_Ptr<int>) __THROW __attribute_const__;
#endif

#pragma CHECKED_SCOPE pop

#endif // guards
#endif // c++
