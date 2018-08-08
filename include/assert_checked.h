//---------------------------------------------------------------------//
// Bounds-safe interfaces for functions in stdlib.h that               //
// take pointer arguments.                                             //
//                                                                     //
// These are listed in the same order that they occur in the C11       //
// specification.                                                      //
/////////////////////////////////////////////////////////////////////////


#include <assert.h>

#ifndef __cplusplus
#ifndef __ASSERT_CHECKED_H
#define __ASSERT_CHECKED_H

#pragma CHECKED_SCOPE ON

#if defined(_WIN32) || defined(_WIN64)
_ACRTIMP void __cdecl _wassert(_In_z_ wchar_t const *_Message : itype(_Nt_array_ptr<const wchar_t>),
                               _In_z_ wchar_t const *_File : itype(_Nt_array_ptr<const wchar_t>),
                               _In_ unsigned _Line);
#else
extern void __assert(const char *msg : itype(_Nt_array_ptr<const char>), 
					 const char *file : itype(_Nt_array_ptr<const char>), 
					 int line);

#endif

#pragma CHECKED_SCOPE OFF

#endif  // guard
#endif  // no c++
