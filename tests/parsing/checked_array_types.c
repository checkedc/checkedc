// Feature tests of parsing new Checked C checked array type
//
// * Test that checked array types can be used as parameter types, local 
//   variable types, structure member types, in typedefs, and as arguments
//   to operators that take types.  In C, return types for functions
//   cannot be array types.
// * Test parsing of array types and function types combined with checked
//   array types.
//
// The following lines are for the LLVM test harness:
//
// RUN: %clang_cc1 -verify -fcheckedc-extension  -Wno-unused-value %s
// expected-no-diagnostics

//
// parameter have new checked array types
//

extern void f1(int a checked[], int y) {
}

extern void f2(int a checked[5], int y) {
}

extern void f3(const int a checked[5], int y) {
}

extern void f4(int a checked[][5], int y) {
}

extern void f5(int a checked[5][5], int y) {
}

// not necessary, but syntactically legal
extern void f6(int a checked[]checked[5], int y) {
}

// not necessary, but syntactically legal
extern void f7(int a checked[5]checked[5], int y) {
}

//
// Second parameter is a new pointer type
//

extern void g1(int y, int a checked[]) {
}

extern void g2(int y, int a checked[5]) {
}

extern void g3(int y, const int a checked[5]) {
}

extern void g4(int y, int a checked[][5]) {
}

extern void g5(int y, int a checked[5][5]) {
}

// not necessary, but syntactically legal
extern void g6(int y, int a checked[]checked[5]) {
}

// not necessary, but syntactically legal
extern void g7(int y, int a checked[5]checked[5]) {
}

//
// Local variables with pointer types
//

extern void k1(int y)
{
    int arr1 checked[] = { 0, 1 , 2 };
    int arr2 checked[5];
    int arr3 checked[][5] = { { 1 }, {2 } };
    int arr4 checked[5][5];
}

//
// Struct member with fixed-sized array types
//

struct Vector {
    int data checked[5];
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
// Declaring checked arrays of function pointers
//


int (*array_of_unchecked_ptr_to_func checked[10])(int x, int y);
extern int (*incomplete_array_of_unchecked_ptr_to_func checked[])(int x, int y);
ptr<int(int x, int  y)> array_of_ptr_to_func checked[10];
extern ptr<int(int x, int  y)> array_of_ptr_to_func checked[];
ptr<int(int x checked[10], int y)> aptr2 checked[10];

//
// Declaring pointers to arrays and arrays of pointers
//
int (*unchecked_ptr_to_array)checked[5];
ptr<int checked[5]> ptr_to_array;
array_ptr<int checked[5]> array_ptr_to_array;

int(*unchecked_ptr_to_incomplete_array)checked[];
ptr<int checked[]> ptr_to_incomplete_array;
array_ptr<int checked[]> array_ptr_to_incomplete_array;

// Declaring checked arrays of pointers
int *array_of_unchecked_ptrs checked[5];
ptr<int> array_of_ptrs checked[5];
array_ptr<int> array_of_array_ptrs checked[5];

// Declare an unchecked pointer to checked arrays of pointers
int *(*uncheckedptr_to_array_of_unchecked_ptrs) checked[5];
ptr<int>(*unchecked_ptr_to_array_of_ptrs) checked[5];
array_ptr<int>(*unchecked_ptr_to_array_of_array_ptrs) checked[5];

// Declare ptr to checked arrays of pointers
ptr<int *checked[5]> ptr_to_array_of_unchecked_ptrs;
ptr<ptr<int> checked[5]> ptr_to_array_of_ptrs;
ptr<array_ptr<int> checked[5]> ptr_to_array_of_array_ptrs;

// Declare ptr to a checked array of function pointers
ptr<int (*checked[5])(int x, int y)> ptr_to_array_of_unchecked_func_ptrs;
ptr<ptr<int (int x, int y)>checked [5]> ptr_to_array_of_checked_func_ptrs;
// Make parameter and return types be ptrs too.
ptr<ptr<ptr<int> (ptr<int> x, ptr<int> y)>checked[5]> ptr_to_array_of_checked_func_ptrs_with_ptr_parameters;

//
// Typedefs using checked pointer types
//

typedef int arr_ty[5];
typedef int incomplete_arr_ty[];
typedef int checked_arr_ty checked[5];
typedef int incomplete_checked_array_ty checked[];

//
// Operators that take types
//

void parse_operators_with_types() {
    int s1 = sizeof(int checked[10]);
    int s3 = sizeof(ptr<int checked[5]>);
    int s4 = sizeof(array_ptr<int checked[5]>);
    int s5 = sizeof(ptr<int>checked[5]);
    int s6 = sizeof(array_ptr<int> checked[5]);

    int s11 = _Alignof(int checked[10]);
    int s13 = _Alignof(ptr<int checked[5]>);
    int s14 = _Alignof(array_ptr<int checked[5]>);
    int s15 = _Alignof(ptr<int> checked[5]);
    int s16 = _Alignof(array_ptr<int>checked[5]);

    // Test parsing of some cast operations that should pass checking
    // of bounds declarations
    int x = 0;
    int arr checked[5];
    ptr<int> px = (ptr<int>) &x;
    array_ptr<int> pax = (array_ptr<int>) &x;
    pax = arr;
    // ptr to array type
    ptr<int checked[5]> parr = 0;
    parr = &arr;
    parr = (int (*)checked[5]) &arr;
    parr = (int (*)checked[5]) ((int (*)checked[]) &arr);
    // ptr to function type
    ptr<int (int x, int y)> pfunc = (ptr<int (int x, int y)>) 0;
    ptr<ptr<int (int x, int y)>checked [5]> ptr_to_pfunc_arr = (ptr<ptr<int (int x, int y)>checked[5]>) 0;
}

