//---------------------------------------------------------------------//
// Wrapper header file that excludes Checked-C-specific declarations   //
// if the compilation is not for Checked C, or if is for Checked C     //
// but the implicit inclusion of checked header files is disabled.     //
/////////////////////////////////////////////////////////////////////////

// The Windows environment may not have sys/stat.h
#if defined __has_include_next && __has_include_next(<sys/stat.h>)
#if !defined __checkedc || defined NO_IMPLICIT_INCLUDE_CHECKED_HDRS

#ifdef __checkedc
#pragma CHECKED_SCOPE push
#pragma CHECKED_SCOPE off
#endif

#include_next <sys/stat.h>

#ifdef __checkedc
#pragma CHECKED_SCOPE pop
#endif

#else // checkedc && implicit include enabled
#include <sys/stat_checked.h>
#endif

#else // doesn't have sys/stat.h
#error "cannot include 'sys/stat.h' because this system does not have the original header, even though Checked C provides a wrapper for it"
#endif
