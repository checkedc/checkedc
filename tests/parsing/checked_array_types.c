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
// RUN: %clang_cc1 -verify  -Wno-unused-value %s
// expected-no-diagnostics

//
// parameter have new checked array types
//

#include <stdchecked.h>

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
// Null-terminated array versions
//

extern void f10(int a nt_checked[], int y) {
}

extern void f11(int a nt_checked[5], int y) {
}

extern void f12(const int a nt_checked[5], int y) {
}

// Arrays of null-terminated checked arrays are allowed.
extern void f13(int a checked[]nt_checked[5], int y) {
}

extern void f14(int a checked[5]nt_checked[5], int y) {
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
// Null-terminated checked array versions.
//
extern void g10(int y, int a nt_checked[]) {
}

extern void g11(int y, int a nt_checked[5]) {
}

extern void g12(int y, const int a nt_checked[5]) {
}

// Arrays of null-terminated checked arrays are allowed.
extern void g13(int y, int a checked[]nt_checked[5]) {
}

extern void g14(int y, int a checked[5]nt_checked[5]) {
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
    int arr5 nt_checked[] = { 0, 1, 2, 0 };
    int arr6 nt_checked[5];
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

extern int Multiply2(struct Vector *_Single vec1p, struct Vector *_Single vec2p) {
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

struct FixedLengthString {
  char str nt_checked[5];
};

int (*array_of_unchecked_ptr_to_func checked[10])(int x, int y);
int (*nullterm_array_of_unchecked_ptr_to_func nt_checked[10])(int x, int y);
extern int (*incomplete_array_of_unchecked_ptr_to_func checked[])(int x, int y);
extern int (*nullterm_incomplete_array_of_unchecked_ptr_to_func nt_checked[])(int x, int y);
ptr<int(int x, int  y)> array_of_ptr_to_func checked[10];
extern ptr<int(int x, int  y)> array_of_ptr_to_func checked[];
ptr<int(int x checked[10], int y)> aptr2 checked[10];
ptr<int(int x nt_checked[10], int y)> aptr3 nt_checked[10];
int (*_Single  m_array_of_ptr_to_func checked[10])(int x, int  y);
extern int (*_Single m_array_of_ptr_to_func checked[]) (int x, int  y) ;
int (*_Single m_aptr2 checked[10])(int x checked[10], int y) ;
int (*_Single m_aptr3 nt_checked[10])(int x nt_checked[10], int y);

//
// Declaring pointers to arrays and arrays of pointers
//
int (*unchecked_ptr_to_array)checked[5];
ptr<int checked[5]> ptr_to_array;
ptr<int nt_checked[5]> ptr_to_nullterm_array;
array_ptr<int checked[5]> array_ptr_to_array;
array_ptr<int nt_checked[5]> array_ptr_to_nullterm_array;
int (*_Single m_ptr_to_array) checked[5] ;
int (*_Single m_ptr_to_nullterm_array) nt_checked[5] ;
int (*_Array m_array_ptr_to_array) checked[5] ;
int (*_Array m_array_ptr_to_nullterm_array) nt_checked[5] ;


int(*unchecked_ptr_to_incomplete_array)checked[];
ptr<int checked[]> ptr_to_incomplete_array;
ptr<int nt_checked[]> ptr_to_nullterm_incomplete_array;
array_ptr<int checked[]> array_ptr_to_incomplete_array;
array_ptr<int nt_checked[]> array_ptr_to_nullterm_incomplete_array;
int (*_Array m_array_ptr_to_incomplete_array) checked[] ;
int (*_Array m_array_ptr_to_nullterm_incomplete_array) nt_checked[] ;

// Declaring checked arrays of pointers
int *array_of_unchecked_ptrs checked[5];
int *nullterm_array_of_unchecked_ptrs nt_checked[5];
ptr<int> array_of_ptrs checked[5];
ptr<int> nullterm_array_of_ptrs nt_checked[5];
array_ptr<int> array_of_array_ptrs checked[5];
array_ptr<int> nullterm_array_of_array_ptrs nt_checked[5];
nt_array_ptr<int> array_of_nullterm_array_ptrs checked[5];
nt_array_ptr<int> nullterm_array_of_nullterm_array_ptrs nt_checked[5];
int *_Array m_nullterm_array_of_array_ptrs nt_checked[5];
int *_Nt_array m_array_of_nullterm_array_ptrs checked[5];
int *_Nt_array m_nullterm_array_of_nullterm_array_ptrs nt_checked[5];


// Declare an unchecked pointer to checked arrays of pointers
int *(*uncheckedptr_to_array_of_unchecked_ptrs) checked[5];
ptr<int>(*unchecked_ptr_to_array_of_ptrs) checked[5];
array_ptr<int>(*unchecked_ptr_to_array_of_array_ptrs) checked[5];
array_ptr<int>(*unchecked_ptr_to_null_term_array_of_array_ptrs) nt_checked[5];
int *_Array (*m_unchecked_ptr_to_array_of_array_ptrs) checked[5];
int *_Array (*m_unchecked_ptr_to_null_term_array_of_array_ptrs) nt_checked[5];

// Declare ptr to checked arrays of pointers
ptr<int *checked[5]> ptr_to_array_of_unchecked_ptrs;
ptr<ptr<int> checked[5]> ptr_to_array_of_ptrs;
ptr<array_ptr<int> checked[5]> ptr_to_array_of_array_ptrs;
ptr<array_ptr<int> nt_checked[5]> ptr_to_nullterm_array_of_array_ptrs;
ptr<nt_array_ptr<int> nt_checked[5]> ptr_to_nullterm_array_of_nullterm_array_ptrs;
int (*_Array *_Single m_ptr_to_nullterm_array_of_array_ptrs) nt_checked[5];
int *_Nt_array (*_Single m_ptr_to_nullterm_array_of_nullterm_array_ptrs) nt_checked[5];

// Declare ptr to a checked array of function pointers
ptr<int (*checked[5])(int x, int y)> ptr_to_array_of_unchecked_func_ptrs;
ptr<ptr<int (int x, int y)>checked [5]> ptr_to_array_of_checked_func_ptrs;
ptr<ptr<int (int x, int y)>nt_checked [5]> ptr_to_nullterm_array_of_checked_func_ptrs;
// Make parameter and return types be ptrs too.
ptr<ptr<ptr<int>(ptr<int> x, ptr<int> y)>checked[5]>
ptr_to_array_of_checked_func_ptrs_with_ptr_parameters;
ptr<ptr<ptr<int> (ptr<int> x, ptr<int> y)>nt_checked[5]>
  ptr_to_nullterm_array_of_checked_func_ptrs_with_ptr_parameters;
int (*_Single *_Single *_Single
         m_ptr_to_array_of_checked_func_ptrs_with_ptr_parameters checked[5])(int *_Single  x, int *_Single y);
int (*_Single *_Single *_Single
         m_ptr_to_nullterm_array_of_checked_func_ptrs_with_ptr_parameters nt_checked[5])(int* _Single x, int* _Single y);


//
// Typedefs using checked pointer types
//

typedef int arr_ty[5];
typedef int incomplete_arr_ty[];
typedef int checked_arr_ty checked[5];
typedef int incomplete_checked_array_ty checked[];
typedef int nullterm_checked_arr_ty nt_checked[5];
typedef int nullterm_incomplete_checked_array_ty nt_checked[];

//
// Operators that take types
//

void parse_operators_with_types(void) {
    int s1 = sizeof(int checked[10]);
    int s2 = sizeof(ptr<int checked[5]>);
    int s3 = sizeof(array_ptr<int checked[5]>);
    int s4 = sizeof(int (*_Single)checked[5]);
    int s5 = sizeof(int (*_Single)checked[5]);

    // null-terminated versions.
    int s6 = sizeof(int nt_checked[10]);
    int s7 = sizeof(ptr<int nt_checked[5]>);
    int s8 = sizeof(array_ptr<int nt_checked[5]>);
    int s9 = sizeof(int (*_Single) nt_checked[5]);
    int s10 = sizeof(int (*_Array) nt_checked[5]);

    int s20 = _Alignof(int checked[10]);
    int s21 = _Alignof(ptr<int checked[5]>);
    int s22 = _Alignof(array_ptr<int checked[5]>);
    int s23 = _Alignof(ptr<int> checked[5]);
    int s24 = _Alignof(int (*_Array)checked[5]);
    int s25 = _Alignof(int *_Nt_array checked[5]);

    // null-terminated versions.
    int s26 = _Alignof(int nt_checked[10]);
    int s27 = _Alignof(ptr<int nt_checked[5]>);
    int s28 = _Alignof(array_ptr<int nt_checked[5]>);
    int s29 = _Alignof(ptr<int> nt_checked[5]);
    int s30 = _Alignof(int *_Array nt_checked[5]);
    int s31 = _Alignof(int *_Nt_array nt_checked[5]);

    // Test parsing of some cast operations that should pass checking
    // of bounds declarations
    int x = 0;
    int arr checked[5];
    ptr<int> px = (ptr<int>) &x;
    int* _Array pax = (int* _Array) &x;
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

