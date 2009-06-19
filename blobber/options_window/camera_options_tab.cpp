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


#include "camera_options_tab.h"

namespace blobber {
  using namespace std;

  CameraOptionsTab::CameraOptionsTab(Camarea *cam) : 
    area(cam), 
    brightnessScale(0, 100, 1.0),
    contrastScale(0, 100, 1.0),
    saturationScale(0, 100, 1.0),
    exposureScale(0, 100, 1.0),
    lblCamDevice("Camera devices: "),
    exposureAuto("Automatic Exposure Control"),
    brightnessFrame("Brightness"),
    contrastFrame("Contrast"),
    saturationFrame("Saturation"),
    exposureFrame("Exposure")
  {
    description.set_text("The options below affect the camera settings.  "
			 "See the help documentation for more information on these settings "
			 "and the effects they have.");
    description.set_line_wrap(true);
    description.set_line_wrap_mode(Pango::WRAP_WORD);
    set_spacing(10);
    pack_start(description, Gtk::PACK_SHRINK, 20);

    config = Configuration::get_config();
    load_settings();

    pack_start(brightnessFrame, Gtk::PACK_SHRINK);
    brightnessFrame.add(brightnessScale);
    pack_start(contrastFrame, Gtk::PACK_SHRINK);
    contrastFrame.add(contrastScale);
    pack_start(saturationFrame, Gtk::PACK_SHRINK);
    saturationFrame.add(saturationScale);
    pack_start(exposureFrame, Gtk::PACK_SHRINK);
    exposureFrame.add(exposureScale);
    pack_start(exposureAuto);


    pack_start(camDevice, Gtk::PACK_SHRINK);
    camDevice.pack_start(lblCamDevice, Gtk::PACK_SHRINK);
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

    brightnessScale.signal_value_changed().connect(sigc::mem_fun(*this, &CameraOptionsTab::brightness_changed));
    contrastScale.signal_value_changed().connect(sigc::mem_fun(*this, &CameraOptionsTab::contrast_changed));
    saturationScale.signal_value_changed().connect(sigc::mem_fun(*this, &CameraOptionsTab::saturation_changed));
    exposureScale.signal_value_changed().connect(sigc::mem_fun(*this, &CameraOptionsTab::exposure_changed));
    exposureAuto.signal_toggled().connect(sigc::mem_fun(*this, &CameraOptionsTab::exposure_changed));
  };

  void CameraOptionsTab::load_settings() {
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
    if (string_to_int(isAuto)) 
      exposureAuto.set_active();    

    // after loading settings, need to update camera
    brightness_changed();
    contrast_changed();
    saturation_changed();
    exposure_changed();
  };

  void CameraOptionsTab::contrast_changed() {
    int contrast = (int) contrastScale.get_value();
    area->set_contrast(contrast);
  };

  void CameraOptionsTab::brightness_changed() {
    int brightness = (int) brightnessScale.get_value();
    area->set_brightness(brightness);
  };
  
  void CameraOptionsTab::saturation_changed() {
    area->set_saturation(saturationScale.get_value());
  }

  void CameraOptionsTab::exposure_changed() {
    area->set_exposure(exposureScale.get_value(), exposureAuto.get_active());
  }

  void CameraOptionsTab::save() {
    string slider, isAuto;
    string device = cboCamDevice.get_entry()->get_text().raw();
    if (device == "") config->get("device", device, DEFAULT_DEVICE);
    config->set("device", device);

    num_to_string(brightnessScale.get_value(), slider);
    config->set("brightness", slider);
    num_to_string(contrastScale.get_value(), slider);
    config->set("contrast", slider);
    num_to_string(saturationScale.get_value(), slider);
    config->set("saturation", slider);
    num_to_string(exposureScale.get_value(), slider);
    config->set("exposure", slider);
    num_to_string(exposureAuto.get_active(), isAuto);
    config->set("autoExposure", isAuto);
  };

};
