//---------------------------------------------------------------------//
// Bounds-safe interfaces for functions in grp.h that                  //
// take pointer arguments.                                             //
//                                                                     //
// These are listed in the same order that they occur in the C11       //
// specification.                                                      //
/////////////////////////////////////////////////////////////////////////

#ifdef __checkedc
#pragma CHECKED_SCOPE push
#pragma CHECKED_SCOPE off
#endif

#include_next <grp.h>

#ifdef __checkedc
#pragma CHECKED_SCOPE pop
#endif

#ifdef __checkedc
#ifndef __GRP_CHECKED_H
#define __GRP_CHECKED_H

#pragma CHECKED_SCOPE push
#pragma CHECKED_SCOPE on

int initgroups(const char *user : itype(_Nt_array_ptr<const char>), gid_t group);

#pragma CHECKED_SCOPE pop

#endif // guard
#endif // Checked C
