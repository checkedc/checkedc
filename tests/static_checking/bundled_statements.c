// Feature tests for bundled statements
//
// The following line is for the LLVM test harness:
//
// RUN: %clang_cc1 -verify -verify-ignore-unexpected=note  %s


void f1()
_Checked{
  _Array_ptr<int> p : count(2) = 0;
  int val = 5;
  int val1 _Checked[3];
  _Array_ptr<int> q : count(1) = &val;
  _Bundled {
    p = q;
    *(p+1) = 4; // expected-error {{out-of-bounds memory access}} 
    p = val1;
  }
  p = q,
  *(p+1) = 4,   // expected-error {{out-of-bounds memory access}} 
  p = val1;
}

void f2(int flag)
_Checked{
  _Array_ptr<int> p : count(2) = 0;
  int val = 5;
  int val1 _Checked[3];
  _Array_ptr<int> q : count(1) = &val;
  _Bundled {
    p = val1;
    p++;
    p++;      // expected-warning {{cannot prove declared bounds for 'p' are valid after increment}}
    *(p+4) = 4; 
  }
  p = val1,
  p++,
  p++,        // expected-warning {{cannot prove declared bounds for 'p' are valid after increment}}
  *(p+4) = 4; 
}

void f3()
_Checked{
  _Array_ptr<int> p : count(2) = 0;
  int val = 5;
  int val1 _Checked[3];
  _Array_ptr<int> q : count(1) = &val;
  _Bundled {
    p = q,
    *(p+1) = 4, // expected-error {{out-of-bounds memory access}}
    p = val1;
  }
  _Bundled {
    p = q,
    *(p+1) = 4, // expected-error {{out-of-bounds memory access}}
    p = val1;
  }
  _Bundled {}
}

void f4(int flag)
_Checked{
  _Array_ptr<int> p : count(2) = 0;
  int val = 5;
  int val1 _Checked[8];
  _Array_ptr<int> q : count(1) = &val;
  _Bundled {
    p = val1;
    p++;
    p++;       // expected-warning {{cannot prove declared bounds for 'p' are valid after increment}}
    *(p+4) = 4; 
  }
  p = val1,
  p++,
  p++,         // expected-warning {{cannot prove declared bounds for 'p' are valid after increment}}
  *(p+4) = 4; 
}

void f5(int flag)
_Checked{
  _Array_ptr<int> p : count(2) = 0;
  int val = 5;
  int val1 _Checked[3];
  _Array_ptr<int> q : count(1) = &val;
  _Bundled {
    p = flag ? q : val1;
    *(p+1) = 4;   // expected-error {{expression has unknown bounds}}
    p = val1;
  }
  p = flag ? q : val1,
  *(p+1) = 4,     // expected-error {{expression has unknown bounds}}
  p = val1;
}

void f6(int flag)
_Checked{
  _Array_ptr<int> p : count(2) = 0;
  int val = 5;
  int val1 _Checked[3];
  _Array_ptr<int> q : count(1) = &val;
  _Bundled {
    p = val1;
    p++;
    p++;
    *(p+1) = 4; 
    p = flag ? q : val1;  // expected-error {{inferred bounds for 'p' are unknown after assignment}}
  }
  p = val1,
  p++,
  p++,
  *(p+1) = 4, 
  p = flag ? q : val1;    // expected-error {{inferred bounds for 'p' are unknown after assignment}}
}

void f7(int flag)
_Checked{
  _Array_ptr<int> p : count(2) = 0;
  int val = 5;
  int val1 _Checked[3];
  _Array_ptr<int> q : count(1) = &val;
  _Bundled {
    p = val1;
    p++;
    p++;
    p = flag ? q : val1; // expected-error {{inferred bounds for 'p' are unknown after assignment}}
    *(p+1) = 4;          // expected-error {{expression has unknown bounds}}
  }
  p = val1,
  p++,
  p++,
  p = flag ? q : val1,   // expected-error {{inferred bounds for 'p' are unknown after assignment}}
  *(p+1) = 4;            // expected-error {{expression has unknown bounds}}
}

void f8(int flag)
_Checked{
  _Bundled {
  _Array_ptr<int> p : count(2) = 0;
  int val = 5;
  int val1 _Checked[3];
  _Array_ptr<int> q : count(1) = &val;
    p = val1;
    p++;
    p++;
    *(p+1) = 4;
    p = flag ? q : val1; // expected-error {{inferred bounds for 'p' are unknown after assignment}}
  }
}


