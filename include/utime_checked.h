//---------------------------------------------------------------------//
// Bounds-safe interfaces for functions in utime.h that                 //
// take pointer arguments.                                             //
//                                                                     //
// These are listed in the same order that they occur in the C11       //
// specification.                                                      //
/////////////////////////////////////////////////////////////////////////

#ifdef __checkedc
#pragma CHECKED_SCOPE push
#pragma CHECKED_SCOPE off
#endif

#include_next <utime.h>

#ifdef __checkedc
#pragma CHECKED_SCOPE pop
#endif

#ifdef __checkedc
#ifndef __UTIME_CHECKED_H
#define __UTIME_CHECKED_H

#pragma CHECKED_SCOPE push
#pragma CHECKED_SCOPE on

extern int utime (const char *file : itype(_Nt_array_ptr<const char>),
		  const struct utimbuf *file_times : itype(_Ptr<const struct utimbuf>));

#pragma CHECKED_SCOPE pop

#endif // guard
#endif // Checked C
