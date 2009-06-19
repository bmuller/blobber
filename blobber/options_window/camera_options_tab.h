#ifndef __CAMERA_OPTIONS_TAB_H
#define __CAMERA_OPTIONS_TAB_H

#include <gtkmm/label.h>
#include <gtkmm/box.h>
#include <gtkmm/scale.h>
#include <gtkmm/comboboxentrytext.h>
#include <gtkmm/frame.h>

#include "camarea.h"
#include "configuration.h"

namespace blobber {
  using namespace std;
  
  class CameraOptionsTab : public Gtk::VBox {
  public:
    CameraOptionsTab(Camarea *cam);
    void save();
  protected:
    Camarea *area;
    Configuration *config;
    Gtk::Label description;
    Gtk::Frame brightnessFrame, contrastFrame, saturationFrame, exposureFrame;    
    Gtk::ComboBoxEntryText cboCamDevice;
    Gtk::HBox camDevice;
    Gtk::Label lblCamDevice;
    Gtk::HScale brightnessScale, contrastScale, saturationScale, exposureScale;
    Gtk::CheckButton exposureAuto;
    void brightness_changed();
    void contrast_changed();
    void saturation_changed();
    void exposure_changed();
    void load_settings();
  };
};

#endif /* __CAMERA_OPTIONS_TAB_H */

