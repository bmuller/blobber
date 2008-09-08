#include <gtkmm.h>
#include <glib.h>
#include <iostream>
#include "test.h"

using namespace std;

int main() {
  Glib::Module module("./test");
  if(module) {
    cout << "found module" << endl;
    //void* test = 0;
    TestParent* (*get_plugin) () ;
    bool found = module.get_symbol("get_module", (void *&) get_plugin);
    if(found) {
      TestParent *t = get_plugin();  
      t->say_hi();
    } else {
      cout << "symbol not found\n";
    }

  } else {
    cout << Glib::Module::get_last_error() << endl;
  }
  return 0;
}
