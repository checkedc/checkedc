//---------------------------------------------------------------------//
// Bounds-safe interfaces for compiler-defined builtin functions       //
// corresponding to stdio.h functions                                  //
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

#if __has_builtin(__builtin___snprintf_chk) || defined(__GNUC__)
extern int __snprintf_chk(char * __restrict s : count(n),
                          size_t n,
                          int flag,
                          size_t obj_size,
                          const char * __restrict format,
                          ...);

int __builtin___snprintf_chk(char * restrict s : count(n),
                             size_t n,
                             int flag,
                             size_t obj_size,
                             const char * restrict format,
                             ...);
#endif


#if __has_builtin(__builtin___vsnprintf_chk) || defined(__GNUC__)
extern int __vsnprintf_chk(char * __restrict s : count(n),
                           size_t n,
                           int flag,
                           size_t obj_size,
                           const char * __restrict format,
                           va_list);

int __builtin___vsnprintf_chk(char * restrict s : count(n),
                              size_t n,
                              int flag,
                              size_t obj_size,
                              const char * restrict format,
                              va_list arg);
#endif

#ifdef _undef__has_builtin
#undef _undef__has_builtin
#undef __has_builtin
#endif
