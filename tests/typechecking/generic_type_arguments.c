// Test type arguments at calls to generic functions in various kinds of
// scopes, including omitting type arguments.   In unchecked scopes, when
// type arguments are omitted, void is used for each type argument instead.
//
// RUN: %clang_cc1 -verify -verify-ignore-unexpected=note %s

_Itype_for_any(T) int f1(void *p : itype(_Array_ptr<T>) byte_count(len), int len);

_Itype_for_any(T, S) int f2(void *p : itype(_Array_ptr<T>) byte_count(len), 
                            void *q : itype(_Array_ptr<S>) byte_count(len),
                            int len);

_Itype_for_any(T) void *alloc(unsigned int len): itype(_Array_ptr<T>) byte_count(len);
_Itype_for_any(T, S) void *alloc2(unsigned int len): itype(_Array_ptr<S>) byte_count(len);

// For each kind of checked scope, there are tests of the following cases:
// - Type applications at function calls where argument and parameter types match or
//   the expected return type and the declared return type match.
// - Type applications at function calls where argument and parameter types mismatch pr
//   the expected return type and the declared return type mismatch.
// - Type applications at function calls where implicit void pointer casts occur for
//   parameters or returns.
// - Function calls where type parameters are omitted.  This cause the type parameters
//   to be instantiated to void by default and for implicit void pointer casts to occur.
//   for parameters or returns.

//
// Checked scopes
//

void test1(_Array_ptr<int> ip : byte_count(len), 
           _Array_ptr<float> fp : byte_count(len),
              int len) _Checked {
  f1<float>(fp, len);
  f1<int>(ip, len);
  _Ptr<double> p = alloc<double>(sizeof(double));
  ip = alloc<int>(len);  //expected-warning {{cannot prove declared bounds}}
                         // Github issue #595: improve bounds declaration checking involving unsigned/signed int conversions.
}

void test2(_Array_ptr<int> ip : byte_count(len), 
           _Array_ptr<float> fp : byte_count(len),
              int len) _Checked {
  f1<float>(ip, len);  // expected-error {{incompatible type}}
  f1<int>(fp, len);    // expected-error {{incompatible type}}
  _Ptr<int> p = alloc<double>(sizeof(double)); // expected-error {{incompatible type}}
  ip = alloc<float>(len);                      // expected-error {{incompatible type}}
}

void test3(_Array_ptr<int> ip : byte_count(len), _Array_ptr<float> fp : byte_count(len),
           int len) _Checked {
  f1<void>(ip, len);   // TODO: this should be an error.
  f1<void>(fp, len);   // TODO: this should be an error.
  _Ptr<double> p = alloc<void>(sizeof(double));  // TODO: Github issue #571: this should be an error.
  ip = alloc<void>(len);                         // TODO: Github issue #571: this should be an error.  TODO: Github issue #595: expected-warning {{cannot prove declared bounds}}
}

void test4(_Array_ptr<int> ip : count(len), _Array_ptr<float> fp : count(len),
           int len) _Checked {
  f1(ip);  // expected-error {{expected a type argument list for a bounds-safe interface call in a checked scope}}
  f1(ip);  // expected-error {{expected a type argument list for a bounds-safe interface call in a checked scope}}
  _Ptr<double> p = alloc(sizeof(double));  // expected-error {{expected a type argument list for a bounds-safe interface call in a checked scope}}
  ip = alloc(len);                          // expected-error {{expected a type argument list for a bounds-safe interface call in a checked scope}}
}

//
// Checked bounds_only scopes
//

void test11(_Array_ptr<int> ip : byte_count(len), 
           _Array_ptr<float> fp : byte_count(len),
              int len) _Checked _Bounds_only {
  f1<float>(fp, len);
  f1<int>(ip, len);
  _Ptr<double> p = alloc<double>(sizeof(double));
  ip = alloc<int>(len);  // TODO: Github issue #595: expected-warning {{cannot prove declared bounds}}
}

void test12(_Array_ptr<int> ip : byte_count(len), 
           _Array_ptr<float> fp : byte_count(len),
              int len) _Checked _Bounds_only {
  f1<float>(ip, len);  // expected-error {{incompatible type}}
  f1<int>(fp, len);    // expected-error {{incompatible type}}
  _Ptr<int> p = alloc<double>(sizeof(double)); // expected-error {{incompatible type}}
  ip = alloc<float>(len);                      // expected-error {{incompatible type}}
}


