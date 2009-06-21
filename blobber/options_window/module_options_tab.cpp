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

#include "module_options_tab.h"
#include "mod_interface.h"

namespace blobber {
  using namespace std;

  ModulesOptionsTab::ModulesOptionsTab() : modsFrame("Available Modules") {
    description.set_text("The modules below are available for blobber to use.  "
			 "Any number of modules can be enabled at the same time.");
    description.set_line_wrap(true);
    description.set_line_wrap_mode(Pango::WRAP_WORD);
    set_spacing(10);
    pack_start(description, Gtk::PACK_SHRINK, 20);
    pack_start(modsFrame, Gtk::PACK_SHRINK);    
    modsFrame.add(modsBox);

    config = Configuration::get_config();
    vector<string> preLoadedMods;
    if(!config->is_set("mods_enabled")) {
      preLoadedMods.push_back("lasertag");
      preLoadedMods.push_back("projectionoptions");
      config->set("mods_enabled", preLoadedMods);
    } else {
      config->get("mods_enabled", preLoadedMods);
    }

    ModInterface::get_available_modules(availableModules, modFiles);

    for(std::map<string, string>::iterator it=availableModules.begin(); it!=availableModules.end(); it++) {
      string modname = it->first;
      string description = it->second;
      modButtons[modname] = Gtk::manage(new Gtk::CheckButton(modname + ": " + description));
      // if module is active, make box checked
      if(includes(preLoadedMods, modFiles[modname]))
      	modButtons[modname]->set_active();
	modsBox.add(*modButtons[modname]);
    }
  };

  void ModulesOptionsTab::save() {
    vector<string> push;
    for(std::map<string, Gtk::CheckButton*>::iterator it = modButtons.begin(); it != modButtons.end(); it++) {
      if (it->second->get_active()) 
	push.push_back(modFiles[it->first]);
    }
    // set config options then save config file
    config->set("mods_enabled", push);
  };
};
