#include "blobber.h"

ModInterface::ModInterface(string n) : name(n) { 
  debug("Module \"" + n + "\" just created"); 
};

void ModInterface::register_poi_criteria(Camarea &area, CRANGE crange) {
  area.register_poi_criteria(name, crange);
};

void ModInterface::get_poi(Camarea &area, vector<PIXEL> &modpoi) {
  area.get_poi(name, modpoi);
};

ModInterface * ModInterface::load_module(string modname) {
  modname = "lib" + modname;
  debug("looking in " + string(LIBDIR) + " for module " + modname);
  Glib::Module module(string(LIBDIR) + '/' + modname);  
  if(module) {
    debug("sucessfully loaded module " + modname);
    ModInterface* (*get_module) () ;
    bool found = module.get_symbol("get_module", (void *&) get_module);
    if(found) {
      module.make_resident();
      return get_module();  
    } else {
      throw ModuleLoadException("get_module function not found in module " + modname);
    }
  } else {
    throw ModuleLoadException(string(Glib::Module::get_last_error()));
  }
};
