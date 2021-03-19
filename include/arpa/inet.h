//---------------------------------------------------------------------//
// Wrapper header file that excludes Checked-C-specific declarations   //
// if the compilation is not for Checked C, or if is for Checked C     //
// but the implicit inclusion of checked header files is disabled.     //
/////////////////////////////////////////////////////////////////////////


#if !defined __checkedc || defined NO_IMPLICIT_INCLUDE_CHECKED_HDRS

// The Windows environment may not have arpa/inet.h
#if defined __has_include_next
#if __has_include_next(<arpa/inet.h>)

#ifdef __checkedc
#pragma CHECKED_SCOPE push
#pragma CHECKED_SCOPE off
#endif

#include_next <arpa/inet.h>

#ifdef __checkedc
#pragma CHECKED_SCOPE pop
#endif

#endif // has inet.h
#endif // defined __has_include_next

#else // checkedc && implicit include enabled
#include <arpa/inet_checked.h>
#endif
