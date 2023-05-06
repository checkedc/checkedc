// Feature tests of restrictions on taking the addresses of:
//
// (1) members used in member bounds
// (2) members with member bounds.
// (3) variables with bounds.
// TODO: write tests of lvalue expressions used in variable bounds.
//
// The following lines are for the LLVM test harness:
//
// RUN: %clang_cc1 -Wno-check-bounds-decls-checked-scope -verify -verify-ignore-unexpected=note  %s

#include <stdchecked.h>

// Test taking addresses of members using in bounds expressions for checked members.
struct S1 {
  int* _Array p  _Count(len);
  int len;
};

struct S2 {
  int padding;
  struct S1 s;
};

struct Nested_Len {
  int len;
};

struct S1_Nested {
   _Array_ptr<int> p : count(child.len);
   struct Nested_Len child;
};

extern void f1(struct S1 s1, struct S2 s2, struct S1_Nested s1_nested, struct Nested_Len nested_len) {
   int *p1 = &s1.len;      // expected-error {{cannot take address of member used in member bounds}}
   int *p2 = &(s1.len);    // expected-error {{cannot take address of member used in member bounds}}
   int *p3 = &(s1).len;    // expected-error {{cannot take address of member used in member bounds}}
   int *p4 = &s2.s.len;    // expected-error {{cannot take address of member used in member bounds}}
   int *p5 = &(s2.s.len);  // expected-error {{cannot take address of member used in member bounds}}
   int *p6 = &(s2.s).len;  // expected-error {{cannot take address of member used in member bounds}}

   int *p7 = &s1_nested.child.len;   // expected-error {{cannot take address of member used in member bounds}}
   int *p8 = &(s1_nested.child.len); // expected-error {{cannot take address of member used in member bounds}}
   int *p9 = &(s1_nested.child).len; // expected-error {{cannot take address of member used in member bounds}}
   struct Nested_Len *p10 = &s1_nested.child;    // expected-error {{cannot take address of member used in member bounds}}
   struct Nested_Len *p11 = &(s1_nested).child;  // expected-error {{cannot take address of member used in member bounds}}
   struct Nested_Len *p12 = &(s1_nested.child);  // expected-error {{cannot take address of member used in member bounds}}

   struct Nested_Len *p13 = &nested_len;
   int *p14 = &nested_len.len;

}

extern void f2(struct S1 *s1, struct S2 *s2, struct S1_Nested *s1_nested, struct Nested_Len *nested_len) {
   int *p1 = &s1->len;      // expected-error {{cannot take address of member used in member bounds}}
   int *p2 = &(s1->len);    // expected-error {{cannot take address of member used in member bounds}}
   int *p3 = &(s1)->len;    // expected-error {{cannot take address of member used in member bounds}}
   int *p4 = &s2->s.len;    // expected-error {{cannot take address of member used in member bounds}}
   int *p5 = &(s2->s.len);  // expected-error {{cannot take address of member used in member bounds}}
   int *p6 = &(s2->s).len;  // expected-error {{cannot take address of member used in member bounds}}

   int *p7 = &s1_nested->child.len;   // expected-error {{cannot take address of member used in member bounds}}
   int *p8 = &(s1_nested->child.len); // expected-error {{cannot take address of member used in member bounds}}
   int *p9 = &(s1_nested->child).len; // expected-error {{cannot take address of member used in member bounds}}
   struct Nested_Len *p10 = &s1_nested->child;    // expected-error {{cannot take address of member used in member bounds}}
   struct Nested_Len *p11 = &(s1_nested)->child;  // expected-error {{cannot take address of member used in member bounds}}
   struct Nested_Len *p12 = &(s1_nested->child);  // expected-error {{cannot take address of member used in member bounds}}

   struct Nested_Len *p13 = &*nested_len;
   int *p14 = &nested_len->len;
}

