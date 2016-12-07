//---------------------------------------------------------------------//
// Bounds-safe interfaces for a functions in signal.h that             //
// take pointer arguments.                                             //
/////////////////////////////////////////////////////////////////////////

#include <signal.h>

void (*signal(int sig, void (*func)(int) : itype(_Ptr<void (int)>)))(int) :
   itype(_Ptr<void (int)>);
