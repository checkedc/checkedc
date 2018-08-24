//---------------------------------------------------------------------//
// Bounds-safe interfaces for functions in unistd.h that               //
// take pointer arguments.                                             //
//                                                                     //
// These are POSIX-only                                                //
/////////////////////////////////////////////////////////////////////////

#include <unistd.h>

#ifndef __cplusplus
#ifndef __UNISTD_CHECKED_H
#define __UNISTD_CHECKED_H

#pragma CHECKED_SCOPE ON

#if _POSIX_VERSION >= 200112L

extern char ** environ : itype(_Nt_array_ptr<_Nt_array_ptr<char>>);

extern int access (const char *__name : itype(_Nt_array_ptr<const char>), int __type) __THROW __nonnull ((1));

extern ssize_t read (int __fd, void *__buf : byte_count(__nbytes), size_t __nbytes) __wur;

extern ssize_t write (int __fd, const void *__buf : byte_count(__nbytes), size_t __n) __wur;

#endif

#pragma CHECKED_SCOPE OFF

#endif
#endif
