#include "common.h"

struct A {
  A();
  ~A();
  virtual void c();
  virtual void d();
  int stage_a;
};

struct B : virtual A {
  B();
  ~B();
  // B's thunks use static this adjustment, assuming A's layout follows the 
  // members of B.
  virtual void c();
  virtual void d();
  int stage_b;
};

// In C, the vbases are laid out in the A, B order. Before we enter C() and
// after we leave ~C(), the static this adjustment in B's thunks should be
// further adjusted account for the different order of A and B.
struct C: virtual B {
  C();
  ~C();
};

#ifdef CONFIG_1
A::A() {
  stage_a = 1;
  c();
}

B::B() {
  CHECK_EQ(1, stage_a);
  stage_a = 2;
  stage_b = 1;
  c();
}

// Force ctor and dtor to be compiled in different TUs. 
C::C() {
  CHECK_EQ(2, stage_a);
  CHECK_EQ(1, stage_b);
  c();
  stage_a = 3;
  stage_b = 2;
}

B::~B() {
  CHECK_EQ(4, stage_a);
  CHECK_EQ(3, stage_b);
  d();
  stage_a = 5;
}

A::~A() {
  CHECK_EQ(5, stage_a);
  d();
}

#else

// Force ctor and dtor to be compiled in different TUs. 
C::~C() {
  CHECK_EQ(3, stage_a);
  CHECK_EQ(2, stage_b);
  stage_a = 4;
  stage_b = 3;
  d();
}

void A::c() { CHECK_EQ(1, stage_a); }

void B::c() {
  CHECK_EQ(2, stage_a);
  CHECK_EQ(1, stage_b);
}

void B::d() {
  CHECK_EQ(4, stage_a);
  CHECK_EQ(3, stage_b);
}

void A::d() { CHECK_EQ(5, stage_a); }

int main() {
  C obj;
}
#endif
