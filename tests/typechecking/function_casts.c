// Unit tests for typechecking new Checked C function pointers
//
// The following line is for the LLVM test harness:
// RUN: %clang_cc1 -verify -verify-ignore-unexpected=note %s
//

#include <stdchecked.h>

int f0(int a) {
  return a;
}
float g0(float a) {
  return a;
}
int (*bad_f0)(int) = (int(*)(int))0xdeadbeef;

void takes_safe(_Ptr<int(int)> fun) {
  return;
}

// Nulls are specifically allowed
ptr<int(int)> allowed_convert_null0 = 0;
ptr<int(int)> allowed_convert_null1 = (int(*)(int))0;
ptr<int(int)> allowed_convert_null3 = *(int(*)(int))0;
ptr<int(int)> allowed_convert_null4 = &*(int(*)(int))0;
ptr<int(int)> allowed_convert_null6 = ***(int(*)(int))0;

// Named Functions are specifically allowed
ptr<int(int)> allowed_convert1 = f0;
// Pointers to named functions are specifically allowed
ptr<int(int)> allowed_convert2 = &f0;
ptr<int(int)> allowed_convert3 = *f0;
ptr<int(int)> allowed_convert4 = &*f0;
ptr<int(int)> allowed_convert5 = *&f0;
ptr<int(int)> allowed_convert6 = ***f0;

// Arbitrary Data is definitely not allowed
ptr<int(int)> bad_convert1 = (int(*)(int))0xdeadbeef; // expected-error {{can only cast function names or null pointers to checked function pointer type '_Ptr<int (int)>'}}
ptr<int(int)> bad_convert2 = (int(*)(int))g0;         // expected-error {{cast to checked function pointer type '_Ptr<int (int)>' from incompatible type 'float (float)'}}

// Conversions of checked function pointers to/from void pointers are not allowed;
void * void_unchecked_ptr1 = 0;
ptr<void> void_ptr1 = 0;
array_ptr<void> void_array_ptr1 = 0;

ptr<int(int)> bad_convert3 = void_unchecked_ptr1; // expected-error {{incompatible type}}
ptr<int(int)> bad_convert4 = void_ptr1;           // expected-error {{incompatible type}}
ptr<int(int)> bad_convert5 = void_array_ptr1;     // expected-error {{incompatible type}}
ptr<void> void_ptr2 = f0;                         // expected-error {{incompatible type}}
array_ptr<void> void_array_ptr2 = f0;             // expected-error {{incompatible type}}

