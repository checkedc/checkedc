//---------------------------------------------------------------------//
// Bounds-safe interfaces for functions in string.h that               //
// take pointer arguments.                                             //
//                                                                     //
// These are listed in the same order that they occur in the C11       //
// specification.                                                      //
//                                                                     //
// TODO: revise string types after support for pointers to             //
// null-terminated arrays is added to C.                               //
//                                                                     //
// TODO: Better Support for _FORTIFY_SOURCE > 0                        //
/////////////////////////////////////////////////////////////////////////
#include <string.h>

#pragma BOUNDS_CHECKED ON

// TODO: Apple System Headers Support
#if !( defined(__APPLE__) && _FORTIFY_SOURCE > 0)
void *memcpy(void * restrict dest : byte_count(n),
             const void * restrict src : byte_count(n),
             size_t n) : bounds(dest, (_Array_ptr<char>) dest + n);

void *memmove(void * restrict dest : byte_count(n),
              const void * restrict src : byte_count(n),
              size_t n) : bounds(dest, (_Array_ptr<char>)dest + n);
#endif

// Dest is left unchecked intentionally. There is no bound on dest, so this
// is always an unchecked function
char *strcpy(char * restrict s1,
              const char * restrict s2 : itype(restrict _Nt_array_ptr<const char>));

// TODO: Apple System Headers Support
#if !( defined(__APPLE__) && _FORTIFY_SOURCE > 0)
char *strncpy(char * restrict dest : count(n),
              const char * restrict src : count(n),
              size_t n) : bounds(dest, (_Array_ptr<char>)dest + n);
#endif

// Dest is left unchecked intentionally. There is no bound on dest, so this
// is always an unchecked function.
_Unchecked
char *strcat(char * restrict dest,
             const char * restrict src : itype(restrict _Nt_array_ptr<const char>));

// TODO: Apple System Headers Support
#if !( defined(__APPLE__) && _FORTIFY_SOURCE > 0)
// TODO: we have no way to express the bounds requirement on dest,
// which needs to be count(strlen(dest) + n).
_Unchecked
char *strncat(char * restrict dest,
              const char * restrict src : count(n),
              size_t n);
#endif

int memcmp(const void *src1 : byte_count(n), const void *src2 : byte_count(n),
           size_t n);

int strcmp(const char *src1 : itype(_Nt_array_ptr<char>),
           const char *src2 : itype(_Nt_array_ptr<char>));
int strcoll(const char *src1 : itype(_Nt_array_ptr<char>),
            const char *src2 : itype(_Nt_array_ptr<char>));

// Linux header files declare strncmp and also define a macro for it.
// Undef the macro so that we can redeclare strncmp.
//
// Section 7.1.4 of the C11 standard allows the use of #undef to prevent
// macros from interfering  with explicit declarations of library functions.
// It is legal to #undef a macro that isn't defined, so we don't need to
// conditionalize this.
#undef strncmp
int strncmp(const char *src : count(n), const char *s2 : count(n), size_t n);

size_t strxfrm(char * restrict dest : count(n),
               const char * restrict src :
                 itype(restrict _Nt_array_ptr<const char>),
               size_t n);

void *memchr(const void *s : byte_count(n), int c, size_t n) :
  bounds(s, (_Array_ptr<char>) s + n);

char *strchr(const char *s : itype(_Nt_array_ptr<const char>), int c) :
  itype(_Nt_array_ptr<char>);

size_t strcspn(const char *s1 : itype(_Nt_array_ptr<const char>),
               const char *s2 : itype(_Nt_array_ptr<const char>));

char *strpbrk(const char *s1 : itype(_Nt_array_ptr<const char>),
              const char *s2 : itype(_Nt_array_ptr<const char>)) :
  itype(_Nt_array_ptr<char>);
char *strrchr(const char *s : itype(_Nt_array_ptr<const char>), int c) :
  itype(_Nt_array_ptr<char>)
size_t strspn(const char *s1 : itype(_Nt_array_ptr<const char>),
              const char *s2 : itype(_Nt_array_ptr<const char>));
char *strstr(const char *s1 : itype(_Nt_array_ptr<const char>),
             const char *s2 : itype(_Nt_array_ptr<const char>)) :
  itype(_Nt_array_ptr<char>);
char *strtok(char * restrict s1 : itype(restrict _Nt_array_ptr<char>),
             const char * restrict s2 : itype(restrict _Nt_array_ptr<char>)) :
  itype(_Nt_array_ptr<char>)

// TODO: Apple System Headers Support
#if !( defined(__APPLE__) && _FORTIFY_SOURCE > 0)
void *memset(void *s : byte_count(n), int c, size_t n) :
  bounds(s, (_Array_ptr<char>) s + n);
#endif

char *strerror(int errnum) : itype(_Nt_array_ptr<char>);
size_t strlen(const char *s : itype(_Nt_array_ptr<const char>));

#include "_builtin_string_checked.h"

#pragma BOUNDS_CHECKED OFF
