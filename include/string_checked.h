//---------------------------------------------------------------------//
// Bounds-safe interfaces for functions in string.h that               //
// take pointer arguments.                                             //
//                                                                     //
// These are listed in the same order that they occur in the C11       //
// specification.                                                      //
//                                                                     //
// TODO: revise string types after support for pointers to             //
// null-terminated arrays is added to C.                               //
/////////////////////////////////////////////////////////////////////////
#include <string.h>


// TODO: Apple System Headers Support
#ifndef __APPLE__
void *memcpy(void * restrict dest : byte_count(n),
             const void * restrict src : byte_count(n),
             size_t n) : bounds(dest, (char *) dest + n);

void *memmove(void * restrict dest : byte_count(n),
              const void * restrict src : byte_count(n),
              size_t n) : bounds(dest, (char *)dest + n);
#endif
// TODO: strings
// char *strcpy(char * restrict dest,
//              const char * restrict src);

// OMITTED INTENTIONALLY: this cannot be made checked.
// There is no bound on s1.
// char *strcpy(char * restrict s1,
//              const char * restrict s2);

// TODO: Apple System Headers Support
#ifndef __APPLE__
char *strncpy(char * restrict dest : count(n),
              const char * restrict src : count(n),
              size_t n) : bounds(dest, (char *)dest + n);
#endif

// OMITTED INTENTIONALLY: this cannot be made checked.
// There is no bound on dest.
// char *strcat(char * restrict dest,
//              const char * restrict src);

// TODO: Apple System Headers Support
#ifndef __APPLE__
char *strncat(char * restrict dest : count(n),
              const char * restrict src : count(n),
              size_t n) : bounds(dest, (char *)dest + n);
#endif

int memcmp(const void *src1 : byte_count(n), const void *src2 : byte_count(n),
           size_t n);

// TODO: strings
// int strcmp(const char *src1, const char *src2);
// int strcoll(const char *src1, const char *src2);

int strncmp(const char *src : count(n), const char *s2 : count(n), size_t n);
size_t strxfrm(char * restrict dest : count(n),
               const char * restrict src,
               size_t n);

void *memchr(const void *s : byte_count(n), int c, size_t n) :
  bounds(s, (char *) s + n);

// TODO: strings
// char *strchr(const char *s, int c);
// size_t strcspn(const char *s1, const char *s2);
// char *strpbrk(const char *s1, const char *s2);
// char *strrchr(const char *s, int c);
// size_t strspn(const char *s1, const char *s2);
// char *strstr(const char *s1, const char *s2);
// char *strtok(char * restrict s1,
//              const char * restrict s2);

// TODO: Apple System Headers Support
#ifndef __APPLE__
void *memset(void *s : byte_count(n), int c, size_t n) :
  bounds(s, (char *) s + n);
#endif

// TODO: strings
// char *strerror(int errnum);
// size_t strlen(const char *s);
