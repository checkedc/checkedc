//---------------------------------------------------------------------//
// Bounds-safe interfaces for a functions in signal.h that             //
// take pointer arguments.                                             //
/////////////////////////////////////////////////////////////////////////

#include <signal.h>

#pragma BOUNDS_CHECKED ON

_Unchecked
void (*signal(int sig,
              void ((*func)(int)) :
                itype(_Ptr<void (int)>) // bound-safe interface for func
              ) : itype(_Ptr<void (int)>) // bounds-safe interface for signal return
     )(int);

#pragma BOUNDS_CHECKED OFF