// Now locally within a function body
void local_convert(int(*f1)(int), ptr<int(int)> f2) {

  //
  // Casts at assignment-time
  //

  ptr<int(int)> local_safe = 0;

  // Nulls are specifically allowed
  ptr<int(int)> safe_null0 = 0;
  ptr<int(int)> safe_null1 = (int(*)(int))0;
  ptr<int(int)> safe_null3 = *(int(*)(int))0;
  ptr<int(int)> safe_null4 = &*(int(*)(int))0;
  ptr<int(int)> safe_null6 = ***(int(*)(int))0;

  // Top-level declared functions are allowed
  ptr<int(int)> safe_f1 = f0;
  ptr<int(int)> safe_f2 = &f0;
  ptr<int(int)> safe_f3 = *f0;
  ptr<int(int)> safe_f4 = &*f0;
  ptr<int(int)> safe_f5 = *&f0;
  ptr<int(int)> safe_f6 = ***f0;

  // Top-level declared functions are allowed
  ptr<int(int)> safe_f7 = f2;
  ptr<int(int)> safe_f8 = *f2;
  ptr<int(int)> safe_f9 = &*f2;
  ptr<int(int)> safe_f10 = *&f2;
  ptr<int(int)> safe_f11 = ***f2;

  // Top-level declared functions are allowed
  ptr<int(int)> safe_f12 = local_safe;
  ptr<int(int)> safe_f13 = *local_safe;
  ptr<int(int)> safe_f14 = &*local_safe;
  ptr<int(int)> safe_f15 = *&local_safe;
  ptr<int(int)> safe_f16 = ***local_safe;

  // Parameter functions are not allowed
  ptr<int(int)> unsafe_f1 = f1;    // expected-error {{cannot guarantee operand of cast to checked function pointer type '_Ptr<int (int)>' is a function pointer}}
  ptr<int(int)> unsafe_f3 = *f1;   // expected-error {{cannot guarantee operand of cast to checked function pointer type '_Ptr<int (int)>' is a function pointer}}
  ptr<int(int)> unsafe_f4 = &*f1;  // expected-error {{cannot guarantee operand of cast to checked function pointer type '_Ptr<int (int)>' is a function pointer}}
  ptr<int(int)> unsafe_f5 = *&f1;  // expected-error {{cannot guarantee operand of cast to checked function pointer type '_Ptr<int (int)>' is a function pointer}}
  ptr<int(int)> unsafe_f6 = ***f1; // expected-error {{cannot guarantee operand of cast to checked function pointer type '_Ptr<int (int)>' is a function pointer}}

  // Arbitary data is definitely not allowed
  ptr<int(int)> bad_convert1 = (int(*)(int))0xdeadbeef; // expected-error {{can only cast function names or null pointers to checked function pointer type '_Ptr<int (int)>'}}
  ptr<int(int)> bad_convert2 = bad_f0;                  // expected-error {{cannot guarantee operand of cast to checked function pointer type '_Ptr<int (int)>' is a function pointer}}
  ptr<int(int)> bad_convert3 = *bad_f0;                 // expected-error {{cannot guarantee operand of cast to checked function pointer type '_Ptr<int (int)>' is a function pointer}}
  ptr<int(int)> bad_convert4 = ***bad_f0;               // expected-error {{cannot guarantee operand of cast to checked function pointer type '_Ptr<int (int)>' is a function pointer}}

  // Conversions of checked function pointers to/from void pointers are not allowed;
  void * void_unchecked_ptr1 = 0;
  ptr<void> void_ptr1 = 0;
  array_ptr<void> void_array_ptr1 = 0;

  ptr<int(int)> bad_convert5 = void_unchecked_ptr1; // expected-error {{incompatible type}}
  ptr<int(int)> bad_convert6 = void_ptr1;           // expected-error {{incompatible type}}
  ptr<int(int)> bad_convert7 = void_array_ptr1;     // expected-error {{incompatible type}}
  ptr<void> void_ptr2 = f0;                         // expected-error {{incompatible type}}
  array_ptr<void> void_array_ptr2 = f0;             // expected-error {{incompatible type}}

  //
  // Casts in a Call
  //

  // Nulls are specifically allowed
  takes_safe(0);
  takes_safe((int(*)(int))0);
  takes_safe(*(int(*)(int))0);
  takes_safe(&*(int(*)(int))0);
  takes_safe(***(int(*)(int))0);

  // Top-level declared functions are allowed
  takes_safe(f0);
  takes_safe(*f0);
  takes_safe(&*f0);
  takes_safe(*&f0);
  takes_safe(***f0);

  // Local checked functions are allowed
  takes_safe(local_safe);
  takes_safe(*local_safe);
  takes_safe(&*local_safe);
  takes_safe(*&local_safe);
  takes_safe(***local_safe);

  // Unchecked parameter functions are not allowed
  takes_safe(f1);    // expected-error {{cannot guarantee operand of cast to checked function pointer type '_Ptr<int (int)>' is a function pointer}}
  takes_safe(*f1);   // expected-error {{cannot guarantee operand of cast to checked function pointer type '_Ptr<int (int)>' is a function pointer}}
  takes_safe(&*f1);  // expected-error {{cannot guarantee operand of cast to checked function pointer type '_Ptr<int (int)>' is a function pointer}}
  takes_safe(*&f1);  // expected-error {{cannot guarantee operand of cast to checked function pointer type '_Ptr<int (int)>' is a function pointer}}
  takes_safe(***f1); // expected-error {{cannot guarantee operand of cast to checked function pointer type '_Ptr<int (int)>' is a function pointer}}

  // Arbitrary data is definitely not allowed
  takes_safe((int(*)(int))0xdeadbeef); // expected-error {{can only cast function names or null pointers to checked function pointer type '_Ptr<int (int)>'}}
  takes_safe(bad_f0);                  // expected-error {{cannot guarantee operand of cast to checked function pointer type '_Ptr<int (int)>' is a function pointer}}
  takes_safe(*bad_f0);                 // expected-error {{cannot guarantee operand of cast to checked function pointer type '_Ptr<int (int)>' is a function pointer}}
  takes_safe(***bad_f0);               // expected-error {{cannot guarantee operand of cast to checked function pointer type '_Ptr<int (int)>' is a function pointer}}

  takes_safe(void_ptr1);        // expected-error {{incompatible type}}
  takes_safe(void_array_ptr1);  // expected-error {{incompatible type}}

  //
  // Explicit User Casts
  //

  int(*local_unsafe)(int) = f0; // This is a valid unchecked to unchecked assignment

  ptr<int(int)> safe_cast_null0 = (ptr<int(int)>)0;
  ptr<int(int)> safe_cast_null1 = (ptr<int(int)>)((int(*)(int))0);
  ptr<int(int)> safe_cast_null3 = (ptr<int(int)>)(*(int(*)(int))0);
  ptr<int(int)> safe_cast_null4 = (ptr<int(int)>)(&*(int(*)(int))0);
  ptr<int(int)> safe_cast_null6 = (ptr<int(int)>)(***(int(*)(int))0);

  // Top-level declared functions are allowed
  ptr<int(int)> safe_cast_f1 = (ptr<int(int)>)(f0);
  ptr<int(int)> safe_cast_f2 = (ptr<int(int)>)(&f0);
  ptr<int(int)> safe_cast_f3 = (ptr<int(int)>)(*f0);
  ptr<int(int)> safe_cast_f4 = (ptr<int(int)>)(&*f0);
  ptr<int(int)> safe_cast_f5 = (ptr<int(int)>)(*&f0);
  ptr<int(int)> safe_cast_f6 = (ptr<int(int)>)(***f0);

  // Checked Parameters are allowed
  ptr<int(int)> safe_cast_f7 = (ptr<int(int)>)(f2);
  ptr<int(int)> safe_cast_f8 = (ptr<int(int)>)(*f2);
  ptr<int(int)> safe_cast_f9 = (ptr<int(int)>)(&*f2);
  ptr<int(int)> safe_cast_f10 = (ptr<int(int)>)(*&f2);
  ptr<int(int)> safe_cast_f11 = (ptr<int(int)>)(***f2);

  // Top-level declared functions are allowed
  ptr<int(int)> safe_cast_f12 = (ptr<int(int)>)(local_safe);
  ptr<int(int)> safe_cast_f13 = (ptr<int(int)>)(*local_safe);
  ptr<int(int)> safe_cast_f14 = (ptr<int(int)>)(&*local_safe);
  ptr<int(int)> safe_cast_f15 = (ptr<int(int)>)(*&local_safe);
  ptr<int(int)> safe_cast_f16 = (ptr<int(int)>)(***local_safe);

  // Parameter functions are not allowed
  ptr<int(int)> unsafe_cast_f1 = (ptr<int(int)>)(f1);    // expected-error {{cannot guarantee operand of cast to checked function pointer type '_Ptr<int (int)>' is a function pointer}}
  ptr<int(int)> unsafe_cast_f3 = (ptr<int(int)>)(*f1);   // expected-error {{cannot guarantee operand of cast to checked function pointer type '_Ptr<int (int)>' is a function pointer}}
  ptr<int(int)> unsafe_cast_f4 = (ptr<int(int)>)(&*f1);  // expected-error {{cannot guarantee operand of cast to checked function pointer type '_Ptr<int (int)>' is a function pointer}}
  ptr<int(int)> unsafe_cast_f5 = (ptr<int(int)>)(*&f1);  // expected-error {{cannot guarantee operand of cast to checked function pointer type '_Ptr<int (int)>' is a function pointer}}
  ptr<int(int)> unsafe_cast_f6 = (ptr<int(int)>)(***f1); // expected-error {{cannot guarantee operand of cast to checked function pointer type '_Ptr<int (int)>' is a function pointer}}

  // Arbitary data is definitely not allowed
  ptr<int(int)> bad_cast_1 = (ptr<int(int)>)((int(*)(int))0xdeadbeef); // expected-error {{can only cast function names or null pointers to checked function pointer type '_Ptr<int (int)>'}}
  ptr<int(int)> bad_cast_2 = (ptr<int(int)>)(bad_f0);                  // expected-error {{cannot guarantee operand of cast to checked function pointer type '_Ptr<int (int)>' is a function pointer}}
  ptr<int(int)> bad_cast_3 = (ptr<int(int)>)(*bad_f0);                 // expected-error {{cannot guarantee operand of cast to checked function pointer type '_Ptr<int (int)>' is a function pointer}}
  ptr<int(int)> bad_cast_4 = (ptr<int(int)>)(***bad_f0);               // expected-error {{cannot guarantee operand of cast to checked function pointer type '_Ptr<int (int)>' is a function pointer}}
  ptr<int(int)> bad_cast_5 = (ptr<int(int)>)(void_ptr1);               // expected-error {{cast to checked function pointer type '_Ptr<int (int)>' from incompatible checked pointer type '_Ptr<void>'}}
  ptr<int(int)> bad_cast_6 = (ptr<int(int)>)(void_array_ptr1);         // expected-error {{cast to checked function pointer type '_Ptr<int (int)>' from incompatible type '_Array_ptr<void>'}}

  //
  // Weird Casts
  //

  ptr<float(float, int)> local_safe2 = 0;
  int(*local_odd)(int) = local_unsafe;
  ptr<int(int)> local_super_unsafe1 = local_unsafe;                // expected-error {{cannot guarantee operand of cast to checked function pointer type '_Ptr<int (int)>' is a function pointer}}
  ptr<int(int)> local_super_unsafe2 = (ptr<int(int)>)local_unsafe; // expected-error {{cannot guarantee operand of cast to checked function pointer type '_Ptr<int (int)>' is a function pointer}}

  //
  // Weird Unary Operators
  //

  // There are more tests in the platform-specific tests

  int(**local_more_unsafe1)(int) = &local_unsafe;
  ptr<int(int)> local_more_unsafe2 = *local_more_unsafe1; // expected-error {{cannot guarantee operand of cast to checked function pointer type '_Ptr<int (int)>' is a function pointer}}

  int local_not_fn;
  ptr<int(int)> local_not_fn2 = (int(*)(int))&local_not_fn; // expected-error {{cast to checked function pointer type '_Ptr<int (int)>' may only take the address of expressions with function type}}

}

