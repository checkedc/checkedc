//---------------------------------------------------------------------//
// Bounds-safe interfaces for compiler-defined builtin functions       //
// corresponding to string.h functions                                 //
//                                                                     //
// These are given in the order they appear in clang's Builtins.def.   //
// Functions that do not appear can not have checked interfaces        //
// defined.                                                            //
//                                                                     //
// These are based on the types as declared within clang               //
// and https://gcc.gnu.org/onlinedocs/gcc/Object-Size-Checking.html    //
/////////////////////////////////////////////////////////////////////////

#include "_builtin_common.h"

#if _USE_FORTIFY_LEVEL > 0

#ifndef __has_builtin
#define _undef__has_builtin
#define __has_builtin(x) 0
#endif

#if __has_builtin(__builtin___memcpy_chk) || defined(__GNUC__)
void *__builtin___memcpy_chk(void * restrict dest : byte_count(n),
                             const void * restrict src : byte_count(n),
                             size_t n,
                             size_t obj_size) : bounds(dest, (_Array_ptr<char>) dest + n);
#endif

#if __has_builtin(__builtin___memmove_chk) || defined(__GNUC__)
void *__builtin___memmove_chk(void * restrict dest : byte_count(n),
                             const void * restrict src : byte_count(n),
                             size_t n,
                             size_t obj_size) : bounds(dest, (_Array_ptr<char>)dest + n);
#endif

#if __has_builtin(__builtin___memset_chk) || defined(__GNUC__)
void *__builtin___memset_chk(void * s : byte_count(n),
                            int c,
                            size_t n,
                            size_t obj_size) : bounds(s, (_Array_ptr<char>) s + n);
#endif

#if __has_builtin(__builtin___strcat_chk) || defined(__GNUC__)
// Left Unchecked Intentionally. See comment in string_checked.h
_Unchecked
char *__builtin___strcat_chk(char * restrict dest,
                             const char * restrict src :
                               itype(restrict _Nt_array_ptr<const char>),
                             size_t obj_size);
#endif

#if __has_builtin(__builtin___strcpy_chk) || defined(__GNUC__)
// Left Unchecked Intentionally. See comment in string_checked.h
_Unchecked
char *__builtin___strcpy_chk(char * restrict dest,
                             const char * restrict src :
                               itype(restrict _Nt_array_ptr<const char>),
                             size_t obj_size);
#endif

#if __has_builtin(__builtin___strncat_chk) || defined(__GNUC__)
// TODO: we have no way to express the bounds requirement on dest,
// which needs to be count(strlen(dest) + n).
_Unchecked
char *__builtin___strncat_chk(char * restrict dest,
                              const char * restrict src : count(n),
                              size_t n,
                              size_t obj_size);
#endif

#if __has_builtin(__builtin___strncpy_chk) || defined(__GNUC__)
char *__builtin___strncpy_chk(char * restrict dest : count(n),
                              const char * restrict src : count(n),
                              size_t n,
                              size_t obj_size) : bounds(dest, (_Array_ptr<char>)dest + n);
#endif

#ifdef _undef__has_builtin
#undef _undef__has_builtin
#undef __has_builtin
#endif

#endif
