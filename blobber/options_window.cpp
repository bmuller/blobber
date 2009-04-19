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

#include <gtkmm/stock.h>

#include "options_window.h"
#include "mod_interface.h"
#include "application.h"

namespace blobber {
  using namespace std;

  OptionsWindow::OptionsWindow(Camarea *cam) : 
    area(cam), 
    okButton(Gtk::Stock::OK),
    brightnessScale(0, 100, 1.0),
    contrastScale(0, 100, 1.0),
    saturationScale(0, 100, 1.0),
    exposureScale(0, 100, 1.0),
    exposureAuto("Automatic Exposure Control"),
    poiCriteriaWindow(1, 100, 1.0),
    lblCamDevice("Camera devices: "),
    modsFrame("Available Modules"),
    brightnessFrame("Brightness"),
    contrastFrame("Contrast"),
    saturationFrame("Saturation"),
    exposureFrame("Exposure"),
    poiCriteriaWindowFrame("Laser Color Window")
  { 

    set_title("Blobber Configuration");
    set_resizable(false);

    okButton.signal_clicked().connect(sigc::mem_fun(*this, &OptionsWindow::ok));

    config = Configuration::get_config();
    vector<string> preLoadedMods;
    config->get("mods_enabled", preLoadedMods);

    ModInterface::get_available_modules(availableModules, modFiles);

    // Get required settings from the config file
    string slidePosition;
    string isAuto;
    config->get_set("brightness", slidePosition, "80");
    brightnessScale.set_value(string_to_int(slidePosition));
    config->get_set("contrast", slidePosition, "75");
    contrastScale.set_value(string_to_int(slidePosition));
    config->get_set("saturation", slidePosition, "75");
    saturationScale.set_value(string_to_int(slidePosition));
    config->get_set("exposure", slidePosition, "5");
    exposureScale.set_value(string_to_int(slidePosition));
    config->get_set("autoExposure", isAuto, "0");
    if (string_to_int(isAuto)) exposureAuto.set_active();
    config->get_set("default_criteria_window", slidePosition, DEFAULT_CRITERIA_WINDOW_SIZE);
    poiCriteriaWindow.set_value(string_to_int(slidePosition));
    // after loading settings, need to update camera
    brightness_changed();
    contrast_changed();

    add(mainBox);
    mainBox.pack_start(modsFrame, Gtk::PACK_SHRINK);
    camDevice.pack_end(okButton, Gtk::PACK_SHRINK);
    mainBox.pack_end(camDevice, Gtk::PACK_SHRINK);
    mainBox.pack_end(brightnessFrame, Gtk::PACK_SHRINK);
    brightnessFrame.add(brightnessScale);
    mainBox.pack_end(contrastFrame, Gtk::PACK_SHRINK);
    contrastFrame.add(contrastScale);
    mainBox.pack_end(saturationFrame, Gtk::PACK_SHRINK);
    saturationFrame.add(saturationScale);
    mainBox.pack_end(exposureFrame, Gtk::PACK_SHRINK);
    exposureFrame.add(exposureScale);
    mainBox.pack_end(exposureAuto);
    mainBox.pack_end(poiCriteriaWindowFrame, Gtk::PACK_SHRINK);
    poiCriteriaWindowFrame.add(poiCriteriaWindow);
    camDevice.pack_start(lblCamDevice, Gtk::PACK_SHRINK);
    modsFrame.add(modsBox);

    brightnessScale.signal_value_changed().connect(sigc::mem_fun(*this, &OptionsWindow::brightness_changed));
    contrastScale.signal_value_changed().connect(sigc::mem_fun(*this, &OptionsWindow::contrast_changed));
    saturationScale.signal_value_changed().connect(sigc::mem_fun(*this, &OptionsWindow::saturation_changed));
    exposureScale.signal_value_changed().connect(sigc::mem_fun(*this, &OptionsWindow::exposure_changed));
    exposureAuto.signal_toggled().connect(sigc::mem_fun(*this, &OptionsWindow::exposure_changed));

    for(std::map<string, string>::iterator it=availableModules.begin(); it!=availableModules.end(); it++) {
      string modname = it->first;
      string description = it->second;
      modButtons[modname] = Gtk::manage(new Gtk::CheckButton(modname + ": " + description));
      for (vector<string>::iterator itt = preLoadedMods.begin(); itt < preLoadedMods.end(); itt++) {
	      if (*itt == modFiles[modname]) {
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
    string device;
    config->get("device", device, DEFAULT_DEVICE);
    cboCamDevice.set_active_text(device);
    camDevice.pack_start(cboCamDevice, Gtk::PACK_SHRINK);
    show_all_children();
  };

  void OptionsWindow::contrast_changed() {
    int contrast = (int) contrastScale.get_value();
    area->set_contrast(contrast);
  };

  void OptionsWindow::brightness_changed() {
    int brightness = (int) brightnessScale.get_value();
    area->set_brightness(brightness);
  };
  
  void OptionsWindow::saturation_changed() {
    area->set_saturation(saturationScale.get_value());
  }

  void OptionsWindow::exposure_changed() {
    area->set_exposure(exposureScale.get_value(), exposureAuto.get_active());
  }

  void OptionsWindow::ok() {
    string device = cboCamDevice.get_entry()->get_text().raw();
    if (device == "") config->get("device", device, DEFAULT_DEVICE);
    
    vector<string> push;
    push.clear();
    for(std::map<string, Gtk::CheckButton*>::iterator it = modButtons.begin(); it != modButtons.end(); it++) {
      if (it->second->get_active()) 
	push.push_back(modFiles[it->first]);
    }
    // set config options then save config file
    config->set("mods_enabled", push);
    config->set("device", device);
    string slider;
    string isAuto;
    num_to_string(brightnessScale.get_value(), slider);
    config->set("brightness", slider);
    num_to_string(contrastScale.get_value(), slider);
    config->set("contrast", slider);
    num_to_string(poiCriteriaWindow.get_value(), slider);
    config->set("default_criteria_window", slider);
    num_to_string(saturationScale.get_value(), slider);
    config->set("saturation", slider);
    num_to_string(exposureScale.get_value(), slider);
    config->set("exposure", slider);
    num_to_string(exposureAuto.get_active(), isAuto);
    config->set("autoExposure", slider);

    // reload config
    Application::get_app()->reload_config();
    hide();
  };

};
