//--------------------------------------------------------------------//
// Bounds-safe interfaces for functions in fenv.h that                //
// take pointer arguments.                                            //
//                                                                    //
// These are listed in the same order that they occur in the C11      //
// specification.                                                     //
////////////////////////////////////////////////////////////////////////

#ifndef __FENV_CHECKED_H
#define __FENV_CHECKED_H

#include <fenv.h>

#pragma CHECKED_SCOPE ON

int fesetexceptflag(const fexcept_t *flagp : itype(_Ptr<const fexcept_t>),
                    int excepts);
int fegetenv(fenv_t *envp : itype(_Ptr<fenv_t>));
int feholdexcept(fenv_t *envp : itype(_Ptr<fenv_t>));
int fesetenv(const fenv_t *envp : itype(_Ptr<const fenv_t>));
int feupdateenv(const fenv_t *envp : itype(_Ptr<const fenv_t>));

#pragma CHECKED_SCOPE OFF

#endif
