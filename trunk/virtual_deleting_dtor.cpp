#include "common.h"

extern int base_dtor_calls, derived_dtor_calls;

class Base {
 public:
  virtual ~Base() {
    base_dtor_calls++;
  }
};

#ifdef CONFIG_1
void call_deleting_dtor(Base* obj);

class Derived: public Base {
 public:
  virtual ~Derived() {
    derived_dtor_calls++;
  }
};

void run() {
  call_deleting_dtor(new Derived);
}

#else
int operator_delete_calls,
    base_dtor_calls, derived_dtor_calls;

void operator delete (void* ptr) {
  operator_delete_calls++;
}

void call_deleting_dtor(Base* obj) {
  delete obj;
}

void run();

int main() {
  run();
  CHECK(base_dtor_calls == 1);
  CHECK(derived_dtor_calls == 1);
  CHECK(operator_delete_calls == 1);
}
#endif