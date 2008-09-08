#include <iostream>
#include "test.h"

using namespace std;

class Test : public TestParent {
  void say_hi() {
    cout << "hello!" << endl;
  };
};

extern "C" {
  Test *get_module() { return new Test(); };
};
