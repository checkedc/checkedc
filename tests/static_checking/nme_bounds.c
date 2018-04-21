// Tests for Non-Modifying Expressions with Checked C Bounds expressions
//
// The following lines are for the LLVM test harness:
//
// RUN: %clang_cc1 -verify -verify-ignore-unexpected=note %s

#include <stdchecked.h>

int f0int(void);
int* f0ptr(void);

struct S1 {
  int m1;
  int* m2;
};

union U1 {
  int m1;
  int* m2;
};

// Expressions explicitly banned by spec within Non-Modifying Expressions
void f1(int i, int* loc) {
  volatile int j;

  array_ptr<int> as0a : count(i = 1) = 0;         // expected-error {{assignment expression not allowed in count expression}}
  array_ptr<int> as0b : byte_count(i = 1) = 0;    // expected-error {{assignment expression not allowed in byte count expression}}
  array_ptr<int> as0c : bounds(loc = 0, loc) = 0; // expected-error {{assignment expression not allowed in bounds expression}}
  array_ptr<int> as0d : bounds(loc, loc = 0) = 0; // expected-error {{assignment expression not allowed in bounds expression}}

  array_ptr<int> as1a : count(i += 1) = 0;         // expected-error {{assignment expression not allowed in count expression}}
  array_ptr<int> as1b : byte_count(i += 1) = 0;    // expected-error {{assignment expression not allowed in byte count expression}}
  array_ptr<int> as1c : bounds(loc += 1, loc) = 0; // expected-error {{assignment expression not allowed in bounds expression}}
  array_ptr<int> as1d : bounds(loc, loc += 1) = 0; // expected-error {{assignment expression not allowed in bounds expression}}

  array_ptr<int> as2a : count(i -= 1) = 0;         // expected-error {{assignment expression not allowed in count expression}}
  array_ptr<int> as2b : byte_count(i -= 1) = 0;    // expected-error {{assignment expression not allowed in byte count expression}}
  array_ptr<int> as2c : bounds(loc -= 1, loc) = 0; // expected-error {{assignment expression not allowed in bounds expression}}
  array_ptr<int> as2d : bounds(loc, loc -= 1) = 0; // expected-error {{assignment expression not allowed in bounds expression}}

  array_ptr<int> as3a : count(i *= 1) = 0;         // expected-error {{assignment expression not allowed in count expression}}
  array_ptr<int> as3b : byte_count(i *= 1) = 0;    // expected-error {{assignment expression not allowed in byte count expression}}
  array_ptr<int> as3c : bounds(loc + (i *= 1), loc) = 0; // expected-error {{assignment expression not allowed in bounds expression}}
  array_ptr<int> as3d : bounds(loc, loc + (i *= 1)) = 0; // expected-error {{assignment expression not allowed in bounds expression}}

  array_ptr<int> as4a : count(i /= 1) = 0;         // expected-error {{assignment expression not allowed in count expression}}
  array_ptr<int> as4b : byte_count(i /= 1) = 0;    // expected-error {{assignment expression not allowed in byte count expression}}
  array_ptr<int> as4c : bounds(loc + (i /= 1), loc) = 0; // expected-error {{assignment expression not allowed in bounds expression}}
  array_ptr<int> as4d : bounds(loc, loc + (i /= 1)) = 0; // expected-error {{assignment expression not allowed in bounds expression}}

  array_ptr<int> as5a : count(i %= 1) = 0;         // expected-error {{assignment expression not allowed in count expression}}
  array_ptr<int> as5b : byte_count(i %= 1) = 0;    // expected-error {{assignment expression not allowed in byte count expression}}
  array_ptr<int> as5c : bounds(loc + (i %= 1), loc) = 0; // expected-error {{assignment expression not allowed in bounds expression}}
  array_ptr<int> as5d : bounds(loc, loc + (i %= 1)) = 0; // expected-error {{assignment expression not allowed in bounds expression}}

  array_ptr<int> as6a : count(i <<= 1) = 0;         // expected-error {{assignment expression not allowed in count expression}}
  array_ptr<int> as6b : byte_count(i <<= 1) = 0;    // expected-error {{assignment expression not allowed in byte count expression}}
  array_ptr<int> as6c : bounds(loc + (i <<= 1), loc) = 0; // expected-error {{assignment expression not allowed in bounds expression}}
  array_ptr<int> as6d : bounds(loc, loc + (i <<= 1)) = 0; // expected-error {{assignment expression not allowed in bounds expression}}

  array_ptr<int> as7a : count(i >>= 1) = 0;         // expected-error {{assignment expression not allowed in count expression}}
  array_ptr<int> as7b : byte_count(i >>= 1) = 0;    // expected-error {{assignment expression not allowed in byte count expression}}
  array_ptr<int> as7c : bounds(loc + (i >>= 1), loc) = 0; // expected-error {{assignment expression not allowed in bounds expression}}
  array_ptr<int> as7d : bounds(loc, loc + (i >>= 1)) = 0; // expected-error {{assignment expression not allowed in bounds expression}}

  array_ptr<int> as8a : count(i &= 1) = 0;         // expected-error {{assignment expression not allowed in count expression}}
  array_ptr<int> as8b : byte_count(i &= 1) = 0;    // expected-error {{assignment expression not allowed in byte count expression}}
  array_ptr<int> as8c : bounds(loc + (i &= 1), loc) = 0; // expected-error {{assignment expression not allowed in bounds expression}}
  array_ptr<int> as8d : bounds(loc, loc + (i &= 1)) = 0; // expected-error {{assignment expression not allowed in bounds expression}}

  array_ptr<int> as9a : count(i ^= 1) = 0;         // expected-error {{assignment expression not allowed in count expression}}
  array_ptr<int> as9b : byte_count(i ^= 1) = 0;    // expected-error {{assignment expression not allowed in byte count expression}}
  array_ptr<int> as9c : bounds(loc + (i ^= 1), loc) = 0; // expected-error {{assignment expression not allowed in bounds expression}}
  array_ptr<int> as9d : bounds(loc, loc + (i ^= 1)) = 0; // expected-error {{assignment expression not allowed in bounds expression}}

  array_ptr<int> as10a : count(i |= 1) = 0;         // expected-error {{assignment expression not allowed in count expression}}
  array_ptr<int> as10b : byte_count(i |= 1) = 0;    // expected-error {{assignment expression not allowed in byte count expression}}
  array_ptr<int> as10c : bounds(loc + (i |= 1), loc) = 0; // expected-error {{assignment expression not allowed in bounds expression}}
  array_ptr<int> as10d : bounds(loc, loc + (i|= 1)) = 0; // expected-error {{assignment expression not allowed in bounds expression}}

  array_ptr<int> as11a : count(i++) = 0;         // expected-error {{increment expression not allowed in count expression}}
  array_ptr<int> as11b : byte_count(i++) = 0;    // expected-error {{increment expression not allowed in byte count expression}}
  array_ptr<int> as11c : bounds(loc++, loc) = 0; // expected-error {{increment expression not allowed in bounds expression}}
  array_ptr<int> as11d : bounds(loc, loc++) = 0; // expected-error {{increment expression not allowed in bounds expression}}

  array_ptr<int> as12a : count(++i) = 0;         // expected-error {{increment expression not allowed in count expression}}
  array_ptr<int> as12b : byte_count(++i) = 0;    // expected-error {{increment expression not allowed in byte count expression}}
  array_ptr<int> as12c : bounds(++loc, loc) = 0; // expected-error {{increment expression not allowed in bounds expression}}
  array_ptr<int> as12d : bounds(loc, ++loc) = 0; // expected-error {{increment expression not allowed in bounds expression}}

  array_ptr<int> as13a : count(i--) = 0;         // expected-error {{decrement expression not allowed in count expression}}
  array_ptr<int> as13b : byte_count(i--) = 0;    // expected-error {{decrement expression not allowed in byte count expression}}
  array_ptr<int> as13c : bounds(loc--, loc) = 0; // expected-error {{decrement expression not allowed in bounds expression}}
  array_ptr<int> as13d : bounds(loc, loc--) = 0; // expected-error {{decrement expression not allowed in bounds expression}}

  array_ptr<int> as14a : count(--i) = 0;         // expected-error {{decrement expression not allowed in count expression}}
  array_ptr<int> as14b : byte_count(--i) = 0;    // expected-error {{decrement expression not allowed in byte count expression}}
  array_ptr<int> as14c : bounds(--loc, loc) = 0; // expected-error {{decrement expression not allowed in bounds expression}}
  array_ptr<int> as14d : bounds(loc, --loc) = 0; // expected-error {{decrement expression not allowed in bounds expression}}

  array_ptr<int> as15a : count(f0int()) = 0;         // expected-error {{call expression not allowed in count expression}}
  array_ptr<int> as15b : byte_count(f0int()) = 0;    // expected-error {{call expression not allowed in byte count expression}}
  array_ptr<int> as15c : bounds(f0ptr(), loc) = 0; // expected-error {{call expression not allowed in bounds expression}}
  array_ptr<int> as15d : bounds(loc, f0ptr()) = 0; // expected-error {{call expression not allowed in bounds expression}}

  array_ptr<int> as16a : count(f0int() + 1) = 0;         // expected-error {{call expression not allowed in count expression}}
  array_ptr<int> as16b : byte_count(f0int() + 1) = 0;    // expected-error {{call expression not allowed in byte count expression}}
  array_ptr<int> as16c : bounds(f0ptr() + 1, loc) = 0; // expected-error {{call expression not allowed in bounds expression}}
  array_ptr<int> as16d : bounds(loc, f0ptr() + 1) = 0; // expected-error {{call expression not allowed in bounds expression}}

  array_ptr<int> as17a : count(j) = 0;         // expected-error {{volatile expression not allowed in count expression}}
  array_ptr<int> as17b : byte_count(j) = 0;    // expected-error {{volatile expression not allowed in byte count expression}}
  array_ptr<int> as17c : bounds(loc + j, loc) = 0; // expected-error {{volatile expression not allowed in bounds expression}}
  array_ptr<int> as17d : bounds(loc, loc + j) = 0; // expected-error {{volatile expression not allowed in bounds expression}}

  array_ptr<int> as18a : count(i + j) = 0;         // expected-error {{volatile expression not allowed in count expression}}
  array_ptr<int> as18b : byte_count(i + j) = 0;    // expected-error {{volatile expression not allowed in byte count expression}}
  array_ptr<int> as18c : bounds(loc + (i + j), loc) = 0; // expected-error {{volatile expression not allowed in bounds expression}}
  array_ptr<int> as18d : bounds(loc, loc + (i + j)) = 0; // expected-error {{volatile expression not allowed in bounds expression}}
}

