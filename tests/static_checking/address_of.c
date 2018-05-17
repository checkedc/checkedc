// Feature tests of restrictions on taking the addresses of:
// (1) members used in member bounds
// (2) non-array members with bounds.
// (2) lvalue expressions used in variable bounds.
// (4) non-array variables with bounds.
//
// The following lines are for the LLVM test harness:
//
// RUN: %clang_cc1 -Wno-check-bounds-decls-checked-scope -verify -verify-ignore-unexpected=note  %s

#include <stdchecked.h>

struct S1 {
  _Array_ptr<int> p : count(len);
  int len;
};

struct S2 {
  int padding;
  struct S1 s;
};

extern void f1(struct S1 s1, struct S2 s2) {
   int *p1 = &s1.len;      // expected-error {{cannot take address of member used in member bounds}}
   int *p2 = &(s1.len);    // expected-error {{cannot take address of member used in member bounds}}
   int *p3 = &(s1).len;    // expected-error {{cannot take address of member used in member bounds}}
   int *p4 = &s2.s.len;    // expected-error {{cannot take address of member used in member bounds}}
   int *p5 = &(s2.s.len);  // expected-error {{cannot take address of member used in member bounds}}
   int *p6 = &(s2.s).len;  // expected-error {{cannot take address of member used in member bounds}}
}

extern void f2(struct S1 *s1, struct S2 *s2) {
   int *p1 = &s1->len;      // expected-error {{cannot take address of member used in member bounds}}
   int *p2 = &(s1->len);    // expected-error {{cannot take address of member used in member bounds}}
   int *p3 = &(s1)->len;    // expected-error {{cannot take address of member used in member bounds}}
   int *p4 = &s2->s.len;    // expected-error {{cannot take address of member used in member bounds}}
   int *p5 = &(s2->s.len);  // expected-error {{cannot take address of member used in member bounds}}
   int *p6 = &(s2->s).len;  // expected-error {{cannot take address of member used in member bounds}}
}

extern void f3(struct S1 *s1, struct S2 *s2) {
   _Array_ptr<int> *p1 = &s1->p;   // expected-error {{cannot take address of member with bounds}}
   _Array_ptr<int> *p2 = &(s1->p); // expected-error {{cannot take address of member with bounds}}
   _Array_ptr<int> *p3 = &(s1)->p; // expected-error {{cannot take address of member with bounds}}
   _Array_ptr<int> *p4 = &s2->s.p;    // expected-error {{cannot take address of member with bounds}}
   _Array_ptr<int> *p5 = &(s2->s.p);  // expected-error {{cannot take address of member with bounds}}
   _Array_ptr<int> *p6 = &(s2->s).p;  // expected-error {{cannot take address of member with bounds}}
}

struct S3 {
  int *p : count(len);
  int len;
};

struct S4 {
  int padding;
  struct S3 s;
};

extern void f4(struct S3 s3, struct S4 s4) {
   int *p1 = &s3.len;
   int *p2 = &(s3.len);
   int *p3 = &(s3).len;
   int *p4 = &s4.s.len;
   int *p5 = &(s4.s.len);
   int *p6 = &(s4.s).len;
}

extern void f5(struct S3 s3, struct S4 s4) {
   int **p1 = &s3.p;
   int **p2 = &(s3.p);
   int **p3 = &(s3).p;
   int **p4 = &s4.s.p;
   int **p5 = &(s4.s.p);
   int **p6 = &(s4.s).p;
}

extern checked void f6(struct S3 s3, struct S4 s4) {
   _Ptr<int> p1 = &s3.len;      // expected-error {{cannot take address of member used in member bounds}}
   _Ptr<int> p2 = &(s3.len);    // expected-error {{cannot take address of member used in member bounds}}
   _Ptr<int> p3 = &(s3).len;    // expected-error {{cannot take address of member used in member bounds}}
   _Ptr<int> p4 = &s4.s.len;    // expected-error {{cannot take address of member used in member bounds}}
   _Ptr<int> p5 = &(s4.s.len);  // expected-error {{cannot take address of member used in member bounds}}
   _Ptr<int> p6 = &(s4.s).len;  // expected-error {{cannot take address of member used in member bounds}}
}

