//---------------------------------------------------------------------//
// Bounds-safe interfaces for some functions in arpa/inet.h that       //
// take pointer arguments.                                             //
//                                                                     //
// These are POSIX-only                                                //
/////////////////////////////////////////////////////////////////////////

// The Windows environment may not have arpa/inet.h
#if defined __has_include_next
#if __has_include_next(<arpa/inet.h>)

#ifdef __checkedc
#pragma CHECKED_SCOPE push
#pragma CHECKED_SCOPE off
#endif

#include_next <arpa/inet.h>

#ifdef __checkedc
#pragma CHECKED_SCOPE pop
#endif

#ifdef __checkedc
#ifndef __INET_CHECKED_H
#define __INET_CHECKED_H

#pragma CHECKED_SCOPE push
#pragma CHECKED_SCOPE on

extern in_addr_t inet_addr (const char *__cp : itype(_Nt_array_ptr<const char>)) __THROW;


#pragma CHECKED_SCOPE pop

#endif // guard
#endif // Checked C

#endif // has inet.h
#endif // defined __has_include_next
