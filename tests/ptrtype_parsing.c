// Unit tests for parsing new Checked C pointer
// types.
//
// Test that they can be used as parameter types,
// return types, local variable types, and structure
// member types.  Also check the case ptr<ptr<T>>,
// where >> could be mistaken for the right-shift
// operator.
//
// The following line is form the LLVM test
// harness:
// RUN: %clang_cc1 -fcheckedc-extension %s

//
// first parameter is a new pointer type
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
   f1(p, y);
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
   f1(p, y);
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