extern void f3(struct S1 *s1, struct S2 *s2, struct S1_Nested *s1_nested, struct Nested_Len *nested_len) {
   _Array_ptr<int> *p1 = &s1->p;   // expected-error {{cannot take address of member with bounds}}
   _Array_ptr<int> *p2 = &(s1->p); // expected-error {{cannot take address of member with bounds}}
   _Array_ptr<int> *p3 = &(s1)->p; // expected-error {{cannot take address of member with bounds}}
   _Array_ptr<int> *p4 = &s2->s.p;    // expected-error {{cannot take address of member with bounds}}
   _Array_ptr<int> *p5 = &(s2->s.p);  // expected-error {{cannot take address of member with bounds}}
   _Array_ptr<int> *p6 = &(s2->s).p;  // expected-error {{cannot take address of member with bounds}}
   _Array_ptr<int> *p7 = &s1_nested->p;    // expected-error {{cannot take address of member with bounds}}
   _Array_ptr<int> *p8 = &(s1_nested->p);  // expected-error {{cannot take address of member with bounds}}
   int* _Array *p9 = &(s1_nested)->p;  // expected-error {{cannot take address of member with bounds}}
}

// Test taking addresses of members with bounds-safe interfaces.
struct S3 {
  int *p : count(len);
  int len;
};

struct S4 {
  int padding;
  struct S3 s;
};

struct S3_Nested {
   int *p : count(child.len);
   struct Nested_Len child;
};

extern void f4(struct S3 s3, struct S4 s4, struct S3_Nested s3_nested) {
   int *p1 = &s3.len;
   int *p2 = &(s3.len);
   int *p3 = &(s3).len;
   int *p4 = &s4.s.len;
   int *p5 = &(s4.s.len);
   int *p6 = &(s4.s).len;

   int *p7 = &s3_nested.child.len;
   int *p8 = &(s3_nested.child.len);
   int *p9 = &(s3_nested.child).len;
   struct Nested_Len *p10 = &s3_nested.child;
   struct Nested_Len *p11 = &(s3_nested).child;
   struct Nested_Len *p12 = &(s3_nested.child);
}

extern checked void f5(struct S3 s3, struct S4 s4, struct S3_Nested s3_nested) {
   _Ptr<int> p1 = &s3.len;      // expected-error {{cannot take address of member used in member bounds}}
   _Ptr<int> p2 = &(s3.len);    // expected-error {{cannot take address of member used in member bounds}}
   _Ptr<int> p3 = &(s3).len;    // expected-error {{cannot take address of member used in member bounds}}
   _Ptr<int> p4 = &s4.s.len;    // expected-error {{cannot take address of member used in member bounds}}
   _Ptr<int> p5 = &(s4.s.len);  // expected-error {{cannot take address of member used in member bounds}}
   _Ptr<int> p6 = &(s4.s).len;  // expected-error {{cannot take address of member used in member bounds}}


   _Ptr<int> p7 = &s3_nested.child.len;              // expected-error {{cannot take address of member used in member bounds}}
   _Ptr<int> p8 = &(s3_nested.child.len);            // expected-error {{cannot take address of member used in member bounds}}
   _Ptr<int> p9 = &(s3_nested.child).len;            // expected-error {{cannot take address of member used in member bounds}}
   _Ptr<struct Nested_Len> p10 = &s3_nested.child;   // expected-error {{cannot take address of member used in member bounds}}
   _Ptr<struct Nested_Len> p11 = &(s3_nested).child; // expected-error {{cannot take address of member used in member bounds}}
   struct Nested_Len* _Single p12 = &(s3_nested.child); // expected-error {{cannot take address of member used in member bounds}}
}

extern void f6(struct S3 *s3, struct S4 *s4, struct S3_Nested *s3_nested) {
   int *p1 = &s3->len;
   int *p2 = &(s3->len);
   int *p3 = &(s3)->len;
   int *p4 = &s4->s.len;
   int *p5 = &(s4->s.len);
   int *p6 = &(s4->s).len;
   int *p7 = &s3_nested->child.len;
   int *p8 = &(s3_nested->child.len);
   int *p9 = &(s3_nested->child).len;
   struct Nested_Len *p10 = &s3_nested->child;
   struct Nested_Len *p11 = &(s3_nested)->child;
   struct Nested_Len *p12 = &(s3_nested->child);
}