void test13(_Array_ptr<int> ip : byte_count(len), _Array_ptr<float> fp : byte_count(len),
           int len) _Checked _Bounds_only {
  f1<void>(ip, len);
  f1<void>(fp, len);
  _Ptr<double> p = alloc<void>(sizeof(double));
  ip = alloc<void>(len);  // TODO: Github issue #595: expected-warning {{cannot prove declared bounds}}
}

void test14(_Array_ptr<int> ip : byte_count(len), 
           _Array_ptr<float> fp : byte_count(len),
           int len) _Checked _Bounds_only {
  f1(ip, len);  // expected-error {{expected a type argument list for a bounds-safe interface call in a checked scope}}
  f1(fp, len);  // expected-error {{expected a type argument list for a bounds-safe interface call in a checked scope}}
  _Ptr<double> p = alloc(sizeof(double)); // expected-error {{expected a type argument list for a bounds-safe interface call in a checked scope}}
  ip = alloc(len);  // expected-error {{expected a type argument list for a bounds-safe interface call in a checked scope}}
}

//
// Unchecked scopes
//

void test21(_Array_ptr<int> ip : byte_count(len), 
           _Array_ptr<float> fp : byte_count(len),
              int len) _Unchecked {
  f1<float>(fp, len);
  f1<int>(ip, len);
  _Ptr<double> p = alloc<double>(sizeof(double));
  ip = alloc<int>(len);  // TODO: Github issue #595: expected-warning {{cannot prove declared bounds}}
}

void test22(_Array_ptr<int> ip : byte_count(len), 
           _Array_ptr<float> fp : byte_count(len),
              int len) _Unchecked {
  f1<float>(ip, len);  // expected-error {{incompatible type}}
  f1<int>(fp, len);    // expected-error {{incompatible type}}
  _Ptr<int> p = alloc<double>(sizeof(double)); // TODO: Github issue #587: this should be a type error.
  ip = alloc<float>(len);                      // TODO: Github issue #587: this should be a type error TODO: Github issue #595: expected-warning {{cannot prove declared bounds}}.

}


void test23(_Array_ptr<int> ip : byte_count(len), _Array_ptr<float> fp : byte_count(len),
           int len) _Unchecked {
  f1<void>(ip, len);
  f1<void>(fp, len);
  _Ptr<double> p = alloc<void>(sizeof(double));
  ip = alloc<void>(len);  // TODO: Github issue #595: expected-warning {{cannot prove declared bounds}}
}

void test24(_Array_ptr<int> ip : byte_count(len), 
           _Array_ptr<float> fp : byte_count(len),
           int len) _Unchecked {
  f1(ip, len);
  f1(fp, len);
  _Ptr<double> p = alloc(sizeof(double));
  ip = alloc(len);  // TODO: Github issue #595: expected-warning {{cannot prove declared bounds}}
}

//
// Unchecked scopes by default
//

void test31(_Array_ptr<int> ip : byte_count(len), 
            _Array_ptr<float> fp : byte_count(len),
            int len) {
  f1<float>(fp, len);
  f1<int>(ip, len);
  _Ptr<double> p = alloc<double>(sizeof(double));
  ip = alloc<int>(len);  // TODO: Github issue #595: expected-warning {{cannot prove declared bounds}}
}


void test32(_Array_ptr<int> ip : byte_count(len), 
           _Array_ptr<float> fp : byte_count(len),
              int len) {
  f1<float>(ip, len);  // expected-error {{incompatible type}}
  f1<int>(fp, len);    // expected-error {{incompatible type}}
  _Ptr<int> p = alloc<double>(sizeof(double)); // TODO: Github issue #587: this should be a type error.
  ip = alloc<float>(len);                      // TODO: Github issue #587: this should be a type error.  TODO: Github issue #595: expected-warning {{cannot prove declared bounds}}
}


void test33(_Array_ptr<int> ip : byte_count(len), _Array_ptr<float> fp : byte_count(len),
           int len) {
  f1<void>(ip, len);
  f1<void>(fp, len);
  _Ptr<int> p = alloc<void>(sizeof(double));
  ip = alloc<void>(len);  // TODO: Github issue #595: expected-warning {{cannot prove declared bounds}}
}

