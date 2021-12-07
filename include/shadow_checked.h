//---------------------------------------------------------------------//
// Bounds-safe interfaces for functions in shadow.h that               //
// take pointer arguments.                                             //
//                                                                     //
/////////////////////////////////////////////////////////////////////////

#ifdef __checkedc
#pragma CHECKED_SCOPE push
#pragma CHECKED_SCOPE off
#endif

#include_next <shadow.h>

#ifdef __checkedc
#pragma CHECKED_SCOPE pop
#endif

#ifdef __checkedc
#ifndef __SHADOW_CHECKED_H
#define __SHADOW_CHECKED_H

#pragma CHECKED_SCOPE push
#pragma CHECKED_SCOPE on

struct spwd *getspnam(const char *name : itype(_Nt_array_ptr<const char>)) : itype(_Ptr<struct spwd>);

#pragma CHECKED_SCOPE pop

#endif // guard
#endif // Checked C
