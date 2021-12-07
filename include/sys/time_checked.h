//---------------------------------------------------------------------//
// Bounds-safe interfaces for functions in sys/time.h that             //
// take pointer arguments.                                             //
//                                                                     //
/////////////////////////////////////////////////////////////////////////

#ifdef __checkedc
#pragma CHECKED_SCOPE push
#pragma CHECKED_SCOPE off
#endif

#include_next <sys/time.h>

#ifdef __checkedc
#pragma CHECKED_SCOPE pop
#endif

#ifdef __checkedc
#ifndef __SYS_TIME_CHECKED_H
#define __SYS_TIME_CHECKED_H

#pragma CHECKED_SCOPE push
#pragma CHECKED_SCOPE on

// The man page shows the second parameter as `struct timezone *tz`, but glibc
// has it as `void *`, and our redeclaration must be consistent with that.
// ~ Matt 2021-12-07
int gettimeofday(struct timeval * tv : itype(_Ptr<struct timeval>), void *tz : itype(_Array_ptr<void>) byte_count(sizeof(struct timezone)));
int settimeofday(const struct timeval *tv : itype(_Ptr<const struct timeval>), const struct timezone *tz : itype(_Ptr<const struct timezone>));

#pragma CHECKED_SCOPE pop

#endif // guard
#endif // Checked C
