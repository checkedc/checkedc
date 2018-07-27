// Quick test for errno bounds safe interface
// 
// The following lines are for the LLVM test harness:
//
// RUN: %clang -fsyntax-only -Werror %s

#include <stdchecked.h>
#include <errno_checked.h>

#pragma CHECKED_SCOPE ON

void f1(void)
{
	errno = 0;
}
