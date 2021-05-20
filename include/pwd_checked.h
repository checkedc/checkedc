//---------------------------------------------------------------------//
// Bounds-safe interfaces for functions in pwd.h that                  //
// take pointer arguments.                                             //
//                                                                     //
/////////////////////////////////////////////////////////////////////////

#ifdef __checkedc
#pragma CHECKED_SCOPE push
#pragma CHECKED_SCOPE off
#endif

#include_next <pwd.h>

#ifdef __checkedc
#pragma CHECKED_SCOPE pop
#endif

#ifdef __checkedc
#ifndef __PWD_CHECKED_H
#define __PWD_CHECKED_H

#pragma CHECKED_SCOPE push
#pragma CHECKED_SCOPE on

struct passwd *getpwnam(const char *user : itype(_Nt_array_ptr<const char>)) : itype(_Ptr<struct passwd>);
_Unchecked
int getpwnam_r(const char *user : itype(_Nt_array_ptr<const char>),
	       struct passwd *pwd : itype(_Ptr<struct passwd>),
	       char *buf : byte_count(bufsize),
	       size_t bufsize, struct passwd **result : itype(_Ptr<_Ptr<struct passwd>>));

struct passwd *getpwuid(uid_t uid) : itype(_Ptr<struct passwd>);
_Unchecked
int            getpwuid_r(uid_t uid,
	       struct passwd *pwd : itype(_Ptr<struct passwd>),
	       char *buf : byte_count(bufsize),
	       size_t bufsize, struct passwd **result : itype(_Ptr<_Ptr<struct passwd>>));

#pragma CHECKED_SCOPE pop

#endif // guard
#endif // Checked C
