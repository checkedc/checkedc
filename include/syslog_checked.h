//---------------------------------------------------------------------//
// Bounds-safe interfaces for functions in syslog.h that               //
// take pointer arguments.                                             //
//                                                                     //
/////////////////////////////////////////////////////////////////////////

// The Windows environment may not have syslog.h
#if defined __has_include_next && __has_include_next(<syslog.h>)

#ifdef __checkedc
#pragma CHECKED_SCOPE push
#pragma CHECKED_SCOPE off
#endif

#include_next <syslog.h>

#ifdef __checkedc
#pragma CHECKED_SCOPE pop
#endif

#ifdef __checkedc
#ifndef __SYSLOG_CHECKED_H
#define __SYSLOG_CHECKED_H

#pragma CHECKED_SCOPE push
#pragma CHECKED_SCOPE on

void closelog(void);
void openlog (const char *ident : itype(_Nt_array_ptr<const char>), int option, int facility);

// TODO: Is this conditional right? There are two considerations:
//
// 1. Precedent in the Checked C system headers. I don't see any cases of
//    __foo_chk without __builtin___foo_chk, but this is my best guess of what
//    would be considered consistent.
// 2. What's actually needed on the OSes we try to support (Linux, Windows, Mac
//    OS), though we might go with a solution that is more complex than
//    necessary if it's consistent with precedent.
#if _FORTIFY_SOURCE == 0 || !defined(syslog)
#undef syslog
_Unchecked
void syslog(int priority, const char * format : itype(_Nt_array_ptr<const char>), ...);
#else
_Unchecked
void __syslog_chk(int priority, int flag, const char * format : itype(_Nt_array_ptr<const char>), ...);
#endif

// TODO: Can we assume that va_list has been included via the `#include_next
// <syslog.h>`, analogous to the situation in stdio_checked.h?
//
// TODO: The same questions about the conditional as for `syslog`.
#if _FORTIFY_SOURCE == 0 || !defined(vsyslog)
#undef vsyslog
_Unchecked
void vsyslog(int priority, const char * format : itype(_Nt_array_ptr<const char>), va_list ap);
#else
_Unchecked
void __vsyslog_chk(int priority, int flag, const char * format : itype(_Nt_array_ptr<const char>), va_list ap);
#endif

#pragma CHECKED_SCOPE pop

#endif // guard
#endif // Checked C

#else // doesn't have syslog.h
#error "cannot include 'syslog_checked.h' because this system does not have the original 'syslog.h'"
#endif
