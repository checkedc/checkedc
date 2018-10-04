//---------------------------------------------------------------------//
// Bounds-safe interfaces for some functions in arpa/inet.h that       //
// take pointer arguments.                                             //
//                                                                     //
// These are POSIX-only                                                //
/////////////////////////////////////////////////////////////////////////

#include <arpa/inet.h>

#ifndef __cplusplus
#ifndef __INET_CHECKED_H
#define __INET_CHECKED_H

#pragma CHECKED_SCOPE ON

extern in_addr_t inet_addr (const char *__cp : itype(_Nt_array_ptr<const char>)) __THROW;


#pragma CHECKED_SCOPE OFF

#endif
#endif
