//---------------------------------------------------------------------//
// Bounds-safe interfaces for some functions in socket.h.              //
//                                                                     //
//                                                                     //
/////////////////////////////////////////////////////////////////////////

#include <socket.h>

#ifndef __cplusplus
#ifndef __SOCKET_CHECKED_H
#define __SOCKET_CHECKED_H

#pragma CHECKED_SCOPE ON

#if _POSIX_VERSION >= 200112L

Itype_for_any(T) extern ssize_t send(int __fd, const void *__buf : itype(_Array_ptr<T>) byte_count(__n), size_t __n, int __flags);

#pragma CHECKED_SCOPE OFF

#endif
#endif
