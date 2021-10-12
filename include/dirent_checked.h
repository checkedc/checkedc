//---------------------------------------------------------------------//
// Bounds-safe interfaces for functions in dirent.h that                 //
// take pointer arguments.                                             //
//                                                                     //
/////////////////////////////////////////////////////////////////////////

// The Windows environment may not have dirent.h
#if defined __has_include_next && __has_include_next(<dirent.h>)

#ifdef __checkedc
#pragma CHECKED_SCOPE push
#pragma CHECKED_SCOPE off
#endif

#include_next <dirent.h>

#ifdef __checkedc
#pragma CHECKED_SCOPE pop
#endif

#ifdef __checkedc
#ifndef __POLL_CHECKED_H
#define __POLL_CHECKED_H

#pragma CHECKED_SCOPE push
#pragma CHECKED_SCOPE on

DIR *opendir(const char *name : itype(_Nt_array_ptr<const char>)) : itype(_Ptr<DIR>);
DIR *fdopendir(int fd) : itype(_Ptr<DIR>);
int closedir(DIR *dirp : itype(_Ptr<DIR>));
struct dirent *readdir(DIR *dirp : itype(_Ptr<DIR>)) : itype(_Ptr<struct dirent>);

#pragma CHECKED_SCOPE pop

#endif // guard
#endif // Checked C

#else // doesn't have dirent.h
#error "cannot include 'dirent_checked.h' because this system does not have the original 'dirent.h'"
#endif
