//---------------------------------------------------------------------//
// Wrapper header file that excludes Checked-C-specific declarations   //
// if implicit inclusion of checked header files is disabled.          //
/////////////////////////////////////////////////////////////////////////

#ifdef NO_IMPLICIT_INCLUDE_CHECKED_HDRS
#include_next <inttypes.h>
#else
#include <inttypes_checked.h>
#endif
