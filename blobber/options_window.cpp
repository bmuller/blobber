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

  OptionsWindow::OptionsWindow(Camarea *cam) : area(cam), okButton("OK"), lblCamDevice("Camera devices: "),
	modsFrame("Available Modules"){ 
    resize(300, 300);
    set_title("Configuration");
    okButton.signal_clicked().connect(sigc::mem_fun(*this, &OptionsWindow::ok));

    refreshAvailableModules();

    add(mainBox);
    mainBox.add(modsFrame);
    mainBox.add(camDevice);
    mainBox.add(okButton);
    camDevice.add(lblCamDevice);
    modsFrame.add(modsBox);

    for (int i = 0; i < availableModules.size(); i++)
    {
	    Gtk::Button* modCheck = Gtk::manage(new Gtk::CheckButton(availableModules[i]));
	    modsBox.add(*modCheck);
    }

    string devDir = "/dev"; 
    Glib::Dir listDev(devDir);
    vector<string> files(listDev.begin(), listDev.end());
    for(unsigned int i=0; i<files.size(); i++) {
      if(files[i].substr(0,5).compare("video") == 0)
        cboCamDevice.append_text(Glib::build_filename(devDir, files[i]));
    }

    camDevice.add(cboCamDevice);
  //  mainBox.add(modules);
    show_all_children();
  };

  void OptionsWindow::ok() {
    area->set_device(cboCamDevice.get_entry()->get_text().raw());
    hide();
  };

  void OptionsWindow::refreshAvailableModules()
  {
    ModInterface::get_available_modules(availableModules);

    // remove extra characters from names
    for (int i = 0; i < availableModules.size(); i++)
    {
	    availableModules[i].erase(0, 3);
	    availableModules[i].erase(availableModules[i].find_first_of("."));
    }

    // remove duplicate listings
    for (int i = 0; i < availableModules.size(); i++)
	    for (int j = i + 1; j < availableModules.size(); j++)
	    {
		    vector<string>::iterator buffer = availableModules.begin() + j;
		    if (availableModules[i] == availableModules[j])
		    {
			    availableModules.erase(buffer);
			    j--;
		    }
	    }
    
    // alphebatize list
    for (int i = 0; i < availableModules.size(); i++)
	    for (int j = i + 1; j < availableModules.size(); j++)
	    {
		    string buffer;
		    if (availableModules[j] < availableModules[i])
		    {
			    buffer = availableModules[i];
			    availableModules[i] = availableModules[j];
			    availableModules[j] = buffer;
		    }
	    }
  }
};
