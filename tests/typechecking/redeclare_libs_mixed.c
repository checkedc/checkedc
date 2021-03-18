// Feature tests of typechecking bounds-safe
// interfaces for the C standard library.
//
// Test include files for all combinations of Checked C enabled/disabled,
// and implicit inclusion of checked headers enabled/disabled. By default, both
// are enabled.
//
// Checked headers are disabled for no Checked C.
// Already tested in redeclare_libs_explicit.c and redeclare_libs_implicit.c.
//    %clang -fno-checkedc-extension -E %s | FileCheck %s --check-prefix CHECK_DISABLED
//    %clang -fno-checkedc-extension -DNO_IMPLICIT_INCLUDE_CHECKED_HDRS -E %s | FileCheck %s --check-prefix CHECK_DISABLED
//
// Checked headers are enabled for Checked C and implicit inclusion.
// Already tested in redeclare_libs_explicit.c and redeclare_libs_implicit.c.
//    %clang -E %s | FileCheck %s --check-prefix CHECK_ENABLED
//
// Checked headers are enabled/disabled depending on whether foo.h or
// foo_checked.h is included for a system header file foo.h.
// RUN: %clang -DNO_IMPLICIT_INCLUDE_CHECKED_HDRS -E %s | FileCheck %s --check-prefix CHECK_MIXED


// C Standard
#include <assert_checked.h>
// CHECK_MIXED: include/assert_checked.h
// CHECK_MIXED: #pragma CHECKED_SCOPE on

#include <errno.h>
// CHECK_MIXED-NOT: include/errno_checked.h
// CHECK_MIXED-NOT: #pragma CHECKED_SCOPE on

#include <fenv_checked.h>
// CHECK_MIXED: include/fenv_checked.h
// CHECK_MIXED: #pragma CHECKED_SCOPE on

#include <inttypes.h>
// CHECK_MIXED-NOT: include/inttypes_checked_internal.h
// CHECK_MIXED-NOT: #pragma CHECKED_SCOPE on

#include <math_checked.h>
// CHECK_MIXED: include/math_checked.h
// CHECK_MIXED: #pragma CHECKED_SCOPE on

#include <signal.h>
// CHECK_MIXED-NOT: include/signal_checked.h
// CHECK_MIXED-NOT: #pragma CHECKED_SCOPE on

#include <stdio_checked.h>
// CHECK_MIXED: include/stdio_checked.h
// CHECK_MIXED: #pragma CHECKED_SCOPE on

#include <stdlib.h>
// CHECK_MIXED-NOT: include/stdlib_checked.h
// CHECK_MIXED-NOT: #pragma CHECKED_SCOPE on

#include <string_checked.h>
// CHECK_MIXED: include/string_checked.h
// CHECK_MIXED: #pragma CHECKED_SCOPE on

#define _CHECKEDC_MOCKUP_THREADS 1
#include <threads.h>
// CHECK_MIXED-NOT: include/threads_checked.h
// CHECK_MIXED-NOT: #pragma CHECKED_SCOPE on

#include <time_checked.h>
// CHECK_MIXED: include/time_checked.h
// CHECK_MIXED: #pragma CHECKED_SCOPE on

#include <checkedc_extensions.h>
// CHECK_MIXED: include/checkedc_extensions.h
// CHECK_MIXED: #pragma CHECKED_SCOPE on


// Posix Headers
//
// Uses clang-specific __has_include macro to detect unistd.h
// which is required by Posix Standard.
#if defined(__has_include)
#if __has_include(<unistd_checked.h>)
#include <unistd_checked.h>
// CHECK_MIXED: include/unistd_checked.h
// CHECK_MIXED: #pragma CHECKED_SCOPE on
#endif

#if __has_include(<sys/socket.h>)
#include <sys/socket.h>
// CHECK_MIXED-NOT: include/sys/socket_checked.h
// CHECK_MIXED-NOT: #pragma CHECKED_SCOPE on
#endif

#if __has_include(<arpa/inet_checked.h>)
#include <arpa/inet_checked.h>
// CHECK_MIXED: include/arpa/inet_checked.h
// CHECK_MIXED: #pragma CHECKED_SCOPE on
#endif

#endif
