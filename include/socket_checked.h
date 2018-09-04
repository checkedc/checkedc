//---------------------------------------------------------------------//
// Bounds-safe interfaces for functions in POSIX socket.h.              //
//                                                                     //
//                                                                     //
/////////////////////////////////////////////////////////////////////////

#include <sys/socket.h>

#ifndef __cplusplus
#ifndef __SOCKET_CHECKED_H
#define __SOCKET_CHECKED_H

#pragma CHECKED_SCOPE ON

#if _POSIX_VERSION >= 200112L

extern int socketpair (int __domain, int __type, int __protocol, 
    int __fds[2] : itype(int _Checked[2])) __THROW;

extern int bind (
    int __fd, 
    __CONST_SOCKADDR_ARG __addr : itype(_Ptr<const struct sockaddr>), 
    socklen_t __len)
     __THROW;

extern int getsockname (
    int __fd, 
    __SOCKADDR_ARG __addr : itype(_Ptr<struct sockaddr> __restrict), 
    socklen_t *__restrict __len : itype(_Ptr<socklen_t> __restrict)
    ) __THROW;

extern int connect (
    int __fd, 
    __CONST_SOCKADDR_ARG __addr : itype(_Ptr<const struct sockaddr>), 
    socklen_t __len);

extern int getpeername (
    int __fd, 
    __SOCKADDR_ARG __addr : itype(_Ptr<struct sockaddr> __restrict), 
    socklen_t *__restrict __len : itype(_Ptr<socklen_t> __restrict)
    ) __THROW;

_Itype_for_any(T) extern ssize_t send(
    int __fd, 
    const void *__buf : itype(_Array_ptr<const T>) byte_count(__n), 
    size_t __n, int __flags);

_Itype_for_any(T) extern ssize_t recv (
    int __fd, 
    void *__buf : itype(_Array_ptr<T>) byte_count(__n), 
    size_t __n, int __flags);

_Itype_for_any(T) extern ssize_t sendto (
    int __fd, 
    const void *__buf : itype(_Array_ptr<const T>) byte_count(__n), 
    size_t __n, 
    int __flags, 
    __CONST_SOCKADDR_ARG __addr : itype(_Ptr<const struct sockaddr>), 
    socklen_t __addr_len);

_Itype_for_any(T) extern ssize_t recvfrom (
    int __fd, 
    void *__restrict __buf : itype(_Array_ptr<T> __restrict) byte_count(__n), 
    size_t __n, int __flags, 
    __SOCKADDR_ARG __addr : itype(_Ptr<struct sockaddr> __restrict), 
    socklen_t *__restrict __addr_len : itype(_Ptr<socklen_t> __restrict));

extern ssize_t sendmsg (
    int __fd, 
    const struct msghdr *__message : itype(_Ptr<const struct msghdr>), 
    int __flags);

#ifdef __USE_GNU
extern int sendmmsg (
    int __fd, 
    struct mmsghdr *__vmessages : itype(_Array_ptr<struct mmsghdr>) count(__vlen), 
    unsigned int __vlen, 
    int __flags);
#endif

extern ssize_t recvmsg (
    int __fd, 
    struct msghdr *__message : itype(_Ptr<struct msghdr>), 
    int __flags);

#ifdef __USE_GNU
extern int recvmmsg (
    int __fd, 
    struct mmsghdr *__vmessages : itype(_Array_ptr<struct mmsghdr>) count(__vlen), 
    unsigned int __vlen, 
    int __flags, 
    struct timespec *__tmo : itype(_Ptr<struct timespec>));
#endif

_Itype_for_any(T) extern int getsockopt (
    int __fd, int __level, int __optname, 
    void *__restrict __optval : itype(_Array_ptr<T> __restrict) byte_count(*__optlen),
    socklen_t *__restrict __optlen : itype(_Ptr<socklen_t> __restrict)
    ) __THROW;

_Itype_for_any(T) extern int setsockopt (
    int __fd, int __level, int __optname, 
    const void *__optval : itype(_Array_ptr<const T>) byte_count(__optlen), 
    socklen_t __optlen) __THROW;

extern int accept (
    int __fd, 
    __SOCKADDR_ARG __addr : itype(_Ptr<struct sockaddr> __restrict), 
    socklen_t *__restrict __addr_len : itype(_Ptr<socklen_t> __restrict));

#ifdef __USE_GNU
extern int accept4 (
    int __fd, 
    __SOCKADDR_ARG __addr : itype(_Ptr<struct sockaddr> __restrict), 
    socklen_t *__restrict __addr_len : itype(_Ptr<socklen_t> __restrict), 
    int __flags);
#endif

#endif // POSIX

#pragma CHECKED_SCOPE OFF

#endif // guard
#endif // C++
