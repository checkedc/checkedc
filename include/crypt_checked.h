//---------------------------------------------------------------------//
// Bounds-safe interfaces for functions in crypt.h that               //
// take pointer arguments.                                             //
//                                                                     //
/////////////////////////////////////////////////////////////////////////

#ifdef __checkedc
#pragma CHECKED_SCOPE push
#pragma CHECKED_SCOPE off
#endif

#include_next <crypt.h>

#ifdef __checkedc
#pragma CHECKED_SCOPE pop
#endif

#ifdef __checkedc
#ifndef __CRYPT_CHECKED_H
#define __CRYPT_CHECKED_H

#pragma CHECKED_SCOPE push
#pragma CHECKED_SCOPE on

char * crypt(const char *phrase : itype(_Nt_array_ptr<const char>), const char *setting : itype(_Nt_array_ptr<const char>)) : itype(_Nt_array_ptr<char>);

#pragma CHECKED_SCOPE pop

#endif // guard
#endif // Checked C
