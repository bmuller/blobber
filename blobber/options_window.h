#ifndef __OPTIONS_WINDOW_H
#define __OPTIONS_WINDOW_H

#include <gtkmm/window.h>
#include <gtkmm/label.h>
#include <gtkmm/box.h>
#include <gtkmm/scale.h>
#include <gtkmm/entry.h>
#include <gtkmm/comboboxentrytext.h>
#include <gtkmm/frame.h>

#include "camarea.h"
#include "configuration.h"

namespace blobber {
  using namespace std;

  class OptionsWindow : public Gtk::Window {
  public:	
    OptionsWindow(Camarea *cam);
    void ok();
  protected:
    Gtk::Label lblCamDevice, lblSavedImagesDir;
    Gtk::VBox mainBox;
    Gtk::HBox camDevice, savedImagesDirBox;
    Gtk::Entry savedImagesDir;
    Gtk::ComboBoxEntryText cboCamDevice;
    Gtk::Button okButton;
    Gtk::Frame modsFrame;
    Gtk::Frame brightnessFrame;
    Gtk::Frame contrastFrame;
    Gtk::Frame saturationFrame;
    Gtk::Frame exposureFrame;
    Gtk::Frame poiCriteriaWindowFrame;
    Gtk::VBox modsBox;
    Gtk::HScale brightnessScale;
    Gtk::HScale contrastScale;
    Gtk::HScale saturationScale;
    Gtk::HScale exposureScale;
    Gtk::CheckButton exposureAuto;
    // this is for the default_criteria_window config option
    Gtk::HScale poiCriteriaWindow;
    Camarea *area;
    // module description by module name
    std::map<string, string> availableModules;
    // module check button pointer by module name
    std::map<string, Gtk::CheckButton*> modButtons;
    // module file name by module name;
    std::map<string, string> modFiles;
    Configuration *config;
    void brightness_changed();
    void contrast_changed();
    void saturation_changed();
    void exposure_changed();
  };
};

#endif /* __OPTIONS_WINDOW_H */

