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
  using namespace std;

  OptionsWindow::OptionsWindow(Camarea *cam) : 
	area(cam), 
	okButton(Gtk::Stock::OK),
	lblCamDevice("Camera devices: "),
	modsFrame("Available Modules"){ 

    set_title("Blobber Configuration");
    set_resizable(false);

    okButton.signal_clicked().connect(sigc::mem_fun(*this, &OptionsWindow::ok));

    config = Configuration::get_config();
    vector<string> preLoadedMods;
    config->get("mods_enabled", preLoadedMods);

    ModInterface::get_available_modules(availableModules, modFiles);

    add(mainBox);
    mainBox.pack_start(modsFrame, Gtk::PACK_SHRINK);
    camDevice.pack_end(okButton, Gtk::PACK_SHRINK);
    mainBox.pack_end(camDevice, Gtk::PACK_SHRINK);
    camDevice.pack_start(lblCamDevice, Gtk::PACK_SHRINK);
    modsFrame.add(modsBox);

    for(std::map<string, string>::iterator it=availableModules.begin(); it!=availableModules.end(); it++) {
      string modname = it->first;
      string description = it->second;
      modButtons[modname] = Gtk::manage(new Gtk::CheckButton(modname + ": " + description));
      for (vector<string>::iterator itt = preLoadedMods.begin(); itt < preLoadedMods.end(); itt++)
      {
	      if (*itt == modFiles[modname])
	      {
		      modButtons[modname]->set_active();
		      break;
	      }
      }
      modsBox.add(*modButtons[modname]);
    }

    string devDir = "/dev"; 
    Glib::Dir listDev(devDir);
    vector<string> files(listDev.begin(), listDev.end());
    for(unsigned int i=0; i<files.size(); i++) {
      if(files[i].substr(0,5).compare("video") == 0)
        cboCamDevice.append_text(Glib::build_filename(devDir, files[i]));
    }

    camDevice.pack_start(cboCamDevice, Gtk::PACK_SHRINK);
    show_all_children();
  };

  void OptionsWindow::ok() {
    string device = cboCamDevice.get_entry()->get_text().raw();
    if (device == "") config->get("device", device, DEFAULT_DEVICE);
    
    vector<string> push;
    push.clear();
    for(std::map<string, Gtk::CheckButton*>::iterator it = modButtons.begin(); it != modButtons.end(); it++)
    {
	    if (it->second->get_active()) push.push_back(modFiles[it->first]);
    }
    // set config options then save config file
    config->set("mods_enabled", push);
    config->set("device", device);

    // reload config
    Application::get_app()->reload_config();
    hide();
  };

};
