//---------------------------------------------------------------------//
// Bounds-safe interfaces for compiler-defined builtin functions       //
// corresponding to secure/_common.h functions                         //
//                                                                     //
// These are given in the order they appear in clang's Builtins.def.   //
// Functions that do not appear can not have checked interfaces        //
// defined.                                                            //
//                                                                     //
// These are based on the types as declared within clang               //
// and https://gcc.gnu.org/onlinedocs/gcc/Object-Size-Checking.html    //
/////////////////////////////////////////////////////////////////////////

#ifndef __has_builtin
#define _undef__has_builtin
#define __has_builtin(x) 0
#endif

#if __has_builtin(__builtin_object_size)
size_t __builtin_object_size(const void* obj : itype(_Array_ptr<const void>),
                             int i);
#endif

#ifdef _undef__has_builtin
#undef _undef__has_builtin
#undef __has_builtin
#endif
