//---------------------------------------------------------------------//
// Bounds-safe interfaces for functions in unistd.h that               //
// take pointer arguments.                                             //
//                                                                     //
// These are POSIX-only                                                //
/////////////////////////////////////////////////////////////////////////


// Uses clang-specific __has_include macro to detect unistd.h
// which is required by Posix Standard.
// The Windows environment also may not have unistd.h
#if defined __has_include_next
#if __has_include_next(<unistd.h>)

#ifdef __checkedc
#pragma CHECKED_SCOPE push
#pragma CHECKED_SCOPE off
#endif

#include_next <unistd.h>

#ifdef __checkedc
#pragma CHECKED_SCOPE pop
#endif

#ifdef __checkedc
#ifndef __UNISTD_CHECKED_H
#define __UNISTD_CHECKED_H

#pragma CHECKED_SCOPE push
#pragma CHECKED_SCOPE on

#if _POSIX_VERSION >= 200112L

extern char ** environ : itype(_Nt_array_ptr<_Nt_array_ptr<char>>);

#ifdef __APPLE__

extern int access (const char *__name : itype(_Nt_array_ptr<const char>), int __type);
extern ssize_t read (int __fd, void *__buf : byte_count(__nbytes), size_t __nbytes);
extern ssize_t write (int __fd, const void *__buf : byte_count(__n), size_t __n);

#else

extern int access (const char *__name : itype(_Nt_array_ptr<const char>), int __type) __THROW __nonnull ((1));
extern ssize_t read (int __fd, void *__buf : byte_count(__nbytes), size_t __nbytes) __wur;
extern ssize_t write (int __fd, const void *__buf : byte_count(__n), size_t __n) __wur;

#endif
#endif

#pragma CHECKED_SCOPE pop

#endif // guard
#endif // Checked C

#endif // has unistd.h
#endif // defined __has_include_next
