//---------------------------------------------------------------------//
// Bounds-safe interfaces for functions in netdb.h that                 //
// take pointer arguments.                                             //
//                                                                     //
// These are listed in the same order that they occur in the C11       //
// specification.                                                      //
/////////////////////////////////////////////////////////////////////////

#ifdef __checkedc
#pragma CHECKED_SCOPE push
#pragma CHECKED_SCOPE off
#endif

#include_next <netdb.h>

#ifdef __checkedc
#pragma CHECKED_SCOPE pop
#endif

#ifdef __checkedc
#ifndef __NETDB_CHECKED_H
#define __NETDB_CHECKED_H

#pragma CHECKED_SCOPE push
#pragma CHECKED_SCOPE on

extern struct hostent *gethostbyname(const char *name : itype(_Nt_array_ptr<const char>)) : itype(_Ptr<struct hostent>);
_Unchecked
extern struct hostent *gethostbyaddr(const void *addr : byte_count(len), socklen_t len, int type);

#pragma CHECKED_SCOPE pop

#endif // guard
#endif // Checked C