void test34(_Array_ptr<int> ip : byte_count(len), 
           _Array_ptr<float> fp : byte_count(len),
           int len) _Unchecked {
  f1(ip, len);
  f1(fp, len);
  _Ptr<double> p = alloc(sizeof(double));
  ip = alloc(len);  // TODO: Github issue #595: expected-warning {{cannot prove declared bounds}}
}

//-------------------------------------------------
//
// Check multiple type arguments
//
//-------------------------------------------------

void test41(_Array_ptr<int> ip : byte_count(len), 
           _Array_ptr<float> fp : byte_count(len),
              int len) _Checked {
  f2<float, int>(fp, ip, len);
  f2<int, float>(ip, fp, len);
  _Ptr<double> p = alloc<double>(sizeof(double));
  ip = alloc<int>(len);  // TODO: Github issue #595: expected-warning {{cannot prove declared bounds}}
}


void test42(_Array_ptr<int> ip : byte_count(len), 
            _Array_ptr<float> fp : byte_count(len),
              int len) _Checked {
  f2<float, int>(ip, ip, len);  // expected-error {{incompatible type}}
  f2<int, float>(fp, fp, len);  // expected-error {{incompatible type}}
  _Ptr<int> p = alloc<double>(sizeof(double)); // expected-error {{incompatible type}}
  ip = alloc<float>(len);                      // expected-error {{incompatible type}}
}

void test43(_Array_ptr<int> ip : byte_count(len), _Array_ptr<float> fp : byte_count(len),
           int len) _Checked {
  f2<void, void>(ip, ip, len);   // TODO: this should be an error.
  f2<void, void>(ip, fp, len);   // TODO: this should be an error.
  f2<void, void>(fp, fp, len);   // TODO: this should be an error.
  f2<void, void>(fp, ip, len);   // TODO: this should be an error.
  _Ptr<double> p = alloc2<void, void>(sizeof(double));  // TODO: this should be an error.
  ip = alloc2<void, void>(len);  // TODO: Github issue #595: expected-warning {{cannot prove declared bounds}}

}

void test44(_Array_ptr<int> ip : byte_count(len), _Array_ptr<float> fp : byte_count(len),
           int len) _Checked {
  f2(ip, fp, len);  // expected-error {{expected a type argument list for a bounds-safe interface call in a checked scope}}
  f2(fp, ip, len);  // expected-error {{expected a type argument list for a bounds-safe interface call in a checked scope}}
  _Ptr<double> p = alloc2(sizeof(double)); // expected-error {{expected a type argument list for a bounds-safe interface call in a checked scope}}
  ip = alloc2(len);                        // expected-error {{expected a type argument list for a bounds-safe interface call in a checked scope}}
}



//
// Checked bounds_only scopes
//

void test51(_Array_ptr<int> ip : byte_count(len), 
           _Array_ptr<float> fp : byte_count(len),
              int len) _Checked _Bounds_only {
  f2<float, int>(fp, ip, len);
  f2<int, float>(ip, fp, len);
  _Ptr<double> p = alloc2<int, double>(sizeof(double));
  ip = alloc2<double, int>(len);  // TODO: Github issue #595: expected-warning {{cannot prove declared bounds}}
}


void test52(_Array_ptr<int> ip : byte_count(len), 
            _Array_ptr<float> fp : byte_count(len),
              int len) _Checked _Bounds_only {
  f2<float, int>(ip, ip, len);  // expected-error {{incompatible type}}
  f2<int, float>(fp, fp, len);  // expected-error {{incompatible type}}
  _Ptr<double> p = alloc2<int, int>(sizeof(double)); // expected-error {{incompatible type}}
  ip = alloc2<int, double>(len);                     // expected-error {{incompatible type}}
}

void test53(_Array_ptr<int> ip : byte_count(len), _Array_ptr<float> fp : byte_count(len),
           int len) _Checked  _Bounds_only{
  f2<void, void>(ip, ip, len);
  f2<void, void>(ip, fp, len); 
  f2<void, void>(fp, fp, len);
  f2<void, void>(fp, ip, len);
  _Ptr<double> p = alloc2<void, void>(sizeof(double));
  ip = alloc2<void, void>(len);  // TODO: Github issue #595: expected-warning {{cannot prove declared bounds}}
}

