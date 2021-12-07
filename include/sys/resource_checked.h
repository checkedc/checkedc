//---------------------------------------------------------------------//
// Bounds-safe interfaces for functions in sys/resource.h that         //
// take pointer arguments.                                             //
//                                                                     //
/////////////////////////////////////////////////////////////////////////

#ifdef __checkedc
#pragma CHECKED_SCOPE push
#pragma CHECKED_SCOPE off
#endif

#include_next <sys/resource.h>

#ifdef __checkedc
#pragma CHECKED_SCOPE pop
#endif

#ifdef __checkedc
#ifndef __RESOURCE_CHECKED_H
#define __RESOURCE_CHECKED_H

#pragma CHECKED_SCOPE push
#pragma CHECKED_SCOPE on

int getrlimit(int resource, struct rlimit *rlim : itype(_Ptr<struct rlimit>));
int setrlimit(int resource, const struct rlimit *rlim : itype(_Ptr<const struct rlimit>));

#pragma CHECKED_SCOPE pop

#endif // guard
#endif // Checked C
