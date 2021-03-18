// Feature tests of typechecking bounds-safe
// interfaces for the C standard library.
//
// The following lines are for the LLVM test harness:
//
// Test include files in an unchecked scope
//
// RUN: %clang -fsyntax-only %s
// RUN: %clang -fsyntax-only -D_FORTIFY_SOURCE=0 %s
// RUN: %clang -fsyntax-only -D_FORTIFY_SOURCE=1 %s
// RUN: %clang -fsyntax-only -D_FORTIFY_SOURCE=2 %s
//
// Test include files in a checked scope.
//
// RUN: %clang -fsyntax-only  -DCHECKED_SCOPE=1 %s
// RUN: %clang -fsyntax-only -D_FORTIFY_SOURCE=0 -DCHECKED_SCOPE=1 %s
// RUN: %clang -fsyntax-only -D_FORTIFY_SOURCE=1 -DCHECKED_SCOPE=1 %s
// RUN: %clang -fsyntax-only -D_FORTIFY_SOURCE=2 -DCHECKED_SCOPE=1 %s
//
// Test include files for all combinations of Checked C enabled/disabled,
// and implicit inclusion of checked headers enabled/disabled. By default, both
// are enabled.
//
// Checked headers are disabled for no Checked C.
// RUN: %clang -fno-checkedc-extension -E %s | FileCheck %s --check-prefix CHECK_DISABLED
// RUN: %clang -fno-checkedc-extension -DNO_IMPLICIT_INCLUDE_CHECKED_HDRS -E %s | FileCheck %s --check-prefix CHECK_DISABLED
//
// Checked headers are enabled for Checked C and implicit inclusion.
// RUN: %clang -E %s | FileCheck %s --check-prefix CHECK_ENABLED
//
// Checked headers are enabled because explicit inclusion overrides the
// disabling of implicit inclusion.
// RUN: %clang -DNO_IMPLICIT_INCLUDE_CHECKED_HDRS -E %s | FileCheck %s --check-prefix CHECK_ENABLED


#if CHECKED_SCOPE
#pragma CHECKED_SCOPE on
#endif

// C Standard
#include <assert_checked.h>
// CHECK_ENABLED: include/assert_checked.h
// CHECK_ENABLED: #pragma CHECKED_SCOPE on
// CHECK_DISABLED: include/assert_checked.h
// CHECK_DISABLED-NOT: #pragma CHECKED_SCOPE on

#include <errno_checked.h>
// CHECK_ENABLED: include/errno_checked.h
// CHECK_ENABLED: #pragma CHECKED_SCOPE on
// CHECK_DISABLED: include/errno_checked.h
// CHECK_DISABLED-NOT: #pragma CHECKED_SCOPE on

#include <fenv_checked.h>
// CHECK_ENABLED: include/fenv_checked.h
// CHECK_ENABLED: #pragma CHECKED_SCOPE on
// CHECK_DISABLED: include/fenv_checked.h
// CHECK_DISABLED-NOT: #pragma CHECKED_SCOPE on

#include <inttypes_checked.h>
// CHECK_ENABLED: include/inttypes_checked_internal.h
// CHECK_ENABLED: #pragma CHECKED_SCOPE on
// CHECK_DISABLED: include/inttypes_checked_internal.h
// CHECK_DISABLED-NOT: #pragma CHECKED_SCOPE on

#include <math_checked.h>
// CHECK_ENABLED: include/math_checked.h
// CHECK_ENABLED: #pragma CHECKED_SCOPE on
// CHECK_DISABLED: include/math_checked.h
// CHECK_DISABLED-NOT: #pragma CHECKED_SCOPE on

#include <signal_checked.h>
// CHECK_ENABLED: include/signal_checked.h
// CHECK_ENABLED: #pragma CHECKED_SCOPE on
// CHECK_DISABLED: include/signal_checked.h
// CHECK_DISABLED-NOT: #pragma CHECKED_SCOPE on

#include <stdio_checked.h>
// CHECK_ENABLED: include/stdio_checked.h
// CHECK_ENABLED: #pragma CHECKED_SCOPE on
// CHECK_DISABLED: include/stdio_checked.h
// CHECK_DISABLED-NOT: #pragma CHECKED_SCOPE on

#include <stdlib_checked.h>
// CHECK_ENABLED: include/stdlib_checked.h
// CHECK_ENABLED: #pragma CHECKED_SCOPE on
// CHECK_DISABLED: include/stdlib_checked.h
// CHECK_DISABLED-NOT: #pragma CHECKED_SCOPE on

#include <string_checked.h>
// CHECK_ENABLED: include/string_checked.h
// CHECK_ENABLED: #pragma CHECKED_SCOPE on
// CHECK_DISABLED: include/string_checked.h
// CHECK_DISABLED-NOT: #pragma CHECKED_SCOPE on

#define _CHECKEDC_MOCKUP_THREADS 1
#include <threads_checked.h>
// CHECK_ENABLED: include/threads_checked.h
// CHECK_ENABLED: #pragma CHECKED_SCOPE on
// CHECK_DISABLED: include/threads_checked.h
// CHECK_DISABLED-NOT: #pragma CHECKED_SCOPE on

#include <time_checked.h>
// CHECK_ENABLED: include/time_checked.h
// CHECK_ENABLED: #pragma CHECKED_SCOPE on
// CHECK_DISABLED: include/time_checked.h
// CHECK_DISABLED-NOT: #pragma CHECKED_SCOPE on

#include <checkedc_extensions.h>
// CHECK_ENABLED: include/checkedc_extensions.h
// CHECK_ENABLED: #pragma CHECKED_SCOPE on
// CHECK_DISABLED: include/checkedc_extensions.h
// CHECK_DISABLED-NOT: #pragma CHECKED_SCOPE on



// Posix Headers
//
// Uses clang-specific __has_include macro to detect unistd.h
// which is required by Posix Standard.
#if defined(__has_include)
#if __has_include(<unistd.h>)
#include <unistd_checked.h>
// CHECK_ENABLED: include/unistd_checked.h
// CHECK_ENABLED: #pragma CHECKED_SCOPE on
// CHECK_DISABLED: include/unistd_checked.h
// CHECK_DISABLED-NOT: #pragma CHECKED_SCOPE on
#endif

#if __has_include(<sys/socket.h>)
#include <sys/socket_checked.h>
// CHECK_ENABLED: include/sys/socket_checked.h
// CHECK_ENABLED: #pragma CHECKED_SCOPE on
// CHECK_DISABLED: include/sys/socket_checked.h
// CHECK_DISABLED-NOT: #pragma CHECKED_SCOPE on
#endif

#if __has_include(<arpa/inet.h>)
#include <arpa/inet_checked.h>
// CHECK_ENABLED: include/arpa/inet_checked.h
// CHECK_ENABLED: #pragma CHECKED_SCOPE on
// CHECK_DISABLED: include/arpa/inet_checked.h
// CHECK_DISABLED-NOT: #pragma CHECKED_SCOPE on
#endif

#endif