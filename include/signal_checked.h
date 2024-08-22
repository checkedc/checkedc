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

_Unchecked
void (*sigset(int sig,
              void ((*func)(int)) :
                itype(_Ptr<void (int)>) // bound-safe interface for func
              ) : itype(_Ptr<void (int)>) // bounds-safe interface for signal return
     )(int);

#ifdef _POSIX_C_SOURCE
int sigemptyset(sigset_t *set : itype(_Ptr<sigset_t>));
int sigfillset(sigset_t *set : itype(_Ptr<sigset_t>));
int sigaddset(sigset_t *set : itype(_Ptr<sigset_t>), int signum);
int sigdelset(sigset_t *set : itype(_Ptr<sigset_t>), int signum);
int sigismember(const sigset_t *set : itype(_Ptr<const sigset_t>), int signum);

int sigaction(int signum, const struct sigaction *act : itype(_Ptr<const struct sigaction>),
              struct sigaction *oldact : itype(_Ptr<struct sigaction>));


int sigprocmask(int how, const sigset_t *set : itype(_Ptr<const sigset_t>), sigset_t *oldset : itype(_Ptr<sigset_t>));
#endif // _POSIX_C_SOURCE

#pragma CHECKED_SCOPE pop

#endif // guard
#endif // Checked C