struct S1 {
  ptr<int(int, int)> f1;
  int(*f2)(int, int);
};

union U1 {
  ptr<int(int, int)> f1;
  int(*f2)(int, int);
};

void field_read(struct S1 s1, union U1 u1) {
  ptr<int(int, int)> safe1 = s1.f1;
  ptr<int(int, int)> unsafe1 = s1.f2; // expected-error {{cannot guarantee operand of cast to checked function pointer type '_Ptr<int (int, int)>' is a function pointer}}

  // safe2 should probably not be allowed, but it is.
  ptr<int(int, int)> safe2 = u1.f1;
  ptr<int(int, int)> unsafe2 = u1.f2; // expected-error {{cannot guarantee operand of cast to checked function pointer type '_Ptr<int (int, int)>' is a function pointer}}
}

void field_write(struct S1 s1, union U1 u1, ptr<int(int, int)> f1, int(*f2)(int, int)) {
  s1.f1 = f1;
  s1.f1 = f2;  // expected-error {{cannot guarantee operand of cast to checked function pointer type '_Ptr<int (int, int)>' is a function pointer}}
  s1.f2 = f1;  // expected-error {{assigning to 'int (*)(int, int)' from incompatible type '_Ptr<int (int, int)>'}}
  s1.f2 = f2;

  u1.f1 = f1;
  u1.f1 = f2;  // expected-error {{cannot guarantee operand of cast to checked function pointer type '_Ptr<int (int, int)>' is a function pointer}}
  u1.f2 = f1;  // expected-error {{assigning to 'int (*)(int, int)' from incompatible type '_Ptr<int (int, int)>'}}
  u1.f2 = f2;
}

