/*
    Blobber is webcam based blob tracking software
    Copyright (C) 2008 Charleston, SC Linux Users Group

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

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