extern checked void f7(_Ptr<struct S3> s3, _Ptr<struct S4> s4, _Ptr<struct S3_Nested> s3_nested) {
   _Ptr<int> p1 = &s3->len;      // expected-error {{cannot take address of member used in member bounds}}
   _Ptr<int> p2 = &(s3->len);    // expected-error {{cannot take address of member used in member bounds}}
   _Ptr<int> p3 = &(s3)->len;    // expected-error {{cannot take address of member used in member bounds}}
   _Ptr<int> p4 = &s4->s.len;    // expected-error {{cannot take address of member used in member bounds}}
   _Ptr<int> p5 = &(s4->s.len);  // expected-error {{cannot take address of member used in member bounds}}
   _Ptr<int> p6 = &(s4->s).len;  // expected-error {{cannot take address of member used in member bounds}}

   _Ptr<int> p7 = &s3_nested->child.len;              // expected-error {{cannot take address of member used in member bounds}}
   _Ptr<int> p8 = &(s3_nested->child.len);            // expected-error {{cannot take address of member used in member bounds}}
   _Ptr<int> p9 = &(s3_nested->child).len;            // expected-error {{cannot take address of member used in member bounds}}
   _Ptr<struct Nested_Len> p10 = &s3_nested->child;   // expected-error {{cannot take address of member used in member bounds}}
   _Ptr<struct Nested_Len> p11 = &(s3_nested)->child; // expected-error {{cannot take address of member used in member bounds}}
   struct Nested_Len* _Single p12 = &(s3_nested->child); // expected-error {{cannot take address of member used in member bounds}}
}

extern void f8(struct S3 s3, struct S4 s4, struct S3_Nested s3_nested) {
   int **p1 = &s3.p;
   int **p2 = &(s3.p);
   int **p3 = &(s3).p;
   int **p4 = &s4.s.p;
   int **p5 = &(s4.s.p);
   int **p6 = &(s4.s).p;
   int **p7 = &s3_nested.p;
   int **p8 = &(s3_nested.p);
   int **p9 = &(s3_nested).p;
}

extern void f9(struct S3 *s3, struct S4 *s4,  struct S3_Nested *s3_nested) {
   int **p1 = &s3->p;
   int **p2 = &(s3->p);
   int **p3 = &(s3)->p;
   int **p4 = &s4->s.p;
   int **p5 = &(s4->s.p);
   int **p6 = &(s4->s).p;
   int **p7 = &s3_nested->p;
   int **p8 = &(s3_nested->p);
   int **p9 = &(s3_nested)->p;
}

extern checked void f10(struct S3 s3, struct S4 s4, struct S3_Nested s3_nested) {
   _Ptr<_Array_ptr<int>> p1 = &s3.p;     // expected-error {{cannot take address of member with bounds}}
   _Ptr<_Array_ptr<int>> p2 = &(s3.p);   // expected-error {{cannot take address of member with bounds}}
   _Ptr<_Array_ptr<int>> p3 = &(s3).p;   // expected-error {{cannot take address of member with bounds}}
   _Ptr<_Array_ptr<int>> p4 = &s4.s.p;   // expected-error {{cannot take address of member with bounds}}
   _Ptr<_Array_ptr<int>> p5 = &(s4.s.p); // expected-error {{cannot take address of member with bounds}}
   _Ptr<_Array_ptr<int>> p6 = &(s4.s).p; // expected-error {{cannot take address of member with bounds}}
   _Ptr<_Array_ptr<int>> p7 = &s3_nested.p;   // expected-error {{cannot take address of member with bounds}}
   _Ptr<_Array_ptr<int>> p8 = &(s3_nested.p); // expected-error {{cannot take address of member with bounds}}
   int* _Array *_Single p9 = &(s3_nested).p; // expected-error {{cannot take address of member with bounds}}
}

