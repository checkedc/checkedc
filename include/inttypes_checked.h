//---------------------------------------------------------------------//
// Wrapper header file that includes Checked-C-specific declarations   //
// explicitly.That is, it overrides the disabling of implicit          //
// inclusion of checked header files.                                  //
//                                                                     //
/////////////////////////////////////////////////////////////////////////


//Force the inclusion of Checked-C-specific declarations
#undef NO_IMPLICIT_INCLUDE_CHECKED_HDRS
#include <inttypes.h>
