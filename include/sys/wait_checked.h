//---------------------------------------------------------------------//
// Bounds-safe interfaces for functions in sys/wait.h that             //
// take pointer arguments.                                             //
//                                                                     //
/////////////////////////////////////////////////////////////////////////

#ifdef __checkedc
#pragma CHECKED_SCOPE push
#pragma CHECKED_SCOPE off
#endif

#include_next <sys/wait.h>

#ifdef __checkedc
#pragma CHECKED_SCOPE pop
#endif

#ifdef __checkedc
#ifndef __WAIT_CHECKED_H
#define __WAIT_CHECKED_H

#pragma CHECKED_SCOPE push
#pragma CHECKED_SCOPE on

pid_t wait(int *wstatus : itype(_Ptr<int>));

#pragma CHECKED_SCOPE pop

#endif // guard
#endif // Checked C