// Expressions explicitly allowed by spec within Non-Modifying Expressions
void f2(int i, int* loc) {
  int j;
  int* loc2;
  int* pi;
  int** ploc;
  union U1 u1;
  struct S1 s1;
  struct S1 *ps1;

  array_ptr<int> as0a : count(i) = 0;
  array_ptr<int> as0b : byte_count(i) = 0;
  array_ptr<int> as0c : bounds(loc, loc) = 0;
  array_ptr<int> as0d : bounds(loc, loc) = 0;

  array_ptr<int> as1a : count(j) = 0;
  array_ptr<int> as1b : byte_count(j) = 0;
  array_ptr<int> as1c : bounds(loc2, loc) = 0;
  array_ptr<int> as1d : bounds(loc, loc2) = 0;

  array_ptr<int> as2a : count(i + j) = 0;
  array_ptr<int> as2b : byte_count(i + j) = 0;
  array_ptr<int> as2c : bounds(loc + j, loc) = 0;
  array_ptr<int> as2d : bounds(loc, loc + j) = 0;

  array_ptr<int> as3a : count(0) = 0;
  array_ptr<int> as3b : byte_count(0) = 0;

  array_ptr<int> as4a : count(&i == &j) = 0;
  array_ptr<int> as4b : byte_count(&i == &j) = 0;
  array_ptr<int> as4c : bounds(&i, loc) = 0;
  array_ptr<int> as4d : bounds(loc, &i) = 0;

  array_ptr<int> as5a : count(*pi) = 0;
  array_ptr<int> as5b : byte_count(*pi) = 0;
  array_ptr<int> as5c : bounds(*ploc, loc) = 0;
  array_ptr<int> as5d : bounds(loc, *ploc) = 0;

  array_ptr<int> as6a : count((int)'a') = 0;
  array_ptr<int> as6b : byte_count((int)'a') = 0;
  array_ptr<int> as6c : bounds((int*)0, loc) = 0;
  array_ptr<int> as6d : bounds(loc, (int*)0) = 0;

  array_ptr<int> as7a : count(+i) = 0;
  array_ptr<int> as7b : byte_count(+i) = 0;
  array_ptr<int> as7c : bounds(loc + (+i), loc) = 0;
  array_ptr<int> as7d : bounds(loc, loc + (+i)) = 0;

  array_ptr<int> as8a : count(-i) = 0;
  array_ptr<int> as8b : byte_count(-i) = 0;
  array_ptr<int> as8c : bounds(loc + -i, loc) = 0;
  array_ptr<int> as8d : bounds(loc, loc + -i) = 0;

  array_ptr<int> as9a : count(~i) = 0;
  array_ptr<int> as9b : byte_count(~i) = 0;
  array_ptr<int> as9c : bounds(loc + ~i, loc) = 0;
  array_ptr<int> as9d : bounds(loc, loc + ~i) = 0;

  array_ptr<int> as10a : count(!i) = 0;
  array_ptr<int> as10b : byte_count(!i) = 0;
  array_ptr<int> as10c : bounds(loc + !i, loc) = 0;
  array_ptr<int> as10d : bounds(loc, loc + !i) = 0;

  array_ptr<int> as11a : count(sizeof(i)) = 0;
  array_ptr<int> as11b : byte_count(sizeof(i)) = 0;
  array_ptr<int> as11c : bounds(loc + sizeof(loc), loc) = 0;
  array_ptr<int> as11d : bounds(loc, loc + sizeof(loc)) = 0;

  array_ptr<int> as12a : count(i * 1 / 1 % 1) = 0;
  array_ptr<int> as12b : byte_count(i * 1 / 1 % 1) = 0;
  array_ptr<int> as12c : bounds(loc + (i * 1 / 1 % 1), loc) = 0;
  array_ptr<int> as12d : bounds(loc, loc + (i * 1 / 1 % 1)) = 0;

  array_ptr<int> as13a : count(i + 1 - 1) = 0;
  array_ptr<int> as13b : byte_count(i + 1 - 1) = 0;
  array_ptr<int> as13c : bounds(loc + 1 - 1, loc) = 0;
  array_ptr<int> as13d : bounds(loc, loc + 1 - 1) = 0;

  array_ptr<int> as14a : count(i << 1 >> 1) = 0;
  array_ptr<int> as14b : byte_count(i << 1 >> 1) = 0;
  array_ptr<int> as14c : bounds(loc + (i << 1 >> 1), loc) = 0;
  array_ptr<int> as14d : bounds(loc, loc + (i << 1 >> 1)) = 0;

  array_ptr<int> as15a : count(i < j) = 0;
  array_ptr<int> as15b : byte_count(i < j) = 0;

  array_ptr<int> as16a : count(i > j) = 0;
  array_ptr<int> as16b : byte_count(i > j) = 0;

  array_ptr<int> as17a : count(i <= j) = 0;
  array_ptr<int> as17b : byte_count(i <= j) = 0;

  array_ptr<int> as18a : count(i >= j) = 0;
  array_ptr<int> as18b : byte_count(i >= j) = 0;

  array_ptr<int> as19a : count(i == j) = 0;
  array_ptr<int> as19b : byte_count(i == j) = 0;
  array_ptr<int> as19c : bounds(loc + (loc == loc2), loc) = 0;
  array_ptr<int> as19d : bounds(loc, loc + (loc == loc2)) = 0;

  array_ptr<int> as20a : count(i & j) = 0;
  array_ptr<int> as20b : byte_count(i & j) = 0;

  array_ptr<int> as21a : count(i ^ j) = 0;
  array_ptr<int> as21b : byte_count(i ^ j) = 0;

  array_ptr<int> as22a : count(i | j) = 0;
  array_ptr<int> as22b : byte_count(i | j) = 0;

  array_ptr<int> as23a : count(i && j) = 0;
  array_ptr<int> as23b : byte_count(i && j) = 0;

  array_ptr<int> as24a : count(i || j) = 0;
  array_ptr<int> as24b : byte_count(i || j) = 0;

  array_ptr<int> as25a : count(i ? j : 0) = 0;
  array_ptr<int> as25b : byte_count(i ? j : 0) = 0;
  array_ptr<int> as25c : bounds(i ? loc : loc2, loc) = 0;
  array_ptr<int> as25d : bounds(loc, i ? loc : loc2) = 0;

  array_ptr<int> as26a : count(u1.m1) = 0;
  array_ptr<int> as26b : byte_count(u1.m1) = 0;
  array_ptr<int> as26c : bounds(u1.m2, loc) = 0;
  array_ptr<int> as26d : bounds(loc, u1.m2) = 0;

  array_ptr<int> as27a : count(s1.m1) = 0;
  array_ptr<int> as27b : byte_count(s1.m1) = 0;
  array_ptr<int> as27c : bounds(s1.m2, loc) = 0;
  array_ptr<int> as27d : bounds(loc, s1.m2) = 0;

  array_ptr<int> as28a : count(ps1->m1) = 0;
  array_ptr<int> as28b : byte_count(ps1->m1) = 0;
  array_ptr<int> as28c : bounds(ps1->m2, loc) = 0;
  array_ptr<int> as28d : bounds(loc, ps1->m2) = 0;
}