void test54(_Array_ptr<int> ip : byte_count(len), _Array_ptr<float> fp : byte_count(len),
           int len) _Checked  _Bounds_only {
  f2(ip, fp, len); // expected-error {{expected a type argument list for a bounds-safe interface call in a checked scope}}
  f2(fp, ip, len); // expected-error {{expected a type argument list for a bounds-safe interface call in a checked scope}}
  _Ptr<double> p = alloc2(sizeof(double)); // expected-error {{expected a type argument list for a bounds-safe interface call in a checked scope}}
  ip = alloc2(len);  // expected-error {{expected a type argument list for a bounds-safe interface call in a checked scope}}
}

//
// Unchecked scopes
//

void test61(_Array_ptr<int> ip : byte_count(len), 
           _Array_ptr<float> fp : byte_count(len),
              int len) _Unchecked {
  f2<float, int>(fp, ip, len);
  f2<int, float>(ip, fp, len);
  _Ptr<double> p = alloc<double>(sizeof(double));
  ip = alloc<int>(len);  // TODO: Github issue #595: expected-warning {{cannot prove declared bounds}}
}


void test62(_Array_ptr<int> ip : byte_count(len), 
            _Array_ptr<float> fp : byte_count(len),
              int len) _Unchecked {
  f2<float, int>(ip, ip, len);  // expected-error {{incompatible type}}
  f2<int, float>(fp, fp, len);  // expected-error {{incompatible type}}
  _Ptr<double> p = alloc2<int, int>(sizeof(double)); // TODO: Github issue #587: this should be a type error.
  ip = alloc2<int, double>(len);  // TODO: Github issue #595: expected-warning {{cannot prove declared bounds}}
}

void test63(_Array_ptr<int> ip : byte_count(len), _Array_ptr<float> fp : byte_count(len),
           int len) _Unchecked {
  f2<void, void>(ip, ip, len);
  f2<void, void>(ip, fp, len); 
  f2<void, void>(fp, fp, len);
  f2<void, void>(fp, ip, len);
  _Ptr<double> p = alloc2<void, void>(sizeof(double));
  ip = alloc2<void, void>(len);  // TODO: Github issue #595: expected-warning {{cannot prove declared bounds}}
}

void test64(_Array_ptr<int> ip : byte_count(len), _Array_ptr<float> fp : byte_count(len),
           int len) _Unchecked {
  f2(ip, fp, len);
  f2(fp, ip, len);
  _Ptr<double> p = alloc2<void, void>(sizeof(double));
  ip = alloc2<void, void>(len);  // TODO: Github issue #595: expected-warning {{cannot prove declared bounds}}
}

//
// Unchecked scopes by default
//


void test71(_Array_ptr<int> ip : byte_count(len), 
           _Array_ptr<float> fp : byte_count(len),
              int len) {
  f2<float, int>(fp, ip, len);
  f2<int, float>(ip, fp, len);
  _Ptr<double> p = alloc<double>(sizeof(double));
  ip = alloc<int>(len);  // TODO: Github issue #595: expected-warning {{cannot prove declared bounds}}
}


void test72(_Array_ptr<int> ip : byte_count(len), 
            _Array_ptr<float> fp : byte_count(len),
              int len) {
  f2<float, int>(ip, ip, len);  // expected-error {{incompatible type}}
  f2<int, float>(fp, fp, len);  // expected-error {{incompatible type}}
  _Ptr<double> p = alloc2<int, int>(sizeof(double)); // TODO: Github issue #587: this should be a type error.
  ip = alloc2<int, double>(len);                     // TODO: Github issue #587: this should be a type error. Github issue #595: expected-warning {{cannot prove declared bounds}}
}

void test73(_Array_ptr<int> ip : byte_count(len), _Array_ptr<float> fp : byte_count(len),
           int len) {
  f2<void, void>(ip, ip, len);
  f2<void, void>(ip, fp, len); 
  f2<void, void>(fp, fp, len);
  f2<void, void>(fp, ip, len);
  _Ptr<double> p = alloc2<void, void>(sizeof(double));
  ip = alloc2<void, void>(len);  // TODO: Github issue #595: expected-warning {{cannot prove declared bounds}}
}

void test74(_Array_ptr<int> ip : byte_count(len), _Array_ptr<float> fp : byte_count(len),
           int len) {
  f2(ip, fp, len);
  f2(fp, ip, len);
  _Ptr<double> p = alloc2<void, void>(sizeof(double));
  ip = alloc2<void, void>(len);  // TODO: Github issue #595: expected-warning {{cannot prove declared bounds}}
}
