//---------------------------------------------------------------------//
// Bounds-safe interfaces for a functions in signal.h that             //
// take pointer arguments.                                             //
/////////////////////////////////////////////////////////////////////////

#include <signal.h>

#ifndef __cplusplus
#ifndef __SIGNAL_CHECKED_H
#define __SIGNAL_CHECKED_H

#pragma CHECKED_SCOPE ON

_Unchecked
void (*signal(int sig,
              void ((*func)(int)) :
                itype(_Ptr<void (int)>) // bound-safe interface for func
              ) : itype(_Ptr<void (int)>) // bounds-safe interface for signal return
     )(int);

#pragma CHECKED_SCOPE OFF

#endif
#endif
