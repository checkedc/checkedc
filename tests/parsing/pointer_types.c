// Feature tests of parsing new Checked C pointer types.
//
// * Test that checked pointer types can be used as parameter types, return
//   types, local variable types, structure member types, in typedefs, and as
//   arguments to operators that take types.
// * Test parsing of array types and function types combined with checked
//   ponter types.
// * Test the case of nested checked types such as ptr<ptr<T>>, where >>
//   could be mistaken for the right-shift operator.
//
// The following lines are for the LLVM test harness:
//
// RUN: %clang_cc1 -verify -fcheckedc-extension %s
// expected-no-diagnostics

#include "../../include/stdchecked.h"

//
// parameter have new pointer types
//

extern void f1(ptr<int> p, int y) {
   *p = y;
}

extern void f2(const ptr<int> p, int y) {
   *p = y;
}

extern void f3(ptr<const int> p, int y) {
   y = *p;
}

extern void f4(ptr<ptr<int>> p, int y) {
   **p = y;
}

extern void f5(ptr<ptr<ptr<int>>> p, int y) {
   ***p = y;
}

extern void f6(array_ptr<int> p, int y) {
    *p = y;
}

extern void f7(array_ptr<int> p, int y) {
   *p = y;
   f6(p, y);
}

//
// Second parameter is a new pointer type
//

extern void g1(int y, ptr<int> p) {
   *p = y;
}

extern void g2(int y, const ptr<int> p) {
   *p = y;
}

extern void g3(int y, ptr<const int> p) {
   y = *p;
}

extern void g4(int y, ptr<ptr<int>> p) {
   **p = y;
}

extern void g5(int y, ptr<ptr<ptr<int>>> p) {
   ***p = y;
}


extern void g6(int y, array_ptr<int> p) {
   *p = y;
   f7(p, y);
}

//
// returns a new pointer type
//

extern ptr<int> h1(int y, ptr<int> p) {
   *p = y;
   return p;
}

extern const ptr<int> h2(int y, const ptr<int> p) {
   *p = y;
   return p;
}

extern ptr<const int> h3(int y, array_ptr<ptr<const int>> p) {
   y = **p;
   return *p;
}

extern ptr<ptr<int>> h4(int y, ptr<ptr<int>> p) {
   **p = y;
   return p;
}

extern ptr<ptr<ptr<int>>> h5(int y, ptr<ptr<ptr<int>>> p) {
   ***p = y;
   return p;
}

//
// Local variables with pointer types
//

extern void k1(int y)
{
   int v = y;
   ptr<int> t1 = &v;
   array_ptr<int> t2 = &v;
   array_ptr<ptr<int>> t3 = &t1;
   *t1 = 0;
   *t2 = 0;
   *t3 = 0;
}

//
// Struct member with pointer types;
//

struct Vector {
    array_ptr<float> data;
    int len;
};

extern int Multiply(struct Vector vec1, struct Vector vec2) {
    if (vec1.len != vec2.len) {
       return 1;
    }
    for (int i = 0; i < vec1.len; i++) {
        vec1.data[i] *= vec2.data[i];
    }
    return 0;
}

extern int Multiply2(ptr<struct Vector> vec1p, ptr<struct Vector> vec2p) {
    if (vec1p->len != vec2p->len) {
       return 1;
    }
    for (int i = 0; i < vec1p->len; i++) {
        vec1p->data[i] *= vec2p->data[i];
    }
    return 0;
}

//
// Declaring pointers to functions
//

// An unchecked pointer to a function that takes two integer parameters and returns an integer.
int (*unchecked_ptr_to_func)(int x, int y);
// A ptr to a function that takes two integer parameters and returns an integer
ptr<int (int x, int y)> ptrfunc;
// An array_ptr to a function that takes two integer paramters and returns an integer
// Note that because function types have no size, you can't declare bounds or do pointer arithmetic,
// so this is pretty useless.
array_ptr<int (int x, int y)> array_ptrfunc;
// An array_ptr to an array of function pointers.  This, on the other hand, is pretty useful.
// You can declare bounds for it, do pointer arithmetic, and access memory.
array_ptr<ptr<int(int x, int  y)>> array_of_ptrfunc;