extern checked void f7(struct S3 s3, struct S4 s4) {
   _Ptr<_Array_ptr<int>> p1 = &s3.p;     // expected-error {{cannot take address of member with bounds}}
   _Ptr<_Array_ptr<int>> p2 = &(s3.p);   // expected-error {{cannot take address of member with bounds}}
   _Ptr<_Array_ptr<int>> p3 = &(s3).p;   // expected-error {{cannot take address of member with bounds}}
   _Ptr<_Array_ptr<int>> p4 = &s4.s.p;   // expected-error {{cannot take address of member with bounds}}
   _Ptr<_Array_ptr<int>> p5 = &(s4.s.p); // expected-error {{cannot take address of member with bounds}}
   _Ptr<_Array_ptr<int>> p6 = &(s4.s).p; // expected-error {{cannot take address of member with bounds}}
}

extern void f8(struct S3 *s3, struct S4 *s4) {
   int *p1 = &s3->len;
   int *p2 = &(s3->len);
   int *p3 = &(s3)->len;
   int *p4 = &s4->s.len;
   int *p5 = &(s4->s.len);
   int *p6 = &(s4->s).len;
}

extern void f9(struct S3 *s3, struct S4 *s4) {
   int **p1 = &s3->p;
   int **p2 = &(s3->p);
   int **p3 = &(s3)->p;
   int **p4 = &s4->s.p;
   int **p5 = &(s4->s.p);
   int **p6 = &(s4->s).p;
}

extern checked void f10(_Ptr<struct S3> s3, _Ptr<struct S4> s4) {
   _Ptr<int> p1 = &s3->len;      // expected-error {{cannot take address of member used in member bounds}}
   _Ptr<int> p2 = &(s3->len);    // expected-error {{cannot take address of member used in member bounds}}
   _Ptr<int> p3 = &(s3)->len;    // expected-error {{cannot take address of member used in member bounds}}
   _Ptr<int> p4 = &s4->s.len;    // expected-error {{cannot take address of member used in member bounds}}
   _Ptr<int> p5 = &(s4->s.len);  // expected-error {{cannot take address of member used in member bounds}}
   _Ptr<int> p6 = &(s4->s).len;  // expected-error {{cannot take address of member used in member bounds}}
}

extern checked void f11(_Ptr<struct S3> s3, _Ptr<struct S4> s4) {
   _Ptr<_Array_ptr<int>> p1 = &s3->p;      // expected-error {{cannot take address of member with bounds}}
   _Ptr<_Array_ptr<int>> p2 = &(s3->p);    // expected-error {{cannot take address of member with bounds}}
   _Ptr<_Array_ptr<int>> p3 = &(s3)->p;    // expected-error {{cannot take address of member with bounds}}
   _Ptr<_Array_ptr<int>> p4 = &s4->s.p;    // expected-error {{cannot take address of member with bounds}}
   _Ptr<_Array_ptr<int>> p5 = &(s4->s.p);  // expected-error {{cannot take address of member with bounds}}
   _Ptr<_Array_ptr<int>> p6 = &(s4->s).p;  // expected-error {{cannot take address of member with bounds}}
}

// Spot check bounds for an array member with known size.
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
  _Ptr<int _Checked[10]> p1 = &s5->arr; // this is OK because arr can't be modified.
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
  _Ptr<int _Checked[10]> p1 = &s7->arr; // this is OK because arr can't be modified.
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
  _Ptr<int _Checked[10]> p2 = &s8->arr; // this is OK because arr can't be modified.
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
  _Ptr<int _Checked[]> p2 = &s10->arr; // this is OK because arr can't be modified.
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
  _Ptr<int _Checked[]> p2 = &s12->arr; // this is OK because arr can't be modified.
}

