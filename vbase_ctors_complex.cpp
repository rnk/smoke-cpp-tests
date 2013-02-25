#include "common.h"

struct BaseLeft { BaseLeft(); };
struct BaseRight { BaseRight(); };

struct Middle : BaseLeft, BaseRight {
  Middle();
};

struct Child : virtual Middle {
  Child();
};

extern int base_left_ctor_calls,
           base_right_ctor_calls,
           middle_ctor_calls,
           child_ctor_calls;

#ifdef CONFIG_1
int base_left_ctor_calls,
    base_right_ctor_calls,
    middle_ctor_calls,
    child_ctor_calls;

Child::Child() {
  CHECK_EQ(1, base_left_ctor_calls);
  CHECK_EQ(1, base_right_ctor_calls);
  CHECK_EQ(1, middle_ctor_calls);
  CHECK_EQ(0, child_ctor_calls);
  child_ctor_calls++;
}

#else
BaseLeft::BaseLeft() {
  CHECK_EQ(0, base_left_ctor_calls);
  CHECK_EQ(0, base_right_ctor_calls);
  CHECK_EQ(0, middle_ctor_calls);
  CHECK_EQ(0, child_ctor_calls);
  base_left_ctor_calls++;
}

BaseRight::BaseRight() {
  CHECK_EQ(1, base_left_ctor_calls);
  CHECK_EQ(0, base_right_ctor_calls);
  CHECK_EQ(0, middle_ctor_calls);
  CHECK_EQ(0, child_ctor_calls);
  base_right_ctor_calls++;
}

Middle::Middle() {
  CHECK_EQ(1, base_left_ctor_calls);
  CHECK_EQ(1, base_right_ctor_calls);
  CHECK_EQ(0, middle_ctor_calls);
  CHECK_EQ(0, child_ctor_calls);
  middle_ctor_calls++;
}

int main() {
  void *old_esp = get_esp();
  Child c;
  CHECK_EQ(old_esp, get_esp());

  CHECK_EQ(1, base_left_ctor_calls);
  CHECK_EQ(1, base_right_ctor_calls);
  CHECK_EQ(1, middle_ctor_calls);
  CHECK_EQ(1, child_ctor_calls);
}
#endif