extern checked void f11(_Ptr<struct S3> s3, _Ptr<struct S4> s4, _Ptr<struct S3_Nested> s3_nested) {
   _Ptr<_Array_ptr<int>> p1 = &s3->p;      // expected-error {{cannot take address of member with bounds}}
   _Ptr<_Array_ptr<int>> p2 = &(s3->p);    // expected-error {{cannot take address of member with bounds}}
   _Ptr<_Array_ptr<int>> p3 = &(s3)->p;    // expected-error {{cannot take address of member with bounds}}
   _Ptr<_Array_ptr<int>> p4 = &s4->s.p;    // expected-error {{cannot take address of member with bounds}}
   _Ptr<_Array_ptr<int>> p5 = &(s4->s.p);  // expected-error {{cannot take address of member with bounds}}
   _Ptr<_Array_ptr<int>> p6 = &(s4->s).p;  // expected-error {{cannot take address of member with bounds}}
   _Ptr<_Array_ptr<int>> p7 = &s3_nested->p;   // expected-error {{cannot take address of member with bounds}}
   _Ptr<_Array_ptr<int>> p8 = &(s3_nested->p); // expected-error {{cannot take address of member with bounds}}
   int* _Array *_Single p9 = &(s3_nested)->p; // expected-error {{cannot take address of member with bounds}}
}

//
// Check taking address of array members and member used in bounds for array members.
//

struct S5 {
   int arr _Checked[10];
};

struct S6 {
  int len;
  int arr _Checked[10] : count(len);
};

struct S7 {
  int arr[10] : count(10);
};

struct S8 {
  int len;
  int arr[10] : count(len);
};


extern void f30(struct S5 *s5) {
  int(* _Single p1) _Checked[10] = &s5->arr; // this is OK because arr can't be modified.
}

extern void f31(struct S6 *s6) {
  _Ptr<int> p1 = &s6->len;           // expected-error {{cannot take address of member used in member bounds}}
  _Ptr<int _Checked[10]> p2 = &s6->arr; // this is OK because arr can't be modified.
}


extern void f32(struct S7 *s7) {
  _Ptr<int _Checked[10]> p1 = &s7->arr; // this is OK because arr can't be modified.
}

extern checked void f33(_Ptr<struct S7> s7) {
  _Ptr<int _Checked[10]> p1 = &s7->arr; // this is OK because arr can't be modified.
}

extern checked void f33a(struct S7 *s7 : itype(_Ptr<struct S7>)) {
  int(* _Single p1) _Checked[10]= &s7->arr; // this is OK because arr can't be modified.
}


extern void f34(struct S8 *s8) {
  _Ptr<int> p1 = &s8->len;
  _Ptr<int _Checked[10]> p2 = &s8->arr; // this is OK because arr can't be modified.
}

extern checked void f35(_Ptr<struct S8> s8) {
  _Ptr<int> p1 = &s8->len;           // expected-error {{cannot take address of member used in member bounds}}
  _Ptr<int _Checked[10]> p2 = &s8->arr; // this is OK because arr can't be modified.
}

extern checked void f35a(struct S8 *s8 : itype(_Ptr<struct S8>)) {
  _Ptr<int> p1 = &s8->len;           // expected-error {{cannot take address of member used in member bounds}}
  int (* _Single p2) _Checked[10]= &s8->arr; // this is OK because arr can't be modified.
}

// Spot check bounds for a flexible array member.

struct S9 {
  int padding;
  int arr _Checked[] : count(0);
};

struct S10 {
  int len;
  int arr _Checked[] : count(len);
};

struct S11 {
  int padding;
  int arr[] : count(0);
};

struct S12 {
  int len;
  int arr[] : count(len);    
};

extern void f40(struct S9 *s9) {
  _Ptr<int _Checked[]> p1 = &s9->arr;  // this is OK because arr can't be modified.
}


extern void f41(struct S10 *s10) {
  _Ptr<int> p1 = &s10->len;            // expected-error {{cannot take address of member used in member bounds}}
  int (*_Single p2) _Checked[] = &s10->arr; // this is OK because arr can't be modified.
}

extern void f42(struct S11 *s11) {
  int (*p1) _Checked[] = &s11->arr; // this is OK because arr can't be modified.
}

extern checked void f43(_Ptr<struct S11> s11) {
  _Ptr<int _Checked[]> p1 = &s11->arr; // this is OK because arr can't be modified.
}