void field_call(struct S1 s1, union U1 u1) {
  s1.f1(0, 0);
  s1.f2(0, 0);
  u1.f1(0, 0);
  u1.f2(0, 0);
}

// Check const function pointers
struct S1_Const {
  const ptr<int(int, int)> f1;
  int(*const f2)(int, int);
};

union U1_Const {
  const ptr<int(int, int)> f1;
  int(*const f2)(int, int);
};

void const_field_read(struct S1_Const s1, union U1_Const u1) {
  ptr<int(int, int)> safe1 = s1.f1;
  ptr<int(int, int)> unsafe1 = s1.f2; // expected-error {{cannot guarantee operand of cast to checked function pointer type '_Ptr<int (int, int)>' is a function pointer}}

  const ptr<int(int, int)> safe1_const = s1.f1;
  const ptr<int(int, int)> unsafe1_const = s1.f2; // expected-error {{cannot guarantee operand of cast to checked function pointer type '_Ptr<int (int, int)>' is a function pointer}}

  // safe2 should probably not be allowed, but it is.
  ptr<int(int, int)> safe2 = u1.f1;
  ptr<int(int, int)> unsafe2 = u1.f2; // expected-error {{cannot guarantee operand of cast to checked function pointer type '_Ptr<int (int, int)>' is a function pointer}}

  const ptr<int(int, int)> safe2_const = u1.f1;
  const ptr<int(int, int)> unsafe2_const = u1.f2; // expected-error {{cannot guarantee operand of cast to checked function pointer type '_Ptr<int (int, int)>' is a function pointer}}
}

