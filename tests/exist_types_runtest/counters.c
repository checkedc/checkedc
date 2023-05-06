// Run-time test for existential types
// There are no expected failures in the execution of this test.
// We're just checking that using _Pack, _Unpack and existentials
// works as inteded.
// The test itself implement a simple counter interface in two different ways:
// with a regular counter and a mod 2 counter.
// We run the two counters in a loop, and then check that the output matches
// what we expect.
//
// RUN: %clang %s -o %t1 %checkedc_target_flags

// RUN: %checkedc_rununder %t1 -DARR | FileCheck %s --check-prefix TEST

#include <stdlib.h>
#include <stdio.h>

struct Counter _For_any(T) {
    T *state;
    void (*inc)(T *st);
    void (*dec)(T *st);
    int (*getVal)();
};

// Regular counter

void incIntCounter(int *x) {
    *x = (*x) + 1;
}

void decIntCounter(int *x) {
    *x = (*x) - 1;
}

int getValIntCounter(int *x) {
    return *x;
}

_Exists(T, struct Counter<T>) getCounter() {
    struct Counter<int> ct;
    int *x = malloc(sizeof(int));
    *x = 0;
    ct.state = x;
    ct.inc = incIntCounter;
    ct.dec = decIntCounter;
    ct.getVal = getValIntCounter;
    return _Pack(ct, _Exists(T, struct Counter<T>), int);
}

// Mod2 counter

void toggleMod2Counter(char *x) {
    *x = !(*x);
}

int getValMod2Counter(char *x) {
    return *x;
}

_Exists(T, struct Counter<T>) getMod2Counter() {
    struct Counter<char> ct;
    char *x = malloc(sizeof(char));
    *x = 0;
    ct.state = x;
    ct.inc = toggleMod2Counter;
    ct.dec = toggleMod2Counter;
    ct.getVal = getValMod2Counter;
    return _Pack(ct, _Exists(T, struct Counter<T>), char);
}

int main(int argc, char *argv[]) {
    _Exists(T, struct Counter<T>) counters[2];
    counters[0] = getCounter();
    counters[1] = getMod2Counter();

    for (int i = 0; i < 2; i++) {
        _Unpack (U) struct Counter<U> unpackCt = counters[i];
        for (int j = 0; j < 10; j++) {
            printf("counter = %d\n", unpackCt.getVal(unpackCt.state));
            unpackCt.inc(unpackCt.state);
        }
        printf("---\n");
    }

    // TEST: counter = 0
    // TEST: counter = 1
    // TEST: counter = 2
    // TEST: counter = 3
    // TEST: counter = 4
    // TEST: counter = 5
    // TEST: counter = 6
    // TEST: counter = 7
    // TEST: counter = 8
    // TEST: counter = 9
    // TEST: ---
    // TEST: counter = 0
    // TEST: counter = 1
    // TEST: counter = 0
    // TEST: counter = 1
    // TEST: counter = 0
    // TEST: counter = 1
    // TEST: counter = 0
    // TEST: counter = 1
    // TEST: counter = 0
    // TEST: counter = 1

    return 0;
}
