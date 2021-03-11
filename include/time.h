//---------------------------------------------------------------------//
// Wrapper header file that excludes Checked-C-specific declarations   //
// if implicit inclusion of checked header files is disabled.          //
//                                                                     //
/////////////////////////////////////////////////////////////////////////


#ifdef NO_IMPLICIT_INCLUDE_CHECKED_HDRS

#ifndef __cplusplus
#pragma CHECKED_SCOPE push
#pragma CHECKED_SCOPE off
#endif

#include_next <time.h>

#ifndef __cplusplus
#pragma CHECKED_SCOPE pop
#endif

#else // implicit include
#include <time_checked.h>
#endif
