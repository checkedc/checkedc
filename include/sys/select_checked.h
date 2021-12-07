//---------------------------------------------------------------------//
// Bounds-safe interfaces for functions in sys/select.h that           //
// take pointer arguments.                                             //
//                                                                     //
/////////////////////////////////////////////////////////////////////////

#ifdef __checkedc
#pragma CHECKED_SCOPE push
#pragma CHECKED_SCOPE off
#endif

#include_next <sys/select.h>

#ifdef __checkedc
#pragma CHECKED_SCOPE pop
#endif

#ifdef __checkedc
#ifndef __SELECT_CHECKED_H
#define __SELECT_CHECKED_H

#pragma CHECKED_SCOPE push
#pragma CHECKED_SCOPE on

int select(int nfds, fd_set *readfds : itype(_Ptr<fd_set>), fd_set *writefds : itype(_Ptr<fd_set>), fd_set *exceptfds : itype(_Ptr<fd_set>), struct timeval *timeout : itype(_Ptr<struct timeval>));

#pragma CHECKED_SCOPE pop

#endif // guard
#endif // Checked C
