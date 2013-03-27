// http://llvm.org/PR13676
#include "common.h"

struct S {
  S() {}
  int a, b;
};

S foo();

#ifdef CONFIG_1
S foo() {
  S ret;
  ret.a = 1;
  ret.b = 2;
  return ret;
}
#else
int main() {
  S s = foo();
  CHECK_EQ(1, s.a);
  CHECK_EQ(2, s.b);
}
#endif
