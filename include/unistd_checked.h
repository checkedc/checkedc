//---------------------------------------------------------------------//
// Bounds-safe interfaces for functions in unistd.h that               //
// take pointer arguments.                                             //
//                                                                     //
// These are POSIX-only                                                //
/////////////////////////////////////////////////////////////////////////

#include <unistd.h>

#pragma BOUNDS_CHECKED ON

#if _POSIX_VERSION==200112L

extern char ** environ : itype(_Nt_array_ptr<_Nt_array_ptr<char>>);

#endif

#pragma BOUNDS_CHECKED OFF
