// Feature tests of typechecking bounds-safe
// interfaces for the C standard library.
//
// The following lines are for the LLVM test harness:
//
// RUN: %clang -fsyntax-only %s
// RUN: %clang -fsyntax-only -D_FORTIFY_SOURCE=0 %s
// RUN: %clang -fsyntax-only -D_FORTIFY_SOURCE=1 %s
// RUN: %clang -fsyntax-only -D_FORTIFY_SOURCE=2 %s

// C Standard
#include "../../include/errno_checked.h"
#include "../../include/fenv_checked.h"
#include "../../include/inttypes_checked.h"
#include "../../include/math_checked.h"
#include "../../include/signal_checked.h"
#include "../../include/stdio_checked.h"
#include "../../include/stdlib_checked.h"
#include "../../include/string_checked.h"
#define _CHECKEDC_MOCKUP_THREADS 1
#include "../../include/threads_checked.h"
#include "../../include/time_checked.h"
#include "../../include/checkedc_extensions.h"

// Posix Headers
//
// Uses clang-specific __has_include macro to detect unistd.h
// which is required by Posix Standard.
#if defined(__has_include)
#if __has_include(<unistd.h>)

#include "../../include/unistd_checked.h"

#endif
#endif