//
// Declaring pointers to arrays and arrays of pointers
//
int (*unchecked_ptr_to_array)[5];
ptr<int[5]> ptr_to_array;
array_ptr<int[5]> array_ptr_to_array;

int(*unchecked_ptr_to_incomplete_array)[];
ptr<int[]> ptr_to_incomplete_array;
array_ptr<int[]> array_ptr_to_incomplete_array;

// Declaring arrays of pointers
int *array_of_unchecked_ptrs[5];
ptr<int> array_of_ptrs[5];
array_ptr<int> array_of_array_ptrs[5];

// Declare an unchecked pointer to arrays of pointers
int *(*uncheckedptr_to_array_of_unchecked_ptrs)[5];
ptr<int>(*unchecked_ptr_to_array_of_ptrs)[5];
array_ptr<int>(*unchecked_ptr_to_array_of_array_ptrs)[5];

// Declare ptr to arrays of pointers
ptr<int *[5]> ptr_to_array_of_unchecked_ptrs;
ptr<ptr<int>[5]> ptr_to_array_of_ptrs;
ptr<array_ptr<int>[5]> ptr_to_array_of_array_ptrs;

// Declare ptr to an array of function pointers
ptr<int (*[5])(int x, int y)> ptr_to_array_of_unchecked_func_ptrs;
ptr<ptr<int (int x, int y)>[5]> ptr_to_array_of_checked_func_ptrs;
// Make parameter and return types be ptrs too.
ptr<ptr<ptr<int> (ptr<int> x, ptr<int> y)>[5]> ptr_to_array_of_checked_func_ptrs_with_ptr_parameters;

//
// Typedefs using checked pointer types
//

typedef ptr<int> t_ptr_int;
typedef ptr<int (int x, int y)> t_ptr_func;
typedef array_ptr<int> t_array_ptr_int;
typedef array_ptr<ptr<int>> t_array_ptr_ptr_int;

//
// Operators that take types
//

void parse_operators_with_types(void) {
    int s1 = sizeof(ptr<int>);
    int s2 = sizeof(array_ptr<int>);
    int s3 = sizeof(ptr<int[5]>);
    int s4 = sizeof(array_ptr<int[5]>);
    int s5 = sizeof(ptr<int>[5]);
    int s6 = sizeof(array_ptr<int>[5]);
    // C11 spec says sizeof function types is illegal, but clang accepts it.
    int s7 = sizeof(int(int x, int y));
    int s8 = sizeof(ptr<int>(int x, int y));
    // These are OK
    int s9 = sizeof(ptr<int(int x, int y)>);
    int s10 = sizeof(int(*)(int x, int y));

    int s11 = _Alignof(ptr<int>);
    int s12 = _Alignof(array_ptr<int>);
    int s13 = _Alignof(ptr<int[5]>);
    int s14 = _Alignof(array_ptr<int[5]>);
    int s15 = _Alignof(ptr<int>[5]);
    int s16 = _Alignof(array_ptr<int>[5]);
    // C11 spec says _Alignof function types is illegal, but clang accepts it.
    int s17 = _Alignof(int(int x, int y));
    int s18 = _Alignof(ptr<int>(int x, int y));
    // These are OK
    int s19 = _Alignof(ptr<int(int x, int y)>);
    int s20 = _Alignof(int(*)(int x, int y));

    // Test parsing of some cast operations that should pass checking
    // of bounds declaration
    int x = 0;
    int arr[5];
    ptr<int> px = (ptr<int>) &x;
    array_ptr<int> pax = (array_ptr<int>) &x;
    // ptr to array type
    ptr<int[5]> parr = 0;
    parr = &arr;
    // ptr to function type
    ptr<int (int x, int y)> pfunc = (ptr<int (int x, int y)>) 0;
    ptr<ptr<int (int x, int y)>[5]> ptr_to_pfunc_arr = (ptr<ptr<int (int x, int y)>[5]>) 0;
}
