// Feature tests of typechecking redeclarations of functions
// and variables
//
// The following lines are for the LLVM test harness:
//
// RUN: %clang -fcheckedc-extension -fsyntax-only %s

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
