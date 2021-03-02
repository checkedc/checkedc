//---------------------------------------------------------------------//
// Wrapper header file that excludes Checked-C-specific declarations   //
// if implicit inclusion of checked header files is disabled.          //
/////////////////////////////////////////////////////////////////////////

#ifdef NO_IMPLICIT_INCLUDE_CHECKED_HDRS
#include_next <unistd.h>
#else
#include <unistd_checked.h>
#endif