extern void f44(struct S12 *s12) {
  int *p1 = &s12->len;
 int (*p2) _Checked[] = &s12->arr; // this is OK because arr can't be modified.
}

extern checked void f45(_Ptr<struct S12> s12) {
  _Ptr<int> p1 = &s12->len;      // expected-error {{cannot take address of member used in member bounds}}
  int (*_Single p2) _Checked[] = &s12->arr; // this is OK because arr can't be modified.
}

//
// Spot check taking address of members used other bounds expressions besides count.
// 

struct S20 {
  _Array_ptr<int> p : bounds(p, p + 5);
  _Array_ptr<int> mid : bounds(upper, lower);
  _Array_ptr<int> upper, lower;
  _Array_ptr<int> q : byte_count(len);
  int len;
};

void f46(struct S20 s) {
  _Ptr<_Array_ptr<int>> p1 = &s.p;      // expected-error {{cannot take address of member with bounds}} expected-error {{cannot take address of member used in member bounds}}
  _Ptr<_Array_ptr<int>> p2 = &s.mid;    // expected-error {{cannot take address of member with bounds}}
  _Ptr<_Array_ptr<int>> p3 = &s.upper;  // expected-error {{cannot take address of member used in member bounds}}
  _Ptr<_Array_ptr<int>> p4 = &s.upper;  // expected-error {{cannot take address of member used in member bounds}}
  _Ptr<_Array_ptr<int>> p5 = &s.q;      // expected-error {{cannot take address of member with bounds}}
  int* _Single  p6 = &s.len;                // expected-error {{cannot take address of member used in member bounds}}
}


//
// Check taking the address of variables with bounds.
//

// Checked variables with bounds declarations.
int global_len1;
_Array_ptr<int> global_var1 : count(global_len1); 
_Ptr<_Array_ptr<int>> pglobal1 = &global_var1; // expected-error {{cannot take address of variable 'global_var1' with bounds}}

extern int global_arr1 _Checked[] : count(global_len1);
_Ptr<int _Checked[]>  parr = &global_arr1;

extern void f60(_Array_ptr<int> x : count(len), int len) {
  _Ptr<_Array_ptr<int>> px1 = &x;    // expected-error {{cannot take address of variable 'x' with bounds}}
   _Ptr<_Array_ptr<int>> px2 = &(x); // expected-error {{cannot take address of variable 'x' with bounds}}
  _Array_ptr<int> y : count(len) = x;
  _Ptr<_Array_ptr<int>> py = &y;     // expected-error {{cannot take address of variable 'y' with bounds}}
  _Ptr<_Array_ptr<int>> pg = &global_var1;  // expected-error {{cannot take address of variable 'global_var1' with bounds}}
  _Ptr<int _Checked[]>  parr1 = &global_arr1;
  int (*_Single parr2) _Checked[]= &(global_arr1);


  if (len >= 0 && len < 10) {
     int arr _Checked[10] : count(len);
     _Ptr<int _Checked[10]> parr = &arr;
  }
}

// Variables with bounds-safe interfaces
int global_len2;
int *global_var2 : count(global_len2); 
int **pglobal2 = &global_var2;

extern int global_arr2[] : count(global_len1);
int (*parr2) _Checked[] = &global_arr2;

extern void f61(int *x : count(len), int len) {
  int **px = &x;
  int **pg = &global_var2;
}

extern _Checked void f62(int *x : count(len), int len) {
  _Ptr<_Array_ptr<int>> px1 = &x;          // expected-error {{cannot take address of variable 'x' with bounds}}
  _Ptr<_Array_ptr<int>> px2 = &(x);        // expected-error {{cannot take address of variable 'x' with bounds}}
  _Ptr<_Array_ptr<int>> px3 = (&x);        // expected-error {{cannot take address of variable 'x' with bounds}}
  int* _Array *_Single pg = &global_var2; // expected-error {{cannot take address of variable 'global_var2' with bounds}}

  _Ptr<int _Checked[]>  parr1 = &global_arr2;
  _Ptr<int _Checked[]>  parr2 = &(global_arr2);
}
