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
//                                                                     //
// TODO: revise string types after support for pointers to             //
// null-terminated arrays is added to C.                               //
/////////////////////////////////////////////////////////////////////////

#ifndef __has_builtin
#define _undef__has_builtin
#define __has_builtin(x) 0
#endif

#if __has_builtin(__builtin___memcpy_chk) || defined(__GNUC__)


void *__builtin___memcpy_chk(void * restrict dest : byte_count(n),
                             const void * restrict src : byte_count(n),
                             size_t n,
                             size_t obj_size) : bounds(dest, (char *) dest + n);
#endif

#if __has_builtin(__builtin__memmove_chk) || defined(__GNUC__)
void *__builtin__memmove_chk(void * restrict dest : byte_count(n),
                             const void * restrict src : byte_count(n),
                             size_t n,
                             size_t obj_size) : bounds(dest, (char *)dest + n);
#endif

#if __has_builtin(__builtin__memset_chk) || defined(__GNUC__)
void *__builtin__memset_chk(void * s : byte_count(n),
                            int c,
                            size_t n,
                            size_t obj_size) : bounds(s, (char *) s + n);
#endif

#if __has_builtin(__builtin___strncat_chk) || defined(__GNUC__)
char *__builtin___strncat_chk(char * restrict dest : count(n),
                              const char * restrict src : count(n),
                              size_t n,
                              size_t obj_size) : bounds(dest, (char *)dest + n);
#endif

#if __has_builtin(__builtin___strncpy_chk) || defined(__GNUC__)
char *__builtin___strncpy_chk(char * restrict dest : count(n),
                              const char * restrict src : count(n),
                              size_t n,
                              size_t obj_size) : bounds(dest, (char *)dest + n);
#endif

#ifdef _undef__has_builtin
#undef _undef__has_builtin
#undef __has_builtin
#endif
