#include <gtkmm.h>
#include <glib.h>
#include <iostream>
#include "test.h"

using namespace std;

TestParent * getit() {
  Glib::Module module("./test");
  if(module) {
    cout << "found module" << endl;
    TestParent* (*get_plugin) () ;
    bool found = module.get_symbol("get_module", (void *&) get_plugin);
    if(found) {
      module.make_resident();
      return get_plugin();  
    } else {
      cout << "symbol not found\n";
    }
  } else {
    cout << Glib::Module::get_last_error() << endl;
  }
  return 0;
}


int main() {
  TestParent *t = getit();
  if(t)
    t->say_hi();
  delete t;
};