void const_field_write(struct S1_Const s1, union U1_Const u1, ptr<int(int, int)> f1, int(*f2)(int, int)) {
  s1.f1 = f1;  // expected-error {{cannot assign to non-static data member 'f1' with const-qualified type '_Ptr<int (int, int)> const'}}
  s1.f1 = f2;  // expected-error {{cannot assign to non-static data member 'f1' with const-qualified type '_Ptr<int (int, int)> const'}}
  s1.f2 = f1;  // expected-error {{cannot assign to non-static data member 'f2' with const-qualified type 'int (*const)(int, int)'}}
  s1.f2 = f2;  // expected-error {{cannot assign to non-static data member 'f2' with const-qualified type 'int (*const)(int, int)'}}

  u1.f1 = f1;  // expected-error {{cannot assign to non-static data member 'f1' with const-qualified type '_Ptr<int (int, int)> const'}}
  u1.f1 = f2;  // expected-error {{cannot assign to non-static data member 'f1' with const-qualified type '_Ptr<int (int, int)> const'}}
  u1.f2 = f1;  // expected-error {{cannot assign to non-static data member 'f2' with const-qualified type 'int (*const)(int, int)'}}
  u1.f2 = f2;  //  expected-error {{cannot assign to non-static data member 'f2' with const-qualified type 'int (*const)(int, int)'}}
}

void const_field_call(struct S1_Const s1, union U1_Const u1) {
  s1.f1(0, 0);
  s1.f2(0, 0);
  u1.f1(0, 0);
  u1.f2(0, 0);
}

// Check _Assume_bounds_casts
void assume_bounds_casts(struct S1 s1, struct S1_Const s1_const, ptr<int(int)> f1, int(*f2)(int, int), void(*g2)(void)) {
  // _Assume_bounds_casts can be used to cast a checked function pointer
  // to a checked function pointer.
  ptr<int(int, int)> safe = _Assume_bounds_cast<ptr<int(int, int)>>(s1.f1);

  // _Assume_bounds_casts can be used to cast an unchecked function pointer
  // to a checked function pointer (if the unchecked pointer's pointee type
  // is compatible with the checked pointer's pointee type).
  ptr<int(int, int)> unsafe_to_safe = _Assume_bounds_cast<ptr<int(int, int)>>(s1.f2);
  const ptr<int(int, int)> unsafe_to_safe_const = _Assume_bounds_cast<const ptr<int(int, int)>>(s1_const.f2);
  s1.f1 = _Assume_bounds_cast<ptr<int(int, int)>>(f2);
  s1.f1 = _Assume_bounds_cast<ptr<int(int, int)>>(s1.f2);

  // _Assume_bounds_casts can be used to cast null to a checked function pointer.
  ptr<int(void)> null_to_safe = _Assume_bounds_cast<ptr<int(void)>>(0);
  s1.f1 = _Assume_bounds_cast<ptr<int(int, int)>>(0);

  // _Assume_bounds_casts cannot be used to cast to a checked function pointer
  // from an incompatible checked or unchecked type.
  ptr<void(void)> incompat_safe = _Assume_bounds_cast<ptr<void(void)>>(f1); // expected-error {{cast to checked function pointer type '_Ptr<void (void)>' from incompatible checked pointer type '_Ptr<int (int)>'}}
  ptr<int(int)> incompat_unsafe = _Assume_bounds_cast<ptr<int(int)>>(f2); // expected-error {{cast to checked function pointer type '_Ptr<int (int)>' from incompatible type 'int (*)(int, int)'}} 
  s1.f1 = _Assume_bounds_cast<ptr<int(int, int)>>(g2); // expected-error {{cast to checked function pointer type '_Ptr<int (int, int)>' from incompatible type 'void (*)(void)'}}
}

// Constness propagated via member access.
struct myops {
  _Ptr<void (void)> myfptr;
};

struct mystruct {
	const struct myops *ops : itype(ptr<const struct myops>);
};

void myfunc(struct mystruct *s) {
	s->ops->myfptr();
}

// Spot check use of bounds-safe interfaces for function pointers
// in checked scopes.
#pragma CHECKED_SCOPE ON

struct myops_checked {
  void ((*myfptr)(void)) : itype(ptr<void (void)>);
};

struct mystruct_checked {
	const struct myops_checked *ops : itype(ptr<const struct myops_checked>);
};

void myfunc_checked(struct mystruct_checked *s : itype(ptr<struct mystruct_checked>)) {
  s->ops->myfptr();
}

#pragma CHECKED_SCOPE OFF

void bounds_safe_interface_assign(struct myops_checked s1, ptr<void(void)> p) {
  s1.myfptr = p;
}
