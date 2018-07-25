//--------------------------------------------------------------------//
// Bounds-safe interfaces for functions in errno.h                    //
//                                                                    //
////////////////////////////////////////////////////////////////////////

#include <errno.h>

#pragma CHECKED_SCOPE ON

extern int* __errno_location(void) : itype(_Ptr<int>) __THROW __attribute_const__;

#pragma CHECKED_SCOPE OFF
