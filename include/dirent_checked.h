//---------------------------------------------------------------------//
// Bounds-safe interfaces for some functions in dirent.h that          //
// take pointer arguments.                                             //
//                                                                     //
/////////////////////////////////////////////////////////////////////////

#ifdef __checkedc
#pragma CHECKED_SCOPE push
#pragma CHECKED_SCOPE off
#endif

#include_next <dirent.h>

#ifdef __checkedc
#pragma CHECKED_SCOPE pop
#endif

#ifdef __checkedc
#ifndef __DIRENT_CHECKED_H
#define __DIRENT_CHECKED_H

#pragma CHECKED_SCOPE push
#pragma CHECKED_SCOPE on

DIR *opendir(const char *name : itype(_Nt_array_ptr<const char>)) : itype(_Ptr<DIR>);
DIR *fdopendir(int fd) : itype(_Ptr<DIR>);
int closedir(DIR *dirp : itype(_Ptr<DIR>));
struct dirent *readdir(DIR *dirp : itype(_Ptr<DIR>)) : itype(_Ptr<struct dirent>);
int dirfd(DIR *dirp : itype(_Ptr<DIR>));

#pragma CHECKED_SCOPE pop

#endif // guard
#endif // Checked C
