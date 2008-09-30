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

namespace blobber {

  ModInterface::ModInterface(string n) : name(n) { 
    debug("Module \"" + n + "\" just created"); 
  };

  void ModInterface::register_poi_criteria(Camarea &area, CRANGE crange, int maxPoi) {
    area.register_poi_criteria(name, crange, maxPoi);
  };

  void ModInterface::get_poi(Camarea &area, vector<PIXEL> &modpoi) {
    area.get_poi(name, modpoi);
  };

  ModInterface * ModInterface::load_module(string modname) {
    modname = "lib" + modname;
    debug("Looking in " + string(LIBDIR) + " for module " + modname);
    Glib::Module module(string(LIBDIR) + '/' + modname);  
    if(module) {
      debug("Sucessfully loaded module " + modname);
      ModInterface* (*get_module) () ;
      bool found = module.get_symbol("get_module", (void *&) get_module);
      if(found) {
	module.make_resident();
	return get_module();  
      } else {
	throw ModuleLoadException(" get_module function not found in module " + modname);
      }
    } else {
      throw ModuleLoadException(string(Glib::Module::get_last_error()));
    }
  };
  
  void ModInterface::get_available_modules(vector<string> &mods) {
    try {
      Glib::Dir dir(LIBDIR);
      mods.clear();
      for (Glib::DirIterator i = dir.begin(); i != dir.end(); i++)
	      if ((*i).find_last_of(".so") == (*i).size() - 3)
		    mods.push_back((*i).substr(3, ((*i).size() - 6)));
    } catch(Glib::FileError fe) {
      throw ModuleListException(" program was compiled with " + string(LIBDIR) + " as library "
				"directory - but not readable.");
    }
  };

  void ModInterface::config_set(string key, string value) {
    config->module_set(key, value, name);
  };

  void ModInterface::config_set(string key, vector<string> values) {
    config->module_set(key, values, name);
  };

  void ModInterface::config_get(string key, string &value, string vdefault) {
    config->module_get(key, value, vdefault, name);
  };

  // this method is useful when you want to get the config value - and in the
  // case that it's not set and you get the default to set it to that value
  void ModInterface::config_get_set(string key, string &value, string vdefault) {
    config_get(key, value, vdefault);
    config_set(key, value);
  };

  void ModInterface::config_get(string key, vector<string> &values) {
    config->module_get(key, values, name);
  };

};