// Expressions that result in inferred bounds containing modifying expressions.

struct S2 {
  array_ptr<int> p : count(5);
};

void f3_helper(array_ptr<int> p : count(5));

void f3(void) {
  struct S2 arr[10] = { 0 };
  int i = 0;
  // TODO: this will eventually be allowed when we add support for current_expr_value.
  (arr[i++]).p = 0;                         // expected-error {{contain a modifying expression}}
  // TODO: this will eventually be allowed when we add support for current_expr_value.
  array_ptr<int> a : count(5) = arr[i++].p; // expected-error {{contain a modifying expression}}
  // TODO: this will eventually be allowed when we add support for current_expr_value.
  f3_helper(arr[i++].p);                    // expected-error {{contain a modifying expression}}
}

struct S3 {
  array_ptr<int> p : count(len);
  int len;
};

void f4(void) {
  struct S3 arr[10] = { 0 };
  int i = 0;
  int j = arr[i++].p[0];   // expected-error {{contain a modifying expression}}
                           // There is no way to write down the bounds for arr[i++].p.  The bounds of
                           // arr[i++].p need to refer to a temporary holding the value of the subexpression
                           // arr[i++].  We have no way of describing this.
}

void f4_helper(array_ptr<int> p : count(1));

void f5(void) {
  int i = 0;
  ptr<int> p = &i;
  array_ptr<ptr<int>> r : count(1) = &p;
  array_ptr<ptr<int>> s : count(1) = &p;
// TODO: this will eventually be allowed when we add support for current_expr_value.
  _Array_ptr<int> t : count(1) = *(r = s);  // expected-error {{contain a modifying expression}}
// TODO: this will eventually be allowed when we add support for current_expr_value.
  f4_helper(*(r = s));     // expected-error {{contain a modifying expression}}
}
