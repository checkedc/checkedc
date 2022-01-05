//---------------------------------------------------------------------//
// Bounds-safe interfaces for functions in sys/time.h that             //
// take pointer arguments.                                             //
//                                                                     //
/////////////////////////////////////////////////////////////////////////

#ifdef __checkedc
#pragma CHECKED_SCOPE push
#pragma CHECKED_SCOPE off
#endif

#include_next <sys/time.h>

#ifdef __checkedc
#pragma CHECKED_SCOPE pop
#endif

#ifdef __checkedc
#ifndef __SYS_TIME_CHECKED_H
#define __SYS_TIME_CHECKED_H

#pragma CHECKED_SCOPE push
#pragma CHECKED_SCOPE on

#ifdef __USE_MISC

// The original system header has defined `struct timezone`, so we
// should accommodate callers that pass a non-null `struct timezone *`
// to gettimeofday. The man page shows the second parameter as `struct
// timezone *tz`, but glibc has it as `void *`, and our redeclaration
// must be consistent with that.
//
// ~ Matt 2022-01-05
int gettimeofday(struct timeval * tv : itype(_Ptr<struct timeval>), void *tz : itype(_Array_ptr<void>) byte_count(sizeof(struct timezone)));

int settimeofday(const struct timeval *tv : itype(_Ptr<const struct timeval>), const struct timezone *tz : itype(_Ptr<const struct timezone>));

#else

// `struct timezone` has not been defined, so we mustn't reference it
// from the Checked C annotations. Currently, it looks like if we
// leave the parameter type as an unchecked pointer (which requires us
// to declare the function as _Unchecked here), gettimeofday can still
// be called from a checked scope with a literal null pointer as the
// argument, though there's a risk of the compiler behavior changing
// and breaking this in the future.
//
// Another reasonable approach would be to declare the parameter
// `bounds(any)` as described in the specification, but the Checked C
// compiler doesn't seem to be able to parse that. (TODO: File an
// issue?) The other obvious approaches (`_Array_ptr<void> :
// bounds(unknown)`, `_Ptr<void>`) would allow pointers with
// insufficient bounds to be passed.

_Unchecked
int gettimeofday(struct timeval * tv : itype(_Ptr<struct timeval>), void *tz);

// glibc doesn't declare settimeofday in this case.

#endif

#pragma CHECKED_SCOPE pop

#endif // guard
#endif // Checked C
