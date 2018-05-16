// Feature tests of restriction on taking the addresses of members
// used in bounds.
//
// The following lines are for the LLVM test harness:
//
// RUN: %clang_cc1 -verify -verify-ignore-unexpected=note  %s

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

struct S3 {
  int *p : count(len);
  int len;
};

struct S4 {
  int padding;
  struct S3 s;
};

extern void f3(struct S3 s3, struct S4 s4) {
   int *p1 = &s3.len;
   int *p2 = &(s3.len);
   int *p3 = &(s3).len;
   int *p4 = &s4.s.len;
   int *p5 = &(s4.s.len);
   int *p6 = &(s4.s).len;
}

extern checked void f4(struct S3 s3, struct S4 s4) {
   _Ptr<int> p1 = &s3.len;      // expected-error {{cannot take address of member used in member bounds}}
   _Ptr<int> p2 = &(s3.len);    // expected-error {{cannot take address of member used in member bounds}}
   _Ptr<int> p3 = &(s3).len;    // expected-error {{cannot take address of member used in member bounds}}
   _Ptr<int> p4 = &s4.s.len;    // expected-error {{cannot take address of member used in member bounds}}
   _Ptr<int> p5 = &(s4.s.len);  // expected-error {{cannot take address of member used in member bounds}}
   _Ptr<int> p6 = &(s4.s).len;  // expected-error {{cannot take address of member used in member bounds}}
}

extern void f5(struct S3 *s3, struct S4 *s4) {
   int *p1 = &s3->len;
   int *p2 = &(s3->len);
   int *p3 = &(s3)->len;
   int *p4 = &s4->s.len;
   int *p5 = &(s4->s.len);
   int *p6 = &(s4->s).len;
}

extern checked void f6(_Ptr<struct S3> s3, _Ptr<struct S4> s4) {
   _Ptr<int> p1 = &s3->len;      // expected-error {{cannot take address of member used in member bounds}}
   _Ptr<int> p2 = &(s3->len);    // expected-error {{cannot take address of member used in member bounds}}
   _Ptr<int> p3 = &(s3)->len;    // expected-error {{cannot take address of member used in member bounds}}
   _Ptr<int> p4 = &s4->s.len;    // expected-error {{cannot take address of member used in member bounds}}
   _Ptr<int> p5 = &(s4->s.len);  // expected-error {{cannot take address of member used in member bounds}}
   _Ptr<int> p6 = &(s4->s).len;  // expected-error {{cannot take address of member used in member bounds}}
}

// Spot check bounds for a flexible array member.
struct S5 {
  int len;
  int arr _Checked[] : count(len);
};

extern void f30(struct S5 *s5) {
  _Ptr<int> p1 = &s5->len;       // expected-error {{cannot take address of member used in member bounds}}
}

struct S6 {
  int len;
  int arr[] : count(len);    
};

extern void f31(struct S6 *s6) {
  int *p1 = &s6->len;
}

extern checked void f32(_Ptr<struct S6> s6) {
  _Ptr<int> p1 = &s6->len;      // expected-error {{cannot take address of member used in member bounds}}
}