void f9(int flag)
_Checked{
  _Array_ptr<int> p : count(2) = 0;
  int val = 5;
  int val1 _Checked[3];
  _Array_ptr<int> q : count(1) = &val;
    p = val1,
    p++,
    p++,
    *(p+1) = 4,
    p = flag ? q : val1; // expected-error {{inferred bounds for 'p' are unknown after assignment}}
}

void f10(int flag)
_Checked{
  int val = 5;
  _Array_ptr<int> q : count(2) = &val; // expected-error {{declared bounds for 'q' are invalid after initialization}}
  _Bundled {
  _Array_ptr<int> r : count(2) = &val; // expected-error {{declared bounds for 'r' are invalid after initialization}}
  }
}

void f11(int flag)
_Checked{
  int val = 5;
  int valarr _Checked[4];
  _Array_ptr<int> q : count(2) = &val; // expected-error {{declared bounds for 'q' are invalid after initialization}}
  _Bundled {
  _Array_ptr<int> r : count(2) = &val;
  r = valarr;
  }
}

extern _Array_ptr<int> my_malloc(int len) : count(len);
extern void copy1(_Array_ptr<int> to, _Array_ptr<int> from : count(n), int n);
extern _Array_ptr<int> copy2(_Array_ptr<int> to, _Array_ptr<int> from : count(n), int n, int new_n) : count(new_n);


_Array_ptr<int> resize1(_Array_ptr<int> buf : count(buflen), int buflen, int len) : count(len) {
  _Array_ptr<int> tmp : count(len) = my_malloc(len);
  copy1(tmp, buf, buflen);
  _Bundled {
    buflen = len;
    buf = tmp;
  }
  return buf;
}

int buflen = 0;
_Array_ptr<int> buf : count(buflen) = 0;

void resize2(int newlen) {
  _Array_ptr<int> tmp : count(newlen) = my_malloc(newlen);
  _Bundled {
    buf = copy2(tmp, buf, buflen, newlen);
    buflen = newlen;
  }
}

int gtmp;
int *g1 = &gtmp;
_Ptr<int> g2 = &gtmp;
_Array_ptr<int> g3 = &gtmp;
_Array_ptr<int> g4 : count(1) = &gtmp;

int *h1 = &gtmp;
_Ptr<int> h2 = &gtmp;
_Array_ptr<int> h3 = &gtmp;
_Array_ptr<int> h4 : count(1) = &gtmp;


void gf0(void) {
  _Bundled {
    g1 = g3;  // expected-error {{assigning to 'int *' from incompatible type '_Array_ptr<int>'}}
    g2 = g3;  // expected-error {{expression has unknown bounds, cast to ptr<T> expects source to have bounds}}
    g4 = g3;  // expected-error {{inferred bounds for 'g4' are unknown after assignment}}
  }

  g1 = g3,    // expected-error {{assigning to 'int *' from incompatible type '_Array_ptr<int>'}}
  g2 = g3,
  g4 = g3;
}

void gf1(void) {
  _Bundled {
    g2 = g3;  // expected-error {{expression has unknown bounds, cast to ptr<T> expects source to have bounds}}
    g4 = g3;  // expected-error {{inferred bounds for 'g4' are unknown after assignment}}
  }

  g2 = g3,    // expected-error {{expression has unknown bounds, cast to ptr<T> expects source to have bounds}}
  g4 = g3;    // expected-error {{inferred bounds for 'g4' are unknown after assignment}}
}

void gf2(void) {
  _Bundled {
    g4 = g1;
    g4 = g2;
    g4 = g3;  // expected-error {{inferred bounds for 'g4' are unknown after assignment}}
  }

  g4 = g1,
  g4 = g2,
  g4 = g3;    // expected-error {{inferred bounds for 'g4' are unknown after assignment}}
}

void gf3(void) {
  _Bundled {
    g1 = g3;  // expected-error {{assigning to 'int *' from incompatible type '_Array_ptr<int>'}}
    g2 = g3;  // expected-error {{expression has unknown bounds, cast to ptr<T> expects source to have bounds}}
    g4 = g3;
    g1 = h1;
    g2 = h2;
    g4 = h4;
  }

  g1 = g3,    // expected-error {{assigning to 'int *' from incompatible type '_Array_ptr<int>'}}
  g2 = g3,
  g4 = g3,
  g1 = h1,
  g2 = h2,
  g4 = h4;
}

void gf4(void) {
  _Bundled {
    g4 = g1;
    g4 = g2;
    g4 = g3;
    g4 = h4;
  }

  g4 = g1,
  g4 = g2,
  g4 = g3,
  g4 = h4;
}
























































