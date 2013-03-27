#include "common.h"

// TODO: need vbase support, but memptr representation in MS ABI is:
// {offset in class, offset in vbtable}

struct A {
  int a;
};

struct B : virtual A {
  int b;
};

struct C : virtual B {
  int c;
};

struct D : C {
  int d;
};

void foo(C *bp, int B::*memptr);

#ifdef CONFIG_1

void foo(C *bp, int B::*memptr) {
  bp->*memptr = 10;
}

#else
int main() {
  D c;
  foo(&c, &B::b);
  CHECK_EQ(10, c.b);
}
#endif