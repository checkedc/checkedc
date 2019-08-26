// RUN: %clang %s
// expected-no-diagnostics
//

#include <stdio.h>
#include <stdbool.h>
#include "safe_mm_checked.h"  

typedef struct data {
    uint64_t ID;
    int i;
    int j;
    long l;
    unsigned long ul;
    float f;
} Data;

typedef struct {
    uint64_t ID;
    int val;
    long long_val;
    char c;
} Node;


//
// Test the basic declaration and dereferences to a mmsafe_ptr.
//
void f0() {
    mmsafe_ptr<Node> node_ptr = mmsafe_alloc<Node>(sizeof(Node));

    // Test writing values to a struct.
    (*node_ptr).val = 10;
    (*node_ptr).long_val = 100;
    (*node_ptr).c = 'z';
    printf("node's val = %d.\n", (*node_ptr).val);  // should print 10
    printf("node's long_val = %ld.\n",
        (*node_ptr).long_val);                      // should print 100
    printf("node's c = %c.\n", (*node_ptr).c);      // should print 'z'
    putchar('\n');

    // Test reading values from a struct.
    int val = (*node_ptr).val;
    long long_val = (*node_ptr).long_val;
    char c = (*node_ptr).c;
    printf("val = %d.\n", val);                     // should print 10
    printf("long_val = %ld.\n", long_val);          // should print 100
    printf("c = %c.\n", c);                         // should print 'z'
    putchar('\n');

    // Test Use-after-Free
    mmsafe_free<Node> (node_ptr);
    // There should be a "illegal instruction" core dump for the next line.
    printf("node's val = %d.\n", (*node_ptr).val);  // should abort
}

//
// Test mmsafe_ptr assignment and dereferences to both the pointers.
//
void f1() {
    mmsafe_ptr<Node> node_ptr = mmsafe_alloc<Node>(sizeof(Node));
    (*node_ptr).val = 10;                
    (*node_ptr).long_val = 100;
    (*node_ptr).c = 'z';

    mmsafe_ptr<Node> new_ptr = node_ptr;
    printf("node's val = %d.\n", (*new_ptr).val);   // should print 10
    printf("node's long_val = %ld.\n", 
        (*new_ptr).long_val);                       // should print 100
    printf("node's c = %c.\n", (*new_ptr).c);       // should print 'z'
    putchar('\n');

    (*new_ptr).val = 20;
    (*new_ptr).long_val = 200;
    (*new_ptr).c = 'y';

    // Test if dereferencing the original pointer gives us the correct values.
    printf("node's val = %d.\n", (*node_ptr).val);  // should print 20
    printf("node's long_val = %ld.\n", 
        (*node_ptr).long_val);                      // should print 200
    printf("node's c = %c.\n", (*node_ptr).c);      // should print 'y'
    putchar('\n');

    // Test freeing the new pointer and then dereferencing the old pointer.
    mmsafe_free<Node>(new_ptr);
    // There should be a "illegal instruction" core dump for the next line.
    printf("node's val = %d.\n", (*node_ptr).val); // should abort
}

//
// Test using a mmsafe_ptr of function parameter.
// 
void f2(mmsafe_ptr<Data> p, bool free_it) {
    (*p).i = 10;
    (*p).j = 20;
    (*p).f = 3.14;

    printf("i = %d.\n", (*p).i);                   // should print 10
    printf("j = %d.\n", (*p).j);                   // should print 20
    printf("f = %f.\n", (*p).f);                   // should print 3.14
    putchar('\n');

    if (free_it) mmsafe_free<Data>(p);
}

// 
// Test passing mmsafe_ptr to a function and checking if the pointee's change
// in the callee are visial in the caller.
//
void f3() {
    mmsafe_ptr<Data> data_p = mmsafe_alloc<Data>(sizeof(Data));
    f2(data_p, false);
    
    // The change in the callee should be reflected here.
    printf("i = %d.\n", (*data_p).i);              // should print 10
    printf("j = %d.\n", (*data_p).j);              // should print 20
    printf("f = %f.\n", (*data_p).f);              // should print 3.14
    putchar('\n');

    // Free the struct in the callee.
    f2(data_p, true);
    // There should be a "illegal instruction" core dump for the next line.
    printf("i = %d.\n", (*data_p).i);              // should abort
}

int main() {
#if 1
    f0();
#endif

#if 0
    f1();
#endif

#if 0
    f3();
#endif

    return 0;
}
