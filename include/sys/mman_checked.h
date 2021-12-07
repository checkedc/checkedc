//---------------------------------------------------------------------//
// Bounds-safe interfaces for functions in sys/mman.h that             //
// take pointer arguments.                                             //
//                                                                     //
/////////////////////////////////////////////////////////////////////////

#ifdef __checkedc
#pragma CHECKED_SCOPE push
#pragma CHECKED_SCOPE off
#endif

#include_next <sys/mman.h>

#ifdef __checkedc
#pragma CHECKED_SCOPE pop
#endif

#ifdef __checkedc
#ifndef __MMAN_CHECKED_H
#define __MMAN_CHECKED_H

#pragma CHECKED_SCOPE push
#pragma CHECKED_SCOPE on

_Itype_for_any(T) void * mmap(void *addr : itype(_Array_ptr<T>) byte_count(0), size_t length, int prot, int flags, int fd, off_t offset) : itype(_Array_ptr<T>) byte_count(length);
_Itype_for_any(T) int munmap(void *addr : itype(_Array_ptr<T>) byte_count(length), size_t length);
_Itype_for_any(T) int mprotect(void *addr : itype(_Array_ptr<T>) byte_count(len) , size_t len, int prot);

#pragma CHECKED_SCOPE pop

#endif // guard
#endif // Checked C
