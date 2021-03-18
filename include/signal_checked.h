//---------------------------------------------------------------------//
// Bounds-safe interfaces for a functions in signal.h that             //
// take pointer arguments.                                             //
/////////////////////////////////////////////////////////////////////////

#ifdef __checkedc
#pragma CHECKED_SCOPE push
#pragma CHECKED_SCOPE off
#endif

#include_next <signal.h>

#ifdef __checkedc
#pragma CHECKED_SCOPE pop
#endif

#ifdef __checkedc
#ifndef __SIGNAL_CHECKED_H
#define __SIGNAL_CHECKED_H

#pragma CHECKED_SCOPE push
#pragma CHECKED_SCOPE on

_Unchecked
void (*signal(int sig,
              void ((*func)(int)) :
                itype(_Ptr<void (int)>) // bound-safe interface for func
              ) : itype(_Ptr<void (int)>) // bounds-safe interface for signal return
     )(int);

#pragma CHECKED_SCOPE pop

#endif // guard
#endif // Checked C
